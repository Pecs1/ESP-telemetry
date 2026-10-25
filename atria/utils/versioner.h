#pragma once

#include <cstdint>

class VersionUtil {
  public:
    const char* getProjectName() const;
    const char* getProjectVer() const;

    uint8_t getMajorVer() const;
    uint8_t getMinorVer() const;
    uint8_t getPatchVer() const;

    uint8_t getLibABI() const;

  private:
    static void parseSemver(const char* ver, uint8_t* major, uint8_t* minor, uint8_t* patch);
};
extern VersionUtil version;
