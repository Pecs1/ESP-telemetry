#pragma once

class WifiManger {
  public:
    void setupNormal();
    void setupMaint(const char* ssid, const char* passwd = nullptr);
    void setupFailsafe(const char* ssid, const char* passwd = nullptr);
};
extern WifiManger wifi;
