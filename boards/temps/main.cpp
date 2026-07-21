//    FILE: address_array.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: use address array of multiple DS18B20
//     URL: https://github.com/milesburton/Arduino-Temperature-Control-Library

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 25

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup() {
  // start serial port
  Serial.begin(115200);
  delay(1000);
  Serial.println("System Started");

  sensors.begin();
}


void loop() {
  sensors.requestTemperatures();
  // argument 1 = sensor address variable, argument 2 = printed sensor name
  printSensorTemperature(sensor_1, "Sensor#1");

  printSensorTemperature(sensor_2, "Sensor#2");
  delay(5000);  // wait 5 seconds before next print
}

// function to print sensor temperature
void printSensorTemperature(DeviceAddress sensor_x, String sensor_name) {
  printAddress(sensor_x);
  Serial.println();
  Serial.print("Temperature " + sensor_name + ": ");
  Serial.print(sensors.getTempC(sensor_x), 2);  // print with 2 decimals
  Serial.println("°C");
  Serial.println();
}

// function to print sensor address
void printAddress(DeviceAddress sensor_x) {
  Serial.print("Device address: ");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");

    if (sensor_x[i] < 16) {
      Serial.print("0");
    }

    Serial.print(sensor_x[i], HEX);
    if (i < 7) {
      Serial.print(", ");
    }
  }
}
