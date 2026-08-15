#pragma once

#include "../logs/logger.h"
#include "./core.h"

#define retry(maxRetries, delayMS, ...) \
    helper.retryHelper(RETRY_NAME, __FUNCTION__, maxRetries, delayMS, __VA_ARGS__)

class HelperUtil {
  public:
    void retryHelper(const char* file, const char* function, int maxRetries, int delayMS,
                     const std::function<bool()>& action) {
        for (int i = 1; i <= maxRetries; i++) {
            if (action()) {
                logger.debug("helper", "function \"%s.%s\" succeeded", file, function);
                return;
            }
            logger.warn("helper", "function \"%s.%s\" failed %i/%i times", file, function, i,
                        maxRetries);

            if (i < maxRetries) {
                delay(delayMS);
            }
        }
        logger.crit("helper", "function \"%s.%s\" failed %i times!", file, function, maxRetries);
        core.setMode(SystemMode::FAILSAFE);
        logger.warn("core", "Rebooting to FAILSAFE!");
        ESP.restart();
    }
};
inline HelperUtil helper;
