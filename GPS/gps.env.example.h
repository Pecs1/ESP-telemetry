#ifndef GPS_ENV_H
#define GPS_ENV_H

// --- main configs ---
// set your 4 digit pin, must be the same between boards!
const uint16_t MAINTENANCE_PIN = 1234;

// AP mode
const char* MAINT_SSID   = "Pecs1 - Maintenance GPS";
const char* SPECIAL_PASSWD = "yourPassword"; // must be more than 7 characters


enum SystemMode : uint8_t {
    NORMAL      = 0,
    MAINTENANCE = 1,
    DEBUG       = 2,
    FAILSAFE    = 3,
};
SystemMode currentMode = NORMAL;

enum TxState {
     TX_IDLE,     // waiting for the next transmission
     TX_PENDING,  // packet is in the air
     TX_SUCCESS,  // Successfully trasnfered packet 
     TX_FAILED    // Failed to transfer packet
};

volatile TxState currentTxState = TX_IDLE;
uint8_t retryCounter = 0;
const uint8_t MAX_RETRIES = 3;

// --- main board MAC ---
uint8_t mainAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


// --- aliases ---
// for Preferences
#define RW false // read-write
#define RO true  // read-only

#endif
