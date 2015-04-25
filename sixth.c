#include <stdio.h>

int main(int argc, char *argv[]) {

  int i = 0;
  while(i < argc) {
    printf("arg %d: %s \n", i, argv[i]);
    i++;
  }

  char *states[] = {
    "California", "Oregon",
    "Texas", "Washington"
  };

  i = 1;
  while(i < argc) {
    states[3 + i] = argv[i];
    i++;
  }

  int num_states = 4 + (argc - 1);
  i = 0;
  while(i < num_states) {
    printf("state %d: %s \n", i, states[i]);
    i++;
  }

  return 0;
}