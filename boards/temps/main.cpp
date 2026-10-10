#define BOARD_NAME "Temps"

#include "atria/core.h"
// #include "atria/wireless.h"

extern "C" void app_main() {
    initApp();
}

#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup() {
    core.setup();

    sensors.begin();
}

void loop() {
    sensors.requestTemperatures();

    float sensorx1 = sensors.getTempC(sensor1);
    float sensorx2 = sensors.getTempC(sensor2);

    logger.info("temps", "current temps: %.2f for sensor1", sensorx1);
    logger.info("temps", "current temps: %.2f for sensor2", sensorx2);

    delay(1000);
}
