#pragma once

#include <cstdint>

enum class SystemMode : uint8_t {
    NORMAL   = 0,
    DEBUG    = 1,
    MAINT    = 2,
    FAILSAFE = 3,
    UNKNOWN  = 4
};

class CoreUtil {
  public:
    // starts up Serial
    //
    // - used to restrict some functions to setup only
    void setup();

    // checks individual keys for persistent storage if they exists
    //
    // - if a key doesnt exist, then it creates that key
    // - if a key does exist, then it does nothing
    void checkKeys();

    // reads mode from persistent storage
    //
    // - should be used together with setMode()
    // - should be used to run mode specific code
    SystemMode readMode();

    // sets the mode used for the next reboot
    //
    // - should be used together with readMode()
    void setMode(SystemMode nextMode);

  private:
    // protection logic
    bool initSetup   = false;
    bool checkedKeys = false;

    // allow to run these function in code just once
    void protectedSetup();
    void protectedCheckKeys();
    SystemMode protectedReadMode();
    void protectedSetMode(SystemMode nextMode);
};
extern CoreUtil core;
