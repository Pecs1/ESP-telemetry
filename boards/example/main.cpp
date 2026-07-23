/*
 * NOTICE:
 * - You can use this file as a template or starting point for your project.
 * - Rename `secrets.example.h` to `secrets.h` in `/shared/secrets`.
 * - Now you can add your credentials/secrets.
 * - Set BOARD_NAME below to match your hardware setup.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// since im using pioarduino (fork of platformio)
// you must set this at the top
#include <Arduino.h>

// you must set this before including config.h
// used to differentiate the boards when they are in AP mode
// must (welp should) be unique
#define BOARD_NAME "YourBoard"

// you will find some basic configs here
// also your secrets are imported within this header
#include <config.h>

// so far used only to setup wifi
// either in STA or AP mode
#include <wifi.h>

void setup() {}

void loop() {}
