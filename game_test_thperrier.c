#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

/* ********** DUMMY ********** */
bool test_dummy() {
	return true;
}

/* ********** game_get_black_number ********** */
bool test_game_get_black_number() {
	square tab_square[] = {S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4};
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (game_get_black_number(g, i, j) != tab_index) {
					game_delete(g);
					return false;
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ********** game_is_black ********** */
bool test_game_is_black() {
	square tab_square[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (tab_square[tab_index] <= S_MARK) {
					if (game_is_black(g, i, j)) {
						game_delete(g);
						return false;
					}
				} else {
					if (!((game_get_square(g, i, j) == tab_square[tab_index]) && game_is_black(g, i, j))) {
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

/* ********** game_is_lightbulb ********** */
bool test_game_is_lightbulb() {
	square tab_square[] = {S_BLANK, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	game g = game_new_empty();
	game_set_square(g, 0, 0, S_LIGHTBULB);
	if (!((game_get_square(g, 0, 0) == S_LIGHTBULB) && game_is_lightbulb(g, 0, 0))) {
		game_delete(g);
		return false;
	}
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (game_is_lightbulb(g, 0, 0)) {
					game_delete(g);
					return false;
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ********** game_is_blank ********** */
bool test_game_is_blank() {
	square tab_square[] = {S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (game_is_blank(g, i, j)) {
					game_delete(g);
					return false;
				}
			}
		}
	}
	game_delete(g);
	return true;
}

/* ********** game_delete ********** */
bool test_game_delete() {
	game g = game_new_empty();
	assert(g);
	game_delete(g);
	return true;
}

/* ********** game_equal ********** */
bool test_game_equal() {
	square tab_square[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	bool equal = true;
	uint index_tab = 0;
	game g1 = game_new_empty();
	for (int i = 0; i < g1->height; i++) {
		for (int j = 0; j < g1->width; j++) {
			// check if index_tab reach end of tab
			if (index_tab == sizeof(tab_square) / sizeof(tab_square[0])) {
				index_tab = 0;
			}
			game_set_square(g1, i, j, tab_square[index_tab]);
			index_tab++;
		}
	}
	game g2 = game_copy(g1);
	for (int i = 0; i < g1->height; i++) {
		for (int j = 0; j < g1->width; j++) {
			if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) {
				equal = false;
			}
		}
	}
	equal = (equal == game_equal(g1, g2));
	game_delete(g1);
	game_delete(g2);
	return equal;
}

/* ********** game_new_empty ********** */
bool test_game_new_empty() {
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			if (game_get_square(g, i, j) != S_BLANK) {
				game_delete(g);
				return false;
			}
		}
	}
	game_delete(g);
	return true;
}

/* ********** game_copy ********** */
bool test_game_copy() {
	square tab_square[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	uint index_tab = 0;
	game g1 = game_new_empty();
	for (int i = 0; i < g1->height; i++) {
		for (int j = 0; j < g1->width; j++) {
			// check if index_tab reach end of tab
			if (index_tab == sizeof(tab_square) / sizeof(tab_square[0])) {
				index_tab = 0;
			}
			game_set_square(g1, i, j, tab_square[index_tab]);
			index_tab++;
		}
	}
	game g2 = game_copy(g1);
	// check if g2 is a correct copy of g1
	if (!game_equal(g1, g2)) {
		game_delete(g1);
		game_delete(g2);
		return false;
	}
	game_delete(g1);
	game_delete(g2);
	return true;
}

/* ********** game_undo ********** */
bool test_game_undo() {
	game g1 = game_new_empty();
	game g2 = game_copy(g1);
	game_undo(g1);
	if (!game_equal(g1, g2)) {
		game_delete(g1);
		game_delete(g2);
		return false;
	}
	game_play_move(g1, 0, 0, S_LIGHTBULB);
	game_play_move(g1, 1, 1, S_MARK);
	game g3 = game_copy(g1);
	game_play_move(g3, 1, 1, S_BLANK);
	game_undo(g1);
	if (!game_equal(g1, g3)) {
		game_delete(g1);
		game_delete(g2);
		game_delete(g3);
		return false;
	}
	game_delete(g1);
	game_delete(g2);
	game_delete(g3);
	return true;
}

/* ********** game_redo ********** */
bool test_game_redo() {
	game g1 = game_new_empty();
	game_play_move(g1, 0, 0, S_LIGHTBULB);
	game_play_move(g1, 1, 1, S_MARK);
	game g2 = game_copy(g1);
	game_undo(g1);
	game_redo(g1);
	game_redo(g1);
	if (!game_equal(g1, g2)) {
		game_delete(g1);
		game_delete(g2);
		return false;
	}
	game_play_move(g1, 2, 2, S_LIGHTBULB);
	game_play_move(g2, 2, 2, S_LIGHTBULB);
	game_redo(g1);
	if (!game_equal(g1, g2)) {
		game_delete(g1);
		game_delete(g2);
		return false;
	}
	game_delete(g1);
	game_delete(g2);
	return true;
}

/* ********** usage ********** */
void usage(int argc, char* argv[]) {
	fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
	exit(EXIT_FAILURE);
}
/* ********** main ********** */
int main(int argc, char* argv[]) {
	bool success = false;
	if (argc != 2) {
		usage(argc, argv);
	} else if (strcmp("dummy", argv[1]) == 0) {
		success = test_dummy();
	} else if (strcmp("game_new_empty", argv[1]) == 0) {
		success = test_game_new_empty();
	} else if (strcmp("game_copy", argv[1]) == 0) {
		success = test_game_copy();
	} else if (strcmp("game_equal", argv[1]) == 0) {
		success = test_game_equal();
	} else if (strcmp("game_delete", argv[1]) == 0) {
		success = test_game_delete();
	} else if (strcmp("game_is_blank", argv[1]) == 0) {
		success = test_game_is_blank();
	} else if (strcmp("game_is_lightbulb", argv[1]) == 0) {
		success = test_game_is_lightbulb();
	} else if (strcmp("game_is_black", argv[1]) == 0) {
		success = test_game_is_black();
	} else if (strcmp("game_get_black_number", argv[1]) == 0) {
		success = test_game_get_black_number();
	} else if (strcmp("game_undo", argv[1]) == 0) {
		success = test_game_undo();
	} else if (strcmp("game_redo", argv[1]) == 0) {
		success = test_game_redo();
	} else {
		fprintf(stderr, "test \"%s\" is not a valid test name.\n", argv[1]);
		return EXIT_FAILURE;
	}
	if (success) {
		fprintf(stderr, "test \"%s\" finished successfully\n", argv[1]);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "test \"%s\" error\n", argv[1]);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}