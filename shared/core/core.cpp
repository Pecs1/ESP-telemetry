#include "./core.h"

#include "utils/guard.h"

#include <HardwareSerial.h>

#define MODULE_NAME "core"
#define SUBMODULE_NAME "prefs"

// *** protect functions ***
void CoreUtil::setup() {
    if (!initSetup) {
        initSetup = true;
        protectedSetup();
    } else {
        guardMSG();
    }
}

void CoreUtil::checkKeys() {
    if (initSetup && !checkedKeys) {
        checkedKeys = true;
        protectedCheckKeys();
    } else if (!initSetup) {
        guardBlockMSG(MODULE_NAME, "setup");
    } else {
        guardMSG();
    }
}

SystemMode CoreUtil::readMode() {
    if (initSetup && checkedKeys) {
        return protectedReadMode();
    }

    if (!checkedKeys) {
        guardDepsMSG(MODULE_NAME, "checkKeys");
    }
    if (!initSetup) {
        guardDepsMSG(MODULE_NAME, "setup");
    }
    return SystemMode::UNKNOWN;
}

void CoreUtil::setMode(SystemMode nextMode) {
    if (initSetup && checkedKeys) {
        protectedSetMode(nextMode);
    }

    if (!checkedKeys) {
        guardDepsMSG(MODULE_NAME, "checkKeys");
    }
    if (!initSetup) {
        guardDepsMSG(MODULE_NAME, "setup");
    }
}

// *** protected core stuff ***
void CoreUtil::protectedSetup() {
    Serial.begin(SERIAL_BAUD);
    delay(200);

    // bit of a fix
    while (Serial.available() > 0) {
        Serial.read();
    }

    Serial.println("\n\n");
    Serial.flush();
    delay(100);

    logger.info(MODULE_NAME, "board has started!");
}

void CoreUtil::protectedCheckKeys() {
    // note that logger isnt in the docs, logger is my custom util
    // docs/example:
    // https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

    logger.debug(SUBMODULE_NAME, "opening \"%s\" persistent storage", nvsName);
    nvs.begin(nvsName, RW); // open RW
    logger.debug(SUBMODULE_NAME, "checking keys");

    // check individual keys
    checkKeyUtil(currentModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    checkKeyUtil(nextModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    // easily expandible ^^

    logger.debug(SUBMODULE_NAME, "closing persistent storage");
    nvs.end(); // close RW
    logger.info(MODULE_NAME, "successfuly checked keys");
}

SystemMode CoreUtil::protectedReadMode() {
    nvs.begin(nvsName, RW); // open RW
    logger.debug(SUBMODULE_NAME, "checking modes");

    uint8_t currentMode = nvs.getUChar(currentModeKey);
    uint8_t nextMode    = nvs.getUChar(nextModeKey);

    if (currentMode != nextMode) {
        logger.info(MODULE_NAME, "switching from \"%s\" to \"%s\" mode", stringify(currentMode),
                    stringify(nextMode));
        nvs.putUChar(currentModeKey, nextMode);

        currentMode = nextMode;
        logger.debug(MODULE_NAME, "successfuly applied mode");
    }

    logger.debug(SUBMODULE_NAME, "closing persistent storage");
    nvs.end(); // close RW
    logger.info(MODULE_NAME, "successfuly checked mode");

    return static_cast<SystemMode>(currentMode);
}

void CoreUtil::protectedSetMode(SystemMode nextMode) {
    nvs.begin(nvsName, RW); // open RW
    uint8_t mode = static_cast<uint8_t>(nextMode);
    logger.debug(SUBMODULE_NAME, "setting \"%s\" mode for next reboot", stringify(mode));

    nvs.putUChar(nextModeKey, mode);

    logger.debug(SUBMODULE_NAME, "closing persistent storage");
    nvs.end();
    logger.info(SUBMODULE_NAME, "mode set to \"%s\"", stringify(mode));
}
#undef SUBMODULE_NAME
#undef MODULE_NAME

CoreUtil core;
