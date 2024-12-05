/* Harold Le, 400502557, 2024-12-05
 * Atharva Kulkarni, 400533314, 2024-12-05
 *
 * C Script that creates the main menue
 * screen for Snakes and Ladders. It includes
 * Player selction and the help button
 * This leads to the board creation
 */
#include <gtk/gtk.h>
#include "game.h"
#include <string.h>

// Function declarations
void on_color_combo_box_changed(GtkComboBoxText *combo_box, gpointer user_data);
void on_num_players_changed(GtkComboBoxText *combo_box, gpointer user_data);
static void on_settings_confirmed(GtkWidget *widget, gpointer data);
void on_help_button_clicked(GtkWidget *widget, gpointer data);

static GtkWidget *color_combos[MAX_PLAYERS];
static GtkWidget *color_labels[MAX_PLAYERS];

// Function to load CSS
static void load_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();

    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    const gchar *css_file = "style.css";
    gtk_css_provider_load_from_path(provider, css_file);

    g_object_unref(provider);
}

void launch_game_settings(GtkApplication *app, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;
    game_state->app = app;

    g_print("Debug: launch_game_settings called.\n");

    // Load CSS
    load_css();

    GtkWidget *window = gtk_application_window_new(app);
    g_print("Debug: Window created: %p\n", window);

    gtk_window_set_title(GTK_WINDOW(window), "Game Settings");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    game_state->grid = grid; // Assign the grid to game_state

    GtkWidget *label = gtk_label_new("Number of players:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    GtkWidget *combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "4");
    gtk_grid_attach(GTK_GRID(grid), combo_box, 1, 0, 1, 1);

    // Connect the combo box to a callback function to update the number of players
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_num_players_changed), game_state);

    // Add "Start Game" button
    GtkWidget *start_button = gtk_button_new_with_label("Start Game");
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_settings_confirmed), game_state);
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, MAX_PLAYERS + 2, 1, 1);

    // Add "Help" button
    GtkWidget *help_button = gtk_button_new_with_label("Help");
    g_signal_connect(help_button, "clicked", G_CALLBACK(on_help_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), help_button, 1, MAX_PLAYERS + 2, 1, 1);

    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_widget_show(window);
}

// Callback function to update the number of players
void on_num_players_changed(GtkComboBoxText *combo_box, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;
    const char *num_players_text = gtk_combo_box_text_get_active_text(combo_box);
    int num_players = atoi(num_players_text);

    // Remove existing color selection widgets if any
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (color_labels[i])
        {
            gtk_widget_unparent(color_labels[i]);
            color_labels[i] = NULL;
        }
        if (color_combos[i])
        {
            gtk_widget_unparent(color_combos[i]);
            color_combos[i] = NULL;
        }
    }

    // Add color selection combo boxes for the selected number of players
    for (int i = 0; i < num_players; i++)
    {
        color_labels[i] = gtk_label_new(g_strdup_printf("Player %d Color:", i + 1));
        gtk_grid_attach(GTK_GRID(game_state->grid), color_labels[i], 0, i + 1, 1, 1);

        color_combos[i] = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Red");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Blue");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Green");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Yellow");
        gtk_grid_attach(GTK_GRID(game_state->grid), color_combos[i], 1, i + 1, 1, 1);

        // Connect the combo box to a callback function to update the player's color
        g_signal_connect(color_combos[i], "changed", G_CALLBACK(on_color_combo_box_changed), &game_state->players[i]);
    }

    gtk_widget_show(GTK_WIDGET(game_state->grid));
}

// Callback function to update player's color
void on_color_combo_box_changed(GtkComboBoxText *combo_box, gpointer user_data)
{
    Player *player = (Player *)user_data;
    const char *color = gtk_combo_box_text_get_active_text(combo_box);
    if (color)
    {
        strncpy(player->color, color, sizeof(player->color) - 1);
        player->color[sizeof(player->color) - 1] = '\0'; // Ensure null-termination
        g_print("Debug: Player color set to %s\n", player->color);
    }
}

// Callback for "Start Game" button
static void on_settings_confirmed(GtkWidget *widget, gpointer data)
{
    GameState *game_state = (GameState *)data;

    g_print("Debug: on_settings_confirmed called.\n");

    // Read number of players from combo box
    GtkComboBoxText *combo_box = GTK_COMBO_BOX_TEXT(gtk_grid_get_child_at(GTK_GRID(game_state->grid), 1, 0));
    if (GTK_IS_COMBO_BOX_TEXT(combo_box)) {
        const char *num_players_text = gtk_combo_box_text_get_active_text(combo_box);
        if (num_players_text) {
            game_state->num_players = atoi(num_players_text);
            g_print("Debug: Number of players set to %d\n", game_state->num_players);
        } else {
            g_print("Error: Failed to get active text from combo box.\n");
            return;
        }
    } else {
        g_print("Error: Failed to get combo box from grid.\n");
        return;
    }

    // Validate player colors
    for (int i = 0; i < game_state->num_players; i++)
    {
        const char *color = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(color_combos[i]));
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

// Callback for "Help" button
void on_help_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Help button clicked. Display help information here.\n");
    // Implement help dialog or information display here
}
