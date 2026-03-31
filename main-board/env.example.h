#ifndef ENV_H
#define ENV_H

// Pinout
#define UART_BAUD    115200
#define PIN_DTR      25
#define PIN_TX       27
#define PIN_RX       26
#define PWR_PIN      4

#define LED_PIN      12

#define SD_MISO      2
#define SD_MOSI      15
#define SD_SCLK      14
#define SD_CS        13

#define BATT_PIN     35


// --- TIMING SETTINGS (ms) ---
#define INTERVAL_WIFI  2000
#define INTERVAL_GPS   10000

#define SERVER_TIMEOUT 5000


// --- BATTERY SETTINGS ---
#define BATT_MIN 3.4
#define BATT_MAX 4.2
#define BATT_CALIBRATION 1.1

// --- CELLULAR SETTINGS ---
static const char apn[]      = "your_APN_NAME"; // some carries dont require
static const char gprsUser[] = "your_APN_USER"; // some carries dont require
static const char gprsPass[] = "your_APN_PASS"; // some carries dont require

static const char server[]   = "your_URL";
static const char resource[] = "/your_endpoint";
static const int  port       = 443; // if http then 80
static const char apiKey[]   = "your_API_KEY";

// --- ESP-NOW ---
static const uint8_t broadcastAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define DEVICE_ID "ESP32-TRACKER-01"

// --- GENERAL ---
#define GPS_BATCH_SIZE 6
#define DEBUG_ENABLED  false // or true

#endif
