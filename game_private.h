#ifndef __GAME_PRIVATE_H__
#define __GAME_PRIVATE_H__
#include "history.h"

struct game_s {
	// tab of tab of square (eg: tab_cell[i][j])
	square** tab_cell;
	bool wrapping;
	uint height;  // i in general
	uint width;   // j in general
	// case of history
	history hist;
};

/**
 * @brief return the max between a and b
 *
 * @param a int to test
 * @param b int to test
 * @return the max between a and b
 */
int max(int a, int b);

/**
 * @brief check if the pointer is NULL fprinf and exit else nothing
 *
 * @param p any pointer
 */
void checkPointer(void* p, char* errMessage);

#endif