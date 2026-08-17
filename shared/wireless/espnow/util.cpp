#include "./util.h"

#include "core.h"
#include "utils/retry.h"

#include <esp_now.h>

#define MODULE_NAME "espnow"

esp_now_peer_info_t peerInfo;

namespace {
    void onDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
        logger.info("espnow", "Packet has %s",
                    status == ESP_NOW_SEND_SUCCESS ? "succeeded" : "failed");
    }
} // namespace

void EspNowUtil::protectedInit() {
    retry(3, 2000, [&]() {
        // check if esp_now was initiated successfully
        if (esp_now_init() != ESP_OK) {
            logger.err("espnow", "failed to init");
            return false;
        }
        return true;
    });
    logger.info("espnow", "Successfuly initiated");

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
    logger.info("espnow", "Successfuly registered/added peer");
}
#undef MODULE_NAME

EspNowUtil espnow;
