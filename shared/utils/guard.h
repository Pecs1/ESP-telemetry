#pragma once

#include "utils/logger.h"

#define MODULE_NAME "guard"

// macros
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByModule, blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

#define guardBlockMSG(blockedByModule, blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

// stuff behind the scenes
inline void executeMSG(const char* module, const char* function) {
    logger.err(MODULE_NAME, "you can only use \"%s.%s\" once", module, function);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", module, function);
}

inline void executeDepsMSG(const char* calledModule, const char* calledFunction,
                           const char* blockedByFile, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "you can use \"%s.%s\" after calling \"%s.%s\"", calledModule,
               calledFunction, blockedByFile, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledModule, calledFunction);
}

// similar to "executeDepsMSG", but diff message
inline void executeBlockMSG(const char* calledModule, const char* calledFunction,
                            const char* blockedByFile, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "function \"%s.%s\" is blocked by \"%s.%s\"", calledModule,
               calledFunction, blockedByFile, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledModule, calledFunction);
}
#undef MODULE_NAME
