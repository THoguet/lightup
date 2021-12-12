#include "game_ext.h"
#include <stdio.h>
#include <stdlib.h>

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping) {
	game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			g->tab_cell[i][j] = squares[g->width * i + j];
		}
	}
	return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) {
	// alloc struct
	game g = (game)malloc(sizeof(struct game_s));
	if (g == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	// set variable of struct
	g->wrapping = wrapping;
	g->height = nb_rows;
	g->width = nb_cols;
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
	return g;
}

uint game_nb_rows(cgame g) {
	return 0;
}

uint game_nb_cols(cgame g) {
	return 0;
}

bool game_is_wrapping(cgame g) {
	return false;
}

void game_undo(game g) {
	if (history_prev(g->hist) == NULL) {
		printf("waiting for your first move");
	} else {
		game_set_square(g, history_i(history_prev(g->hist)), history_j(history_prev(g->hist)), history_state(history_prev(g->hist)));
		game_update_flags(g);
		g->hist = history_prev(g->hist);
	}
	return;
}

void game_redo(game g) {
	return;
}