/* Prashanta Kandel, 400521817, 2024-12-05
 *
 * C Script test out the functions
 * for the creation of the game
 */
#include "game.h"

/* test_launch_game_board
 *
 * Parameters: N/A
 * Side Effect: N/A
 * Description: test out launch_game_board function
 * Return: void
 */
void test_launch_game_board()
{
    // Mock a GameState
    GameState game_state = {0};

    // Initialize GTK
    GtkApplication *app = gtk_application_new("com.example.snakesladders", G_APPLICATION_FLAGS_NONE);
    game_state.app = app;

    // Call the function
    launch_game_board(&game_state);

    // Validate grid and overlay
    if (game_state.overlay && GTK_IS_OVERLAY(game_state.overlay))
    {
        g_print("Test Passed: Overlay created successfully.\n");
    }
    else
    {
        g_print("Test Failed: Overlay not created.\n");
    }

    // Validate buttons in the grid
    int all_buttons_created = 1;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (!GTK_IS_BUTTON(game_state.board[i][j].button))
            {
                all_buttons_created = 0;
                break;
            }
        }
    }
    if (all_buttons_created)
    {
        g_print("Test Passed: All board buttons created successfully.\n");
    }
    else
    {
        g_print("Test Failed: Some board buttons are missing.\n");
    }

    // Clean up
    g_object_unref(app);
}

/* test_initialize_board
 *
 * Parameters: N/A
 * Side Effect: N/A
 * Description: test out initilized_board function
 * Return: void
 */
void test_initialize_board()
{
    Tile board[BOARD_SIZE][BOARD_SIZE];

    // Call the function to initialize the board
    initialize_board(board);

    // Test 1: Verify the numbering of the tiles (1 to BOARD_SIZE * BOARD_SIZE)
    int passed_numbering_test = 1;

    // Check the first row (bottom of the board)
    int expected_number = 1;
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        if (board[BOARD_SIZE - 1][j].number != expected_number)
        {
            printf("Test Failed: Tile numbering is incorrect at first row, board[%d][%d]. Expected %d, got %d.\n",
                   BOARD_SIZE - 1, j, expected_number, board[BOARD_SIZE - 1][j].number);
            passed_numbering_test = 0;
        }
        expected_number++;
    }

    // Check the last row (top of the board)
    expected_number = 100;
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        if (board[0][j].number != expected_number)
        {
            printf("Test Failed: Tile numbering is incorrect at last row, board[0][%d]. Expected %d, got %d.\n",
                   j, expected_number, board[0][j].number);
            passed_numbering_test = 0;
        }
        expected_number--;
    }

    if (passed_numbering_test)
    {
        printf("Test Passed: Tile numbering for first and last rows is correct.\n");
    }

    // Test 2: Verify the snake and ladder assignments
    // Snakes
    if (board[0][1].snake_to == 28 && board[3][1].snake_to == 19 &&
        board[5][2].snake_to == 18 && board[1][8].snake_to == 53)
    {
        printf("Test Passed: Snakes are correctly assigned.\n");
    }
    else
    {
        printf("Test Failed: Snake assignments are incorrect.\n");
    }

    // Ladders
    if (board[9][3].ladder_to == 24 && board[8][7].ladder_to == 44 &&
        board[6][7].ladder_to == 50 && board[9][6].ladder_to == 56)
    {
        printf("Test Passed: Ladders are correctly assigned.\n");
    }
    else
    {
        printf("Test Failed: Ladder assignments are incorrect.\n");
    }
}

/* test_launch_game_settings
 *
 * Parameters: GTK applicatioon
 * Side Effect: N/A
 * Description: test out game settings
 * Return: void
 */
void test_launch_game_settings(GtkApplication *app)
{
    g_print("Testing launch_game_settings...\n");

    // Prepare game state
    GameState game_state;
    game_state.app = app;

    // Call the function to test
    launch_game_settings(app, &game_state);

    // Get the main window
    GtkWidget *window = GTK_WIDGET(gtk_application_get_active_window(app));
    if (!window)
    {
        g_print("Test Failed: No active window found.\n");
        return;
    }
    g_print("Test Passed: Main window created.\n");

    // Check if the grid is present
    GtkWidget *grid = gtk_widget_get_first_child(window);
    if (!GTK_IS_GRID(grid))
    {
        g_print("Test Failed: Grid not found as the child of the window.\n");
        return;
    }
    g_print("Test Passed: Grid found in the window.\n");

    // Iterate through the grid's children to check widgets
    GtkWidget *child = gtk_widget_get_first_child(grid);
    int player_label_found = 0, start_button_found = 0, help_button_found = 0;

    while (child)
    {
        const char *label_text = NULL;

        if (GTK_IS_LABEL(child))
        {
            label_text = gtk_label_get_text(GTK_LABEL(child));
            if (label_text && g_strcmp0(label_text, "Number of players:") == 0)
            {
                player_label_found = 1;
                g_print("Test Passed: 'Number of players' label found.\n");
            }
        }
        else if (GTK_IS_BUTTON(child))
        {
            label_text = gtk_button_get_label(GTK_BUTTON(child));
            if (label_text && g_strcmp0(label_text, "Start Game") == 0)
            {
                start_button_found = 1;
                g_print("Test Passed: 'Start Game' button found.\n");
            }
            else if (label_text && g_strcmp0(label_text, "Help") == 0)
            {
                help_button_found = 1;
                g_print("Test Passed: 'Help' button found.\n");
            }
        }

        child = gtk_widget_get_next_sibling(child);
    }

    // Final results
    if (!player_label_found)
        g_print("Test Failed: 'Number of players' label not found.\n");
    if (!start_button_found)
        g_print("Test Failed: 'Start Game' button not found.\n");
    if (!help_button_found)
        g_print("Test Failed: 'Help' button not found.\n");

    if (player_label_found && start_button_found && help_button_found)
        g_print("Test Passed: All key components found.\n");
    else
        g_print("Test Failed: Some components are missing.\n");
}

/* on_activate
 *
 * Parameters: GTK application and pointer to user_data
 * Side Effect: N/A
 * Description: call the test_launch_game_settings to test
 * in program
 * Return: int
 */
void on_activate(GtkApplication *app, gpointer user_data)
{
    g_print("Starting tests...\n");
    test_launch_game_settings(app); // Test the `launch_game_settings` function
    g_print("Tests completed.\n");
    g_application_quit(G_APPLICATION(app)); // Quit application after testing
}

/* main
 *
 * Parameters: N/A
 * Side Effect: N/A
 * Description: test out indivisual functions
 * in program
 * Return: int
 */
int main(int argc, char *argv[])
{
    gtk_init();

    g_print("Running test: test_launch_game_board\n");
    test_launch_game_board(); // test 1
    test_initialize_board();  // test 2

    // test 3
    GtkApplication *app = gtk_application_new("com.example.snakesandladders", G_APPLICATION_FLAGS_NONE);
    int status;

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return 0;
}
