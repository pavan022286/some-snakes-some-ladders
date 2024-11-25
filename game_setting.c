#include <gtk/gtk.h>
#include "game.h"
#include <string.h>

static GtkComboBoxText *color_combos[MAX_PLAYERS];
static GtkWidget *color_labels[MAX_PLAYERS];

// Callback for "Start Game" button
static void on_settings_confirmed(GtkWidget *widget, gpointer data) {
    GameState *game_state = (GameState *)data;

    g_print("Debug: on_settings_confirmed called.\n");

    for (int i = 0; i < game_state->num_players; i++) {
        const char *color = gtk_combo_box_text_get_active_text(color_combos[i]);
        g_print("Debug: Player %d selected color: %s\n", i + 1, color);

        if (!color) {
            g_print("Player %d must select a color.\n", i + 1);
            return;
        }

        for (int j = 0; j < i; j++) {
            if (strcmp(color, game_state->players[j].color) == 0) {
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
static void on_player_count_changed(GtkComboBoxText *widget, gpointer data) {
    GameState *game_state = (GameState *)data;
    g_print("Debug: on_player_count_changed called.\n");

    const char *num_players_text = gtk_combo_box_text_get_active_text(widget);
    if (!num_players_text) return;

    game_state->num_players = atoi(num_players_text);
    g_print("Debug: Number of players selected: %d\n", game_state->num_players);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        gtk_widget_set_visible(color_labels[i], i < game_state->num_players);
        gtk_widget_set_visible(GTK_WIDGET(color_combos[i]), i < game_state->num_players);
    }
}

// Launch the settings window
void launch_game_settings(GtkApplication *app, gpointer user_data) {
    GameState *game_state = (GameState *)user_data;

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

    for (int i = 0; i < MAX_PLAYERS; i++) {
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

    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_widget_show(window);

    g_print("Debug: Settings window displayed.\n");
}
