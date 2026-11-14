#include "./atria.h"

#include "atria/utils/guard.h"
#include "atria/utils/logger.h"
#include "atria/utils/prefs.h"
#include "atria/utils/versioner.h"

#include <HardwareSerial.h>
#include <esp_system.h>

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
    // check and return if setup was called already
    if (initSetup) {
        guardMSG();
        return;
    }
    initSetup = true;
    protectedSetup();
}

void CoreUtil::checkKeys() {
    // check and return if setup wasnt called yet
    if (!initSetup) {
        guardBlockMSG("setup");
        return;
    }

    // check and return if checkKeys was called already
    if (checkedKeys) {
        guardMSG();
        return;
    }
    checkedKeys = true;
    protectedCheckKeys();
}

SystemMode CoreUtil::readMode() {
    // check and return if setup wasnt called yet
    if (!initSetup) {
        guardDepsMSG("setup");
        return SystemMode::UNKNOWN;
    }

    // check and return if checkKeys was called already
    if (!checkedKeys) {
        guardDepsMSG("checkKeys");
        return SystemMode::UNKNOWN;
    }
    return protectedReadMode();
}

void CoreUtil::setMode(SystemMode nextMode) {
    // check and return if setup wasnt called yet
    if (!initSetup) {
        guardDepsMSG("setup");
        return;
    }

    // check and return if checkKeys wasnt called yet
    if (!checkedKeys) {
        guardDepsMSG("checkKeys");
        return;
    }
    protectedSetMode(nextMode);
}

void CoreUtil::fallbackReboot(SystemMode fallbackMode) {
    if (!initSetup) {
        guardDepsMSG("setup");
        return;
    }

    if (!checkedKeys) {
        guardDepsMSG("checkKeys");
        return;
    }
    protectedFallbackReboot(fallbackMode);
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

    logger.info(MODULE_NAME, "\"%s\" board started (app: v%s, ABI: v%u)", version.getProjectName(),
                version.getProjectVer(), version.getLibABI());
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

void CoreUtil::protectedFallbackReboot(SystemMode fallbackMode) {
    logger.crit(MODULE_NAME, "valid mode not found!");

    if (fallbackMode == SystemMode::UNKNOWN) {
        logger.err(MODULE_NAME, "UNKNOWN is not a valid fallback");
        logger.warn(MODULE_NAME, "setting to FAILSAFE");
        fallbackMode = SystemMode::FAILSAFE;
    }

    // no need to use the public definition, since using the same guards
    protectedSetMode(fallbackMode);
    esp_restart();
}
CoreUtil core;
