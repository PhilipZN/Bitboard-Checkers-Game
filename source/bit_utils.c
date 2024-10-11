#include "bit_utils.h"
#include <stdio.h>

// Set a specific bit
void set_bit(uint64_t *num, int bit) {
    *num |= ((uint64_t)1 << bit);
}

// Clear a specific bit
void clear_bit(uint64_t *num, int bit) {
    *num &= ~((uint64_t)1 << bit);
}

// Toggle a specific bit
void toggle_bit(uint64_t *num, int bit) {
    *num ^= ((uint64_t)1 << bit);
}

// Get the value of a specific bit
int get_bit(uint64_t num, int bit) {
    return (num >> bit) & 1;
}

// Basic binary arithmetic
uint64_t add(uint64_t a, uint64_t b) {
    return a + b;
}

uint64_t subtract(uint64_t a, uint64_t b) {
    return a - b;
}

uint64_t multiply(uint64_t a, uint64_t b) {
    return a * b;
}

uint64_t divide(uint64_t a, uint64_t b) {
    if (b == 0) {
        fprintf(stderr, "Division by zero error.\n");
        return 0;
    }
    return a / b;
}

// Data conversion
void print_binary(uint64_t num) {
    printf("Binary: ");
    for (int i = 63; i >= 0; i--) {
        printf("%d", get_bit(num, i));
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

void print_hexadecimal(uint64_t num) {
    printf("Hexadecimal: 0x%016llX\n", (unsigned long long)num);
}

void print_decimal(uint64_t num) {
    printf("Decimal: %llu\n", (unsigned long long)num);
}
