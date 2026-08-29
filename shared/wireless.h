#pragma once

// just a wrapper for you
// for cleaner include ^^

#ifdef WIRELESS_USE_ALL
#define WIRELESS_USE_WIFI
#define WIRELESS_USE_ESPNOW
// #define WIRELESS_USE_LORA
#endif

#if !defined(WIRELESS_USE_WIFI) && !defined(WIRELESS_USE_ESPNOW) // && !defined(WIRELESS_USE_LORA)
#error "wireless.h: no protocol defined"
#warning You must define WIRELESS_USE_WIFI, WIRELESS_USE_ESPNOW, WIRELESS_USE_LORA, or WIRELESS_USE_ALL before including this file.
#endif

#if defined(WIRELESS_USE_WIFI) || defined(WIRELESS_USE_ESPNOW)
#include "wireless/wifi/manager.h"
#endif

#ifdef WIRELESS_USE_ESPNOW
#include "wireless/espnow/util.h"
#endif

// #ifdef WIRELESS_USE_LORA
// #include "wireless/lora/"
// #endif
