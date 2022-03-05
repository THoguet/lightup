#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.c"
#include "game_tools.h"

void usage() {
	fprintf(stderr, "problem with paramametre");
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	// check if nb of param is correct
	if (argc != 3 && argc != 4) {
		// nb param is not correct
		usage();
	}
	// get the name of the file and load it
	char* file = argv[2];

	char* option = argv[1];
	// if option is -s
	if (strcmp(option, "-s") == 0) {
		game g1 = game_load(file);
		// check if there are any solutions
		bool solution = game_solve(g1);
		// if the game has not solutions EXIT
		if (solution == false) {
			fprintf(stderr, "there is not solution");
			exit(EXIT_FAILURE);
		}
		// IF THERE ARE 3 ARGUMENTS WE LOAD THE GAME
		if (argc == 4) {
			char* filename = argv[3];
			game_save(g1, filename);
		}
	}
	// if the option is -s
	else if (strcmp(option, "-c") == 0) {
		// GET THE NB OF SOLUTIONS
		cgame g2 = game_load(file);
		uint nbsolutions = game_nb_solutions(g2);
		// IF THERE ARE 3 ARGUMENTs, I create a file with nb of solutions
		if (argc == 4) {
			char* filename = argv[3];
			FILE* f = fopen(filename, "w");
			checkPointer((void*)f, "Couldn't open file\n");
			test_output(fprintf(f, "%u\n", nbsolutions), 2, "Error in scan of file\n");
			fclose(f);
		}
	}
	// if argv[1] is neither -c nor -w
	/*else {
	    printf("the first parametre must be '-s' if you want the solution of the game\n");
	    printf("or '-c' if you want the number of solutions\n");
	    exit(EXIT_FAILURE);
	}*/

	return EXIT_SUCCESS;
}