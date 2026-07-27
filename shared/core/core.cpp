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

    logger.debug("prefs", "opening \"%s\" persistant storage", nvsName);
    nvs.begin(nvsName, RW); // open RW
    logger.info("prefs", "checking keys");

    // check individual keys
    checkKeyUtil(currentModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    checkKeyUtil(nextModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    // easily expandible ^^

    logger.debug("prefs", "closing persistant storage");
    nvs.end(); // close RW
    logger.info("prefs", "done checking keys");
}

SystemMode CoreUtil::readMode() {
    nvs.begin(nvsName, RW); // open RW
    logger.debug("prefs", "checking modes");

    uint8_t currentMode = nvs.getUChar(currentModeKey);
    uint8_t nextMode    = nvs.getUChar(nextModeKey);

    if (currentMode != nextMode) {
        logger.info("core", "switching from \"%s\" to \"%s\" mode", stringify(currentMode),
                    stringify(nextMode));
        nvs.putUChar(currentModeKey, nextMode);

        currentMode = nextMode;
    }

    logger.debug("prefs", "closing persistant storage");
    nvs.end(); // close RW
    logger.info("core", "successfuly checked & applied mode");

    return static_cast<SystemMode>(currentMode);
}
CoreUtil core;
