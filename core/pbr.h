//
// Created by Ricky on 2016/12/27.
//

#ifndef PBR_PBR_H
#define PBR_PBR_H

#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <cstring>

#ifdef PBR_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif

// Useful Constants

static const Float Pi      = 3.14159265358979323846;
static const Float InvPi   = 0.31830988618379067154;
static const Float Inv2Pi  = 0.15915494309189533577;
static const Float Inv4Pi  = 0.07957747154594766788;
static const Float PiOver2 = 1.57079632679489661923;
static const Float PiOver4 = 0.78539816339744830961;
static const Float Sqrt2   = 1.41421356237309504880;

static constexpr Float MaxFloat = std::numeric_limits<Float>::max();
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();
static constexpr Float MachineEpsilon = std::numeric_limits<Float>::epsilon() * 0.5;

// Utility functions

// Clamp() clamps the given value val to lie between the values low and high.

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

// Mod() computes the remainder of a/b. Different the C++ default implementation, the Mod() of a negative number is
// always positive.

template <typename  T>
inline T Mod(T a, T b) {
    T result = a - (a/b) * b;
    return (T)((result < 0) ? result + b : result);
}

template<>
inline Float Mod(Float a, Float b) {
    return std::fmod(a, b);
}

// Converting between angle measures

inline Float Radians(Float deg) {
    return (Pi / 180) * deg;
}

inline Float Degrees(Float rad) {
    return (180 / Pi) * rad;
}

// Base-2 Operations

inline Float Log2(Float x) {
    const Float invLog2 = 1.442695040888963387004650940071;
    return std::log(x) * invLog2;
}

inline int Log2Int(uint32_t v) {
    // __builtin_clz() returns the number of leading 0-bits in x. If x is 0, the result is undefined.
    return 31 - __builtin_clz(v);
}

template <typename T>
inline bool IsPowerOf2(T v) {
    return v && !(v & (v - 1));
}

inline int32_t RoundUpPow2(int32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}

inline int CountTrailingZeros(uint32_t v) {
    return __builtin_ctz(v);
}

// Converts between floating-points values and bits interpretations

inline uint32_t FloatToBits(float f) {
    uint32_t ui;
    std::memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float BitsToFloat(uint32_t ui) {
    float f;
    std::memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}

inline float NextFloatUp(float v) {
    // handle infinity and negative zero for NextFloatUp()
    if (std::isinf(v) && v > 0.f) return v;
    if (v == -0.f) return 0.f;
    // advance v to next higher float
    uint32_t ui = FloatToBits(v);
    v >= 0 ? ++ui : --ui;
    return BitsToFloat(ui);
}

#endif //PBR_PBR_H
