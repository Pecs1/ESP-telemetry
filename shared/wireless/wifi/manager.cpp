#include "./manager.h"

#include "core.h"

#include <WiFi.h>
#include <WiFiType.h>

void WifiManger::setupNormal(const char* ssid, const char* passwd) {
    logger.debug("wifi", "setting to STA");

    WiFi.mode(WIFI_STA);

    if (ssid != nullptr && passwd != nullptr) {
        WiFi.begin(ssid, passwd);

        while (WiFi.status() != WL_CONNECTED) {
            delay(200);
            logger.info("wifi", "connecting...");
        }
        logger.info("wifi", "connected to %s", ssid);
    }
}

void WifiManger::setupMaint(const char* ssid, const char* passwd) {
    logger.debug("wifi", "setting to AP");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, passwd);

    logger.info("wifi", "started AP");
}

void WifiManger::setupFailsafe(const char* ssid, const char* passwd) {
    logger.debug("wifi", "setting to AP+STA");

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, passwd);

    logger.info("wifi", "started AP+STA");
}

WifiManger wifi;
