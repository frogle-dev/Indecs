#include <stdio.h>

#include "despawning.c"
#include "spawning.c"

int main(void) {
  printf("Running spawning test...\n");

  clock_t start = clock();
  spawning();
  clock_t end = clock();
  printf("took: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  printf("\nRunning despawning test...\n");

  start = clock();
  despawning();
  end = clock();
  printf("took: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
