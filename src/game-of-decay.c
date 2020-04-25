#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>


#define DEFAULT_ROWS 20
#define DEFAULT_COLS 40

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


#define ABOVE (row - 1)
#define BELOW (row + 1)
#define LEFT  (col - 1)
#define RIGHT (col + 1)


int rows        = DEFAULT_ROWS;
int cols        = DEFAULT_COLS;
int range_min   = 0;
int range_max   = 1;


#if defined(TRUE) && TRUE != 1
    #undef TRUE
#endif
#if defined(FALSE) && FALSE != 0
    #undef FALSE
#endif

#ifndef TRUE
    #define TRUE  1
#endif
#ifndef FALSE
    #define FALSE 0
#endif


int print_help()
{
    printf("help\n");
}


int print_version()
{
    printf("version\n");
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
        if ((* min) >= 0) {
            return TRUE;
        }
    }

    return FALSE;
}


int main (int argc, char **argv)
{
    char c = 0;
    int option_index = 0;
    int result = 0;
    int i = 0;

    int row = 0;
    int col = 0;

    int r = 0;

    int value = 0;
    int neighbors = 0;

    static struct option long_options[] = {
        { "help",    no_argument,       0, 'h' },
        { "version", no_argument,       0, 'v' },

        { "rows",    optional_argument, 0, 'r' },
        { "cols",    optional_argument, 0, 'c' },

        { "range",   optional_argument, 0, 'R' },
    };

    srand(time(NULL));

    while (TRUE) {

        c = getopt_long(argc, argv, "hvr:c:R:", long_options, &option_index);

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
            rows = atoi(optarg);

            if (rows <= 0) {
                return print_help();
            }

            break;

        case 'c':
            cols = atoi(optarg);

            if (cols <= 0) {
                return print_help();
            }

            break;

        case 'R':
            result = parse_range(optarg, &range_min, &range_max);

            if (result == FALSE) {
                return print_help();
            }

            break;
        }
    }

    // thanks c99
    int current_iteration[rows][cols];
    int next_iteration[rows][cols];

    // listen, i chose deliberately to not use function calls here
    // for no reason other than dealing with returning multi-
    // dimensional arrays
    // if you have a way that is both easy to understand and implement
    // (and also understand) then by all means please submit a pr

    // TODO: accept initial seed input
    // generate initial seed
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {

            r = rand();

            current_iteration[row][col] = r % (range_max - range_min + 1);

            if (FULL_RANDOM == FALSE) {
                if (r % 100 > PERCENTAGE_DEAD) {
                    current_iteration[row][col] = 0;
                }
            }
        }
    }

    while (TRUE) {

        // print line seperator between generations
        for (i = 0; i < (cols * 2 - 1); i++) {
            printf("%s", "-");
        }
        printf("%s", "\n");

        // display current iteration
        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {

                if (current_iteration[row][col] == range_min) {
                    printf("%s", " ");
                }
                else {
                    printf("%d", current_iteration[row][col]);
                }

                // print a space unless we're the last col
                if (col < cols - 1) {
                    printf("%s", " ");
                }
            }
            printf("%s", "\n");
        }

        // get next iteration based on current iteration
        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {

                value = current_iteration[row][col];

                // top left corner
                if (row == 0 && col == 0) {
                    neighbors = current_iteration[row]  [RIGHT] +
                                current_iteration[BELOW][RIGHT] +
                                current_iteration[BELOW][col];
                }
                // top right corner
                else if (row == 0 && col == cols - 1) {
                    neighbors = current_iteration[row]  [LEFT] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[BELOW][col];
                }
                // bottom left corner
                else if (row == rows - 1 && col == 0) {
                    neighbors = current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][RIGHT] +
                                current_iteration[row]  [RIGHT];
                }
                // bottom right corner
                else if (row == rows - 1 && col == cols - 1) {
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
                else if (col == cols - 1) {
                    neighbors = current_iteration[ABOVE][col] +
                                current_iteration[ABOVE][LEFT] +
                                current_iteration[row]  [LEFT] +
                                current_iteration[BELOW][LEFT] +
                                current_iteration[BELOW][col];
                }
                // bottom edge
                else if (row == rows - 1) {
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
                if (value == range_min) {

                    // bring it alive, boys
                    /*
                    if (neighbors > range_min && neighbors == range_max * COME_ALIVE) {
                        next_iteration[row][col] = range_max;
                    }
                    */
                    if (neighbors > range_min) {

                        // i don't think 3 decaying cells should be able to create a fully
                        // healthy cell.. open for discussion...
                        for (i = range_min + 1; i <= range_max; i++) {
                            if (neighbors == i * COME_ALIVE) {
                                next_iteration[row][col] = i;
                            }
                        }
                    }

                    continue;
                }

                // everything else is in some state of decay
                if (neighbors < value * STAY_ALIVE_MIN) {
                    next_iteration[row][col] = value - 1;
                    if (next_iteration[row][col] < range_min) {
                        next_iteration[row][col] = range_min;
                    }
                }
                if (neighbors >= value * STAY_ALIVE_MIN && neighbors <= value * COME_ALIVE) {
                    next_iteration[row][col] = value;
                }
                if (neighbors > value * COME_ALIVE) {
                    next_iteration[row][col] = range_min;
                }
            }
        }

        // set current iteration to next iteration
        for (row = 0; row < rows; row++) {
            for (col = 0; col < cols; col++) {
                current_iteration[row][col] = next_iteration[row][col];
            }
        }

        sleep(1);
    }

    return 0;
}
