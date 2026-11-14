#define WIRELESS_USE_ESPNOW

#include "atria/core.h"
#include "atria/wireless.h"

extern "C" void app_main() {
    initApp();
}

#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

SystemMode mode;

void setup() {
    mode = core.readMode();

    switch (mode) {
        case SystemMode::DEBUG: {
            // setup same as in normal,
            // just pass additional debug overhead
            logger.setMinLevel(LogLevel::DEBUG);

            wifi.setupNormal();

            espnow.init();
            espnow.registerPeer(mainAddress);

            sensors.begin();
            break;
        }

        case SystemMode::NORMAL: {
            wifi.setupNormal();

            espnow.init();
            espnow.registerPeer(mainAddress);

            sensors.begin();
            break;
        }

        case SystemMode::MAINT: {
            wifi.setupMaint(maintSSID);
            break;
        }

        case SystemMode::FAILSAFE: {
            wifi.setupFailsafe(failsafeSSID);

            espnow.init();
            espnow.registerPeer(mainAddress);
            break;
        }

        default: {
            core.fallbackReboot(SystemMode::FAILSAFE);
        }
    }
}

void loop() {
    switch (mode) {
        case SystemMode::NORMAL: {
            sensors.requestTemperatures();

            float sensorx1 = sensors.getTempC(sensor1);
            float sensorx2 = sensors.getTempC(sensor2);

            logger.info("temps", "current temps: %.2f for sensor1", sensorx1);
            logger.info("temps", "current temps: %.2f for sensor2", sensorx2);

            delay(1000);
            break;
        }

        case SystemMode::DEBUG: {
            logger.debug("board", "new loop");
            sensors.requestTemperatures();

            float sensorx1 = sensors.getTempC(sensor1);
            float sensorx2 = sensors.getTempC(sensor2);

            logger.info("temps", "current temps: %.2f for sensor1", sensorx1);
            logger.info("temps", "current temps: %.2f for sensor2", sensorx2);

            delay(1000);
            break;
        }

        case SystemMode::MAINT: {
            // TODO: add maint
            break;
        }

        default: {
            // TODO: add failsafe
            // can move failsafe to default,
            // since not having mode should be handled in setup
            break;
        }
    }
}
