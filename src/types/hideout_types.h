#ifndef HIDEOUT_TYPES_H
#define HIDEOUT_TYPES_H

#include "../engine/graphics/g2d.h"

typedef enum {
    HIDEOUT_WARDROBE
} HideoutType;

typedef struct {
    HideoutType type;
    int collision_x, collision_y;
    char hint_enter_text[256];
    char hint_exit_text[256];
    char use_hint_text[128];
    unsigned short hint_id;
} Hideout;

#endif // HIDEOUT_TYPES_H