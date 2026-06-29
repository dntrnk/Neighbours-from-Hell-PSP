#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H

#include <stdbool.h>

#include "item_types.h"

#include "../engine/graphics/g2d.h"

typedef enum {
    SPRITE_TYPE_OPENS,
    SPRITE_TYPE_DISAPPEARS
} StorageSpriteType;

typedef enum {
    SIDE_LEFT_HIGH,
    SIDE_LEFT_MID,
    SIDE_LEFT_LOW,
    SIDE_UP_HIGH,
    SIDE_UP_MID,
    SIDE_UP_LOW,
    SIDE_RIGHT_HIGH,
    SIDE_RIGHT_MID,
    SIDE_RIGHT_LOW,
} StorageSide;

typedef struct {
    g2dImage* spritelist;
    int sprite_x, sprite_y;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    StorageSpriteType sprite_type;
    bool sprite_show;

    int collision_x, collision_y;
    char hint_text[256];
    char use_hint_text[128];
    unsigned short hint_id;

    Item items[4];
    bool opened;
    StorageSide side;
} Storage;

#endif // STORAGE_TYPES_H