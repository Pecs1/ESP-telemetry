#pragma once

#include "./logs/logger.h"
#include "utils/guard.h"

#include <Preferences.h>
#include <cstdint>

#define MODULE_NAME "core"

// prefs
#define RW false // read-write
#define RO true  // read-only

inline constexpr const char* nvsName        = "sys_state";
inline constexpr const char* currentModeKey = "current_mode";
inline constexpr const char* nextModeKey    = "next_mode";

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
    void setup() {
        if (!initSetup) {
            initSetup = true;
            protectedSetup();
        } else {
            guardMSG();
        }
    }

    // checks individual keys for persistent storage if they exists
    //
    // - if a key doesnt exist, then it creates that key
    // - if a key does exist, then it does nothing
    void checkKeys() {
        if (initSetup && !checkedKeys) {
            checkedKeys = true;
            protectedCheckKeys();
        } else {
            guardMSG();
        }
    }

    // reads mode from persistent storage
    //
    // - should be used together with setMode()
    // - should be used to run mode specific code
    SystemMode readMode() {
        if (checkedKeys) {
            return protectedReadMode();
        } else {
            guardDepsMSG("core", "checkKeys");
            return SystemMode::UNKNOWN;
        }
    }

    // sets the mode used for the next reboot
    //
    // - should be used together with readMode()
    void setMode(SystemMode nextMode);

  private:
    Preferences nvs;

    // protection logic
    bool initSetup   = false;
    bool checkedKeys = false;

    // allow to run these function in code just once
    void protectedSetup();
    void protectedCheckKeys();

    // allow to run after checking keys
    SystemMode protectedReadMode();

    // helper to reduce code duplication
    template <typename Func>
    void checkKeyUtil(const char* key, Func&& arg) {
        if (nvs.isKey(key) == false) {
            logger.warn("prefs", "\"%s\" key not found", key);
            logger.debug("prefs", "creating \"%s\" key", key);

            arg(key);

            logger.info("prefs", "\"%s\" key created", key);
        }
    }

    // helper to print mode instead of uint value
    constexpr const char* stringify(uint8_t mode) {
        SystemMode m = static_cast<SystemMode>(mode);
        switch (m) {
            case SystemMode::NORMAL:
                return "NORMAL";
            case SystemMode::DEBUG:
                return "DEBUG";
            case SystemMode::MAINT:
                return "MAINT";
            case SystemMode::FAILSAFE:
                return "FAILSAFE";
            default:
                return "UNKNOWN";
        }
    }
};
#undef MODULE_NAME

extern CoreUtil core;
