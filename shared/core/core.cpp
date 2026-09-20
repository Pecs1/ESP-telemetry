#include "./core.h"

#include "utils/guard.h"
#include "utils/logger.h"

#include <HardwareSerial.h>
#include <Preferences.h>

#define MODULE_NAME "core"
#define SUBMODULE_NAME "prefs"

namespace {
    // Read/Write access flags
    constexpr bool rw = false;
    // constexpr bool ro = true; // would get lint error if uncommented :/

    // NVS state keys
    constexpr const char* nvsName        = "sys_state";
    constexpr const char* currentModeKey = "current_mode";
    constexpr const char* nextModeKey    = "next_mode";

    Preferences nvs;

    // Internal helper to stringify system mode
    constexpr const char* stringify(uint8_t mode) {
        switch (static_cast<SystemMode>(mode)) {
            case SystemMode::NORMAL:
                return "NORMAL";
            case SystemMode::DEBUG:
                return "DEBUG";
            case SystemMode::MAINT:
                return "MAINT";
            case SystemMode::FAILSAFE:
                return "FAILSAFE";
            default:
                return "UNKNOWN";
        }
    }

    // Helper for non-existent key creation
    template <typename Func>
    void checkKeyUtil(const char* key, Func&& arg) {
        if (!nvs.isKey(key)) {
            logger.warn(SUBMODULE_NAME, "\"%s\" key not found", key);
            logger.debug(SUBMODULE_NAME, "creating \"%s\" key", key);

            arg(key);

            logger.info(SUBMODULE_NAME, "\"%s\" key created", key);
        }
    }
} // namespace

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
    nvs.begin(nvsName, rw); // open in RW
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
    nvs.begin(nvsName, rw); // open in RW
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
    nvs.begin(nvsName, rw); // open in RW
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
