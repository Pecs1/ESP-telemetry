#pragma once

inline constexpr const char* colorReset = "\033[0m"; // clear

// foreground colors
inline constexpr const char* green   = "\033[1;32m"; // debug
inline constexpr const char* yellow  = "\033[1;33m"; // warn
inline constexpr const char* blue    = "\033[1;34m"; // info
inline constexpr const char* magenta = "\033[1;35m"; // error

// backround colors
inline constexpr const char* red = "\033[41;1;97m"; // crit (critical)
