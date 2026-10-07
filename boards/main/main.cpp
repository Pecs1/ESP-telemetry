#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "atria/core.h"
// #include "atria/wireless.h"

extern "C" void app_main() {
    initArduino();
    setup();
    while (true) {
        loop();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

// the rest ill see, when the new board will be bought & delivered :/
void setup() {}

void loop() {}
