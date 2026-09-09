Import("env")

# fix pio reporting bad/diff offset
Import("env")

# Set partition table offset to 0xC000 for 40KB Secure Boot v2 bootloader
env.Replace(PARTITION_TABLE_OFFSET="0xC000")

def fix_partition_offset(target, source, env):
    if "FLASH_EXTRA_IMAGES" in env:
        env["FLASH_EXTRA_IMAGES"] = [
            ("0xC000", path) if addr == "0x8000" else (addr, path)
            for addr, path in env["FLASH_EXTRA_IMAGES"]
        ]

env.AddPreAction("$BUILD_DIR/${PROGNAME}.bin", fix_partition_offset)
