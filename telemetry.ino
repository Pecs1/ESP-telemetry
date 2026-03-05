/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/lilygo-t-sim7000g-esp32-gps-data/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/


// LOOP VARs
  unsigned long previousMillis[] = {0, 0};
  const long interval[] = {2000, 10000}; // in ms
  const String SECRET_KEY = ""; // Your phrase to connect to your website, not implemented yet lol
  String proccessedGPS = "";

// GPS stuff
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <TinyGsmClient.h> // GPS

// LilyGO T-SIM7000G Pinout
#define UART_BAUD   115200
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands
#define SerialAT  Serial1

TinyGsm modem(SerialAT);

// WIFI THINGS
  #include <esp_now.h> // wifi?
  #include <WiFi.h> // wifi duh

  // REPLACE WITH YOUR RECEIVER MAC Address
  uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x8E, 0xEE, 0xCC};

  // Structure example to send dat
  // Must match the receiver structure
  typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
  } struct_message;

  // Create a struct_message called myData
  struct_message myData;

  esp_now_peer_info_t peerInfo;

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

  delay(1000);
  
  // Set module baud rate and UART pins
  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
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
}

void getGPSdata(){
  // Set SIM7000G GPIO4 HIGH ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,1");
  if (modem.waitResponse(10000L) != 1) {
    SerialMon.println(" SGPIO=0,4,1,1 false ");
  }

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
      
  //   proccessedGPS = String(lat, 8) ?? NULL + String(lon, 8) ?? NULL + "Speed: " + String(speed) ?? NULL + "\tAltitude: " + String(alt);
  //   SerialMon.println("Latitude: " + String(lat, 8) + "\tLongitude: " + String(lon, 8));
  //   SerialMon.println("Speed: " + String(speed) + "\tAltitude: " + String(alt));
  //   SerialMon.println("Visible Satellites: " + String(vsat) + "\tUsed Satellites: " + String(usat));
  //   SerialMon.println("Accuracy: " + String(accuracy));
  //   SerialMon.println("Year: " + String(year) + "\tMonth: " + String(month) + "\tDay: " + String(day));
  //   SerialMon.println("Hour: " + String(hour) + "\tMinute: " + String(min) + "\tSecond: " + String(sec));


  // Create a clean, comma-separated string (CSV)
  proccessedGPS = String(lat, 8) + "," + 
                  String(lon, 8) + "," + 
                  String(speed) + "," + 
                  String(alt) + "," + 
                  String(usat) + "," +
                  String(accuracy) + "," +
                  String(hour) + ":" + 
                  String(min) + ":" + 
                  String(sec) + ",1";
                    
  //SerialMon.println("Success! Data: " + proccessedGPS);

  } 
  else {
    SerialMon.println("Couldn't get GPS/GNSS/GLONASS location :C");
    proccessedGPS = "0,0,0,0,0,0,0:0:0,0";
  }

  String gps_raw = modem.getGPSraw();
  SerialMon.println("debug:" + gps_raw + "\n");

  SerialMon.println(proccessedGPS);

  
  //SerialMon.println("Retrieving GPS/GNSS/GLONASS location as a string");

  // SerialMon.println("Disabling GPS");
  // modem.disableGPS();

  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  // modem.sendAT("+SGPIO=0,4,1,0");
  // if (modem.waitResponse(10000L) != 1) {
  //  SerialMon.println(" SGPIO=0,4,1,0 false ");
  // }

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