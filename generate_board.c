#include <gtk/gtk.h>
#include "game.h"
#include <string.h>

// Helper to get pawn SVG path based on color
const char *get_pawn_svg(const char *color) {
    g_print("Debug: Resolving SVG for color: %s\n", color);

    if (strcmp(color, "Red") == 0) return "red_pawn.svg";
    if (strcmp(color, "Blue") == 0) return "blue_pawn.svg";
    if (strcmp(color, "Green") == 0) return "green_pawn.svg";
    if (strcmp(color, "Yellow") == 0) return "yellow_pawn.svg";

    g_print("Warning: Unknown color '%s', defaulting to default_pawn.svg.\n", color);
    return "default_pawn.svg";
}

// Initialize the game board
void initialize_board(Tile board[BOARD_SIZE][BOARD_SIZE]) {
    int counter = 1; // Start numbering from 1

    for (int i = BOARD_SIZE - 1; i >= 0; i--) { // Rows from bottom to top
        if ((BOARD_SIZE - 1 - i) % 2 == 0) { // Even rows (from bottom) left to right
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j].number = counter++;
                board[i][j].snake_to = 0;
                board[i][j].ladder_to = 0;
            }
        } else { // Odd rows right to left
            for (int j = BOARD_SIZE - 1; j >= 0; j--) {
                board[i][j].number = counter++;
                board[i][j].snake_to = 0;
                board[i][j].ladder_to = 0;
            }
        }
    }

    // Example snake and ladder for demonstration
    board[1][5].snake_to = 6;   // Snake from 16 to 6
    board[0][2].ladder_to = 22; // Ladder from 3 to 22
}

// Launch the game board window
void launch_game_board(GameState *game_state) {
    g_print("Debug: launch_game_board called.\n");

    // Verify the GtkApplication instance
    if (!GTK_IS_APPLICATION(game_state->app)) {
        g_print("Error: Invalid GtkApplication instance in game_state->app.\n");
        game_state->app = GTK_APPLICATION(g_application_get_default());
        if (!GTK_IS_APPLICATION(game_state->app)) {
            g_print("Error: Failed to recover GtkApplication instance.\n");
            return;
        }
        g_print("Debug: GtkApplication recovered: %p\n", game_state->app);
    }

    // Create the game board window
    GtkWidget *window = gtk_application_window_new(game_state->app);
    if (!GTK_IS_WINDOW(window)) {
        g_print("Error: Failed to create game board window.\n");
        return;
    }
    g_print("Debug: Window created: %p\n", window);

    gtk_window_set_title(GTK_WINDOW(window), "Snakes and Ladders");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    initialize_board(game_state->board);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            GtkWidget *button = gtk_button_new_with_label(g_strdup_printf("%d", game_state->board[i][j].number));
            gtk_widget_set_size_request(button, 60, 60);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
        }
    }

    int start_row = BOARD_SIZE - 1;
    int start_col = 0;

    GtkWidget *player1_pawn = gtk_image_new_from_file(get_pawn_svg(game_state->players[0].color));
    if (player1_pawn) {
        gtk_grid_attach(GTK_GRID(grid), player1_pawn, start_col, start_row, 1, 1);
        g_print("Debug: Player 1 pawn added with color %s at square 1 (row: %d, col: %d).\n",
                game_state->players[0].color, start_row, start_col);
    } else {
        g_print("Error: Failed to load SVG for Player 1.\n");
    }

    gtk_widget_show(window);
    g_print("Debug: Game board displayed.\n");
}
