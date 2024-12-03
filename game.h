#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 10
#define MAX_PLAYERS 4

#include <gtk/gtk.h>

// Player structure
typedef struct {
    int position;            // Player's position on the board
    char color[20];          // Player's chosen color
    GtkWidget *pawn_widget;  // Reference to the player's pawn widget
} Player;

// Tile structure
typedef struct {
    int number;              // Tile number
    int snake_to;            // Destination for a snake
    int ladder_to;           // Destination for a ladder
    GtkWidget *button;       // Store button widget reference
    double x;                // x coordinate of button center
    double y;                // y coordinate of button center
} Tile;

// Game state structure
typedef struct {
    GtkApplication *app;
    int num_players;
    Player players[MAX_PLAYERS];
    Tile board[BOARD_SIZE][BOARD_SIZE];
    GtkWidget *drawing_area;
    GtkWidget *overlay;
    GtkWidget *grid;
    GtkWidget *dice_button; // Reference to the dice roll button
} GameState;



// Function declarations
void launch_game_settings(GtkApplication *app, gpointer user_data);
void launch_game_board(GameState *game_state);
void initialize_board(Tile board[BOARD_SIZE][BOARD_SIZE]);
void draw_snake(cairo_t *cr, double startX, double startY, double endX, double endY);
void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

// Helper functions
int calculate_row(int position);
int calculate_col(int position, int row);

#endif // GAME_H
