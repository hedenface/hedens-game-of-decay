#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "game-of-decay.h"


/* just because you're paranoid doesn't mean
   they aren't out to get you. */
START_TEST (booleans_are_sane)
{
    ck_assert_int_eq(TRUE, 1);
    ck_assert_int_eq(FALSE, 0);
}
END_TEST


Suite * decay_suite(void)
{
    Suite * suite = NULL;
    TCase * tc    = NULL;

    suite = suite_create("Heden's Game of Decay");

    tc = tcase_create("100 percent of the time they pass some of the time");

    tcase_add_test(tc, booleans_are_sane);

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
