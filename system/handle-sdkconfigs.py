Import("env")
import os

project_dir = env.get("PROJECT_DIR")
pio_env = env.get("PIOENV")

print(f"--- [SDKConfig Handler] Running for environment: {pio_env} ---")

# Get MCU target (e.g., 'esp32', 'esp32s3')
board_config = env.BoardConfig()
mcu = board_config.get("build.mcu", "esp32")

# Set ccache environment variables to normalize paths across build folders
os.environ["CCACHE_BASEDIR"] = project_dir
os.environ["CCACHE_SLOPPINESS"] = "time_macros,include_file_mtime,include_file_ctime,file_stat_matches"

# Separate sdkconfig path by MCU chip type
build_dir = os.path.join(project_dir, ".pio", "sdk", mcu)
os.makedirs(build_dir, exist_ok=True)

target_sdkconfig = os.path.join(build_dir, "sdkconfig")

# Force PlatformIO/ESP-IDF to use the MCU-specific sdkconfig path
board_config.update("build.esp-idf.sdkconfig_path", target_sdkconfig)

# Inject ccache launchers into CMake arguments
existing_args = board_config.get("build.cmake_extra_args", "")
ccache_flags = "-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"

if "CMAKE_C_COMPILER_LAUNCHER" not in existing_args:
    board_config.update(
        "build.cmake_extra_args", 
        f"{ccache_flags} {existing_args}".strip()
    )
