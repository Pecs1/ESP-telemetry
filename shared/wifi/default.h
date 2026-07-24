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
    void setupNormal(const char* ssid = nullptr, const char* passwd = nullptr);
    void setupMaint(const char* ssid, const char* passwd = nullptr);
    void setupFailsafe(const char* ssid, const char* passwd = nullptr);
};
extern WifiManger wifi;
