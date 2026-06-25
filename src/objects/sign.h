#ifndef SIGN_H
#define SIGN_H

#include "../engine/graphics/g2d.h"

#include "../types/gfx_types.h"
#include "../types/room_types.h"
#include "../types/door_types.h"

#include "tutorial.h"

typedef struct Woody Woody;

typedef struct {
    int x, y;

    int collision_x1, collision_x2;
    RoomID room;

    hDoor* (*h_doors)[2];
    vDoor* (*v_doors)[3];

    int animation_frame;
    int animation_frame_time;
    int animation_length;

    g2dImage* spritelist;
    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    int sprite_offset_x, sprite_offset_y;

    int current_state;

    Tutorial* tutorial;
    Woody* woody;
} Sign;

Sign* sign_create(
    hDoor* (*h_doors)[2],
    vDoor* (*v_doors)[3],
    Tutorial* tutorial,
    Woody* woody
);

void sign_animation_update_frame(Sign* sign);
void sign_animation_set(Sign* sign);
void sign_show(Sign* sign);
void sign_update(Sign* sign, bool woody_colliding);
void sign_draw(const Sign* sign);
void sign_unload(Sign* sign);

#endif // SIGN_H