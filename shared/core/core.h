#pragma once

#include "../logs/logger.h"

#include <Preferences.h>

#define RW false // read-write
#define RO true  // read-only

inline constexpr const char* nvsName        = "sys_state";
inline constexpr const char* currentModeKey = "current_mode";

class CoreUtil {
  public:
    void setup();
    void checkKeys();

  private:
    Preferences nvs;

    template <typename Func>
    void checkKeyUtil(const char* key, Func&& arg) {
        if (nvs.isKey(key) == false) {
            logger.warn("core - prefs", "\"%s\" key not found", key);
            logger.debug("core - prefs", "creating \"%s\" key", key);

            arg(key);

            logger.info("core - prefs", "\"%s\" key created", key);
        }
    }
};
extern CoreUtil core;
