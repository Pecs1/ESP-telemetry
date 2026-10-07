Import("env")
import os

project_dir = env.get("PROJECT_DIR")
pio_env = env.get("PIOENV")

print(f"--- [SDKConfig Handler] Running for environment: {pio_env} ---")

# Get MCU target (e.g., 'esp32', 'esp32s3')
board_config = env.BoardConfig()
mcu = board_config.get("build.mcu", "esp32")

# Separate sdkconfig path by MCU chip type
build_dir = os.path.join(project_dir, ".pio", "sdk", mcu)
os.makedirs(build_dir, exist_ok=True)

target_sdkconfig = os.path.join(build_dir, "sdkconfig")

# Force PlatformIO/ESP-IDF to use the MCU-specific sdkconfig path
board_config.update("build.esp-idf.sdkconfig_path", target_sdkconfig)
