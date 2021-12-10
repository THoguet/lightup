#include <stdbool.h>
#include <stdio.h>
#include "game.h"

char charPrintCell(cgame g, uint i, uint j) {
	square sqState = game_get_state(g, i, j);
	square sqFlags = game_get_flags(g, i, j);
	char tab[] = {' ', '*', '-', '?', '?', '?', '?', '?', '0', '1', '2', '3', '4', 'w', '?', '?', '.'};
	if (!sqState)
		return tab[sqFlags];
	return tab[sqState];
}

void game_print(cgame g) {
	if (g != NULL) {
		uint i;
		printf("   0");
		for (i = 1; i < DEFAULT_SIZE; i++) {
			printf("%u", i);
		}
		printf("\n   -");
		for (i = 1; i < DEFAULT_SIZE; i++) {
			printf("-");
		}
		printf("\n");
		for (i = 0; i < DEFAULT_SIZE; i++) {
			printf("%u |", i);
			for (uint j = 0; j < DEFAULT_SIZE; j++) {
				printf("%c", charPrintCell(g, i, j));
			}
			printf("|\n");
		}
		printf("   -");
		for (i = 1; i < DEFAULT_SIZE; i++) {
			printf("-");
		}
		printf("\n");
	}
}
game game_default(void) {
	game g = game_new_empty();
	game_set_square(g, 0, 2, S_BLACK1);
	game_set_square(g, 1, 2, S_BLACK2);
	game_set_square(g, 2, 5, S_BLACKU);
	game_set_square(g, 2, 6, S_BLACK2);
	game_set_square(g, 4, 0, S_BLACK1);
	game_set_square(g, 4, 1, S_BLACKU);
	game_set_square(g, 5, 4, S_BLACK2);
	game_set_square(g, 6, 4, S_BLACKU);
	return g;
}

game game_default_solution(void) {
	game g = game_default();
	game_set_square(g, 0, 0, S_LIGHTBULB);
	game_set_square(g, 1, 1, S_LIGHTBULB);
	game_set_square(g, 0, 3, S_LIGHTBULB);
	game_set_square(g, 1, 6, S_LIGHTBULB);
	game_set_square(g, 2, 2, S_LIGHTBULB);
	game_set_square(g, 3, 6, S_LIGHTBULB);
	game_set_square(g, 4, 4, S_LIGHTBULB);
	game_set_square(g, 5, 0, S_LIGHTBULB);
	game_set_square(g, 5, 5, S_LIGHTBULB);
	game_set_square(g, 6, 1, S_LIGHTBULB);
	return g;
}