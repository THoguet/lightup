#include "game_tools.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

game game_load(char* filename) {
	// ouvrons le fichier
	FILE* f = fopen(filename, "r");
	// récuperer les valeurs dont on a beson pour créer le jeu initail
	uint nb_rows = fscanf(f, "%u"), nb_cols = fscanf(f, "%u");
	bool wrapping = fscanf(f, "%d");
	// créer le jeu initail
	game g1 = game_new_empty_ext(nb_rows, nb_cols, wrapping);
	// récuperer les coups à jouer
	for (uint i = 0; i < nb_rows; i++) {
		for (uint j = 0; j < nb_cols; j++) {
			while (1) {
				// obtenir les squares pour les jouer dans g1
				char play = fgetc(f);
				if (feof(f)) {
					break;
				}
				if (play = 'b')
					game_set_square(g1, i, j, S_BLANK);
				else if (play = '*')
					game_set_square(g1, i, j, S_LIGHTBULB);
				else if (play = '-')
					game_set_square(g1, i, j, S_MARK);
				else if (play = 'w')
					game_set_square(g1, i, j, S_BLACKU);
				else if (play = '0')
					game_set_square(g1, i, j, S_BLACK0);
				else if (play = '1')
					game_set_square(g1, i, j, S_BLACK1);
				else if (play = '2')
					game_set_square(g1, i, j, S_BLACK2);
				else if (play = '3')
					game_set_square(g1, i, j, S_BLACK3);
				else if (play = '4')
					game_set_square(g1, i, j, S_BLACK0);
			}
		}
	}
	return g1;
}

void game_save(cgame g, char* filename) {
	return;
}
