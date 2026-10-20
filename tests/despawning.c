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

  assert(world.position.entities[0] == 0);
  assert(world.position.entities[1] == 1);
  assert(world.position.entities[2] == 2);

  // checking if swapping worked, 1st element should be id3
  ecs_despawn(&world, id1);

  assert(world.position.entities[0] == 2);
  assert(world.position.entities[1] == 1);

  assert(world.entity_locations[id1].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id3].row == 0);
  assert(world.position.entities[0] == id3);
  assert(world.position.position[0].x == 5);
  assert(world.position.position[0].y == 6);

  // storage should be empty
  ecs_despawn(&world, id2);
  ecs_despawn(&world, id3);

  assert(world.entity_locations[id1].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id2].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id3].archetype == ECS_ARCHETYPE_NONE);
  assert(world.position.size == 0);
  assert(world.position.capacity == 1 * 2 * 2);

  id1 = ecs_spawn(&world, ((A_PosVel){
                              .pos = {.x = 1, .y = 2},
                              .vel = {.vx = 1, .vy = 2},
                          }));

  id2 = ecs_spawn(&world, ((A_PosVel){
                              .pos = {.x = 3, .y = 4},
                              .vel = {.vx = 3, .vy = 4},
                          }));

  id3 = ecs_spawn(&world, ((A_PosVelAcc){
                              .pos = {.x = 5, .y = 6},
                              .vel = {.vx = 5, .vy = 6},
                              .acc = {.ax = 5, .ay = 6},
                          }));

  EntityID id4 = ecs_spawn(&world, ((A_PosVelAcc){
                                       .pos = {.x = 7, .y = 8},
                                       .vel = {.vx = 7, .vy = 8},
                                       .acc = {.ax = 7, .ay = 8},
                                   }));

  ecs_despawn(&world, id1);

  assert(world.entity_locations[id1].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id2].row == 0);
  assert(world.position_velocity.entities[0] == id2);
  assert(world.position_velocity.position[0].x == 3);
  assert(world.position_velocity.position[0].y == 4);
  assert(world.position_velocity.velocity[0].vx == 3);
  assert(world.position_velocity.velocity[0].vy == 4);

  ecs_despawn(&world, id3);

  assert(world.entity_locations[id3].archetype == ECS_ARCHETYPE_NONE);
  assert(world.entity_locations[id4].row == 0);
  assert(world.position_velocity_acceleration.entities[0] == id4);
  assert(world.position_velocity_acceleration.position[0].x == 7);
  assert(world.position_velocity_acceleration.position[0].y == 8);
  assert(world.position_velocity_acceleration.velocity[0].vx == 7);
  assert(world.position_velocity_acceleration.velocity[0].vy == 8);
  assert(world.position_velocity_acceleration.acceleration[0].ax == 7);
  assert(world.position_velocity_acceleration.acceleration[0].ay == 8);

  ecs_destroy_world(&world);
}
