#pragma once

#include <cstdint>
#include <esp_now.h>

class EspNowUtil {
  public:
    // check if esp_now was initiated successfully
    void init();

    // check if peer was added successfully
    void registerPeer(const uint8_t* address, uint8_t channel = 0, bool encrypt = false);
};
extern EspNowUtil espnow;
