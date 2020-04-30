#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>

#include "game-of-decay.h"


// if full random is false, then we pick about PERCENTAGE_DEAD % to kill from the beginning, otherwise it gets weird :)
#define FULL_RANDOM FALSE
#define PERCENTAGE_DEAD 50

/*
    * < 200% neighbor neighbors (2 live cells, STAY_ALIVE_MIN) will kill a live cell
    * >= 200% neighbor neighbors (2 live cells, STAY_ALIVE_MIN) and <= 300% (3 live cells, COME_ALIVE) will keep a cell alive
    * == 300% (3 live cells, COME_ALIVE) will bring a dead cell to life
    * > 300% (3 live cells, COME_ALIVE) will kill a live cell
*/
#define STAY_ALIVE_MIN 2 /* 200% */
#define COME_ALIVE     3 /* 300% */






int print_version()
{
    printf("%s\n", "Hedens Game of Decay - version 0.0.0\n");
}


int print_help()
{
    print_version();
    printf("%s\n", "--------------------");
    printf("%s", "\n");
    printf("%s\n", "-= based on Conways Game of Life =-");
    printf("%s", "\n");
    printf("%s", "\n");
    printf("%s\n", "Options:");
    printf("%s", "\n");
    printf("%s\n", "-h,--help         Print this help and exit");
    printf("%s\n", "-v,--version      Print the current version and exit");
    printf("%s\n", "-r,--rows         Specify how many rows of cells to iterate over");
    printf("%s\n", "-c,--cols         Specify how many columns of cells to iterate over");
    printf("%s\n", "-R,--range        Specify a range of values [X-Y]");
    printf("%s\n", "                  default: 0-1, this reproduces Conways Game of Life");
    printf("%s\n", "-m,--map          Specify a map for values");
    printf("%s\n", "                  Each value can be space, comma, or semi-colon delimited");
    printf("%s\n", "                  default: \"0= ,1=1,2=2,3=3,...Y=Y\"");
    printf("%s\n", "-g,--generations  Specify maximum number of generations to print");
    printf("%s\n", "                  minimum: 1");
    printf("%s", "\n");
    printf("%s", "\n");
}


int parse_range(char * range, int * min, int * max)
{
    char * token = strtok(range, "-");

    if (token == NULL) {
        return FALSE;
    }

    * min = atoi(token);

    token = strtok(NULL, "-");

    if (token == NULL) {
        return FALSE;
    }

    * max = atoi(token);

    if ((* min) < (* max)) {
        if ((* min) >= 0 && (* max) <= MAX_RANGE_MAX) {
            return TRUE;
        }
    }

    return FALSE;
}


int parse_arguments(int argc, char **argv, game_options * opts)
{
    char c = 0;
    int option_index = 0;

    static struct option long_options[] = {
        { "help",        no_argument,       0, 'h' },
        { "version",     no_argument,       0, 'v' },
        { "rows",        optional_argument, 0, 'r' },
        { "cols",        optional_argument, 0, 'c' },
        { "range",       optional_argument, 0, 'R' },
        { "map",         optional_argument, 0, 'm' },
        { "generations", optional_argument, 0, 'g' },
    };

    while (TRUE) {

        c = getopt_long(argc, argv, "hvr:c:R:m:g:", long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {

        case 'h':
            return print_help();
            break;

        case 'v':
            return print_version();
            break;

        case 'r':
            opts->rows = atoi(optarg);
            break;

        case 'c':
            opts->cols = atoi(optarg);
            break;

        case 'R':
            opts->range_string = strdup(optarg);
            //result = parse_range(optarg, &range_min, &range_max);
            break;

        case 'm':
            opts->map_string = strdup(optarg);
            break;

        case 'g':
            opts->max_generations = atoi(optarg);
            break;
        }
    }
}


int set_default_map(game_options * opts)
{
    int i = 0;

    opts->max_default_map_entry_len = snprintf(NULL, 0, "%d", opts->range_max);

    // but first set the default values - we can override these when
    // we parse the map string
    for (i = 0; i <= opts->range_max; i++) {
        opts->map[i] = calloc(opts->max_default_map_entry_len + 1, sizeof(char));
        snprintf(opts->map[i], opts->max_default_map_entry_len + 1, "%d", i);
    }
}


int parse_map_string(game_options * opts)
{
    int i = 0;
    char * ptr = NULL;

    if (opts->map_string == NULL) {
        return TRUE;
    }

    // now lets parse the map_string and get our matches setup
    ptr = strtok(opts->map_string, "; ,");
    while (ptr != NULL) {

        i = 0;

        char key[opts->max_default_map_entry_len];
        int map_key = 0;
        char value[MAX_TEMP_BUFFER] = { 0 };
        size_t value_len = 0;

        char * token = strdup(ptr);
        char * free_token = token;
        char c = 0;
        int processing_key = TRUE;

        // probably a much more efficient way to do this
        // i just didn't want to interrupt strtok on the potentially
        // long string
        while (c = *token++) {
            if (processing_key == TRUE) {
                if (c >= '0' && c <= '9') {
                    key[i] = c;
                }
                else if (c == '=') {
                    processing_key = FALSE;
                    key[i] = 0;

                    // just set it to -1 so it ends up at 0 on the
                    // next pass for value[i]
                    i = -1;
                }
            }
            else {
                value[i] = c;
            }
            i++;
        }

        value_len = strlen(value);

        if (value_len > opts->max_map_entry_len) {
            opts->max_map_entry_len = value_len;
        }

        map_key = atoi(key);

        // first make sure the map_key is in appropriate range
        if (map_key >= 0 && map_key <= opts->range_max) {

            // we can just overwrite the current map[key] hopefully..
            if (value_len > opts->max_default_map_entry_len) {
                free(opts->map[map_key]);
                opts->map[map_key] = calloc(value_len + 1, sizeof(char));
            }

            // update the value
            snprintf(opts->map[map_key], value_len + 1, "%s", value);
        }

        ptr = strtok(NULL, "; ,");
        free(free_token);
    }

    free(opts->map_string);
}


int sanity_check(game_options * opts)
{

}


int main (int argc, char **argv)
{
    int result = 0;
    int i = 0;
    int j = 0;

    int row = 0;
    int col = 0;

    int r = 0;

    int value = 0;
    int neighbors = 0;

    int generations = 0;

    game_options * opts = calloc(1, sizeof(game_options));


    srand(time(NULL));


    // calculate final map
    opts->map = calloc(opts->range_max + 1, sizeof(* (opts->map)));


    // thanks c99
    int current_iteration[opts->rows][opts->cols];
    int next_iteration[opts->rows][opts->cols];


    // TODO: accept initial seed input
    // generate initial seed
    for (row = 0; row < opts->rows; row++) {
        for (col = 0; col < opts->cols; col++) {

            r = rand();

            current_iteration[row][col] = r % (opts->range_max - opts->range_min + 1);

            if (FULL_RANDOM == FALSE) {
                if (r % 100 > PERCENTAGE_DEAD) {
                    current_iteration[row][col] = 0;
                }
            }
        }
    }

    while (TRUE) {

        if (opts->max_generations > 0 && opts->max_generations == generations) {
            break;
        }

        // print line seperator between generations
        for (i = 0; i < opts->cols; i++) {
            for (j = 0; j < opts->max_map_entry_len + 1; j++) {
                printf("%s", "-");
            }
        }
        printf("%s", "\n");

        // display current iteration
        for (row = 0; row < opts->rows; row++) {
            for (col = 0; col < opts->cols; col++) {

                char fmt_string[MAX_TEMP_BUFFER] = { 0 };
                snprintf(fmt_string, MAX_TEMP_BUFFER, "%%%ds", opts->max_map_entry_len);

                printf(fmt_string, opts->map[current_iteration[row][col]]);

                // print a space unless we're the last col
                if (col < opts->cols - 1) {
                    printf("%s", " ");
                }
            }
            printf("%s", "\n");
        }

        // get next iteration based on current iteration
        for (row = 0; row < opts->rows; row++) {
            for (col = 0; col < opts->cols; col++) {

                value = current_iteration[row][col];

                // top left corner
                if (row == 0 && col == 0) {
                    neighbors = current_iteration[row]  [RIGHT] +
                                current_iteration[BELOW][RIGHT] +
                                current_iteration[BELOW][col];
                }
                // top right corner
                else if (row == 0 && col == opts->cols - 1) {
                    neighbors = current_iteration[row]  [LEFT] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[BELOW][col];
                }
                // bottom left corner
                else if (row == opts->rows - 1 && col == 0) {
                    neighbors = current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][RIGHT] +
                                current_iteration[row]  [RIGHT];
                }
                // bottom right corner
                else if (row == opts->rows - 1 && col == opts->cols - 1) {
                    neighbors = current_iteration[row]  [LEFT] +
                                current_iteration[ABOVE][LEFT] +
                                current_iteration[ABOVE][col];
                }
                // top edge
                else if (row == 0) {
                    neighbors = current_iteration[row]  [LEFT] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[BELOW][col] +
                                current_iteration[BELOW][RIGHT] +
                                current_iteration[row]  [RIGHT];
                }
                // left edge
                else if (col == 0) {
                    neighbors = current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][RIGHT] +
                                current_iteration[row]  [RIGHT] +
                                current_iteration[BELOW][RIGHT] +
                                current_iteration[BELOW][col];
                }
                // right edge
                else if (col == opts->cols - 1) {
                    neighbors = current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][LEFT] +
                                current_iteration[row]  [LEFT] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[BELOW][col];
                }
                // bottom edge
                else if (row == opts->rows - 1) {
                    neighbors = current_iteration[row]  [LEFT] +
                                current_iteration[ABOVE][LEFT] +
                                current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][RIGHT] +
                                current_iteration[row]  [RIGHT];
                }
                // everything not on an edge
                else {
                    neighbors = current_iteration[ABOVE][LEFT] +
                                current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][RIGHT] +
                                current_iteration[row]  [RIGHT] +
                                current_iteration[BELOW][RIGHT] +
                                current_iteration[BELOW][col] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[row]  [LEFT];
                }

                /*
                * < 200% neighbor value (2 live cells) will kill a live cell
                * >= 200% neighbor value (2 live cells) and <= 300% (3 live cells) will keep a cell alive
                * == 300% (3 live cells) will bring a dead cell to life
                * > 300% (3 live cells) will kill a live cell
                * We can adjust these slightly to account for decay:
                    * < 200% neighbor value will decay a living cell, eventually to death (by decrementing its value by 1)
                */

                // currently dead cell
                if (value == opts->range_min) {

                    // bring it alive, boys
                    /*
                    if (neighbors > range_min && neighbors == range_max * COME_ALIVE) {
                        next_iteration[row][col] = range_max;
                    }
                    */
                    if (neighbors > opts->range_min) {

                        // i don't think 3 decaying cells should be able to create a fully
                        // healthy cell.. open for discussion...
                        for (i = opts->range_min + 1; i <= opts->range_max; i++) {
                            if (neighbors == i * COME_ALIVE) {
                                next_iteration[row][col] = i;
                            }
                        }
                    }

                    continue;
                }

                // stay the same value..
                if (neighbors >= value * STAY_ALIVE_MIN && neighbors <= value * COME_ALIVE) {
                    next_iteration[row][col] = value;
                }

                // everything else is in some state of decay
                if (neighbors < value * STAY_ALIVE_MIN || neighbors > value * COME_ALIVE) {
                    next_iteration[row][col] = value - 1;
                    if (next_iteration[row][col] < opts->range_min) {
                        next_iteration[row][col] = opts->range_min;
                    }
                }
            }
        }

        // set current iteration to next iteration
        for (row = 0; row < opts->rows; row++) {
            for (col = 0; col < opts->cols; col++) {
                current_iteration[row][col] = next_iteration[row][col];
            }
        }

        sleep(1);
        generations++;
    }

    for (i = 0; i <= opts->range_max; i++) {
        free(opts->map[i]);
    }
    free(opts->map);
    free(opts);

    return 0;
}
