#include <stdlib.h>
#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

void usage(char* argv[]) {
	printf("Usage : %s            => Joue la partie par défaut\n", argv[0]);
	printf("        %s <filename> => Charge la partie sur le fichier <filename>\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	game g;
	if (argc == 3) {
		char* gameFile = argv[1];
		g = game_load(gameFile);
	} else {
		usage(argv);
	}
	char* option = argv[0];
	char* output = argv[2];
}
