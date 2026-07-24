#include "./default.h"

#include "../config.h"

#include <WiFi.h>
#include <WiFiType.h>

void WifiManger::setupNormal(const char* ssid, const char* passwd) {
    WiFi.mode(WIFI_STA);
    if (ssid != nullptr && passwd != nullptr) {
        WiFi.begin(ssid, passwd);
    }
}

void WifiManger::setupMaint(const char* ssid, const char* passwd) {
    WiFi.mode(WIFI_AP);

    if (passwd == nullptr) {
        passwd = apPasswd;
    }
    WiFi.softAP(ssid, passwd);
}

void WifiManger::setupFailsafe(const char* ssid, const char* passwd) {
    WiFi.mode(WIFI_AP_STA);

    if (passwd == nullptr) {
        passwd = apPasswd;
    }
    WiFi.softAP(ssid, passwd);
}

WifiManger wifi;
