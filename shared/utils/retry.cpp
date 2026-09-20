#include "retry.h"

#include "core.h"
#include "utils/logger.h"

#include <Arduino.h>

#define MODULE_NAME "retry"

void retryExecutor(const char* file, const char* function, int maxRetries, int delayMS, RetryFn fn,
                   void* userCtx) {
    for (int i = 1; i <= maxRetries; i++) {
        if (fn(userCtx)) {
            logger.debug(MODULE_NAME, "function \"%s.%s\" succeeded", file, function);
            return;
        }
        logger.err(MODULE_NAME, "function \"%s.%s\" failed %i/%i times", file, function, i,
                   maxRetries);

        if (i < maxRetries) {
            delay(delayMS);
        }
    }
    logger.crit(MODULE_NAME, "function \"%s.%s\" failed %i times!", file, function, maxRetries);
    core.setMode(SystemMode::FAILSAFE);
    logger.warn("core", "Rebooting to FAILSAFE!");
    ESP.restart();
}
