# Bitboard Checkers Game Documentation
  

## Table of Contents

  

1. [Introduction](#introduction)

2. [How to Play](#how-to-play)

3. [Code Overview](#code-overview)

	- [Data Structures](#data-structures)

	- [Function Descriptions](#function-descriptions)

4. [Detailed Code Explanation](#detailed-code-explanation)

	- [Main Function](#main-function)

	- [Game Initialization](#game-initialization)

	- [Board Representation](#board-representation)

	- [Move Generation and Validation](#move-generation-and-validation)

	- [Making Moves and Captures](#making-moves-and-captures)

	- [Game Loop](#game-loop)

5. [Future Improvements](#future-improvements)

  

---

  

## Introduction

  

Welcome to the ****Checkers Game****! This is a console-based checkers game implemented in C using bitboards for efficient computation. The game supports two players, adhering to standard checkers rules, including mandatory captures and king promotions.

  
## How to Play

  

•  **Players**: Two players take turns. **Player 1** uses the symbol '1' for regular pieces and 'K' for kings. **Player 2** uses the symbol '2' for regular pieces and 'k' for kings.

•  **Board Layout**: The board is an 8x8 grid with columns labeled **A** to **H** and rows numbered **1** to **8**. Only the dark squares (playable squares) are used in the game.

•  **Starting Position**:

•  **Player 1**’s pieces occupy the bottom three rows.

•  **Player 2**’s pieces occupy the top three rows.

•  **Making a Move**:

•  Enter moves in the format FROM TO or FROM to TO (e.g., B6 A5 or B6 to A5).

•  Coordinates are case-insensitive.

•  **Game Rules**:

•  **Simple Moves**: Move a piece diagonally forward to an adjacent empty square.

•  **Captures**: Jump over an opponent’s piece diagonally to capture it. If a capture is possible, it must be taken.

•  **King Promotion**: When a piece reaches the opponent’s back row, it is promoted to a king.

•  **Kings**: Can move and capture both forward and backward.

•  **Winning the Game**: The game ends when one player has no remaining pieces on the board. The other player is declared the winner.

  

## Code Overview

  

### Data Structures

  
**Bitboard**

  

•  **Type Definition**: typedef uint32_t Bitboard;

•  **Description**: Represents the board using a 32-bit unsigned integer where each bit corresponds to one of the 32 playable squares on a checkers board.

  

**GameState**

  

```typedef  struct {

Bitboard player_one_pieces; _// Bitboard representing Player 1's pieces_

Bitboard player_two_pieces; _// Bitboard representing Player 2's pieces_

Bitboard kings; _// Bitboard representing kings for both players_

} GameState;

 ```

•  **Description**: Holds the current state of the game, including the positions of pieces for both players and which pieces are kings.

  
### Function Descriptions

  

1. **initialize_game**: Sets up the initial game state with starting positions for all pieces.

2. **print_board**: Displays the current state of the board in the console.

3. **coord_to_index**: Converts board coordinates (e.g., “B6”) to a bitboard index (0-31).

4. **index_to_coord**: Converts a bitboard index back to board coordinates (e.g., “B6”).

5. **generate_moves**: Generates all possible simple (non-capture) moves for a piece at a given index.

6. **generate_captures**: Generates all possible capture moves for a piece at a given index.

7. **has_any_captures**: Checks if the current player has any captures available.

8. **is_move_valid**: Validates if a move is legal according to the game rules.

9. **make_move**: Updates the game state by performing the move, handling captures and promotions.

10. **game_loop**: Manages the main game loop, handling turns and user input.

11. **clear_input_buffer**: Clears the input buffer to prevent residual input from affecting the game.

  

## Detailed Code Explanation

  

### Main Function

  
```
int main() {

GameState state;

initialize_game(&state); _// Initialize the game state with starting positions_

game_loop(&state);  _// Start the main game loop_

return 0;

}
```
  

•  **Purpose**: The entry point of the program. It initializes the game state and starts the game loop.

  

### Game Initialization

  
```
void initialize_game(GameState *state) {

_// Player 1 occupies the bottom three rows (bits 20-31)_

state->player_one_pieces = 0xFFF00000;

_// Player 2 occupies the top three rows (bits 0-11)_

state->player_two_pieces = 0x00000FFF;

_// No kings at the start_

state->kings = 0;

}
```
  

•  **Purpose**: Sets up the starting positions for both players.

**Details**:

•  **Player 1**: Bits 20 to 31 are set to 1, representing Player 1’s pieces.

•  **Player 2**: Bits 0 to 11 are set to 1, representing Player 2’s pieces.

•  **Kings**: All bits are set to 0 initially since no pieces are kings at the start.

  

### Board Representation

  
```
void print_board(GameState *state) {

printf("\n  A B C D E F G H\n");

printf("  +---+---+---+---+---+---+---+---+\n");

for (int row = 0; row < 8; row++) {

printf("%d ", 8 - row);

for (int col = 0; col < 8; col++) {

printf("|");

if ((row + col) % 2 == 0) {

printf(" ");

} else {

int idx = row * 4 + col / 2;

Bitboard mask = (Bitboard)1 << idx;

if (state->player_one_pieces & mask) {

if (state->kings & mask)

printf(" K ");

else

printf(" 1 ");

} else if (state->player_two_pieces & mask) {

if (state->kings & mask)

printf(" k ");

else

printf(" 2 ");

} else {

printf(" . ");

}

}

}

printf("|\n  +---+---+---+---+---+---+---+---+\n");

}

}
```
  

•  **Purpose**: Displays the current state of the board in the console.

•  **Details**:

•  Iterates over each row and column, printing the board’s grid.

•  Uses bitmasking to determine which pieces are on each square.

•  Symbols:

•  '1': Player 1’s regular piece.

•  '2': Player 2’s regular piece.

•  'K': Player 1’s king.

•  'k': Player 2’s king.

•  '.': Empty playable square.

•  ' ' (spaces): Non-playable square.

  

**Coordinate Conversion**

  

**coord_to_index**

  
```
int coord_to_index(const char *coord) {

if (strlen(coord) < 2 || strlen(coord) > 3)

return -1;

char col_char = toupper(coord[0]);

int col = col_char - 'A';

int row = atoi(&coord[1]);

if (col < 0 || col > 7 || row < 1 || row > 8)

return -1;

  

int row_idx = 8 - row;

if ((row_idx + col) % 2 == 0)

return  -1; _// Invalid square_

int index = (row_idx * 4) + (col / 2);

return index;

}
```
  

•  **Purpose**: Converts board coordinates (e.g., “B6”) to a bitboard index (0-31).

•  **Parameters**:

•  const char *coord: The coordinate string input by the user.

•  **Returns**:

•  The corresponding bitboard index (0-31).

•  -1 if the coordinate is invalid.

•  **Logic**:

•  Parses the column and row from the coordinate string.

•  Validates the column (A-H) and row (1-8).

•  Calculates the row index based on the board’s internal representation.

•  Checks if the square is playable (dark square).

•  Calculates the index using the formula: index = (row_index * 4) + (col / 2).

  

**index_to_coord**

  
```
void index_to_coord(int index, char *coord) {

int row = index / 4;

int col = (index % 4) * 2 + ((row + 1) % 2);

sprintf(coord, "%c%d", 'A' + col, 8 - row);

}
```
  

**Purpose**: Converts a bitboard index (0-31) back to board coordinates (e.g., “B6”).

**Parameters**:

•  int index: The bitboard index to convert.

•  char *coord: A buffer to store the resulting coordinate string.

 **Logic**:

•  Calculates the row and column based on the index.

•  Adjusts for the board’s representation to get the correct coordinate.

•  Formats the coordinate string using sprintf.

  

### Move Generation and Validation

  

**generate_moves**

  
```
Bitboard generate_moves(GameState *state, int idx, int player) {

Bitboard moves = 0;

Bitboard occupied = state->player_one_pieces | state->player_two_pieces;

Bitboard empty = ~occupied;

Bitboard piece = (Bitboard)1 << idx;

  

int is_king = (state->kings & piece) != 0;

  

_// Move offsets for left and right moves_

int move_offsets[2];

if (player == 1) {

move_offsets[0] = -5; _// Move left_

move_offsets[1] = -4; _// Move right_

} else {

move_offsets[0] = 4;  _// Move left_

move_offsets[1] = 5;  _// Move right_

}

  

_// Check simple moves_

for (int i = 0; i < 2; i++) {

int target_idx = idx + move_offsets[i];

if (target_idx >= 0 && target_idx < 32) {

Bitboard target_mask = (Bitboard)1 << target_idx;

if ((empty & target_mask) != 0) {

moves |= target_mask;

}

}

}

  

_// Add backward moves for kings_

if (is_king) {

int king_offsets[2];

if (player == 1) {

king_offsets[0] = 4;  _// Move backward left_

king_offsets[1] = 5;  _// Move backward right_

} else {

king_offsets[0] = -5; _// Move backward left_

king_offsets[1] = -4; _// Move backward right_

}

for (int i = 0; i < 2; i++) {

int target_idx = idx + king_offsets[i];

if (target_idx >= 0 && target_idx < 32) {

Bitboard target_mask = (Bitboard)1 << target_idx;

if ((empty & target_mask) != 0) {

moves |= target_mask;

}

}

}

}

  

return moves;

}
```
  

•  **Purpose**: Generates all possible simple (non-capture) moves for a piece at a given index.

 **Parameters**:

•  GameState *state: Pointer to the current game state.

•  int idx: The index of the piece to generate moves for.

•  int player: The player number (1 or 2).

**Returns**:

•  A Bitboard where bits set to 1 represent possible destination squares for simple moves.

**Logic**:

•  Determines if the piece is a king.

•  Defines move offsets based on the player’s direction.

-  **Player 1**: Moves upwards (-5 and -4).

-  **Player 2**: Moves downwards (4 and 5).

•  Checks if the destination squares are within the board boundaries and unoccupied.

•  If the piece is a king, also considers backward moves.

  

**generate_captures**

  
```
Bitboard generate_captures(GameState *state, int idx, int player) {

Bitboard captures = 0;

Bitboard opponent_pieces = (player == 1) ? state->player_two_pieces : state->player_one_pieces;

Bitboard occupied = state->player_one_pieces | state->player_two_pieces;

Bitboard empty = ~occupied;

Bitboard piece = (Bitboard)1 << idx;

  

int is_king = (state->kings & piece) != 0;

  

_// Corrected capture offsets_

int capture_offsets[2][2];

if (player == 1) {

_// Capture Left: Middle idx = idx - 4, Target idx = idx - 9_

capture_offsets[0][0] = -4; capture_offsets[0][1] = -9;

_// Capture Right: Middle idx = idx - 3, Target idx = idx - 7_

capture_offsets[1][0] = -3; capture_offsets[1][1] = -7;

} else {

_// Capture Left: Middle idx = idx + 4, Target idx = idx + 9_

capture_offsets[0][0] = 4;  capture_offsets[0][1] = 9;

_// Capture Right: Middle idx = idx + 5, Target idx = idx + 7_

capture_offsets[1][0] = 5;  capture_offsets[1][1] = 7;

}

  

_// Check captures_

for (int i = 0; i < 2; i++) {

int middle_idx = idx + capture_offsets[i][0];

int target_idx = idx + capture_offsets[i][1];

if (middle_idx >= 0 && middle_idx < 32 && target_idx >= 0 && target_idx < 32) {

Bitboard middle_mask = (Bitboard)1 << middle_idx;

Bitboard target_mask = (Bitboard)1 << target_idx;

if ((opponent_pieces & middle_mask) != 0 && (empty & target_mask) != 0) {

captures |= target_mask;

}

}

}

  

_// Add backward captures for kings_

if (is_king) {

int king_capture_offsets[2][2];

if (player == 1) {

_// Backward captures for Player 1 kings_

king_capture_offsets[0][0] = 4;  king_capture_offsets[0][1] = 9;

king_capture_offsets[1][0] = 5;  king_capture_offsets[1][1] = 7;

} else {

_// Backward captures for Player 2 kings_

king_capture_offsets[0][0] = -5; king_capture_offsets[0][1] = -9;

king_capture_offsets[1][0] = -4; king_capture_offsets[1][1] = -7;

}

for (int i = 0; i < 2; i++) {

int middle_idx = idx + king_capture_offsets[i][0];

int target_idx = idx + king_capture_offsets[i][1];

if (middle_idx >= 0 && middle_idx < 32 && target_idx >= 0 && target_idx < 32) {

Bitboard middle_mask = (Bitboard)1 << middle_idx;

Bitboard target_mask = (Bitboard)1 << target_idx;

if ((opponent_pieces & middle_mask) != 0 && (empty & target_mask) != 0) {

captures |= target_mask;

}

}

}

}

  

return captures;

}
```
  

•  **Purpose**: Generates all possible capture moves for a piece at a given index.

**Parameters**:

•  GameState *state: Pointer to the current game state.

•  int idx: The index of the piece to generate captures for.

•  int player: The player number (1 or 2).

**Returns**:

•  A Bitboard where bits set to 1 represent possible destination squares after capturing.

**Logic**:

•  Determines if the piece is a king.

•  Defines capture offsets based on the player’s direction.

-  **Player 1**:

	-  Capture Left: Middle idx - 4, Target idx - 9

	-  Capture Right: Middle idx - 3, Target idx - 7

-  **Player 2**:

	- Capture Left: Middle idx + 4, Target idx + 9

	-  Capture Right: Middle idx + 5, Target idx + 7

•  Checks if the capture is valid by ensuring:

-  The middle square contains an opponent’s piece.

-  The landing square is empty.

-  The move is within board boundaries.

-  If the piece is a king, also considers backward captures.

  

**has_any_captures**

  
```
int has_any_captures(GameState *state, int player) {

Bitboard player_pieces = (player == 1) ? state->player_one_pieces : state->player_two_pieces;

for (int idx = 0; idx < 32; idx++) {

if (player_pieces & ((Bitboard)1 << idx)) {

Bitboard captures = generate_captures(state, idx, player);

if (captures != 0) {

return  1; _// Captures are available_

}

}

}

return  0; _// No captures available_

}
```
  

•  **Purpose**: Determines if the current player has any captures available.

 **Parameters**:

•  GameState *state: Pointer to the current game state.

•  int player: The player number (1 or 2).

**Returns**:

•  1 if the player has captures available.

•  0 if no captures are available.

**Logic**:

•  Iterates over all the player’s pieces.

•  Uses generate_captures to check for possible captures for each piece.

•  Returns as soon as a capture is found.

  


**Move Validation**

  

**is_move_valid**

  
```
int is_move_valid(GameState *state, int from_idx, int to_idx, int player) {

Bitboard piece = (Bitboard)1 << from_idx;

Bitboard moves = generate_moves(state, from_idx, player);

Bitboard captures = generate_captures(state, from_idx, player);

  

Bitboard to_mask = (Bitboard)1 << to_idx;

  

int must_capture = has_any_captures(state, player);

  

if (must_capture) {

if ((captures & to_mask) != 0) {

return  2; _// Valid capture move_

} else {

printf("You must capture if possible.\n");

return  0; _// Invalid move, must capture_

}

} else {

if ((captures & to_mask) != 0) {

return  2; _// Valid capture move_

} else if ((moves & to_mask) != 0) {

return 1; _// Simple move_

} else {

return 0; _// Invalid move_

}

}

}
```
  

•  **Purpose**: Validates whether a move from from_idx to to_idx is valid for the player.

**Parameters**:

•  GameState *state: Pointer to the current game state.

•  int from_idx: The index of the piece to move.

•  int to_idx: The destination index.

•  int player: The player number (1 or 2).

**Returns**:

•  2 if the move is a valid capture.

•  1 if the move is a valid simple move.

•  0 if the move is invalid.

 **Logic**:

•  Checks if the piece at from_idx belongs to the player.

•  Generates possible moves and captures using generate_moves and generate_captures.

•  Checks if the player must capture using has_any_captures.

•  Enforces mandatory captures:

•  If captures are available, the player must perform a capture.

•  If no captures are available, simple moves are allowed.

  ### Making Moves and Captures

  

**make_move**

  
```
void make_move(GameState *state, int from_idx, int to_idx, int player) {

Bitboard from_mask = (Bitboard)1 << from_idx;

Bitboard to_mask = (Bitboard)1 << to_idx;

  

Bitboard *player_pieces = (player == 1) ? &state->player_one_pieces : &state->player_two_pieces;

Bitboard *opponent_pieces = (player == 1) ? &state->player_two_pieces : &state->player_one_pieces;

  

int move_type = is_move_valid(state, from_idx, to_idx, player);

  

_// Move the piece_

*player_pieces &= ~from_mask; _// Remove piece from original position_

*player_pieces |= to_mask;  _// Place piece at new position_

  

_// Update kings_

if (state->kings & from_mask) {

state->kings &= ~from_mask; _// Remove king status from original position_

state->kings |= to_mask; _// Assign king status to new position_

}

  

_// Handle captures_

if (move_type == 2) {

int captured_idx = from_idx + ((to_idx - from_idx) / 2);

Bitboard captured_mask = (Bitboard)1 << captured_idx;

*opponent_pieces &= ~captured_mask; _// Remove captured piece from opponent_

state->kings &= ~captured_mask; _// Remove captured piece from kings if necessary_

}

  

_// Check for king promotion_

int row = to_idx / 4;

if ((player == 1 && row == 0) || (player == 2 && row == 7)) {

state->kings |= to_mask; _// Promote to king_

}

}

```  

•  **Purpose**: Updates the game state by moving a piece from from_idx to to_idx, handling captures and king promotions.

**Parameters**:

•  GameState *state: Pointer to the current game state.

•  int from_idx: The index of the piece to move.

•  int to_idx: The destination index.

•  int player: The player number (1 or 2).

**Logic**:

•  Determines the move type (simple move or capture) using is_move_valid.

•  Updates the player’s bitboard by clearing the from_idx bit and setting the to_idx bit.

•  Updates the kings bitboard if the piece is a king.

•  If it’s a capture:

•  Calculates the captured_idx using captured_idx = from_idx + ((to_idx - from_idx) / 2);

•  Removes the captured piece from the opponent’s bitboard and kings.

•  Checks for king promotion if the piece reaches the opponent’s back row.

  

### Game Loop

  

**game_loop**

  
```
void game_loop(GameState *state) {

int current_player = 1; _// Player 1 starts_

char input[20];

while (1) {

print_board(state); _// Display the board_

  

_// Check for game over conditions_

if (state->player_one_pieces == 0) {

printf("Player 2 wins!\n");

break;

} else if (state->player_two_pieces == 0) {

printf("Player 1 wins!\n");

break;

}

  

_// Prompt the current player for a move_

printf("Player %d's turn. Enter move (e.g., B6 to A5): ", current_player);

if (!fgets(input, sizeof(input), stdin)) {

clear_input_buffer();

continue;

}

  

_// Parse input coordinates_

char from_coord[4], to_coord[4];

if (sscanf(input, "%3s to %3s", from_coord, to_coord) != 2 &&

sscanf(input, "%3s %3s", from_coord, to_coord) != 2) {

printf("Invalid input. Please enter moves like 'B6 to A5'.\n");

continue;

}

  

_// Convert coordinates to indices_

int from_idx = coord_to_index(from_coord);

int to_idx = coord_to_index(to_coord);

  

if (from_idx == -1 || to_idx == -1) {

printf("Invalid coordinates. Please use columns A-H and rows 1-8.\n");

continue;

}

  

_// Check if the player has a piece at the 'from' position_

Bitboard player_pieces = (current_player == 1) ? state->player_one_pieces : state->player_two_pieces;

Bitboard from_mask = (Bitboard)1 << from_idx;

  

if ((player_pieces & from_mask) == 0) {

printf("You don't have a piece at that position.\n");

continue;

}

  

_// Validate the move_

int move_result = is_move_valid(state, from_idx, to_idx, current_player);

if (move_result == 0) {

printf("Invalid move. Try again.\n");

continue;

}

  

_// Make the move_

make_move(state, from_idx, to_idx, current_player);

  

_// Switch player_

current_player = (current_player == 1) ? 2 : 1;

}

}
```  

•  **Purpose**: Manages the main game loop, handling player turns, input parsing, move validation, and game termination.

**Parameters**:

•  GameState *state: Pointer to the current game state.

 **Logic**:

•  Alternates turns between Player 1 and Player 2.

•  Displays the board at the start of each turn.

•  Checks for game over conditions (i.e., if a player has no pieces left).

•  Prompts the current player for a move.

•  Parses the input and converts coordinates to indices.

•  Validates the move using is_move_valid.

-  If the move is valid, updates the game state using make_move.

-  If the move is invalid, prompts the player to try again.

•  Switches to the next player after a successful move.

  

**Input Buffer Clearing**

  

**clear_input_buffer**

  
```
void clear_input_buffer() {

int c;

while ((c = getchar()) != '\n' && c != EOF) {}

}
```
  

•  **Purpose**: Clears the input buffer to prevent leftover input from affecting subsequent input operations.

•  **Parameters**: None.

•  **Logic**:

•  Reads characters from the input until a newline or EOF is encountered.

  

## Future Improvements

  

•  **Multiple Jumps**: Implement logic to allow multiple captures in a single turn when applicable.

•  **AI Opponent**: Create an AI player to enable single-player mode.

•  **Graphical Interface**: Develop a GUI version for enhanced user experience.

•  **Save and Load**: Add functionality to save the game state and load it later.

•  **Network Play**: Enable playing over a network or the internet.

•  **Enhanced Move Validation**: Improve the robustness of move validation to handle all edge cases.

•  **User-Friendly Messages**: Provide more detailed feedback and instructions to players.

  


  

## Appendix

  

**Data Structures**

  

**Bitboard**

  

•  **Purpose**: Efficiently represent the state of the board using bits.

•  **Playable Squares**: Only 32 squares are playable in checkers; each bit corresponds to one of these squares.

  

**GameState Structure**

  

•  **Fields**:

•  player_one_pieces: Bitboard for Player 1’s pieces.

•  player_two_pieces: Bitboard for Player 2’s pieces.

•  kings: Bitboard indicating which pieces are kings.

  

**Function Reference**

  

**initialize_game**

  

•  **Sets up** the initial positions of the pieces.

  

**print_board**

  

•  **Displays** the board in the console.

•  **Symbols**:

•  '1': Player 1’s piece.

•  '2': Player 2’s piece.

•  'K': Player 1’s king.

•  'k': Player 2’s king.

•  '.': Empty playable square.

  

**coord_to_index**

  

•  **Converts** board coordinates to bitboard indices.

•  **Validates** the input coordinates.

  

**index_to_coord**

  

•  **Converts** bitboard indices back to board coordinates.

  

**generate_moves**

  

•  **Generates** possible simple moves for a piece.

•  **Considers** the player’s direction and whether the piece is a king.

  

**generate_captures**

  

•  **Generates** possible captures for a piece.

•  **Checks** for opponent pieces in capture positions.

  

**has_any_captures**

  

•  **Determines** if the current player must perform a capture.

  

**is_move_valid**

  

•  **Validates** the player’s move.

•  **Enforces** mandatory captures.

  

**make_move**

  

•  **Updates** the game state after a valid move.

•  **Handles** captures and king promotions.

  

**game_loop**

  

•  **Manages** the main game flow.

•  **Handles** user input and turn progression.

  

**clear_input_buffer**

  

•  **Clears** the input buffer to prevent unwanted input.

  

  

