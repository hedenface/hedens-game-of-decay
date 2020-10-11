#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <unistd.h>

#include "game-of-decay.h"


void decay_exit(int code)
{
#if TESTING == 0

    printf("exiting...\n");
    exit(code);

#else
    printf("not exiting...\n");
#endif
}


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
    printf("%s\n", "-r,--rows         The amount of rows in the board");
    printf("%s\n", "                  minimum: 2");
    printf("%s\n", "-c,--cols         The amount of columns in the board");
    printf("%s\n", "                  minimum: 2");
    printf("%s\n", "-m,--max-value    Maximum value of any given cell");
    printf("%s\n", "                  default: 1 (this reproduces Conways Game of Life)");
    printf("%s\n", "                  minimum: 1");
    printf("%s\n", "-g,--generations  The maximum number of generations to print");
    printf("%s\n", "                  minimum: 1");
    printf("%s\n", "-L,--left-pad     The amount of padding to the left of each cells value");
    printf("%s\n", "-R,--right-pad    The amount of padding to the right of each cells value");
    printf("%s\n", "-M,--char-map     The character map for values");
    printf("%s\n", "                  (Key/values are separated by commas)");
    printf("%s\n", "                  default: \"0= ,1=1,2=2,3=3,...,X=X\"");
    printf("%s\n", "                  hint: Given a max-value of 10 (range of 0-10), you");
    printf("%s\n", "                        don't need to specify the entire range, simply");
    printf("%s\n", "                        \"10=A\" would allow for 0 to be \" \", all of");
    printf("%s\n", "                        the other integers to be their values, and then");
    printf("%s\n", "                        substitute 10 with \"A\"");
    printf("%s", "\n");
    printf("%s", "\n");
}


void parse_arguments(int argc, char ** argv, 
                     int * rows, int * cols, int * max, int * generations,
                     int * pad_left, int * pad_right, char ** map)
{
    char c           = 0;
    int option_index = 0;
    int errors       = 0;

    static struct option long_options[] = {
        { "help",        no_argument,       0, 'h' },
        { "version",     no_argument,       0, 'v' },
        { "rows",        required_argument, 0, 'r' },
        { "cols",        required_argument, 0, 'c' },
        { "generations", required_argument, 0, 'g' },
        { "left-pad",    required_argument, 0, 'L' },
        { "right-pad",   required_argument, 0, 'R' },
        { "pad-char",    required_argument, 0, 'P' },
        { "char-map",    required_argument, 0, 'M' },
    };

    while (TRUE) {

        c = getopt_long(argc, argv, "hvr:c:m:g:L:R:P:M:", long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {

        case 'h':
            print_help();
            decay_exit(1);
            break;

        case 'v':
            print_version();
            decay_exit(1);
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

        case 'L':
            * pad_left = atoi(optarg);
            break;

        case 'R':
            * pad_right = atoi(optarg);
            break;

        case 'M':
            * map = strdup(optarg);
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

    if (* pad_left < 0) {
        printf("%s\n", "Invalid left padding!");
        errors++;
    }

    if (* pad_right < 0) {
        printf("%s\n", "Invalid right padding!");
        errors++;
    }

    if (errors > 0) {
        decay_exit(2);
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


void print_grid(int rows, int cols, int (* generation)[cols], char ** char_map, char * cell_fmt)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf(cell_fmt, char_map[generation[i][j]]);
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
                        if (neighbors == (k * COME_ALIVE)) {
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


int get_printed_integer_len(int val)
{
    /* max amount of possible chars is 10 (2,147,483,648) */
    char printed_number[12] = { 0 };
    snprintf(printed_number, 12, "%d", val);
    return (int) strlen(printed_number);
}


int get_widest_map_entry(int max, char * orig_map)
{
    /* if no map specified, just return the length of the largest
       possible value */
    if (orig_map == NULL) {
        return get_printed_integer_len(max);
    }

    char * map      = strdup(orig_map);
    char * free_map = map;
    char * token    = NULL;
    char * key      = NULL;
    char * val      = NULL;

    size_t longest  = 0;

    while ((token = strtok_r(map, ",", &map))) {

        key = strtok_r(token, "=", &token);
        if (key == NULL || atoi(key) < 0 || atoi(key) > max) {
            continue;
        }

        val = strtok_r(token, "=", &token);
        if (val == NULL) {
            continue;
        }

        if (strlen(val) > longest) {
            longest = strlen(val);
        }
    }

    free(free_map);
    return (int) longest;
}


char * get_char_map_entry(int max, int lookup, char * orig_map)
{
    char * str  = NULL;
    int lookup_len = get_printed_integer_len(lookup) + 1;

    /* this shouldn't ever happen... */
    if (lookup < 0 || lookup > max) {
        return NULL;
    }

    if (orig_map != NULL) {
        char * map      = strdup(orig_map);
        char * free_map = map;
        char * token    = NULL;
        char * key      = NULL;
        char * val      = NULL;

        while ((token = strtok_r(map, ",", &map))) {

            key = strtok_r(token, "=", &token);
            if (key == NULL || atoi(key) != lookup) {
                continue;
            }

            val = strtok_r(token, "=", &token);
            if (val == NULL) {
                /* the fact we made it this far means that
                   key == lookup, so need to continue the search */
                break;
            }

            str = strdup(val);
            break;
        }

        free(free_map);
    }

    /* if orig_map is NULL or we never found a match,
       just return lookup as a string */
    if (str == NULL) {

        /* if we didn't find a match, the only value we override
           by default is 0 - we use a space for that */
        if (lookup == 0) {
            return strdup(" ");
        }

        str = calloc(lookup_len, sizeof(char));
        snprintf(str, lookup_len, "%d", lookup);
    }

    return str;
}


char ** setup_char_map(int max, char * map)
{
    int i = 0;

    char ** char_map = calloc(max + 1, sizeof(* char_map));

    for (i = 0; i <= max; i++) {
        char_map[i] = get_char_map_entry(max, i, map);
    }

    return char_map;
}


void free_char_map(int max, char ** char_map)
{
    int i = 0;

    if (char_map == NULL) {
        return;
    }

    for (i = 0; i <= max; i++) {
        if (char_map[i] != NULL) {
            free(char_map[i]);
        }
    }

    free(char_map);
}


char * setup_cell_fmt(int pad_left, int pad_right, int max, char * map)
{
    char * cell_fmt = NULL;
    char * right_padding = strdup("");

    /* start with 1 for the null term */
    int fmt_size = 1;

    pad_left += get_widest_map_entry(max, map);

    fmt_size += pad_left + strlen("%s");

    if (pad_right > 0) {
        int i = 0;

        free(right_padding);
        right_padding = calloc(pad_right + 1, sizeof(char));

        for (i = 0; i < pad_right; i++) {
            right_padding[i] = ' ';
        }

        fmt_size += pad_right;
    }

    cell_fmt = calloc(fmt_size, sizeof(char));

    snprintf(cell_fmt, fmt_size, "%%%ds%s", pad_left, right_padding);

    free(right_padding);
    return cell_fmt;
}
