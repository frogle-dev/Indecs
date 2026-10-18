#include <indecs.h>

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

  ecs_destroy_world(&world);

  return 0;
}
