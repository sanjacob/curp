/**
 * Copyright (C) 2025, Jacob Sánchez Pérez
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>
 */

#include <ctype.h>
#include <stdlib.h>
#include <check.h>
#include <theft.h>
#include "curp.h"
#include "strategies.h"

static void
theft_generic_print_cb(FILE *f, const void *instance, void *env) {
	fprintf(f, "%s", instance);
}

static int factor[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* digitos */
	0, 0, 0, 0, 0, 0, 0, /* simbolos */
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 5, 6, 7, 8, 9,
	0, 1, 2, 3, 4, 5, 6 /* letras */
};

static int calcular_verificador(const char* curp) {
	int i, sum;

	for (i = sum = 0; curp[i] != '\0'; ++i)
		sum += (CURP_LENGTH - i) * factor[curp[i] - '0'];

	sum = sum % 10;
	return sum == 0 ? 0 : 10 - sum;
}

static enum theft_alloc_res
random_curp_alloc_cb(struct theft *t, void *env, void **instance) {
	/* Dynamically allocate string */
	//const char *curp = "SABC 560626 MDF LRN 01";

	/* 
	 * LVLL-NN-[NN-NN]-H/M/X-[OC]-CCC-0-D*/

	int i;
	char *curp;
	uint64_t mes, dia, sexo;
	const char *entidad;

	curp = malloc(CURP_LENGTH + 1);
	curp[CURP_LENGTH] = '\0';

	/* iniciales */
	curp[0] = UPPER_CHARACTER(t);
	curp[1] = 'X';
	curp[2] = UPPER_CHARACTER(t);
	curp[3] = UPPER_CHARACTER(t);

	/* fecha de nacimiento */

	/* anio: 00-99 */
	curp[4] = DIGIT_CHARACTER(t);
	curp[5] = DIGIT_CHARACTER(t);

	/* mes: 01-12 */
	mes = NUMBER(t, 1, 12);
	curp[6] = (char)(mes / 10) + '0';
	curp[7] = (char)(mes % 10) + '0';

	/* dia: 01-29 */
	dia = NUMBER(t, 1, 28);
	curp[8] = (char)(mes / 10) + '0';
	curp[9] = (char)(mes % 10) + '0';

	/* sexo: HMX */
	sexo = theft_random_choice(t, 3);
	curp[10] = sexo == 1 ? 'H' : sexo == 2 ? 'M' : 'X';

	/* entidad de nacimiento */
	entidad = entidades[NUMBER(t, 1, 32)];
	curp[11] = entidad[0]; 
	curp[12] = entidad[1];

	/* consonantes */
	curp[13] = CONSONANT(t);
	curp[14] = CONSONANT(t);
	curp[15] = CONSONANT(t);

	/* homonimia */
	curp[16] =  BOOLEAN(t) ? DIGIT_CHARACTER(t) : UPPER_CHARACTER(t);

	/* digito verificador */
	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	*instance = curp;
	return THEFT_ALLOC_OK;
}

static struct theft_type_info random_curp_info = {
	.alloc = random_curp_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_trial_res
curp_debe_ser_valida(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);
	return curp_error(r) == CURP_VALIDA ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_ser_valida)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_ser_valida,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

	/* Run the property test */
	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

Suite * theft_suite(void)
{
	Suite *suite;
	TCase *tc_core;

	suite = suite_create("theft");

	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_curp_debe_ser_valida);
	suite_add_tcase(suite, tc_core);

	return suite;
}
