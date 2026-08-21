#pragma once

#include "utils/guard.h"

#include <cstdint>

#define MODULE_NAME "espnow"

class EspNowUtil {
  public:
    // check if esp_now was initiated successfully
    void init() {
        if (!espnowInitd) {
            espnowInitd = true;
            protectedInit();
        } else {
            guardMSG();
        }
    }

    // check if peer was added successfully
    void registerPeer(const uint8_t* address, uint8_t channel = 0, bool encrypt = false) {
        if (espnowInitd) {
            protectedRegisterPeer(address, channel, encrypt);
        } else {
            guardDepsMSG(MODULE_NAME, "init");
        }
    }

  private:
    bool espnowInitd = false;

    void protectedInit();
    void protectedRegisterPeer(const uint8_t* address, uint8_t channel, bool encrypt);
};
#undef MODULE_NAME

extern EspNowUtil espnow;
