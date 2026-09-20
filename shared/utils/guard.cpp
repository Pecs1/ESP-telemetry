#include "guard.h"

#include "utils/logger.h"

#define MODULE_NAME "guard"

void executeMSG(const char* module, const char* function) {
    logger.err(MODULE_NAME, "you can only use \"%s.%s\" once", module, function);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", module, function);
}

void executeDepsMSG(const char* calledModule, const char* calledFunction,
                    const char* blockedByModule, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "you can use \"%s.%s\" after calling \"%s.%s\"", calledModule,
               calledFunction, blockedByModule, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledModule, calledFunction);
}

void executeBlockMSG(const char* calledModule, const char* calledFunction,
                     const char* blockedByModule, const char* blockedByFunction) {
    logger.err(MODULE_NAME, "function \"%s.%s\" is blocked by \"%s.%s\"", calledModule,
               calledFunction, blockedByModule, blockedByFunction);
    logger.warn(MODULE_NAME, "skipping \"%s.%s\"...", calledModule, calledFunction);
}
