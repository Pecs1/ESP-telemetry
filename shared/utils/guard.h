#pragma once

#include "core/logs/logger.h"

// macros
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByModule, blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

#define guardBlockMSG(blockedByModule, blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

// stuff behind the scenes
inline void executeMSG(const char* file, const char* function) {
    logger.warn("core", "you can only use \"%s.%s\" once", file, function);
    logger.info("core", "skipping \"%s.%s\"...", file, function);
}

inline void executeDepsMSG(const char* calledFile, const char* calledFunction,
                           const char* blockedByFile, const char* blockedByFunction) {
    logger.warn("core", "you can use \"%s.%s\" after calling \"%s.%s\"", calledFile, calledFunction,
                blockedByFile, blockedByFunction);
    logger.info("core", "skipping \"%s.%s\"...", calledFile, calledFunction);
}

// similar to "executeDepsMSG", but diff message
inline void executeBlockMSG(const char* calledFile, const char* calledFunction,
                            const char* blockedByFile, const char* blockedByFunction) {
    logger.warn("core", "function \"%s.%s\" is blocked by \"%s.%s\"", calledFile, calledFunction,
                blockedByFile, blockedByFunction);
    logger.info("core", "skipping \"%s.%s\"...", calledFile, calledFunction);
}
