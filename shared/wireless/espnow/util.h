#pragma once

#include <cstdint>

class EspNowUtil {
  public:
    // check if esp_now was initiated successfully
    void init();

    // check if peer was added successfully
    void registerPeer(const uint8_t* address, uint8_t channel = 0, bool encrypt = false);

  private:
    bool espnowInitd = false;

    void protectedInit();
    void protectedRegisterPeer(const uint8_t* address, uint8_t channel, bool encrypt);
};
extern EspNowUtil espnow;
