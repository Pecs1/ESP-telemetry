#pragma once

#include "./logs/logger.h"

#include <Preferences.h>
#include <cstdint>

#define RW false // read-write
#define RO true  // read-only

inline constexpr const char* nvsName        = "sys_state";
inline constexpr const char* currentModeKey = "current_mode";
inline constexpr const char* nextModeKey    = "next_mode";

enum class SystemMode : uint8_t {
    NORMAL   = 0,
    DEBUG    = 1,
    MAINT    = 2,
    FAILSAFE = 3
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
    Preferences nvs;

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
extern CoreUtil core;
