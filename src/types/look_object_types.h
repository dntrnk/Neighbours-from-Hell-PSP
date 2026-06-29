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
    char use_hint_text[128];
    char tricked_hint_text[256];
    char tricked_use_hint_text[128];
    unsigned short hint_id;

    char phrase_text[256];
    int phrase_y;
    int bubble_size;

    bool tricked;
    int item_to_trick;

    char tricked_phrase_text[256];
    int tricked_phrase_y;
    int tricked_bubble_size;

    void (*on_trick)(void);
    void (*on_untrick)(void);
} LookObject;

#endif // LOOK_OBJECT_TYPES_H