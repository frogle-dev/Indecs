#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

void despawning(void);
void despawning(void) {
  srand(time(NULL));

  ecs_World world = ecs_new_world();

  EntityID id1 = ecs_spawn(&world, ((A_Pos){
                                       .pos = {.x = 1, .y = 2},
                                   }));

  EntityID id2 = ecs_spawn(&world, ((A_Pos){
                                       .pos = {.x = 3, .y = 4},
                                   }));

  EntityID id3 = ecs_spawn(&world, ((A_Pos){
                                       .pos = {.x = 5, .y = 6},
                                   }));

  printf("%u %u %u\n", world.position.entities[0], world.position.entities[1],
         world.position.entities[2]);

  // checking if swapping worked, 1st element should be id3
  ecs_despawn(&world, id1);

  printf("%u %u %u\n", world.position.entities[0], world.position.entities[1],
         world.position.entities[2]);

  assert(world.entity_locations[id1].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id3].row == 0);
  assert(world.position.entities[0] == id3);
  assert(world.position.position[0].x == 5);
  assert(world.position.position[0].y == 6);

  // checking if location changing worked
  ecs_despawn(&world, id3);

  printf("%u %u %u\n", world.position.entities[0], world.position.entities[1],
         world.position.entities[2]);

  assert(world.entity_locations[id3].archetype == ECS_ARCHETYPE_NONE);
  // memory shouldnt change, nothing is freed, but dynamic array size is
  // decremented
  assert(world.position.position[2].x == 5);
  assert(world.position.position[2].y == 6);

  ecs_destroy_world(&world);
}
