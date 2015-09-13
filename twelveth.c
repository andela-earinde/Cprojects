#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char* message) {

  if(errno) {
    perror(message);
  }
  else{
    printf("Error: %s\n", message);
  }
  exit(1);
}

typedef int (*compare_cb)(int a, int b);

typedef int *(*sort_function)(int *numbers, int count, compare_cb cmp);

int *bubble_sort(int *numbers, int count, compare_cb cmp) {

  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if(!target) die("Memory error.");

  memcpy(target, numbers, count * sizeof(int));

  for(i = 0; i < count; i++) {
    for(j = 0; j < count - 1; j++) {
      if(cmp(target[j], target[j+1]) > 0) {
        temp = target[j+1];
        target[j+1] = target[j];
        target[j] = temp;
      }
    }
  }
  return target;
}

int *selection_sort(int *numbers, int count, compare_cb cmp) {

  int i = 0;
  int min = 0;
  int temp = 0;
  int *target = malloc(count * sizeof(int));
  if(!target) die("Memory error.");

  memcpy(target, numbers, count * sizeof(int));

  for(i = 0; i < count - 1; i++) {
    min = i;
    for(int j = i + 1; j < count; j++) {
      if(cmp(target[j], target[min]) < 0) {
        min = j;
      }
      temp = target[i];
      target[i] = target[min];
      target[min] = temp;
    }
  }
  return target;
}

int sorted_order(int a, int b) {
  return a - b;
}

int reverse_order(int a, int b) {
  return b - a;
}

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  }
  else {
    return a % b;
  }
}

void test_sorting(int *numbers, int count, compare_cb cmp, sort_function sort) {
  int i = 0;
  int *sorted = sort(numbers, count, cmp);

  if(!sorted) die("Failed to sort as requested");

  for(i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");

  free(sorted);
}

int main(int argc, char *argv[]) {

  if(argc < 2) die("Usage: twelveth 1 3 5 4 2 6");

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if(!numbers) die("Memory error");

  for(i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  test_sorting(numbers, count, sorted_order, selection_sort);
  test_sorting(numbers, count, sorted_order, bubble_sort);
  test_sorting(numbers, count, strange_order, selection_sort);
  test_sorting(numbers, count, strange_order, bubble_sort);

  free(numbers);

  return 0;
}












