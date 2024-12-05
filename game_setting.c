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

// Function to apply CSS
static void apply_css(GtkWidget *widget, const char *css)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

// Launch the settings window
void launch_game_settings(GtkApplication *app, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;
    game_state->app = app;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Game Settings");

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_margin_start(grid, 30);
    gtk_widget_set_margin_end(grid, 30);
    gtk_widget_set_margin_top(grid, 30);
    gtk_widget_set_margin_bottom(grid, 30);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    GtkWidget *num_players_label = gtk_label_new("Number of Players:");
    gtk_grid_attach(GTK_GRID(grid), num_players_label, 0, 0, 1, 1);

    GtkWidget *num_players_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(num_players_combo), "1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(num_players_combo), "2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(num_players_combo), "3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(num_players_combo), "4");
    gtk_grid_attach(GTK_GRID(grid), num_players_combo, 1, 0, 1, 1);
    g_signal_connect(num_players_combo, "changed", G_CALLBACK(on_num_players_changed), game_state);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        color_labels[i] = gtk_label_new(NULL);
        gtk_grid_attach(GTK_GRID(grid), color_labels[i], 0, i + 1, 1, 1);

        color_combos[i] = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Red");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Blue");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Green");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(color_combos[i]), "Yellow");
        gtk_grid_attach(GTK_GRID(grid), color_combos[i], 1, i + 1, 1, 1);
        g_signal_connect(color_combos[i], "changed", G_CALLBACK(on_color_combo_box_changed), &game_state->players[i]);

        // Hide color options initially
        gtk_widget_hide(color_labels[i]);
        gtk_widget_hide(color_combos[i]);

        // Set the text color to black using CSS class
        GtkWidget *entry = gtk_widget_get_first_child(GTK_WIDGET(color_combos[i]));
        if (GTK_IS_ENTRY(entry)) {
            gtk_widget_add_css_class(entry, "combo-box-entry");
        }
    }

    GtkWidget *start_button = gtk_button_new_with_label("Start Game");
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, MAX_PLAYERS + 1, 2, 1);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_settings_confirmed), game_state);

    GtkWidget *help_button = gtk_button_new_with_label("Help");
    gtk_grid_attach(GTK_GRID(grid), help_button, 0, MAX_PLAYERS + 2, 2, 1);
    g_signal_connect(help_button, "clicked", G_CALLBACK(on_help_button_clicked), NULL);

    gtk_widget_show(window);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // Make window non-resizable
    gtk_window_set_default_size(GTK_WINDOW(window), -1, -1); // Adjust window size to fit content
}

// Callback function to update the number of players
void on_num_players_changed(GtkComboBoxText *combo_box, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;
    const gchar *num_players_str = gtk_combo_box_text_get_active_text(combo_box);
    int num_players = atoi(num_players_str);

    game_state->num_players = num_players;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (i < num_players) {
            gtk_widget_show(color_labels[i]);
            gtk_widget_show(color_combos[i]);
        } else {
            gtk_widget_hide(color_labels[i]);
            gtk_widget_hide(color_combos[i]);
        }
    }

    // Adjust window size to fit content
    GtkWidget *window = gtk_widget_get_ancestor(GTK_WIDGET(combo_box), GTK_TYPE_WINDOW);
    gtk_window_set_default_size(GTK_WINDOW(window), -1, -1);
}

// Callback function to update player's color
void on_color_combo_box_changed(GtkComboBoxText *combo_box, gpointer user_data)
{
    const gchar *color = gtk_combo_box_text_get_active_text(combo_box);

    // Set the text color to black using CSS class
    GtkWidget *entry = gtk_widget_get_first_child(GTK_WIDGET(combo_box));
    if (GTK_IS_ENTRY(entry)) {
        gtk_widget_add_css_class(entry, "combo-box-entry");
    }

    Player *player = (Player *)user_data;
    strncpy(player->color, color, sizeof(player->color) - 1);
    player->color[sizeof(player->color) - 1] = '\0';

    g_print("Player color changed to: %s\n", color);
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
        }
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
        game_state->players[i].position = 1; // Start at position 1
    }

    g_print("Debug: Launching game board.\n");
    launch_game_board(game_state);
}

// Callback for "Help" button
void on_help_button_clicked(GtkWidget *widget, gpointer data)
{
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
