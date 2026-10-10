#pragma once

class AethrOTA {
  public:
    void setup();
    void verifyBoot();
    void confirmBoot();

  private:
    bool otaInitd      = false;
    bool bootVerified  = false;
    bool confirmedBoot = false;

    void protectedSetup();
    void protectedVerifyBoot();
    void protectedConfirmBoot();
};
extern AethrOTA aethr;
