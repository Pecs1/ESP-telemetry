Import("env")
import os
import shutil
import sys

project_dir = env.subst("$PROJECT_DIR")
bootloader_rel = env.GetProjectOption("board_build.bootloader_bin", "")

def abort(msg):
    sys.stderr.write(f"\n[ERROR] {msg}\n\n")
    env.Exit(1)

if bootloader_rel:
    custom_bl = os.path.join(project_dir, bootloader_rel)
    if not os.path.isfile(custom_bl):
        abort(f"Custom bootloader not found: {custom_bl}")

    def inject_bootloader(target, source, env):
        target_path = str(target[0])
        shutil.copy2(custom_bl, target_path)
        print(f"\n[SUCCESS] Injected custom bootloader -> {target_path}")

    env.AddPostAction("$BUILD_DIR/bootloader.bin", inject_bootloader)
