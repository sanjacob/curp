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

#include <stdlib.h>
#include <check.h>
#include "curp.h"

/* CURP test suite */

/******************* curp_cargar ************************/

START_TEST(test_curp_cargar)
{
	const char *curp = "SABC560626MDFLRN01";
	struct curp p;

	curp_cargar(curp, &p);
	ck_assert_int_eq(p.anio_nacimiento, 1956);
	ck_assert_int_eq(p.mes_nacimiento, 6);
	ck_assert_int_eq(p.dia_nacimiento, 26);
	ck_assert_int_eq(p.sexo, 2);
}
END_TEST

START_TEST(test_curp_cargar_error)
{
	const char *curp = "SABC560626";
	struct curp p;

	ck_assert_int_eq(curp_cargar(curp, &p), EXIT_FAILURE);
}
END_TEST

/******************* curp_validar ***********************/

/* CURP_VALIDA *****************************************/

START_TEST(test_curp_valida)
{
	const char *curp = "SABC560626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_eq(code, 1);
	ck_assert_int_eq(curp_error(code), CURP_VALIDA);
}
END_TEST

START_TEST(test_curp_valida_propiedades)
{
	const char *curp = "SABC560626MDFLRN01";
	struct curp p;

	curp_validar(curp, &p);

	ck_assert_int_eq(p.anio_nacimiento, 1956);
	ck_assert_int_eq(p.mes_nacimiento, 6);
	ck_assert_int_eq(p.dia_nacimiento, 26);
	ck_assert_int_eq(p.sexo, 2);
}
END_TEST

/* ERROR_FORMATO ***************************************/

START_TEST(test_curp_corta)
{
	/*                                   v */
	const char *curp = "SABC560626MDFLRN0";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_larga)
{
	const char *curp = "SABC560626MDFLRN012";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_inicial)
{
	/*                  v                 */
	const char *curp = "9ABC560626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_fecha)
{
	/*                      v            */
	const char *curp = "SABCA60626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_sexo)
{
	/*                            v       */
	const char *curp = "SABC5606260DFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_entidad)
{
	/*                             vv     */
	const char *curp = "SABC560626M00LRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_consonante)
{
	/*                               vvv */
	const char *curp = "SABC560626MDF00001";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_homonimia)
{
	/*                                  v */
	const char *curp = "SABC560626MDFLRN_1";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST

START_TEST(test_curp_formato_verificador)
{
	/*                                   v */
	const char *curp = "SABC560626MDFLRN0A";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FORMATO);
}
END_TEST


/* ERROR_XXXXXXXXXXX ***********************************/

START_TEST(test_curp_fecha_mes_1)
{
	/*                        vv          */
	const char *curp = "SABC560026MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FECHA);
}
END_TEST

START_TEST(test_curp_fecha_mes_2)
{
	/*                        vv          */
	const char *curp = "SABC561626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FECHA);
}
END_TEST

START_TEST(test_curp_fecha_dia_1)
{
	/*                          vv        */
	const char *curp = "SABC560600MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FECHA);
}
END_TEST

START_TEST(test_curp_fecha_dia_2)
{
	/*                          vv        */
	const char *curp = "SABC560632MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_FECHA);
}
END_TEST

START_TEST(test_curp_sexo)
{
	/*                            v       */
	const char *curp = "SABC560626ADFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_SEXO);
}
END_TEST

START_TEST(test_curp_entidad)
{
	/*                             vv     */
	const char *curp = "SABC560626MXXLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_ENTIDAD);
}
END_TEST

START_TEST(test_curp_entidad_2)
{
	/*                             vv     */
	const char *curp = "SABC560626MOXLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_ENTIDAD);
}
END_TEST

START_TEST(test_curp_verificador)
{
	/*                                   v*/
	const char *curp = "SABC560626MDFLRN09";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_VERIFICADOR);
}
END_TEST


/* ERROR_ALTISONANTE ***********************************/

START_TEST(test_curp_altisonante_1)
{
	/*                  vvvv              */
	const char *curp = "NACO560626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_ALTISONANTE);
}
END_TEST

START_TEST(test_curp_altisonante_2)
{
	/*                  vvvv              */
	const char *curp = "MOCO560626MDFLRN01";
	const int code = curp_validar(curp, NULL);

	ck_assert_int_lt(code, 0);
	ck_assert_int_eq(curp_error(code), ERROR_ALTISONANTE);
}
END_TEST

/******************* curp_entidad ************************/

START_TEST(test_curp_entidad_iso)
{
	const char *iso = curp_entidad_iso(1);
	ck_assert_str_eq(iso, "BCN");
}
END_TEST

START_TEST(test_curp_entidad_iso_ne)
{
	const char *iso = curp_entidad_iso(32);
	ck_assert_ptr_null(iso);
}
END_TEST

START_TEST(test_curp_entidad_nombre)
{
	const char *nombre = curp_entidad_nombre(19);
	ck_assert_str_eq(nombre, "Oaxaca");
}
END_TEST

START_TEST(test_curp_entidad_nombre_ne)
{
	const char *nombre = curp_entidad_nombre(32);
	ck_assert_ptr_null(nombre);
}
END_TEST


Suite * curp_suite(void)
{
	Suite *suite;
	TCase *tc_core, *tc_formato, *tc_entidad;

	suite = suite_create("curp");

	tc_formato = tcase_create("Formato");
	tc_core = tcase_create("Core");
	tc_entidad = tcase_create("Entidad");

	tcase_add_test(tc_core, test_curp_cargar);
	tcase_add_test(tc_core, test_curp_cargar_error);

	/* VALIDA */
	tcase_add_test(tc_core, test_curp_valida);
	tcase_add_test(tc_core, test_curp_valida_propiedades);

	/* FORMATO */
	tcase_add_test(tc_formato, test_curp_corta);
	tcase_add_test(tc_formato, test_curp_larga);
	tcase_add_test(tc_formato, test_curp_formato_inicial);
	tcase_add_test(tc_formato, test_curp_formato_fecha);
	tcase_add_test(tc_formato, test_curp_formato_sexo);
	tcase_add_test(tc_formato, test_curp_formato_entidad);
	tcase_add_test(tc_formato, test_curp_formato_consonante);
	tcase_add_test(tc_formato, test_curp_formato_homonimia);
	tcase_add_test(tc_formato, test_curp_formato_verificador);

	/* PROPIEDADES */
	tcase_add_test(tc_core, test_curp_fecha_mes_1);
	tcase_add_test(tc_core, test_curp_fecha_mes_2);
	tcase_add_test(tc_core, test_curp_fecha_dia_1);
	tcase_add_test(tc_core, test_curp_fecha_dia_2);
	tcase_add_test(tc_core, test_curp_sexo);
	tcase_add_test(tc_core, test_curp_entidad);
	tcase_add_test(tc_core, test_curp_entidad_2);
	tcase_add_test(tc_core, test_curp_verificador);

	/* ALTISONANTE */
	tcase_add_test(tc_core, test_curp_altisonante_1);
	tcase_add_test(tc_core, test_curp_altisonante_2);

	/* ENTIDAD */
	tcase_add_test(tc_entidad, test_curp_entidad_iso);
	tcase_add_test(tc_entidad, test_curp_entidad_iso_ne);
	tcase_add_test(tc_entidad, test_curp_entidad_nombre);
	tcase_add_test(tc_entidad, test_curp_entidad_nombre_ne);

	suite_add_tcase(suite, tc_core);
	suite_add_tcase(suite, tc_formato);
	suite_add_tcase(suite, tc_entidad);

	return suite;
}

int main(void)
{
	int failed;
	Suite *suite;
	SRunner *runner;

	/* setup */
	if (!curp_init()) return EXIT_FAILURE;

	suite = curp_suite();
	runner = srunner_create(suite);

	srunner_run_all(runner, CK_NORMAL);
	failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	/* teardown */
	curp_exit();

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
