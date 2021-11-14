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
	game g = game_default_solution();
	game_update_flags(g);
	assert(game_is_over(g));
	g = game_default();
	game_update_flags(g);
	assert(!game_is_over(g));
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
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK,S_BLACK0,S_BLACK1,S_BLACK2,S_BLACK3,S_BLACK4,S_BLACKU,F_ERROR};
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
	square tab[DEFAULT_SIZE*DEFAULT_SIZE];
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK0,S_BLACK1,S_BLACK2,S_BLACK3,S_BLACK4,S_BLACKU};
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
	square list[] = {S_BLANK,S_LIGHTBULB,S_MARK,S_BLACK,S_BLACK0,S_BLACK1,S_BLACK2,S_BLACK3,S_BLACK4,S_BLACKU,F_ERROR};
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

int checklightbulb(game g, uint i, uint j, bool wall){
	square flags = game_get_flags(g,i,j);
	square state = game_get_state(g,i,j);
	//test if the current case is a lightbulb and if it has been well updated if true we can break the loop because every others cases wont be lighted by the bulb we are testing
	if (game_is_lightbulb(g,i,j)){
		if (/*test if this lightbulb is well updated*/ flags == S_BLANK || flags == F_ERROR){
			//printf("156\n");
			return 0;
		}
		return -1;
	}
	//if no wall(s) before test if the light of the lightbulb has been well updated
	if (!wall){
		
		//test if the current case is not lighted
		if (flags != F_LIGHTED){
			//printf("170\n");
			return 0;							
		}
		
	}
	else{
		if (/*test if the current case is lighted*/ flags == F_LIGHTED){
			//printf("i:%d j:%d\n",i,j);
			for (int i2 = i-1 ; i2 >= 0; i2--){
				flags = game_get_flags(g,i2,j);
				state = game_get_state(g,i2,j);
				//printf("i2:%d i2state:%d i2flags:%d\n",i2,state,flags);
				if (state == S_LIGHTBULB){
					return -1;
				}
				if (flags != F_LIGHTED){
					break;
				}
			}
			for (int i2 = i+1 ; i2 < DEFAULT_SIZE; i2++){
				flags = game_get_flags(g,i2,j);
				state = game_get_state(g,i2,j);
				if (state == S_LIGHTBULB){
					return -1;
				}
				if (flags != F_LIGHTED){
					break;
				}
			}
			for (int j2 = j-1 ; j2 >= 0; j2--){
				flags = game_get_flags(g,i,j2);
				state = game_get_state(g,i,j2);
				if (state == S_LIGHTBULB){
					return -1;
				}
				if (flags != F_LIGHTED){
					break;
				}
			}
			for (int j2 = j+1 ; j2 < DEFAULT_SIZE; j2++){
				flags = game_get_flags(g,i,j2);
				state = game_get_state(g,i,j2);
				if (state == S_LIGHTBULB){
					return -1;
				}
				if (flags != F_LIGHTED){
					break;
				}
			}
			//printf("220\n");
			return 0;
		}
	}
	return 1;
}

bool check_update(game g){
	uint size;
	square choice[4];
	uint lb;
	uint notempty;
	bool res;
	bool wall;
	for (uint i = 0; i < DEFAULT_SIZE; i++){
		for (uint j = 0; j< DEFAULT_SIZE; j++){
			if (game_is_lightbulb(g,i,j)){
				square flags = game_get_flags(g,i,j);
				if (/*test if this lightbulb is well updated*/ flags == S_BLANK || flags == F_ERROR){
					//printf("196\n");
					return false;
				}
				//loop checking every case before i
				wall = false;
				for (int k = i-1; k > 0; k--){
					if (/*test if current case is a wall*/ game_is_black(g,k,j)){
						wall = true;
					}
					else {
						int tmp = checklightbulb(g,k,j,wall);
						if (tmp == -1){
							break;
						}
						if (!tmp){
							return tmp;
						}
					}
				}
				wall = false;
				for (int k = i+1; k < DEFAULT_SIZE; k++){
					if (/*test if current case is a wall*/ game_is_black(g,k,j)){
						wall = true;
					}
					else {
						int tmp = checklightbulb(g,k,j,wall);
						if (tmp == -1){
							break;
						}
						if (!tmp){
							return tmp;
						}
					}
				}
				wall = false;
				for (int k = j-1; k > 0; k--){
					if (/*test if current case is a wall*/ game_is_black(g,i,k)){
						wall = true;
					}
					else {
						int tmp = checklightbulb(g,i,k,wall);
						if (tmp == -1){
							break;
						}
						if (!tmp){
							return tmp;
						}
					}
				}
				wall = false;
				for (int k = j+1; k < DEFAULT_SIZE; k++){
					if (/*test if current case is a wall*/ game_is_black(g,i,k)){
						wall = true;
					}
					else {
						int tmp = checklightbulb(g,i,k,wall);
						if (tmp == -1){
							break;
						}
						if (!tmp){
							return tmp;
						}
					}
				}
			}
			else if (/*test if current case is a wall*/game_is_black(g,i,j)){
				if (game_get_black_number(g,i,j) != -1){
					//lookup for lightbulb and emptycells
					size = 0;
					lb = 0;
					notempty = 0;
					res = true;
					if (i > 0){
						choice[size] = game_get_square(g,i-1,j);
						size++;
					}
					else{
						notempty++;
					}
					if (i < DEFAULT_SIZE-1){
						choice[size] = game_get_square(g,i+1,j);
						size++;
					}
					else{
						notempty++;
					}
					if (j > 0){
						choice[size] = game_get_square(g,i,j-1);
						size++;
					}
					else{
						notempty++;
					}
					if (j < DEFAULT_SIZE-1){
						choice[size] = game_get_square(g,i,j+1);
						size++;
					}
					else{
						notempty++;
					}
					for (int l = 0 ; l < size ; l++){
						if (choice[l] == (S_LIGHTBULB|F_LIGHTED) || choice[l] == (S_LIGHTBULB|F_LIGHTED|F_ERROR)){
							lb++;
						}
						else if (choice[l] != S_BLANK){
							notempty++;
						}
					}
					if (lb > game_get_black_number(g,i,j)){
						res = false;
					}
					//look if there is enough empty cell around the wall
					else if (notempty > abs(game_get_black_number(g,i,j)-4)){
						res = false;
					}
					if (game_get_flags(g,i,j) == F_ERROR){
						res = !res;
					}
					if (!res){
						//printf("386\n");
						//printf("lb:%d notempty:%d i:%d j:%d state : %d, flags: %d\n",lb,notempty,i,j,game_get_state(g,i,j),game_get_flags(g,i,j));
						return res;
					}
				}
			}
		}
	}
	return true;
}

// a > 2 = temps d'exec > 3 min et expo
bool brutforce(game g,int a){
	square list[] = {S_LIGHTBULB,S_MARK,S_BLACK0,S_BLACK1,S_BLACK2,S_BLACK3,S_BLACK4,S_BLACKU,S_BLANK};
	uint * deldup = (uint *) calloc(DEFAULT_SIZE*DEFAULT_SIZE,sizeof(uint));
	for (uint i = 0; i < DEFAULT_SIZE; i++){
		for (uint j = 0 ;j < DEFAULT_SIZE; j++){
			if (!deldup[i+j]){
				if (game_get_state(g,i,j) == S_BLANK){
					for (uint k = 0 ; k < sizeof(list)/sizeof(list[0]); k++){
						game_set_square(g,i,j,list[k]);
						deldup[i+j] = 1;
						game_update_flags(g);
						// game_print(g);
						if (!check_update(g)){
							game_print(g);
							//printf("i:%d j:%d state : %d, flags: %d\n",i,j,game_get_state(g,i,j),game_get_flags(g,i,j));
							free(deldup);
							return false;
						}
						if (a > 0){
							if (!brutforce(g,a-1)){
								//printf("341\n");
								free(deldup);
								return false;
							}
						}
					}
				}
			}
		}
	}
	free(deldup);
	return true;
}

bool test_game_update_flags(void){
	game g = game_new_empty();
	if (!brutforce(g,3)){
		game_delete(g);
		return EXIT_FAILURE;
	}
	game_delete(g);
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