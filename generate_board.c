#include <gtk/gtk.h>

#define BOARD_SIZE 10

// Tile structure to store information about each tile
typedef struct
{
    int number;    // Tile number
    int snake_to;  // Tile to go to if there's a snake (0 if no snake)
    int ladder_to; // Tile to go to if there's a ladder (0 if no ladder)
} Tile;

// Function to create a button for each tile
GtkWidget *create_tile_button(Tile tile)
{
    char label[20];
    snprintf(label, sizeof(label), "%d", tile.number); // Display the tile number
    GtkWidget *button = gtk_button_new_with_label(label);

    // Store information in the button's user data (tile information)
    g_object_set_data(G_OBJECT(button), "tile", &tile);

    return button;
}

// Function to handle button clicks (player movement)
static void on_tile_button_clicked(GtkButton *button, gpointer user_data)
{
    Tile *tile = g_object_get_data(G_OBJECT(button), "tile");

    printf("You clicked Tile %d\n", tile->number);

    // Handle movement for snake or ladder
    if (tile->snake_to != 0)
    {
        printf("Snake! Go to tile %d\n", tile->snake_to);
    }
    else if (tile->ladder_to != 0)
    {
        printf("Ladder! Go to tile %d\n", tile->ladder_to);
    }
}

// Function to initialize the game board
void initialize_board(Tile board[BOARD_SIZE][BOARD_SIZE])
{
    int counter = 100; // Start with tile number 100
    int k = 0;
    for (int i = 0; i < BOARD_SIZE; i++) // Loop from 0 to BOARD_SIZE - 1 (rows)
    {
        if (k % 2 == 0)
        {
            // even mens count nomally
            for (int j = 0; j < BOARD_SIZE; j++) // Loop from 0 to BOARD_SIZE - 1 (columns)
            {
                board[i][j].number = counter;
                board[i][j].snake_to = 0;  // No snake by default
                board[i][j].ladder_to = 0; // No ladder by default
                counter--;                 // Decrement the counter to number tiles in reverse
            }
        }
        else
        {
            // odd means count backwards
            int reverseCount = counter - 9;      // we start with the oppsite one and count up
            for (int j = 0; j < BOARD_SIZE; j++) // Loop from 0 to BOARD_SIZE - 1 (columns)
            {
                board[i][j].number = reverseCount;
                board[i][j].snake_to = 0;  // No snake by default
                board[i][j].ladder_to = 0; // No ladder by default
                counter--;                 // Decrement the counter to number tiles in reverse
                reverseCount++;
            }
        }
        k++;
    }

    // Example: Adding a snake from tile 16 to 6
    board[1][5].snake_to = 6; // Tile 16 has a snake to tile 6

    // Example: Adding a ladder from tile 3 to 22
    board[0][2].ladder_to = 22; // Tile 3 has a ladder to tile 22
}

// The activate function that gets called when the application is activated
static void activate(GtkApplication *app, gpointer user_data)
{
    // Create the main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Some Snakes and Some Ladders");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    // Create a fixed container to place widgets with coordinates
    GtkWidget *fixed = gtk_fixed_new();

    // Add the fixed container to the window (GTK4)
    gtk_window_set_child(GTK_WINDOW(window), fixed); // GTK4 method

    // Create a button
    GtkWidget *button = gtk_button_new_with_label("Roll Dice");

    // Place the button at coordinates (x=100, y=150) on the window
    gtk_fixed_put(GTK_FIXED(fixed), button, 250, 400);

    // Create a grid container
    GtkWidget *grid = gtk_grid_new();

    // Initialize the board
    Tile board[BOARD_SIZE][BOARD_SIZE];
    initialize_board(board);

    // Add buttons to the grid for each tile
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            GtkWidget *button = create_tile_button(board[i][j]);
            g_signal_connect(button, "clicked", G_CALLBACK(on_tile_button_clicked), NULL);
            gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1); // Attach to grid
        }
    }

    // Set the window's child to the grid (this replaces gtk_container_add)
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Show the window and its contents
    gtk_widget_show(window);
}

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    // Create the application and connect the activate function
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app); // Clean up the application object
    return status;
}

