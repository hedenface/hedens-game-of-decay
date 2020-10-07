#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>

#include "game-of-decay.h"


/* if full random is false, then we pick about PERCENTAGE_DEAD % to
   kill from the beginning, otherwise it gets weird :) */
#define FULL_RANDOM     FALSE
#define PERCENTAGE_DEAD 50


/*
    * < 200% neighbor neighbors (2 live cells, STAY_ALIVE_MIN) will kill a live cell
    * >= 200% neighbor neighbors (2 live cells, STAY_ALIVE_MIN) and <= 300% (3 live cells, COME_ALIVE) will keep a cell alive
    * == 300% (3 live cells, COME_ALIVE) will bring a dead cell to life
    * > 300% (3 live cells, COME_ALIVE) will kill a live cell
*/
#define STAY_ALIVE_MIN 2 /* 200% */
#define COME_ALIVE     3 /* 300% */


#define TRUE  1
#define FALSE 0


void print_version()
{
    printf("%s\n", "Hedens Game of Decay - version 0.0.0\n");
}


void print_help()
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
    printf("%s\n", "                  minimum: 2");
    printf("%s\n", "-c,--cols         Specify how many columns of cells to iterate over");
    printf("%s\n", "                  minimum: 2");
    printf("%s\n", "-m,--max-value    Maximum value of any given cell");
    printf("%s\n", "                  default: 1 (this reproduces Conways Game of Life)");
    printf("%s\n", "                  minimum: 1");
    /*
    printf("%s\n", "-R,--range        Specify a range of values [X-Y]");
    printf("%s\n", "                  default: 0-1 (this reproduces Conways Game of Life)");
    printf("%s\n", "-m,--map          Specify a map for values");
    printf("%s\n", "                  Each value can be space, comma, or semi-colon delimited");
    printf("%s\n", "                  default: \"0= ,1=1,2=2,3=3,...Y=Y\"");
    */
    printf("%s\n", "-g,--generations  Specify maximum number of generations to print");
    printf("%s\n", "                  minimum: 1");
    printf("%s", "\n");
    printf("%s", "\n");
}


void parse_arguments(int argc, char ** argv, int * rows, int * cols, int * max, int * generations)
{
    char c           = 0;
    int option_index = 0;
    int errors       = 0;

    static struct option long_options[] = {
        { "help",        no_argument,       0, 'h' },
        { "version",     no_argument,       0, 'v' },
        { "rows",        required_argument, 0, 'r' },
        { "cols",        required_argument, 0, 'c' },
        { "max-value",   required_argument, 0, 'm' },
        /*
        { "range",       optional_argument, 0, 'R' },
        { "map",         optional_argument, 0, 'm' },
        */
        { "generations", required_argument, 0, 'g' },
    };

    while (TRUE) {

        c = getopt_long(argc, argv, "hvr:c:m:g:", long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {

        case 'h':
            print_help();
            exit(1);
            break;

        case 'v':
            print_version();
            exit(1);
            break;

        case 'r':
            * rows = atoi(optarg);
            break;

        case 'c':
            * cols = atoi(optarg);
            break;

        case 'm':
            * max = atoi(optarg);
            break;

        case 'g':
            * generations = atoi(optarg);
            break;
        }
    }

    if (* rows < 2) {
        printf("%s\n", "Invalid number of rows!");
        errors++;
    }

    if (* cols < 2) {
        printf("%s\n", "Invalid number of cols!");
        errors++;
    }

    if (* max < 1) {
        printf("%s\n", "Invalid maximum cell value!");
        errors++;
    }

    if (* generations < 0) {
        printf("%s\n", "Invalid number of generations (we've not implemented time travel)!");
        errors++;
    }

    if (errors > 0) {
        exit(2);
    }
}

void seed_grid(int rows, int cols, int (* generation)[cols], int max)
{
    int i = 0;
    int j = 0;
    int r = 0;

    srand(time(NULL));

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {

            r = rand();

            generation[i][j] = r % (max + 1);

            if (FULL_RANDOM == FALSE) {
                if (r % 100 > PERCENTAGE_DEAD) {
                    generation[i][j] = 0;
                }
            }
        }
    }
}


void initialize_grid(int rows, int cols, int (* generation)[cols])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            generation[i][j] = 0;
        }
    }
}


void print_grid(int rows, int cols, int (* generation)[cols])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (generation[i][j] == 0) {
                printf("%s", " ");
            }
            else {
                printf("%d", generation[i][j]);
            }
        }
        printf("%s\n", "");
    }
}


int get_neighbors_value(int rows, int cols, int (* this)[cols], int cur_row, int cur_col)
{
    int i = 0;
    int j = 0;
    int neighbors = 0;

    int row = 0;
    int col = 0;

    for (i = -1; i <= 1; i++) {

        row = cur_row + i;
        if (row < 0 || row > (rows - 1)) {
            continue;
        }

        for (j = -1; j <= 1; j++) {

            col = cur_col + j;
            if (col < 0 || col > (cols - 1)) {
                continue;
            }

            neighbors += this[row][col];
        }
    }

    neighbors -= this[cur_row][cur_col];

    return neighbors;
}


void decay_grid(int rows, int cols, int (* this)[cols], int (* next)[cols], int max)
{
    int i = 0;
    int j = 0;
    int cell      = 0;
    int neighbors = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            neighbors = get_neighbors_value(rows, cols, this, i, j);
            cell = this[i][j];

            /* already dead cell */
            if (cell == 0) {

                if (neighbors > 0) {
                    int k = 0;

                    /* i'm not sure what sort of pet semetary we're running
                       that allows 3(COME_ALIVE) decaying cells to bring a
                       dead one back to life, but hey... */
                    for (k = 1; k <= max; k++) {
                        if (neighbors == (i * COME_ALIVE)) {
                            next[i][j] = k;
                        }
                    }
                }
            }

            /* if we're in that sweet spot of no decay */
            else if (neighbors >= (cell * STAY_ALIVE_MIN) && neighbors <= (cell * COME_ALIVE)) {
                next[i][j] = cell;
            }

            /* everything else is in some sort of decay */
            else {
                next[i][j] = cell - 1;
                if (next[i][j] < 0) {
                    next[i][j] = 0;
                }
            }
        }
    }
}


void next_grid(int rows, int cols, int (* this)[cols], int (* next)[cols])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            this[i][j] = next[i][j];
            next[i][j] = 0;
        }
    }
}


void print_line_seperator(int cols)
{
    int i = 0;

    for (i = 0; i < cols; i++) {
        printf("%s", "-");
    }

    printf("%s", "\n");
}


int main(int argc, char ** argv)
{
    int rows        = 0;
    int cols        = 0;
    int max         = 1;
    int generations = 0;
    int generation  = 0;

    parse_arguments(argc, argv, &rows, &cols, &max, &generations);

    int this_generation[rows][cols];
    int next_generation[rows][cols];

    initialize_grid(rows, cols, this_generation);
    initialize_grid(rows, cols, next_generation);

    seed_grid(rows, cols, this_generation, max);

    while (TRUE) {
        if (generations > 0 && generation >= generations) {
            break;
        }

        print_grid(rows, cols, this_generation);
        decay_grid(rows, cols, this_generation, next_generation, max);
        next_grid(rows, cols, this_generation, next_generation);

        /* if generations == 1, then we're only going to print the
           one generation and then exit. we don't need a line sep */
        if (generations != 1) {
            print_line_seperator(cols);
        }

        generation++;
        sleep(1);
    }

    return 0;
}
