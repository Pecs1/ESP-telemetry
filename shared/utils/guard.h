#pragma once

// macros
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByModule, blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

#define guardBlockMSG(blockedByModule, blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

// functions
void executeMSG(const char* module, const char* function);

void executeDepsMSG(const char* calledModule, const char* calledFunction,
                    const char* blockedByModule, const char* blockedByFunction);

void executeBlockMSG(const char* calledModule, const char* calledFunction,
                     const char* blockedByModule, const char* blockedByFunction);
