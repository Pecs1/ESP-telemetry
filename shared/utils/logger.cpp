#include "logger.h"

#include "utils/colors.h"

#include <HardwareSerial.h>
#include <cstdint>

void LogUtil::debug(const char* component, const char* fmt, ...) {
    if (LogLevel::DEBUG < minLevel) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    LogUtil::printer(green, "DEBUG", component, fmt, args);
    va_end(args);
}

void LogUtil::info(const char* component, const char* fmt, ...) {
    if (LogLevel::INFO < minLevel) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    LogUtil::printer(blue, "INFO", component, fmt, args);
    va_end(args);
}

void LogUtil::warn(const char* component, const char* fmt, ...) {
    if (LogLevel::WARN < minLevel) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    LogUtil::printer(yellow, "WARN", component, fmt, args);
    va_end(args);
}

void LogUtil::err(const char* component, const char* fmt, ...) {
    if (LogLevel::ERR < minLevel) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    LogUtil::printer(magenta, "ERROR", component, fmt, args, true);
    va_end(args);
}

void LogUtil::crit(const char* component, const char* fmt, ...) {
    if (LogLevel::CRIT < minLevel) {
        return;
    }
    va_list args;
    va_start(args, fmt);
    LogUtil::printer(red, "CRIT", component, fmt, args, true);
    va_end(args);
}

void LogUtil::printer(const char* color, const char* level, const char* component, const char* fmt,
                      va_list args, bool fullColored) {
    char payload[192];
    vsnprintf(payload, sizeof(payload), fmt, args);

    if (!fullColored) {
        Serial.printf("%s[%s] [%s]%s %s\n", color, level, component, colorReset, payload);
    } else {
        Serial.printf("%s[%s] [%s] %s%s\n", color, level, component, payload, colorReset);
    }
}

LogUtil logger;
