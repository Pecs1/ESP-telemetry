#include "./atria.h"

#include "atria/utils/guard.h"
#include "atria/utils/logger.h"
#include "atria/utils/prefs.h"

#include <HardwareSerial.h>

#define MODULE_NAME "core"

namespace {
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
        guardBlockMSG("setup");
    } else {
        guardMSG();
    }
}

SystemMode CoreUtil::readMode() {
    if (initSetup && checkedKeys) {
        return protectedReadMode();
    }

    if (!checkedKeys) {
        guardDepsMSG("checkKeys");
    }
    if (!initSetup) {
        guardDepsMSG("setup");
    }
    return SystemMode::UNKNOWN;
}

void CoreUtil::setMode(SystemMode nextMode) {
    if (initSetup && checkedKeys) {
        protectedSetMode(nextMode);
    }

    if (!checkedKeys) {
        guardDepsMSG("checkKeys");
    }
    if (!initSetup) {
        guardDepsMSG("setup");
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
    logger.debug(MODULE_NAME, "checking keys");

    prefs.keyGenerator();
    
    logger.info(MODULE_NAME, "successfuly checked keys");
}

SystemMode CoreUtil::protectedReadMode() {
    logger.debug(MODULE_NAME, "checking modes");

    uint8_t currentMode = prefs.keyReader(currentModeKey);
    uint8_t nextMode    = prefs.keyReader(nextModeKey);

    if (currentMode != nextMode) {
        logger.info(MODULE_NAME, "switching from \"%s\" to \"%s\" mode", stringify(currentMode),
                    stringify(nextMode));

        prefs.keyWriter(currentModeKey, nextMode);

        currentMode = nextMode;
        logger.debug(MODULE_NAME, "successfuly applied mode");
    }

    logger.info(MODULE_NAME, "successfuly checked mode");

    return static_cast<SystemMode>(currentMode);
}

void CoreUtil::protectedSetMode(SystemMode nextMode) {
    uint8_t mode = static_cast<uint8_t>(nextMode);
    logger.debug(MODULE_NAME, "setting \"%s\" mode for next reboot", stringify(mode));

    prefs.keyWriter(nextModeKey, mode);

    logger.info(MODULE_NAME, "mode set to \"%s\"", stringify(mode));
}
#undef MODULE_NAME

CoreUtil core;
