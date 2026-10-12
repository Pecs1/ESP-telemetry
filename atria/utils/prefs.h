#pragma once

#include <cstdint>

// NVS keys
constexpr const char* nvsName        = "sys_state";
constexpr const char* currentModeKey = "current_mode";
constexpr const char* nextModeKey    = "next_mode";

class Prefs {
  public:
    void keyGenerator();

    // since im reading/writing only uint8_t no need to complicate it with auto ^^
    // at least for now... :"}
    uint8_t keyReader(const char* whatKey);
    void keyWriter(const char* key, uint8_t value);

  private:
    void open(bool readOnly);
    void close();

    // for guards.h
    bool keysGenerated = false;
    void protectedKeyGenerator();
    uint8_t protectedKeyReader(const char* key);
    void protectedKeyWriter(const char* key, uint8_t value);
};

extern Prefs prefs;
