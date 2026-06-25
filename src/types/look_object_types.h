#ifndef LOOK_OBJECT_TYPES_H
#define LOOK_OBJECT_TYPES_H

#include "../engine/graphics/g2d.h"

typedef struct {
    g2dImage* spritelist;
    int sprite_x, sprite_y;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    bool sprite_show;

    int collision_x, collision_y;
    char hint_text[256];
    unsigned short hint_id;

    char phrase_text[256];
    int phrase_y;
    int bubble_size;
} LookObject;

#endif // LOOK_OBJECT_TYPES_H