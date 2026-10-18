#include <indecs.h>

#include <stdio.h>

int main(void) {
  ecs_World world = ecs_new_world();

  ecs_archetype_position_spawn(&world, (Position){
                                           .x = 50,
                                           .y = 10,
                                       });

  ecs_archetype_position_spawn(&world, (Position){
                                           .x = 123,
                                           .y = 1237129,
                                       });

  ecs_archetype_position_spawn(&world, (Position){
                                           .x = 2113,
                                           .y = 212.5,
                                       });

  printf("%f == %f\n", world.position.position[0].x, 50.0);
  printf("%f == %f\n", world.position.position[0].y, 10.0);
  printf("%f == %f\n", world.position.position[1].x, 123.0);
  printf("%f == %f\n", world.position.position[1].y, 1237129.0);
  printf("%f == %f\n", world.position.position[2].x, 2113.0);
  printf("%f == %f\n", world.position.position[2].y, 212.5);

  ecs_destroy_world(&world);

  return 0;
}
