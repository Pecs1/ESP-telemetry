#pragma once

#include <cstdarg>
#include <cstdint>

enum class LogLevel : uint8_t {
    DEBUG,
    INFO,
    WARN,
    ERR,
    CRIT
};

class LogUtil {
  public:
    // sets the minimum log severity level
    void setMinLevel(LogLevel level) {
        minLevel = level;
    }

    void debug(const char* component, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
    void info(const char* component, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
    void warn(const char* component, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
    void err(const char* component, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
    void crit(const char* component, const char* fmt, ...) __attribute__((format(printf, 3, 4)));

  private:
    LogLevel minLevel = LogLevel::INFO;
    void printer(const char* color, const char* level, const char* component, const char* fmt,
                 va_list args, bool fullColored = false);
};
extern LogUtil logger;
