#include "test_linear_hash_dictionary.h"

void
run_linearHashDictionary_generic_test_set_1(
	planck_unit_test_t *tc
) {
	generic_test_t test;

	init_generic_dictionary_test(&test, lhdict_init, key_type_numeric_signed, sizeof(int), sizeof(int), 256	/*initial size for lh */
	);

	dictionary_test_init(&test, tc);

	dictionary_test_insert_get(&test, 10000, tc);

	dictionary_test_insert_get_edge_cases(&test, tc);

	/*hat are return codes */
	/*int to_delete[] = {7, -9, 32, 1000001};
	int i;

	for (i = 0; i < sizeof(to_delete)/sizeof(int); i++)
	{
		dictionary_test_delete(
			&test,
			(ion_key_t)(&(to_delete[i])),
			tc
		);
	}*/

	int update_key;
	int update_value;

	update_key		= 1;
	update_value	= -12;

	dictionary_test_update(&test, (ion_key_t) (&update_key), (ion_value_t) (&update_value), tc);

	update_key		= 1;
	update_value	= 12;

	dictionary_test_update(&test, (ion_key_t) (&update_key), (ion_value_t) (&update_value), tc);

	update_key		= 12;
	update_value	= 1;

	dictionary_test_update(&test, (ion_key_t) (&update_key), (ion_value_t) (&update_value), tc);

	dictionary_insert(&test.dictionary, IONIZE(5, int), IONIZE(3, int));
	dictionary_insert(&test.dictionary, IONIZE(5, int), IONIZE(5, int));
	dictionary_insert(&test.dictionary, IONIZE(5, int), IONIZE(7, int));

	dictionary_insert(&test.dictionary, IONIZE(-5, int), IONIZE(14, int));
	dictionary_insert(&test.dictionary, IONIZE(-7, int), IONIZE(6, int));
	dictionary_insert(&test.dictionary, IONIZE(-10, int), IONIZE(23, int));
	dictionary_insert(&test.dictionary, IONIZE(-205, int), IONIZE(9, int));

	dictionary_test_equality(&test, IONIZE(5, int), tc);

	dictionary_test_equality(&test, IONIZE(-10, int), tc);

/*	dictionary_test_range(
		&test,
		IONIZE(5, int),
		IONIZE(3777, int),
		tc
	);

	dictionary_test_range(
		&test,
		IONIZE(-5, int),
		IONIZE(3777, int),
		tc
	);*/

/*
	dictionary_test_all_records(
		&test,
		10006,
		tc
	);
*/

/*		dictionary_test_open_close(&test, tc); */

	cleanup_generic_dictionary_test(&test);
}

planck_unit_suite_t *
linearHashDictionary_get_suite(
) {
	planck_unit_suite_t *suite = planck_unit_new_suite();

	planck_unit_add_to_suite(suite, run_linearHashDictionary_generic_test_set_1);

	return suite;
}

void
run_all_tests_linearHashDictionary(
) {
	/* CuString	*output	= CuStringNew(); */
	planck_unit_suite_t *suite = linearHashDictionary_get_suite();

	planck_unit_run_suite(suite);
	/* CuSuiteSummary(suite, output); */
	/* CuSuiteDetails(suite, output); */
	/* printf("%s\n", output->buffer); */

	planck_unit_destroy_suite(suite);
	/* CuSuiteDelete(suite); */
	/* CuStringDelete(output); */
}
