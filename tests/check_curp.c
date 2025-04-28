#include <stdlib.h>
#include "curp.h"
#include "check_curp_examples.h"
#include "check_curp_pbt.h"

int main(void)
{
	int failed;
	Suite *ex_suite, *pbt_suite;
	SRunner *runner;

	/* setup */
	if (!curp_init()) return EXIT_FAILURE;

	ex_suite = curp_suite();
	pbt_suite = theft_suite();

	runner = srunner_create(ex_suite);
	srunner_add_suite(runner, pbt_suite);

	srunner_run_all(runner, CK_NORMAL);
	failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	/* teardown */
	curp_exit();

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
