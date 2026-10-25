#pragma once
#include <cstdint>

#ifndef LIB_ABI_VER
    #define LIB_ABI_VER 0
#endif

#define ABI_DESC_MAGIC 0x00494241u

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint8_t lib_abi;
} esp_custom_lib_desc_t;

extern const esp_custom_lib_desc_t custom_lib_desc;
