#include "./manager.h"

#include "utils/guard.h"
#include "utils/logger.h"

#include <WiFi.h>
#include <WiFiType.h>

#define MODULE_NAME "wifi"

// *** protect functions to run only once ***
void WifiManger::setupNormal(const char* ssid, const char* passwd) {
    if (!wifiInitd) {
        wifiInitd       = true;
        wifiInitdNormal = true;
        protectedSetupNormal(ssid, passwd);
    } else {
        if (wifiInitdMaint) {
            guardBlockMSG(MODULE_NAME, "setupMaint");
        } else if (wifiInitdFail) {
            guardBlockMSG(MODULE_NAME, "setupFailSafe");
        } else {
            guardMSG();
        }
    }
}

void WifiManger::setupMaint(const char* ssid, const char* passwd) {
    if (!wifiInitd) {
        wifiInitd      = true;
        wifiInitdMaint = true;
        protectedSetupMaint(ssid, passwd);
    } else {
        if (wifiInitdNormal) {
            guardBlockMSG(MODULE_NAME, "setupNormal");
        } else if (wifiInitdFail) {
            guardBlockMSG(MODULE_NAME, "setupFailSafe");
        } else {
            guardMSG();
        }
    }
}

void WifiManger::setupFailsafe(const char* ssid, const char* passwd) {
    if (!wifiInitd) {
        wifiInitd     = true;
        wifiInitdFail = true;
        protectedSetupFail(ssid, passwd);
    } else {
        if (wifiInitdNormal) {
            guardBlockMSG(MODULE_NAME, "setupNormal");
        } else if (wifiInitdMaint) {
            guardBlockMSG(MODULE_NAME, "setupMaint");
        } else {
            guardMSG();
        }
    }
}

// *** protected wifi implementation ***
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
