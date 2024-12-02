/* Name, student number, 2024-12-05
 *
 * C Script that darws the snakes
 * and ladders using cairo for
 * appliaction
 */
#include <gtk/gtk.h>
#include <gtk/gtk.h>
#include "game.h"
#include <string.h>
#include <math.h>

/* method_name
 *
 * Parameters: Tiles board[][] is a 2D array of size 10
 * Side Effect:
 * Description:
 * Return:
 */
void draw_snake(cairo_t *cr, double start_x, double start_y, double end_x, double end_y)
{
    // using three curves for snake
    const int num_segments = 3;
    double start_width = 12.0;
    double end_width = 0.5;

    // calculate overall direction
    double angle = atan2(end_y - start_y, end_x - start_x);
    double distance = sqrt(pow(end_x - start_x, 2) + pow(end_y - start_y, 2));
    double wave_amplitude = distance * 0.15;

    // draw body of snake
    cairo_set_source_rgb(cr, 0.8, 0.2, 0.2); // set to red
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

    cairo_move_to(cr, start_x, start_y);

    for (int i = 0; i < num_segments; i++)
    {
        double t = (double)i / num_segments;
        double next_t = (double)(i + 1) / num_segments;

        double seg_start_x = start_x + t * (end_x - start_x);
        double seg_start_y = start_y + t * (end_y - start_y);
        double seg_end_x = start_x + next_t * (end_x - start_x);
        double seg_end_y = start_y + next_t * (end_y - start_y);

        double offset = (i % 2 == 0 ? 1 : -1) * wave_amplitude;
        double ctrl1_x = seg_start_x + (seg_end_x - seg_start_x) / 3 + offset * cos(angle + M_PI / 2);
        double ctrl1_y = seg_start_y + (seg_end_y - seg_start_y) / 3 + offset * sin(angle + M_PI / 2);
        double ctrl2_x = seg_start_x + 2 * (seg_end_x - seg_start_x) / 3 + offset * cos(angle + M_PI / 2);
        double ctrl2_y = seg_start_y + 2 * (seg_end_y - seg_start_y) / 3 + offset * sin(angle + M_PI / 2);

        double progress = t * t;
        double current_width = start_width + (end_width - start_width) * progress;
        cairo_set_line_width(cr, current_width);

        cairo_curve_to(cr, ctrl1_x, ctrl1_y, ctrl2_x, ctrl2_y, seg_end_x, seg_end_y);
        cairo_stroke(cr);
        cairo_move_to(cr, seg_end_x, seg_end_y);
    }

    // draw snake head
    cairo_set_source_rgb(cr, 0.8, 0.2, 0.2);
    cairo_save(cr);
    cairo_translate(cr, start_x, start_y);
    cairo_scale(cr, start_width / 1.5, start_width / 1.5);
    cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_restore(cr);

    // draw eyes
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_save(cr);
    cairo_translate(cr, start_x - start_width / 3, start_y - start_width / 3);
    cairo_scale(cr, 4, 4);
    cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_restore(cr);

    cairo_save(cr);
    cairo_translate(cr, start_x + start_width / 3, start_y - start_width / 3);
    cairo_scale(cr, 4, 4);
    cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_restore(cr);

    // draw pupils
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_save(cr);
    cairo_translate(cr, start_x - start_width / 3, start_y - start_width / 3);
    cairo_scale(cr, 2, 2);
    cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_restore(cr);

    cairo_save(cr);
    cairo_translate(cr, start_x + start_width / 3, start_y - start_width / 3);
    cairo_scale(cr, 2, 2);
    cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_restore(cr);
}

/* method_name
 *
 * Parameters: Tiles board[][] is a 2D array of size 10
 * Side Effect:
 * Description:
 * Return:
 */
void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data)
{
    GameState *game_state = (GameState *)user_data;

    // Set line properties and clear background
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_paint(cr);

    // Draw all snakes and ladders
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Tile *current_tile = &game_state->board[i][j];

            if (current_tile->snake_to > 0 || current_tile->ladder_to > 0)
            {

                int target_number = current_tile->snake_to > 0 ? current_tile->snake_to : current_tile->ladder_to;

                int target_row = (target_number - 1) / BOARD_SIZE;
                target_row = BOARD_SIZE - 1 - target_row;

                int target_col;
                if (target_row % 2 == 0)
                {

                    target_col = BOARD_SIZE - (target_number - (BOARD_SIZE - 1 - target_row) * BOARD_SIZE);
                }
                else
                {

                    target_col = (target_number - 1) % BOARD_SIZE;
                }

                Tile *target_tile = &game_state->board[target_row][target_col];

                if (current_tile->snake_to > 0)
                {
                    // Draw snake
                    draw_snake(cr, current_tile->x, current_tile->y,
                               target_tile->x, target_tile->y);
                }
                else
                {
                    // Draw ladder
                    cairo_set_source_rgb(cr, 0.4, 0.6, 0.1);
                    cairo_set_line_width(cr, 3.0);

                    double angle = atan2(target_tile->y - current_tile->y,
                                         target_tile->x - current_tile->x);
                    double perpendicular = angle + M_PI / 2;
                    double spacing = 10.0;

                    // Draw parallel lines
                    for (int offset = -1; offset <= 1; offset += 2)
                    {
                        double dx = spacing * cos(perpendicular) * offset;
                        double dy = spacing * sin(perpendicular) * offset;

                        cairo_move_to(cr, current_tile->x + dx, current_tile->y + dy);
                        cairo_line_to(cr, target_tile->x + dx, target_tile->y + dy);
                        cairo_stroke(cr);
                    }

                    // Draw rungs
                    int num_rungs = 5;
                    for (int k = 0; k <= num_rungs; k++)
                    {
                        double t = (double)k / num_rungs;
                        double x = current_tile->x + t * (target_tile->x - current_tile->x);
                        double y = current_tile->y + t * (target_tile->y - current_tile->y);

                        cairo_move_to(cr, x - spacing * cos(perpendicular),
                                      y - spacing * sin(perpendicular));
                        cairo_line_to(cr, x + spacing * cos(perpendicular),
                                      y + spacing * sin(perpendicular));
                        cairo_stroke(cr);
                    }
                }
            }
        }
    }
}