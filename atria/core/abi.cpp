#include "abi.h"

static_assert(LIB_ABI_VER >= 1 && LIB_ABI_VER <= 255,
              "LIB_ABI_VER must be set via build flags (1-255)");

const
    __attribute__((section(".rodata_custom_desc"), used)) esp_custom_lib_desc_t custom_lib_desc = {
        .magic   = ABI_DESC_MAGIC,
        .lib_abi = LIB_ABI_VER,
    };
