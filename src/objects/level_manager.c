#include "level_manager.h"

// #include "../../types/room_types.h"
// #include "../../objects/gfx_types.h"
// #include "../../objects/door_types.h"

// #include "../../objects/woody.h"
// #include "../../objects/neighbour.h"
// #include "../../objects/intro.h"
// #include "../../objects/sign.h"
// #include "../../objects/tutorial.h"
// #include "../../objects/level_end.h"

static hDoor* h_doors[9][2];
static vDoor* v_doors[9][3];

void level_doors_draw(void) {
    // Отрисовка дверей

    // h_doors

    // Левые двери
    for (int room = 0; room < room_count; room++) {
        hDoor* current_door = h_doors[room][0]; // Левая дверь всегда под индексом 0

        if (current_door == NULL)
            continue;

        if (current_door->using_by == USING_NONE) {
            if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                current_door->sprite_x >= camera_right ||
                current_door->sprite_y + current_door->sprite_h <= camera_y ||
                current_door->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImage(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // Правые двери
    for (int room = 0; room < room_count; room++) {
        hDoor* current_door = h_doors[room][1]; // Левая дверь всегда под индексом 1

        if (current_door == NULL)
            continue;

        if (current_door->using_by == USING_NONE) {
            if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                current_door->sprite_x >= camera_right ||
                current_door->sprite_y + current_door->sprite_h <= camera_y ||
                current_door->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImage(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // v_doors
    for (int room = 0; room < room_count; room++) {
        for (int door = 0; door < 3; door++) {
            vDoor* current_door = v_doors[room][door];

            if (current_door == NULL)
                continue;

            if (current_door->using_by == USING_NONE) {
                if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                    current_door->sprite_x >= camera_right ||
                    current_door->sprite_y + current_door->sprite_h <= camera_y ||
                    current_door->sprite_y >= camera_bottom)
                    continue;

                g2d_DrawImage(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
            }
        }
    }   
}
