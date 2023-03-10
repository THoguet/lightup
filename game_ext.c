#include "game_ext.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_private.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping) {
	game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			g->tab_cell[i][j] = squares[game_nb_cols(g) * i + j];
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
	// alloc first tab
	g->tab_cell = (square**)malloc(sizeof(square*) * game_nb_rows(g));
	if (g->tab_cell == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	// alloc each tab of tab_cell
	for (uint i = 0; i < game_nb_rows(g); i++) {
		g->tab_cell[i] = (square*)calloc(game_nb_cols(g), sizeof(square));
		if (g->tab_cell[i] == NULL) {
			fprintf(stderr, "not enough memory\n");
			exit(EXIT_FAILURE);
		}
	}
	// insert f_error on first cell of histo. initalized to avoid bug on first move undo
	g->hist = history_insert_first(g->hist, F_ERROR, 0, 0);
	return g;
}

uint game_nb_rows(cgame g) {
	return g->height;
}

uint game_nb_cols(cgame g) {
	return g->width;
}

bool game_is_wrapping(cgame g) {
	return g->wrapping;
}

void game_undo(game g) {
	// check if g->hist is not empty or if it is possible to undo
	if (g->hist == NULL || history_is_empty(history_prev(g->hist))) {
		printf("you are at your first move\n");
	} else {
		square tmp = game_get_state(g, history_i(g->hist), history_j(g->hist));
		game_set_square(g, history_i(g->hist), history_j(g->hist), history_state(g->hist));
		game_update_flags(g);
		g->hist->state = tmp;
		g->hist = history_prev(g->hist);
	}
}

void game_redo(game g) {
	// check if g->hist is not empty or if it is possible to redo
	if (g->hist == NULL || history_next(g->hist) == NULL) {
		printf("you are at your last move\n");
	} else {
		g->hist = history_next(g->hist);
		square tmp = game_get_state(g, history_i(g->hist), history_j(g->hist));
		game_set_square(g, history_i(g->hist), history_j(g->hist), history_state(g->hist));
		game_update_flags(g);
		g->hist->state = tmp;
	}
}