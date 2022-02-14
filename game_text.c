#include <stdlib.h>
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

#define NBCHARERR 4
#define CHAR0ASCII 48
#define GAME_PRINT_PLUS_LINE(g)                                     \
	printf("--------------------------------------------------\n"); \
	game_print(g);

/**
 * @brief Edit the stringError with all errors of g
 * @param g the game to analyze
 * @param stringError A pointer toward a string allocated before
 * @pre @p stringError size must be at least height * width * NBCHARERR
 * @return true if there is an error on the game g and the stringError has been modified
 */
bool checkerrors(cgame g, char* stringError) {
	if (stringError == NULL || g == NULL) {
		return -1;
	}
	uint index_taberr = 0;
	// initialize stringError
	stringError[0] = '\0';
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
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
		GAME_PRINT_PLUS_LINE(g);
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
	printf("w <n> : Sauvegardez votre partie au nom <n>.\n");
}

void usage(char* argv[]) {
	printf("Usage : %s            => Joue la partie par défaut\n", argv[0]);
	printf("        %s <filename> => Charge la partie sur le fichier <filename>\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	game g;
	if (argc == 1) {
		// Create new game
		g = game_default();
	} else if (argc == 2) {
		char* gameFile = argv[1];
		g = game_load(gameFile);
		game_update_flags(g);
	} else {
		usage(argv);
	}
	// init string for errors
	char stringError[game_nb_rows(g) * game_nb_cols(g) * NBCHARERR];
	char c;
	char* fileSave = (char*)malloc(sizeof(char) * 20);
	// print game
	GAME_PRINT_PLUS_LINE(g);
	// test if game is over
	while (!game_is_over(g)) {
		// print errors
		if (checkerrors(g, stringError))
			printf("\nIl y a un problème case(s): %s\n", stringError);
		printf("\nEntrez une commande (h pour afficher l'aide): ");
		// scan the first command without args
		if (scanf(" %c", &c)) {
			switch (c) {
				case 'h':
					printhelp();
					break;
				case 'r':
					game_restart(g);
					printf("\nPartie réinitialisé avec succes !\n");
					GAME_PRINT_PLUS_LINE(g);
					break;
				case 'q':
					game_delete(g);
					printf("shame on you :o\n");
					return EXIT_SUCCESS;
				case 'z':
					game_undo(g);
					GAME_PRINT_PLUS_LINE(g);
					break;
				case 'y':
					game_redo(g);
					GAME_PRINT_PLUS_LINE(g);
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
				case 'w':
					scanf(" %s", fileSave);
					game_save(g, fileSave);
					printf("Partie sauvegardée au nom %s\n", fileSave);
					GAME_PRINT_PLUS_LINE(g);
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