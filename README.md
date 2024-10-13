# Bitboard Checkers

## Introduction

This project showcases a console-based checkers game developed in C. It utilizes bitboards to represent the game state, employing bitwise operations and binary arithmetic for quick computations and to move pieces on the board efficiently.

## Table of Contents

- [Research and Design Overview](#research-and-design-overview)
  - [Language Choice](#language-choice)
  - [Significance of Bitwise Operations in Computing and Game Development](#significance-of-bitwise-operations-in-computing-and-game-development)
  - [Bitboard Representation](#bitboard-representation)
  - [Data Structures](#data-structures)
  - [Utility Class Design and Implementation](#utility-class-design-and-implementation)
- [Implementation Details](#implementation-details)
  - [Bitwise Operations and Binary Arithmetic](#bitwise-operations-and-binary-arithmetic)
    - [Usage in the Utility Class](#usage-in-the-utility-class)
    - [Usage in the Bitboard](#usage-in-the-bitboard)
  - [Data Conversion Between Formats](#data-conversion-between-formats)
- [Challenges Faced](#challenges-faced)
  - [Consistent Board Indexing and Mapping](#consistent-board-indexing-and-mapping)
  - [Move Generation Logic](#move-generation-logic)
  - [Implementing Mandatory Captures](#implementing-mandatory-captures)
  - [Handling King Pieces](#handling-king-pieces)
  - [Edge Cases and Input Validation](#edge-cases-and-input-validation)
- [Gameplay Instructions](#gameplay-instructions)

---

## Research and Design Overview

### Language Choice
I opted to build this game using the C programming language. C provides six built-in bitwise operators: AND(&), OR(|), XOR(^), NOT(~), left shift (<<), and right shift (>>). These operators enable precise bit manipulation and low-level memory management, making C an ideal choice for handling bitboards effectively. [^1] [^2]  

![Various operators in C](https://media.geeksforgeeks.org/wp-content/cdn-uploads/Operators-In-C.png)

### Significance of Bitwise Operations in Computing and Game Development

#### In Low-Level Computing

- **Efficiency**: Bitwise operations are fast, often executing in just one CPU cycle.
- **Memory Usage**: Bitboards help compactly represent data, reducing memory usage.
- **Data Control**: They allow for detailed control over individual bits, which is essential in systems programming and embedded systems.

#### In Game Development

- **Performance**: Crucial for real-time games where quick calculations are key.
- **State Representation**: Simplifies complex game states, making it easier to implement game logic.
- **Artificial Intelligence**: Commonly used in AI for board games to evaluate positions and generate moves efficiently.

### Bitboard Representation

A bitboard is a 64-bit integer used to represent the state of a board in a board game. For this project, it represents: 
- **Playable Squares**: Only 32 of the 64 squares are playable (the dark squares). Each playable square corresponds to a specific bit in the bitboard.
- **Player Pieces**: Each player has a bitboard indicating the positions of their pieces.
- **Kings**: An additional bitboard tracks the positions of king pieces.

### Data Structures

- **`GameState` Structure**:

  ```c
  typedef struct {
      uint64_t player_one_pieces; // Player 1's pieces
      uint64_t player_two_pieces; // Player 2's pieces
      uint64_t kings;             // King pieces for both players
  } GameState;
  ```

- **Purpose**: This structure encapsulates the entire state of the game, including the positions of all pieces and kings.

### Utility Class Design and Implementation

The utility class, found in `bit_utils.h` and `bit_utils.c`, offers essential functions for:

- **Bit Manipulation**: Setting, clearing, toggling, and retrieving the value of bits.
- **Binary Arithmetic**: Performing arithmetic operations on `uint64_t` numbers.
- **Data Conversion**: Converting and displaying numbers in binary, hexadecimal, and decimal formats.

**Header File (`bit_utils.h`):**

```c
#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>

// Bit manipulation functions
void set_bit(uint64_t *num, int bit);
void clear_bit(uint64_t *num, int bit);
void toggle_bit(uint64_t *num, int bit);
int get_bit(uint64_t num, int bit);

// Binary arithmetic functions
uint64_t add(uint64_t a, uint64_t b);
uint64_t subtract(uint64_t a, uint64_t b);
uint64_t multiply(uint64_t a, uint64_t b);
uint64_t divide(uint64_t a, uint64_t b);

// Data conversion functions
void print_binary(uint64_t num);
void print_hexadecimal(uint64_t num);
void print_decimal(uint64_t num);

#endif // BIT_UTILS_H
```

**Source File (`bit_utils.c`):**

```c
#include "bit_utils.h"
#include <stdio.h>

// Set a specific bit to 1
void set_bit(uint64_t *num, int bit) {
    *num |= ((uint64_t)1 << bit);
}

// Clear a specific bit to 0
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

// Basic binary arithmetic operations
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

// Data conversion and display functions
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
```

---

## Implementation Details

### Bitwise Operations and Binary Arithmetic

#### Usage in the Utility Class

- **Bit Manipulation Functions**:

  - **`set_bit`**: Sets a bit to 1 using bitwise OR (`|`).
  - **`clear_bit`**: Clears a bit to 0 using bitwise AND (`&`) and bitwise NOT (`~`).
  - **`toggle_bit`**: Flips a bit using bitwise XOR (`^`).
  - **`get_bit`**: Retrieves the value of a bit using bitwise shift (`>>`) and bitwise AND (`&`).

- **Binary Arithmetic Functions**:

  - Provide basic arithmetic operations on `uint64_t` numbers.
  - Useful for calculations related to the game state and debugging.

#### Usage in the Bitboard

- **Representing the Board**:

  - Each bit in a `uint64_t` represents a square on the 8x8 checkers board.
  - The mapping corresponds to the board coordinates for consistent indexing.

- **Placing and Moving Pieces**:

  - **Placing a Piece**: Use `set_bit` to place a piece at a specific index.
  - **Moving a Piece**:
    - Clear the original position using `clear_bit`.
    - Set the new position using `set_bit`.

- **Checking Occupied Squares**:

  - Use `get_bit` to check if a square is occupied.
  - Combine bitboards with bitwise OR (`|`) to create occupancy masks.

- **Move Calculations**:

  - Calculate potential moves by adding direction offsets to the current index.
  - Use bitwise operations to ensure moves are within bounds and on playable squares.

### Data Conversion Between Formats

#### Coordinate Conversion

- **Board Coordinates to Bit Indices**:

  ```c
  int coord_to_index(const char *coord) {
      char col_char = toupper(coord[0]);
      int col = col_char - 'A'; // Columns 0 to 7
      int row = atoi(&coord[1]); // Rows 1 to 8
      int row_idx = 8 - row; // Convert to 0-based index

      if ((row_idx + col) % 2 == 0)
          return -1; // Unplayable square

      int index = row_idx * 8 + col;
      return index;
  }
  ```

  - **Example**: "B6

" converts to index 17.

- **Bit Indices to Board Coordinates**:

  ```c
  void index_to_coord(int index, char *coord) {
      int row = index / 8;
      int col = index % 8;
      sprintf(coord, "%c%d", 'A' + col, 8 - row);
  }
  ```

  - **Example**: Index 17 converts back to "B6".

#### Displaying Bitboards

- **Binary Format**: Use `print_binary` to display the bitboard in binary.
- **Hexadecimal Format**: Use `print_hexadecimal` to display the bitboard in hexadecimal.
- **Decimal Format**: Use `print_decimal` to display the bitboard in decimal.

These conversions aid in debugging and understanding the internal state of the game.

---

## Challenges Faced

### Consistent Board Indexing and Mapping

- **Problem**: Ensuring consistent mapping between board coordinates and internal bit indices.
- **Solution**:

  - Established a standardized mapping where the top-left corner (`A8`) is index 0.
  - Ensured all functions (`coord_to_index`, `index_to_coord`, move generation) use the same indexing system.
  - Tested the mapping with various coordinates to verify correctness.

### Move Generation Logic

- **Problem**: Accurately generating legal moves and captures, accounting for direction and board boundaries.
- **Solution**:

  - Created arrays of direction offsets for move calculations.
  - Added boundary checks to prevent moves off the board.
  - Verified that generated moves land on playable squares (dark squares).

### Implementing Mandatory Captures

- **Problem**: Enforcing the rule that players must make a capture if one is available.
- **Solution**:

  - Implemented the `has_any_captures` function to check for available captures.
  - Modified `is_move_valid` to prioritize captures over simple moves.
  - Provided feedback to the player when a capture is available but not chosen.

### Handling King Pieces

- **Problem**: Managing the additional movement capabilities of king pieces.
- **Solution**:

  - Updated move generation functions to include backward moves for kings.
  - Used the `kings` bitboard to track which pieces are kings.
  - Implemented king promotion logic when a piece reaches the opponent's back row.

### Edge Cases and Input Validation

- **Problem**: Handling invalid inputs and moves that could cause errors or crashes.
- **Solution**:

  - Added comprehensive input validation in `coord_to_index` and `game_loop`.
  - Ensured that functions gracefully handle out-of-bounds indices.
  - Provided clear error messages to guide the player.

---

## Gameplay Instructions

- **Starting the Game**: The game begins with Player 1's turn.
- **Input Format**: Enter moves using coordinates, e.g., `"A3 to B4"` or `"A3 B4"`. Input is case-insensitive.
- **Making Moves**:
  - Input the source and destination coordinates when prompted.
  - The game will validate the move and update the board accordingly.
  - If a move is invalid, the player will be asked to make a valid move.
- **Winning the Game**: The game ends when a player loses all their pieces.

[^1]: [Bitwise Operators in C - GeeksforGeeks.](https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/)

[^2]: [Bitwise Operators in C - Tutorial Point.](https://www.tutorialspoint.com/cprogramming/c_bitwise_operators.htm)

[^3]: [Bitboards - Rustic Chess.](https://rustic-chess.org/board_representation/bitboards.html)
