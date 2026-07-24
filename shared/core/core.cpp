#include "./core.h"

#include "../logs/logger.h"

#include <HardwareSerial.h>

void CoreUtil::setup() {
    Serial.begin(SERIAL_BAUD);
    delay(200);
    Serial.flush();
    // bit of a fix
    Serial.println();
    delay(300);
    Serial.flush();
    logger.info("core", "board has started!");
}

CoreUtil core;
