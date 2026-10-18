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

EntityID ecs_archetype_position_spawn(ecs_World *world, Position position) {
  size_t size = world->position.size;

  if (size >= world->position.capacity) {
    size_t new_cap = 0;
    if (world->position.capacity == 0)
      new_cap = 1;
    else
      new_cap = world->position.capacity * 2;

    world->position.capacity = new_cap;

    Position *new_positions = (Position *)realloc(world->position.position,
                                                  new_cap * sizeof(Position));
    if (new_positions == NULL) {
      printf("New positions is null\n");
      exit(EXIT_FAILURE);
    }

    EntityID *new_entities = (EntityID *)realloc(world->position.entities,
                                                 new_cap * sizeof(EntityID));
    if (new_entities == NULL) {
      printf("New entities is null\n");
      exit(EXIT_FAILURE);
    }

    world->position.position = new_positions;
    world->position.entities = new_entities;
  }

  world->position.position[size] = position;
  world->position.entities[size] = world->next_available_entity_id;

  uint32_t id = world->next_available_entity_id;

  world->next_available_entity_id++;
  world->position.size++;
  return id;
}

EntityID ecs_archetype_position_velocity_spawn(ecs_World *world,
                                               Position position,
                                               Velocity velocity);

EntityID ecs_archetype_position_velocity_acceleration_spawn(
    ecs_World *world, Position position, Velocity velocity,
    Acceleration acceleration);

bool ecs_despawn(ecs_World *world, EntityID entity_id);
