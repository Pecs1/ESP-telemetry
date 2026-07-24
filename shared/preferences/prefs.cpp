#include "./prefs.h"

#define RW false // read-write
#define RO true  // read-only

constexpr const char* nvsName        = "sys_state";
constexpr const char* currentModeKey = "current_mode";

void PrefsUtil::check() {

    // note that logger isnt in the docs, logger is my custom util
    // docs/example:
    // https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html

    logger.debug("prefs", "opening \"%s\" persistant storage", nvsName);
    nvs.begin(nvsName, RW); // open RW
    logger.info("prefs", "checking keys");

    // check individual keys
    checkUtil(currentModeKey, [&](const char* key) { nvs.putUChar(key, 0); });
    // easily expandible ^^

    logger.debug("prefs", "closing persistant storage");
    nvs.end(); // close RW
    logger.info("prefs", "done checking keys");
}

PrefsUtil prefs;
