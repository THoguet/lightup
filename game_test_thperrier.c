#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool test_dummy(){
    return EXIT_SUCCESS;
}

void test_game_text(){
    return EXIT_SUCCESS;
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
    else{
        fprintf ("\"%s\" is not a valid test name.\n", argv[1]);
        return EXIT_FAILURE;
    }
    if (success){
        fprintf (stderr, "test \"%s\" finished successfully", argv[1]);
        return EXIT_SUCCESS;
    }
    else {
        fprintf (stderr, "test \"%s\" error ", argv[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}