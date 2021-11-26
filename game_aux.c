#include <stdbool.h>
#include <stdio.h>
#include "game.h"

char charPrintCell(square sqState) {
	if (sqState == S_BLANK) {
		return ' ';
	}
	if ((sqState == S_LIGHTBULB) ||
	    (sqState == (S_LIGHTBULB | F_ERROR)) |
	        (sqState == (S_LIGHTBULB | F_LIGHTED)) |
	        (sqState == (S_LIGHTBULB | F_ERROR | F_LIGHTED))) {
		return '*';
	}
	if ((sqState == S_MARK) ||
	    (sqState == (S_MARK | F_ERROR)) | (sqState == (S_MARK | F_LIGHTED))) {
		return '-';
	}
	if (sqState == S_BLACK0 || sqState == (S_BLACK0 | F_ERROR)) {
		return '0';
	}
	if (sqState == S_BLACK1 || sqState == (S_BLACK1 | F_ERROR)) {
		return '1';
	}
	if (sqState == S_BLACK2 || sqState == (S_BLACK2 | F_ERROR)) {
		return '2';
	}
	if (sqState == S_BLACK3 || sqState == (S_BLACK3 | F_ERROR)) {
		return '3';
	}
	if (sqState == S_BLACK4 || sqState == (S_BLACK4 | F_ERROR)) {
		return '4';
	}
	if (sqState == S_BLACKU || sqState == (S_BLACKU | F_ERROR)) {
		return 'w';
	}
	if (sqState == F_LIGHTED) {
		return '.';
	}
	return -1;
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
				printf("%c", charPrintCell(game_get_square(g, i, j)));
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
	game_update_flags(g);
	return g;
}