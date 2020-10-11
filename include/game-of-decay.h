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


#ifndef TESTING
#define TESTING 0
#endif


void decay_exit(int code);
void print_version();
void print_help();
void parse_arguments(int argc, char ** argv, int * rows, int * cols, int * max, int * generations, int * pad_left, int * pad_right, char ** map);
void seed_grid(int rows, int cols, int (* generation)[cols], int max);
void initialize_grid(int rows, int cols, int (* generation)[cols]);
void print_grid(int rows, int cols, int (* generation)[cols], char ** char_map, char * cell_fmt);
int get_neighbors_value(int rows, int cols, int (* this)[cols], int cur_row, int cur_col);
void decay_grid(int rows, int cols, int (* this)[cols], int (* next)[cols], int max);
void next_grid(int rows, int cols, int (* this)[cols], int (* next)[cols]);
int get_printed_integer_len(int val);
int get_widest_map_entry(int max, char * orig_map);
char * get_char_map_entry(int max, int lookup, char * orig_map);
char ** setup_char_map(int max, char * map);
void free_char_map(int max, char ** char_map);
char * setup_cell_fmt(int pad_left, int pad_right, int max, char * map);
void get_term_size(int * rows, int * cols);


#endif /* _GAME_OF_DECAY_H_ */
