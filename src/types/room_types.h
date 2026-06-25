#ifndef ROOM_TYPES_H
#define ROOM_TYPES_H

typedef enum {
    ROOM_KIT,
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