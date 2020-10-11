#include <stdlib.h>
#include <unistd.h>

#include "game-of-decay.h"


int main(int argc, char ** argv)
{
    int rows         = 0;
    int cols         = 0;
    int max          = 1;
    int generations  = 0;
    int pad_left     = 0;
    int pad_right    = 0;
    char * map       = NULL;
    char ** char_map = NULL;
    char * cell_fmt  = NULL;

    int term_rows    = 0;
    int term_cols    = 0;

    int generation  = 0;

    parse_arguments(argc, argv, &rows, &cols, &max, &generations, &pad_left, &pad_right, &map);

    char_map = setup_char_map(max, map);

    int this_generation[rows][cols];
    int next_generation[rows][cols];

    initialize_grid(rows, cols, this_generation);
    initialize_grid(rows, cols, next_generation);

    seed_grid(rows, cols, this_generation, max);

    cell_fmt = setup_cell_fmt(pad_left, pad_right, max, map);

    while (TRUE) {

        if (generations > 0 && generation >= generations) {
            break;
        }

        print_grid(rows, cols, this_generation, char_map, cell_fmt);
        decay_grid(rows, cols, this_generation, next_generation, max);
        next_grid(rows, cols, this_generation, next_generation);

        generation++;
        sleep(1);
    }

    free(cell_fmt);
    free_char_map(max, char_map);
    if (map != NULL) {
        free(map);
    }

    return 0;
}
