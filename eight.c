#include <stdio.h>
#include <ctype.h>
#include <string.h>

void print_letters(char arg[], int str_length) {

  int i = 0;

  for(i = 0; i < str_length; i++) {
    char ch = arg[i];

    if(isalpha(ch) || isblank(ch)) {
      printf("'%c' == '%d' \n", ch, ch);
    }
  }
}

int main(int argc, char *argv[]) {
  
  int i = 0;

  for(i = 0; i < argc; i++) {
    print_letters(argv[i], strlen(argv[i]));
  }
  
  return 0;
}