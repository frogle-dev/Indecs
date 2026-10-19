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
}

EntityID *realloc_entities(EntityID *entities, size_t new_cap) {
  EntityID *new_entities = (EntityID *)realloc(entities, new_cap * sizeof(EntityID));

  if (new_entities == NULL) {
    printf("New entities is null\n");
    exit(EXIT_FAILURE);
  }

  return new_entities;
}

void *realloc_components(void *components, size_t component_size, size_t new_cap) {
  void *new_components = realloc(components, new_cap * component_size);

  if (new_components == NULL) {
    printf("New components is null\n");
    exit(EXIT_FAILURE);
  }

  return new_components;
}

EntityID add_entity(ecs_World *world, EntityID *entities, size_t *row) {
  entities[*row] = world->next_available_entity_id;

  EntityID id = world->next_available_entity_id;

  world->next_available_entity_id++;

  (*row)++;

  return id;
}

bool should_grow_arraylist(size_t size, size_t *capacity) {
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

  if (should_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position), a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;

  return add_entity(world, a->entities, &a->size);
}

EntityID ecs_archetype_position_velocity_spawn(ecs_World *world, A_PosVel entity_data) {
  Archetype_Position_Velocity *a = &world->position_velocity;

  if (should_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position), a->capacity);
    a->velocity = (Velocity *)realloc_components(a->velocity, sizeof(Velocity), a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;
  a->velocity[a->size] = entity_data.vel;

  return add_entity(world, a->entities, &a->size);
}

EntityID ecs_archetype_position_velocity_acceleration_spawn(ecs_World *world,
                                                            A_PosVelAcc entity_data) {
  Archetype_Position_Velocity_Acceleration *a = &world->position_velocity_acceleration;

  if (should_grow_arraylist(a->size, &a->capacity)) {
    a->position = (Position *)realloc_components(a->position, sizeof(Position), a->capacity);
    a->velocity = (Velocity *)realloc_components(a->velocity, sizeof(Velocity), a->capacity);
    a->acceleration =
        (Acceleration *)realloc_components(a->acceleration, sizeof(Acceleration), a->capacity);

    a->entities = realloc_entities(a->entities, a->capacity);
  }

  a->position[a->size] = entity_data.pos;
  a->velocity[a->size] = entity_data.vel;
  a->acceleration[a->size] = entity_data.acc;

  return add_entity(world, a->entities, &a->size);
}

bool ecs_despawn(ecs_World *world, EntityID entity_id);
