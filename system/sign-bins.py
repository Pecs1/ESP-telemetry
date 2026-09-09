Import("env")
import os
import shutil
import sys

project_dir = env.subst("$PROJECT_DIR")
key_rel = os.getenv("SIGNING_KEY_PATH") or env.GetProjectOption("board_build.signing_key", "")

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
          cmd = f'espsecure sign-data -v 2 -k "{key_path}" -o "{temp_signed_path}" "{target_path}"'
          
          if env.Execute(cmd) != 0:
               if os.path.exists(temp_signed_path):
                    os.remove(temp_signed_path)
               abort(f"espsecure failed to sign {filename}.")
          
          # Replace original file with signed file
          shutil.move(temp_signed_path, target_path)
          print(f"[SUCCESS] {filename} successfully signed!\n")

     # Attach signing hooks
     env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", sign_bin)
     env.AddPostAction("$BUILD_DIR/partitions.bin", sign_bin)
