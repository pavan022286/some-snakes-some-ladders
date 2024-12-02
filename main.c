/* Prashanta Kandel, 400521817, 2024-12-05
 *
 * C Script the driver class that starts
 * the creation of Some Snakes Some Ladders
 * game
 */
#include <gtk/gtk.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void help();

/* main
 *
 * Parameters: argc, argv
 * Side Effect: N/A
 * Description: Display help for --help argument used
 * and create and start GTK aplication
 * Return: int status
 */
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    // HELP FLAG
    if (argc > 1)
    {
        // there is an argument
        if (strcmp(argv[1], "--help") == 0)
        {
            // if --help flag is used
            help();
            return EXIT_SUCCESS;
        }
        else
        {
            // wrong option used
            return EXIT_FAILURE;
        }
    }

    GameState game_state = {0};

    g_print("Debug: Initializing GtkApplication...\n");
    app = gtk_application_new("org.example.snake_ladder", G_APPLICATION_FLAGS_NONE); // steps to start GTK application
    g_print("Debug: GtkApplication initialized: %p\n", app);

    g_signal_connect(app, "activate", G_CALLBACK(launch_game_settings), &game_state);
    g_print("Debug: Signal connected to launch_game_settings.\n");

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_print("Debug: g_application_run returned with status: %d\n", status);

    g_object_unref(app);
    g_print("Debug: GtkApplication unref completed.\n");

    srand(time(NULL)); // seeds rng (makes sure result is truly random)

    return status;
}

/* help
 *
 * Parameters: N/A
 * Side Effect: N/A
 * Description: Display help message
 * Return: void
 */
void help()
{
    puts("Usage: snakesLadders");
    puts("Program creates GUI using GTK4");
    puts("Select number of players and the colour for each player");
    puts("Click play, press the button to role the dice");
    puts("First player to reach tile 100 wins, Good Luck!");
    puts("Options:");
    puts("  --help            Display this help message and exit");
    puts("Examples:");
    puts("  snakesLadders --help");
}
