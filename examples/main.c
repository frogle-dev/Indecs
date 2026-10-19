#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

int main(void) {
  srand(time(NULL));

  ecs_World world = ecs_new_world();

  for (size_t i = 0; i < 10; i++) {
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

    ecs_spawn(&world, ((A_Pos){.pos = (Position){
                                   .x = x1,
                                   .y = y1,
                               }}));

    ecs_spawn(&world, ((A_PosVel){.pos =
                                      (Position){
                                          .x = x2,
                                          .y = y2,
                                      },
                                  .vel = (Velocity){
                                      .vx = vx2,
                                      .vy = vy2,
                                  }}));

    ecs_spawn(&world, ((A_PosVelAcc){.pos =
                                         (Position){
                                             .x = x3,
                                             .y = y3,
                                         },
                                     .vel =
                                         (Velocity){
                                             .vx = vx3,
                                             .vy = vy3,
                                         },
                                     .acc = (Acceleration){
                                         .ax = ax3,
                                         .ay = ay3,
                                     }}));

    assert(world.position.position[i].x == x1);
    assert(world.position.position[i].y == y1);

    assert(world.position_velocity.position[i].x == x2);
    assert(world.position_velocity.position[i].y == y2);
    assert(world.position_velocity.velocity[i].vx == vx2);
    assert(world.position_velocity.velocity[i].vy == vy2);

    assert(world.position_velocity_acceleration.position[i].x == x3);
    assert(world.position_velocity_acceleration.position[i].y == y3);
    assert(world.position_velocity_acceleration.velocity[i].vx == vx3);
    assert(world.position_velocity_acceleration.velocity[i].vy == vy3);
    assert(world.position_velocity_acceleration.acceleration[i].ax == ax3);
    assert(world.position_velocity_acceleration.acceleration[i].ay == ay3);
  }

  ecs_destroy_world(&world);

  return 0;
}
