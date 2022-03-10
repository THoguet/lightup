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
 * @param a uint to test
 * @param b uint to test
 * @return the max between a and b
 */
unsigned int max(unsigned int a, unsigned int b);

/**
 * @brief check if the pointer is NULL fprinf and exit else nothing
 *
 * @param p any pointer
 */
void checkPointer(void* p, char* errMessage);

/**
 * @brief test if i and excepted are the same else exit the prog with the message
 *
 * @param i
 * @param excepted
 * @param message to print in stderr
 */
void test_output(int i, int excepted, char* message);

/**
 * @brief Get the nb char from int object
 *
 * @param number
 * @return unsigned int
 */
unsigned int get_nb_char_from_int(unsigned int number);

#endif