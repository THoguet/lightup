#include "history.h"
#include <stdio.h>
#include <stdlib.h>

void memoryError(void) {
	fprintf(stderr, "Not enought memory\n");
	exit(EXIT_FAILURE);
}

void errPointer(void) {
	fprintf(stderr, "Error on the given pointer\n");
	exit(EXIT_FAILURE);
}

history history_alloc() {
	history h = (history)malloc(sizeof(struct s_history));
	if (h == NULL)
		memoryError();
	h->i = 0;
	h->j = 0;
	h->next = NULL;
	h->prev = NULL;
	return h;
}

void history_free(history h) {
	free(h);
	h = NULL;
}

history history_create_empty(void) {
	return NULL;
}

bool history_is_empty(history h) {
	return h == NULL;
}

history history_next(history h) {
	if (history_is_empty(h))
		errPointer();
	return h->next;
}

history history_prev(history h) {
	if (history_is_empty(h))
		errPointer();
	return h->prev;
}

square history_state(history h) {
	if (history_is_empty(h))
		errPointer();
	return h->state;
}

square history_i(history h) {
	if (history_is_empty(h))
		errPointer();
	return h->i;
}

square history_j(history h) {
	if (history_is_empty(h))
		errPointer();
	return h->j;
}

history history_insert_first(history h, square state, uint i, uint j) {
	history new = history_alloc();
	new->state = state;
	new->i = i;
	new->j = j;
	new->next = h;
	if (!history_is_empty(h))
		h->prev = new;
	return new;
}

history history_delete_first(history h) {
	if (!history_is_empty(h)) {
		history tmp = h->next;
		history_free(h);
		h = tmp;
		if (!history_is_empty(h)) {
			h->prev = NULL;
		}
	}
	return h;
}

history history_insert_after(history h, history p, square state, uint i, uint j) {
	if (history_is_empty(h) || history_is_empty(p))
		errPointer();
	history new = history_alloc();
	new->state = state;
	new->i = i;
	new->j = j;
	new->next = p->next;
	p->next = new;
	new->prev = p;
	if (!history_is_empty(history_next(new)))
		new->next->prev = new;
	return h;
}

history history_delete_after(history h, history p) {
	if (history_is_empty(h) || history_is_empty(p))
		errPointer();
	if (!history_is_empty(history_next(p))) {
		history to_be_deleted = p->next;
		if (!history_is_empty(history_next(to_be_deleted))) {
			to_be_deleted->next->prev = p;
		}
		p->next = to_be_deleted->next;
		history_free(to_be_deleted);
	}
	return h;
}

history history_insert_before(history h, history p, square state, uint i, uint j) {
	if (history_is_empty(h) || history_is_empty(p))
		errPointer();
	history new = history_alloc();
	new->state = state;
	new->i = i;
	new->j = j;
	new->next = p;
	new->prev = p->prev;
	p->prev = new;
	if (!history_is_empty(history_prev(new)))
		new->prev->next = new;
	return h;
}

history history_delete_before(history h, history p) {
	if (history_is_empty(h) || history_is_empty(p))
		errPointer();
	if (!history_is_empty(history_prev(p))) {
		history to_be_deleted = p->prev;
		if (!history_is_empty(history_prev(to_be_deleted))) {
			to_be_deleted->prev->next = p;
		}
		p->prev = to_be_deleted->prev;
		history_free(to_be_deleted);
	}
	return h;
}