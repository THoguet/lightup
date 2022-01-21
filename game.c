#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_ext.h"
#include "game_private.h"

// JWRAPPING and IWRAPPING have a "+ game_nb_xxxx(g)" to avoid falling in negative when we are at 0 for exemple
#define JWRAPPING (j + game_nb_cols(g) + gap_position * tab[index_tab + 1]) % game_nb_cols(g)
#define IWRAPPING (i + game_nb_rows(g) + gap_position * tab[index_tab]) % game_nb_rows(g)
#define JNORMAL j + gap_position* tab[index_tab + 1]
#define INORMAL i + gap_position* tab[index_tab]

game game_new(square* squares) {
	game g = game_new_empty();
	for (int i = 0; i < game_nb_rows(g); i++) {
		for (int j = 0; j < game_nb_cols(g); j++) {
			g->tab_cell[i][j] = squares[game_nb_cols(g) * i + j];
		}
	}
	return g;
}

game game_new_empty(void) {
	// alloc struct
	game g = (game)malloc(sizeof(struct game_s));
	if (g == NULL) {
		memoryError();
	}
	// set variable of struct
	g->wrapping = false;
	g->height = DEFAULT_SIZE;
	g->width = DEFAULT_SIZE;
	g->hist = history_create_empty();
	// alloc the tab with variables set before
	g->tab_cell = (square**)malloc(sizeof(square*) * game_nb_rows(g));
	if (g->tab_cell == NULL) {
		memoryError();
	}
	for (int i = 0; i < game_nb_rows(g); i++) {
		g->tab_cell[i] = (square*)malloc(sizeof(square) * game_nb_cols(g));
		if (g->tab_cell[i] == NULL) {
			memoryError();
		}
		for (int j = 0; j < game_nb_cols(g); j++) {
			g->tab_cell[i][j] = S_BLANK;
		}
	}
	// init history with an cell (if there is only this cell we consider the history as empty)
	g->hist = history_insert_first(g->hist, F_ERROR, 0, 0);
	return g;
}

game game_copy(cgame g1) {
	game g2 = game_new_empty_ext(g1->height, g1->width, g1->wrapping);
	for (int i = 0; i < g2->height; i++) {
		for (int j = 0; j < g2->width; j++) {
			g2->tab_cell[i][j] = g1->tab_cell[i][j];
		}
	}
	// delete first allocated hist (by game new) of g2
	history_delete_entire_history(g2->hist);
	g2->hist = history_copy(g1->hist);
	return g2;
}

bool game_equal(cgame g1, cgame g2) {
	if (g1->height != g2->height || g1->width != g2->width || g1->wrapping != g2->wrapping)
		return false;
	for (int i = 0; i < g1->height; i++) {
		for (int j = 0; j < g1->width; j++) {
			if (g1->tab_cell[i][j] != g2->tab_cell[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void game_delete(game g) {
	// free each tab of tabcell
	for (int i = 0; i < game_nb_rows(g); i++) {
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
	square temp = g->tab_cell[i][j];
	square list_flag[] = {F_ERROR, F_LIGHTED};
	for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
		if (temp >= list_flag[flag_index]) {
			temp -= list_flag[flag_index];
		}
	}
	return temp;
}

square game_get_flags(cgame g, uint i, uint j) {
	square temp = g->tab_cell[i][j];
	square list_flag[] = {(F_ERROR | F_LIGHTED), F_ERROR, F_LIGHTED};
	for (uint flag_index = 0; flag_index < sizeof(list_flag) / sizeof(list_flag[0]); flag_index++) {
		if (temp >= list_flag[flag_index]) {
			return list_flag[flag_index];
		}
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
	return game_get_state(g, i, j) - S_BLACK;
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
	return (i < game_nb_rows(g) && j < game_nb_cols(g) && (s == S_BLANK || s == S_MARK || s == S_LIGHTBULB) && !game_is_black(g, i, j));
}

void game_play_move(game g, uint i, uint j, square s) {
	// shouldn't happen because we initalized in game new
	if (g->hist == NULL) {
		g->hist = history_create_empty();
		g->hist = history_insert_first(g->hist, game_get_state(g, i, j), i, j);
	} else {
		// to avoid redo after play
		g->hist = history_delete_all_after(g->hist, g->hist);
		// add new played element to hist and get the last element of hist
		g->hist = history_next(history_insert_after(g->hist, g->hist, game_get_state(g, i, j), i, j));
	}
	game_set_square(g, i, j, s);
	game_update_flags(g);
}

void delete_flags(game g) {
	// delete prev flags
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
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

void update_flags_lightbulb(game g, uint i, uint j) {
	// add F_LIGHTED to the current lightbulb
	addF_LIGHTED(g, i, j);
	// add F_LIGHTED on the 4 directions, when reaching a wall by following a direction, remove it from tab, when reaching another lightbulb, put a
	// F_ERROR on the initial lightbulb
	//  tab[] = { i, j, i, j, i,  j, i, j}
	int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
	for (int gap_position = 1; gap_position < max(game_nb_cols(g), game_nb_rows(g)); gap_position++) {
		// index_tab => pair = i
		// index_tab => odd = j
		for (uint index_tab = 0; index_tab < sizeof(tab) / sizeof(tab[0]) - 1; index_tab += 2) {
			// big if to check if INORMAL and JNORMAL are correct or if wrapping is true and IWRAPPING and JWRAPPING are not the initial case
			if (/* test if both tab are not 0*/ tab[index_tab] != tab[index_tab + 1] &&
			    (/* normal check */ (JNORMAL >= 0 && JNORMAL < game_nb_cols(g) && INORMAL >= 0 && INORMAL < game_nb_rows(g)) ||
			     /* wrapping check + not the inital case*/ (game_is_wrapping(g) && !(IWRAPPING == i && JWRAPPING == j)))) {
				if (game_is_black(g, IWRAPPING, JWRAPPING)) {
					tab[index_tab] = 0;
					tab[index_tab + 1] = 0;
				} else {
					// if we found another lightbulb on the line
					if (game_is_lightbulb(g, IWRAPPING, JWRAPPING))
						// update initial lightbulb on F_ERROR
						game_set_square(g, i, j, (S_LIGHTBULB | F_LIGHTED | F_ERROR));
					addF_LIGHTED(g, IWRAPPING, JWRAPPING);
				}
			}
		}
	}
}

void update_flags_walls(game g, uint i, uint j) {
	int lightbulb = 0;
	int not_empty = 0;
	int tab[] = {-1, 0, 1, 0, 0, -1, 0, 1};
	for (uint index_tab = 0; index_tab < (sizeof(tab) / sizeof(tab[0])); index_tab = index_tab + 2) {
		uint gap_position = 1;
		if (/* normal check*/ (JNORMAL >= 0 && JNORMAL < game_nb_cols(g) && INORMAL >= 0 && INORMAL < game_nb_rows(g)) ||
		    /*wrapping check*/ (game_is_wrapping(g) &&
		                        /*not the same case*/ !(IWRAPPING == i && JWRAPPING == j))) {
			if (game_is_lightbulb(g, IWRAPPING, JWRAPPING))
				lightbulb++;
			else if (!game_is_blank(g, IWRAPPING, JWRAPPING) || game_is_lighted(g, IWRAPPING, JWRAPPING))
				not_empty++;
		} else {
			not_empty++;
		}
	}
	if (lightbulb > game_get_black_number(g, i, j)) {
		game_set_square(g, i, j, (game_get_state(g, i, j) | F_ERROR));
	}
	// look if there is enough empty cell around the wall (4 directions)
	else if (not_empty > abs(game_get_black_number(g, i, j) - 4)) {
		game_set_square(g, i, j, (game_get_state(g, i, j) | F_ERROR));
	}
}

void game_update_flags(game g) {
	delete_flags(g);
	// update flags lighted and lightbulb error
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_is_lightbulb(g, i, j)) {
				update_flags_lightbulb(g, i, j);
			}
		}
	}
	// update flags wall (need to wait all FLIGHTED being updated)
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_is_black(g, i, j) && game_get_black_number(g, i, j) >= 0) {
				update_flags_walls(g, i, j);
			}
		}
	}
}

bool game_is_over(cgame g) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if ((game_get_state(g, i, j) == S_BLANK && game_get_flags(g, i, j) == S_BLANK) || game_get_flags(g, i, j) == F_ERROR ||
			    game_get_flags(g, i, j) == (F_LIGHTED | F_ERROR))
				return false;
		}
	}
	return true;
}

void game_restart(game g) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_get_state(g, i, j) == S_MARK || game_is_lightbulb(g, i, j))
				game_set_square(g, i, j, S_BLANK);
		}
	}
	game_update_flags(g);
	history_delete_entire_history(g->hist);
	// insert f_error on first cell of histo. initalized to avoid bug on first move undo
	g->hist = history_insert_first(history_create_empty(), F_ERROR, 0, 0);
}
