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

void setup() {}

void loop() {}
