#pragma once

#include "core/logs/logger.h"

#define MODULE_NAME "guard"

// macros
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByModule, blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

#define guardBlockMSG(blockedByModule, blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

// stuff behind the scenes
inline void executeMSG(const char* file, const char* function) {
    logger.err(MODULE_NAME, "you can only use \"%s.%s\" once", file, function);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", file, function);
}

inline void executeDepsMSG(const char* calledFile, const char* calledFunction,
                           const char* blockedByFile, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "you can use \"%s.%s\" after calling \"%s.%s\"", calledFile,
                calledFunction, blockedByFile, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledFile, calledFunction);
}

// similar to "executeDepsMSG", but diff message
inline void executeBlockMSG(const char* calledFile, const char* calledFunction,
                            const char* blockedByFile, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "function \"%s.%s\" is blocked by \"%s.%s\"", calledFile,
                calledFunction, blockedByFile, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledFile, calledFunction);
}
#undef MODULE_NAME
