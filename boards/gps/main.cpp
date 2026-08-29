#include <Arduino.h>

#define BOARD_NAME "GPS"
#define WIRELESS_USE_ESPNOW

#include "core.h"
#include "wireless.h"

SystemMode mode;

void setup() {
    core.setup();

    core.checkKeys();

    mode = core.readMode();

    switch (mode) {
        case SystemMode::DEBUG:
            // setup same as in normal,
            // just pass additional debug overhead
            logger.setMinLevel(LogLevel::DEBUG);

            wifi.setupNormal();

            espnow.init();
            espnow.registerPeer(mainAddress);
            break;

        case SystemMode::NORMAL:
            wifi.setupNormal();

            espnow.init();
            espnow.registerPeer(mainAddress);
            break;

        case SystemMode::MAINT:
            wifi.setupMaint(maintSSID);
            break;

        case SystemMode::FAILSAFE:
            wifi.setupFailsafe(failsafeSSID);

            espnow.init();
            espnow.registerPeer(mainAddress);
            break;

        default:
            logger.crit("core", "mode not found!");
            logger.warn("core", "setting mode to failsafe!");

            core.setMode(SystemMode::FAILSAFE);
            ESP.restart();
    }
}

void loop() {
    switch (mode) {
        case SystemMode::NORMAL:
            // TODO: add normal
            break;

        case SystemMode::DEBUG:
            logger.debug("board", "new loop");
            break;

        case SystemMode::MAINT:
            // TODO: add maint
            break;

        default:
            // TODO: add failsafe
            // can move failsafe to default,
            // since not having mode should be handled in setup
            break;
    }
}
