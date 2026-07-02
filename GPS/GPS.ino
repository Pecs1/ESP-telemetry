#include "gps.env.h"

// for wifi communication
#include <esp_now.h>
#include <WiFi.h>

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
          prefs.end(); // close RO
          prefs.begin("sys_state", RW); // reopen in RW

          // write to persistant storage
          prefs.putBool("ready", true);
          prefs.putBool("is_normal", true);

          prefs.end(); // close RW
          prefs.begin("sys_state", RO); // reopen in RO
     }

     isNormal = prefs.getBool("is_normal");
     prefs.end(); // close RO

}

void loop() {

}
