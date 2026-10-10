#include "init.h"

#include "atria/utils/logger.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void initApp() {
    initArduino();

    // will see if ill put the silencer above initArduino(),
    // since it prints some stuff...
    logger.silenceBootloader();
    setup();
    while (true) {
        loop();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
