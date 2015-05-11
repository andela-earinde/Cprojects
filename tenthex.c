//rewriting the tenth solution
//to not use pointers and malloc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person{
  char *name;
  int age;
  int height;
  int weight;
};

struct Person Person_create(char *name, int age, int height, int weight) {
  
  struct Person who;

  who.name = strdup(name);
  who.age = age;
  who.height = height;
  who.weight = weight;

  return who;
}

void Person_destroy(struct Person who) {

  free(who.name);
}

void Person_print(struct Person who) {
  printf("Name: %s \n", who.name);
  printf("\tAge: %d\n", who.age);
  printf("\theight: %d\n", who.height);
  printf("\tweight: %d\n", who.weight);
}

int main(int argc, char *argv[]) {

  struct Person joe = Person_create("Joe", 23, 5, 30);
  struct Person bob = Person_create("Bob", 24, 9, 55);
  
  Person_print(joe);
  Person_print(bob);

  Person_destroy(joe);
  Person_destroy(bob);

  return 0;
}


