#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

/* ********** DUMMY ********** */
bool test_dummy(void) {
	return EXIT_SUCCESS;
}

/* ******* game_new_ext_empty ******* */

bool test_game_new_empty_ext(void) {
	for (int h = 1; h <= 10; h++) {
		for (int w = 1; w <= 10; w++) {
			for (int wrap = 0; wrap < 2; wrap++) {
				game g = game_new_empty_ext(h, w, wrap);
				assert(g);
				game_delete(g);
			}
		}
	}
	return EXIT_SUCCESS;
}

/* ******* game_new_ext ******* */

bool test_game_new_ext(void) {
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR};
	for (int h = 1; h <= 10; h++) {
		for (int w = 1; w <= 10; w++) {
			square tab[h * w];
			for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
				for (uint j = 0; j < h * w; j++) {
					tab[j] = list[i];
				}
				for (int wrap = 0; wrap < 2; wrap++) {
					printf("%d\n", i);
					game g = game_new_ext(h, w, tab, wrap);
					assert(g);
					for (uint i2 = 0; i2 < game_nb_rows(g); i2++) {
						for (uint j = 0; j < game_nb_cols(g); j++) {
							printf("list :%d, get square :%d\n", list[i], game_get_square(g, i2, j));
							assert(list[i] == game_get_square(g, i2, j));
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

bool test_game_restart(void) {
	game g = game_default();
	game g1 = game_default_solution();
	game_set_square(g, 1, 5, S_BLACK4);
	game_set_square(g1, 1, 5, S_BLACK4);
	game_set_square(g, 3, 2, S_BLACK3);
	game_set_square(g1, 3, 2, S_BLACK3);
	game_play_move(g, 0, 1, S_LIGHTBULB);
	game_play_move(g1, 0, 1, S_LIGHTBULB);
	game_play_move(g, 2, 0, S_MARK);
	game_play_move(g1, 2, 0, S_MARK);
	game_play_move(g, 1, 1, S_LIGHTBULB);
	game_play_move(g1, 1, 1, S_LIGHTBULB);
	game_play_move(g, 1, 1, S_MARK);
	game_play_move(g1, 1, 1, S_MARK);
	game_undo(g);
	game_undo(g1);
	assert(!game_equal(g, g1));
	game_restart(g1);
	game_restart(g);
	assert(game_equal(g, g1));
	game_redo(g);
	assert(game_equal(g, g1));
	game_undo(g);
	assert(game_equal(g, g1));
	game_delete(g);
	game_delete(g1);
	return EXIT_SUCCESS;
}

/* ********* game_new ********* */

bool test_game_new(void) {
	square tab[DEFAULT_SIZE * DEFAULT_SIZE];
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR};
	for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
		for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
			tab[j] = list[i];
		}
		printf("%d\n", i);
		game g = game_new(tab);
		for (uint i2 = 0; i2 < DEFAULT_SIZE; i2++) {
			for (uint j = 0; j < DEFAULT_SIZE; j++) {
				printf("list :%d, get square :%d\n", list[i], game_get_square(g, i2, j));
				assert(list[i] == game_get_square(g, i2, j));
			}
		}
		assert(g);
		game_delete(g);
		g = NULL;
	}
	return EXIT_SUCCESS;
}

/* ******** game_print ******** */

bool test_game_print(void) {
	for (int h = 1; h <= 10; h++) {
		for (int w = 1; w <= 10; w++) {
			square tab[w * h];
			square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU};
			game g;
			for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
				for (uint j = 0; j < w * h; j++) {
					tab[j] = list[i];
				}
				printf("%d\n", i);
				g = game_new_ext(h, w, tab, false);
				assert(g);
				game_print(g);
				game_delete(g);
			}
		}
	}
	return EXIT_SUCCESS;
}

/* ******* game_get_square ******* */

bool test_game_get_square(void) {
	for (int h = 1; h <= 10; h++) {
		for (int w = 1; w <= 10; w++) {
			square tab[w * h];
			square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR};
			for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
				for (uint j = 0; j < h * w; j++) {
					tab[j] = list[i];
					game g = game_new_ext(h, w, tab, false);
					printf("[%d, %d] %d %d\n", j / game_nb_cols(g), j % game_nb_cols(g), game_get_square(g, j / game_nb_cols(g), j % game_nb_cols(g)), tab[j]);
					assert(game_get_square(g, j / game_nb_cols(g), j % game_nb_cols(g)) == tab[j]);
					game_delete(g);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

/* ******* game_default ******* */

bool test_game_default(void) {
	game g = game_default();
	assert(g);
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
	game g1 = game_new(tab);
	assert(game_equal(g, g1));
	game_delete(g);
	game_delete(g1);
	return EXIT_SUCCESS;
}

/* *** game_default_solution *** */

bool test_game_default_solution(void) {
	game g = game_default_solution();
	game g1 = game_default();
	assert(g);
	// solution
	game_set_square(g1, 0, 0, S_LIGHTBULB);
	game_set_square(g1, 1, 1, S_LIGHTBULB);
	game_set_square(g1, 0, 3, S_LIGHTBULB);
	game_set_square(g1, 1, 6, S_LIGHTBULB);
	game_set_square(g1, 2, 2, S_LIGHTBULB);
	game_set_square(g1, 3, 6, S_LIGHTBULB);
	game_set_square(g1, 4, 4, S_LIGHTBULB);
	game_set_square(g1, 5, 0, S_LIGHTBULB);
	game_set_square(g1, 5, 5, S_LIGHTBULB);
	game_set_square(g1, 6, 1, S_LIGHTBULB);
	game_update_flags(g1);
	assert(game_equal(g, g1));
	game_delete(g);
	game_delete(g1);
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
	square test2x2w[4] = {S_BLACK4, S_BLANK, S_BLANK, S_BLANK};
	square test3x3w[9] = {S_BLANK, S_BLACKU, S_BLACK2, S_BLANK, S_BLACKU, S_BLACKU, S_BLANK, S_BLANK, S_BLANK};
	square test5x3[15] = {S_BLANK, S_BLANK, S_BLANK, S_BLACKU, S_BLANK, S_BLACK1, S_BLANK, S_BLACK2,
	                      S_BLANK, S_BLANK, S_BLANK, S_BLANK,  S_BLANK, S_BLANK,  S_BLANK};
	square test2x2[4] = {S_BLACK2, S_BLANK, S_BLANK, S_BLANK};
	square test3x3[9] = {S_BLANK, S_BLACKU, S_BLACK0, S_BLANK, S_BLACK1, S_BLACKU, S_BLANK, S_BLANK, S_BLANK};
	// avec wrapping
	game g2x2w = game_new_ext(2, 2, test2x2w, true);
	game g3x3w = game_new_ext(3, 3, test3x3w, true);
	game g5x3w = game_new_ext(5, 3, test5x3, true);
	game_play_move(g2x2w, 0, 1, S_LIGHTBULB);
	game_play_move(g2x2w, 1, 0, S_LIGHTBULB);
	assert(game_is_over(g2x2w));
	game_play_move(g3x3w, 0, 0, S_LIGHTBULB);
	game_play_move(g3x3w, 2, 2, S_LIGHTBULB);
	assert(game_is_over(g3x3w));
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
	game_delete(g2x2w);
	game_delete(g3x3w);
	game_delete(g5x3w);
	// sans wrapping
	game g2x2 = game_new_ext(2, 2, test2x2, false);
	game g3x3 = game_new_ext(3, 3, test3x3, false);
	game g5x3 = game_new_ext(5, 3, test5x3, false);
	game_play_move(g2x2, 0, 1, S_LIGHTBULB);
	game_play_move(g2x2, 1, 0, S_LIGHTBULB);
	assert(game_is_over(g2x2));
	game_play_move(g3x3, 1, 0, S_LIGHTBULB);
	game_play_move(g3x3, 2, 2, S_LIGHTBULB);
	assert(game_is_over(g3x3));
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
	game_delete(g3x3);
	game_delete(g2x2);
	return EXIT_SUCCESS;
}

/* ****** game_play_move ****** */

bool test_game_play_move(void) {
	game g = game_new_empty();
	game g1 = game_new_empty();
	game_play_move(g, 0, 0, S_LIGHTBULB);
	assert(!game_equal(g, g1));
	game_set_square(g1, 0, 0, S_LIGHTBULB);
	assert(!game_equal(g, g1));
	game_update_flags(g1);
	assert(game_equal(g, g1));
	game_play_move(g, 3, 0, S_MARK);
	assert(!game_equal(g, g1));
	game_set_square(g1, 3, 0, S_MARK);
	assert(!game_equal(g, g1));
	game_update_flags(g1);
	assert(game_equal(g, g1));
	game_play_move(g, 0, 3, S_BLANK);
	assert(game_equal(g, g1));
	game_set_square(g1, 0, 3, S_BLANK);
	assert(!game_equal(g, g1));
	game_update_flags(g1);
	assert(game_equal(g, g1));
	game_delete(g);
	game_delete(g1);
	return EXIT_SUCCESS;
}

/* ***** game_check_move ***** */

bool test_game_check_move(void) {
	game g = game_new_empty();
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR, F_LIGHTED};
	// test if we can place something else than S_BLANK, S_LIGHTBULB, S_MARK + if we can place something over the limits
	for (uint i = 0; i < 20; i++) {
		for (uint j = 0; j < 20; j++) {
			for (uint index_list = 0; index_list < sizeof(list) / sizeof(list[0]); index_list++) {
				if (i >= game_nb_rows(g) || j >= game_nb_cols(g) || index_list > 2)
					assert(!game_check_move(g, i, j, list[index_list]));
				else
					assert(game_check_move(g, i, j, list[index_list]));
			}
		}
	}
	// test if we can place something on a wall
	for (uint i = 0; i < S_BLACKU - S_BLACK0; i++) {
		game_set_square(g, 0, 0, S_BLACK + i);
		assert(!game_check_move(g, 0, 0, S_BLANK));
		assert(!game_check_move(g, 0, 0, S_MARK));
		assert(!game_check_move(g, 0, 0, S_LIGHTBULB));
	}
	game_delete(g);
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