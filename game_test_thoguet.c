#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "game_aux.h"
typedef unsigned int uint;

/* ********** DUMMY ********** */
bool test_dummy(void){
	return EXIT_SUCCESS;
}

/* ******* game_is_over ******* */

bool test_game_is_over(void){
	square tab[49] = 
	{
		S_LIGHTBULB,S_BLANK    ,S_BLACK1   ,S_LIGHTBULB,S_BLANK    ,S_BLANK    ,S_BLANK    ,
		S_BLANK    ,S_LIGHTBULB,S_BLACK2   ,S_BLANK    ,S_BLANK    ,S_BLANK    ,S_LIGHTBULB,
		S_BLANK    ,S_BLANK    ,S_LIGHTBULB,S_BLANK    ,S_BLANK    ,S_BLACKU   ,S_BLACK2   ,
		S_BLANK    ,S_BLANK    ,S_BLANK    ,S_BLANK    ,S_BLANK    ,S_BLANK    ,S_LIGHTBULB,
		S_BLACK1   ,S_BLACKU   ,S_BLANK    ,S_BLANK    ,S_LIGHTBULB,S_BLANK    ,S_BLANK    ,
		S_LIGHTBULB,S_BLANK    ,S_BLANK    ,S_BLANK    ,S_BLACK2   ,S_LIGHTBULB,S_BLANK    ,
		S_BLANK    ,S_LIGHTBULB,S_BLANK    ,S_BLANK    ,S_BLACKU   ,S_BLANK    ,S_BLANK
	};
	game g = game_new(tab);
	game_update_flags(g);
	assert(game_is_over(g));
	game_delete(g);
	return EXIT_SUCCESS;
}

/* ******* game_restart ******* */

bool test_game_restart(void){
	game g = game_default_solution();
	game g1 = game_default();
	game_restart(g);
	assert(game_equal(g,g1));
	game_delete(g);
	game_delete(g1);
	return EXIT_SUCCESS;
}

/* ********* game_new ********* */

bool test_game_new(void){
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	game g;
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK,S_BLACK0,S_BLACK,S_BLACK1,S_BLACK2,S_BLACK2,S_BLACK4,S_BLACKU,S_BLACKU,F_ERROR};
	for (uint i = 0; i < sizeof(list)/sizeof(list[0]) ; i ++){
		g = NULL;
		for (uint j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = list[i];
		}
		printf("%d\n",i);
		g = game_new(tab);
		assert(g);
		game_delete(g);
	}
	return EXIT_SUCCESS;
}

/* ******** game_print ******** */

// est ce qu'il faut faire avec un tab invalide ?

bool test_game_print(void){
	game_print(NULL);
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK,S_BLACK0,S_BLACK,S_BLACK1,S_BLACK2,S_BLACK2,S_BLACK4,S_BLACKU,S_BLACKU,F_ERROR};
	game g;
	for (uint i = 0; i < sizeof(list)/sizeof(list[0]) ; i ++){
		for (uint j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = list[i];
		}
		printf("%d\n",i);
		g = game_new(tab);
		game_print(g);
		game_delete(g);
	}
	return EXIT_SUCCESS;
}

/* ******* game_get_square ******* */

bool test_game_get_square(void){
	game g;
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK,S_BLACK0,S_BLACK,S_BLACK1,S_BLACK2,S_BLACK2,S_BLACK4,S_BLACKU,S_BLACKU,F_ERROR};
	for (uint i = 0; i < sizeof(list)/sizeof(list[0]) ; i ++){
		for (uint j = 0; j < DEFAULT_SIZE*DEFAULT_SIZE; j++){
			tab[j] = list[i];
			g = game_new(tab);
			printf("[%d, %d] %d %d\n",j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1)),game_get_square(g,j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1))),tab[j]);
			assert(game_get_square(g,j/(DEFAULT_SIZE),j-(j/(DEFAULT_SIZE))-(j/(DEFAULT_SIZE)*(DEFAULT_SIZE-1))) == tab[j]);
			game_delete(g);
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
	game_delete(g);
	return EXIT_SUCCESS;
}

/* *** game_default_solution *** */

bool test_game_default_solution(void){
	game g = game_default_solution();
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
	game_delete(g);
	return EXIT_SUCCESS;
}

/* **** game_update_flags **** */

bool test_game_update_flags(void){
	game g = game_new_empty();
	game_update_flags(g);
	for (int i = 0; i < DEFAULT_SIZE; i++){
		
	}
	return EXIT_SUCCESS;
}

bool check_update(game g){
	game tab_bulb[DEFAULT_SIZE*DEFAULT_SIZE];
	uint size;
	square choice[4];
	uint lb;
	uint notempty;
	bool res;
	for (uint i = 0; i < DEFAULT_SIZE; i++){
		for (uint j = 0; j< DEFAULT_SIZE; j++){
			square state = game_get_state(g,i,j);
			if (/*test if the current case is a lightbulb*/(state == S_LIGHTBULB) || (state == (S_LIGHTBULB|F_ERROR)) | (state == (S_LIGHTBULB|F_LIGHTED)) | (state == (S_LIGHTBULB|F_ERROR|F_LIGHTED))){
				bool wall = false;
				//loop checking every case before i
				for (int k = i-1; k > 0; k--){
					square state2 = game_get_state(g,k,j);
					if (/*test if current case is a wall*/state2 == S_BLACK0 || state2 == (S_BLACK0|F_ERROR) || state2 == S_BLACK1 || state2 == (S_BLACK1|F_ERROR) || state2 == S_BLACK2 || state2 == (S_BLACK2|F_ERROR) || state2 == S_BLACK3 || state2 == (S_BLACK3|F_ERROR) || state2 == S_BLACK4 || state2 == (S_BLACK4|F_ERROR) || state2 == S_BLACKU || state2 == (S_BLACKU|F_ERROR)){
						wall = true;
						if (state2 != S_BLACKU && state2 != (S_BLACKU|F_ERROR)){
							//lookup for lightbulb and emptycells
							size = 0;
							lb = 0;
							notempty = 0;
							res = true;
							if (k > 0){
								choice[size] = game_get_state(g,k-1,j);
								size++;
							}
							else{
								notempty++;
							}
							if (k < DEFAULT_SIZE-1){
								choice[size] = game_get_state(g,k+1,j);
								size++;
							}
							else{
								notempty++;
							}
							if (j > 0){
								choice[size] = game_get_state(g,k,j-1);
								size++;
							}
							else{
								notempty++;
							}
							if (j < DEFAULT_SIZE-1){
								choice[size] = game_get_state(g,k,j+1);
								size++;
							}
							else{
								notempty++;
							}
							for (int l = 0 ; l < size ; l++){
								if (choice[l] == S_LIGHTBULB || choice[l] == (S_LIGHTBULB|F_ERROR) || choice[l] == (S_LIGHTBULB|F_LIGHTED) || choice[l] == (S_LIGHTBULB|F_ERROR|F_LIGHTED)){
									lb++;
								}
								else if (choice[l] != S_BLANK){
									notempty++;
								}
							}
							if (lb+8 > state2){
								res = false;
							}
							//look if there is enough empty cell around the wall
							else if (notempty > abs(state2-S_BLACK4)){
								res = false;
							}

							if (!(state2 == S_BLACK0 || state2 == S_BLACK1 || state2 == S_BLACK2 || state2 == S_BLACK3 || state2 == S_BLACK4)){
								res = !res;
							}
							if (!res){
								return res;
							}
						}
					}
					//if no wall(s) before test if the light of the lightbulb has been well updated
					else if (!wall){
						//test if the current case is a lightbulb and if it has been well updated if true we can break the loop because every others cases wont be lighted by the bulb we are testing
						if (state2 == (S_LIGHTBULB|F_ERROR|F_LIGHTED)){
							break;
						}
						if ((state2 == S_LIGHTBULB) || (state2 == (S_LIGHTBULB|F_ERROR)) | (state2 == (S_LIGHTBULB|F_LIGHTED))){
							return false;
						}
						//test if the current case isn't lighted or ... if not the lightbulb isn't well updated
						if (/*test if current case isn't lighted*/!(state2 == F_LIGHTED || /*or isn't a mark*/!((state2 == S_MARK) || (state2 == (S_MARK|F_ERROR)) | (state2 == (S_MARK|F_LIGHTED))))){
							
							return false;
						}
					}
					else{
						if (/*test if the current case is lighted*/state2 == F_LIGHTED || state2 == (F_LIGHTED|S_MARK) || (state2 == (S_LIGHTBULB|F_ERROR)) | state2 == (S_LIGHTBULB|F_LIGHTED) | state2 == (S_LIGHTBULB|F_ERROR|F_LIGHTED)){
							return false;
						}
					}
				}
			}
		}
	}
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
	bool ok = true;
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
	else if (strcmp("game_restart", argv[1]) == 0)
		ok = test_game_restart();
	else if (strcmp("game_get_square", argv[1]) == 0)
		ok = test_game_get_square();
	else if (strcmp("game_is_over", argv[1]) == 0)
		ok = test_game_is_over();
	else if (strcmp("game_update_flags", argv[1]) == 0)
		ok = test_game_update_flags();
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