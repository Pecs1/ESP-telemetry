#pragma once

#include "secrets/secrets.h"
#include "utils/guard.h"

#ifndef BOARD_NAME
#define BOARD_NAME "NAMELESS!"
#endif

#define MODULE_NAME "wifi"

// ssids for AP
inline const char* maintSSID    = TEAM_NAME " - Maintenance " BOARD_NAME;
inline const char* failsafeSSID = TEAM_NAME " - ERROR " BOARD_NAME;

class WifiManger {
  public:
    // sets wifi to WIFI_STA
    void setupNormal(const char* ssid = nullptr, const char* passwd = nullptr) {
        if (!wifiInitd) {
            wifiInitd       = true;
            wifiInitdNormal = true;
            protectedSetupNormal(ssid, passwd);
        } else {
            if (wifiInitdMaint) {
                guardBlockMSG(MODULE_NAME, "setupNormal");
            } else if (wifiInitdFail) {
                guardBlockMSG(MODULE_NAME, "setupMaint");
            } else {
                guardMSG();
            }
        }
    }
    // sets wifi to WIFI_AP
    void setupMaint(const char* ssid, const char* passwd = apPasswd) {
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

    // sets wifi to WIFI_AP_STA
    void setupFailsafe(const char* ssid, const char* passwd = apPasswd) {
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

  private:
    bool wifiInitd       = false;
    bool wifiInitdNormal = false;
    bool wifiInitdMaint  = false;
    bool wifiInitdFail   = false;

    void protectedSetupNormal(const char* ssid = nullptr, const char* passwd = nullptr);
    void protectedSetupMaint(const char* ssid, const char* passwd);
    void protectedSetupFail(const char* ssid, const char* passwd);
};
#undef MODULE_NAME

extern WifiManger wifi;
