#pragma once

// macros
#define guardMSG() executeMSG(MODULE_NAME, __FUNCTION__)

#define guardDepsMSG(blockedByFunction) executeDepsMSG(MODULE_NAME, __FUNCTION__, blockedByFunction)

#define guardBlockMSG(blockedByFunction) \
    executeBlockMSG(MODULE_NAME, __FUNCTION__, blockedByFunction)

// functions
void executeMSG(const char* module, const char* function);

void executeDepsMSG(const char* module, const char* calledFunction, const char* blockedByFunction);

void executeBlockMSG(const char* module, const char* calledFunction, const char* blockedByFunction);
