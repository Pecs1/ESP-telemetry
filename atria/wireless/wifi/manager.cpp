#include "./manager.h"

#include "atria/utils/guard.h"
#include "atria/utils/logger.h"

#include <esp_wifi.h>
#include <string.h>

#define MODULE_NAME "wifi"

void WifiManger::init() {
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);
}

void WifiManger::copyField(uint8_t* dst, size_t dstSize, const char* src) {
    if (src == nullptr)
        return;
    size_t len = strnlen(src, dstSize);
    memcpy(dst, src, len);
}

// *** protect functions to run only once ***
void WifiManger::setupNormal(const char* ssid, const char* passwd) {
    if (!wifiInitd) {
        wifiInitd       = true;
        wifiInitdNormal = true;
        protectedSetupNormal(ssid, passwd);
    } else {
        if (wifiInitdMaint) {
            guardBlockMSG("setupMaint");
        } else if (wifiInitdFail) {
            guardBlockMSG("setupFailSafe");
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
            guardBlockMSG("setupNormal");
        } else if (wifiInitdFail) {
            guardBlockMSG("setupFailSafe");
        } else {
            guardMSG();
        }
    }
}

void WifiManger::setupFailsafe(const char* ssidAP, const char* passwdAP, const char* ssidSTA,
                               const char* passwdSTA) {
    if (!wifiInitd) {
        wifiInitd     = true;
        wifiInitdFail = true;
        protectedSetupFail(ssidAP, passwdAP, ssidSTA, passwdSTA);
    } else {
        if (wifiInitdNormal) {
            guardBlockMSG("setupNormal");
        } else if (wifiInitdMaint) {
            guardBlockMSG("setupMaint");
        } else {
            guardMSG();
        }
    }
}

// *** protected wifi implementation ***
void WifiManger::protectedSetupNormal(const char* ssid, const char* passwd) {
    logger.debug(MODULE_NAME, "setting to STA");

    const bool shouldConnect = (ssid != nullptr && ssid[0] != '\0');

    init();
    esp_netif_create_default_wifi_sta();
    esp_wifi_set_mode(WIFI_MODE_STA);

    if (shouldConnect) {
        wifi_config_t wifiConfig = {};
        copyField(wifiConfig.sta.ssid, sizeof(wifiConfig.sta.ssid), ssid);
        copyField(wifiConfig.sta.password, sizeof(wifiConfig.sta.password), passwd);
        esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
    }

    esp_wifi_start();

    if (shouldConnect) {
        esp_wifi_connect();
    } else {
        logger.info(MODULE_NAME, "STA started without connecting");
    }
}

void WifiManger::protectedSetupMaint(const char* ssid, const char* passwd) {
    logger.debug(MODULE_NAME, "setting to AP");

    init();
    esp_netif_create_default_wifi_ap();

    wifi_config_t wifiConfig = {};

    strncpy((char*)wifiConfig.ap.ssid, ssid, sizeof(wifiConfig.ap.ssid));
    wifiConfig.ap.ssid_len = strlen(ssid);
    strncpy((char*)wifiConfig.ap.password, passwd, sizeof(wifiConfig.ap.password));
    wifiConfig.ap.max_connection = 4;
    wifiConfig.ap.authmode       = (strlen(passwd) == 0) ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifiConfig);

    esp_wifi_start();

    logger.info(MODULE_NAME, "started AP");
}

void WifiManger::protectedSetupFail(const char* ssidAP, const char* passwdAP, const char* ssidSTA,
                                    const char* passwdSTA) {
    logger.debug(MODULE_NAME, "setting to AP+STA");

    const bool shouldConnect = (ssidSTA != nullptr && ssidSTA[0] != '\0');

    init();
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    esp_wifi_set_mode(WIFI_MODE_APSTA);

    if (shouldConnect) {
        wifi_config_t staConfig = {};
        copyField(staConfig.sta.ssid, sizeof(staConfig.sta.ssid), ssidSTA);
        copyField(staConfig.sta.password, sizeof(staConfig.sta.password), passwdSTA);
        esp_wifi_set_config(WIFI_IF_STA, &staConfig);
    }

    wifi_config_t apConfig = {};
    strncpy((char*)apConfig.ap.ssid, ssidAP, sizeof(apConfig.ap.ssid));
    apConfig.ap.ssid_len = strlen(ssidAP);
    strncpy((char*)apConfig.ap.password, passwdAP, sizeof(apConfig.ap.password));
    apConfig.ap.max_connection = 4;
    apConfig.ap.authmode       = (strlen(passwdAP) == 0) ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;
    esp_wifi_set_config(WIFI_IF_AP, &apConfig);

    esp_wifi_start();

    if (shouldConnect) {
        esp_wifi_connect();
    }

    logger.info(MODULE_NAME, "started AP+STA");
}
#undef MODULE_NAME

WifiManger wifi;
