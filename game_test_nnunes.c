#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

bool test_dummy(void) {
	return true;
}

void usage(int argc, char* argv[]) {
	fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
	exit(EXIT_FAILURE);
}

/* ************** game_set_square ************** */
bool test_game_set_square(void) {
	game g = game_new_empty();
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	square list_flag[] = {F_ERROR, F_LIGHTED, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				for (uint tab_index = 0; tab_index < sizeof(list) / sizeof(list[0]); tab_index++) {
					game_set_square(g, i, j, (list[tab_index] | list_flag[flag_index]));
					if (game_get_square(g, i, j) != (list[tab_index] | list_flag[flag_index])) {
						game_delete(g);
						return false;
					}
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ************** game_get_state ************** */
bool test_game_get_state(void) {
	game g = game_new_empty();
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	square list_flag[] = {S_BLANK, F_ERROR, F_LIGHTED, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				for (uint tab_index = 0; tab_index < sizeof(list) / sizeof(list[0]); tab_index++) {
					game_set_square(g, i, j, (list[tab_index] | list_flag[flag_index]));
					if (game_get_state(g, i, j) != list[tab_index]) {
						game_delete(g);
						return false;
					}
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ************** game_get_flags ************** */
bool test_game_get_flags(void) {
	game g = game_new_empty();
	square list_flag[] = {F_ERROR, F_LIGHTED, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				game_set_square(g, i, j, (S_BLANK | list_flag[flag_index]));
				if (game_get_flags(g, i, j) != list_flag[flag_index]) {
					game_delete(g);
					return false;
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ************** game_is_marked ************** */
bool test_game_is_marked(void) {
	game g = game_new_empty();
	square list_flag[] = {S_BLANK, F_ERROR, F_LIGHTED, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				game_set_square(g, i, j, (S_MARK | list_flag[flag_index]));
				if ((game_get_state(g, i, j) == S_MARK) && game_is_marked(g, i, j)) {
					game_delete(g);
					return true;
				}
			}
		}
	}
	game_delete(g);
	return false;
}

/* ************** game_is_lighted ************** */
bool test_game_is_lighted(void) {
	game g = game_new_empty();
	square list_flag[] = {F_LIGHTED, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				game_set_square(g, i, j, (S_BLANK | list_flag[flag_index]));
				if ((game_get_flags(g, i, j) == list_flag[flag_index]) && game_is_lighted(g, i, j)) {
					game_delete(g);
					return true;
				}
			}
		}
	}
	game_delete(g);
	return false;
}

/* ************** game_has_error ************** */
bool test_game_has_error(void) {
	game g = game_new_empty();
	square list_flag[] = {F_ERROR, (F_ERROR | F_LIGHTED)};
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
				game_set_square(g, i, j, (S_BLANK | list_flag[flag_index]));
				if ((game_get_flags(g, i, j) == list_flag[flag_index]) && game_has_error(g, i, j)) {
					game_delete(g);
					return true;
				}
			}
		}
	}
	game_delete(g);
	return false;
}

/* ************** game_nb_rows ************** */
bool test_game_nb_rows(void) {
	game g = game_new_empty();
	if (game_get_square(g, game_nb_rows(g) -1, 0) == S_BLANK) {
		game_delete(g);
		return true;
	}	
	game_delete(g);
	return false;
}

/* ************** game_nb_cols ************** */
bool test_game_nb_cols(void) {
	game g = game_new_empty();
	if (game_get_square(g, 0, game_nb_cols(g) - 1) == S_BLANK) {
		game_delete(g);
		return true;
	}
	game_delete(g);
	return false;
}


/* ************** game_is_wrapping ************** */
bool test_game_is_wrapping(void) {
	game g = game_new_empty();
	game_set_square(g, 0, 0, S_LIGHTBULB);
	game_set_square(g, 1, 0, S_BLACK);
	game_set_square(g, 0, 1, S_BLACK);
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		if (!(game_is_lighted(g, 0, i) && game_is_wrapping(g))) {
			game_delete(g);
			return true;
		}	
		if (!(game_is_lighted(g, i, 0) && game_is_wrapping(g))) {
			game_delete(g);
			return true;
		}
	}				
	game_delete(g);
	return false;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		usage(argc, argv);
	}
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool success = false;
	if (strcmp("dummy", argv[1]) == 0)
		success = test_dummy();
	else if (strcmp("game_set_square", argv[1]) == 0) {
		success = test_game_set_square();
	} else if (strcmp("game_get_state", argv[1]) == 0) {
		success = test_game_get_state();
	} else if (strcmp("game_get_flags", argv[1]) == 0) {
		success = test_game_get_flags();
	} else if (strcmp("game_is_marked", argv[1]) == 0) {
		success = test_game_is_marked();
	} else if (strcmp("game_is_lighted", argv[1]) == 0) {
		success = test_game_is_lighted();
	} else if (strcmp("game_has_error", argv[1]) == 0) {
		success = test_game_has_error();
	} else if (strcmp("game_nb_rows", argv[1]) == 0) {
		success = test_game_nb_rows();
	} else if (strcmp("game_nb_cols", argv[1]) == 0) {
		success = test_game_nb_cols();
	} else if (strcmp("game_is_wrapping", argv[1]) == 0) {
		success = test_game_is_wrapping();
	} else {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	if (success) {
		fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
		return EXIT_FAILURE;
	}
}