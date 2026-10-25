#include "versioner.h"

#include "atria/core/abi.h"

#include <esp_app_desc.h>

const char* VersionUtil::getProjectName() const {
    return esp_app_get_description()->project_name;
}

const char* VersionUtil::getProjectVer() const {
    return esp_app_get_description()->version;
}

uint8_t VersionUtil::getLibABI() const {
    return libDescABI.lib_abi;
}

uint8_t VersionUtil::getMajorVer() const {
    uint8_t major = 0;
    parseSemver(getProjectVer(), &major, nullptr, nullptr);
    return major;
}

uint8_t VersionUtil::getMinorVer() const {
    uint8_t minor = 0;
    parseSemver(getProjectVer(), nullptr, &minor, nullptr);
    return minor;
}

uint8_t VersionUtil::getPatchVer() const {
    uint8_t patch = 0;
    parseSemver(getProjectVer(), nullptr, nullptr, &patch);
    return patch;
}

void VersionUtil::parseSemver(const char* ver, uint8_t* major, uint8_t* minor, uint8_t* patch) {
    int m = 0, n = 0, p = 0;
    sscanf(ver, "%d.%d.%d", &m, &n, &p);

    if (major) {
        *major = (uint8_t)m;
    }
    if (minor) {
        *minor = (uint8_t)n;
    }
    if (patch) {
        *patch = (uint8_t)p;
    }
}
VersionUtil version;
