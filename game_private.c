#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
	if (a > b)
		return a;
	return b;
}

void checkPointer(void* p) {
	if (!p) {
		fprintf(stderr, "Error on given pointer.\n");
		exit(EXIT_FAILURE);
	}
}