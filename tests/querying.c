#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include "check.h"

static void only_posvel() {
  ecs_World world = ecs_new_world();

  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 1, .y = 2},
                        .vel = {.vx = 1, .vy = 2},
                    }));

  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 3, .y = 4},
                        .vel = {.vx = 3, .vy = 4},
                    }));

  ecs_spawn(&world, ((A_PosVel){
                        .pos = {.x = 5, .y = 6},
                        .vel = {.vx = 5, .vy = 6},
                    }));

  MovementIter iter = {0};
  while (ecs_iter_movement(&world, &iter)) {
    size_t row = world.entity_locations[iter.entity].row;

    switch (iter.archetype_i) {
    case 0:
      assert(iter.entity == world.position_velocity.entities[row]);
      assert(iter.pos->x == world.position_velocity.position[row].x);
      assert(iter.pos->y == world.position_velocity.position[row].y);
      assert(iter.vel->vx == world.position_velocity.velocity[row].vx);
      assert(iter.vel->vy == world.position_velocity.velocity[row].vy);
      break;
    case 1: // iter should never reach here
      assert(false);
      break;
    };
  }

  ecs_destroy_world(&world);
}

static void only_posvelacc() {
  ecs_World world = ecs_new_world();

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 1, .y = 2},
                                   .vel = {.vx = 1, .vy = 2},
                                   .acc = {.ax = 1, .ay = 2}}));

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 3, .y = 2},
                                   .vel = {.vx = 3, .vy = 4},
                                   .acc = {.ax = 3, .ay = 4}}));

  ecs_spawn(&world, ((A_PosVelAcc){.pos = {.x = 5, .y = 6},
                                   .vel = {.vx = 5, .vy = 6},
                                   .acc = {.ax = 5, .ay = 6}}));

  MovementIter iter = {0};
  while (ecs_iter_movement(&world, &iter)) {
    size_t row = world.entity_locations[iter.entity].row;

    switch (iter.archetype_i) {
    case 0: // iter should never reach here
      assert(false);
      break;
    case 1:
      assert(iter.entity == world.position_velocity_acceleration.entities[row]);
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

static void only_pos() {
  ecs_World world = ecs_new_world();

  ecs_spawn(&world, ((A_Pos){
                        .pos = {.x = 1, .y = 2},
                    }));

  ecs_spawn(&world, ((A_Pos){
                        .pos = {.x = 3, .y = 4},
                    }));

  MovementIter iter = {0};
  while (ecs_iter_movement(&world, &iter)) {
    switch (iter.archetype_i) {
    case 0: // iter should never reach here
      assert(false);
      break;
    case 1: // or here
      assert(false);
      break;
    };
  }

  ecs_destroy_world(&world);
}

void querying(void);
void querying(void) {
  ecs_World world = ecs_new_world();

  // test that nothing gets iterated in empty world
  MovementIter iter = {0};
  CHECK(!ecs_iter_movement(&world, &iter));

  assert(iter.iterations == 0);

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
  iter = (MovementIter){0};
  while (ecs_iter_movement(&world, &iter)) {
    size_t row = world.entity_locations[iter.entity].row;

    switch (iter.archetype_i) {
    case 0:
      assert(iter.entity == world.position_velocity.entities[row]);
      assert(iter.pos->x == world.position_velocity.position[row].x);
      assert(iter.pos->y == world.position_velocity.position[row].y);
      assert(iter.vel->vx == world.position_velocity.velocity[row].vx);
      assert(iter.vel->vy == world.position_velocity.velocity[row].vy);
      break;
    case 1:
      assert(iter.entity == world.position_velocity_acceleration.entities[row]);
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

  assert(iter.iterations == 6);

  only_posvel();
  only_posvelacc();

  ecs_destroy_world(&world);
}
