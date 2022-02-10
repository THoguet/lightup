#include "game_tools.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_ext.h"
#include "game_private.h"

game game_load(char* filename) {
	return NULL;
}
/**
 * @brief Get the char from state object
 *
 * @param g game the get the element from
 * @param i the i coordinate
 * @param j the j coordinate
 * @return char the corresponding char
 */
char get_char_from_state(game g, uint i, uint j) {
	// char tab[index] => index in enum square
	char tab[] = {'b', '*', '-', '?', '?', '?', '?', '?', '0', '1', '2', '3', '4', 'w'};
	return tab[game_get_state(g, i, j)];
}

void game_save(cgame g, char* filename) {
	checkPointer(filename);
	checkPointer(g);
	FILE* f = fopen(filename, 'w');
	if (f == NULL) {
		fprintf(stderr, "Couldn't open file %s.\n", filename);
		exit(EXIT_FAILURE);
	}
	fprintf(f, "%d %d %d\n", game_nb_rows(g), game_nb_cols(g), game_is_wrapping(g));
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			fprintf(f, "%c", get_char_from_state(g, i, j));
		}
		fprintf(f, "\n");
	}
	fclose(f);
}