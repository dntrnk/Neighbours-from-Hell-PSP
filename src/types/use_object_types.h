#ifndef USE_OBJECT_TYPES_H
#define USE_OBJECT_TYPES_H

#include "../engine/graphics/g2d.h"

#define MAX_USE_OBJECTS_IN_ROOM 2

typedef struct {
    g2dImage* spritelist;
    int sprite_x, sprite_y;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    bool sprite_show;

    int collision_x, collision_y;
    char hint_text[256];
    char use_hint_text[128];
    unsigned short hint_id;

    bool tricked;
    bool first_time;

    int trick_making_length;
    int trick_tv_rating;

    void (*on_making_trick)(void);
    void (*on_stop_making_trick)(void);
    void (*on_trick)(void);
    void (*on_untrick)(void);
} UseObject;

#endif // USE_OBJECT_TYPES_H