/*
 * NOTICE:
 * - You can use this file as a template or starting point for your project.
 * - Rename `secrets.example.h` to `secrets.h` in `/shared/secrets`.
 * - Now you can add your credentials/secrets.
 * - Set BOARD_NAME below to match your hardware setup.
 *
 * - At the time of this writing, you can include my headers in any order,
 * after defining the "BOARD_NME", if that will change some day, i will add warnings ^^
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// since im using pioarduino (fork of platformio)
// you must set this at the top
#include <Arduino.h>

// you must set this before including wifi.h
// used to differentiate the boards when they are in AP mode
// must (welp should) be unique
#define BOARD_NAME "YourBoard"

// your secrets + logger utility are imported within this header
#include <config.h>

// contains core things/utilities related stuff
#include <core.h>

// contains persistant storage related stuff
#include <prefs.h>

// so far used only to setup wifi
// either in STA or AP mode
#include <wifi.h>

void setup() {
    // starts serial monitor
    // it *MAY* display "broken/trailing" info log - just visual bug
    // lazy to fix + it should be fine with the next log entry
    core.setup();

    // checks and create keys for later use
    // e.g. used to change modes after rebooting
    prefs.check();
}

void loop() {}
