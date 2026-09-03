#pragma once

#include "utils/guard.h"

#define MODULE_NAME "aethr"

class AethrOTA {
  public:
    void setup() {
        if (!otaInitd) {
            otaInitd = true;
            protectedSetup();
        } else {
            guardMSG();
        }
    }

    void verifyBoot() {
        if (otaInitd && !bootVerified) {
            bootVerified = true;
            protectedVerifyBoot();
        } else if (!otaInitd) {
            guardDepsMSG(MODULE_NAME, "setup()");
        } else {
            guardMSG();
        }
    }

    void confirmBoot() {
        if (bootVerified && confirmedBoot) {
            confirmedBoot = true;
            protectedConfirmBoot();
        } else if (!bootVerified) {
            guardDepsMSG(MODULE_NAME, "verifyBoot()");
        } else if (!otaInitd) {
            guardDepsMSG(MODULE_NAME, "setup()");
        } else {
            guardMSG();
        }
    }

  private:
    bool otaInitd      = false;
    bool bootVerified  = false;
    bool confirmedBoot = false;

    void protectedSetup();
    void protectedVerifyBoot();
    void protectedConfirmBoot();
};
#undef MODULE_NAME

extern AethrOTA aethr;
