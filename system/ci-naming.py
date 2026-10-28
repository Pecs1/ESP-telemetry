#!/usr/bin/env python3
import json, re, sys
from platformio.project.config import ProjectConfig

SKIP = {"example", "testing-ground"}

def defines(value):
    # get() can return a str or a list depending on the option
    if isinstance(value, (list, tuple)):
        value = "\n".join(value)
    return dict(re.findall(r"-D\s*([A-Za-z_]\w*)=(\S+)", value or ""))

cfg = ProjectConfig.get_instance("platformio.ini")
result = {}
for env in cfg.envs():
    if env in SKIP:
        continue
    section = f"env:{env}"
    cmake = defines(cfg.get(section, "board_build.cmake_extra_args", ""))
    flags = defines(cfg.get(section, "build_flags", ""))
    result[env] = {
        "env": env,
        "path": cmake.get("BOARD_ENV_PATH"),
        "version": cmake.get("PROJECT_VER"),
        "lib_abi": flags.get("LIB_ABI_VER"),
    }

# `python read_versions.py gps` -> one env; no args -> JSON list for a matrix
if len(sys.argv) > 1:
    print(json.dumps(result[sys.argv[1]]))
else:
    print(json.dumps(list(result.values())))
