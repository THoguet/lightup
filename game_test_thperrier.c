#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

#define TAB_SQUARE \
	{ S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU }

/* ********** DUMMY ********** */
bool test_dummy() {
	return true;
}

/* ********** game_get_black_number ********** */
bool test_game_get_black_number() {
	square tab_square[] = TAB_SQUARE;
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (tab_square[tab_index] == S_BLACKU) {
					if (game_get_black_number(g, i, j) != -1) {
						game_delete(g);
						return false;
					}
				} else if (tab_square[tab_index] > S_MARK && game_get_black_number(g, i, j) != tab_square[tab_index] - S_BLACK) {
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
	square tab_square[] = TAB_SQUARE;
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
	square tab_square[] = TAB_SQUARE;
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (int tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (tab_square[tab_index] == S_LIGHTBULB) {
					if (!game_is_lightbulb(g, i, j)) {
						game_delete(g);
						return false;
					}
				} else if (game_is_lightbulb(g, 0, 0)) {
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
	game_set_square(g, 0, 0, S_BLANK);
	if (!((game_get_square(g, 0, 0) == S_BLANK) && game_is_blank(g, 0, 0))) {
		game_delete(g);
		return false;
	}
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

int test_equal_ext(void) {
	square ext_3x10_squares[] = {
	    S_BLANK,  S_BLANK,  S_BLANK, S_BLACK1, S_BLANK,  /* row 0 */
	    S_BLANK,  S_BLANK,  S_BLANK, S_BLACK1, S_BLACKU, /* row 0 */
	    S_BLACK1, S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  /* row 1 */
	    S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLACK1, /* row 1 */
	    S_BLACKU, S_BLACK0, S_BLANK, S_BLANK,  S_BLANK,  /* row 2 */
	    S_BLANK,  S_BLACK0, S_BLANK, S_BLANK,  S_BLANK,  /* row 2 */
	};
	game g1 = game_new_ext(3, 10, ext_3x10_squares, false);
	game g2 = game_new_ext(3, 10, ext_3x10_squares, false);
	game g3 = game_new_ext(3, 10, ext_3x10_squares, true);

	// same game
	bool test1 = (game_equal(g1, g2) == true);

	// set a single different square
	game_set_square(g2, 2, 9, S_LIGHTBULB);
	bool test2 = (game_equal(g1, g2) == false);

	// different options
	bool test3 = (game_equal(g1, g3) == false);

	game_delete(g1);
	game_delete(g2);
	game_delete(g3);

	if (test1 && test2 && test3)
		return true;
	return false;
}

int test_equal(void) {
	game g1 = game_default();
	game g2 = game_default();
	game g3 = game_default();
	game g4 = game_default();

	bool test1 = (game_equal(g1, g2) == true);

	game_play_move(g3, 0, 0, S_LIGHTBULB);
	bool test2 = (game_equal(g1, g3) == false);

	game_set_square(g4, 0, 0, F_LIGHTED);
	bool test3 = (game_equal(g1, g4) == false);

	game_delete(g1);
	game_delete(g2);
	game_delete(g3);
	game_delete(g4);

	if (test1 && test2 && test3)
		return true;
	return false;
}

/* ********** game_new_empty ********** */
bool test_game_new_empty() {
	game g = game_new_empty();
	// check if each square is a S_BLANK
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
	for (int wrap = 0; wrap < 2 /*we only need to test two versions (with and without wrapping)*/; wrap++) {
		game g1 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, wrap);
		for (int i = 0; i < game_nb_rows(g1); i++) {
			for (int j = 0; j < game_nb_cols(g1); j++) {
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
	}
	return true;
}

/* ********** game_undo ********** */
bool test_game_undo() {
	game g1 = game_new_empty();
	game g2 = game_copy(g1);
	// check if you can undo without playing a move
	game_undo(g2);
	if (!game_equal(g1, g2)) {
		game_delete(g1);
		game_delete(g2);
		return false;
	}
	game_play_move(g1, 0, 0, S_LIGHTBULB);
	game_play_move(g1, 1, 1, S_MARK);
	game g3 = game_copy(g1);
	// check if the function undo correctly
	game_play_move(g3, 2, 2, S_LIGHTBULB);
	game_undo(g3);
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
	// check if you can redo a non existant move
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
	// check if the function redo correctly
	game_undo(g1);
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
		success = test_equal_ext() && test_equal();
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