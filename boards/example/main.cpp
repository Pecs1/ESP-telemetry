/*
 * NOTICE:
 * - You can use this file as a template or starting point for your project.
 * - Rename `secrets.example.h` to `secrets.h` in `/shared/secrets`.
 * - Now you can add your credentials/secrets.
 * - Set BOARD_NAME below to match your hardware setup.
 * - Set correct flag for wireless to your use case
 *
 * - After you build and monitor, you may see messages such as: "no core dump partition found!"
 *   you can safely ignore them
 *
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// since im using pioarduino (fork of platformio)
// you must set this at the top
#include <Arduino.h>

// NOTICE:
// you must set these macros before including wireless.h
// - used to differentiate the boards when they are in AP mode
//   should be unique if you plan to use more boards...
#define BOARD_NAME "YourBoard"

// - used to only include only the protocols you will need
//   options:
//     - WIRELESS_USE_WIFI   - includes only wifi
//     - WIRELESS_USE_ESPNOW - includes wifi and espnow
//     - WIRELESS_USE_ALL    - includes all protocols
#define WIRELESS_USE_ESPNOW

// contains core things/utilities, persistant storage, logger...
#include "core.h"

// includes wifi + espnow utility
#include "wireless.h"

// note: you can rename "mode" to your liking
// must be set after including core.h
SystemMode mode;

void setup() {
    // starts serial monitor
    core.setup();

    // checks and create missing keys for later use
    // e.g. used to change modes after rebooting
    core.checkKeys();

    // reads the mode, that was set with "core.setMode()"
    //
    // usage: you could run/load mode specific code
    mode = core.readMode();

    // init esp_now & check if it was initiated successfully
    espnow.init();

    // register peer/peers & check if they were added successfully
    espnow.registerPeer(mainAddress);
}

void loop() {}
