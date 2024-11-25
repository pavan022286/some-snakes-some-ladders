#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#define MAX_PLAYERS 4

#include <gtk/gtk.h>

// Player structure
typedef struct {
    int position;   // Player's position on the board
    char color[20]; // Player's chosen color
} Player;

// Tile structure
typedef struct {
    int number;    // Tile number
    int snake_to;  // Destination for a snake
    int ladder_to; // Destination for a ladder
} Tile;

// Game state structure
typedef struct {
    GtkApplication *app;          // GTK application reference
    int num_players;              // Number of players
    Player players[MAX_PLAYERS];  // Array of players
    Tile board[BOARD_SIZE][BOARD_SIZE]; // Game board
} GameState;

// Function declarations
void launch_game_settings(GtkApplication *app, gpointer user_data);
void launch_game_board(GameState *game_state);
void initialize_board(Tile board[BOARD_SIZE][BOARD_SIZE]);

#endif // GAME_H
