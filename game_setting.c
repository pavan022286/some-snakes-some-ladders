/* Harold Le, 400502557, 2024-12-05
 *
 * C Script that creates the main menue
 * screen for Snakes and Ladders. It includes
 * Player selction and the help button
 * This leads to the board creation
 */
#include <gtk/gtk.h>
#include "game.h"
#include <string.h>

static GtkComboBoxText *color_combos[MAX_PLAYERS];
static GtkWidget *color_labels[MAX_PLAYERS];

// Callback for "Start Game" button
// Harold Le, 400502557, 2024-12-05
/* on_settings_confirmed
 *
 * Parameters: 
 *   GtkWidget *widget - The "Start Game" button widget (unused).
 *   gpointer data - Pointer to the GameState structure.
 * Side Effect: 
 *   - Updates player colors and initial positions based on user selection.
 *   - Logs debug information about player color selection.
 *   - Validates color uniqueness across players.
 *   - Calls `launch_game_board` to proceed to the game board screen.
 * Description: Processes player settings (e.g., color selection) and transitions 
 *              to the game board after validation.
 * Return: None.
 */

static void on_settings_confirmed(GtkWidget *widget, gpointer data)
{
    GameState *game_state = (GameState *)data;

    g_print("Debug: on_settings_confirmed called.\n");

    for (int i = 0; i < game_state->num_players; i++)
    {
        const char *color = gtk_combo_box_text_get_active_text(color_combos[i]);
        g_print("Debug: Player %d selected color: %s\n", i + 1, color);

        if (!color)
        {
            g_print("Player %d must select a color.\n", i + 1);
            return;
        }

        for (int j = 0; j < i; j++)
        {
            if (strcmp(color, game_state->players[j].color) == 0)
            {
                g_print("Players cannot choose the same color.\n");
                return;
            }
        }

        strncpy(game_state->players[i].color, color, sizeof(game_state->players[i].color) - 1);
        game_state->players[i].color[sizeof(game_state->players[i].color) - 1] = '\0';
        g_print("Debug: Player %d confirmed color: %s\n", i + 1, game_state->players[i].color);
        game_state->players[i].position = 1; // Start at position 1
    }

    g_print("Debug: Launching game board.\n");
    launch_game_board(game_state);
}

// Callback for player count change
// Harold Le, 400502557, 2024-12-05
/* on_player_count_changed
 *
 * Parameters: 
 *   GtkComboBoxText *widget - The combo box widget for selecting the number of players.
 *   gpointer data - Pointer to the GameState structure.
 * Side Effect: 
 *   - Updates the `num_players` field in the GameState structure.
 *   - Shows or hides player color selection widgets based on the number of players.
 *   - Logs debug information about the player count selection.
 * Description: Handles changes to the number of players, updating the GameState and 
 *              adjusting the visibility of color selection widgets.
 * Return: None.
 */
static void on_player_count_changed(GtkComboBoxText *widget, gpointer data)
{
    GameState *game_state = (GameState *)data;
    g_print("Debug: on_player_count_changed called.\n");

    const char *num_players_text = gtk_combo_box_text_get_active_text(widget);
    if (!num_players_text)
        return;

    game_state->num_players = atoi(num_players_text);
    g_print("Debug: Number of players selected: %d\n", game_state->num_players);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        gtk_widget_set_visible(color_labels[i], i < game_state->num_players);
        gtk_widget_set_visible(GTK_WIDGET(color_combos[i]), i < game_state->num_players);
    }
}

// Callback for help button
// Harold Le, 400502557, 2024-12-05
/* method_name
 *
 * Parameters: Tiles board[][] is a 2D array of size 10
 * Side Effect:
 * Description:
 * Return:
 */
static void on_help_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Debug: on_help_button_clicked called.\n");

    puts("Usage: snakesLadders [OPTIONS]");
    puts("Launch the Snakes and Ladders game with a GUI built using GTK4.");
    puts("\nOptions:");
    puts("  --help            Display this help message and exit");
    puts("\nControls:");
    puts("  Use the mouse to select the number of players and assign colors.");
    puts("  Click the 'Play' button to start the game.");
    puts("  Press the dice button to roll the dice for your turn.");
    puts("\nExamples:");
    puts("  snakesLadders          Launch the game.");
    puts("  snakesLadders --help   Show this help message.");
}

// Launch the settings window

/* launch_game_settings
 *
 * Parameters: 
 *   GtkApplication *app - The GTK application instance.
 *   gpointer user_data - Pointer to the GameState structure.
 * Side Effect: 
 *   - Creates and displays a settings window with UI components for player count and color selection.
 *   - Updates the GameState structure with the application instance.
 *   - Connects signals for player count changes, help button, and start game functionality.
 * Description: Initializes and displays the game settings screen, allowing users to select the number of players,
 *              assign colors, and proceed to the game board.
 * Return: None.
 */

void launch_game_settings(GtkApplication *app, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;
    game_state->app = app;

    g_print("Debug: launch_game_settings called.\n");

    GtkWidget *window = gtk_application_window_new(app);
    g_print("Debug: Window created: %p\n", window);

    gtk_window_set_title(GTK_WINDOW(window), "Game Settings");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    GtkWidget *label = gtk_label_new("Number of players:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    GtkWidget *combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "4");
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_player_count_changed), game_state);
    gtk_grid_attach(GTK_GRID(grid), combo_box, 1, 0, 1, 1);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        GtkWidget *color_label = gtk_label_new(g_strdup_printf("Player %d Color:", i + 1));
        gtk_grid_attach(GTK_GRID(grid), color_label, 0, i + 1, 1, 1);

        GtkWidget *color_combo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combo), "Red");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combo), "Blue");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combo), "Green");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combo), "Yellow");
        gtk_grid_attach(GTK_GRID(grid), color_combo, 1, i + 1, 1, 1);

        color_labels[i] = color_label;
        color_combos[i] = GTK_COMBO_BOX_TEXT(color_combo);

        gtk_widget_set_visible(color_label, FALSE);
        gtk_widget_set_visible(color_combo, FALSE);
    }

    GtkWidget *button = gtk_button_new_with_label("Start Game");
    g_signal_connect(button, "clicked", G_CALLBACK(on_settings_confirmed), game_state);
    gtk_grid_attach(GTK_GRID(grid), button, 0, MAX_PLAYERS + 1, 2, 1);

    // Create and add the help button
    GtkWidget *help_button = gtk_button_new_with_label("Help");
    g_signal_connect(help_button, "clicked", G_CALLBACK(on_help_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), help_button, 0, MAX_PLAYERS + 2, 2, 1);

    // // Create and add the exit button
    // GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    // g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_button_clicked), NULL);
    // gtk_grid_attach(GTK_GRID(grid), exit_button, 2, 0, 1, 1);

    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_widget_show(window);

    g_print("Debug: Settings window displayed.\n");
}
