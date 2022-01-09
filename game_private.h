#ifndef __GAME_PRIVATE_H__
#define __GAME_PRIVATE_H__
#include "history.h"

struct game_s {
	square** tab_cell;
	bool wrapping;
	uint height;  // i in general
	uint width;   // j in general
	history hist;
};

int max(int a, int b);

#endif