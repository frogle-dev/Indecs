#include <indecs.h>
#include <stdio.h>

int main(void) {
  ecs_World world = ecs_new_world();

  ecs_spawn(&world, ((A_Pos){.pos = (Position){
                                 .x = 50,
                                 .y = 10,
                             }}));

  ecs_spawn(&world, ((A_PosVel){.pos =
                                    (Position){
                                        .x = 2113,
                                        .y = 212.5,
                                    },
                                .vel = (Velocity){
                                    .vx = 100,
                                    .vy = 54.123,
                                }}));

  ecs_spawn(&world, ((A_PosVelAcc){.pos =
                                       (Position){
                                           .x = 78,
                                           .y = 91,
                                       },
                                   .vel =
                                       (Velocity){
                                           .vx = 12.74,
                                           .vy = 978.3,
                                       },
                                   .acc = (Acceleration){
                                       .ax = 12.2,
                                       .ay = 2.3,
                                   }}));

  printf("%f == %f\n", world.position.position[0].x, 50.0);
  printf("%f == %f\n", world.position.position[0].y, 10.0);

  printf("%f == %f\n", world.position_velocity.position[0].x, 2113.0);
  printf("%f == %f\n", world.position_velocity.position[0].y, 212.5);
  printf("%f == %f\n", world.position_velocity.velocity[0].vx, 100.0);
  printf("%f == %f\n", world.position_velocity.velocity[0].vy, 54.123);

  printf("%f == %f\n", world.position_velocity_acceleration.position[0].x, 78.0);
  printf("%f == %f\n", world.position_velocity_acceleration.position[0].x, 91.0);
  printf("%f == %f\n", world.position_velocity_acceleration.velocity[0].vx, 12.74);
  printf("%f == %f\n", world.position_velocity_acceleration.velocity[0].vy, 978.3);
  printf("%f == %f\n", world.position_velocity_acceleration.acceleration[0].ax, 12.2);
  printf("%f == %f\n", world.position_velocity_acceleration.acceleration[0].ay, 2.3);

  ecs_destroy_world(&world);

  return 0;
}
