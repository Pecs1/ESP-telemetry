#pragma once

// set your passcodes/secrets in here:
#include "./secrets/secrets.h"

// for Preferences
#define RW false // read-write
#define RO true  // read-only

#ifndef BOARD_NAME
#define BOARD_NAME "NAMELESS!"
#endif

// ssids for AP
inline const char* maintSSID    = TEAM_NAME " - Maintenance " BOARD_NAME;
inline const char* failsafeSSID = TEAM_NAME " - ERROR " BOARD_NAME;
