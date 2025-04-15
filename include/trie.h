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

#ifndef CURP_TRIE_H
#define CURP_TRIE_H

#define TRIE_WIDTH 26 /* por cada letra del alfabeto */

/* macro para obtener nodo en char c */
#define TRIE_GET(t, c) t->children[c - 'A']

/* constructo para busqueda de palabras eficiente */
struct trie_node {
	struct trie_node *children[TRIE_WIDTH];
	int is_eow;
	int value; /* cualquier valor asociado */
};

/* crear un trie vacio */
struct trie_node *trie_create();

/* destruir un trie y sus descendientes */
void trie_destroy(struct trie_node *t);

/* insertar una palabra en un trie */
void trie_insert(struct trie_node *t, const char *s, int v);

#endif
