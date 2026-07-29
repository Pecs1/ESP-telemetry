#pragma once

#include "./colors.h"

#include <HardwareSerial.h>
#include <cstdint>
#include <string_view>

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
    void debug(std::string_view component, const char* fmt, Args&&... args) {
        if (DEBUG < minLevel) {
            return;
        }
        printTagColor(green, "DEBUG", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(std::string_view component, const char* fmt, Args&&... args) {
        if (INFO < minLevel) {
            return;
        }
        printTagColor(blue, "INFO", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(std::string_view component, const char* fmt, Args&&... args) {
        if (WARN < minLevel) {
            return;
        }
        printTagColor(yellow, "WARN", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void err(std::string_view component, const char* fmt, Args&&... args) {
        if (ERR < minLevel) {
            return;
        }
        printFullColor(magenta, "ERROR", component, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void crit(std::string_view component, const char* fmt, Args&&... args) {
        if (CRIT < minLevel) {
            return;
        }
        printFullColor(red, "CRIT", component, fmt, std::forward<Args>(args)...);
    }

  private:
    LogLevel minLevel = INFO;

    template <typename... Args>
    void printTagColor(const char* color, const char* level, std::string_view component,
                       const char* fmt, Args&&... args) {
        Serial.printf("%s[%-5s] [%.*s]%s ", color, level, svLen(component), svData(component),
                      colorReset);

        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("\n");
    }

    // similar, but prints the whole message colored
    template <typename... Args>
    void printFullColor(const char* color, const char* level, std::string_view component,
                        const char* fmt, Args&&... args) {
        Serial.printf("%s[%-5s] [%.*s] ", color, level, svLen(component), svData(component));
        argPrintf(fmt, std::forward<Args>(args)...);
        Serial.printf("%s\n", colorReset);
    }

    // helpers
    int svLen(std::string_view task) {
        return static_cast<int>(task.length());
    }

    const char* svData(std::string_view task) {
        return task.data();
    }

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
