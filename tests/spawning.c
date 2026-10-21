#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#define R() rand() % 1000

typedef struct {
  float x1;
  float y1;

  float x2;
  float y2;
  float vx2;
  float vy2;

  float x3;
  float y3;
  float vx3;
  float vy3;
  float ax3;
  float ay3;

  EntityID id1;
  EntityID id2;
  EntityID id3;
} Vals;

void spawning(void);
void spawning(void) {
  srand(123);

  ecs_World world = ecs_new_world();

  Vals allvals[1000] = {0};

  for (size_t i = 0; i < 1000; i++) {
    Vals v = {
        .x1 = R(),
        .y1 = R(),

        .x2 = R(),
        .y2 = R(),
        .vx2 = R(),
        .vy2 = R(),

        .x3 = R(),
        .y3 = R(),
        .vx3 = R(),
        .vy3 = R(),
        .ax3 = R(),
        .ay3 = R(),
    };

    v.id1 = ecs_spawn(&world, ((A_Pos){
                                  .pos = {.x = v.x1, .y = v.y1},
                              }));

    v.id2 = ecs_spawn(&world, ((A_PosVel){
                                  .pos = {.x = v.x2, .y = v.y2},
                                  .vel = {.vx = v.vx2, .vy = v.vy2},
                              }));

    v.id3 = ecs_spawn(&world, ((A_PosVelAcc){
                                  .pos = {.x = v.x3, .y = v.y3},
                                  .vel = {.vx = v.vx3, .vy = v.vy3},
                                  .acc = {.ax = v.ax3, .ay = v.ay3},
                              }));

    allvals[i] = v;
  }

  for (size_t i = 0; i < 1000; i++) {
    Vals v = allvals[i];

    Archetype_Position a1 = world.position;
    size_t row = world.entity_locations[v.id1].row;

    assert(a1.position[row].x == v.x1);
    assert(a1.position[row].y == v.y1);

    Archetype_Position_Velocity a2 = world.position_velocity;
    row = world.entity_locations[v.id2].row;

    assert(a2.position[row].x == v.x2);
    assert(a2.position[row].y == v.y2);
    assert(a2.velocity[row].vx == v.vx2);
    assert(a2.velocity[row].vy == v.vy2);

    Archetype_Position_Velocity_Acceleration a3 =
        world.position_velocity_acceleration;
    row = world.entity_locations[v.id3].row;

    assert(a3.position[row].x == v.x3);
    assert(a3.position[row].y == v.y3);
    assert(a3.velocity[row].vx == v.vx3);
    assert(a3.velocity[row].vy == v.vy3);
    assert(a3.acceleration[row].ax == v.ax3);
    assert(a3.acceleration[row].ay == v.ay3);
  }

  ecs_destroy_world(&world);
}
