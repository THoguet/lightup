#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game_aux.h"
#include "game_examples.h"
#include "game_ext.h"
#include "game_tools.h"

#define TAB_SQUARE \
	{ S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU }

#define SIZE_LIMIT_GAME 10

// put usable square at the start of the list and S_BLANK at the first element
#define LIST_OF_SQUARE \
	{ S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_LIGHTED, F_ERROR }

#define USABLE_SQUARE 2

#define NB_CHAR_SAVED_FILE_FIRST_LINE 6  // <nb_rows>' '<nb_cols>' '<wrapping>'\n'

/* ********** game_get_black_number ********** */
bool test_game_get_black_number(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab_square[] = TAB_SQUARE;
	game g = game_new_empty();
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (uint tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
				game_set_square(g, i, j, tab_square[tab_index]);
				if (tab_square[tab_index] == S_BLACKU) {
					if (game_get_black_number(g, i, j) != -1) {
						game_delete(g);
						return false;
					}
				} else if (tab_square[tab_index] > S_MARK && (uint)game_get_black_number(g, i, j) != tab_square[tab_index] - S_BLACK) {
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
bool test_game_is_black(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab_square[] = TAB_SQUARE;
	game g = game_new_empty();
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (uint tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
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
bool test_game_is_lightbulb(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab_square[] = TAB_SQUARE;
	game g = game_new_empty();
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (uint tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
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
bool test_game_is_blank(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab_square[] = {S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	game g = game_new_empty();
	game_set_square(g, 0, 0, S_BLANK);
	if (!((game_get_square(g, 0, 0) == S_BLANK) && game_is_blank(g, 0, 0))) {
		game_delete(g);
		return false;
	}
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			// check for each case given in tab_square if the function return the expected result
			for (uint tab_index = 0; tab_index < (sizeof(tab_square) / sizeof(tab_square[0])); tab_index++) {
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
bool test_game_delete(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_new_empty();
	assert(g);
	game_delete(g);
	return true;
}

/* ********** game_equal ********** */

bool test_equal_ext(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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

bool test_equal(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_new_empty(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_new_empty();
	// check if each square is a S_BLANK
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
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
bool test_game_copy(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab_square[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
	uint index_tab = 0;
	for (int wrap = 0; wrap < 2 /*we only need to test two versions (with and without wrapping)*/; wrap++) {
		game g1 = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, wrap);
		for (uint i = 0; i < game_nb_rows(g1); i++) {
			for (uint j = 0; j < game_nb_cols(g1); j++) {
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
bool test_game_undo(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_redo(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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

/* ******* game_new_ext_empty ******* */
/**
 * @brief test creating every game between size 0 to SIZE_LIMIT_GAME
 *
 * @return true if every game has been created well
 */
bool test_game_new_empty_ext(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	for (uint height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (uint width = 1; width <= SIZE_LIMIT_GAME; width++) {
			for (int wrap = 0; wrap < 2; wrap++) {
				game g = game_new_empty_ext(height, width, wrap);
				assert(g);
				game_delete(g);
				g = NULL;
			}
		}
	}
	return true;
}

/* ******* game_new_ext ******* */
/**
 * @brief  test creating every game between size 0 to SIZE_LIMIT_GAME + fill it with every square of list
 *
 * @return true if every game has been created well and well filled
 */
bool test_game_new_ext(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square list[] = LIST_OF_SQUARE;
	for (uint height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (uint width = 1; width <= SIZE_LIMIT_GAME; width++) {
			square tab[height * width];
			for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
				for (uint index_tab = 0; index_tab < height * width; index_tab++) {
					tab[index_tab] = list[index_list];
				}
				for (int wrap = 0; wrap < 2; wrap++) {
					printf("%d\n", index_list);
					game g = game_new_ext(height, width, tab, wrap);
					assert(g);
					for (uint i = 0; i < game_nb_rows(g); i++) {
						for (uint j = 0; j < game_nb_cols(g); j++) {
							printf("list :%d, get square :%d\n", list[index_list], game_get_square(g, i, j));
							assert(list[index_list] == game_get_square(g, i, j));
						}
					}
					game_delete(g);
					g = NULL;
				}
			}
		}
	}
	return true;
}

/* ******* game_is_over ******* */
/**
 * @brief test if game_is_over return true with default_solution, false with one flag F_ERROR, false with one S_BLANK and false with game_default
 *
 * @return true if there is no errors
 */
bool test_game_is_over(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_default_solution();
	game_update_flags(g);
	assert(game_is_over(g));
	game_set_square(g, 0, 0, (S_LIGHTBULB | F_LIGHTED | F_ERROR));
	assert(!game_is_over(g));
	game_update_flags(g);
	game_set_square(g, 0, 1, S_BLANK);
	assert(!game_is_over(g));
	game_delete(g);
	g = game_default();
	game_update_flags(g);
	assert(!game_is_over(g));
	game_delete(g);
	return true;
}

/* ******* game_restart ******* */
/**
 * @brief test if game_restart remove every S_LIGHTBULB, S_MARK, and F_LIGHTED but nothing else and if the history has been reset
 *
 * @return true if there is no error
 */
bool test_game_restart(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game game_def = game_default();
	game game_def_solution = game_default_solution();
	game_set_square(game_def, 1, 5, S_BLACK4);
	game_set_square(game_def_solution, 1, 5, S_BLACK4);
	game_set_square(game_def, 3, 2, S_BLACK3);
	game_set_square(game_def_solution, 3, 2, S_BLACK3);
	game_play_move(game_def, 0, 1, S_LIGHTBULB);
	game_play_move(game_def_solution, 0, 1, S_LIGHTBULB);
	game_play_move(game_def, 2, 0, S_MARK);
	game_play_move(game_def_solution, 2, 0, S_MARK);
	game_play_move(game_def, 1, 1, S_LIGHTBULB);
	game_play_move(game_def_solution, 1, 1, S_LIGHTBULB);
	game_play_move(game_def, 1, 1, S_MARK);
	game_play_move(game_def_solution, 1, 1, S_MARK);
	game_undo(game_def);
	game_undo(game_def_solution);
	assert(!game_equal(game_def, game_def_solution));
	game_restart(game_def_solution);
	game_restart(game_def);
	assert(game_equal(game_def, game_def_solution));
	game_redo(game_def);
	assert(game_equal(game_def, game_def_solution));
	game_undo(game_def);
	assert(game_equal(game_def, game_def_solution));
	game_delete(game_def);
	game_delete(game_def_solution);
	return true;
}

/* ********* game_new ********* */
/**
 * @brief test if created game with game_new are well filled by the tab
 *
 * @return true if there is no error
 */
bool test_game_new(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	square tab[DEFAULT_SIZE * DEFAULT_SIZE];
	square list[] = LIST_OF_SQUARE;
	for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
		for (uint index_tab = 0; index_tab < DEFAULT_SIZE * DEFAULT_SIZE; index_tab++) {
			tab[index_tab] = list[index_list];
		}
		printf("%d\n", index_list);
		game g = game_new(tab);
		for (uint i = 0; i < DEFAULT_SIZE; i++) {
			for (uint j = 0; j < DEFAULT_SIZE; j++) {
				printf("list :%d, get square :%d\n", list[index_list], game_get_square(g, i, j));
				assert(list[index_list] == game_get_square(g, i, j));
			}
		}
		assert(g);
		game_delete(g);
		g = NULL;
	}
	return true;
}

/* ******** game_print ******** */
/**
 * @brief test if there is no major problems by printing every square of list
 *
 * @return true if there is no error
 */
bool test_game_print(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	for (uint height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (uint width = 1; width <= SIZE_LIMIT_GAME; width++) {
			square tab[width * height];
			square list[] = LIST_OF_SQUARE;
			game g;
			for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
				for (uint index_tab = 0; index_tab < width * height; index_tab++) {
					tab[index_tab] = list[index_list];
				}
				printf("%d\n", list[index_list]);
				g = game_new_ext(height, width, tab, false);
				assert(g);
				game_print(g);
				game_delete(g);
			}
		}
	}
	return true;
}

/* ******* game_get_square ******* */
/**
 * @brief test if get_square return the same as tab, used to create the game
 *
 * @return true if there is no error
 */
bool test_game_get_square(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	for (uint height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (uint width = 1; width <= SIZE_LIMIT_GAME; width++) {
			square tab[width * height];
			square list[] = LIST_OF_SQUARE;
			for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
				for (uint index_tab = 0; index_tab < height * width; index_tab++) {
					tab[index_tab] = list[index_list];
					game g = game_new_ext(height, width, tab, false);
					printf("[%d, %d] %d %d\n", index_tab / game_nb_cols(g), index_tab % game_nb_cols(g),
					       game_get_square(g, index_tab / game_nb_cols(g), index_tab % game_nb_cols(g)), tab[index_tab]);
					assert(game_get_square(g, index_tab / game_nb_cols(g), index_tab % game_nb_cols(g)) == tab[index_tab]);
					game_delete(g);
				}
			}
		}
	}
	return true;
}

/* ******* game_default ******* */
/**
 * @brief test if the game_default is the same as tab (tab is game_default)
 *
 * @return true if there is no error
 */
bool test_game_default(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game game_def = game_default();
	assert(game_def);
	// clang-format off
	square tab[49] = 
	{
				/*X0       X1       X2       X3       X4        X5      X6*/
		/*Y 0*/ S_BLANK, S_BLANK, S_BLACK1, S_BLANK, S_BLANK, S_BLANK, S_BLANK,
		/*Y 1*/ S_BLANK, S_BLANK, S_BLACK2, S_BLANK, S_BLANK, S_BLANK, S_BLANK,
		/*Y 2*/ S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLACKU, S_BLACK2,
		/*Y 3*/ S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLANK,
		/*Y 4*/ S_BLACK1, S_BLACKU, S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLANK,
		/*Y 5*/ S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLACK2, S_BLANK, S_BLANK,
		/*Y 6*/ S_BLANK, S_BLANK, S_BLANK, S_BLANK, S_BLACKU, S_BLANK, S_BLANK
	};
	// clang-format on
	game game_test = game_new(tab);
	assert(game_equal(game_def, game_test));
	game_delete(game_def);
	game_delete(game_test);
	return true;
}

/* *** game_default_solution *** */
/**
 * @brief test if game_default_solution is the same as game_default with moves needed to win
 *
 * @return true if there is no error
 */
bool test_game_default_solution(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game game_def_solution = game_default_solution();
	game game_def = game_default();
	assert(game_def_solution);
	// solution
	game_set_square(game_def, 0, 0, S_LIGHTBULB);
	game_set_square(game_def, 1, 1, S_LIGHTBULB);
	game_set_square(game_def, 0, 3, S_LIGHTBULB);
	game_set_square(game_def, 1, 6, S_LIGHTBULB);
	game_set_square(game_def, 2, 2, S_LIGHTBULB);
	game_set_square(game_def, 3, 6, S_LIGHTBULB);
	game_set_square(game_def, 4, 4, S_LIGHTBULB);
	game_set_square(game_def, 5, 0, S_LIGHTBULB);
	game_set_square(game_def, 5, 5, S_LIGHTBULB);
	game_set_square(game_def, 6, 1, S_LIGHTBULB);
	game_update_flags(game_def);
	assert(game_equal(game_def_solution, game_def));
	game_delete(game_def_solution);
	game_delete(game_def);
	return true;
}

/* **** game_update_flags **** */
/**
 * @brief count the number of F_ERROR on the entire game g
 *
 * @param g the game to analyze
 * @return uint the number of F_ERROR on the game g
 */
uint counterF_ERROR(game g) {
	uint cpt = 0;
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_has_error(g, i, j))
				cpt++;
		}
	}
	return cpt;
}
/*
Tests without the wrapping option

test on a 2x2 game without *wrapping*
test on a 3x3 game without *wrapping*
test on a 5x3 game without *wrapping*
test flags *error* on specifics cases

On the 2x2, 3x3 and 5x3 tests, we play these move to win and we test if the grid is correct

   01              012              012
   --              ---              ---
0 |2*|          0 |.w0|          0 |*..|
1 |*.|          1 |*1w|          1 |w*1|
   --           2 |..*|          2 |*2.|
  (2x2)            ---           3 |..*|
                  (3x3)          4 |.*.|
                                    ---
                                   (5x3)

Tests of the wrapping option

test on a 2x2 game with option *wrapping* (cas limite)
test on a 3x3 game with option *wrapping*
test on a 5x3 game with option *wrapping*
test of the flags *error* in the specifics cases of the option *wrapping*

On the 2x2, 3x3 and 5x3 tests, we play these move to win and we test if the grid is correct

   01              012              012
   --              ---              ---
0 |4*|          0 |*w2|          0 |...|
1 |*.|          1 |.ww|          1 |w*1|
   --           2 |..*|          2 |*2.|
  (2x2)            ---           3 |..*|
                  (3x3)          4 |...|
                                    ---
                                   (5x3)
*/
bool test_game_update_flags(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	// with wrapping
	game g2x2w = game_new_ext(2, 2, ext_2x2w_squares, true);
	game_play_move(g2x2w, 0, 1, S_LIGHTBULB);
	game_play_move(g2x2w, 1, 0, S_LIGHTBULB);
	assert(game_is_over(g2x2w));
	game g3x3w = game_new_ext(3, 3, ext_3x3w_squares, true);
	game_play_move(g3x3w, 0, 0, S_LIGHTBULB);
	game_play_move(g3x3w, 2, 2, S_LIGHTBULB);
	assert(game_is_over(g3x3w));
	game_delete(g3x3w);
	game g5x3w = game_new_ext(5, 3, ext_5x3w_squares, true);
	game_play_move(g5x3w, 1, 1, S_LIGHTBULB);
	game_play_move(g5x3w, 2, 0, S_LIGHTBULB);
	game_play_move(g5x3w, 3, 2, S_LIGHTBULB);
	assert(game_is_over(g5x3w));
	game_restart(g2x2w);
	game_restart(g5x3w);
	game_play_move(g2x2w, 0, 1, S_MARK);
	assert(!game_is_over(g2x2w));
	assert(game_has_error(g2x2w, 0, 0));
	assert(counterF_ERROR(g2x2w) == 1);
	game_delete(g2x2w);
	game_play_move(g5x3w, 0, 2, S_LIGHTBULB);
	game_play_move(g5x3w, 2, 2, S_LIGHTBULB);
	game_play_move(g5x3w, 1, 1, S_LIGHTBULB);
	game_play_move(g5x3w, 2, 0, S_LIGHTBULB);
	assert(!game_is_over(g5x3w));
	assert(game_has_error(g5x3w, 0, 2));
	assert(game_has_error(g5x3w, 1, 2));
	assert(game_has_error(g5x3w, 2, 2));
	assert(game_has_error(g5x3w, 2, 0));
	assert(game_has_error(g5x3w, 2, 1));
	assert(counterF_ERROR(g5x3w) == 5);
	game_delete(g5x3w);
	// without wrapping
	game g4x4 = game_new_ext(4, 4, ext_4x4_squares, false);
	game_play_move(g4x4, 0, 0, S_LIGHTBULB);
	game_play_move(g4x4, 1, 3, S_LIGHTBULB);
	game_play_move(g4x4, 2, 1, S_LIGHTBULB);
	game_play_move(g4x4, 3, 2, S_LIGHTBULB);
	assert(game_is_over(g4x4));
	game g3x10 = game_new_ext(3, 10, ext_3x10_squares, false);
	game_play_move(g3x10, 0, 0, S_LIGHTBULB);
	game_play_move(g3x10, 1, 3, S_LIGHTBULB);
	game_play_move(g3x10, 2, 4, S_LIGHTBULB);
	game_play_move(g3x10, 0, 8, S_LIGHTBULB);
	game_play_move(g3x10, 2, 9, S_LIGHTBULB);
	assert(game_is_over(g3x10));
	game_delete(g3x10);
	game g5x1 = game_new_ext(5, 1, ext_5x1_squares, false);
	game_play_move(g5x1, 0, 0, S_LIGHTBULB);
	game_play_move(g5x1, 4, 0, S_LIGHTBULB);
	assert(game_is_over(g5x1));
	game_restart(g4x4);
	game_restart(g5x1);
	game_play_move(g4x4, 2, 1, S_MARK);
	game_play_move(g4x4, 3, 2, S_MARK);
	game_play_move(g4x4, 2, 3, S_MARK);
	assert(!game_is_over(g4x4));
	assert(game_has_error(g4x4, 2, 2));
	assert(counterF_ERROR(g4x4) == 1);
	game_play_move(g4x4, 0, 1, S_LIGHTBULB);
	assert(!game_is_over(g4x4));
	assert(game_has_error(g4x4, 2, 2));
	assert(game_has_error(g4x4, 0, 2));
	assert(counterF_ERROR(g4x4) == 2);
	game_delete(g4x4);
	game_play_move(g5x1, 0, 0, S_LIGHTBULB);
	game_play_move(g5x1, 1, 0, S_LIGHTBULB);
	game_play_move(g5x1, 4, 0, S_LIGHTBULB);
	assert(!game_is_over(g5x1));
	assert(game_has_error(g5x1, 0, 0));
	assert(game_has_error(g5x1, 1, 0));
	assert(game_has_error(g5x1, 2, 0));
	assert(counterF_ERROR(g5x1) == 3);
	game_delete(g5x1);
	return true;
}

/* ****** game_play_move ****** */
/**
 * @brief test if play move set square + update the flags + update history
 *
 * @return true if there is no error
 */
bool test_game_play_move(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game game_play = game_new_empty();
	game game_set = game_new_empty();
	square list[] = LIST_OF_SQUARE;
	for (uint index_list = 0; index_list <= USABLE_SQUARE; index_list++) {
		game_play_move(game_play, 0, 0, list[index_list]);
		game_set_square(game_set, 0, 0, list[index_list]);
		game_update_flags(game_set);
		assert(game_equal(game_play, game_set));
	}
	// undo / redo
	game_undo(game_play);
	game_set_square(game_set, 0, 0, list[USABLE_SQUARE - 1]);  // take the before last usable element of list
	game_update_flags(game_set);
	assert(game_equal(game_play, game_set));
	// delete
	game_delete(game_play);
	game_delete(game_set);
	return true;
}

/* ***** game_check_move ***** */
/**
 * @brief  test if we can place usable square (here : S_BLANK, S_LIGHTBULB, S_MARK) and nothing else + if we can place something over the limits
 *
 * @return true if there is no error
 */
bool test_game_check_move(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_new_empty();
	// put usable square at the start of the list
	square list[] = LIST_OF_SQUARE;
	// test limits
	for (uint i = 0; i < SIZE_LIMIT_GAME + 10; i++) {
		for (uint j = 0; j < SIZE_LIMIT_GAME + 10; j++) {
			for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
				if (i >= game_nb_rows(g) || j >= game_nb_cols(g) || index_list > USABLE_SQUARE)
					assert(!game_check_move(g, i, j, list[index_list]));
				else
					assert(game_check_move(g, i, j, list[index_list]));
			}
		}
	}
	// test if we can place something on a wall
	for (uint i = 0; i < S_BLACKU - S_BLACK0; i++) {
		game_set_square(g, 0, 0, S_BLACK + i);
		for (uint index_list = 0; index_list <= USABLE_SQUARE; index_list++) {
			assert(!game_check_move(g, 0, 0, list[index_list]));
		}
	}
	game_delete(g);
	return true;
}

/* ************** game_set_square ************** */
bool test_game_set_square(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_get_state(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_get_flags(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_is_marked(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_is_lighted(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_has_error(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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
bool test_game_nb_rows(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_new_empty();
	if (game_get_square(g, game_nb_rows(g) - 1, 0) == S_BLANK) {
		game_delete(g);
		return true;
	}
	game_delete(g);
	return false;
}

/* ************** game_nb_cols ************** */
bool test_game_nb_cols(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game g = game_new_empty();
	if (game_get_square(g, 0, game_nb_cols(g) - 1) == S_BLANK) {
		game_delete(g);
		return true;
	}
	game_delete(g);
	return false;
}

/* ************** game_is_wrapping ************** */
bool test_game_is_wrapping(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
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

/* ***** game_save ***** */
bool test_game_save(bool whoami, char** name) {
	// if whoami is true return in name (second parms) the name of the function
	if (whoami) {
		*name = (char*)__func__;
		return false;
	}
	game gdefault = game_default();
	game_save(gdefault, "gdefault.txt");
	// try to open the created save file
	FILE* f_gdefault = fopen("gdefault.txt", "r");
	assert(f_gdefault);
	// explore default_str and test if each char of the saved file == char of default_str
	for (uint index_default_str = 0;
	     index_default_str < game_nb_cols(gdefault) * game_nb_rows(gdefault) + NB_CHAR_SAVED_FILE_FIRST_LINE + game_nb_rows(gdefault); index_default_str++) {
		int char_to_test = fgetc(f_gdefault);
		assert(char_to_test != -1);
		assert((char)char_to_test == default_str[index_default_str]);
	}
	game_delete(gdefault);
	fclose(f_gdefault);
	// do the same as gdefault but with ext_5x3w_squares
	game g5x3w = game_new_ext(5, 3, ext_5x3w_squares, true);
	game_save(g5x3w, "g5x3w.txt");
	FILE* file_g5x3w = fopen("g5x3w.txt", "r");
	assert(file_g5x3w);
	for (uint index_ext_5x3w_str = 0; index_ext_5x3w_str < game_nb_cols(g5x3w) * game_nb_rows(g5x3w) + NB_CHAR_SAVED_FILE_FIRST_LINE + game_nb_rows(g5x3w);
	     index_ext_5x3w_str++) {
		int char_to_test = fgetc(file_g5x3w);
		assert(char_to_test != -1);
		assert((char)char_to_test == ext_5x3w_str[index_ext_5x3w_str]);
	}
	game_delete(g5x3w);
	fclose(file_g5x3w);
	// do the same as gdefault but with other_squares
	game gother = game_new(other_squares);
	game_save(gother, "gother.txt");
	FILE* file_gother = fopen("gother.txt", "r");
	assert(file_gother);
	for (uint index_other_str = 0; index_other_str < game_nb_cols(gother) * game_nb_rows(gother) + NB_CHAR_SAVED_FILE_FIRST_LINE + game_nb_rows(gother);
	     index_other_str++) {
		int char_to_test = fgetc(file_gother);
		assert(char_to_test != -1);
		assert((char)char_to_test == other_str[index_other_str]);
	}
	game_delete(gother);
	fclose(file_gother);
	return true;
}

/* ********** USAGE ********** */

int usage(char* argv[]) {
	fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
	exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char* argv[]) {
	// function type
	typedef bool (*func_t)(bool, char**);
	// array of all tests functions
	func_t tab_fct[] = {&test_equal,
	                    &test_equal_ext,
	                    &test_game_check_move,
	                    &test_game_copy,
	                    &test_game_default,
	                    &test_game_default_solution,
	                    &test_game_delete,
	                    &test_game_get_black_number,
	                    &test_game_get_flags,
	                    &test_game_get_square,
	                    &test_game_get_state,
	                    &test_game_has_error,
	                    &test_game_is_black,
	                    &test_game_is_blank,
	                    &test_game_is_lightbulb,
	                    &test_game_is_lighted,
	                    &test_game_is_marked,
	                    &test_game_is_over,
	                    &test_game_is_wrapping,
	                    &test_game_nb_cols,
	                    &test_game_nb_rows,
	                    &test_game_new,
	                    &test_game_new_empty,
	                    &test_game_new_empty_ext,
	                    &test_game_new_ext,
	                    &test_game_play_move,
	                    &test_game_print,
	                    &test_game_redo,
	                    &test_game_restart,
	                    &test_game_save,
	                    &test_game_set_square,
	                    &test_game_undo,
	                    &test_game_update_flags};
	// array of the tests functions's name
	char* tab_fct_name[sizeof(tab_fct) / sizeof(tab_fct[0])];
	// for each function on tab_fct call the function to fill tab_fct_name
	for (uint index_tab_fct = 0; index_tab_fct < sizeof(tab_fct) / sizeof(tab_fct[0]); index_tab_fct++) {
		(*(tab_fct[index_tab_fct]))(true, &tab_fct_name[index_tab_fct]);
		tab_fct_name[index_tab_fct] = tab_fct_name[index_tab_fct] + 5;  // remove the test_ from the functions's name
	}
	if (argc != 2) {
		usage(argv);
	}
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool test_success = false;
	bool test_found = false;
	for (uint index_tab_fct = 0; index_tab_fct < sizeof(tab_fct) / sizeof(tab_fct[0]); index_tab_fct++) {
		if (strcmp(argv[1], tab_fct_name[index_tab_fct]) == 0) {
			test_success = tab_fct[index_tab_fct](false, NULL);
			test_found = true;
		}
	}
	if (!test_found) {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	// print test result
	if (test_success) {
		fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
		return EXIT_FAILURE;
	}
}