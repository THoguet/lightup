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
	game g = (game)malloc(sizeof(game));
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
		g->tab_cell[i] = NULL;
	}
	free(g);
	g = NULL;
	return;
}

void game_set_square(game g, uint i, uint j, square s) {
	return;
}

square game_get_square(cgame g, uint i, uint j) {
	return g->tab_cell[i][j];
}

square game_get_state(cgame g, uint i, uint j) {
	return 0;
}

square game_get_flags(cgame g, uint i, uint j) {
	return 0;
}

bool game_is_blank(cgame g, uint i, uint j) {
	return false;
}

bool game_is_lightbulb(cgame g, uint i, uint j) {
	return false;
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
	return false;
}

bool game_is_lighted(cgame g, uint i, uint j) {
	return false;
}

bool game_has_error(cgame g, uint i, uint j) {
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
	return;
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
}
