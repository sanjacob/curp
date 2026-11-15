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
#include <string.h>
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

static int isvowel(char c) {
	return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

static int isstate(char a, char b) {
	int i;

	for (i = 0; entidades[i] != NULL; ++i)
		if (entidades[i][0] == a && entidades[i][1] == b) return 1;
	return 0;
}

static int isstateprefix(char a) {
	int i;

	for (i = 0; entidades[i] != NULL; ++i)
		if (entidades[i][0] == a) return 1;
	return 0;
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
	curp[8] = (char)(dia / 10) + '0';
	curp[9] = (char)(dia % 10) + '0';

	/* sexo: HMX */
	sexo = theft_random_choice(t, 3);
	curp[10] = sexo == 1 ? 'H' : sexo == 2 ? 'M' : 'X';

	/* entidad de nacimiento */
	entidad = entidades[NUMBER(t, 0, 32)];
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

static enum theft_alloc_res
curp_error_vocal_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	curp[1] = SAMPLED_FROM(t, consonants_no_x);

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_vocal_info = {
	.alloc = curp_error_vocal_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_altisonante_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;
	const char *palabra;
	size_t n;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	n = sizeof(altisonantes) / sizeof(altisonantes[0]) - 1;
	palabra = altisonantes[NUMBER(t, 0, n - 1)];
	memcpy(curp, palabra, 4);

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_altisonante_info = {
	.alloc = curp_error_altisonante_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static const uint64_t dia_maximo[] = {
	0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static enum theft_alloc_res
curp_error_dia_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;
	uint64_t mes, dia;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	mes = (curp[6] - '0') * 10 + (curp[7] - '0');
	dia = NUMBER(t, dia_maximo[mes], 99);
	if (dia == dia_maximo[mes]) dia = 0;
	curp[8] = (char)(dia / 10) + '0';
	curp[9] = (char)(dia % 10) + '0';

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_dia_info = {
	.alloc = curp_error_dia_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_mes_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;
	uint64_t mes;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	mes = NUMBER(t, 12, 99);
	if (mes == 12) mes = 0;
	curp[6] = (char)(mes / 10) + '0';
	curp[7] = (char)(mes % 10) + '0';

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_mes_info = {
	.alloc = curp_error_mes_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_sexo_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	do curp[10] = UPPER_CHARACTER(t);
	while (curp[10] == 'H' || curp[10] == 'M' || curp[10] == 'X');

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_sexo_info = {
	.alloc = curp_error_sexo_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_entidad_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	do {
		curp[11] = UPPER_CHARACTER(t);
		curp[12] = UPPER_CHARACTER(t);
	} while (isstate(curp[11], curp[12]));

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_entidad_info = {
	.alloc = curp_error_entidad_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_consonante_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	curp[NUMBER(t, 13, 15)] = VOWEL(t);

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_consonante_info = {
	.alloc = curp_error_consonante_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_verificador_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	curp[17] = (char)((curp[17] - '0' + NUMBER(t, 1, 9)) % 10) + '0';

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_verificador_info = {
	.alloc = curp_error_verificador_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_larga_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;
	uint64_t i, n;

	random_curp_alloc_cb(t, env, instance);

	n = NUMBER(t, 1, 64);
	curp = realloc(*instance, CURP_LENGTH + n + 1);
	if (curp == NULL) {
		free(*instance);
		return THEFT_ALLOC_ERROR;
	}

	for (i = 0; i < n; ++i)
		curp[CURP_LENGTH + i] = (char)NUMBER(t, 1, 255);
	curp[CURP_LENGTH + n] = '\0';

	*instance = curp;
	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_larga_info = {
	.alloc = curp_error_larga_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static enum theft_alloc_res
curp_error_corta_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	curp[NUMBER(t, 0, CURP_LENGTH - 1)] = '\0';

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_corta_info = {
	.alloc = curp_error_corta_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static const int posiciones_letra[] = {
	0, 1, 2, 3, 10, 11, 12, 13, 14, 15
};

static enum theft_alloc_res
curp_error_digito_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	curp[posiciones_letra[NUMBER(t, 0, 9)]] = DIGIT_CHARACTER(t);

	curp[17] = '0';
	curp[17] = (char)(calcular_verificador(curp) + '0');

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_digito_info = {
	.alloc = curp_error_digito_alloc_cb,
	.free = theft_generic_free_cb,
	.print = theft_generic_print_cb,
	.autoshrink_config = { .enable = 1 }
};

static const int posiciones_digito[] = {
	4, 5, 6, 7, 8, 9, 17
};

static enum theft_alloc_res
curp_error_letra_alloc_cb(struct theft *t, void *env, void **instance) {
	char *curp;
	int pos;

	random_curp_alloc_cb(t, env, instance);
	curp = *instance;

	pos = posiciones_digito[NUMBER(t, 0, 6)];
	curp[pos] = UPPER_CHARACTER(t);

	if (pos != 17) {
		curp[17] = '0';
		curp[17] = (char)(calcular_verificador(curp) + '0');
	}

	return THEFT_ALLOC_OK;
}

static struct theft_type_info curp_error_letra_info = {
	.alloc = curp_error_letra_alloc_cb,
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

static enum theft_trial_res
curp_debe_dar_error_vocal(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_VOCAL) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 1) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_vocal)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_vocal,
		.type_info = { &curp_error_vocal_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_altisonante(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_ALTISONANTE) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 3) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_altisonante)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_altisonante,
		.type_info = { &curp_error_altisonante_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_mes(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_FECHA) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 8) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_mes)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_mes,
		.type_info = { &curp_error_mes_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_dia(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_FECHA) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 4) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_dia)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_dia,
		.type_info = { &curp_error_dia_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_sexo(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_SEXO) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 10) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_sexo)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_sexo,
		.type_info = { &curp_error_sexo_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_entidad(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);
	int pos = isstateprefix(curp[11]) ? 12 : 11;

	if (curp_error(r) != ERROR_ENTIDAD) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != pos) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_entidad)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_entidad,
		.type_info = { &curp_error_entidad_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_consonante(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);
	int pos;

	for (pos = 13; !isvowel(curp[pos]); ++pos);

	if (curp_error(r) != ERROR_CONSONANTE) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != pos) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_consonante)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_consonante,
		.type_info = { &curp_error_consonante_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_verificador(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_VERIFICADOR) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 18) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_verificador)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_verificador,
		.type_info = { &curp_error_verificador_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_larga(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_FORMATO) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != 18) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_larga)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_larga,
		.type_info = { &curp_error_larga_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_corta(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);

	if (curp_error(r) != ERROR_FORMATO) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != (int)strlen(curp)) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_corta)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_corta,
		.type_info = { &curp_error_corta_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_digito(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);
	int i;

	for (i = 0; !isdigit(curp[posiciones_letra[i]]); ++i);

	if (curp_error(r) != ERROR_FORMATO) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != posiciones_letra[i]) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_digito)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_digito,
		.type_info = { &curp_error_digito_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_error_letra(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	int r = curp_validar(curp, NULL);
	int i;

	for (i = 0; isdigit(curp[posiciones_digito[i]]); ++i);

	if (curp_error(r) != ERROR_FORMATO) return THEFT_TRIAL_FAIL;
	if (CURP_ERROR_POSICION(r) != posiciones_digito[i]) return THEFT_TRIAL_FAIL;
	return THEFT_TRIAL_PASS;
}

START_TEST(test_curp_debe_dar_error_letra)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_error_letra,
		.type_info = { &curp_error_letra_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_anio(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	struct curp p;
	unsigned int anio;

	if (curp_validar(curp, &p) != CURP_VALIDA) return THEFT_TRIAL_FAIL;

	anio = 1900 + (curp[4] - '0') * 10 + (curp[5] - '0');
	if (isupper(curp[16])) anio += 100;

	return p.anio_nacimiento == anio ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_dar_anio)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_anio,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_mes(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	struct curp p;
	unsigned int mes;

	if (curp_validar(curp, &p) != CURP_VALIDA) return THEFT_TRIAL_FAIL;

	mes = (curp[6] - '0') * 10 + (curp[7] - '0');

	return p.mes_nacimiento == mes ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_dar_mes)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_mes,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_dia(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	struct curp p;
	unsigned int dia;

	if (curp_validar(curp, &p) != CURP_VALIDA) return THEFT_TRIAL_FAIL;

	dia = (curp[8] - '0') * 10 + (curp[9] - '0');

	return p.dia_nacimiento == dia ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_dar_dia)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_dia,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_sexo(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	struct curp p;
	int sexo;

	if (curp_validar(curp, &p) != CURP_VALIDA) return THEFT_TRIAL_FAIL;

	sexo = curp[10] == 'H' ? HOMBRE : curp[10] == 'M' ? MUJER : 0;

	return (int)p.sexo == sexo ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_dar_sexo)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_sexo,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

	enum theft_run_res res = theft_run(&config);
	ck_assert(res == THEFT_RUN_PASS);
}
END_TEST

static enum theft_trial_res
curp_debe_dar_entidad(struct theft *t, void *arg1) {
	const char *curp = (char *)arg1;
	struct curp p;
	int i;

	if (curp_validar(curp, &p) != CURP_VALIDA) return THEFT_TRIAL_FAIL;

	for (i = 0; entidades[i] != NULL; ++i)
		if (entidades[i][0] == curp[11] && entidades[i][1] == curp[12]) break;

	return (int)p.entidad_nacimiento == i ? THEFT_TRIAL_PASS : THEFT_TRIAL_FAIL;
}

START_TEST(test_curp_debe_dar_entidad)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config config = {
		.name = __func__,
		.prop1 = curp_debe_dar_entidad,
		.type_info = { &random_curp_info },
		.seed = seed,
		.trials = 1000000
	};

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
	tcase_add_test(tc_core, test_curp_debe_dar_anio);
	tcase_add_test(tc_core, test_curp_debe_dar_mes);
	tcase_add_test(tc_core, test_curp_debe_dar_dia);
	tcase_add_test(tc_core, test_curp_debe_dar_sexo);
	tcase_add_test(tc_core, test_curp_debe_dar_entidad);
	tcase_add_test(tc_core, test_curp_debe_dar_error_corta);
	tcase_add_test(tc_core, test_curp_debe_dar_error_larga);
	tcase_add_test(tc_core, test_curp_debe_dar_error_digito);
	tcase_add_test(tc_core, test_curp_debe_dar_error_letra);
	tcase_add_test(tc_core, test_curp_debe_dar_error_vocal);
	tcase_add_test(tc_core, test_curp_debe_dar_error_altisonante);
	tcase_add_test(tc_core, test_curp_debe_dar_error_mes);
	tcase_add_test(tc_core, test_curp_debe_dar_error_dia);
	tcase_add_test(tc_core, test_curp_debe_dar_error_sexo);
	tcase_add_test(tc_core, test_curp_debe_dar_error_entidad);
	tcase_add_test(tc_core, test_curp_debe_dar_error_consonante);
	tcase_add_test(tc_core, test_curp_debe_dar_error_verificador);
	suite_add_tcase(suite, tc_core);

	return suite;
}
