#include <assert.h>
#include <indecs.h>
#include <stdio.h>

void querying(void);
void querying(void) {
  ecs_World world = ecs_new_world();

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
  MovementQuery query = ecs_query_movement(&world);

  assert(query.size[0] == world.position_velocity.size);
  assert(query.size[1] == world.position_velocity_acceleration.size);

  assert(query.pos[0] == world.position_velocity.position);
  assert(query.vel[0] == world.position_velocity.velocity);
  assert(query.pos[1] == world.position_velocity_acceleration.position);
  assert(query.vel[1] == world.position_velocity_acceleration.velocity);

  MovementIter iter = {.first_iter = true};

  while (ecs_iter_movement(&query, &iter)) {
    switch (iter.archetype_i) {
    case 0:
      assert(iter.pos->x == world.position_velocity.position[iter.row - 1].x);
      assert(iter.pos->y == world.position_velocity.position[iter.row - 1].y);
      assert(iter.vel->vx == world.position_velocity.velocity[iter.row - 1].vx);
      assert(iter.vel->vy == world.position_velocity.velocity[iter.row - 1].vy);
      break;
    case 1:
      assert(iter.pos->x ==
             world.position_velocity_acceleration.position[iter.row - 1].x);
      assert(iter.pos->y ==
             world.position_velocity_acceleration.position[iter.row - 1].y);
      assert(iter.vel->vx ==
             world.position_velocity_acceleration.velocity[iter.row - 1].vx);
      assert(iter.vel->vy ==
             world.position_velocity_acceleration.velocity[iter.row - 1].vy);
      break;
    };
  }

  ecs_destroy_world(&world);
}
