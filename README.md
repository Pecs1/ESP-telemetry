<div align="center">

# ESP-Telemetry

Project to send telemetry data to a [ESP-Website](https://github.com/Pecs1/ESP-Website) for [Shell eco-marathon](https://www.shellecomarathon.com/) competition

</div>

> [!Important]
> I'll be slowly migrating to pioarduino, so expect some breaking changes

## Prerequisites

Make sure you have **[pioarduino](https://github.com/pioarduino)** installed

### Dependencies

> [!Note]
> Dependencies are automatically managed by pioarduino via `platformio.ini`.

- [**DallasTemperature**](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [**OneWire**](https://github.com/PaulStoffregen/OneWire)

### Microcontrollers

Used boards in `/boards` directory


| Board | Microcontroller |
| :---: | :---: |
| [`main`](./boards/main/main.cpp) | *to be added* |
| [`temps`](./boards/temps/main.cpp) | **ESP32-WROOM-32U** |
| [`gps`](./boards/gps/main.cpp) |  **ESP32-WROOM-32U** |
| [`R-LoRa`](./boards/R-LoRa/main.cpp) | **LilyGO TTGO LoRa32** |
| [`S-LoRa`](./boards/S-LoRa/main.cpp) | **LilyGO TTGO LoRa32** |


### Sensors

| Sensor | Function |
| :---: | :---: |
| **DS18B20** | Temperature |
| **SparkFun GPS NEO-M9N, U.FL (Qwiic)** | GPS |

## Developing

> [!Note]
> The `example` board is regularly updated with new features and fixes.

### Getting Started

1. **Install pioarduino**
2. **Explore the example project:** Open [/boards/example/main.cpp](./boards/example/main.cpp). 
3. **Read the file header:** Please read the notice at the top of `main.cpp`. It's there for you!


## License

<div align="center">

This project is licensed under the [MPL-2.0](https://github.com/Pecs1/ESP-telemetry/blob/main/LICENSE)

</div>
