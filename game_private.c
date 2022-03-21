#include "game_private.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_ext.h"

uint max(uint a, uint b) {
	if (a > b)
		return a;
	return b;
}

uint min(uint a, uint b) {
	if (a < b)
		return a;
	return b;
}

int int_min(int a, int b) {
	if (a < b)
		return a;
	return b;
}

void checkPointer(void* p, char* errMessage) {
	if (!p) {
		fprintf(stderr, "%s", errMessage);
		exit(EXIT_FAILURE);
	}
}

void test_output(int i, int excepted, char* message) {
	if (i != excepted) {
		fprintf(stderr, message);
		exit(EXIT_FAILURE);
	}
}

uint get_nb_char_from_int(uint number) {
	uint res = 0;
	do {
		res++;
		number = number / 10;
	} while (number != 0);
	return res;
}

bool game_has_error_general(cgame g) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_has_error(g, i, j))
				return true;
		}
	}
	return false;
}