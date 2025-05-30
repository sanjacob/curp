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
#include "trie.h"
#include "curp.h"

static struct trie_node *entidades_trie,
                        *altisonantes_trie,
                        *preposiciones_trie;


/* para reconocer vocales*/
static const unsigned int vowels[26] = {
/*  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, ... */
	1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1
};

/* sexos validos de la CURP */
static const unsigned int sexos[26] = {
/*  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x */
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

/* cuales caracteres en la CURP deben ser letras */
static const unsigned int uppers[] = {
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0
};

static const unsigned int digits[] = {
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

/* para calcular digito verificador */
static const unsigned int factor[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, /* digitos */
    0, 0, 0, 0, 0, 0, 0, /* simbolos */
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 5, 6, 7, 8, 9,
	0, 1, 2, 3, 4, 5, 6 /* letras */
};

/* dias en cada mes */
static const unsigned int days_in_month[] = {
    0, /* unused; this vector uses 1-based indexing */
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};	

#define isvowel(c) vowels[c - 'A']
#define isconsonant(c) !vowels[c - 'A']


void curp_exit(void)
{
	trie_destroy(entidades_trie);
	trie_destroy(altisonantes_trie);
	trie_destroy(preposiciones_trie);

	entidades_trie = NULL;
	altisonantes_trie = NULL;
	preposiciones_trie = NULL;
}


int curp_init(void)
{
	int i;
	
	entidades_trie = trie_create();
	altisonantes_trie = trie_create();
	preposiciones_trie = trie_create();

	if (entidades_trie == NULL
		|| altisonantes_trie == NULL
		|| preposiciones_trie == NULL)
	{
		curp_exit();
		return -1;
	}

	/* almacenar indice del estado */
	for (i = 0; entidades[i] != NULL; ++i)
		trie_insert(entidades_trie, entidades[i], i);
	
	/* preposiciones de nombre a ignorar */
	for (i = 0; preposiciones[i] != NULL; ++i)
		trie_insert(preposiciones_trie, preposiciones[i], -1);
	
	/* palabras altisonantes o inconvenientes */
	for (i = 0; altisonantes[i] != NULL; ++i)
		trie_insert(altisonantes_trie, altisonantes[i], -1);

	return 1;
}

/* para curp prevalidada */
int curp_cargar(const char *s, struct curp* p)
{
	struct trie_node *node;
	const char *c;
	unsigned int i;

	node = entidades_trie;
	p->sexo = 0;
	p->entidad_nacimiento = 0;
	p->anio_nacimiento = 0;
	p->mes_nacimiento = 0;
	p->dia_nacimiento = 0;

	for (c = s, i = 0; *c != '\0' && i <= 16; ++i, ++c) {
		switch (i) {
		case 4: case 5:
			p->anio_nacimiento = (10 * p->anio_nacimiento) + (*c - '0');
			break;
		case 6: case 7:
			p->mes_nacimiento = (10 * p->mes_nacimiento) + (*c - '0');
			break;
		case 8: case 9:
			p->dia_nacimiento = (10 * p->dia_nacimiento) + (*c - '0');
			break;
		case 10:
			p->sexo = (*c == 'H' ? HOMBRE : (*c == 'M' ? MUJER : 0));
			break;
		case 11: case 12:
			if (node != NULL) node = TRIE_GET(node, *c);
			break;
		case 16:
			p->anio_nacimiento += 1900 + (isupper(*c) * 100);
			break;
		default:
			break;
		}
	}

	if (node != NULL) p->entidad_nacimiento = node->value;
	return i == 18 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int curp_validar(const char *s, struct curp* p)
{
	struct trie_node *node;
	const char *c;
	unsigned int i, sum, ok;

	/* guarda la fecha para validarla */
	unsigned int h, year, month, day, dim, is_leap;
	unsigned char sexo;

	if (s == NULL) return ERROR_FORMATO;

	node = altisonantes_trie;
	year = month = day = h = 0;

    /* 0 1 2 3   4 5 6 7 8 9   0   1 2   3 4 5   6 7
       S A B C | 5 6 0 6 2 6 | M | D F | L R N | 0 9 */
	for (c = s, i = sum = 0, ok = 1; *c != '\0'; ++i, ++c) {
		/* verificar formato */
		if (uppers[i] && !isupper(*c)) break;
		else if (digits[i] && !isdigit(*c)) break;

		switch (i) {
		case 1: ok = (isvowel(*c) || *c == 'X');
		case 0: case 2: case 3:
			if (node != NULL) node = TRIE_GET(node, *c);
			ok = node == NULL || !node->is_eow;
			break;
		case 4: case 5: year = (10 * year) + (*c - '0'); break;
		case 6: case 7: month = (10 * month) + (*c - '0'); break;
		case 8: ok = month >= 1 && month <= 12;
		case 9: day = (10 * day) + (*c - '0'); break;
		case 10: ok = sexos[*c - 'A']; sexo = *c; break;
		case 11: node = entidades_trie;
		case 12: node = TRIE_GET(node, *c); ok = node != NULL; break;
		case 13: case 14: case 15: ok = isconsonant(*c); break;
		case 16: ok = isdigit(*c) || (h = isupper(*c)); break;
		case 17: ok = isdigit(*c); break;
		default: ok = 0; break;
		}

		if (!ok) break;
		sum += (18 - i) * factor[*c - '0'];
	}

	if (i != 18 || !ok) return -(ok << 7 | i);

	/* revisar dia del mes, anio bisiesto */
	is_leap = (year % 4 == 0) && (year || h);
	dim = month == 2 && is_leap ? 29 : days_in_month[month];
	if (day < 1 || day > dim) return ERROR_FECHA;

	if (sum % 10 != 0) return -(ok << 7 | i);

	/* guardar detalles de la curp */
	if (p != NULL) {
		p->anio_nacimiento = 1900 + year + (100 * h);
		p->mes_nacimiento = month;
		p->dia_nacimiento = day;
		p->entidad_nacimiento = node->value;
		p->sexo = sexo == 'H' ? HOMBRE : (sexo == 'M' ? MUJER : 0);
	}

	return CURP_VALIDA;
}

enum curp_err curp_error(int code) {
	int pos, ok;

	if (code == CURP_VALIDA) return CURP_VALIDA;

	pos = -code & 0x1F;
	ok = -code >> 7;

	if (ok) return pos == 18 ? ERROR_VERIFICADOR : ERROR_FORMATO;

	switch(pos) {
	case 8: return ERROR_FECHA;
	case 12: return ERROR_ENTIDAD;
	case 14: case 15: return ERROR_CONSONANTE;
	case 16: case 18: return ERROR_FORMATO;
	default: return -pos;
	}
}

const char *curp_entidad_iso(unsigned int entidad)
{
	if (entidad > 31) return NULL;
	return entidades_iso[entidad];
}

const char *curp_entidad_nombre(unsigned int entidad)
{
	if (entidad > 31) return NULL;
	return entidades_nombre[entidad];
}

static struct name_features find_char_vowel_consonant(const char *s)
{
	const char *c;
    char letter, vowel, consonant;
	struct name_features n;

    static unsigned int vowels[26] = {
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 1
    };

    /* Find first char, vowel, and consonant */
    n.initial = *s;
    n.vowel = '\0';
    n.consonant = '\0';

    for (c = s, ++c; *c != '\0'; ++c) {
        if (isupper(*c)) {
            unsigned int is_vowel = vowels[*c - 'A'];

            if (!n.vowel && is_vowel) {
                /* found vowel */
                n.vowel = *c;
                if (n.consonant) break;
            } else if (!n.consonant && !is_vowel) {
                /* found consonant */
                n.consonant = *c;
                if (n.vowel) break;
            }
        } else if (isspace(*c)) break;
    }

	return n;
}

void curp_crear(char *curp, struct curp p, const char *nombre,
	const char *primer_apellido, const char *segundo_apellido)
{
	struct trie_node *node;
	struct name_features *n, nombres[] = {
		'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'
	};

	int *d, date[3] = { 2000, 1, 1 };

	unsigned int anio, sexo, i, sum;
	const char *entidad;
	char c;

	node = altisonantes_trie;
	n = nombres;
	d = date;
	anio = sexo = 0;
	entidad = entidades[0];

	if (primer_apellido != NULL)
		nombres[0] = find_char_vowel_consonant(primer_apellido);
	if (segundo_apellido != NULL)
		nombres[1] = find_char_vowel_consonant(segundo_apellido);
	if (nombre != NULL)
		nombres[2] = find_char_vowel_consonant(nombre);

	if (p.anio_nacimiento >= 1) anio = p.anio_nacimiento;
	if (p.mes_nacimiento > 0 && p.mes_nacimiento <= 12) date[1] = p.mes_nacimiento;
	if (p.dia_nacimiento > 0 && p.dia_nacimiento <= 31) date[2] = p.dia_nacimiento;

	date[0] = p.anio_nacimiento % 100;

	if (p.sexo >= 0 && p.sexo <= 2) sexo = p.sexo;
	if (p.entidad_nacimiento >= 0 && p.entidad_nacimiento <= 32)
		entidad = entidades[p.entidad_nacimiento];

	for (i = sum = 0; curp[i] != '\0' && i < CURP_LENGTH; ++i) {
		switch (i) {
			case 1: c = nombres->vowel;
				if (node != NULL) node = TRIE_GET(node, c); break;
			case 0: case 2: case 3: c = n++->initial;
				if (node != NULL) node = TRIE_GET(node, c); break;
			case 4: case 6: case 8: c = '0' + (*d / 10); break;
			case 5: case 7: case 9: c = '0' + (*d++ % 10); break;
			case 10: c = sexo_chars[sexo]; break;
			case 11: case 12: c = *entidad++; break;
			case 13: n = nombres;
			case 14: case 15: c = n++->consonant; break;
			case 16: c = anio >= 2000 ? 'A' : '1'; break;
			case 17: c = '0' + ((10 - (sum % 10)) % 10); break;
			default: c = 'X'; break;
		}

		curp[i] = c;
		sum += (CURP_LENGTH - i) * factor[c - '0'];
	}

	/* censurar palabra altisonante */
	if (node != NULL) curp[1] = 'X';
}
