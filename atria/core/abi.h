#pragma once
#include <cstdint>

#ifndef LIB_ABI_VER
    #define LIB_ABI_VER 0
#endif

inline constexpr uint32_t ABI_DESC_MAGIC = 0x00494241u; // prints in ascii "ABI\0"

struct __attribute__((packed)) DescLibABI {
    uint32_t magic;
    uint8_t lib_abi;
};

static_assert(sizeof(DescLibABI) == 5, "descriptor layout changed");
static_assert(sizeof(DescLibABI) <= 16, "descriptor must fit the 16-byte slot");
static_assert(__builtin_offsetof(DescLibABI, lib_abi) == 4, "lib_abi offset changed");

extern const DescLibABI libDescABI;
