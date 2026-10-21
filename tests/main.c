#include <stdio.h>

#include "despawning.c"
#include "querying.c"
#include "spawning.c"
#include "stress.c"

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

  start = clock();
  querying();
  end = clock();
  printf("Running querying test... %f seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  start = clock();
  stress();
  end = clock();
  printf("Running stress test... %f seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
