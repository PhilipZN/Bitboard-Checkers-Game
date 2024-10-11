#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bit_utils.h"

// Define bitboards for players
typedef uint64_t Bitboard;

typedef struct {
    Bitboard player_one_pieces; // Player 1's pieces
    Bitboard player_two_pieces; // Player 2's pieces
    Bitboard kings;             // Kings for both players
} GameState;


void initialize_game(GameState *state);
void print_board(GameState *state);
int coord_to_index(const char *coord);
void index_to_coord(int index, char *coord);
int is_move_valid(GameState *state, int from_idx, int to_idx, int player);
void make_move(GameState *state, int from_idx, int to_idx, int player);
Bitboard generate_moves(GameState *state, int idx, int player);
Bitboard generate_captures(GameState *state, int idx, int player);
void game_loop(GameState *state);
void clear_input_buffer();
int has_any_captures(GameState *state, int player);
void display_bitboard(Bitboard bb);

int main() {
    GameState state;
    initialize_game(&state);
    game_loop(&state);
    return 0;
}


void initialize_game(GameState *state) {
    state->player_one_pieces = 0;
    state->player_two_pieces = 0;
    state->kings = 0;

    // Initialize Player 2's pieces (rows 0 to 2)
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                int idx = row * 8 + col;
                set_bit(&state->player_two_pieces, idx);
            }
        }
    }

    // Initialize Player 1's pieces (rows 5 to 7)
    for (int row = 5; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                int idx = row * 8 + col;
                set_bit(&state->player_one_pieces, idx);
            }
        }
    }
}

// Convert board coordinate (e.g., "B6") to index (0-63)
int coord_to_index(const char *coord) {
    if (strlen(coord) < 2 || strlen(coord) > 3)
        return -1;

    char col_char = toupper(coord[0]);
    int col = col_char - 'A'; // 0 to 7
    int row = atoi(&coord[1]); // 1 to 8

    if (col < 0 || col > 7 || row < 1 || row > 8)
        return -1;

    int row_idx = 8 - row; // 0-based row index (row 8 -> 0)

    if ((row_idx + col) % 2 == 0)
        return -1; // Unplayable square

    int index = row_idx * 8 + col;
    return index;
}

// Convert index (0-63) to board coordinate (e.g., "B6")
void index_to_coord(int index, char *coord) {
    int row = index / 8;
    int col = index % 8;
    sprintf(coord, "%c%d", 'A' + col, 8 - row);
}

// Print the board
void print_board(GameState *state) {
    printf("\n    A   B   C   D   E   F   G   H\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int row = 0; row < 8; row++) {
        printf("%d ", 8 - row);
        for (int col = 0; col < 8; col++) {
            printf("|");
            if ((row + col) % 2 == 0) {
                printf("   ");
            } else {
                int idx = row * 8 + col;
                if (get_bit(state->player_one_pieces, idx)) {
                    if (get_bit(state->kings, idx))
                        printf(" K ");
                    else
                        printf(" 1 ");
                } else if (get_bit(state->player_two_pieces, idx)) {
                    if (get_bit(state->kings, idx))
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

// Clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Generate possible moves for a piece
Bitboard generate_moves(GameState *state, int idx, int player) {
    Bitboard moves = 0;
    Bitboard occupied = state->player_one_pieces | state->player_two_pieces;
    int is_king = get_bit(state->kings, idx);

    int directions[4];
    int num_directions = 0;

    // Determine movement directions
    if (player == 1 || is_king) {
        // Player 1 moves upwards (negative indices)
        directions[num_directions++] = -9; // Up-left
        directions[num_directions++] = -7; // Up-right
    }
    if (player == 2 || is_king) {
        // Player 2 moves downwards (positive indices)
        directions[num_directions++] = 7;  // Down-left
        directions[num_directions++] = 9;  // Down-right
    }

    for (int i = 0; i < num_directions; i++) {
        int target_idx = idx + directions[i];
        if (target_idx >= 0 && target_idx < 64) {
            if (!get_bit(occupied, target_idx)) {
                // Ensure move is to a playable square
                int row = idx / 8;
                int col = idx % 8;
                int target_row = target_idx / 8;
                int target_col = target_idx % 8;
                if (abs(target_row - row) == 1 && abs(target_col - col) == 1 && ((target_row + target_col) % 2 == 1)) {
                    set_bit(&moves, target_idx);
                }
            }
        }
    }
    return moves;
}

// Generate possible captures for a piece
Bitboard generate_captures(GameState *state, int idx, int player) {
    Bitboard captures = 0;
    Bitboard opponent_pieces = (player == 1) ? state->player_two_pieces : state->player_one_pieces;
    Bitboard occupied = state->player_one_pieces | state->player_two_pieces;
    int is_king = get_bit(state->kings, idx);

    int directions[4];
    int num_directions = 0;

    // Determine movement directions
    if (player == 1 || is_king) {
        // Player 1 captures upwards
        directions[num_directions++] = -9; // Up-left
        directions[num_directions++] = -7; // Up-right
    }
    if (player == 2 || is_king) {
        // Player 2 captures downwards
        directions[num_directions++] = 7;  // Down-left
        directions[num_directions++] = 9;  // Down-right
    }

    for (int i = 0; i < num_directions; i++) {
        int middle_idx = idx + directions[i];
        int target_idx = idx + 2 * directions[i];
        if (middle_idx >= 0 && middle_idx < 64 && target_idx >= 0 && target_idx < 64) {
            if (get_bit(opponent_pieces, middle_idx) && !get_bit(occupied, target_idx)) {
                // Ensure capture is to a playable square
                int row = idx / 8;
                int col = idx % 8;
                int target_row = target_idx / 8;
                int target_col = target_idx % 8;
                if (abs(target_row - row) == 2 && abs(target_col - col) == 2 && ((target_row + target_col) % 2 == 1)) {
                    set_bit(&captures, target_idx);
                }
            }
        }
    }
    return captures;
}

// Check if any captures are available for a player
int has_any_captures(GameState *state, int player) {
    Bitboard player_pieces = (player == 1) ? state->player_one_pieces : state->player_two_pieces;
    for (int idx = 0; idx < 64; idx++) {
        if (get_bit(player_pieces, idx)) {
            Bitboard captures = generate_captures(state, idx, player);
            if (captures != 0) {
                return 1; // Captures are available
            }
        }
    }
    return 0; // No captures available
}

// Check if a move is valid
int is_move_valid(GameState *state, int from_idx, int to_idx, int player) {
    Bitboard piece = ((Bitboard)1 << from_idx);
    Bitboard moves = generate_moves(state, from_idx, player);
    Bitboard captures = generate_captures(state, from_idx, player);

    Bitboard to_mask = ((Bitboard)1 << to_idx);

    int must_capture = has_any_captures(state, player);

    if (must_capture) { // Force players to capture
        if (captures & to_mask) {
            return 2; // Capture move
        } else {
            printf("You must capture if possible.\n");
            return 0; // Invalid move, must capture
        }
    } else {
        if (captures & to_mask) {
            return 2; // Capture move
        } else if (moves & to_mask) {
            return 1; // Simple move
        } else {
            return 0; // Invalid move
        }
    }
}

// Make a move
void make_move(GameState *state, int from_idx, int to_idx, int player) {
    Bitboard from_mask = ((Bitboard)1 << from_idx);
    Bitboard to_mask = ((Bitboard)1 << to_idx);

    Bitboard *player_pieces = (player == 1) ? &state->player_one_pieces : &state->player_two_pieces;
    Bitboard *opponent_pieces = (player == 1) ? &state->player_two_pieces : &state->player_one_pieces;

    int move_type = is_move_valid(state, from_idx, to_idx, player);

    // Move the piece
    clear_bit(player_pieces, from_idx);
    set_bit(player_pieces, to_idx);

    // Update kings
    int became_king = 0;
    int row = to_idx / 8;
    if ((player == 1 && row == 0) || (player == 2 && row == 7)) {
        set_bit(&state->kings, to_idx);
        became_king = 1;
    } else if (get_bit(state->kings, from_idx)) {
        clear_bit(&state->kings, from_idx);
        set_bit(&state->kings, to_idx);
    } else {
        clear_bit(&state->kings, to_idx);
    }

    // Handle captures
    if (move_type == 2) {
        int captured_idx = from_idx + ((to_idx - from_idx) / 2);
        clear_bit(opponent_pieces, captured_idx);
        clear_bit(&state->kings, captured_idx);
    }
}

// Display the bitboard in binary and hexadecimal formats
void display_bitboard(Bitboard bb) {
    print_binary(bb);
    print_hexadecimal(bb);
}

// Main game loop
void game_loop(GameState *state) {
    int current_player = 1;
    char input[20];
    while (1) {
        print_board(state);

        // Display bitboards
        printf("\nPlayer 1 Pieces:\n");
        display_bitboard(state->player_one_pieces);
        printf("Player 2 Pieces:\n");
        display_bitboard(state->player_two_pieces);
        printf("Kings:\n");
        display_bitboard(state->kings);

        // Check for game over
        if (state->player_one_pieces == 0) {
            printf("Player 2 wins!\n");
            break;
        } else if (state->player_two_pieces == 0) {
            printf("Player 1 wins!\n");
            break;
        }

        printf("Player %d's turn. Enter move (e.g., B6 to A5): ", current_player);
        if (!fgets(input, sizeof(input), stdin)) {
            clear_input_buffer();
            continue;
        }

        // Parse input
        char from_coord[4], to_coord[4];
        if (sscanf(input, "%3s to %3s", from_coord, to_coord) != 2 && sscanf(input, "%3s %3s", from_coord, to_coord) != 2) {
            printf("Invalid input. Please enter moves like 'B6 to A5'.\n");
            continue;
        }

        int from_idx = coord_to_index(from_coord);
        int to_idx = coord_to_index(to_coord);

        if (from_idx == -1 || to_idx == -1) {
            printf("Invalid coordinates. Please use columns A-H and rows 1-8.\n");
            continue;
        }

        Bitboard player_pieces = (current_player == 1) ? state->player_one_pieces : state->player_two_pieces;
        if (!get_bit(player_pieces, from_idx)) {
            printf("You don't have a piece at that position.\n");
            continue;
        }

        int move_result = is_move_valid(state, from_idx, to_idx, current_player);
        if (move_result == 0) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        make_move(state, from_idx, to_idx, current_player);

        // Switch player
        current_player = (current_player == 1) ? 2 : 1;
    }
}
