#include "init.h"

#include "atria/core/atria.h"
#include "atria/utils/logger.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void initApp() {
    initArduino();

    // will see if ill put the silencer above initArduino(),
    // since it prints some stuff...
    logger.silenceBootloader();

    // starts serial monitor
    core.setup();

    // checks and creates missing keys for later use
    // e.g. used to change modes after rebooting
    core.checkKeys();

    // run functions/stuff declared inside boards setup()
    setup();
    while (true) {
        loop();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
