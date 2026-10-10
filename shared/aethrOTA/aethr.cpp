#include "./aethr.h"

#include "utils/guard.h"
#include "utils/logger.h"

#include <esp_http_server.h>
#include <esp_ota_ops.h>

#define MODULE_NAME "aethr"

// *** protect functions ***
void AethrOTA::setup() {
    if (!otaInitd) {
        otaInitd = true;
        protectedSetup();
    } else {
        guardMSG();
    }
}

void AethrOTA::verifyBoot() {
    if (otaInitd && !bootVerified) {
        bootVerified = true;
        protectedVerifyBoot();
    } else if (!otaInitd) {
        guardDepsMSG("setup()");
    } else {
        guardMSG();
    }
}

void AethrOTA::confirmBoot() {
    if (bootVerified && !confirmedBoot) {
        confirmedBoot = true;
        protectedConfirmBoot();
    } else if (!bootVerified) {
        guardDepsMSG("verifyBoot()");
    } else if (!otaInitd) {
        guardDepsMSG("setup()");
    } else {
        guardMSG();
    }
}

// *** protected functions ***
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
