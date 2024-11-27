#include <gtk/gtk.h>
#include "game.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

// Function to update button coordinates
static void update_button_coordinates(Tile *tile) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(tile->button, &allocation);
    
    // Get the button's position relative to its parent
    tile->x = allocation.x + allocation.width / 2.0;
    tile->y = allocation.y + allocation.height / 2.0;
}

// Function to update all button coordinates and trigger redraw
static gboolean update_coordinates(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer user_data) {
    GameState *game_state = (GameState *)user_data;
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            update_button_coordinates(&game_state->board[i][j]);
        }
    }
    
    gtk_widget_queue_draw(game_state->drawing_area);
    return G_SOURCE_CONTINUE;
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

    // Add snakes(down)
    board[0][1].snake_to = 28;   // 99 to 28
    board[3][1].snake_to = 19;   // 62 to 19
    board[5][2].snake_to = 18;   // 43 to 18
    board[1][8].snake_to = 53;   // 89 to 53

    // Add ladders(up)
    board[9][3].ladder_to = 24;  // 4 to 24
    board[8][7].ladder_to = 44;  // 13 to 44
    board[6][7].ladder_to = 50;  // 33 to 50
    board[9][6].ladder_to = 56;  // 7 to 56
}

// On tile button click handler
static void on_tile_button_clicked(GtkButton *button, gpointer user_data) {
    Tile *tile = (Tile *)user_data;
    g_print("Clicked tile %d\n", tile->number);
    
    if (tile->snake_to != 0) {
        g_print("Snake! Go to tile %d\n", tile->snake_to);
    } else if (tile->ladder_to != 0) {
        g_print("Ladder! Go to tile %d\n", tile->ladder_to);
    }
}
//dice roll segment, function to roll the dice
static int roll_dice() {
    return (rand() % 6) + 1;
}

//function to move a player
static void move_player(GameState *game_state, int player_index) {
    Player *player = &game_state->players[player_index];
    int dice_result = roll_dice();
    g_print("Player %d rolled a %d\n", player_index + 1, dice_result); //prints roll result

    int new_position = player->position + dice_result;
    if (new_position > 100) {
        g_print("Player %d cannot move, dice roll exceeds 100.\n", player_index + 1); //makes sure roll is not more than 100
        return;
    }

    player->position = new_position;
    g_print("Player %d moved to tile %d\n", player_index + 1, player->position); //displays new position
}

// callback for the dice roll button
static void on_dice_roll_clicked(GtkButton *button, gpointer user_data) {
    GameState *game_state = (GameState *)user_data;

    static int current_player = 0;
    move_player(game_state, current_player);

    //moves to the next player
    current_player = (current_player + 1) % game_state->num_players;
    g_print("Next turn: Player %d\n", current_player + 1);
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


    // Create overlay to stack drawing area and grid
    game_state->overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), game_state->overlay);

    // Create grid for buttons
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    

    // Create drawing area for snakes and ladders
    game_state->drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(game_state->drawing_area), 
                                  draw_function, game_state, NULL);
    gtk_widget_set_can_target(game_state->drawing_area, FALSE);

    initialize_board(game_state->board);

    // Create and attach buttons
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Tile *tile = &game_state->board[i][j];
            tile->button = gtk_button_new_with_label(g_strdup_printf("%d", tile->number));
            gtk_widget_set_size_request(tile->button, 60, 60);
            g_signal_connect(tile->button, "clicked", 
                           G_CALLBACK(on_tile_button_clicked), tile);
            gtk_grid_attach(GTK_GRID(grid), tile->button, j, i, 1, 1);
        }
    }

    // Add grid and drawing area to overlay
    gtk_overlay_set_child(GTK_OVERLAY(game_state->overlay), grid);
    gtk_overlay_add_overlay(GTK_OVERLAY(game_state->overlay), 
                          game_state->drawing_area);

    // Add tick callback to update coordinates
    gtk_widget_add_tick_callback(window, update_coordinates, game_state, NULL);

    // Add pawns for players
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

    //dice roll buttion
    GtkWidget *dice_button = gtk_button_new_with_label("Roll Dice");
    g_signal_connect(dice_button, "clicked", G_CALLBACK(on_dice_roll_clicked), game_state);
    gtk_grid_attach(GTK_GRID(grid), dice_button, 0, BOARD_SIZE, BOARD_SIZE, 1);

    gtk_widget_show(window);
    g_print("Debug: Game board displayed.\n");
}
