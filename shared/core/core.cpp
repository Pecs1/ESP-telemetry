#include "./core.h"

#include "../logs/logger.h"

#include <HardwareSerial.h>

void CoreUtil::setup() {
    Serial.begin(SERIAL_BAUD);
    delay(200);

    // bit of a fix
    while (Serial.available() > 0) {
        Serial.read();
    }

    Serial.println("\n\n");
    Serial.flush();
    delay(100);

    logger.info("core", "board has started!");
}
CoreUtil core;
