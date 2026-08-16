/*
 * NOTICE:
 * - You can use this file as a template or starting point for your project.
 * - Rename `secrets.example.h` to `secrets.h` in `/shared/secrets`.
 * - Now you can add your credentials/secrets.
 * - Set BOARD_NAME below to match your hardware setup.
 *
 * - At the time of this writing, you can include my headers in any order,
 * after defining the "BOARD_NAME", if that will change some day, i will add warnings ^^
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// since im using pioarduino (fork of platformio)
// you must set this at the top
#include <Arduino.h>

// you must set this before including wireless.h
// used to differentiate the boards when they are in AP mode
// must (welp should) be unique
#define BOARD_NAME "YourBoard"

// contains core things/utilities, persistant storage, logger...
#include "core.h"

// includes wifi + espnow utility
#include "wireless.h"

// note you can rename "mode" to your liking
// must be set after including core.h
SystemMode mode;

void setup() {
    // starts serial monitor
    core.setup();

    // checks and create keys for later use
    // e.g. used to change modes after rebooting
    core.checkKeys();

    // MUST be called after checkKeys()
    // can only be once in void setup()
    // checks if "current mode" is the same as "next mode"
    // sets the "next mode" value to "current mode" if different

    // usage: if you have a switch in your code,
    // you could run/load mode specific code
    mode = core.readMode();

    // check if esp_now was initiated successfully
    espnow.init();

    // register peer/peers & check if they were added successfully
    espnow.registerPeer(mainAddress);
}

void loop() {}
