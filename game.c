#include "game.h"
#include <stdio.h>
#include <stdlib.h>

struct game_s {
	square** tab_cell;
};

game game_new(square* squares) {
	game g = game_new_empty();
	for (int i = 0; i < DEFAULT_SIZE; i++) {
		for (int j = 0; j < DEFAULT_SIZE; j++) {
			g->tab_cell[i][j] = squares[DEFAULT_SIZE * i + j];
		}
	}
	return g;
}

game game_new_empty(void) {
	game g = (game)malloc(sizeof(struct game_s));
	if (g == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	g->tab_cell = (square**)malloc(sizeof(square*) * DEFAULT_SIZE);
	if (g->tab_cell == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < DEFAULT_SIZE; i++) {
		g->tab_cell[i] = (square*)malloc(sizeof(square) * DEFAULT_SIZE);
		if (g->tab_cell[i] == NULL) {
			fprintf(stderr, "not enough memory\n");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < DEFAULT_SIZE; j++) {
			g->tab_cell[i][j] = S_BLANK;
		}
	}
	return g;
}

game game_copy(cgame g1) {
	game g2 = game_new_empty();
	for (int i = 0; i < DEFAULT_SIZE; i++) {
		for (int j = 0; j < DEFAULT_SIZE; j++) {
			g2->tab_cell[i][j] = g1->tab_cell[i][j];
		}
	}
	return g2;
}

bool game_equal(cgame g1, cgame g2) {
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
	for (int i = 0; i < DEFAULT_SIZE; i++) {
		free(g->tab_cell[i]);
		if (g->tab_cell[i] != NULL)
			g->tab_cell[i] = NULL;
	}
	free(g->tab_cell);
	if (g->tab_cell != NULL)
		g->tab_cell = NULL;
	free(g);
	if (g != NULL)
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
	return (g->tab_cell[i][j] == S_BLANK);
}

bool game_is_lightbulb(cgame g, uint i, uint j) {
	return ((g->tab_cell[i][j] == S_LIGHTBULB) ||
	        (g->tab_cell[i][j] == (S_LIGHTBULB | F_LIGHTED)) ||
	        (g->tab_cell[i][j] == (S_LIGHTBULB | F_ERROR)) ||
	        (g->tab_cell[i][j] == (S_LIGHTBULB | F_LIGHTED | F_ERROR)));
}

bool game_is_black(cgame g, uint i, uint j) {
	if (game_get_state(g, i, j) >= 8 && game_get_state(g, i, j) <= 13)
		return true;
	return false;
}

int game_get_black_number(cgame g, uint i, uint j) {
	if (game_get_state(g, i, j) == S_BLACKU)
		return -1;
	return game_get_state(g, i, j) - 8;
}

bool game_is_marked(cgame g, uint i, uint j) {
	return ((g->tab_cell[i][j] == S_MARK) ||
	        (g->tab_cell[i][j] == (S_MARK | F_LIGHTED)) ||
	        (g->tab_cell[i][j] == (S_MARK | F_ERROR)) ||
	        (g->tab_cell[i][j] == (S_MARK | F_LIGHTED | F_ERROR)));
}

bool game_is_lighted(cgame g, uint i, uint j) {
	if (g->tab_cell[i][j] >= F_ERROR) {
		g->tab_cell[i][j] -= F_ERROR;
	}
	if (g->tab_cell[i][j] >= F_LIGHTED) {
		return true;
	}
	return false;
}

bool game_has_error(cgame g, uint i, uint j) {
	if (g->tab_cell[i][j] >= F_ERROR) {
		return true;
	}
	return false;
}

bool game_check_move(cgame g, uint i, uint j, square s) {
	if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE ||
	    (s != S_BLANK && s != S_MARK && s != S_LIGHTBULB) ||
	    game_is_black(g, i, j))
		return false;
	return true;
}

void game_play_move(game g, uint i, uint j, square s) {
	game_set_square(g, i, j, s);
	game_update_flags(g);
}

void game_update_flags(game g) {
	// delete prev flags
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			game_set_square(g, i, j, game_get_state(g, i, j));
		}
	}
	// update flags lighted and lightbulb error
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			if (game_get_state(g, i, j) == S_LIGHTBULB) {
				if (game_get_flags(g, i, j) > 16)
					game_set_square(g, i, j,
					                (S_LIGHTBULB | F_LIGHTED | F_ERROR));
				else
					game_set_square(g, i, j, (S_LIGHTBULB | F_LIGHTED));
				for (uint i2 = i + 1;
				     i2 < DEFAULT_SIZE && !game_is_black(g, i2, j); i2++) {
					if (game_get_state(g, i2, j) == S_LIGHTBULB)
						// update initial lightbulb on F_ERROR
						game_set_square(g, i, j,
						                (S_LIGHTBULB | F_LIGHTED | F_ERROR));
					// update cases on FLIGHTED
					game_set_square(g, i2, j,
					                (game_get_state(g, i2, j) | F_LIGHTED));
				}
				for (int i2 = i - 1; i2 > 0 && !game_is_black(g, i2, j); i2--) {
					if (game_get_state(g, i2, j) == S_LIGHTBULB)
						// update initial lightbulb on F_ERROR
						game_set_square(g, i, j,
						                (S_LIGHTBULB | F_LIGHTED | F_ERROR));
					game_set_square(g, i2, j,
					                (game_get_state(g, i2, j) | F_LIGHTED));
				}
				for (int j2 = j - 1; j2 > 0 && !game_is_black(g, i, j2); j2--) {
					if (game_get_state(g, i, j2) == S_LIGHTBULB)
						// update initial lightbulb on F_ERROR
						game_set_square(g, i, j,
						                (S_LIGHTBULB | F_LIGHTED | F_ERROR));
					game_set_square(g, i, j2,
					                (game_get_state(g, i, j2) | F_LIGHTED));
				}
				for (uint j2 = j + 1;
				     j2 < DEFAULT_SIZE && !game_is_black(g, i, j2); j2++) {
					if (game_get_state(g, i, j2) == S_LIGHTBULB)
						// update initial lightbulb on F_ERROR
						game_set_square(g, i, j,
						                (S_LIGHTBULB | F_LIGHTED | F_ERROR));
					game_set_square(g, i, j2,
					                (game_get_state(g, i, j2) | F_LIGHTED));
				}
			}
		}
	}
	// update flags wall
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			if (game_is_black(g, i, j)) {
				if (game_get_black_number(g, i, j) >= 0) {
					square choice[4];
					int size = 0;
					int lb = 0;
					int notempty = 0;
					if (i > 0) {
						choice[size] = game_get_square(g, i - 1, j);
						size++;
					} else {
						notempty++;
					}
					if (i < DEFAULT_SIZE - 1) {
						choice[size] = game_get_square(g, i + 1, j);
						size++;
					} else {
						notempty++;
					}
					if (j > 0) {
						choice[size] = game_get_square(g, i, j - 1);
						size++;
					} else {
						notempty++;
					}
					if (j < DEFAULT_SIZE - 1) {
						choice[size] = game_get_square(g, i, j + 1);
						size++;
					} else {
						notempty++;
					}
					for (int l = 0; l < size; l++) {
						if (choice[l] == S_LIGHTBULB ||
						    choice[l] == (S_LIGHTBULB | F_LIGHTED) ||
						    choice[l] == (S_LIGHTBULB | F_LIGHTED | F_ERROR)) {
							lb++;
						} else if (choice[l] != S_BLANK) {
							notempty++;
						}
					}
					if (lb > game_get_black_number(g, i, j)) {
						game_set_square(g, i, j,
						                (game_get_state(g, i, j) | F_ERROR));
					}
					// look if there is enough empty cell around the wall
					else if (notempty >
					         abs(game_get_black_number(g, i, j) - 4)) {
						game_set_square(g, i, j,
						                (game_get_state(g, i, j) | F_ERROR));
					}
				}
			}
		}
	}
}

bool game_is_over(cgame g) {
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			if ((game_get_state(g, i, j) == S_BLANK &&
			     game_get_flags(g, i, j) == S_BLANK) ||
			    game_get_flags(g, i, j) == F_ERROR ||
			    game_get_flags(g, i, j) == (F_LIGHTED | F_ERROR))
				return false;
		}
	}
	return true;
}

void game_restart(game g) {
	for (uint i = 0; i < DEFAULT_SIZE; i++) {
		for (uint j = 0; j < DEFAULT_SIZE; j++) {
			if (game_get_state(g, i, j) == S_MARK ||
			    game_get_state(g, i, j) == S_LIGHTBULB)
				game_set_square(g, i, j, S_BLANK);
		}
	}
	game_update_flags(g);
}
