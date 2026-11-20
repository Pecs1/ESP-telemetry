> [!Caution]
> **Pre-built CI binaries are signed with a private key that is not public.** Flashing release or CI-generated binaries will lock your device's Secure Boot to that key. Always generate your own `signature-key.pem` and build locally for development—private keys will **NOT** be provided to unbrick devices.

<div align="center">

# ESP-Telemetry

Project to send telemetry data to an [ESP-Website](https://github.com/Pecs1/ESP-Website) for the [Shell eco-marathon](https://www.shellecomarathon.com/) competition.

[![Lint check & Build](https://github.com/Pecs1/ESP-telemetry/actions/workflows/check.yml/badge.svg)](https://github.com/Pecs1/ESP-telemetry/actions/workflows/check.yml) [![Release](https://github.com/Pecs1/ESP-telemetry/actions/workflows/release.yml/badge.svg)](https://github.com/Pecs1/ESP-telemetry/actions/workflows/release.yml)

</div>

## Hardware & Dependencies

### Microcontrollers

Located in the [`/boards`](./boards) directory:

| Board | Microcontroller |
| :---: | :---: |
| [`example`](./boards/example/main.cpp) | *template/example* |
| [`main`](./boards/main/main.cpp) | *to be added* |
| [`temps`](./boards/temps/main.cpp) | **ESP32-WROOM-32U** |
| [`gps`](./boards/gps/main.cpp) |  **ESP32-WROOM-32U** |
| [`r-lora`](./boards/r-lora/main.cpp) | **ESP32-S3-LR1121-HF-Kit** |
| [`s-lora`](./boards/s-lora/main.cpp) | **ESP32-S3-LR1121-HF-Kit** |


### Sensors

| Sensor | Function |
| :---: | :---: |
| **DS18B20** | Temperature |
| **SparkFun GPS NEO-M9N, U.FL (Qwiic)** | GPS |


### External Library Dependencies

> [!Note]
> Dependencies are automatically installed by pioarduino via `platformio.ini`.

- [**DallasTemperature**](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [**OneWire**](https://github.com/PaulStoffregen/OneWire)

---

## Prerequisites

It's best to use **VS Code** or **VSCodium** with the following required extensions:

* [clangd](https://open-vsx.org/vscode/item?itemName=llvm-vs-code-extensions.vscode-clangd)
* [pioarduino IDE](https://open-vsx.org/vscode/item?itemName=pioarduino.pioarduino-ide)

> [!Important]
> Don't forget to configure pioarduino IDE settings to use `clangd` as the IntelliSense Engine.

---

## Setup & Development

Follow these steps sequentially to set up and start developing on the project.

### Step 1: Creating Signature Key

> [!Important]
> Creating signature key is a mandatory initial setup step. You have to run this just once though.

```bash
# 1. clone the repo and enter it
git clone https://github.com/Pecs1/ESP-telemetry.git && cd ESP-telemetry

# 2. generate signature key for secure boot
# note: you might have to wait till pioarduino downloads all dependencies
~/.platformio/penv/bin/espsecure generate-signing-key --version 2 --scheme rsa3072 ./system/signature-key.pem
```

### Step 2: Developing & Building

> [!Note]
> The `example` board is regularly updated with new features and fixes.

1. Ensure you have the [required prerequisites](#prerequisites) installed and extensions active.
2. **Explore the example project:** Open [/boards/example/main.cpp](./boards/example/main.cpp). 
3. **Read the file header:** Please read the notice at the top of `main.cpp`. It's there for you!

---

<div align="center">

## License

This project is licensed under the [MPL-2.0](https://github.com/Pecs1/ESP-telemetry/blob/main/LICENSE).

</div>
