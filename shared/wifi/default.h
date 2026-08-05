#pragma once

#include "../config.h"

#ifndef BOARD_NAME
#define BOARD_NAME "NAMELESS!"
#endif

// ssids for AP
inline const char* maintSSID    = TEAM_NAME " - Maintenance " BOARD_NAME;
inline const char* failsafeSSID = TEAM_NAME " - ERROR " BOARD_NAME;

class WifiManger {
  public:
    // sets wifi to WIFI_STA
    void setupNormal(const char* ssid = nullptr, const char* passwd = nullptr);

    // sets wifi to WIFI_AP
    void setupMaint(const char* ssid, const char* passwd = apPasswd);

    // sets wifi to WIFI_AP_STA
    void setupFailsafe(const char* ssid, const char* passwd = apPasswd);
};
extern WifiManger wifi;
