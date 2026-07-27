#pragma once

#include "../logs/logger.h"

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
    void setup();
    void checkKeys();

    // reads mode from persistent storage
    SystemMode readMode();


  private:
    Preferences nvs;

    template <typename Func>
    void checkKeyUtil(const char* key, Func&& arg) {
        if (nvs.isKey(key) == false) {
            logger.warn("prefs", "\"%s\" key not found", key);
            logger.debug("prefs", "creating \"%s\" key", key);

            arg(key);

            logger.info("prefs", "\"%s\" key created", key);
        }
    }

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
