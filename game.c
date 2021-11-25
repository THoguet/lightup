#include "game.h"

game game_new(square *squares){
    return;
}


game game_new_empty(void){
    return;
}


game game_copy(cgame g){
    return;
}


bool game_equal(cgame g1, cgame g2){
    return;
}


void game_delete(game g){
    return;
}



void game_set_square(game g, uint i, uint j, square s){
    return;
}



square game_get_square(cgame g, uint i, uint j){
    return;
}



square game_get_state(cgame g, uint i, uint j){
    return;
}



square game_get_flags(cgame g, uint i, uint j){
    return;
}



bool game_is_blank(cgame g, uint i, uint j){
    return;
}



bool game_is_lightbulb(cgame g, uint i, uint j){
    return;
}



bool game_is_black(cgame g, uint i, uint j){
    return;
}



int game_get_black_number(cgame g, uint i, uint j){
    return;
}



bool game_is_marked(cgame g, uint i, uint j){
    return;
}



bool game_is_lighted(cgame g, uint i, uint j){
    return;
}



bool game_has_error(cgame g, uint i, uint j){
    return;
}




bool game_check_move(cgame g, uint i, uint j, square s){
    return;
}



void game_play_move(game g, uint i, uint j, square s){
    return;
}



void game_update_flags(game g){
    return;
}



bool game_is_over(cgame g){
    return;
}



void game_restart(game g){
    return;
}

