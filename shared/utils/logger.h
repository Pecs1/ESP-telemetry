#pragma once

#include "utils/colors.h"

#include <HardwareSerial.h>
#include <cstdint>

enum LogLevel : uint8_t {
    DEBUG,
    INFO,
    WARN,
    ERR,
    CRIT
};

class Log {
  public:
    // sets the minimum log severity level
    void setMinLevel(LogLevel level) {
        minLevel = level;
    }

// just be careful... macros arent being type checked
#define LOG_INPUT_ARGS const char *component, const char *fmt, Args &&... args
#define LOG_PAYLOAD_ARGS component, fmt, std::forward<Args>(args)...

    template <typename... Args>
    void debug(LOG_INPUT_ARGS) {
        if (DEBUG < minLevel) {
            return;
        }
        printTagColor(green, "DEBUG", LOG_PAYLOAD_ARGS);
    }

    template <typename... Args>
    void info(LOG_INPUT_ARGS) {
        if (INFO < minLevel) {
            return;
        }
        printTagColor(blue, "INFO", LOG_PAYLOAD_ARGS);
    }

    template <typename... Args>
    void warn(LOG_INPUT_ARGS) {
        if (WARN < minLevel) {
            return;
        }
        printTagColor(yellow, "WARN", LOG_PAYLOAD_ARGS);
    }

    template <typename... Args>
    void err(LOG_INPUT_ARGS) {
        if (ERR < minLevel) {
            return;
        }
        printFullColor(magenta, "ERROR", LOG_PAYLOAD_ARGS);
    }

    template <typename... Args>
    void crit(LOG_INPUT_ARGS) {
        if (CRIT < minLevel) {
            return;
        }
        printFullColor(red, "CRIT", LOG_PAYLOAD_ARGS);
    }

// can safely undefine, coz users dont need this
#undef LOG_INPUT_ARGS
#undef LOG_PAYLOAD_ARGS

  private:
    LogLevel minLevel = INFO;

#define LOG_COLOR_ARGS \
    const char *color, const char *level, const char *component, const char *fmt, Args &&... args

    // prints only the severity level + component colored
    // colored just this for e.g. [DEBUG] [core]
    template <typename... Args>
    void printTagColor(LOG_COLOR_ARGS) {
        Serial.printf("%s[%s] [%s]%s ", color, level, component, colorReset);

        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("\n");
    }

    // similar, but prints the whole message colored
    template <typename... Args>
    void printFullColor(LOG_COLOR_ARGS) {
        Serial.printf("%s[%s] [%s] ", color, level, component);
        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("%s\n", colorReset);
    }
#undef LOG_COLOR_ARGS

    // helpers
    template <typename... Args>
    void argPrintf(const char* fmt, Args&&... args) {
        if constexpr (sizeof...(Args) == 0) {
            Serial.printf("%s", fmt);
        } else {
            Serial.printf(fmt, std::forward<Args>(args)...);
        }
    }
};

inline Log logger;
