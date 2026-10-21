#include <indecs.h>

#include <stdio.h>
#include <stdlib.h>

ecs_World ecs_new_world(void) {
  ecs_World world = {0};
  return world;
}

void ecs_destroy_world(ecs_World *world) {
  // position archetype
  free(world->position.position);
  free(world->position.entities);

  // position, velocity archetype
  free(world->position_velocity.position);
  free(world->position_velocity.velocity);
  free(world->position_velocity.entities);

  // position velocity acceleration archetype
  free(world->position_velocity_acceleration.position);
  free(world->position_velocity_acceleration.velocity);
  free(world->position_velocity_acceleration.acceleration);
  free(world->position_velocity_acceleration.entities);

  free(world->entity_locations);
}

static EntityID *realloc_entities(EntityID *entities, size_t new_cap) {
  EntityID *new_entities =
      (EntityID *)realloc(entities, new_cap * sizeof(EntityID));

  if (new_entities == NULL) {
    printf("New entities is null\n");
    exit(EXIT_FAILURE);
  }

  return new_entities;
}

static void *realloc_components(void *components, size_t component_size,
                                size_t new_cap) {
  void *new_components = realloc(components, new_cap * component_size);

  if (new_components == NULL) {
    printf("New components is null\n");
    exit(EXIT_FAILURE);
  }

  return new_components;
}

static void add_entity_location(ecs_World *world, ArchetypeID archetype_id,
                                EntityID id, size_t row) {
  if (world->locs_size >= world->locs_capacity) {
    size_t new_cap = 0;
    if (world->locs_capacity == 0)
      new_cap = 1;
    else
      new_cap = (world->locs_capacity) * 2;

    world->locs_capacity = new_cap;

    EntityLocation *new_locs = (EntityLocation *)realloc(
        world->entity_locations, new_cap * sizeof(EntityLocation));

    if (new_locs == NULL) {
      printf("New entity locations is null\n");
      exit(EXIT_FAILURE);
    }

    world->entity_locations = new_locs;
  }

  EntityLocation loc = {
      .archetype = archetype_id,
      .row = row,
  };

  world->entity_locations[id] = loc;

  world->locs_size++;
}

static EntityID add_entity(ecs_World *world, ArchetypeID archetype_id,
                           EntityID *entities, size_t *size) {
  (*size)++;

  const size_t row = (*size) - 1;

  entities[row] = world->next_available_entity_id;

  EntityID id = world->next_available_entity_id;

  world->next_available_entity_id++;

  add_entity_location(world, archetype_id, id, row);

  return id;
}

static bool check_ifso_grow_arraylist(size_t size, size_t *capacity) {
  if (size >= *capacity) {
    size_t new_cap = 0;
    if (*capacity == 0)
      new_cap = 1;
    else
      new_cap = (*capacity) * 2;

    *capacity = new_cap;

    return true;
  }

  return false;
}

EntityID ecs_archetype_position_spawn(ecs_World *world, A_Pos entity_data) {
  Archetype_Position *a = &world->position;

  if (check_ifso_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position),
                                                 a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;

  return add_entity(world, ECS_ARCHETYPE_POSITION, a->entities, &a->size);
}

EntityID ecs_archetype_position_velocity_spawn(ecs_World *world,
                                               A_PosVel entity_data) {
  Archetype_Position_Velocity *a = &world->position_velocity;

  if (check_ifso_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position),
                                                 a->capacity);
    a->velocity = (Velocity *)realloc_components(a->velocity, sizeof(Velocity),
                                                 a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;
  a->velocity[a->size] = entity_data.vel;

  return add_entity(world, ECS_ARCHETYPE_POSITION_VELOCITY, a->entities,
                    &a->size);
}

EntityID
ecs_archetype_position_velocity_acceleration_spawn(ecs_World *world,
                                                   A_PosVelAcc entity_data) {
  Archetype_Position_Velocity_Acceleration *a =
      &world->position_velocity_acceleration;

  if (check_ifso_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position),
                                                 a->capacity);
    a->velocity = (Velocity *)realloc_components(a->velocity, sizeof(Velocity),
                                                 a->capacity);
    a->acceleration = (Acceleration *)realloc_components(
        a->acceleration, sizeof(Acceleration), a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;
  a->velocity[a->size] = entity_data.vel;
  a->acceleration[a->size] = entity_data.acc;

  return add_entity(world, ECS_ARCHETYPE_POSITION_VELOCITY_ACCELERATION,
                    a->entities, &a->size);
}

static const EntityLocation DEAD_LOC = {
    .archetype = ECS_ARCHETYPE_NONE,
    .row = 0, // doesnt matter
};

static void replace_entities(ecs_World *world, EntityID *entities,
                             EntityID replaced, size_t cur_row, size_t new_row,
                             EntityLocation new_loc) {
  entities[cur_row] = entities[new_row];

  // update the swapped entity's location to point at its new memory location
  world->entity_locations[entities[cur_row]] = new_loc;

  world->entity_locations[replaced] = DEAD_LOC;
}

bool ecs_despawn(ecs_World *world, EntityID entity_id) {
  if (entity_id >= world->locs_size)
    return false;

  EntityLocation loc = world->entity_locations[entity_id];

  // replace despawning entity's data with the last entity's data to fill the
  // hole, then decrement the size of the heap array
  // 123456789
  //    ^
  // 12395678
  //    ^    x

  switch (loc.archetype) {
  case ECS_ARCHETYPE_POSITION: {
    Archetype_Position *a = &world->position;

    size_t last_row_idx = a->size - 1;

    a->position[loc.row] = a->position[last_row_idx];

    replace_entities(world, a->entities, entity_id, loc.row, last_row_idx, loc);

    a->size--;
    break;
  }
  case ECS_ARCHETYPE_POSITION_VELOCITY: {
    Archetype_Position_Velocity *a = &world->position_velocity;

    size_t last_row_idx = a->size - 1;

    a->position[loc.row] = a->position[last_row_idx];
    a->velocity[loc.row] = a->velocity[last_row_idx];

    replace_entities(world, a->entities, entity_id, loc.row, last_row_idx, loc);

    a->size--;
    break;
  }
  case ECS_ARCHETYPE_POSITION_VELOCITY_ACCELERATION: {
    Archetype_Position_Velocity_Acceleration *a =
        &world->position_velocity_acceleration;

    size_t last_row_idx = a->size - 1;

    a->position[loc.row] = a->position[last_row_idx];
    a->velocity[loc.row] = a->velocity[last_row_idx];
    a->acceleration[loc.row] = a->acceleration[last_row_idx];

    replace_entities(world, a->entities, entity_id, loc.row, last_row_idx, loc);

    a->size--;
    break;
  }
  case ECS_ARCHETYPE_NONE: {
    return false;
  }
  };

  return true;
}

///// QUERIES

MovementIter ecs_query_movement(ecs_World *world, MovementQuery *query) {
  *query = (MovementQuery){
      .pos = {world->position_velocity.position,
              world->position_velocity_acceleration.position},
      .vel = {world->position_velocity.velocity,
              world->position_velocity_acceleration.velocity},
      .size = {world->position_velocity.size,
               world->position_velocity_acceleration.size},
  };

  MovementIter iter = {0};

  return iter;
}

bool ecs_iter_movement(MovementQuery const *query, MovementIter *iter) {
  size_t matched_archetypes = 2;

  // has to be here otherwise archetype_i can increment after the check,
  // effectively bypassing it on some cases
  if (iter->archetype_i >= matched_archetypes) {
    return false;
  }

  if (iter->row >= query->size[iter->archetype_i]) {
    iter->archetype_i++;
    iter->row = 0;

    if (iter->archetype_i >= matched_archetypes)
      return false;
  }

  iter->pos = &(query->pos[iter->archetype_i])[iter->row];
  iter->vel = &(query->vel[iter->archetype_i])[iter->row];

  iter->row++;

  return true;
}
