#pragma once

#include "./colors.h"

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

    template <typename... Args>
    void debug(const char* component, const char* fmt, Args&&... args) {
        if (DEBUG < minLevel) {
            return;
        }
        printTagColor(green, "DEBUG", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(const char* component, const char* fmt, Args&&... args) {
        if (INFO < minLevel) {
            return;
        }
        printTagColor(blue, "INFO", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(const char* component, const char* fmt, Args&&... args) {
        if (WARN < minLevel) {
            return;
        }
        printTagColor(yellow, "WARN", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void err(const char* component, const char* fmt, Args&&... args) {
        if (ERR < minLevel) {
            return;
        }
        printFullColor(magenta, "ERROR", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void crit(const char* component, const char* fmt, Args&&... args) {
        if (CRIT < minLevel) {
            return;
        }
        printFullColor(red, "CRIT", component, fmt, std::forward<Args>(args)...);
    }

  private:
    LogLevel minLevel = INFO;

    template <typename... Args>
    void printTagColor(const char* color, const char* level, const char* component, const char* fmt,
                       Args&&... args) {
        Serial.printf("%s[%s] [%s]%s ", color, level, component, colorReset);

        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("\n");
    }

    // similar, but prints the whole message colored
    template <typename... Args>
    void printFullColor(const char* color, const char* level, const char* component,
                        const char* fmt, Args&&... args) {
        Serial.printf("%s[%s] [%s] ", color, level, component);
        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("%s\n", colorReset);
    }

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
