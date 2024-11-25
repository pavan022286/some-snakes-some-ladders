#include <gtk/gtk.h>
#include <stdbool.h>

// Data structure to store player selections
typedef struct {
    GtkComboBoxText *player_count;
    GtkComboBoxText *player_colors[4]; // Max 4 players
    GtkWidget *color_labels[4];        // Labels for players
    GtkWidget *color_combos[4];        // Combo boxes for players
} PlayerSelection;

// Helper function to check for duplicate colors
static bool is_duplicate_color(PlayerSelection *selection, int num_players) {
    const char *selected_colors[4] = {NULL};

    for (int i = 0; i < num_players; i++) {
        selected_colors[i] = gtk_combo_box_text_get_active_text(selection->player_colors[i]);
        if (!selected_colors[i]) {
            return false; // If any color is not selected, no duplicates yet
        }
        for (int j = 0; j < i; j++) {
            if (strcmp(selected_colors[i], selected_colors[j]) == 0) {
                return true; // Duplicate found
            }
        }
    }
    return false;
}

// Callback for when "OK!" button is clicked
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    PlayerSelection *selection = (PlayerSelection *)data;

    // Get the number of players selected
    const char *num_players_text = gtk_combo_box_text_get_active_text(selection->player_count);
    if (!num_players_text) {
        g_print("Please select the number of players.\n");
        return;
    }

    int num_players = atoi(num_players_text);

    // Validate colors and check for duplicates
    if (is_duplicate_color(selection, num_players)) {
        g_print("Players cannot select the same color.\n");
        return;
    }

    g_print("Number of players: %d\n", num_players);
    for (int i = 0; i < num_players; i++) {
        const char *color = gtk_combo_box_text_get_active_text(selection->player_colors[i]);
        g_print("Player %d color: %s\n", i + 1, color ? color : "Not selected");
    }
}

// Callback for when player count changes
static void on_player_count_changed(GtkComboBoxText *widget, gpointer data) {
    PlayerSelection *selection = (PlayerSelection *)data;

    const char *num_players_text = gtk_combo_box_text_get_active_text(widget);
    if (!num_players_text) {
        return;
    }

    int num_players = atoi(num_players_text);

    // Show/hide player color selection based on number of players
    for (int i = 0; i < 4; i++) {
        if (i < num_players) {
            gtk_widget_show(selection->color_labels[i]);
            gtk_widget_show(selection->color_combos[i]);
        } else {
            gtk_widget_hide(selection->color_labels[i]);
            gtk_widget_hide(selection->color_combos[i]);
        }
    }
}

// Function to add player color selection widgets
static void add_color_selection(GtkGrid *grid, PlayerSelection *selection) {
    const char *colors[] = {"Red", "Blue", "Green", "Yellow"};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < 4; i++) {
        // Create label for player
        selection->color_labels[i] = gtk_label_new(g_strdup_printf("Player %d Color:", i + 1));
        gtk_grid_attach(grid, selection->color_labels[i], 0, 2 + i, 1, 1);

        // Create combo box for player colors
        GtkWidget *combo = gtk_combo_box_text_new();
        for (int j = 0; j < num_colors; j++) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), colors[j]);
        }
        gtk_grid_attach(grid, combo, 1, 2 + i, 1, 1);
        selection->player_colors[i] = GTK_COMBO_BOX_TEXT(combo);
        selection->color_combos[i] = combo;

        // Hide these by default
        gtk_widget_hide(selection->color_labels[i]);
        gtk_widget_hide(selection->color_combos[i]);
    }
}

// GTK application activation function
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *combo_box;
    GtkWidget *button;

    // Allocate memory for player selection data
    PlayerSelection *selection = g_malloc(sizeof(PlayerSelection));

    // Create window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Snake and Ladder Setup");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create grid layout
    grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // Add label for player count
    label = gtk_label_new("Choose number of players:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Create combo box for player count
    combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "4");
    gtk_grid_attach(GTK_GRID(grid), combo_box, 1, 0, 1, 1);
    selection->player_count = GTK_COMBO_BOX_TEXT(combo_box);

    // Connect signal for player count changes
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_player_count_changed), selection);

    // Add color selection widgets
    add_color_selection(GTK_GRID(grid), selection);

    // Add OK button
    button = gtk_button_new_with_label("OK!");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), selection);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 2, 1);

    // Set the grid as the window content
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Show the window
    gtk_widget_show(window);
}

// Main function
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Create GTK application
    app = gtk_application_new("org.example.snake_ladder", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
