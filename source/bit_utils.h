#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>

// Set a specific bit
void set_bit(uint64_t *num, int bit);

// Clear a specific bit
void clear_bit(uint64_t *num, int bit);

// Toggle a specific bit
void toggle_bit(uint64_t *num, int bit);

// Get the value of a specific bit
int get_bit(uint64_t num, int bit);

// Basic binary arithmetic
uint64_t add(uint64_t a, uint64_t b);
uint64_t subtract(uint64_t a, uint64_t b);
uint64_t multiply(uint64_t a, uint64_t b);
uint64_t divide(uint64_t a, uint64_t b);

// Data conversion
void print_binary(uint64_t num);
void print_hexadecimal(uint64_t num);
void print_decimal(uint64_t num);

#endif // BIT_UTILS_H
