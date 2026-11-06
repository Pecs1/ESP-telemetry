#pragma once

// macros
// maybe i should rename this macros... maybe some day :")
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, MODULE_NAME, blockedByFunction)

#define guardBlockMSG(blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByFunction)

#define guardGlobDepsMSG(blockedByModule, blockedByFunction) \
    executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByModule, blockedByFunction)

// functions
void executeMSG(const char* module, const char* function);

void executeDepsMSG(const char* module, const char* calledFunction, const char* blockedByModule,
                    const char* blockedByFunction);

void executeBlockMSG(const char* module, const char* calledFunction, const char* blockedByFunction);
