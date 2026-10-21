#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

void stress(void);
void stress(void) {
  srand(123);

  ecs_World world = ecs_new_world();

  for (size_t i = 0; i < 100000; i++) {
    int x1 = rand();
    int y1 = rand();

    int x2 = rand();
    int y2 = rand();
    int vx2 = rand();
    int vy2 = rand();

    int x3 = rand();
    int y3 = rand();
    int vx3 = rand();
    int vy3 = rand();
    int ax3 = rand();
    int ay3 = rand();

    EntityID id1 = ecs_spawn(&world, ((A_Pos){
                                         .pos = {.x = x1, .y = y1},
                                     }));

    EntityID id2 = ecs_spawn(&world, ((A_PosVel){
                                         .pos = {.x = x2, .y = y2},
                                         .vel = {.vx = vx2, .vy = vy2},
                                     }));

    EntityID id3 = ecs_spawn(&world, ((A_PosVelAcc){
                                         .pos = {.x = x3, .y = y3},
                                         .vel = {.vx = vx3, .vy = vy3},
                                         .acc = {.ax = ax3, .ay = ay3},
                                     }));

    size_t random = rand() % 3;

    switch (random) {
    case 0: {
      assert(ecs_despawn(&world, id1));

      assert(world.entity_locations[id1].archetype == ECS_ARCHETYPE_NONE);
      assert(
          world.position_velocity.entities[world.entity_locations[id2].row] ==
          id2);
      assert(world.position_velocity_acceleration
                 .entities[world.entity_locations[id3].row] == id3);
      break;
    }
    case 1: {
      assert(ecs_despawn(&world, id2));

      assert(world.position.entities[world.entity_locations[id1].row] == id1);
      assert(world.entity_locations[id2].archetype == ECS_ARCHETYPE_NONE);
      assert(world.position_velocity_acceleration
                 .entities[world.entity_locations[id3].row] == id3);
      break;
    }
    case 2: {
      assert(ecs_despawn(&world, id3));

      assert(world.position.entities[world.entity_locations[id1].row] == id1);
      assert(
          world.position_velocity.entities[world.entity_locations[id2].row] ==
          id2);
      assert(world.entity_locations[id3].archetype == ECS_ARCHETYPE_NONE);
      break;
    }
    }
  }

  ecs_destroy_world(&world);
}
