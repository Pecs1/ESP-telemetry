#include "prefs.h"

#include "atria/utils/guard.h"
#include "atria/utils/logger.h"

#include <Preferences.h>

// docs/example:
// https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

Preferences nvs;

#define MODULE_NAME "prefs"

namespace {
    // Read/Write access flags
    constexpr bool rw = false;
    constexpr bool ro = true;

    // Helper for non-existent key creation
    template <typename Func>
    void checkKeyUtil(const char* key, Func&& arg) {
        if (!nvs.isKey(key)) {
            logger.warn(MODULE_NAME, "\"%s\" key not found", key);
            logger.debug(MODULE_NAME, "creating \"%s\" key", key);

            arg(key);

            logger.info(MODULE_NAME, "\"%s\" key created", key);
        }
    }
} // namespace

void Prefs::open(bool readOnly) {
    logger.debug(MODULE_NAME, "opening \"%s\" persistent storage", nvsName);
    nvs.begin(nvsName, readOnly); // open persistent storage
}

void Prefs::close() {
    logger.debug(MODULE_NAME, "closing persistent storage");
    nvs.end(); // close persistent storage
}

// *** protect functions ***
void Prefs::keyGenerator() {
    if (!keysGenerated) {
        keysGenerated = true;
        protectedKeyGenerator();
    } else {
        guardMSG();
    }
}

uint8_t Prefs::keyReader(const char* whatKey) {
    if (keysGenerated) {
        return protectedKeyReader(whatKey);
    } else {
        guardDepsMSG(".keyGenerator");
    }
    // will be safest to return the highest possible value
    // since values above 10 arent used
    return 255;
}

void Prefs::keyWriter(const char* key, uint8_t value) {
    if (keysGenerated) {
        protectedKeyWriter(key, value);
    } else {
        guardDepsMSG(".keyGenerator");
    }
}

// *** protected functions ***
void Prefs::protectedKeyGenerator() {
    open(rw); // open in RW

    // check individual keys
    checkKeyUtil(currentModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    checkKeyUtil(nextModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    // easily expandible ^^

    close();
}

uint8_t Prefs::protectedKeyReader(const char* key) {
    open(ro); // open in RO

    uint8_t value = nvs.getUChar(key);

    close();

    return value;
}

void Prefs::protectedKeyWriter(const char* key, uint8_t value) {
    open(rw); // open in RW

    nvs.putUChar(key, value);

    close();
}
Prefs prefs;
