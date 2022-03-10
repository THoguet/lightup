#include <stdio.h>
#include <stdlib.h>

unsigned int max(unsigned int a, unsigned int b) {
	if (a > b)
		return a;
	return b;
}

unsigned int min(unsigned int a, unsigned int b) {
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

unsigned int get_nb_char_from_int(unsigned int number) {
	unsigned int res = 0;
	do {
		res++;
		number = number / 10;
	} while (number != 0);
	return res;
}