#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "game_aux.h"

bool test_dummy(void){
    return EXIT_SUCCESS;
}

void usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ************** game_get_square ************** */
bool test_game_set_square(void){
  game g = game_new_empty();
  game_set_square(g, 0, 0, S_LIGHTBULB);
  if (game_get_square(g, 0, 0) != S_LIGHTBULB){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLANK);
  if (game_get_square(g, 0, 0) != S_BLANK){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_MARK);
  if (game_get_square(g, 0, 0) != S_MARK){
    game_delete(g);
    return false;
  }  
  game_set_square(g, 0, 0, S_BLACK);
  if (game_get_square(g, 0, 0) != S_BLACK){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACK0);
  if (game_get_square(g, 0, 0) != S_BLACK0){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACK1);
  if (game_get_square(g, 0, 0) != S_BLACK1){
    game_delete(g);
    return false;
  }  
  game_set_square(g, 0, 0, S_BLACK2);
  if (game_get_square(g, 0, 0) != S_BLACK2){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLACK3);
  if (game_get_square(g, 0, 0) != S_BLACK3){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLACK4);
  if (game_get_square(g, 0, 0) != S_BLACK4){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACKU);
  if (game_get_square(g, 0, 0) != S_BLACKU){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_F_LIGHTED);
  if (game_get_square(g, 0, 0) != F_LIGHTED){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, F_ERROR);
  if (game_get_square(g, 0, 0) != F_ERROR){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;                           
}

/* ************** game_get_state ************** */
bool test_game_get_state(void){
  game g = game_new_empty();
  game_set_square(g, 0, 0, S_LIGHTBULB);
  if (game_get_state(g, 0, 0) != S_LIGHTBULB){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLANK);
  if (game_get_state(g, 0, 0) != S_BLANK){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_MARK);
  if (game_get_state(g, 0, 0) != S_MARK){
    game_delete(g);
    return false;
  }  
  game_set_square(g, 0, 0, S_BLACK);
  if (game_get_state(g, 0, 0) != S_BLACK){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACK0);
  if (game_get_state(g, 0, 0) != S_BLACK0){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACK1);
  if (game_get_state(g, 0, 0) != S_BLACK1){
    game_delete(g);
    return false;
  }  
  game_set_square(g, 0, 0, S_BLACK2);
  if (game_get_state(g, 0, 0) != S_BLACK2){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLACK3);
  if (game_get_state(g, 0, 0) != S_BLACK3){
    game_delete(g);
    return false;
  }
  game_set_square(g, 0, 0, S_BLACK4);
  if (game_get_state(g, 0, 0) != S_BLACK4){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, S_BLACKU);
  if (game_get_state(g, 0, 0) != S_BLACKU){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;   
}

/* ************** game_get_flags ************** */
bool test_game_get_flags(void){
  game g = game_new_empty();
  game_set_square(g, 0, 0, S_F_LIGHTED);
  if (game_get_flags(g, 0, 0) != F_LIGHTED){
    game_delete(g);
    return false;
  } 
  game_set_square(g, 0, 0, F_ERROR);
  if (game_get_flags(g, 0, 0) != F_ERROR){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;      
}

/* ************** game_is_marked ************** */
bool test_game_is_marked(void){
  game g = game_new_empty();
  game_set_square(g, 0, 0, S_MARK);
  if ((game_get_square(g, 0, 0) == S_MARK) && game_is_marked(g, 0, 0)){
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

/* ************** game_is_lighted ************** */
bool test_game_is_marked(void){
  game g = game_new_empty();
  game_set_square(g, 0, 0, F_LIGHTED);
  if ((game_get_square(g, 0, 0) == F_LIGHTED) && game_is_lighted(g, 0, 0)){
    game_delete(g);
    return true;
  }
  game_delete(g);
  return false;
}

int main(int argc, char * argv[]){
  if (argc != 2){
    usage(argc, argv);
  }
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("game_get_square", argv[1]) == 0){
    success = test_game_get_square();
  }
  else if (strcmp("game_get_state", argv[1]) == 0){
    success = test_game_get_state();
  }
  else if (strcmp("game_get_falgs", argv[1]) == 0){
    success = test_game_get_flags();
  }
  else if (strcmp("game_is_marked", argv[1]) == 0){
    success = test_game_is_marked();
  }
  else if (strcmp("game_is_lighted", argv[1]) == 0){
    success = test_game_is_lighted();
  }                            
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (!ok){
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  }
  else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }  
}