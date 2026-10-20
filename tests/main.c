#include <stdio.h>

#include "despawning.c"
#include "spawning.c"

int main(void) {
  clock_t start = clock();
  spawning();
  clock_t end = clock();
  printf("Running spawning test... %f seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  start = clock();
  despawning();
  end = clock();
  printf("Running despawning test... %f seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
