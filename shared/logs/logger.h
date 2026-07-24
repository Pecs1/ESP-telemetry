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
    void setMinLevel(LogLevel level) {
        minLevel = level;
    }

    void debug(std::string_view component, std::string_view message) {
        if (DEBUG < minLevel) {
            return;
        }
        printTagColor(green, "DEBUG", component, message);
    }

    void info(std::string_view component, std::string_view message) {
        if (INFO < minLevel) {
            return;
        }
        printTagColor(blue, "INFO", component, message);
    }

    void warn(std::string_view component, std::string_view message) {
        if (WARN < minLevel) {
            return;
        }
        printTagColor(yellow, "WARN", component, message);
    }

    void err(std::string_view component, std::string_view message) {
        if (ERR < minLevel) {
            return;
        }
        printFullColor(magenta, "ERROR", component, message);
    }

    void crit(std::string_view component, std::string_view message) {
        if (CRIT < minLevel) {
            return;
        }
        printFullColor(red, "CRIT", component, message);
    }

  private:
    LogLevel minLevel = INFO;

    void printTagColor(const char* color, const char* level, std::string_view component,
                       std::string_view message) {
        Serial.printf("%s[%-5s] [%.*s]%s %.*s\n", color, level, svLen(component), svData(component),
                      colorReset, svLen(message), svData(message));
    }

    // similar, but prints the whole message colored
    void printFullColor(const char* color, const char* level, std::string_view component,
                        std::string_view message) {
        Serial.printf("%s[%-5s] [%.*s] %.*s %s\n", color, level, svLen(component),
                      svData(component), svLen(message), svData(message), colorReset);
    }

    // helpers
    int svLen(std::string_view task) {
        return static_cast<int>(task.length());
    }

    const char* svData(std::string_view task) {
        return task.data();
    }
};

inline Log logger;
