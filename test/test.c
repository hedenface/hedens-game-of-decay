#include <stdio.h>
#include <stdlib.h>
#include <check.h>


#define TESTING 1
#include "game-of-decay.h"


/* just because you're paranoid doesn't mean
   they aren't out to get you. */
START_TEST (booleans_are_sane)
{
    ck_assert_int_eq(TRUE, 1);
    ck_assert_int_eq(FALSE, 0);
}
END_TEST


/*
START_TEST (arguments_are_parsed)
{
    int rows = 0;
    int cols = 0;
    int max = 0;
    int generations = 0;
    int pad_left = 0;
    int pad_right;
    char * map = NULL;

    int argc = 15;
    char ** argv = calloc(1, sizeof(argv));
    argv[0] = strdup("./test");
    argv[1] = strdup("-r");
    argv[2] = strdup("2");
    argv[3] = strdup("-c");
    argv[4] = strdup("3");
    argv[5] = strdup("-m");
    argv[6] = strdup("4");
    argv[7] = strdup("-g");
    argv[8] = strdup("5");
    argv[9] = strdup("-L");
    argv[10] = strdup("6");
    argv[11] = strdup("-R");
    argv[12] = strdup("7");
    argv[13] = strdup("-M");
    argv[14] = strdup("0=0");

    parse_arguments(argc, argv, &rows, &cols, &max, &generations, &pad_left, &pad_right, &map);

    ck_assert_int_eq(rows, 2);
    ck_assert_int_eq(cols, 3);
    ck_assert_int_eq(max, 4);
    ck_assert_int_eq(generations, 5);
    ck_assert_int_eq(pad_left, 6);
    ck_assert_int_eq(pad_right, 7);
}
END_TEST
*/


START_TEST (grids_initialize)
{
    int rows = 16;
    int cols = 16;
    int grid[rows][cols];

    int i = 0;
    int j = 0;

    initialize_grid(rows, cols, grid);

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            ck_assert_int_eq(grid[i][j], 0);
        }
    }
}
END_TEST


START_TEST (neighbors_values_accurate)
{
    int grid[3][3] = {
        { 1, 2, 3 }, 
        { 4, 5, 6 },
        { 7, 8, 9 }
    };

    int neighbors = get_neighbors_value(3, 3, grid, 1, 1);

    ck_assert_int_eq(neighbors, ((1 + 2 + 3) + (4 + 6) + (7 + 8 + 9)));
}
END_TEST


START_TEST (decay_accurate)
{
    int grid[3][3] = {
        { 0, 0, 0 }, 
        { 0, 9, 0 },
        { 0, 0, 0 }
    };

    int next[3][3] = { 0 };

    ck_assert_int_eq(next[1][1], 0);
    decay_grid(3, 3, grid, next, 9);
    ck_assert_int_eq(next[1][1], 8);

    initialize_grid(3, 3, grid);
    initialize_grid(3, 3, next);
    grid[1][1] = -1;

    decay_grid(3, 3, grid, next, 9);
    ck_assert_int_eq(next[1][1], 0);
}
END_TEST


START_TEST (come_alive_accurate)
{
    int grid[3][3] = {
        { 9, 9, 9 }, 
        { 0, 0, 0 },
        { 0, 0, 0 }
    };

    int next[3][3] = { 0 };

    ck_assert_int_eq(next[1][1], 0);
    decay_grid(3, 3, grid, next, 9);
    ck_assert_int_eq(next[1][1], 9);
}
END_TEST


START_TEST (no_change_accurate)
{
    int perfect_neighbor_val = 9 * STAY_ALIVE_MIN / 3;
    int grid[3][3] = {
        { perfect_neighbor_val, perfect_neighbor_val, perfect_neighbor_val }, 
        { 0, 9, 0 },
        { 0, 0, 0 }
    };

    int next[3][3] = { 0 };

    ck_assert_int_eq(next[1][1], 0);
    decay_grid(3, 3, grid, next, 9);
    ck_assert_int_eq(next[1][1], 9);
}
END_TEST


START_TEST (next_grid_accurate)
{
    int this[3][3] = { 0 };

    int next[3][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 } 
    };

    ck_assert_int_eq(this[1][1], 0);
    next_grid(3, 3, this, next);
    ck_assert_int_eq(this[0][0], 1);
    ck_assert_int_eq(this[1][1], 5);
    ck_assert_int_eq(this[2][2], 9);
}
END_TEST


START_TEST (printed_integer_len_accurate)
{
    ck_assert_int_eq(get_printed_integer_len(1), 1);
    ck_assert_int_eq(get_printed_integer_len(10), 2);
    ck_assert_int_eq(get_printed_integer_len(100), 3);
    ck_assert_int_eq(get_printed_integer_len(1000), 4);
    ck_assert_int_eq(get_printed_integer_len(10000), 5);
    ck_assert_int_eq(get_printed_integer_len(100000), 6);
}
END_TEST


START_TEST (widest_map_entry_accurate)
{
    ck_assert_int_eq(get_widest_map_entry(9,  "9=12,10=123"), 2);
    ck_assert_int_eq(get_widest_map_entry(10, "9=12,10=123"), 3);
    ck_assert_int_eq(get_widest_map_entry(9, "0=1234,1=123,2=12,3="), 4);
    ck_assert_int_eq(get_widest_map_entry(100, NULL), 3);
}
END_TEST


START_TEST (char_map_entry_accurate)
{
    char * map = strdup("9=a,10=b");
    char * str = NULL;

    str = get_char_map_entry(9, 10, map);
    ck_assert(str == NULL);
    free(str);

    str = get_char_map_entry(10, 10, map);
    ck_assert_str_eq(str, "b");
    free(str);

    str = get_char_map_entry(9, 0, NULL);
    ck_assert_str_eq(str, " ");
    free(str);

    free(map);
}
END_TEST


START_TEST (setup_char_map_accurate)
{
    char * map = strdup("0=a,1=b,2=c,3=d,4=e");
    char ** char_map = NULL;

    char_map = setup_char_map(6, map);

    ck_assert_str_eq(char_map[0], "a");
    ck_assert_str_eq(char_map[1], "b");
    ck_assert_str_eq(char_map[2], "c");
    ck_assert_str_eq(char_map[3], "d");
    ck_assert_str_eq(char_map[4], "e");
    ck_assert_str_eq(char_map[5], "5");
    ck_assert_str_eq(char_map[6], "6");

    free_char_map(6, char_map);
    free(map);

    free_char_map(0, NULL);
}
END_TEST


START_TEST (setup_cell_fmt_accurate)
{
    char * map = strdup("0=aaa");
    char * fmt = setup_cell_fmt(6, 1, 1, map);

    ck_assert_str_eq(fmt, "%9s ");

    free(fmt);
    free(map);
}
END_TEST


Suite * decay_suite(void)
{
    Suite * suite = NULL;
    TCase * tc    = NULL;

    suite = suite_create("Heden's Game of Decay");

    tc = tcase_create("");

    tcase_add_test(tc, booleans_are_sane);
    tcase_add_test(tc, grids_initialize);
    tcase_add_test(tc, neighbors_values_accurate);
    tcase_add_test(tc, decay_accurate);
    tcase_add_test(tc, come_alive_accurate);
    tcase_add_test(tc, no_change_accurate);
    tcase_add_test(tc, next_grid_accurate);
    tcase_add_test(tc, printed_integer_len_accurate);
    tcase_add_test(tc, widest_map_entry_accurate);
    tcase_add_test(tc, char_map_entry_accurate);
    tcase_add_test(tc, setup_char_map_accurate);
    tcase_add_test(tc, setup_cell_fmt_accurate);

    suite_add_tcase(suite, tc);

    return suite;
}


/* the rest of these tests are simply for coverage sake,
   and so we can check this against valgrind afterwards to see if
   we have any memory leaks */


START_TEST (print_help_and_version)
{
    print_version();
    print_help();
}
END_TEST


START_TEST (seed_and_print)
{
    char * map = strdup("0=0");
    char ** char_map = setup_char_map(9, map);
    char * fmt = strdup("%s");
    int grid[3][3] = { 0 };
    seed_grid(3, 3, grid, 9);
    print_grid(3, 3, grid, char_map, fmt);

    free_char_map(9, char_map);
    free(map);
    free(fmt);
}
END_TEST


START_TEST (get_terminal_info)
{
    int rows = 0;
    int cols = 0;
    get_term_size(&rows, &cols);
}
END_TEST


Suite * coverage_suite(void)
{
    Suite * suite = NULL;
    TCase * tc    = NULL;

    suite = suite_create("Coverage / Memcheck");

    tc = tcase_create("");

    tcase_add_test(tc, print_help_and_version);
    tcase_add_test(tc, seed_and_print);
    tcase_add_test(tc, get_terminal_info);

    suite_add_tcase(suite, tc);

    return suite;
}


int main(void)
{
    int number_failed   = 0;
    int i               = 0;
    Suite * decay       = decay_suite();
    Suite * coverage    = coverage_suite();
    SRunner * runner[2] = { NULL };

    runner[0] = srunner_create(decay);
    runner[1] = srunner_create(coverage);

    for (i = 0; i < 2; i++) {

        /* disable forking for runner */
        srunner_set_fork_status(runner[i], CK_NOFORK);

        /* available print_modes:
            CK_SILENT
            CK_MINIMAL
            CK_NORMAL
            CK_VERBOSE
            CK_ENV (get print mode from ENV CK_VERBOSITY (silent, minimal, etc.))
            CK_SUBUNIT
        */

        srunner_run_all(runner[i], CK_VERBOSE);

        number_failed += srunner_ntests_failed(runner[i]);

        srunner_free(runner[i]);
    }

    printf("%s\n", "************************");
    printf("Total failures: %d\n", number_failed);
    printf("%s\n", "************************");

    if (number_failed == 0) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
