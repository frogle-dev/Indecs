#include <indecs.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "check.h"

typedef enum {
  STRESS_POS,
  STRESS_POSVEL,
  STRESS_POSVELACC,
} StressKind;

typedef struct {
  bool alive;
  StressKind kind;
  Position pos;
  Velocity vel;
  Acceleration acc;
} StressModel;

// whole numbers below 1000 are exact as float
static float stress_value(void) { return (float)(rand() % 1000); }

static EntityID stress_spawn(ecs_World *world, StressModel *model,
                             size_t model_cap) {
  StressModel m = {.alive = true, .kind = (StressKind)(rand() % 3)};

  m.pos.x = stress_value();
  m.pos.y = stress_value();
  m.vel.vx = stress_value();
  m.vel.vy = stress_value();
  m.acc.ax = stress_value();
  m.acc.ay = stress_value();

  EntityID id = 0;

  switch (m.kind) {
  case STRESS_POS:
    id = ecs_spawn(world, ((A_Pos){.pos = m.pos}));
    break;
  case STRESS_POSVEL:
    id = ecs_spawn(world, ((A_PosVel){.pos = m.pos, .vel = m.vel}));
    break;
  case STRESS_POSVELACC:
    id = ecs_spawn(world,
                   ((A_PosVelAcc){.pos = m.pos, .vel = m.vel, .acc = m.acc}));
    break;
  }

  CHECK(id < model_cap);
  CHECK(!model[id].alive);

  model[id] = m;

  return id;
}

static void stress_check_world(ecs_World const *world, StressModel const *model,
                               size_t model_len, size_t live_count) {
  CHECK(ecs_validate(world));

  size_t counts[3] = {0, 0, 0};

  for (size_t i = 0; i < model_len; i++) {
    EntityID id = (EntityID)i;
    StressModel const *m = &model[i];

    CHECK(id < world->locs_size);
    EntityLocation loc = world->entity_locations[id];

    if (!m->alive) {
      CHECK(loc.archetype == ECS_ARCHETYPE_NONE);
      continue;
    }

    counts[m->kind]++;

    switch (m->kind) {
    case STRESS_POS: {
      Archetype_Position const *a = &world->position;

      CHECK(loc.archetype == ECS_ARCHETYPE_POSITION);
      CHECK(loc.row < a->size);
      CHECK(a->entities[loc.row] == id);
      CHECK(a->position[loc.row].x == m->pos.x);
      CHECK(a->position[loc.row].y == m->pos.y);
      break;
    }
    case STRESS_POSVEL: {
      Archetype_Position_Velocity const *a = &world->position_velocity;

      CHECK(loc.archetype == ECS_ARCHETYPE_POSITION_VELOCITY);
      CHECK(loc.row < a->size);
      CHECK(a->entities[loc.row] == id);
      CHECK(a->position[loc.row].x == m->pos.x);
      CHECK(a->position[loc.row].y == m->pos.y);
      CHECK(a->velocity[loc.row].vx == m->vel.vx);
      CHECK(a->velocity[loc.row].vy == m->vel.vy);
      break;
    }
    case STRESS_POSVELACC: {
      Archetype_Position_Velocity_Acceleration const *a =
          &world->position_velocity_acceleration;

      CHECK(loc.archetype == ECS_ARCHETYPE_POSITION_VELOCITY_ACCELERATION);
      CHECK(loc.row < a->size);
      CHECK(a->entities[loc.row] == id);
      CHECK(a->position[loc.row].x == m->pos.x);
      CHECK(a->position[loc.row].y == m->pos.y);
      CHECK(a->velocity[loc.row].vx == m->vel.vx);
      CHECK(a->velocity[loc.row].vy == m->vel.vy);
      CHECK(a->acceleration[loc.row].ax == m->acc.ax);
      CHECK(a->acceleration[loc.row].ay == m->acc.ay);
      break;
    }
    }
  }

  CHECK(world->position.size == counts[STRESS_POS]);
  CHECK(world->position_velocity.size == counts[STRESS_POSVEL]);
  CHECK(world->position_velocity_acceleration.size == counts[STRESS_POSVELACC]);
  CHECK(counts[0] + counts[1] + counts[2] == live_count);
}

static void stress_check_query(ecs_World *world, StressModel *model,
                               size_t model_len, unsigned char *seen) {
  memset(seen, 0, model_len);

  size_t expected = 0;
  for (size_t i = 0; i < model_len; i++) {
    if (model[i].alive && model[i].kind != STRESS_POS)
      expected++;
  }

  size_t visited = 0;
  MovementIter iter = {0};

  while (ecs_iter_movement(world, &iter)) {
    EntityID id = iter.entity;

    CHECK(id < model_len);

    StressModel *m = &model[id];

    CHECK(m->alive);
    CHECK(m->kind != STRESS_POS);
    CHECK(!seen[id]);
    seen[id] = 1;
    visited++;

    CHECK(iter.pos->x == m->pos.x);
    CHECK(iter.pos->y == m->pos.y);
    CHECK(iter.vel->vx == m->vel.vx);
    CHECK(iter.vel->vy == m->vel.vy);

    iter.pos->x += iter.vel->vx;
    iter.pos->y += iter.vel->vy;
    m->pos.x += m->vel.vx;
    m->pos.y += m->vel.vy;
  }

  CHECK(visited == expected);
}

static void stress_run(unsigned seed, size_t steps, size_t validate_every) {
  srand(seed);

  ecs_World world = ecs_new_world();

  StressModel *model = calloc(steps, sizeof *model);
  EntityID *live = calloc(steps, sizeof *live);
  unsigned char *seen = calloc(steps, 1);
  CHECK(model != NULL && live != NULL && seen != NULL);

  size_t model_len = 0;
  size_t live_count = 0;

  for (size_t step = 0; step < steps; step++) {
    int action = rand() % 100;

    if (live_count == 0 || action < 55) {
      EntityID id = stress_spawn(&world, model, steps);

      if ((size_t)id >= model_len)
        model_len = (size_t)id + 1;

      live[live_count++] = id;
    } else if (action < 95) {
      size_t pick = (size_t)rand() % live_count;
      EntityID id = live[pick];

      CHECK(ecs_despawn(&world, id));
      model[id].alive = false;

      live[pick] = live[--live_count];
    } else {
      if (model_len > 0) {
        EntityID id = (EntityID)((size_t)rand() % model_len);

        if (!model[id].alive)
          CHECK(!ecs_despawn(&world, id));
      }

      CHECK(!ecs_despawn(&world, (EntityID)(model_len + 1000)));
    }

    if (step % validate_every == 0) {
      stress_check_world(&world, model, model_len, live_count);
      stress_check_query(&world, model, model_len, seen);
    }
  }

  stress_check_world(&world, model, model_len, live_count);
  stress_check_query(&world, model, model_len, seen);

  ecs_destroy_world(&world);

  free(model);
  free(live);
  free(seen);
}

void stress(void);
void stress(void) {
  for (unsigned seed = 1; seed <= 200; seed++)
    stress_run(seed, 300, 1);

  stress_run(123, 20000, 50);
}
