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

/* ********** game_get_black_number ********** */
bool test_game_get_black_number(){
    game g = game_new_empty();
    game_set_square(g, 0, 0, S_BLACK0);
    if (game_get_black_number(g, 0, 0) != 0){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK1);
    if (game_get_black_number(g, 0, 0) != 1){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK2);
    if (game_get_black_number(g, 0, 0) != 2){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK3);
    if (game_get_black_number(g, 0, 0) != 3){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK4);
    if (game_get_black_number(g, 0, 0) != 4){
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** game_is_black ********** */
bool test_game_is_black(){
    game g = game_new_empty();
    if (game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACKU);
    if (!game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    if (!((game_get_square(g, 0, 0) == S_BLACKU) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK0);
    if (!((game_get_square(g, 0, 0) == S_BLACK0) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK1);
    if (!((game_get_square(g, 0, 0) == S_BLACK1) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK2);
    if (!((game_get_square(g, 0, 0) == S_BLACK2) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK3);
    if (!((game_get_square(g, 0, 0) == S_BLACK3) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_BLACK4);
    if (!((game_get_square(g, 0, 0) == S_BLACK4) && game_is_black(g, 0, 0))){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_MARK);
    if ((game_get_square(g, 0, 0) == S_MARK) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_MARK);
    if ((game_get_square(g, 0, 0) == S_MARK) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_LIGHTBULB);
    if ((game_get_square(g, 0, 0) == S_LIGHTBULB) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, S_LIGHTBULB);
    if ((game_get_square(g, 0, 0) == S_LIGHTBULB) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, F_ERROR);
    if ((game_get_square(g, 0, 0) == F_ERROR) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_set_square(g, 0, 0, F_LIGHTED);
    if ((game_get_square(g, 0, 0) == F_LIGHTED) && game_is_black(g, 0, 0)){
        game_delete(g);
        return false;
    }
    game_delete(g);
    return true;
}

/* ********** game_is_lightbulb ********** */
bool test_game_is_lightbulb(){
    game g = game_new_empty();
    game_set_square(g, 0, 0, S_LIGHTBULB);
    if ((game_get_square(g, 0, 0) == S_LIGHTBULB) && game_is_lightbulb(g, 0, 0)){
        game_delete(g);
        return true;
    }
    game_delete(g);
    return false;
}

/* ********** game_is_blank ********** */
bool test_game_is_blank(){
    game g = game_new_empty();
    if (game_is_blank(g, 0, 0)){
        game_delete(g);
        return true;
    }
    game_delete(g);
    return false;
}

/* ********** game_delete ********** */
bool test_game_delete(){
    game g1 = game_new_empty();
    game g2 = game_new_empty();
    game_delete(g1);
    if (game_equal(g1, g2)){
        return false;
    }
    return true;
}

/* ********** game_equal ********** */
bool test_game_equal(){
    bool equal = true;
    game g1 = game_new_empty();
    game g2 = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++){
        for (int j = 0; j < DEFAULT_SIZE; j++){
            if (game_get_square(g1, i, j) != game_get_square(g2, i, j)){
                game_delete(g1);
                game_delete(g2);
                equal = false;
            }
        }
    }
    return (equal == game_equal(g1, g2));
}

/* ********** game_new_empty ********** */
bool test_game_new_empty(){
    game g = game_new_empty();
    assert(g);
    for (int i = 0; i < DEFAULT_SIZE; i++){
        for (int j = 0; j < DEFAULT_SIZE; j++){
            if (game_get_square(g, i, j) != S_BLANK){
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
/* ********** usage ********** */
void usage(int argc, char *argv[]){
    fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
    exit(EXIT_FAILURE);
}
/* ********** main ********** */
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
    else if (strcmp("game_equal", argv[1]) == 0){
        success = test_game_equal();
    }
    else if (strcmp("game_delete", argv[1]) == 0){
        success = test_game_delete();
    }
    else if (strcmp("game_is_blank", argv[1]) == 0){
        success = test_game_is_blank();
    }
    else if (strcmp("game_is_lightbulb", argv[1]) == 0){
        success = test_game_is_lightbulb();
    }
    else if (strcmp("game_is_black", argv[1]) == 0){
        success = test_game_is_black();
    }
    else if (strcmp("game_get_black_number", argv[1]) == 0){
        success = test_game_get_black_number();
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