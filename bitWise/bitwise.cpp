// #ifndef BITWISE_HPP
// #define BITWISE_HPP

#include <bitset>

#include <cmath>
#include <climits>
#include <functional>
#include <map>
#include <string>

enum class BitwiseOp {
    AND,
    OR,
    XOR,
    NOT,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    FLIP,
    SWAP,
};
const std::map<BitwiseOp, std::string> bitwiseOpLabels = {
    {BitwiseOp::AND, "AND"},
    {BitwiseOp::OR, "OR"},
    {BitwiseOp::XOR, "XOR"},
    {BitwiseOp::NOT, "NOT"},
    {BitwiseOp::SHIFT_LEFT, "SHIFT_LEFT"},
    {BitwiseOp::SHIFT_RIGHT, "SHIFT_RIGHT"},
    {BitwiseOp::ROTATE_LEFT, "ROTATE_LEFT"},
    {BitwiseOp::ROTATE_RIGHT, "ROTATE_RIGHT"},
    {BitwiseOp::FLIP, "FLIP"},
    {BitwiseOp::SWAP, "SWAP"}
};

// Function pointer type for bitwise operations
typedef int32_t (*BitwiseFunc)(int32_t, int32_t);

// Bitwise AND Function
int32_t bitwiseAND(int32_t a, int32_t b);

// Bitwise OR Function
int32_t bitwiseOR(int32_t a, int32_t b);

// Bitwise XOR Function
int32_t bitwiseXOR(int32_t a, int32_t b);

// Bitwise NOT Function
int32_t bitwiseNOT(int32_t a, int32_t b);

// Bitwise Left Shift Function
int32_t bitwiseShiftLeft(int32_t a, int32_t b);

// Bitwise Right Shift Function
int32_t bitwiseShiftRight(int32_t a, int32_t b);

// Bitwise Rotate Left Function
int32_t bitwiseRotateLeft(int32_t a, int32_t b);

// Bitwise Rotate Right Function
int32_t bitwiseRotateRight(int32_t a, int32_t b);

// Bitwise Flip Function
int32_t bitwiseFlip(int32_t a, int32_t b);

// Bitwise Swap Function (returns swapped value without modifying original parameter)
int32_t swapBits(int32_t value, int i, int j);

// Bitwise function dispatcher
int32_t bitwise(int32_t a, int32_t b, BitwiseOp op);


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

// FUNCTION DEFINITIONS
int32_t bitwiseAND(int32_t a, int32_t b) {
    return a & b;
}

int32_t bitwiseOR(int32_t a, int32_t b) {
    return a | b;
}

int32_t bitwiseXOR(int32_t a, int32_t b) {
    return a ^ b;
}

int32_t bitwiseNOT(int32_t a, int32_t b) {
    return ~a;
}

int32_t bitwiseShiftLeft(int32_t a, int32_t b) {
    return a << b;
}

int32_t bitwiseShiftRight(int32_t a, int32_t b) {
    return a >> b;
}

int32_t bitwiseRotateLeft(int32_t a, int32_t b) {
    return (a << b) | (a >> (32 - b));
}

int32_t bitwiseRotateRight(int32_t a, int32_t b) {
    return (a >> b) | (a << (32 - b));
}

int32_t bitwiseFlip(int32_t a, int32_t b) {
    return ~a;
}

int32_t swapBits(int32_t value, int i, int j) {
    // Get the i-th and j-th bits
    int32_t bitI = (value >> i) & 1;
    int32_t bitJ = (value >> j) & 1;

    // Swap the i-th and j-th bits
    value ^= (bitI << j) | (bitJ << i);

    return value;
}

int32_t bitwise(int32_t a, int32_t b, BitwiseOp op) {
    int index = static_cast<int>(op);
    return bitwiseOps[index](a, b);
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

double bitwise(double a, double b, BitwiseOp op) {
    int32_t intA = floatTo24bit(a);
    int32_t intB = floatTo24bit(b);
    int32_t intResult = bitwise(intA, intB, op);
    return intToFloat24bit(intResult);
}

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

int main() {
    // Approx. Sine Wave
    std::vector<double> samples = {0.0, 0.707, 1.0, 0.707, 0.0, -0.707, -1.0, -0.707};

    // Convert the samples to signed 32-bit integers (ranging from -2147483648 to 2147483647)
    std::vector<int32_t> intSamples;
    for (double sample : samples) {
        intSamples.push_back(floatTo24bit(sample));
    }

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Perform a random bitwise operation on each sample
    std::cout << "Original samples: ";
    for (double sample : samples) {
        std::cout << sample << " ";
    }
    
    std::cout << std::endl;
    
    int32_t intOperand = std::rand() % 32;
    BitwiseOp op = static_cast<BitwiseOp>(std::rand() % 10); // Generate a random bitwise operation
    std::cout << "\n>> Bitwise op " << bitwiseOpLabels.at(op) << " with operand val " << intOperand << " (" << std::bitset<16>(intOperand) << ") <<\n";
    std::cout << "\nBitwise results: ";
    for (int32_t intSample : intSamples) {
        int32_t result = bitwise(intSample, intOperand, op); // Perform the operation on the sample
        double doubleResult = intToFloat24bit(result); // Convert the result back to a double
        std::cout << doubleResult << " ";
    }
    std::cout << "\n";

    return 0;
}
