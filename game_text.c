#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"

bool checkerr(cgame g, char taberr[]) {
	if (taberr == NULL || g == NULL) {
		return -1;
	}
	uint err = 0;
	taberr[0] = '\0';
	for (int i = 0; i < g->height; i++) {
		for (int j = 0; j < g->width; j++) {
			// check each case
			if (game_has_error(g, i, j)) {
				// if error append taberr with coo+,
				taberr[err] = i + 48;
				err++;
				taberr[err] = ':';
				err++;
				taberr[err] = j + 48;
				err++;
				taberr[err] = ' ';
				err++;
			}
		}
	}
	// if taberr changes print
	if (taberr[0] != '\0') {
		taberr[err] = '\0';
		return true;
	}
	return false;
}

void printhelp(void) {
	printf("Liste des différentes commandes :\n");
	printf("h : Affiche cette aide.\n");
	printf("r : Réinitialise la partie.\n");
	printf("q : Quitter la partie (abandon).\n");
	printf(
	    "l <i> <j> : Place une ampoule (*light bulb*) dans la case "
	    "(<i>,<j>).\n");
	printf("m <i> <j> : Place une marque (*mark*) dans la case (<i>,<j>).\n");
	printf(
	    "b <i> <j> : Place placer une case vide (*blank*) à la position "
	    "(<i>,<j>).\n");
}

int main(void) {
	// Create new game
	game g = game_default();
	// test if game is over
	uint i;
	uint j;
	char taberr[g->height * g->width * 4];
	char c;
	// print game
	game_print(g);
	while (!game_is_over(g)) {
		// print errors
		if (checkerr(g, taberr)) {
			printf("Il y a un problème case: %s\n", taberr);
		}
		printf("Entrez une commande (h pour afficher l'aide): ");
		if (scanf(" %c", &c)) {
			if (c == 'h') {
				printhelp();
			} else if (c == 'r') {
				game_restart(g);
				printf("Partie réinitialisé avec success !\n");
				game_update_flags(g);
				game_print(g);
			} else if (c == 'q') {
				printf("Solution: \n");
				game sol = game_default_solution();
				game_update_flags(sol);
				game_print(sol);
				game_delete(sol);
				game_delete(g);
				printf("shame\n");
				return EXIT_SUCCESS;
			} else if (c == 'l' || c == 'm' || c == 'b') {
				if (scanf(" %u %u", &i, &j) == 2) {
					if (c == 'l' && game_check_move(g, i, j, S_LIGHTBULB)) {
						game_play_move(g, i, j, S_LIGHTBULB);
						game_print(g);
					} else if (c == 'm' && game_check_move(g, i, j, S_MARK)) {
						game_play_move(g, i, j, S_MARK);
						game_print(g);
					} else if (c == 'b' && game_check_move(g, i, j, S_BLANK)) {
						game_play_move(g, i, j, S_BLANK);
						game_print(g);
					} else
						printf("Erreur: (<i>,<j>) invalide.\n");
				}
			} else {
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