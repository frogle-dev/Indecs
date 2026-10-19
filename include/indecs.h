#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t EntityID;

///// COMPONENTS

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

///// INTERNAL ARCHETYPES
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

typedef enum {
  NONE,
  POSITION,
  POSITION_VELOCITY,
  POSITION_VELOCITY_ACCELERATION,
} ArchetypeID;

typedef struct {
  ArchetypeID archetype;
  size_t entity_row;
} EntityLocation;

///// EXTERNAL ARCHETYPES

typedef struct {
  Position pos;
} A_Pos;

typedef struct {
  Position pos;
  Velocity vel;
} A_PosVel;

typedef struct {
  Position pos;
  Velocity vel;
  Acceleration acc;
} A_PosVelAcc;

///// WORLD

typedef struct {
  uint32_t next_available_entity_id;

  Archetype_Position position;
  Archetype_Position_Velocity position_velocity;
  Archetype_Position_Velocity_Acceleration position_velocity_acceleration;

  // indexed by EntityID
  EntityLocation *entity_locations;
} ecs_World;

///// FUNCTIONS

ecs_World ecs_new_world(void);
void ecs_destroy_world(ecs_World *world);

#define ecs_spawn(world, Archetype)                                                                \
  _Generic((Archetype),                                                                            \
      A_Pos: ecs_archetype_position_spawn,                                                         \
      A_PosVel: ecs_archetype_position_velocity_spawn,                                             \
      A_PosVelAcc: ecs_archetype_position_velocity_acceleration_spawn)(world, Archetype)

EntityID ecs_archetype_position_spawn(ecs_World *world, A_Pos archetype);
EntityID ecs_archetype_position_velocity_spawn(ecs_World *world, A_PosVel archetype);
EntityID ecs_archetype_position_velocity_acceleration_spawn(ecs_World *world,
                                                            A_PosVelAcc archetype);
bool ecs_despawn(ecs_World *world, EntityID entity_id);
