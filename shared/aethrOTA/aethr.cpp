#include "./aethr.h"

#include "utils/logger.h"

#include <esp_http_server.h>
#include <esp_ota_ops.h>

#define MODULE_NAME "aethr"

void AethrOTA::protectedSetup() {
    logger.debug(MODULE_NAME, "starting...");
}

void AethrOTA::protectedVerifyBoot() {}

void AethrOTA::protectedConfirmBoot() {
    logger.debug(MODULE_NAME, "marking OTA image as valid boot");
    esp_ota_mark_app_valid_cancel_rollback();
    logger.info(MODULE_NAME, "marked OTA image as valid");
}

#undef MODULE_NAME

AethrOTA aethr;
