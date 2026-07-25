#include "./core.h"

#include "../logs/logger.h"

#include <HardwareSerial.h>

void CoreUtil::setup() {
    Serial.begin(SERIAL_BAUD);
    delay(200);

    // bit of a fix
    while (Serial.available() > 0) {
        Serial.read();
    }

    Serial.println("\n\n");
    Serial.flush();
    delay(100);

    logger.info("core", "board has started!");
}

void CoreUtil::checkKeys() {

    // note that logger isnt in the docs, logger is my custom util
    // docs/example:
    // https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

    logger.debug("core - prefs", "opening \"%s\" persistant storage", nvsName);
    nvs.begin(nvsName, RW); // open RW
    logger.info("core - prefs", "checking keys");

    // check individual keys
    checkKeyUtil(currentModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    // easily expandible ^^

    logger.debug("core - prefs", "closing persistant storage");
    nvs.end(); // close RW
    logger.info("core - prefs", "done checking keys");
}

CoreUtil core;
