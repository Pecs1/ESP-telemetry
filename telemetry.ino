/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/lilygo-t-sim7000g-esp32-gps-data/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#define TINY_GSM_MODEM_SIM7000SSL // need to use this instead of SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <esp_now.h>
#include <WiFi.h>

#define SerialMon Serial
#define SerialAT  Serial1

// #include <StreamDebugger.h>
// StreamDebugger debugger(SerialAT, SerialMon);
// TinyGsm modem(debugger); // Pass the debugger instead of SerialAT

// Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13



// --- CELLULAR SETTINGS ---
const char apn[]      = "o2internet"; // your APN
const char gprsUser[] = "";
const char gprsPass[] = "";

const char server[]   = "telemetry.pecs.dev"; // your URL
const char resource[] = "/ingest.php"; // your endpoint
const int  port       = 443; // if http then 80
const char apiKey[]   = "your-key";

TinyGsm modem(SerialAT);
TinyGsmClientSecure client(modem);
HttpClient          http(client, server, port);

// Global Vars
unsigned long previousMillis[] = {0, 0}; 
const long interval[] = {2000, 10000}; // 0: Wifi, 1: GPS
String proccessedGPS = "";
String gpsBuffer = "";
int batchCounter = 0;

// ESP-NOW Setup
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x8E, 0xEE, 0xCC};
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;

void modemPowerOn(){
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff(){
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);
  digitalWrite(PWR_PIN, HIGH);
}

void modemRestart(){
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}

void setup(){
  SerialMon.begin(115200);
  SerialMon.println("Place your board outside to catch satelite signal");

  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  //Turn on the modem
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);


  modemPowerOn();

  // Set module baud rate and UART pins
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(6000);
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
  }

  SerialMon.println("Performing Hard Network Reset...");
  
  // Personal configs that work for me, you might not need these
    // Reset the profiles
    modem.sendAT("+CDNSCFG=\"8.8.8.8\",\"8.8.4.4\""); // Set DNS manually just in case
    modem.waitResponse();

    // Define the APN in BOTH slots 0 and 1 (to cover all bases)
    modem.sendAT("+CGDCONT=0,\"IP\",\"o2internet\""); 
    modem.waitResponse();
    modem.sendAT("+CGDCONT=1,\"IP\",\"o2internet\""); 
    modem.waitResponse();

    // Enable the network to "Auto-Select" (COPS=0)
    // This is better than forcing 23106 if the tower is being picky
    modem.sendAT("+COPS=0"); 
    modem.waitResponse();

    // Try to activate the packet service on Slot 0
    modem.sendAT("+CNACT=0,1"); 
    modem.waitResponse();

    modem.sendAT("+CSSLCFG=\"sslversion\",0,3"); // Set TLS 1.2
    modem.waitResponse();
    modem.sendAT("+CSSLCFG=\"sni\",0,\"telemetry.pecs.dev\""); // Required for Cloudflare
    modem.waitResponse();
    modem.sendAT("+CASSLCFG=0,\"SSL\",0"); // Bind Link 0 to SSL Context 0
    modem.waitResponse();
      

  // --- CONNECT TO GPRS ---
  SerialMon.print("Connecting to APN: ");
  SerialMon.println(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println("GPRS connection failed. Check SIM or signal.");
  } else {
    SerialMon.println("GPRS connected successfully!");
  }
  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }

  // Print modem info
  String modemName = modem.getModemName();
  delay(500);
  SerialMon.println("Modem Name: " + modemName);

  String modemInfo = modem.getModemInfo();
  delay(500);
  SerialMon.println("Modem Info: " + modemInfo);
  
  delay(5000);



  /*******************************************
  ================ WIFI SETUP ================
  *******************************************/

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  modem.sendAT("+SGPIO=0,4,1,1");
  modem.enableGPS();
}

void loop(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis[0] >= interval[0]) { // loop every 2s
    previousMillis[0] = currentMillis;

    wifiComunication();
  }

  if (currentMillis - previousMillis[1] >= interval[1]) { // loop every 10s
    previousMillis[1] = currentMillis;

    getGPSdata();
  }

  if (batchCounter >= 6) {
      if (modem.isGprsConnected()) {
          sendToAPI(gpsBuffer);
          gpsBuffer = "";
          batchCounter = 0;
      }
  }
}

void getGPSdata(){

  modem.enableGPS();
  float lat      = 0;
  float lon      = 0;
  float speed    = 0;
  float alt      = 0;
  int   vsat     = 0;
  int   usat     = 0;
  float accuracy = 0;
  int   year     = 0;
  int   month    = 0;
  int   day      = 0;
  int   hour     = 0;
  int   min      = 0;
  int   sec      = 0;

  SerialMon.println("Requesting current GPS/GNSS/GLONASS location");
  if (modem.getGPS(&lat, &lon, &speed, &alt, &vsat, &usat, &accuracy,
                    &year, &month, &day, &hour, &min, &sec)) {
                      // seems like it needs to be like this,
                      // even tho i dont need all vars

    // Create a clean, comma-separated string (CSV)
    proccessedGPS = String(lat, 8) + "," + 
                    String(lon, 8) + "," + 
                    String(speed) + "," + 
                    String(alt) + "," + 
                    String(usat) + "," +
                    String(accuracy) + "," +
                    String(hour) + ":" + 
                    String(min) + ":" + 
                    String(sec);

    if (gpsBuffer != "") {
      gpsBuffer += ";"; 
    }
    gpsBuffer += proccessedGPS;
    
    batchCounter++;
    SerialMon.println("Point added to buffer (" + String(batchCounter) + "/6)");

  } 
  else {
    SerialMon.println("Couldn't get GPS/GNSS/GLONASS location :C");
  }

  SerialMon.println(proccessedGPS);

  // String gps_raw = modem.getGPSraw(); // uncomment for debug if you have issues with GPS
  // SerialMon.println("debug:" + gps_raw + "\n");
}

void wifiComunication() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Wifi packet sent with success \n");
  }
  else {
    Serial.println("Wifi packet error, couldnt send the data \n");
  }
}

// wifi thingie
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sendToAPI(String data) {
  if (data == "") return;
  
  SerialMon.println("Sending via Single-Block Transmission...");

  // Build the entire request string manually
  String httpRequest = "POST " + String(resource) + " HTTP/1.1\r\n";
  httpRequest += "Host: " + String(server) + "\r\n";
  httpRequest += "X-API-KEY: " + String(apiKey) + "\r\n";
  httpRequest += "Content-Type: text/plain\r\n";
  httpRequest += "Content-Length: " + String(data.length()) + "\r\n";
  httpRequest += "Connection: close\r\n\r\n";
  httpRequest += data;

  // Use the SECURE client directly to bypass the fragmented HttpClient logic
  if (!client.connect(server, port)) {
    SerialMon.println("Connection failed");
    return;
  }

  // SEND EVERYTHING AT ONCE
  client.print(httpRequest);

  // Read response
  unsigned long start = millis();
  bool gotResponse = false;

  while (millis() - start < 5000) { // Wait up to 5 seconds for response
    if (modem.waitResponse("+CADATAIND: 0") == 1) {
      modem.sendAT("+CARECV=0,1024");
      modem.waitResponse(); // This will print the HTTP 200 OK to your Serial Monitor
      gotResponse = true;
      break;
    }
  }

  if (!gotResponse) {
    SerialMon.println("No response from server, but data was sent.");
  }

  client.stop();
}
