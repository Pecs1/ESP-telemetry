#ifndef GPS_ENV_H
#define GPS_ENV_H

// --- main configs ---
const uint16_t MAINTENANCE_PIN = 0000; // set your 4 digit pin, must be the same between boards!

// --- main board MAC ---
uint8_t mainAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


// --- aliases ---
// for Preferences
#define RW false // read-write
#define RO true  // read-only

#endif
