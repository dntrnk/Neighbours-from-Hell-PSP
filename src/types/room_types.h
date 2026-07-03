#ifndef ROOM_TYPES_H
#define ROOM_TYPES_H

typedef enum {
    ROOM_NONE = -1,
    ROOM_KIT = 0,
    ROOM_LIR,
    ROOM_ANC,
    ROOM_FRO,
    ROOM_TOI
} RoomID;

typedef struct {
    int x1;
    int x2;
    int floor;
} RoomCollision;

#endif // ROOM_TYPES_H