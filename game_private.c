#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
	if (a > b)
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