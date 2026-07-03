#include "gps.env.h"

// for normal mode
#include <esp_now.h>
#include <WiFi.h>

// for maintenance mode
#include <WiFiAP.h>


// persistant storage between reboots
#include <Preferences.h>


// esp_now.h
esp_now_peer_info_t peerInfo;

// preferences.h
Preferences prefs;


void setup() {
  Serial.begin(115200);

  // docs: https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html
  // read-only the persistant storage
  prefs.begin("sys_state", RO);

  // check if persistant storage has been created
  if (prefs.isKey("ready") == false) {
    prefs.end();                   // close RO
    prefs.begin("sys_state", RW);  // reopen in RW

    // write to persistant storage
    prefs.putBool("ready", true);
    prefs.putUChar("current_mode", 0);

    prefs.end();                   // close RW
    prefs.begin("sys_state", RO);  // reopen in RO
  }

  uint8_t savedMode = prefs.getUChar("current_mode");
  currentMode = (SystemMode)savedMode;

  prefs.end();  // close RO


  switch (currentMode) {
    case NORMAL:
      initNormal();
      break;

    case MAINTENANCE:
      initMaintenance();
      break;

    default:
      initFailSafe();
      break;
  }
}

void loop() {
  switch (currentMode) {
    case NORMAL:

      return;

    case MAINTENANCE:

      return;

    default:

      return;
  }
}

void initNormal() {
  // docs: https://docs.arduino.cc/tutorials/nano-esp32/esp-now/#code
  // set as WiFi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // get status of transmitted packet
  esp_now_register_send_cb(OnDataSent);

  // clear out data just in case
  memset(&peerInfo, 0, sizeof(peerInfo));

  // copy the peer address into peer structure
  memcpy(peerInfo.peer_addr, mainAddress, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add main board!");
    return;
  }
}

void initMaintenance() {
  // docs: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html#wi-fi-ap-example
  // set to AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSWD);
}

void initFailSafe() {
}

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    currentTxState = TX_SUCCESS;
  } else {
    currentTxState = TX_FAILED;
  }
}
