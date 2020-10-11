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


#define a(t) tcase_add_test(tc, t)

Suite * decay_suite(void)
{
    Suite * suite = NULL;
    TCase * tc    = NULL;

    suite = suite_create("Heden's Game of Decay");

    tc = tcase_create("");

    a(booleans_are_sane);
    a(grids_initialize);
    a(neighbors_values_accurate);

    suite_add_tcase(suite, tc);

    return suite;
}


int main(void)
{
    int number_failed = 0;
    Suite * decay     = decay_suite();
    SRunner * runner  = srunner_create(decay);

    /* disable forking for runner */
    srunner_set_fork_status(runner, CK_NOFORK);


    /* available print_modes:
        CK_SILENT
        CK_MINIMAL
        CK_NORMAL
        CK_VERBOSE
        CK_ENV (get print mode from ENV CK_VERBOSITY (silent, minimal, etc.))
        CK_SUBUNIT
    */

    srunner_run_all(runner, CK_VERBOSE);

    number_failed += srunner_ntests_failed(runner);

    srunner_free(runner);

    printf("%s\n", "************************");
    printf("Total failures: %d\n", number_failed);
    printf("%s\n", "************************");

    if (number_failed == 0) {
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
