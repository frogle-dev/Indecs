#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

void spawning(void) {
  srand(time(NULL));

  ecs_World world = ecs_new_world();

  for (size_t i = 0; i < 1000; i++) {
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

    Archetype_Position a1 = world.position;
    size_t row = world.entity_locations[id1].row;

    assert(a1.position[row].x == x1);
    assert(a1.position[row].y == y1);

    Archetype_Position_Velocity a2 = world.position_velocity;
    row = world.entity_locations[id2].row;

    assert(a2.position[row].x == x2);
    assert(a2.position[row].y == y2);
    assert(a2.velocity[row].vx == vx2);
    assert(a2.velocity[row].vy == vy2);

    Archetype_Position_Velocity_Acceleration a3 =
        world.position_velocity_acceleration;
    row = world.entity_locations[id3].row;

    assert(a3.position[row].x == x3);
    assert(a3.position[row].y == y3);
    assert(a3.velocity[row].vx == vx3);
    assert(a3.velocity[row].vy == vy3);
    assert(a3.acceleration[row].ax == ax3);
    assert(a3.acceleration[row].ay == ay3);
  }

  ecs_destroy_world(&world);
}
