#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "game_aux.h"


/* ********** DUMMY ********** */
bool test_dummy(){
    return true;
}

/* ********** game_new_empty ********** */
bool test_game_new_empty(){
    game g = game_new_empty();
    assert(g);
    for (int i = 0; i < DEFAULT_SIZE; i++){
        for (int j = 0; j < DEFAULT_SIZE; j++){
            if (game_get_flags(g, i, j) != S_BLANK){
                game_delete(g);
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

/* ********** game_copy ********** */
bool test_game_copy(){
    game g1 = game_new_empty();
    game g2 = game_copy(g1);
    assert(g2);
    if (game_equal(g1, g2) == true){
        game_delete(g1);
        game_delete(g2);
        return true;
    }
    game_delete(g1);
    game_delete(g2);
    return true;
}

void usage(int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]){
    bool success = false;
    if (argc != 2){
        usage(argc, argv);
    }
    else if (strcmp("dummy", argv[1]) == 0){
        success = test_dummy();
    }
    else if (strcmp("game_new_empty", argv[1]) == 0){
        success = test_game_new_empty();
    }
    else if (strcmp("game_copy", argv[1]) == 0){
        success = test_game_copy();
    }
    else{
        fprintf (stderr, "test \"%s\" is not a valid test name.\n", argv[1]);
        return EXIT_FAILURE;
    }
    if (success){
        fprintf (stderr, "test \"%s\" finished successfully\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else {
        fprintf (stderr, "test \"%s\" error\n", argv[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}