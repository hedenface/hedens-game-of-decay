#ifndef _GAME_OF_DECAY_H_
#define _GAME_OF_DECAY_H_


#if defined(TRUE) && TRUE != 1
    #undef TRUE
#endif
#ifndef TRUE
    #define TRUE  1
#endif

#if defined(FALSE) && FALSE != 0
    #undef FALSE
#endif
#ifndef FALSE
    #define FALSE 0
#endif


/* arbitrary upper limit on range */
#define MAX_RANGE_MAX 9

/* arbitrary limit on what we'll use as a temporary buffer
   for copying map_string values */
#define MAX_TEMP_BUFFER 32

/* arbitrary defaults for rows and columns of a game board */
#define DEFAULT_ROWS 20
#define DEFAULT_COLS 40

/* used for easier understanding of the cell math during
   iterations on the board */
#define ABOVE (row - 1)
#define BELOW (row + 1)
#define LEFT  (col - 1)
#define RIGHT (col + 1)


/* these are the options that a given game of decay requires */
typedef struct game_options {

    int rows;
    int cols;

    char * range_string;
    int range_min;
    int range_max;

    int max_default_map_entry_len;
    int max_map_entry_len;
    char ** map;
    char * map_string;

    int max_generations;

} game_options;




int parse_range(char * range, int * min, int * max);
int parse_arguments(int argc, char **argv, game_options * opts);
int set_default_map(game_options * opts);
int parse_map_string(game_options * opts);
int sanity_check(game_options * opts);


#endif /* _GAME_OF_DECAY_H_ */
