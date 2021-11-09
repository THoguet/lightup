#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool test_dummy(void){
    return EXIT_SUCCESS;
}

int usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]){
    if (argc == 1){
        usage(argc, argv);
    }
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("dummy", argv[1]) == 0)
        ok = test_dummy();
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