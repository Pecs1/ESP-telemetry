# ESP-Telemetry

Project to send telemetry data to a [ESP-Website](https://github.com/Pecs1/ESP-Website) for [Shell eco-marathon](https://www.shellecomarathon.com/) competition

## Prerequisites

### Make sure you have installed these external libraries:

- [**DallasTemperature**](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [**OneWire**](https://github.com/PaulStoffregen/OneWire)
- [**ESP32-OTA**](https://github.com/scottchiefbaker/ESP-WebOTA) (WebOTA)

### Microcontrollers:

Used boards in these directories
- `main`:     not decided yet :/
- `temps`:  **ESP32-WROOM-32U**
- `GPS`:    **ESP32-WROOM-32U**
- `R-LoRa`: **LilyGO TTGO LoRa32**
- `S-LoRa`: **LilyGO TTGO LoRa32**


### Sensors:

- **DS18B20** ➔ Temperature
- **SparkFun GPS NEO-M9N, U.FL (Qwiic)** ➔ GPS

## Developing

1. Navigate to a board directory
2. copy and remove the `.example` from `*.env.example.h`
     > for example `main.env.example.h` to `main.env.h`
3. Put your credentials into the `*.env.h`.

### Credits

This project incorporates logic and inspiration from various open-source contributors and friends within the Shell Eco-marathon community.


## License

This project is licensed under the [MPL-2.0](https://github.com/Pecs1/ESP-telemetry/blob/main/LICENSE)
