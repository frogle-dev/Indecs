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
  ECS_ARCHETYPE_NONE,
  ECS_ARCHETYPE_POSITION,
  ECS_ARCHETYPE_POSITION_VELOCITY,
  ECS_ARCHETYPE_POSITION_VELOCITY_ACCELERATION,
} ArchetypeID;

typedef struct {
  ArchetypeID archetype;
  size_t row;
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
  EntityID next_available_entity_id;

  Archetype_Position position;
  Archetype_Position_Velocity position_velocity;
  Archetype_Position_Velocity_Acceleration position_velocity_acceleration;

  size_t locs_size;
  size_t locs_capacity;
  // indexed by EntityID
  EntityLocation *entity_locations;
} ecs_World;

///// QUERIES

typedef struct {
  // the size of the array is the number of matched archetypes
  Position *pos[2];
  Velocity *vel[2];

  size_t size[2];
} MovementQuery;

typedef struct {
  Position *pos;
  Velocity *vel;

  size_t archetype_i;
  size_t row;
} MovementIter;

// will assign to movement query
MovementIter ecs_query_movement(ecs_World *world, MovementQuery *query);

// returns: true = iteration continuing, false = iteration finished
bool ecs_iter_movement(MovementQuery const *query, MovementIter *iter);

///// FUNCTIONS

ecs_World ecs_new_world(void);
void ecs_destroy_world(ecs_World *world);

EntityID ecs_archetype_position_spawn(ecs_World *world, A_Pos archetype);
EntityID ecs_archetype_position_velocity_spawn(ecs_World *world,
                                               A_PosVel archetype);
EntityID
ecs_archetype_position_velocity_acceleration_spawn(ecs_World *world,
                                                   A_PosVelAcc archetype);

#define ecs_spawn(world, Archetype)                                            \
  _Generic((Archetype),                                                        \
      A_Pos: ecs_archetype_position_spawn,                                     \
      A_PosVel: ecs_archetype_position_velocity_spawn,                         \
      A_PosVelAcc: ecs_archetype_position_velocity_acceleration_spawn)(        \
      world, Archetype)

bool ecs_despawn(ecs_World *world, EntityID entity_id);
