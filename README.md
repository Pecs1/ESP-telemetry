# ESP-Telemetry

Project to send telemetry data to a [ESP-Website](https://github.com/Pecs1/ESP-Website) for [Shell eco-marathon](https://www.shellecomarathon.com/) competition

## Prerequisites

### Make sure you have installed these libraries:

- ArduinoHttpClient
- DallasTemperature
- OneWire
- TinyGSM

#### Optionally:

- StreamDebugger


### Microcontrollers:

- `main_board.ino`: **LILYGO T-SIM7000G** (ESP32 + GSM/GPS)
- other `.ino` files: **ESP32-WROOM-32U**


### Sensors:

- **DS18B20** for temperature
- **WCS1500-1662** for current sensor

## Developing

1. Navigate to a board directory
2. copy and rename `env.example.h` to `env.h`
3. Put your credentials into the `env.h`.

### Credits

This project incorporates logic and inspiration from various open-source contributors and friends within the Shell Eco-marathon community.
