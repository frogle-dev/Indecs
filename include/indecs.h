#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t EntityID;

typedef struct {
  float x;
  float y;
} Position;

typedef struct {
  float vx;
  float vy;
} Velocity;

typedef struct {
  float ax;
  float ay;
} Acceleration;

// archetypes are SOA arraylists

typedef struct {
  EntityID *entities;

  Position *position;

  size_t size;
  size_t capacity;
} Archetype_Position;

typedef struct {
  EntityID *entities;

  Position *position;
  Velocity *velocity;

  size_t size;
  size_t capacity;
} Archetype_Position_Velocity;

typedef struct {
  EntityID *entities;

  Position *position;
  Velocity *velocity;
  Acceleration *acceleration;

  size_t size;
  size_t capacity;
} Archetype_Position_Velocity_Acceleration;

typedef struct {
  uint32_t next_available_entity_id;

  Archetype_Position position;
  Archetype_Position_Velocity position_velocity;
  Archetype_Position_Velocity_Acceleration position_velocity_acceleration;
} ecs_World;

ecs_World ecs_new_world(void);
void ecs_destroy_world(ecs_World *world);
EntityID ecs_archetype_position_spawn(ecs_World *world, Position position);
EntityID ecs_archetype_position_velocity_spawn(ecs_World *world,
                                               Position position,
                                               Velocity velocity);
EntityID ecs_archetype_position_velocity_acceleration_spawn(
    ecs_World *world, Position position, Velocity velocity,
    Acceleration acceleration);
bool ecs_despawn(ecs_World *world, EntityID entity_id);
