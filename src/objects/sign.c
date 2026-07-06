#include "sign.h"

#include <stdlib.h>

#include "woody.h"

#include "../animations/ui/sign.h"

extern const Frame ui_sign_frames[];
extern const Animation ui_sign_animations[];

extern int camera_x;
extern int camera_y;
extern int camera_right;
extern int camera_bottom;

Sign* sign_create(
    hDoor* (*h_doors)[2],
    vDoor* (*v_doors)[3],
    Tutorial* tutorial,
    Woody* woody
) {
    Sign* sign = malloc(sizeof(Sign));
    memset(sign, 0, sizeof(Sign));

    sign->x = -128;
    sign->y = -128;

    sign->collision_x1 = 500;
    sign->collision_x2 = 554;
    sign->room = ROOM_KIT;

    sign->h_doors = h_doors;
    sign->v_doors = v_doors;

    sign->animation_frame = 0;
    sign->animation_frame_time = 0;
    sign->animation_length = 0;

    sign->spritelist = g2d_LoadImage("assets_thq/sprites/ui/ingame/sign.png", G2D_CLUT4);
    sign->sprite_src_x = 0;
    sign->sprite_src_y = 0;
    sign->sprite_w = 0;
    sign->sprite_h = 0;
    sign->sprite_offset_x = 0;
    sign->sprite_offset_y = 0;

    sign_animation_set(sign);

    sign->current_state = 0;

    sign->tutorial = tutorial;
    sign->woody = woody;

    return sign;
}

void sign_animation_update_frame(Sign* sign) {
    int animation_frame = ui_sign_animations[0].frames[sign->animation_frame];
    const Frame* frame = &ui_sign_frames[animation_frame];

    sign->sprite_src_x = frame->src_x;
    sign->sprite_src_y = frame->src_y;
    sign->sprite_w = frame->w;
    sign->sprite_h = frame->h;
    sign->sprite_offset_x = frame->offset_x;
    sign->sprite_offset_y = frame->offset_y;
}

void sign_animation_set(Sign* sign) {
    sign->animation_frame = 0;
    sign->animation_frame_time = 0;
    sign->animation_length = ui_sign_animations[0].frame_count;
    sign_animation_update_frame(sign);
}

void sign_show(Sign* sign) {
    sign->x = 523;
    sign->y = 182;
    sign->animation_frame = 0;
}

void sign_update(Sign* sign, bool woody_colliding) {
    if (woody_colliding) {
        sign->current_state++;

        switch (sign->current_state) {
            case 1: {
                sign->x = -128;
                sign->y = -128;

                sign->collision_x1 = 0;
                sign->collision_x2 = 390;

                sign->room = ROOM_LIR;

                hDoor* left_door_kit = sign->h_doors[ROOM_KIT][LEFT_DOOR];
                left_door_kit->sprite_src_x = 423;
                left_door_kit->sprite_src_y = 287;
                left_door_kit->sprite_w = 17;
                left_door_kit->sprite_h = 94;

                hDoor* right_door_lir = sign->h_doors[ROOM_LIR][RIGHT_DOOR];
                right_door_lir->sprite_src_x = 423;
                right_door_lir->sprite_src_y = 101;
                right_door_lir->sprite_w = 14;
                right_door_lir->sprite_h = 94;

                break;
            }

            case 2: {
                sign->x = 215;
                sign->y = 180;

                sign->collision_x1 = 0;
                sign->collision_x2 = 251;

                sign->room = ROOM_LIR;

                break;
            }

            case 3: {
                sign->x = 381;
                sign->y = 322;

                sign->collision_x1 = 412;
                sign->collision_x2 = 600;

                sign->room = ROOM_ANC;

                vDoor* back_door_lir = sign->v_doors[ROOM_LIR][0];
                back_door_lir->sprite_src_x = 92;
                back_door_lir->sprite_src_y = 77;
                back_door_lir->sprite_w = 46;
                back_door_lir->sprite_h = 67;

                vDoor* back_door_anc = sign->v_doors[ROOM_ANC][0];
                back_door_anc->sprite_src_x = 92;
                back_door_anc->sprite_src_y = 77;
                back_door_anc->sprite_w = 46;
                back_door_anc->sprite_h = 67;

                break;
            }

            case 4: {
                sign->x = -128;
                sign->y = -128;

                sign->collision_x1 = 644;
                sign->collision_x2 = 700;

                sign->room = ROOM_FRO;

                hDoor* left_door_fro = sign->h_doors[ROOM_FRO][LEFT_DOOR];
                left_door_fro->sprite_src_x = 423;
                left_door_fro->sprite_src_y = 287;
                left_door_fro->sprite_w = 17;
                left_door_fro->sprite_h = 94;

                hDoor* right_door_anc = sign->h_doors[ROOM_ANC][RIGHT_DOOR];
                right_door_anc->sprite_src_x = 423;
                right_door_anc->sprite_src_y = 101;
                right_door_anc->sprite_w = 14;
                right_door_anc->sprite_h = 94;

                break;
            }

            case 5: {
                sign->x = -128;
                sign->y = -128;

                sign->collision_x1 = 600;
                sign->collision_x2 = 800;

                sign->room = ROOM_FRO;

                break;
            }

            case 6: {
                sign->collision_x1 = -128;
                sign->collision_x2 = -128;

                tutorial_animation_set(sign->tutorial, ANIMATION_UI_JOE_HEAD);

                woody_tricks_counter_update(sign->woody, 100);

                break;
            }
        }
        tutorial_update_phrase(sign->tutorial);
    }

    // Анимация
    sign->animation_frame_time++;
    if (sign->animation_frame_time == 5) {
        sign->animation_frame_time = 0;
        sign->animation_frame++;
        if (sign->animation_frame == sign->animation_length) {
            sign->animation_frame = 0;
        }
        sign_animation_update_frame(sign);
    }

}

void sign_draw(const Sign* sign) {
    if (sign->x >= camera_right ||
        sign->x + sign->sprite_w <= camera_x ||
        sign->y >= camera_bottom ||
        sign->y + sign->sprite_h <= camera_y)
        return;
    
    g2d_DrawImageExt(sign->spritelist, sign->x + sign->sprite_offset_x - camera_x, sign->y + sign->sprite_offset_y - camera_y, sign->sprite_w, sign->sprite_h, WHITE, sign->sprite_src_x, sign->sprite_src_y, sign->sprite_w, sign->sprite_h, 0, 255, G2D_UP_LEFT);
}

void sign_unload(Sign* sign) {
    g2d_FreeImage(sign->spritelist);
    free(sign);
}
