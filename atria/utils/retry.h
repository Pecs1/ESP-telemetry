#pragma once

#include <type_traits>

#define retry(maxRetries, delayMS, ...) \
    executeRetry(MODULE_NAME, __FUNCTION__, maxRetries, delayMS, __VA_ARGS__)

using RetryFn = bool (*)(void* userCtx);

void retryExecutor(const char* file, const char* function, int maxRetries, int delayMS, RetryFn fn,
                   void* userCtx);

template <typename Func>
inline void executeRetry(const char* file, const char* function, int maxRetries, int delayMS,
                         Func&& action) {
    auto invoker = [](void* ctx) -> bool {
        return (*static_cast<typename std::remove_reference<Func>::type*>(ctx))();
    };
    retryExecutor(file, function, maxRetries, delayMS, invoker, static_cast<void*>(&action));
}
