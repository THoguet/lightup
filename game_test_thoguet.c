#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "game_aux.h"

/* ********** DUMMY ********** */

bool test_dummy(void){
	return EXIT_SUCCESS;
}

/* ********* game_new ********* */

bool test_game_new(void){
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	game g;
	for (int i = 0; i <= 128; i ++){
		g = NULL;
		for (int j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = i;
		}
		printf("%d\n",i);
		g = game_new(tab);
		assert(g);
	}
	return EXIT_SUCCESS;
}

/* ******** game_print ******** */

// est ce qu'il faut faire avec un tab invalide ?

bool test_game_print(void){
	game_print(NULL);
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	for (int i = 0; i <= 128; i ++){
		for (int j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = i;
		}
		printf("%d\n",i);
		game_print(game_new(tab));
	}
	return EXIT_SUCCESS;
}

/* ******* game_get_square ******* */

bool test_game_get_square(void){
	game g = game_new_empty();
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	for (int i = 0; i <= 128; i ++){
		for (int j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = i;
			g = game_new(tab);
			printf("[%d, %d] %d %d\n",j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1)),game_get_square(g,j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1))),tab[j]);
			assert(game_get_square(g,j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1))) == tab[j]);
		}
	}
	return EXIT_SUCCESS;
}

/* ******* game_default ******* */

bool test_game_default(void){
	game g = game_default();
	//a verif aps sur
	assert(g);
	assert(game_get_square(g,0,2) == S_BLACK1);
	assert(game_get_square(g,1,2) == S_BLACK2);
	assert(game_get_square(g,2,5) == S_BLACKU);
	assert(game_get_square(g,2,6) == S_BLACK2);
	assert(game_get_square(g,4,0) == S_BLACK1);
	assert(game_get_square(g,4,1) == S_BLACKU);
	assert(game_get_square(g,5,4) == S_BLACK2);
	assert(game_get_square(g,6,4) == S_BLACKU);
	return EXIT_SUCCESS;
}

/* *** game_default_solution *** */

bool test_game_default_solution(void){
	game g = game_default_solution();
	//a verif aps sur
	assert(g);
	//default
	assert(game_get_square(g,0,2) == S_BLACK1);
	assert(game_get_square(g,1,2) == S_BLACK2);
	assert(game_get_square(g,2,5) == S_BLACKU);
	assert(game_get_square(g,2,6) == S_BLACK2);
	assert(game_get_square(g,4,0) == S_BLACK1);
	assert(game_get_square(g,4,1) == S_BLACKU);
	assert(game_get_square(g,5,4) == S_BLACK2);
	assert(game_get_square(g,6,4) == S_BLACKU);
	//solution
	assert(game_get_square(g,0,0) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,1,1) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,0,3) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,1,6) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,2,2) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,3,6) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,4,4) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,5,0) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,5,5) == (S_LIGHTBULB|F_LIGHTED));
	assert(game_get_square(g,6,1) == (S_LIGHTBULB|F_LIGHTED));
	return EXIT_SUCCESS;
}

/* ********** USAGE ********** */

int usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ********** */

int main(int argc, char * argv[]){
	if (argc == 1){
		usage(argc, argv);
	}
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool ok = false;
	if (strcmp("dummy", argv[1]) == 0)
		ok = test_dummy();
	else if (strcmp("game_print", argv[1]) == 0)
		ok = test_game_print();
	else if (strcmp("game_default", argv[1]) == 0)
		ok = test_game_default();
	else if (strcmp("game_default_solution", argv[1]) == 0)
		ok = test_game_default_solution();
	else if (strcmp("game_new", argv[1]) == 0)
		ok = test_game_new();
	else if (strcmp("game_get_square", argv[1]) == 0)
		ok = test_game_get_square();
	else {
    	fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    	exit(EXIT_FAILURE);
	}
	// print test result
  	if (!ok){
    	fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    	return EXIT_SUCCESS;
  	}
  	else {
    	fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    	return EXIT_FAILURE;
	}
}