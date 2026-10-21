#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

void stress(void);
void stress(void) {
  srand(123);

  ecs_World world = ecs_new_world();

  EntityID ids[100000] = {0};
  //
  // for (size_t i = 0; i < 100000; i++) {
  //   int x = rand();
  //   int y = rand();
  //   int vx = rand();
  //   int vy = rand();
  //   int ax = rand();
  //   int ay = rand();
  //
  //   size_t random = rand() % 3;
  //
  //   switch (random) {
  //   case 0: {
  //     ids[i] = ecs_spawn(&world, ((A_Pos){
  //                                    .pos = {.x = x, .y = y},
  //                                }));
  //     break;
  //   }
  //   case 1: {
  //     ids[i] = ecs_spawn(&world, ((A_PosVel){
  //                                    .pos = {.x = x, .y = y},
  //                                    .vel = {.vx = vx, .vy = vy},
  //                                }));
  //     break;
  //   }
  //   case 2: {
  //     ids[i] = ecs_spawn(&world, ((A_PosVelAcc){
  //                                    .pos = {.x = x, .y = y},
  //                                    .vel = {.vx = vx, .vy = vy},
  //                                    .acc = {.ax = ax, .ay = ay},
  //                                }));
  //
  //     break;
  //   }
  //   }
  //
  //   random = rand() % i;
  //
  //   if (!ecs_despawn(&world, random))
  //     exit(EXIT_FAILURE);
  //
  //   if (i % 100 == 0) {
  //     for (size_t i = 0; 100000; i++) {
  //       // assert(world.entity_locations[random].archetype ==
  //       // ECS_ARCHETYPE_NONE);
  //       assert(
  //           world.position_velocity.entities[world.entity_locations[id2].row]
  //           == id2);
  //       assert(world.position_velocity_acceleration
  //                  .entities[world.entity_locations[id3].row] == id3);
  //     }
  //   }
  // }

  ecs_destroy_world(&world);
}
