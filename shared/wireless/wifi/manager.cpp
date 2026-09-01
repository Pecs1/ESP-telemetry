#include "./manager.h"

#include "core.h"

#include <WiFi.h>
#include <WiFiType.h>

#define MODULE_NAME "wifi"

void WifiManger::protectedSetupNormal(const char* ssid, const char* passwd) {
    logger.debug(MODULE_NAME, "setting to STA");

    WiFi.mode(WIFI_MODE_STA);

    if (ssid != nullptr && passwd != nullptr) {
        WiFi.begin(ssid, passwd);

        while (WiFi.status() != WL_CONNECTED) {
            delay(200);
            logger.info(MODULE_NAME, "connecting...");
        }
        logger.info(MODULE_NAME, "connected to %s", ssid);
    }
}

void WifiManger::protectedSetupMaint(const char* ssid, const char* passwd) {
    logger.debug(MODULE_NAME, "setting to AP");

    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ssid, passwd);

    logger.info(MODULE_NAME, "started AP");
}

void WifiManger::protectedSetupFail(const char* ssid, const char* passwd) {
    logger.debug(MODULE_NAME, "setting to AP+STA");

    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.softAP(ssid, passwd);

    logger.info(MODULE_NAME, "started AP+STA");
}
#undef MODULE_NAME

WifiManger wifi;
