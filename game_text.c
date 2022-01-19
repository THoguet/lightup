#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

#define NBCHARERR 4
#define CHAR0ASCII 48

/**
 * @brief Edit the stringError with all errors of g
 * @param g the game to analyze
 * @param stringError A pointer toward a string allocated before
 * @pre @p stringError size must be at least g->height * g->width * NBCHARERR
 * @return true if there is an error on the game g and the stringError has been modified
 */
bool checkerrors(cgame g, char* stringError) {
	if (stringError == NULL || g == NULL) {
		return -1;
	}
	uint index_taberr = 0;
	// initialize stringError
	stringError[0] = '\0';
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			// check each case
			if (game_has_error(g, i, j)) {
				// if error append stringError with coordinate of the error,
				stringError[index_taberr] = i + CHAR0ASCII;
				index_taberr++;
				stringError[index_taberr] = ':';
				index_taberr++;
				stringError[index_taberr] = j + CHAR0ASCII;
				index_taberr++;
				stringError[index_taberr] = ' ';
				index_taberr++;
			}
		}
	}
	// if stringError changed return true
	if (stringError[0] != '\0') {
		stringError[index_taberr] = '\0';
		return true;
	}
	return false;
}

/**
 * @brief scan the coordinate and check if the move is possible if yes play it
 * @param g the game to check and play
 * @param s the square to update
 */
void checkPlayMove(game g, square s) {
	uint i;
	uint j;
	if (scanf(" %u %u", &i, &j) == 2) {
		if (game_check_move(g, i, j, s))
			game_play_move(g, i, j, s);
		else
			printf("Coordonnées invalide.\n");
		game_print(g);
		return;
	}
	printf("Commande inconnue.\n");
}

void printhelp(void) {
	printf("Liste des différentes commandes :\n");
	printf("h : Affiche cette aide.\n");
	printf("r : Réinitialise la partie.\n");
	printf("q : Quitter la partie (abandon).\n");
	printf("z : Annulez le coup (undo).\n");
	printf("y : Annulez l'annulation du coup (redo).\n");
	printf("l <i> <j> : Place une ampoule (*) à la position (<i>,<j>).\n");
	printf("m <i> <j> : Place une marque (-) à la position (<i>,<j>).\n");
	printf("b <i> <j> : Place une case vide ( ) à la position (<i>,<j>).\n");
}

void usage(int argc, char* argv[]) {
	printf("Usage : %s => Joue la partie par défaut\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	game g;
	if (argc == 1)
		// Create new game
		g = game_default();
	else
		usage(argc, argv);
	// init string for errors
	char stringError[g->height * g->width * NBCHARERR];
	char c;
	// print game
	game_print(g);
	// test if game is over
	while (!game_is_over(g)) {
		// print errors
		if (checkerrors(g, stringError))
			printf("Il y a un problème case(s): %s\n", stringError);
		printf("Entrez une commande (h pour afficher l'aide): ");
		// scan the first command without args
		if (scanf(" %c", &c)) {
			switch (c) {
				case 'h':
					printhelp();
					break;
				case 'r':
					game_restart(g);
					printf("Partie réinitialisé avec succes !\n");
					game_print(g);
					break;
				case 'q':
					printf("Solution: \n");
					game solution = game_default_solution();
					game_print(solution);
					game_delete(solution);
					game_delete(g);
					printf("shame\n");
					return EXIT_SUCCESS;
					break;
				case 'z':
					game_undo(g);
					game_print(g);
					break;
				case 'y':
					game_redo(g);
					game_print(g);
					break;
				case 'l':
					checkPlayMove(g, S_LIGHTBULB);
					break;
				case 'm':
					checkPlayMove(g, S_MARK);
					break;
				case 'b':
					checkPlayMove(g, S_BLANK);
					break;
				default:
					printf("Commande inconnue.\n");
			}
		} else {
			printf("Commande inconnue.\n");
		}
	}
	printf("congratulation\n");
	game_delete(g);
	return EXIT_SUCCESS;
}