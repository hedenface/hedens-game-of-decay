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


void print_version();
void print_help();
void parse_arguments(int argc, char ** argv, int * rows, int * cols, int * max, int * generations, int * pad_left, int * pad_right, char * pad_char, char ** map);
void seed_grid(int rows, int cols, int (* generation)[cols], int max);
void initialize_grid(int rows, int cols, int (* generation)[cols]);
void print_grid(int rows, int cols, int (* generation)[cols], char ** char_map);
int get_neighbors_value(int rows, int cols, int (* this)[cols], int cur_row, int cur_col);
void decay_grid(int rows, int cols, int (* this)[cols], int (* next)[cols], int max);
void next_grid(int rows, int cols, int (* this)[cols], int (* next)[cols]);
void print_line_seperator(int cols);
int get_printed_integer_len(int val);
int get_widest_map_entry(int max, char * orig_map);
char * get_char_map_entry(int max, int lookup, char * orig_map);
char ** setup_char_map(int max, char * map);
void free_char_map(int max, char ** char_map);
int main(int argc, char ** argv);


#endif /* _GAME_OF_DECAY_H_ */
