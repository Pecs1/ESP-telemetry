Import("env")
import os
import shutil
import sys

project_dir = env.subst("$PROJECT_DIR")
core_dir = env.subst("$PROJECT_CORE_DIR")
key_rel = os.getenv("SIGNING_KEY_PATH") or env.GetProjectOption("board_build.signing_key", "")

# Locate PlatformIO's penv
penv_win = os.path.join(core_dir, "penv", "Scripts", "python.exe")
penv_posix = os.path.join(core_dir, "penv", "bin", "python")

if os.path.isfile(penv_win):
    python_exe = penv_win
elif os.path.isfile(penv_posix):
    python_exe = penv_posix
else:
    python_exe = sys.executable

# Locate espsecure inside tool-esptoolpy package
platform = env.PioPlatform()
esptool_dir = platform.get_package_dir("tool-esptoolpy")

espsecure_py = os.path.join(esptool_dir, "espsecure") if esptool_dir else None

if espsecure_py and os.path.isfile(espsecure_py):
    espsecure_cmd = f'"{python_exe}" "{espsecure_py}"'
else:
    espsecure_cmd = f'"{python_exe}" -m espsecure'

def abort(msg):
    sys.stderr.write(f"\n[ERROR] {msg}\n\n")
    env.Exit(1)

if key_rel:
     key_path = os.path.join(project_dir, key_rel)
     if not os.path.isfile(key_path):
          abort(f"Signing key missing at: {key_path}")

     def sign_bin(target, source, env):
          target_path = str(target[0])
          filename = os.path.basename(target_path)
          temp_signed_path = target_path + ".signed"

          print(f"\n[INFO] Signing {filename}...")

          # Sign to temporary output file
          cmd = f'{espsecure_cmd} sign-data --version 2 -k "{key_path}" -o "{temp_signed_path}" "{target_path}"'

          if env.Execute(cmd) != 0:
               if os.path.exists(temp_signed_path):
                    os.remove(temp_signed_path)
               abort(f"espsecure failed to sign {filename}.")

          # Verify the generated signature against the key
          verify_cmd = f'{espsecure_cmd} verify-signature --version 2 --keyfile "{key_path}" "{temp_signed_path}"'

          if env.Execute(verify_cmd) != 0:
               if os.path.exists(temp_signed_path):
                    os.remove(temp_signed_path)
               abort(f"espsecure signature verification failed for {filename}.")

          # Replace original file with signed file
          shutil.move(temp_signed_path, target_path)
          print(f"[SUCCESS] {filename} successfully signed!\n")

     # Attach signing hooks
     env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", sign_bin)
     env.AddPostAction("$BUILD_DIR/partitions.bin", sign_bin)
