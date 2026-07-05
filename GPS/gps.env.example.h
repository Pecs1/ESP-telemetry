#ifndef GPS_ENV_H
#define GPS_ENV_H

// --- main configs ---
// set your 4 digit passcode
// each passcode must be the same between boards!
const uint16_t MAINTENANCE_PASSCODE = 1234;
#define TEAM_NAME  "Pecs1"
#define BOARD_NAME "GPS"


// --- system ---
enum SystemMode : uint8_t {
    NORMAL      = 0, // sensors will function
    DEBUG       = 1, // NORMAL mode + debug overhead
    MAINTENANCE = 2, // sensors will stop + only allows OTA updates
    FAILSAFE    = 3  // sensors will try to function, will also try to spin up OTA updates
};
SystemMode currentMode = NORMAL;


// create APs on different modes
const char* MAINT_SSID    = TEAM_NAME " - Maintenance " BOARD_NAME;
const char* SPECIAL_PASSWD = "SpecialPasswd"; // must be more than 7 characters


// --- main board MAC ---
uint8_t mainAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


// --- aliases ---
// for Preferences
#define RW false // read-write
#define RO true  // read-only


// --- send packet state & behaviour ---
enum TxState {
     TX_IDLE,     // waiting for the next transmission
     TX_PENDING,  // packet is in the air
     TX_SUCCESS,  // Successfully trasnfered packet
     TX_FAILED    // Failed to transfer packet
};
volatile TxState currentTxState = TX_IDLE;
uint8_t retryCounter = 0;
const uint8_t MAX_RETRIES = 3;

#endif
