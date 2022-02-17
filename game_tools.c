#include "game_tools.h"
#include <stdlib.h>
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

#define NB_CHAR_HEADER_WITHTOUT_DIMENSIONS 4

void test_output(int i, int excepted, char* message) {
	if (i != excepted) {
		fprintf(stderr, "%s", message);
		exit(EXIT_FAILURE);
	}
}

game game_load(char* filename) {
	checkPointer((void*)filename);
	// ouvrons le fichier
	FILE* f = fopen(filename, "r");
	checkPointer((void*)f);
	// récuperer les valeurs dont on a beson pour créer le jeu initail
	uint nb_rows, nb_cols;
	int wrapping;
	test_output(fscanf(f, "%u %u %d\n", &nb_rows, &nb_cols, &wrapping), 3, "Error in scan of file");
	// créer le jeu initail
	game g1 = game_new_empty_ext(nb_rows, nb_cols, wrapping);
	checkPointer((void*)g1);
	// récuperer les coups à jouer
	for (uint i = 0; i < nb_rows; i++) {
		for (uint j = 0; j < nb_cols; j++) {
			if (feof(f)) {
				fprintf(stderr, "end of file");
				fclose(f);
				exit(EXIT_FAILURE);
			}
			// obtenir les squares pour les jouer dans g1
			char play = fgetc(f);
			if (play == '\n')
				play = fgetc(f);
			if (play == 'b')
				game_set_square(g1, i, j, S_BLANK);
			else if (play == '*')
				game_set_square(g1, i, j, S_LIGHTBULB);
			else if (play == '-')
				game_set_square(g1, i, j, S_MARK);
			else if (play == 'w')
				game_set_square(g1, i, j, S_BLACKU);
			else if (play == '0')
				game_set_square(g1, i, j, S_BLACK0);
			else if (play == '1')
				game_set_square(g1, i, j, S_BLACK1);
			else if (play == '2')
				game_set_square(g1, i, j, S_BLACK2);
			else if (play == '3')
				game_set_square(g1, i, j, S_BLACK3);
			else if (play == '4')
				game_set_square(g1, i, j, S_BLACK4);
		}
	}
	fclose(f);
	game_update_flags(g1);
	return g1;
}

/**
 * @brief Get the char from state object
 *
 * @param g game to get the square from
 * @param i the i coordinate
 * @param j the j coordinate
 * @return char of the corresponding square
 */
char get_char_from_state(cgame g, uint i, uint j) {
	// char tab[index] => index in enum square
	char tab[] = {'b', '*', '-', '?', '?', '?', '?', '?', '0', '1', '2', '3', '4', 'w'};
	return tab[(int)game_get_state(g, i, j)];
}

void game_save(cgame g, char* filename) {
	checkPointer((void*)filename);
	checkPointer((void*)g);
	// try create or open file named filename
	FILE* f = fopen(filename, "w");
	checkPointer((void*)f);
	// try to print the first line
	test_output(fprintf(f, "%d %d %d\n", game_nb_rows(g), game_nb_cols(g), game_is_wrapping(g)),
	            NB_CHAR_HEADER_WITHTOUT_DIMENSIONS + game_nb_rows(g) / 10 + 1 + game_nb_cols(g) / 10 + 1, "Couldn't print in file.\n");
	// for each case of the game get the char of the given case and try to print it on the file
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			test_output(fprintf(f, "%c", get_char_from_state(g, i, j)), 1, "Couldn't print in file.\n");
		}
		// print a \n at the end of each row
		test_output(fprintf(f, "\n"), 1, "Couldn't print in file.\n");
	}
	fclose(f);
}