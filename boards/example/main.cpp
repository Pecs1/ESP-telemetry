// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/*
 * NOTICE & QUICK START:
 * - Use this file as a starting point or template for new board implementations.
 *
 * SETUP INSTRUCTIONS:
 * 1. Secrets: Rename `secrets.example.h` to `secrets.h` in `/atria/secrets`
 *    and fill in your credentials.
 * 2. Configuration:
 *    - Define `BOARD_NAME` in platformio.ini to match your hardware setup.
 *    - Set the appropriate wireless flags/definitons for your use case.
 *
 * NOTE:
 * - `BOARD_NAME` is used to differentiate the boards when they are in AP mode,
 *   thats why it should be unique if you plan to use more boards...
 */

// NOTICE:
// you must set one of these definition before including wireless.h
// - used to include only the protocols you will need
//   options:
//     - WIRELESS_USE_WIFI   - includes only wifi
//     - WIRELESS_USE_ESPNOW - includes wifi and espnow
//     - WIRELESS_USE_ALL    - includes all protocols
#define WIRELESS_USE_ESPNOW

// contains core things/utilities, persistant storage, logger...
#include "atria/core.h"

// includes wifi + espnow utility
#include "atria/wireless.h"

// required to have framework = arduino, espidf
extern "C" void app_main() {
    initApp();
}

// note: you can rename "mode" to your liking
// must be set after including core.h
SystemMode mode;

void setup() {
    // reads the mode that was set with "core.setMode()"
    // usage: you could run/load mode specific code
    mode = core.readMode();

    // for example:
    switch (mode) {
        case SystemMode::NORMAL: {
            // start wifi in STA
            wifi.setupNormal();

            // init esp_now & check if it was initiated successfully
            espnow.init();

            // register peer/peers & check if they were added successfully
            espnow.registerPeer(mainAddress);

            // init sensors....
            // handle/start-up stuff required for normal mode, etc etc
            break;
        }

        case SystemMode::MAINT: {
            // start wifi in AP
            wifi.setupMaint(maintSSID);

            espnow.init();
            espnow.registerPeer(mainAddress);

            // do other stuff, like for e.g. start & handle webserver, etc etc
            break;
        }

        // pretty much required to handle incorrect modes
        default: {
            logger.crit("core", "mode not found!");
            logger.warn("core", "setting mode to failsafe!");

            // you should set this to the mode you need, for example, here you need just MAINT
            // for my boards i need to have ::FAILSAFE,
            // so i could still try to poll sensors and handle updating
            core.setMode(SystemMode::MAINT);
            ESP.restart();
        }
    }
}

void loop() {
    switch (mode) {
        case SystemMode::NORMAL: {
            // TODO: add normal
            break;
        }

        // can be here either `default:` or `case SystemMode::MAINT:`
        // its up to your preference, which one you use
        default: {
            // TODO: add maint
            break;
        }
    }
}
