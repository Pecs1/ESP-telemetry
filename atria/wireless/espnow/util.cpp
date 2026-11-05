#include "./util.h"

#include "atria/utils/guard.h"
#include "atria/utils/logger.h"
#include "atria/utils/retry.h"
#include "atria/wireless/wifi/manager.h"

#include <cstring>
#include <esp_now.h>

#define MODULE_NAME "espnow"

esp_now_peer_info_t peerInfo;

namespace {
    void onDataSent(const esp_now_send_info_t* info, esp_now_send_status_t status) {
        logger.info(MODULE_NAME, "Packet has %s",
                    status == ESP_NOW_SEND_SUCCESS ? "succeeded" : "failed");
    }
} // namespace

// *** protect functions to run only once ***
void EspNowUtil::init() {
    bool sta = wifi.isInSTA();

    // checks and returns if wifi.setupNormal or wifi.setupFailSafe wasnt called yet
    if (!sta) {
        return;
    }

    // check and return if init was called already
    if (espnowInitd) {
        guardMSG();
        return;
    }
    espnowInitd = true;
    protectedInit();
}

void EspNowUtil::registerPeer(const uint8_t* address, uint8_t channel, bool encrypt) {
    // check and return if init wasnt called yet
    if (!espnowInitd) {
        guardDepsMSG("init");
        return;
    }
    protectedRegisterPeer(address, channel, encrypt);
}

// *** protected esp_now implementation ***
void EspNowUtil::protectedInit() {
    retry(3, 2000, [&]() {
        // check if esp_now was initiated successfully
        if (esp_now_init() != ESP_OK) {
            logger.err(MODULE_NAME, "failed to init");
            return false;
        }
        return true;
    });
    logger.info(MODULE_NAME, "Successfuly initiated");

    esp_now_register_send_cb(esp_now_send_cb_t(onDataSent));
}

void EspNowUtil::protectedRegisterPeer(const uint8_t* address, uint8_t channel, bool encrypt) {
    peerInfo.channel = channel;
    peerInfo.encrypt = encrypt;

    // copy to memory
    memcpy(peerInfo.peer_addr, address, 6);

    retry(3, 2000, [&]() {
        // check if peer was added successfully
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
            return false;
        }
        return true;
    });
    logger.info(MODULE_NAME, "Successfuly registered/added peer");
}
EspNowUtil espnow;
