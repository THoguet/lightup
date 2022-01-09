#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

typedef unsigned int uint;

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
			game g;
			for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
				g = NULL;
				for (uint j = 0; j < h * w; j++) {
					tab[j] = list[i];
				}
				for (int wrap = 0; wrap < 2; wrap++) {
					printf("%d\n", i);
					g = game_new_ext(h, w, tab, wrap);
					assert(g);
					for (uint i2 = 0; i2 < g->height; i2++) {
						for (uint j = 0; j < g->width; j++) {
							printf("list :%d, get square :%d\n", list[i], game_get_square(g, i2, j));
							assert(list[i] == game_get_square(g, i2, j));
						}
					}
					game_delete(g);
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
	game_set_square(g, 0, 1, S_LIGHTBULB);
	game_set_square(g1, 0, 1, S_LIGHTBULB);
	game_play_move(g, 2, 0, S_MARK);
	game_play_move(g1, 2, 0, S_MARK);
	assert(!game_equal(g, g1));
	game_restart(g1);
	game_restart(g);
	assert(game_equal(g, g1));
	assert(g1->hist->next == NULL && g->hist->prev == NULL && g1->hist->state == F_ERROR);
	assert(g->hist->next == NULL && g->hist->prev == NULL && g->hist->state == F_ERROR);
	game_delete(g);
	game_delete(g1);
	return EXIT_SUCCESS;
}

/* ********* game_new ********* */

bool test_game_new(void) {
	square tab[DEFAULT_SIZE * DEFAULT_SIZE];
	game g;
	square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR};
	for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
		g = NULL;
		for (uint j = 0; j < DEFAULT_SIZE * DEFAULT_SIZE; j++) {
			tab[j] = list[i];
		}
		printf("%d\n", i);
		g = game_new(tab);
		for (uint i2 = 0; i2 < DEFAULT_SIZE; i2++) {
			for (uint j = 0; j < DEFAULT_SIZE; j++) {
				printf("list :%d, get square :%d\n", list[i], game_get_square(g, i2, j));
				assert(list[i] == game_get_square(g, i2, j));
			}
		}
		assert(g);
		game_delete(g);
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
	game g;
	for (int h = 1; h <= 10; h++) {
		for (int w = 1; w <= 10; w++) {
			square tab[w * h];
			square list[] = {S_BLANK, S_LIGHTBULB, S_MARK, S_BLACK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, F_ERROR};
			for (uint i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
				for (uint j = 0; j < h * w; j++) {
					tab[j] = list[i];
					g = game_new_ext(h, w, tab, false);
					printf("[%d, %d] %d %d\n", j / g->width, j % g->width, game_get_square(g, j / g->width, j % g->width), tab[j]);
					assert(game_get_square(g, j / g->width, j % g->width) == tab[j]);
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
	// a verif aps sur
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

int checklightbulb(game g, uint i, uint j, bool wall) {
	square flags = game_get_flags(g, i, j);
	// square state = game_get_state(g,i,j);
	// test if the current case is a lightbulb and if it has been well updated
	// if true we can break the loop because every others cases wont be lighted
	// by the bulb we are testing
	if (game_is_lightbulb(g, i, j)) {
		if (/*test if this lightbulb is well updated*/ flags == S_BLANK || flags == F_ERROR) {
			// printf("156\n");
			return 0;
		}
		return -1;
	}
	// if no wall(s) before test if the light of the lightbulb has been well
	// updated
	if (!wall) {
		// test if the current case is not lighted
		if (flags != F_LIGHTED) {
			// printf("170\n");
			return 0;
		}

	} else {
		if (/*test if the current case is lighted*/ flags == F_LIGHTED) {
			int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
			for (uint y = 0; y < 7 /*tab size*/; y = y + 2) {
				for (int x = 1; x < max(g->width, g->height); x++) {
					if (/* test if both tab are not 0*/ tab[y] != tab[y + 1] &&
					    (/* normal check */ (j + x * tab[y + 1] >= 0 && j + x * tab[y + 1] < g->width && i + x * tab[y] >= 0 && i + x * tab[y] < g->height) ||
					     /* wrapping check*/ g->wrapping)) {
						flags = game_get_square(g, ((i + g->height + x * tab[y]) % g->height), ((j + g->width + x * tab[y + 1]) % g->width));
						if (flags == (S_LIGHTBULB | F_LIGHTED) || flags == (S_LIGHTBULB | F_LIGHTED | F_ERROR)) {
							return -1;
						}
						if (flags != F_LIGHTED && flags != (F_LIGHTED | S_MARK)) {
							break;
						}
					}
				}
			}
			return 0;
		}
	}
	return 1;
}

bool check_update(game g) {
	for (uint i = 0; i < g->height; i++) {
		for (uint j = 0; j < g->width; j++) {
			if (game_is_lightbulb(g, i, j)) {
				square flags = game_get_flags(g, i, j);
				if (/*test if this lightbulb is well updated*/ flags == S_BLANK || flags == F_ERROR) {
					// printf("196\n");
					return false;
				}
				int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
				for (uint y = 0; y < 7 /*tab size*/; y = y + 2) {
					bool wall = false;
					for (int x = 1; x < max(g->width, g->height); x++) {
						if (/* test if both tab are not 0*/ tab[y] != tab[y + 1] &&
						    (/* normal check */ (j + x * tab[y + 1] >= 0 && j + x * tab[y + 1] < g->width && i + x * tab[y] >= 0 &&
						                         i + x * tab[y] < g->height) ||
						     /* wrapping check*/ g->wrapping)) {
							if (/*test if current case is a wall*/ game_is_black(g, ((i + g->height + x * tab[y]) % g->height),
							                                                     ((j + g->width + x * tab[y + 1]) % g->width))) {
								wall = true;
							} else {
								int tmp = checklightbulb(g, ((i + g->height + x * tab[y]) % g->height), ((j + g->width + x * tab[y + 1]) % g->width), wall);
								if (tmp == -1) {
									break;
								}
								if (!tmp) {
									return tmp;
								}
							}
						}
					}
				}
			} else if (/*test if current case is a wall*/ game_is_black(g, i, j)) {
				if (game_get_black_number(g, i, j) != -1) {
					// lookup for lightbulb and emptycells
					int lb = 0;
					int notempty = 0;
					int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
					bool res = true;
					for (uint y = 0; y < 7; y = y + 2) {
						if (/* normal check*/ (j + tab[y + 1] >= 0 && j + tab[y + 1] < g->width && i + tab[y] >= 0 && i + tab[y] < g->height) ||
						    /*wrapping check*/ g->wrapping) {
							if (game_is_lightbulb(g, (i + g->height + tab[y]) % g->height, (j + g->width + tab[y + 1]) % g->width))
								lb++;
							else if (!game_is_blank(g, (i + g->height + tab[y]) % g->height, (j + g->width + tab[y + 1]) % g->width) ||
							         game_is_lighted(g, (i + g->height + tab[y]) % g->height, (j + g->width + tab[y + 1]) % g->width))
								notempty++;
						} else {
							notempty++;
						}
					}
					if (lb > game_get_black_number(g, i, j)) {
						res = false;
					}
					// look if there is enough empty cell around the wall
					else if (notempty > abs(game_get_black_number(g, i, j) - 4)) {
						res = false;
					}
					if (game_get_flags(g, i, j) == F_ERROR) {
						res = !res;
					}
					if (!res) {
						// printf("386\n");
						// printf("lb:%d notempty:%d i:%d j:%d state : %d,
						// flags:
						// %d\n",lb,notempty,i,j,game_get_state(g,i,j),game_get_flags(g,i,j));
						return res;
					}
				}
			}
		}
	}
	return true;
}

// a > 2 = temps d'exec > 3 min et expo
bool brutforce(game g, int a, bool* deldup) {
	// char c;
	bool* delDupNext = (bool*)calloc(g->height * g->width, sizeof(bool));
	if (delDupNext == NULL) {
		fprintf(stderr, "Not enought memory\n");
		exit(EXIT_FAILURE);
	}
	square list[] = {S_LIGHTBULB, S_MARK, S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU, S_BLANK};
	for (uint i = 0; i < g->height; i++) {
		for (uint j = 0; j < g->width; j++) {
			// printf("state: %d i:%d j:%d\n",game_get_state(g,i,j),i,j);
			if (deldup == NULL || !(deldup[i * g->width + j])) {
				if (game_get_state(g, i, j) == S_BLANK) {
					if (delDupNext != NULL && a > 0)
						delDupNext[i * g->width + j] = true;
					for (uint k = 0; k < 9; k++) {
						game_set_square(g, i, j, list[k]);
						// if (k == 0){
						// 	game_print(g);
						// 	scanf("%c",&c);
						// }
						if (a > 0) {
							if (!brutforce(g, a - 1, delDupNext)) {
								// printf("341\n");
								free(delDupNext);
								return false;
							}
						}
						game_update_flags(g);
						if (!check_update(g)) {
							game_print(g);
							// printf("i:%d j:%d state : %d, flags:
							// %d\n",i,j,game_get_state(g,i,j),game_get_flags(g,i,j));
							free(delDupNext);
							return false;
						}
					}
				}
			}
		}
	}
	free(delDupNext);
	return true;
}

void randomise(game g) {
	time_t t;
	srand((unsigned)time(&t));
	assert(g);
	square list[] = {S_LIGHTBULB, S_MARK,  S_BLACK0, S_BLACK1, S_BLACK2, S_BLACK3, S_BLACK4, S_BLACKU,
	                 S_BLANK,     S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK};
	for (uint h = 0; h < g->height; h++) {
		for (uint w = 0; w < g->width; w++) {
			game_set_square(g, h, w, list[rand() % 16]);
		}
	}
}

bool test_game_update_flags(void) {
	for (uint h = 1; h <= 6; h++) {
		for (uint w = 1; w <= 6; w++) {
			for (int wrap = 0; wrap < 2; wrap++) {
				game g = game_new_empty_ext(h, w, wrap);
				assert(g);
				randomise(g);
				if (!brutforce(g, 1, NULL)) {
					game_delete(g);
					return EXIT_FAILURE;
				}
				game_delete(g);
			}
		}
	}
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
	assert(!game_check_move(g, 10, 10, S_BLANK));
	assert(!game_check_move(g, 10, 10, S_BLACK0));
	assert(!game_check_move(g, 0, 0, S_BLACK0));
	assert(!game_check_move(g, 0, 0, S_BLACK1));
	assert(!game_check_move(g, 0, 0, S_BLACK2));
	assert(!game_check_move(g, 0, 0, S_BLACK3));
	assert(!game_check_move(g, 0, 0, S_BLACK4));
	assert(!game_check_move(g, 0, 0, S_BLACKU));
	assert(!game_check_move(g, 0, 0, F_LIGHTED));
	assert(!game_check_move(g, 0, 0, F_ERROR));
	for (uint i = 0; i < S_BLACKU - S_BLACK0; i++) {
		game_set_square(g, 0, 0, S_BLACK0 + i);
		assert(!game_check_move(g, 0, 0, S_BLANK));
		assert(!game_check_move(g, 0, 0, S_MARK));
		assert(!game_check_move(g, 0, 0, S_LIGHTBULB));
	}
	assert(game_check_move(g, 1, 0, S_BLANK));
	assert(game_check_move(g, 1, 0, S_MARK));
	assert(game_check_move(g, 1, 0, S_LIGHTBULB));
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