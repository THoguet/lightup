#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_ext.h"

int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

game game_new(square* squares) {
	game g = game_new_empty();
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			g->tab_cell[i][j] = squares[g->width * i + j];
		}
	}
	return g;
}

game game_new_empty(void) {
	// alloc struct
	game g = (game)malloc(sizeof(struct game_s));
	if (g == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	// set variable of struct
	g->wrapping = false;
	g->height = DEFAULT_SIZE;
	g->width = DEFAULT_SIZE;
	g->hist = history_create_empty();
	// alloc the tab with variables set before
	g->tab_cell = (square**)malloc(sizeof(square*) * g->height);
	if (g->tab_cell == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < g->height; i++) {
		g->tab_cell[i] = (square*)malloc(sizeof(square) * g->width);
		if (g->tab_cell[i] == NULL) {
			fprintf(stderr, "not enough memory\n");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < g->width; j++) {
			g->tab_cell[i][j] = S_BLANK;
		}
	}
	g->hist = history_insert_first(g->hist, F_ERROR, 0, 0);
	return g;
}
// TODO
game game_copy(cgame g1) {
	game g2 = game_new_empty_ext(g1->height, g1->width, g1->wrapping);
	for (int i = 0; i < g2->height; i++) {
		for (int j = 0; j < g2->width; j++) {
			g2->tab_cell[i][j] = g1->tab_cell[i][j];
		}
	}
	g2->hist = history_copy(g1->hist);
	return g2;
}

bool game_equal(cgame g1, cgame g2) {
	if (g1->height != g2->height || g1->width != g2->width || g1->wrapping != g2->wrapping)
		return false;
	for (int i = 0; i < DEFAULT_SIZE; i++) {
		for (int j = 0; j < DEFAULT_SIZE; j++) {
			if (g1->tab_cell[i][j] != g2->tab_cell[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void game_delete(game g) {
	for (int i = 0; i < g->height; i++) {
		free(g->tab_cell[i]);
		g->tab_cell[i] = NULL;
	}
	free(g->tab_cell);
	g->tab_cell = NULL;
	history_delete_entire_history(g->hist);
	g->hist = NULL;
	free(g);
	g = NULL;
	return;
}

void game_set_square(game g, uint i, uint j, square s) {
	g->tab_cell[i][j] = s;
	return;
}

square game_get_square(cgame g, uint i, uint j) {
	return g->tab_cell[i][j];
}

square game_get_state(cgame g, uint i, uint j) {
	square tmp = g->tab_cell[i][j];
	if (tmp >= F_ERROR) {
		tmp -= F_ERROR;
	}
	if (tmp >= F_LIGHTED) {
		tmp -= F_LIGHTED;
	}
	return tmp;
}

square game_get_flags(cgame g, uint i, uint j) {
	square temp = g->tab_cell[i][j];
	if (temp >= F_ERROR) {
		temp -= F_ERROR;
		if (temp >= F_LIGHTED) {
			return (F_ERROR | F_LIGHTED);
		}
		return (F_ERROR);
	}
	if (temp >= F_LIGHTED) {
		return (F_LIGHTED);
	}
	return (S_BLANK);
}

bool game_is_blank(cgame g, uint i, uint j) {
	return game_get_state(g, i, j) == S_BLANK;
}

bool game_is_lightbulb(cgame g, uint i, uint j) {
	return game_get_state(g, i, j) == S_LIGHTBULB;
}

bool game_is_black(cgame g, uint i, uint j) {
	return game_get_state(g, i, j) >= S_BLACK0 && game_get_state(g, i, j) <= S_BLACKU;
}

int game_get_black_number(cgame g, uint i, uint j) {
	if (game_get_state(g, i, j) == S_BLACKU)
		return -1;
	return game_get_state(g, i, j) - 8;
}

bool game_is_marked(cgame g, uint i, uint j) {
	return game_get_state(g, i, j) == S_MARK;
}

bool game_is_lighted(cgame g, uint i, uint j) {
	return game_get_flags(g, i, j) >= F_LIGHTED;
}

bool game_has_error(cgame g, uint i, uint j) {
	return g->tab_cell[i][j] >= F_ERROR;
}

bool game_check_move(cgame g, uint i, uint j, square s) {
	return !(i >= g->height || j >= g->width || (s != S_BLANK && s != S_MARK && s != S_LIGHTBULB) || game_is_black(g, i, j));
}

void game_play_move(game g, uint i, uint j, square s) {
	if (g->hist == NULL) {
		g->hist = history_create_empty();
		g->hist = history_insert_first(g->hist, game_get_state(g, i, j), i, j);
	} else {
		g->hist = history_next(history_insert_after(g->hist, g->hist, game_get_state(g, i, j), i, j));
		g->hist = history_delete_all_after(g->hist, g->hist);
	}
	game_set_square(g, i, j, s);
	game_update_flags(g);
}

void delete_flags(game g) {
	// delete prev flags
	for (uint i = 0; i < g->height; i++) {
		for (uint j = 0; j < g->width; j++) {
			game_set_square(g, i, j, game_get_state(g, i, j));
		}
	}
}

void addF_LIGHTED(game g, uint i, uint j) {
	square flags = game_get_flags(g, i, j);
	if (flags == S_BLANK)
		game_set_square(g, i, j, game_get_state(g, i, j) | F_LIGHTED);
	else if (flags == F_ERROR) {
		game_set_square(g, i, j, game_get_state(g, i, j) | F_LIGHTED | F_ERROR);
	}
}

void game_update_flags(game g) {
	delete_flags(g);
	// update flags lighted and lightbulb error
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			if (game_get_state(g, i, j) == S_LIGHTBULB) {
				// add FLIGHTED to the current lightbulb
				addF_LIGHTED(g, i, j);
				// add FLIGHTED on the 4 directions, when a wall is hit stop this direction, when another lightbulb is hit put the initial lighbulb on error
				int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
				for (int x = 1; x < max(g->width, g->height); x++) {
					for (uint y = 0; y < 7 /*tab size*/; y = y + 2) {
						if (/* test if both tab are not 0*/ tab[y] != tab[y + 1] &&
						    (/* normal check */ (j + x * tab[y + 1] >= 0 && j + x * tab[y + 1] < g->width && i + x * tab[y] >= 0 &&
						                         i + x * tab[y] < g->height) ||
						     /* wrapping check*/ g->wrapping)) {
							if (game_is_black(g, ((i + g->height + x * tab[y]) % g->height), ((j + g->width + x * tab[y + 1]) % g->width))) {
								tab[y] = 0;
								tab[y + 1] = 0;
							} else {
								// if we found another lightbulb on the line
								if (game_get_state(g, ((i + g->height + x * tab[y]) % g->height), ((j + g->width + x * tab[y + 1]) % g->width)) == S_LIGHTBULB)
									// update initial lightbulb on F_ERROR
									game_set_square(g, i, j, (S_LIGHTBULB | F_LIGHTED | F_ERROR));
								addF_LIGHTED(g, ((i + g->height + x * tab[y]) % g->height), ((j + g->width + x * tab[y + 1]) % g->width));
							}
						}
					}
				}
			}
		}
	}
	// update flags wall (need to wait all FLIGHTED being updated)
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			if (game_is_black(g, i, j) && game_get_black_number(g, i, j) >= 0) {
				int lb = 0;
				int notempty = 0;
				int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
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
					game_set_square(g, i, j, (game_get_state(g, i, j) | F_ERROR));
				}
				// look if there is enough empty cell around the wall
				else if (notempty > abs(game_get_black_number(g, i, j) - 4)) {
					game_set_square(g, i, j, (game_get_state(g, i, j) | F_ERROR));
				}
			}
		}
	}
}

bool game_is_over(cgame g) {
	for (uint i = 0; i < g->height; i++) {
		for (uint j = 0; j < g->width; j++) {
			if ((game_get_state(g, i, j) == S_BLANK && game_get_flags(g, i, j) == S_BLANK) || game_get_flags(g, i, j) == F_ERROR ||
			    game_get_flags(g, i, j) == (F_LIGHTED | F_ERROR))
				return false;
		}
	}
	return true;
}

void game_restart(game g) {
	for (uint i = 0; i < g->height; i++) {
		for (uint j = 0; j < g->width; j++) {
			if (game_get_state(g, i, j) == S_MARK || game_get_state(g, i, j) == S_LIGHTBULB)
				game_set_square(g, i, j, S_BLANK);
		}
	}
	game_update_flags(g);
	history_delete_entire_history(g->hist);
	g->hist = history_insert_first(history_create_empty(), F_ERROR, 0, 0);
}
