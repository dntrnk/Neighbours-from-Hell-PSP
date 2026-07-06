#ifndef DOOR_TYPES_H
#define DOOR_TYPES_H

#include "../types/room_types.h"

#define LEFT_DOOR 0
#define RIGHT_DOOR 1

#define MAX_V_DOORS_IN_ROOM 3

typedef enum {
    USING_NONE,
    USING_WOODY,
    USING_NEIGHBOUR
} DoorUsingBy;

typedef struct {
    g2dImage* spritelist;
    int sprite_x, sprite_y;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    int collision_x;
    int exit_x;
    RoomID dest_door_room;
    int dest_door_id;
    DoorUsingBy using_by;
} hDoor;

typedef struct {
    g2dImage* spritelist;
    int sprite_x, sprite_y;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned int sprite_w, sprite_h;
    int collision_x, collision_y;
    RoomID dest_door_room;
    int dest_door_id;
    DoorUsingBy using_by;
} vDoor;

#endif // DOOR_TYPES_H