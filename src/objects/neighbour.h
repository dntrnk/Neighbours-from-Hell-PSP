#ifndef NEIGHBOUR_H
#define NEIGHBOUR_H

#include "../engine/graphics/g2d.h"

#include "../types/gfx_types.h"
#include "../types/door_types.h"
#include "../types/look_object_types.h"
#include "../types/bubble_types.h"

#include "../animations/neighbour/generic.h"
#include "../animations/neighbour/doorleft.h"
#include "../animations/neighbour/doorright.h"
// doorback
#include "../animations/neighbour/sofa.h"

typedef struct {
    // Анимации
    g2dImage* spritelists[16];
    const Frame* gfxdata[16];
    const Animation* animations[16];

    int current_spritelist;

    int current_animation_pack;
    int current_animation_index;
    
    bool animation_loop;
    int animation_frame;
    int animation_frame_time;
    int animation_length;

    unsigned short sprite_src_x, sprite_src_y;
    unsigned short sprite_w, sprite_h;
    int sprite_offset_x, sprite_offset_y;

    // Анимации дверей
    void* enter_door;
    void* dest_door;

    bool dest_door_animation_play;

    int dest_door_current_spritelist;

    int dest_door_current_animation_pack;
    int dest_door_current_animation_index;

    int dest_door_animation_frame;
    int dest_door_animation_frame_time;
    int dest_door_animation_length;

    int dest_door_sprite_x;
    int dest_door_sprite_y;
    unsigned short dest_door_sprite_src_x;
    unsigned short dest_door_sprite_src_y;
    unsigned short dest_door_sprite_w;
    unsigned short dest_door_sprite_h;
    int dest_door_sprite_offset_x;
    int dest_door_sprite_offset_y;

    // Основа соседа
    int x, y;

    int action_state;
    bool action_done;

    bool in_door;

    hDoor* (*h_doors)[2];
    vDoor* (*v_doors)[3];

    LookObject* (*look_objects)[8];

    RoomID room;

    // Прохождение уровня
    int emotion;
    int breakdowns;

    // Интерфейс
    // local spriteatlasUI = SpriteAtlas_INGAMEUI
    // local uiFont = Font_BLUEHIGC_24

    // local cOrange = Color.new(254, 159, 0)
    // local cWhite = Color.new(255, 255, 255)
    
    unsigned short head_icon_src_x;
    // local breakdownsText = tostring(breakdowns)
    Bubble current_bubble;
    unsigned short bubble_sprite_src_x, bubble_sprite_src_y;
    bool bubble_show;
} Neighbour;

Neighbour* neighbour_create(
    g2dImage* spritelists[16],
    const Frame* gfxdata[16],
    const Animation* animations[16],
    int start_x,
    int start_y,
    int start_bubble,
    hDoor* (*h_doors)[2],
    vDoor* (*v_doors)[3],
    LookObject* (*look_objects)[8]
);

void neighbour_animation_update_frame(Neighbour* neighbour);
void neighbour_animation_set(Neighbour* neighbour, int pack, int animation);
void neighbour_update(Neighbour* neighbour);
void neighbour_draw(const Neighbour* neighbour);
void neighbour_door_draw(const Neighbour* neighbour);
void neighbour_draw_ui(const Neighbour* neighbour);
void neighbour_unload(Neighbour* neighbour);

#endif // NEIGHBOUR_H