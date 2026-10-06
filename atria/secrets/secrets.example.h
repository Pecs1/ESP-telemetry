#pragma once

#include <cstdint>

// team name for every board
// i mean... its not a secret really... but shhh
#define TEAM_NAME "Pecs1"

// set your 4 digit passcode for changing modes
inline const uint16_t maintPasscode = 1234;

// password for WiFi in AP mode
inline const char* apPasswd = "SpecialPasswd"; // must be more than 7 characters

// board MACs
inline uint8_t mainAddress[6]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
inline uint8_t gpsAddress[6]   = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
inline uint8_t sloraAddress[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
inline uint8_t tempsAddress[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// temp sensors
inline uint8_t sensor1[8] = { 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
inline uint8_t sensor2[8] = { 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
