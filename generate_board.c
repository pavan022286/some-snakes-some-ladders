/* Prashanta Kandel, 400521817, 2024-12-05
 *
 * C Script that implements GTK4 to simulate a
 * Snakes and Ladders Game
 * There are various functions that include the
 * logic for the game, and the generation of the
 * board for the game
 */
#include <gtk/gtk.h>
#include "game.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Helper to get pawn SVG path based on color
/* get_pawn_svg
 *
 * Parameters: String for color
 * Side Effect: N/A
 * Description: set the player color to correct file
 * Return:  file name of correct color piece
 */
const char *get_pawn_svg(const char *color)
{
    g_print("Debug: Resolving SVG for color: %s\n", color);

    if (strcmp(color, "Red") == 0)
        return "red_pawn.svg";
    if (strcmp(color, "Blue") == 0)
        return "blue_pawn.svg";
    if (strcmp(color, "Green") == 0)
        return "green_pawn.svg";
    if (strcmp(color, "Yellow") == 0)
        return "yellow_pawn.svg";

    g_print("Warning: Unknown color '%s', defaulting to default_pawn.svg.\n", color);
    return "default_pawn.svg";
}

// Function to update button coordinates
/* update_button_coordinates
 *
 * Parameters: Tile *tile pointer to tile typedef
 * Side Effect: set tile x and y cooridnates
 * Description: updates the buttons coordinates
 * Return:  void
 */
static void update_button_coordinates(Tile *tile)
{
    GtkAllocation allocation;
    gtk_widget_get_allocation(tile->button, &allocation);

    // Get the button's position relative to its parent
    tile->x = allocation.x + allocation.width / 2.0;
    tile->y = allocation.y + allocation.height / 2.0;
}
// Function to update all button coordinates and trigger redraw
/* update_coordinates
 *
 * Parameters: GtkWidget, gdkFrameClock, gpointer to user data
 * Side Effect: N/A
 * Description: Loop through board and updated buttons cooridnate
 * Return: gboolean
 */
static gboolean update_coordinates(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            update_button_coordinates(&game_state->board[i][j]);
        }
    }

    gtk_widget_queue_draw(game_state->drawing_area);
    return G_SOURCE_CONTINUE;
}

// Initialize the game board
// Prashanta Kandel
/* initialize_board
 *
 * Tiles board[][] is a 2D array of size 10
 * The struct Tiles holds the information of each tile such as its number,
 * and if it leads to a snake or a ladder
 * This function iterates over the 2D array and initlizes the number,snakes and ladders
 * void method
 */
void initialize_board(Tile board[BOARD_SIZE][BOARD_SIZE])
{
    int counter = 1; // Start numbering from 1

    for (int i = BOARD_SIZE - 1; i >= 0; i--)
    { // Rows from bottom to top
        if ((BOARD_SIZE - 1 - i) % 2 == 0)
        { // Even rows (from bottom) left to right
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                board[i][j].number = counter++;
                board[i][j].snake_to = 0;
                board[i][j].ladder_to = 0;
            }
        }
        else
        { // Odd rows right to left
            for (int j = BOARD_SIZE - 1; j >= 0; j--)
            {
                board[i][j].number = counter++;
                board[i][j].snake_to = 0;
                board[i][j].ladder_to = 0;
            }
        }
    }

    // Add snakes(down)
    board[0][1].snake_to = 28; // 99 to 28
    board[3][1].snake_to = 19; // 62 to 19
    board[5][2].snake_to = 18; // 43 to 18
    board[1][8].snake_to = 53; // 89 to 53

    // Add ladders(up)
    board[9][3].ladder_to = 24; // 4 to 24
    board[8][7].ladder_to = 44; // 13 to 44
    board[6][7].ladder_to = 50; // 33 to 50
    board[9][6].ladder_to = 56; // 7 to 56
}

// On tile button click handler

// Prashanta Kandel
/* on_tile_button_clicked
 *
 * Parameters: GtkButton is listening for the button clicked, user_data is
 * the pointer to the tile the user is on
 * This function outputs the place where the tile leads to if the button has been clicked
 * void method
 */
static void on_tile_button_clicked(GtkButton *button, gpointer user_data)
{
    Tile *tile = (Tile *)user_data;
    g_print("Clicked tile %d\n", tile->number);

    if (tile->snake_to != 0)
    {
        g_print("Snake! Go to tile %d\n", tile->snake_to);
    }
    else if (tile->ladder_to != 0)
    {
        g_print("Ladder! Go to tile %d\n", tile->ladder_to);
    }
}

// dice roll segment, function to roll the dice
/* roll_dice
 *
 * Parameters: N/A
 * Side Effect: N/A
 * Description: get random number from 1 - 6
 * Return: random int number
 */
static int roll_dice()
{
    return (rand() % 6) + 1;
}

// function to move a player
/* move_player
 *
 * Parameters: Game state, player_index
 * Side Effect: player position
 * Description: updates the location of the player after rolling dice
 * Return: void
 */
static void move_player(GameState *game_state, int player_index)
{
    Player *player = &game_state->players[player_index];
    int dice_result = roll_dice();
    g_print("Player %d rolled a %d\n", player_index + 1, dice_result);

    int new_position = player->position + dice_result;

    if (new_position > 100)
    {
        g_print("Player %d cannot move, dice roll exceeds 100.\n", player_index + 1);
        return;
    }

    // Handle snakes and ladders
    Tile *tile = NULL;
    while (1)
    {
        int row = (BOARD_SIZE - 1) - ((new_position - 1) / BOARD_SIZE);
        int col = ((BOARD_SIZE - 1 - row) % 2 == 0)
                      ? (new_position - 1) % BOARD_SIZE                       // Even row: left-to-right
                      : (BOARD_SIZE - 1) - ((new_position - 1) % BOARD_SIZE); // Odd row: right-to-left

        tile = &game_state->board[row][col];

        // Debugging log for the new position
        g_print("Player %d landed on tile %d (row: %d, col: %d)\n",
                player_index + 1, new_position, row, col);

        if (tile->snake_to > 0)
        {
            g_print("Snake! Player %d moves from tile %d to tile %d\n",
                    player_index + 1, new_position, tile->snake_to);
            new_position = tile->snake_to;
        }
        else if (tile->ladder_to > 0)
        {
            g_print("Ladder! Player %d moves from tile %d to tile %d\n",
                    player_index + 1, new_position, tile->ladder_to);
            new_position = tile->ladder_to;
        }
        else
        {
            break; // No snake or ladder; stop
        }
    }

    // Update player's position
    player->position = new_position;

    // Calculate final row and column
    int row = (BOARD_SIZE - 1) - ((new_position - 1) / BOARD_SIZE);
    int col = ((BOARD_SIZE - 1 - row) % 2 == 0)
                  ? (new_position - 1) % BOARD_SIZE                       // Even row: left-to-right
                  : (BOARD_SIZE - 1) - ((new_position - 1) % BOARD_SIZE); // Odd row: right-to-left

    // Hide the old pawn (reuse the widget)
    if (player->pawn_widget && GTK_IS_WIDGET(player->pawn_widget))
    {
        gtk_widget_hide(player->pawn_widget);
    }

    // Create a new pawn widget
    GtkWidget *new_pawn = gtk_image_new_from_file(get_pawn_svg(player->color));
    player->pawn_widget = new_pawn;

    // Add pawn with offsets for stacking
    int offset_x = (player_index % 2) * 10; // Horizontal offset
    int offset_y = (player_index / 2) * 10; // Vertical offset
    gtk_grid_attach(GTK_GRID(game_state->grid), new_pawn, col, row, 1, 1);
    gtk_widget_set_margin_start(new_pawn, offset_x);
    gtk_widget_set_margin_top(new_pawn, offset_y);

    // Ensure the pawn widget is visible
    if (GTK_IS_WIDGET(new_pawn))
    {
        gtk_widget_show(new_pawn);
    }
    else
    {
        g_print("Error: Invalid pawn widget for Player %d.\n", player_index + 1);
    }

    g_print("Player %d moved to tile %d (row: %d, col: %d).\n", player_index + 1, new_position, row, col);

    // Ensure the dice roll button remains visible
    if (GTK_IS_WIDGET(game_state->dice_button) && !gtk_widget_is_visible(game_state->dice_button))
    {
        gtk_widget_show(game_state->dice_button);
    }
}

// callback for the dice roll button
/* on_dice_roll_clicked
 *
 * Parameters: button pointer, user data
 * Side Effect: N/A
 * Description: calls other functions if the roll button is clicked
 * Return:  void
 */
// callback for the dice roll buttons
static void on_dice_roll_clicked(GtkButton *button, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;

    static int current_player = 0;
    move_player(game_state, current_player);

    // moves to the next player
    current_player = (current_player + 1) % game_state->num_players;
    g_print("Next turn: Player %d\n", current_player + 1);
}

// Launch the game board window
/* launch_game_board
 *
 * Parameter: GameSate is pointer to what the game state is currently in
 * This function creates each componenet of the GUI, such as the grid for the board
 * the overlay to draw the snakes and ladders, the pieces on the board, and the
 * roll button to move the players
 * Side effect, updates game_state
 * void function
 */
void launch_game_board(GameState *game_state)
{
    g_print("Debug: launch_game_board called.\n");

    // Verify the GtkApplication instance
    if (!GTK_IS_APPLICATION(game_state->app))
    {
        g_print("Error: Invalid GtkApplication instance in game_state->app.\n");
        game_state->app = GTK_APPLICATION(g_application_get_default());
        if (!GTK_IS_APPLICATION(game_state->app))
        {
            g_print("Error: Failed to recover GtkApplication instance.\n");
            return;
        }
        g_print("Debug: GtkApplication recovered: %p\n", game_state->app);
    }

    // Create the game board window
    GtkWidget *window = gtk_application_window_new(game_state->app);
    if (!GTK_IS_WINDOW(window))
    {
        g_print("Error: Failed to create game board window.\n");
        return;
    }
    g_print("Debug: Window created: %p\n", window);

    gtk_window_set_title(GTK_WINDOW(window), "Snakes and Ladders");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    // Create overlay to stack drawing area and grid
    game_state->overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), game_state->overlay);

    // Create grid for buttons and save it to game_state
    game_state->grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(game_state->grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(game_state->grid), 5);

    // Create drawing area for snakes and ladders
    game_state->drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(game_state->drawing_area),
                                   draw_function, game_state, NULL);
    gtk_widget_set_can_target(game_state->drawing_area, FALSE);

    // Initialize the game board
    initialize_board(game_state->board);

    // Create and attach buttons for each tile
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Tile *tile = &game_state->board[i][j];
            tile->button = gtk_button_new_with_label(g_strdup_printf("%d", tile->number));
            gtk_widget_set_size_request(tile->button, 60, 60);
            g_signal_connect(tile->button, "clicked",
                             G_CALLBACK(on_tile_button_clicked), tile);
            gtk_grid_attach(GTK_GRID(game_state->grid), tile->button, j, i, 1, 1);
        }
    }

    // Add grid and drawing area to overlay
    gtk_overlay_set_child(GTK_OVERLAY(game_state->overlay), game_state->grid);
    gtk_overlay_add_overlay(GTK_OVERLAY(game_state->overlay),
                            game_state->drawing_area);

    // Add tick callback to update coordinates
    gtk_widget_add_tick_callback(window, update_coordinates, game_state, NULL);

    // Add pawns for players
    int start_row = BOARD_SIZE - 1; // Tile 1's row
    int start_col = 0;              // Tile 1's column

    // Spawn players in reverse order
    for (int i = game_state->num_players - 1; i >= 0; i--)
    { // Start from Player N (last player)
        GtkWidget *pawn = gtk_image_new_from_file(get_pawn_svg(game_state->players[i].color));
        if (pawn)
        {
            // Attach each player's pawn to the starting tile (1)
            gtk_grid_attach(GTK_GRID(game_state->grid), pawn, start_col, start_row, 1, 1);
            g_print("Debug: Player %d pawn added with color %s at square 1 (row: %d, col: %d).\n",
                    i + 1, game_state->players[i].color, start_row, start_col);
            game_state->players[i].pawn_widget = pawn; // Save the pawn widget in the player struct
        }
        else
        {
            g_print("Error: Failed to load SVG for Player %d.\n", i + 1);
        }
    }

    // Add dice roll button
    GtkWidget *dice_button = gtk_button_new_with_label("Roll Dice");
    g_signal_connect(dice_button, "clicked", G_CALLBACK(on_dice_roll_clicked), game_state);
    gtk_grid_attach(GTK_GRID(game_state->grid), dice_button, 0, BOARD_SIZE, BOARD_SIZE, 1);

    gtk_widget_show(window);
    g_print("Debug: Game board displayed.\n");
}
