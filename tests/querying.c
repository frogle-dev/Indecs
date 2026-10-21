#include <assert.h>
#include <indecs.h>
#include <stdio.h>

void querying(void);
void querying(void) {
  ecs_World world = ecs_new_world();

  // test that nothing gets iterated in empty world
  MovementIter iter = ecs_query_movement(&world);

  while (ecs_iter_movement(&world, &iter)) {
  }

  // pos archetypes wont be queried
  ecs_spawn(&world, ((A_Pos){
                        .pos = {.x = 1, .y = 2},
                    }));

  ecs_spawn(&world, ((A_Pos){
                        .pos = {.x = 3, .y = 4},
                    }));

  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 1, .y = 2},
                        .vel = {.vx = 1, .vy = 2},
                    }));

  // pos_vel and pos_vel_acc archetypes will be queried
  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 3, .y = 4},
                        .vel = {.vx = 3, .vy = 4},
                    }));

  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 5, .y = 6},
                        .vel = {.vx = 5, .vy = 6},
                    }));

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 1, .y = 2},
                                   .vel = {.vx = 1, .vy = 2},
                                   .acc = {.ax = 1, .ay = 2}}));

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 3, .y = 2},
                                   .vel = {.vx = 3, .vy = 4},
                                   .acc = {.ax = 3, .ay = 4}}));

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 5, .y = 6},
                                   .vel = {.vx = 5, .vy = 6},
                                   .acc = {.ax = 5, .ay = 6}}));

  // querying
  iter = ecs_query_movement(&world);

  assert(*iter.sizes[0] == world.position_velocity.size);
  assert(*iter.sizes[1] == world.position_velocity_acceleration.size);

  // assert(query.pos[0] == world.position_velocity.position);
  // assert(query.vel[0] == world.position_velocity.velocity);
  // assert(query.pos[1] == world.position_velocity_acceleration.position);
  // assert(query.vel[1] == world.position_velocity_acceleration.velocity);

  while (ecs_iter_movement(&world, &iter)) {
    size_t row = world.entity_locations[iter.entity].row;

    switch (iter.archetype_i) {
    case 0:
      assert(iter.pos->x == world.position_velocity.position[row].x);
      assert(iter.pos->y == world.position_velocity.position[row].y);
      assert(iter.vel->vx == world.position_velocity.velocity[row].vx);
      assert(iter.vel->vy == world.position_velocity.velocity[row].vy);
      break;
    case 1:
      assert(iter.pos->x ==
             world.position_velocity_acceleration.position[row].x);
      assert(iter.pos->y ==
             world.position_velocity_acceleration.position[row].y);
      assert(iter.vel->vx ==
             world.position_velocity_acceleration.velocity[row].vx);
      assert(iter.vel->vy ==
             world.position_velocity_acceleration.velocity[row].vy);
      break;
    };
  }

  ecs_destroy_world(&world);
}
