#include <assert.h>
#include <indecs.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

size_t const NUM_PARTICLES = 100000;
float const WIDTH = 1000;
float const HEIGHT = 1000;

void physics_sim(void);
void physics_sim(void) {
  ecs_World world = ecs_new_world();

  EntityID *particles = malloc(NUM_PARTICLES * sizeof(EntityID));

  for (size_t i = 0; i < NUM_PARTICLES; i++) {
    particles[i] =
        ecs_spawn(&world, ((A_PosVel){
                              .pos =
                                  {
                                      .x = ((float)i / NUM_PARTICLES) * 100,
                                      .y = ((float)i / NUM_PARTICLES) * 100,
                                  },
                              .vel =
                                  {
                                      .vx = ((float)i / NUM_PARTICLES) * 10,
                                      .vy = ((float)i / NUM_PARTICLES) * 10,
                                  },
                          }));
  }

  MovementIter iter = {0};
  while (true) {
    while (ecs_iter_movement(&world, &iter)) {
      iter.pos->x += iter.vel->vx;
      iter.pos->y += iter.vel->vy;

      if (iter.pos->x >= WIDTH | iter.pos->x <= 0)
        iter.vel->vx = -iter.vel->vx;
      if (iter.pos->y >= HEIGHT | iter.pos->y <= 0)
        iter.vel->vy = -iter.vel->vy;
    }
  }

  ecs_destroy_world(&world);
}
