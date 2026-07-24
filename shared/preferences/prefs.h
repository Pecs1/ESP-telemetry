#pragma once

#include "../logs/logger.h"

#include <Preferences.h>

class PrefsUtil {
  public:
    void check();

  private:
    Preferences nvs;

    template <typename Func>
    void checkUtil(const char* key, Func&& arg) {
        if (nvs.isKey(key) == false) {
            logger.warn("prefs", "\"%s\" key not found", key);
            logger.debug("prefs", "creating \"%s\" key", key);

            arg(key);

            logger.info("prefs", "\"%s\" key created", key);
        }
    }
};
extern PrefsUtil prefs;
