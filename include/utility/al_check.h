#pragma once

#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>

#ifndef NDEBUG

inline const char* al_error_string(ALenum err)
{
    switch (err)
    {
    case AL_NO_ERROR: return "AL_NO_ERROR";
    case AL_INVALID_NAME: return "AL_INVALID_NAME";
    case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
    default: return "Unknown AL error";
    }
}

inline void al_check_errors(const char* expr, const char* file, int line)
{
    if (ALenum err = alGetError(); err != AL_NO_ERROR)
    {
        std::cerr << "[OpenAL Error] " << al_error_string(err)
                  << " at " << file << ":" << line
                  << " for expression: " << expr << std::endl;
    }
}

// Specialization for non-void return types
template <typename Func>
auto al_checked(Func&& func, const char* expr, const char* file, int line)
    -> typename std::enable_if<!std::is_void<decltype(func())>::value, decltype(func())>::type
{
    auto result = func();
    al_check_errors(expr, file, line);
    return result;
}

// Specialization for void return types
template <typename Func>
auto al_checked(Func&& func, const char* expr, const char* file, int line)
    -> typename std::enable_if<std::is_void<decltype(func())>::value, void>::type
{
    func();
    al_check_errors(expr, file, line);
}

#define AL_CALL(expr) \
al_checked([&]() { return (expr); }, #expr, __FILE__, __LINE__)

#else

#define AL_CALL(expr) (expr)

#endif