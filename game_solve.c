#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

void usage(char* argv[]) {
	printf("Usage : %s -s <input> [<output>] => Find a solution of the game saved in file <input> and optionally save the solution to the file <output>.\n",
	       argv[0]);
	printf(
	    "        %s -c <input> [<output>] => Find the number of solution(s) of the game saved in the file <input> and optionally save this number in the file "
	    "<output>.\n",
	    argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	// check if nb of param is correct
	if (argc != 3 && argc != 4) {
		// nb param is not correct
		usage(argv);
	}
	// get the name of the file and load it

	char* option = argv[1];
	char* file = argv[2];
	// if option is -s
	if (strcmp(option, "-s") == 0) {
		game g1 = game_load(file);
		printf("Game successfully load.\n");
		// check if there are any solutions
		bool solution = game_solve(g1);
		// if the game has not solutions EXIT
		if (solution == false) {
			fprintf(stderr, "There is no solution.\n");
			exit(EXIT_FAILURE);
		}
		printf("A solution has been found :\n");
		game_print(g1);
		// IF THERE ARE 3 ARGUMENTS WE LOAD THE GAME
		if (argc == 4) {
			char* filename = argv[3];
			game_save(g1, filename);
			printf("This game has been saved in the file named : %s.\n", filename);
		}
	}
	// if the option is -c
	else if (strcmp(option, "-c") == 0) {
		// GET THE NB OF SOLUTIONS
		cgame g2 = game_load(file);
		printf("Game successfully load.\n");
		uint nbSolutions = game_nb_solutions(g2);
		// IF THERE ARE 3 ARGUMENTs, I create a file with nb of solutions
		printf("%u solution(s) has been found", nbSolutions);
		if (argc == 4) {
			char* filename = argv[3];
			FILE* f = fopen(filename, "w");
			checkPointer((void*)f, "Couldn't open file in game_solve executable\n");
			test_output(fprintf(f, "%u\n", nbSolutions), 2, "Error when printing in file in game_solve executable\n");
			fclose(f);
			printf(" this number has been saved in file named : %s", filename);
		}
		printf("\n");
	}

	else {
		usage(argv);
	}
	return EXIT_SUCCESS;
}