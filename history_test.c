#include "history.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_history_alloc(void) {
	history new = history_alloc();
	assert(new);
	history_free(new);
	return true;
}

bool test_history_create_empty(void) {
	assert(history_create_empty() == NULL);
	return true;
}

bool test_history_delete_entire_history(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history_insert_after(new, new, S_MARK, 3, 3);
	assert(new->next);
	history_delete_entire_history(new);
	return true;
}

bool test_history_is_empty(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	assert(!history_is_empty(new));
	assert(history_is_empty(history_create_empty()));
	history_free(new);
	return true;
}

bool test_history_next(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	new = history_insert_after(new, new, S_BLACKU, 2, 6);
	assert(new->next == history_next(new));
	history_delete_entire_history(new);
	return true;
}

bool test_history_prev(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	new = history_insert_after(new, new, S_BLACKU, 2, 6);
	new = history_next(new);
	assert(new->prev == history_prev(new));
	history_delete_entire_history(new);
	return true;
}

bool test_history_insert_first(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history_free(new);
	return true;
}

bool test_history_delete_first(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	new = history_insert_after(new, new, S_BLACK3, 2, 5);
	new = history_next(new);
	history_delete_first(new);
	assert(new->prev == NULL);
	history_delete_entire_history(new);
	return true;
}

bool test_history_insert_after(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	history_insert_after(new, new, S_BLACK2, 4, 3);
	assert(new->next->next == newafter);
	assert(new == newafter->prev->prev);
	assert(new->next->state == S_BLACK2);
	assert(new->next->i == 4);
	assert(new->next->j == 3);
	history_delete_entire_history(new);
	return true;
}

bool test_history_delete_after(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	history_insert_after(new, new, S_BLACK2, 4, 3);
	history_delete_after(new, new);
	assert(new->next == newafter);
	assert(new == newafter->prev);
	history_delete_entire_history(new);
	return true;
}

bool test_history_insert_before(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	history_insert_before(new, newafter, S_BLACK2, 4, 3);
	assert(new->next->next == newafter);
	assert(new == newafter->prev->prev);
	assert(new->next->state == S_BLACK2);
	assert(new->next->i == 4);
	assert(new->next->j == 3);
	history_delete_entire_history(new);
	return true;
}

bool test_history_delete_before(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	history_insert_after(new, new, S_BLACK2, 4, 3);
	history_delete_before(new, newafter);
	assert(new->next == newafter);
	assert(new == newafter->prev);
	history_delete_entire_history(new);
	return true;
}

bool test_history_last(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	assert(history_last(new) == newafter);
	history_delete_entire_history(new);
	return true;
}

bool test_history_first(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history newafter = history_alloc();
	newafter->prev = new;
	new->next = newafter;
	assert(history_first(newafter) == new);
	history_delete_entire_history(new);
	return true;
}

bool test_history_append(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	history_append(new, S_BLACK3, 2, 4);
	history last = history_last(new);
	assert(new->next == last);
	assert(last->state == S_BLACK3);
	assert(last->i == 2);
	assert(last->j == 4);
	history_delete_entire_history(new);
	return true;
}

bool test_history_copy(void) {
	history new = history_insert_first(history_create_empty(), S_LIGHTBULB, 4, 3);
	assert(new);
	new = history_insert_after(new, new, S_BLANK, 5, 8);
	history h2 = history_copy(new);
	assert(history_state(h2) != history_state(new));
	assert(history_state(history_next(h2)) == history_state(history_next(new)));
	history_delete_entire_history(h2);
	history_delete_entire_history(new);
	return true;
}

int usage(int argc, char* argv[]) {
	fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		usage(argc, argv);
	}
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool ok = true;
	if (strcmp("history_alloc", argv[1]) == 0)
		ok = test_history_alloc();
	else if (strcmp("history_create_empty", argv[1]) == 0)
		ok = test_history_create_empty();
	else if (strcmp("history_delete_entire_history", argv[1]) == 0)
		ok = test_history_delete_entire_history();
	else if (strcmp("history_is_empty", argv[1]) == 0)
		ok = test_history_is_empty();
	else if (strcmp("history_next", argv[1]) == 0)
		ok = test_history_next();
	else if (strcmp("history_prev", argv[1]) == 0)
		ok = test_history_prev();
	else if (strcmp("history_insert_first", argv[1]) == 0)
		ok = test_history_insert_first();
	else if (strcmp("history_delete_first", argv[1]) == 0)
		ok = test_history_delete_first();
	else if (strcmp("history_insert_after", argv[1]) == 0)
		ok = test_history_insert_after();
	else if (strcmp("history_delete_after", argv[1]) == 0)
		ok = test_history_delete_after();
	else if (strcmp("history_insert_before", argv[1]) == 0)
		ok = test_history_insert_before();
	else if (strcmp("history_delete_before", argv[1]) == 0)
		ok = test_history_delete_before();
	else if (strcmp("history_last", argv[1]) == 0)
		ok = test_history_last();
	else if (strcmp("history_first", argv[1]) == 0)
		ok = test_history_first();
	else if (strcmp("history_append", argv[1]) == 0)
		ok = test_history_append();
	else if (strcmp("history_copy", argv[1]) == 0)
		ok = test_history_copy();
	else {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	// print test result
	if (ok) {
		fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
		return EXIT_FAILURE;
	}
}