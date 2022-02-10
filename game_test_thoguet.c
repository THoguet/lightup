#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "game_aux.h"
#include "game_examples.h"
#include "game_ext.h"
#include "game_tools.h"

#define SIZE_LIMIT_GAME 10
// put usable square at the start of the list and S_BLANK at the first element
#define LIST_OF_SQUARE \
	{ S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_LIGHTED, F_ERROR }
#define USABLE_SQUARE 2

/* ********** DUMMY ********** */
bool test_dummy(void) {
	return EXIT_SUCCESS;
}

/* ******* game_new_ext_empty ******* */
/**
 * @brief test creating every game between size 0 to SIZE_LIMIT_GAME
 *
 * @return EXIT_SUCCESS if every game has been created well
 */
bool test_game_new_empty_ext(void) {
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
	return EXIT_SUCCESS;
}

/* ******* game_new_ext ******* */
/**
 * @brief  test creating every game between size 0 to SIZE_LIMIT_GAME + fill it with every square of list
 *
 * @return EXIT_SUCCESS if every game has been created well and well filled
 */
bool test_game_new_ext(void) {
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
	return EXIT_SUCCESS;
}

/* ******* game_is_over ******* */
/**
 * @brief test if game_is_over return true with default_solution, false with one flag F_ERROR, false with one S_BLANK and false with game_default
 *
 * @return EXIT_SUCCESS if there is no errors
 */
bool test_game_is_over(void) {
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
	return EXIT_SUCCESS;
}

/* ******* game_restart ******* */
/**
 * @brief test if game_restart remove every S_LIGHTBULB, S_MARK, and F_LIGHTED but nothing else and if the history has been reset
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_restart(void) {
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
	return EXIT_SUCCESS;
}

/* ********* game_new ********* */
/**
 * @brief test if created game with game_new are well filled by the tab
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_new(void) {
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
	return EXIT_SUCCESS;
}

/* ******** game_print ******** */
/**
 * @brief test if there is no major problems by printing every square of list
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_print(void) {
	for (int height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (int width = 1; width <= SIZE_LIMIT_GAME; width++) {
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
	return EXIT_SUCCESS;
}

/* ******* game_get_square ******* */
/**
 * @brief test if get_square return the same as tab, used to create the game
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_get_square(void) {
	for (int height = 1; height <= SIZE_LIMIT_GAME; height++) {
		for (int width = 1; width <= SIZE_LIMIT_GAME; width++) {
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
	return EXIT_SUCCESS;
}

/* ******* game_default ******* */
/**
 * @brief test if the game_default is the same as tab (tab is game_default)
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_default(void) {
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
	return EXIT_SUCCESS;
}

/* *** game_default_solution *** */
/**
 * @brief test if game_default_solution is the same as game_default with moves needed to win
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_default_solution(void) {
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
	return EXIT_SUCCESS;
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
bool test_game_update_flags(void) {
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
	game g2x2 = game_new_ext(2, 2, ext_2x2w_squares, false);
	game_play_move(g2x2, 0, 1, S_LIGHTBULB);
	game_play_move(g2x2, 1, 0, S_LIGHTBULB);
	assert(game_is_over(g2x2));
	game g3x3 = game_new_ext(3, 3, ext_3x3w_squares, false);
	game_play_move(g3x3, 1, 0, S_LIGHTBULB);
	game_play_move(g3x3, 2, 2, S_LIGHTBULB);
	assert(game_is_over(g3x3));
	game_delete(g3x3);
	game g5x3 = game_new_ext(5, 3, ext_5x3w_squares, false);
	game_play_move(g5x3, 0, 0, S_LIGHTBULB);
	game_play_move(g5x3, 1, 1, S_LIGHTBULB);
	game_play_move(g5x3, 2, 0, S_LIGHTBULB);
	game_play_move(g5x3, 3, 2, S_LIGHTBULB);
	game_play_move(g5x3, 4, 1, S_LIGHTBULB);
	assert(game_is_over(g5x3));
	game_restart(g2x2);
	game_restart(g5x3);
	game_play_move(g2x2, 1, 0, S_MARK);
	assert(!game_is_over(g2x2));
	assert(game_has_error(g2x2, 0, 0));
	assert(counterF_ERROR(g2x2) == 1);
	game_delete(g2x2);
	game_play_move(g5x3, 0, 2, S_LIGHTBULB);
	game_play_move(g5x3, 1, 1, S_LIGHTBULB);
	game_play_move(g5x3, 2, 0, S_LIGHTBULB);
	game_play_move(g5x3, 2, 2, S_LIGHTBULB);
	game_play_move(g5x3, 3, 2, S_LIGHTBULB);
	game_play_move(g5x3, 4, 1, S_LIGHTBULB);
	assert(!game_is_over(g5x3));
	assert(game_has_error(g5x3, 1, 2));
	assert(game_has_error(g5x3, 2, 1));
	assert(game_has_error(g5x3, 2, 2));
	assert(game_has_error(g5x3, 3, 2));
	assert(counterF_ERROR(g5x3) == 4);
	game_delete(g5x3);
	return EXIT_SUCCESS;
}

/* ****** game_play_move ****** */
/**
 * @brief test if play move set square + update the flags + update history
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_play_move(void) {
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
	return EXIT_SUCCESS;
}

/* ***** game_check_move ***** */
/**
 * @brief  test if we can place usable square (here : S_BLANK, S_LIGHTBULB, S_MARK) and nothing else + if we can place something over the limits
 *
 * @return EXIT_SUCCESS if there is no error
 */
bool test_game_check_move(void) {
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
	return EXIT_SUCCESS;
}

/* ***** game_save ***** */
bool test_game_save(void) {
	game g4x4w = game_new_ext(4, 4, ext_4x4_squares, true);
	game_save(g4x4w, "g4x4w.txt");
	game g4x4w_from_file = game_load("g4x4w.txt");
	assert(game_equal(g4x4w, g4x4w_from_file));
	game_delete(g4x4w);
	game_delete(g4x4w_from_file);
	game g4x4 = game_new_ext(4, 4, ext_4x4_squares, false);
	game_save(g4x4, "g4x4.txt");
	game g4x4_from_file = game_load("g4x4.txt");
	assert(game_equal(g4x4, g4x4_from_file));
	game_delete(g4x4);
	game_delete(g4x4_from_file);
	game g3x10w = game_new_ext(3, 10, ext_3x10_squares, true);
	game_save(g3x10w, "g3x10w.txt");
	game g3x10w_from_file = game_load("g3x10w.txt");
	assert(game_equal(g3x10w, g3x10w_from_file));
	game_delete(g3x10w);
	game_delete(g3x10w_from_file);
	game g3x10 = game_new_ext(3, 10, ext_3x10_squares, false);
	game_save(g3x10, "g3x10.txt");
	game g3x10_from_file = game_load("g3x10.txt");
	assert(game_equal(g3x10, g3x10_from_file));
	game_delete(g3x10);
	game_delete(g3x10_from_file);
	game g5x1w = game_new_ext(5, 1, ext_5x1_squares, true);
	game_save(g5x1w, "g5x1w.txt");
	game g5x1w_from_file = game_load("g5x1w.txt");
	assert(game_equal(g5x1w, g5x1w_from_file));
	game_delete(g5x1w);
	game_delete(g5x1w_from_file);
	game g5x1 = game_new_ext(5, 1, ext_5x1_squares, false);
	game_save(g5x1, "g5x1.txt");
	game g5x1_from_file = game_load("g5x1.txt");
	assert(game_equal(g5x1, g5x1_from_file));
	game_delete(g5x1);
	game_delete(g5x1_from_file);
	game g2x2w = game_new_ext(5, 1, ext_2x2w_squares, true);
	game_save(g2x2w, "g2x2w.txt");
	game g2x2w_from_file = game_load("g2x2w.txt");
	assert(game_equal(g2x2w, g2x2w_from_file));
	game_delete(g2x2w);
	game_delete(g2x2w_from_file);
	game g2x2 = game_new_ext(5, 1, ext_2x2w_squares, false);
	game_save(g2x2, "g2x2.txt");
	game g2x2_from_file = game_load("g2x2.txt");
	assert(game_equal(g2x2, g2x2_from_file));
	game_delete(g2x2);
	game_delete(g2x2_from_file);
	game g3x3w = game_new_ext(3, 3, ext_3x3w_squares, true);
	game_save(g3x3w, "g3x3w.txt");
	game g3x3w_from_file = game_load("g3x3w.txt");
	assert(game_equal(g3x3w, g3x3w_from_file));
	game_delete(g3x3w);
	game_delete(g3x3w_from_file);
	game g3x3 = game_new_ext(3, 3, ext_3x3w_squares, false);
	game_save(g3x3, "g3x3.txt");
	game g3x3_from_file = game_load("g3x3.txt");
	assert(game_equal(g3x3, g3x3_from_file));
	game_delete(g3x3);
	game_delete(g3x3_from_file);

	return EXIT_SUCCESS;
}

/* ********** USAGE ********** */

int usage(int argc, char* argv[]) {
	fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
	exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char* argv[]) {
	if (argc == 1) {
		usage(argc, argv);
	}
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool ok = true;
	if (strcmp("dummy", argv[1]) == 0)
		ok = test_dummy();
	else if (strcmp("game_print", argv[1]) == 0)
		ok = test_game_print();
	else if (strcmp("game_default", argv[1]) == 0)
		ok = test_game_default();
	else if (strcmp("game_default_solution", argv[1]) == 0)
		ok = test_game_default_solution();
	else if (strcmp("game_new", argv[1]) == 0)
		ok = test_game_new();
	else if (strcmp("game_restart", argv[1]) == 0)
		ok = test_game_restart();
	else if (strcmp("game_get_square", argv[1]) == 0)
		ok = test_game_get_square();
	else if (strcmp("game_is_over", argv[1]) == 0)
		ok = test_game_is_over();
	else if (strcmp("game_update_flags", argv[1]) == 0)
		ok = test_game_update_flags();
	else if (strcmp("game_play_move", argv[1]) == 0)
		ok = test_game_play_move();
	else if (strcmp("game_check_move", argv[1]) == 0)
		ok = test_game_check_move();
	else if (strcmp("game_new_empty_ext", argv[1]) == 0)
		ok = test_game_new_empty_ext();
	else if (strcmp("game_new_ext", argv[1]) == 0)
		ok = test_game_new_ext();
	else {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	// print test result
	if (!ok) {
		fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
		return EXIT_FAILURE;
	}
}