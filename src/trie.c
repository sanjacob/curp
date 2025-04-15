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
#include "trie.h"

/* malloc, initializar un solo nodo de trie */
struct trie_node *trie_create() {
	struct trie_node *t;
	int i;

	/* obtener memoria */
	t = (struct trie_node *) malloc(sizeof(struct trie_node));

	if (t == NULL) return NULL;

	/* inicializar todos los ptrs */
	for (i = 0; i < TRIE_WIDTH; ++i)
		t->children[i] = NULL;

	t->is_eow = 0;
	t->value = -1;
	return t;
}

/* dealloc un trie de forma recursiva */
void trie_destroy(struct trie_node *t) {
	int i;
	if (t == NULL) return;

	for (i = 0; i < 26; ++i) {
		if (t->children[i] != NULL) {
			trie_destroy(t->children[i]);
			t->children[i] = NULL;
		}
	}

	free(t);
}

/* insertar una palabra, creando mas nodos como sea necesario */
void trie_insert(struct trie_node *t, const char *s, int v) {
	struct trie_node *p;
	int i, c;

	p = t;
	for (i = 0; s[i] != '\0'; ++i) {
		c = s[i] - 'A';

		if (p->children[c] == NULL)
			p->children[c] = trie_create();

		p = p->children[c];
	}

	p->is_eow = 1;
	p->value = v;
}
