#include <gtk/gtk.h>
#include "game.h"

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    GameState game_state = {0};

    g_print("Debug: Initializing GtkApplication...\n");
    app = gtk_application_new("org.example.snake_ladder", G_APPLICATION_FLAGS_NONE);
    g_print("Debug: GtkApplication initialized: %p\n", app);

    g_signal_connect(app, "activate", G_CALLBACK(launch_game_settings), &game_state);
    g_print("Debug: Signal connected to launch_game_settings.\n");

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_print("Debug: g_application_run returned with status: %d\n", status);

    g_object_unref(app);
    g_print("Debug: GtkApplication unref completed.\n");

    return status;
}
