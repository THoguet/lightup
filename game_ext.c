#include "game_ext.h"
#include <stdio.h>
#include <stdlib.h>

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping) {
	game g = (game)malloc(sizeof(struct game_s));
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) {
	return;
}

uint game_nb_rows(cgame g) {
	return;
}

uint game_nb_cols(cgame g) {
	return;
}

bool game_is_wrapping(cgame g) {
	return;
}

void game_undo(game g) {
	return;
}

void game_redo(game g) {
	return;
}