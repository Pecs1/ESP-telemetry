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
 *    - Define `BOARD_NAME` below to match your hardware setup.
 *    - Set the appropriate wireless flags/mode for your use case.
 *
 * NOTE:
 * - `BOARD_NAME` must be defined BEFORE including project headers.
 */

// NOTICE:
// you must set these macros before including wireless.h
// - used to differentiate the boards when they are in AP mode
//   should be unique if you plan to use more boards...
#define BOARD_NAME "YourBoard"

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
    // starts serial monitor
    core.setup();

    // checks and creates missing keys for later use
    // e.g. used to change modes after rebooting
    core.checkKeys();

    // reads the mode that was set with "core.setMode()"
    //
    // usage: you could run/load mode specific code
    mode = core.readMode();

    // init esp_now & check if it was initiated successfully
    espnow.init();

    // register peer/peers & check if they were added successfully
    espnow.registerPeer(mainAddress);
}

void loop() {}
