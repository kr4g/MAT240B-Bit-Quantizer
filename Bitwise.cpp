#include "Bitwise.h"

// FUNCTION POINTERS
std::function<int32_t(int32_t, int32_t)> bitwiseOps[] = {
    [](int32_t a, int32_t b) { return a & b; }, // Bitwise AND
    [](int32_t a, int32_t b) { return a | b; }, // Bitwise OR
    [](int32_t a, int32_t b) { return a ^ b; }, // Bitwise XOR
    [](int32_t a, int32_t b) { return ~a; }, // Bitwise NOT
    [](int32_t a, int32_t b) { return a << b; }, // Bitwise Left Shift
    [](int32_t a, int32_t b) { return a >> b; }, // Bitwise Right Shift
    [](int32_t a, int32_t b) { return (a << b) | (a >> (32 - b)); }, // Bitwise Rotate Left
    [](int32_t a, int32_t b) { return (a >> b) | (a << (32 - b)); }, // Bitwise Rotate Right
    [](int32_t a, int32_t b) { return ~a; }, // Bitwise Flip
    [](int32_t a, int32_t b) { return (swapBits(a, b, b + 1) << 1) | (swapBits(a, b + 1, b) >> 1); } // Bitwise Swap
};

int32_t swapBits(int32_t value, int i, int j) {
    // Get the i-th and j-th bits
    int32_t bitI = (value >> i) & 1;
    int32_t bitJ = (value >> j) & 1;

    // Swap the i-th and j-th bits
    value ^= (bitI << j) | (bitJ << i);

    return value;
}

int32_t floatTo24bit(float sample) {
    int32_t intSample = static_cast<int32_t>(sample * 8388607.0f);
    return intSample & 0xFFFFFF; // Truncate to 24 bits
}

float intToFloat24bit(int32_t sample) {
    // Convert signed 24-bit integer to float
    if (sample & 0x800000) {
        sample |= 0xFF000000; // Sign-extend the value if it's negative
    }
    return static_cast<float>(sample) / 8388607.0f;
}

int32_t bitwise(int32_t a, int32_t b, BitwiseOp op) {
    int index = static_cast<int>(op);
    return bitwiseOps[index](a, b);
}

double bitwise(double a, double b, BitwiseOp op) {
    int32_t intA = floatTo24bit(a);
    int32_t intB = floatTo24bit(b);
    int32_t intResult = bitwise(intA, intB, op);
    return intToFloat24bit(intResult);
}
