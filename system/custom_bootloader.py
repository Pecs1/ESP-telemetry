Import("env")
import os
import shutil
import sys

# --- 1. Custom Bootloader Injection ---
bootloader_rel_path = env.GetProjectOption("board_build.bootloader_bin", None)

if bootloader_rel_path:
    custom_bootloader = os.path.join(env.subst("$PROJECT_DIR"), bootloader_rel_path)

    if not os.path.isfile(custom_bootloader):
        sys.stderr.write(f"\n[ERROR] Custom bootloader binary not found: {custom_bootloader}\n\n")
        env.Exit(1)

    target_bootloader = os.path.join(env.subst("$BUILD_DIR"), "bootloader.bin")

    def replace_bootloader_bin(target, source, env):
        shutil.copyfile(custom_bootloader, str(target[0]))
        print(f"\n[SUCCESS] Custom bootloader injected -> {target[0]}\n")

    env.AddPostAction(target_bootloader, replace_bootloader_bin)
