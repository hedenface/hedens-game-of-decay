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
void parse_arguments(int argc, char ** argv, int * rows, int * cols, int * max, int * generations);
void seed_grid(int rows, int cols, int (* generation)[cols], int max);
void initialize_grid(int rows, int cols, int (* generation)[cols]);
void print_grid(int rows, int cols, int (* generation)[cols]);
int get_neighbors_value(int rows, int cols, int (* this)[cols], int cur_row, int cur_col);
void decay_grid(int rows, int cols, int (* this)[cols], int (* next)[cols], int max);
void next_grid(int rows, int cols, int (* this)[cols], int (* next)[cols]);
void print_line_seperator(int cols);
int main(int argc, char ** argv);


#endif /* _GAME_OF_DECAY_H_ */
