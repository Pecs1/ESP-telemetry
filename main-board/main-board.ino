/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/lilygo-t-sim7000g-esp32-gps-data/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include "env.h"

#define TINY_GSM_MODEM_SIM7000SSL // need to use this instead of SIM7000
#define TINY_GSM_RX_BUFFER 1024   // Set RX buffer to 1Kb

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// needed for wifi
#include <esp_now.h>
#include <WiFi.h>

// needed to config watchdog
#include "esp_task_wdt.h"

#define SerialMon Serial
#define SerialAT  Serial1

#if DEBUG_ENABLED
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT); 
#endif

TinyGsmClientSecure client(modem);
HttpClient          http(client, server, port);

// Global Vars
unsigned long previousMillis[] = {0, 0};
const long interval[] = {INTERVAL_WIFI, INTERVAL_GPS};
String processedGPS = "";
String gpsBuffer = "";
int batchCounter = 0;
float currentBatVoltage = 0.0;
int currentBatPercentage = 0;

// ESP-NOW Setup
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;

void modemPowerOn() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);
  digitalWrite(PWR_PIN, HIGH);
}

void modemPowerOff() {
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);
  digitalWrite(PWR_PIN, HIGH);
}

void modemRestart() {
  modemPowerOff();
  delay(1000);
  modemPowerOn();
}

// --- MULTICORE / FREERTOS VARIABLES ---
SemaphoreHandle_t lteMutex;
String sharedGpsBuffer = "";
bool readyToUpload = false;


void setup() {
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
    // Define the APN in BOTH slots 0 and 1 (to cover all bases)
    modem.sendAT("+CGDCONT=0,\"IP\",\"" + String(apn) + "\""); 
    modem.waitResponse();
    modem.sendAT("+CGDCONT=1,\"IP\",\"" + String(apn) + "\"");
    modem.waitResponse();

    // Enable the network to "Auto-Select" (COPS=0)
    // This is better than forcing 23106 if the tower is being picky
    modem.sendAT("+COPS=0"); 
    modem.waitResponse();

    // Try to activate the packet service on Slot 0
    modem.sendAT("+CNACT=0,1"); 
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

  // ================= WIFI SETUP ================
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  modem.sendAT("+SGPIO=0,4,1,1");
  modem.enableGPS();

  // --- INITIALIZE MUTEX & CORE 0 TASK ---
  lteMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
    LTE_Upload_Task,  // Task function
    "LTE_Task",       // name of task.
    20000,            // Stack size of task (20KB for Strings/HTTP)
    NULL,             // parameter of the task
    1,                // priority of the task
    NULL,             // Task handle
    0                 // pin task to core 0
  );
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis[0] >= interval[0]) { // loop every 2s
    previousMillis[0] = currentMillis;

    wifiComunication();
  }

  if (currentMillis - previousMillis[1] >= interval[1]) { // loop every 10s
    previousMillis[1] = currentMillis;

    getGPSdata();
  }

  if (batchCounter >= GPS_BATCH_SIZE) {
    if (xSemaphoreTake(lteMutex, 0) == pdTRUE) {
      sharedGpsBuffer = gpsBuffer; // Copy the data to shared memory
      readyToUpload = true;        // Signal Core 0 to wake up

      gpsBuffer = "";              // Clear the local buffer immediately
      batchCounter = 0;            // Reset the counter
      xSemaphoreGive(lteMutex);
      Serial.println(">>> Core 1: Data handed off to Core 0.");
    }
  }
}

// --- CORE 0: BACKGROUND LTE TASK ---
void LTE_Upload_Task(void *pvParameters) {

  esp_task_wdt_config_t twdt_config = {
    .timeout_ms = 30000,        // 30 seconds
    .idle_core_mask = (1 << 0), // Specifically watch Core 0
    .trigger_panic = true       // Reset if it hangs for 30s
  };

  esp_task_wdt_reconfigure(&twdt_config);

  for (;;) {
    String dataToSend = "";
    bool shouldSend = false;

    // check if Core 1 has provided new data
    if (xSemaphoreTake(lteMutex, (TickType_t)10) == pdTRUE) {
      if (readyToUpload) {
        dataToSend = sharedGpsBuffer;
        shouldSend = true;

        readyToUpload = false; // Reset the flag
        sharedGpsBuffer = "";  // Clear shared memory
      }
      xSemaphoreGive(lteMutex);
    }

    // If data is ready, do the slow LTE upload here
    if (shouldSend) {
      if (modem.isGprsConnected()) {
        sendToAPI(dataToSend);
      } else {
        SerialMon.println("Cannot upload: GPRS disconnected.");
      }
    }

    // Sleep to feed the watchdog timer and prevent a crash
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void getGPSdata() {

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
    processedGPS = String(lat, 8) + "," + 
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
    gpsBuffer += processedGPS;

    batchCounter++;
    SerialMon.println("Point added to buffer (" + String(batchCounter) + "/6)");

  } else {
    SerialMon.println("Couldn't get GPS/GNSS/GLONASS location :C");
  }

  SerialMon.println(processedGPS);
}

void wifiComunication() {
  // Set values to send
  strcpy(myData.a, DEVICE_ID);
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Wifi packet sent with success \n");
  } else {
    Serial.println("Wifi packet error, couldnt send the data \n");
  }
}

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

  while (millis() - start < SERVER_TIMEOUT) { // Wait up to set amount of seconds for response
    yield();

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

void batteryStatus() {
  int raw = analogRead(BATT_PIN);
  currentBatVoltage = (raw / 4095.0) * 2.0 * 3.3 * BATT_CALIBRATION;

  if (currentBatVoltage < 1.0) {
    Serial.println("Battery: Most Likely Using USB Power");
  } else if (currentBatVoltage > (BATT_MIN - 0.4) && currentBatVoltage < (BATT_MAX + 0.4)) {
    float pc = (currentBatVoltage - BATT_MIN) / (BATT_MAX - BATT_MIN) * 100.0;
    currentBatPercentage = (int)constrain(pc, 0, 100);
    Serial.printf("Battery: %.2fV | %d%%\n", currentBatVoltage, currentBatPercentage);
  } else {
    Serial.println("Battery: Bad Values / ERROR :C");
  }
}
