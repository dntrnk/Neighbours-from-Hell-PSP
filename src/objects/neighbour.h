#ifndef NEIGHBOUR_H
#define NEIGHBOUR_H

#include "../engine/graphics/g2d.h"

#include "../types/gfx_types.h"
#include "../types/door_types.h"
#include "../types/look_object_types.h"
#include "../types/use_object_types.h"

#include "../types/bubble_types.h"

#include "../animations/neighbour/generic.h"
#include "../animations/neighbour/doorleft.h"
#include "../animations/neighbour/doorright.h"
// doorback
#include "../animations/neighbour/sofa.h"
#include "../animations/neighbour/binoculars.h"
#include "../animations/neighbour/generic2.h"
#include "../animations/neighbour/generic3.h"
#include "../animations/neighbour/look.h"
#include "../animations/neighbour/shout0.h"
#include "../animations/neighbour/sofa1.h"
#include "../animations/neighbour/gameover.h"

typedef struct Woody Woody;

typedef enum {
    STATE_GAME_OVER_START,
    STATE_GO_TO_FLOOR,
    STATE_GO_TO_WOODY,
    STATE_LOSE_ANIMATION
} GameOverState;

typedef struct Neighbour {
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

    // Used in gameover
    int origin_x, origin_y;

    int action_state;
    bool action_done;

    bool in_door;

    hDoor* (*h_doors)[2];
    vDoor* (*v_doors)[3];

    LookObject* (*look_objects)[8];
    UseObject* (*use_objects)[2];

    RoomID room;

    Woody* woody;

    // Прохождение уровня
    int emotion;
    int breakdowns;
    float angry;
    int angry_cooldown; // Время до того, пока angry будет уменьшаться

    bool jingle_joke_playing;
    int jingle_joke_timer;

    bool woody_caught;
    GameOverState game_over_state;
    int game_over_goal_x;
    bool* neighbour_active;
    bool* level_end_active;

    // Интерфейс
    // local spriteatlasUI = SpriteAtlas_INGAMEUI
    // local uiFont = Font_BLUEHIGC_24

    // local cOrange = Color.new(254, 159, 0)
    // local cWhite = Color.new(255, 255, 255)
    
    unsigned short head_icon_src_x;
    bool head_icon_show;
    bool head_icon_animation_play;
    int head_icon_animation_frame;

    Bubble current_bubble;
    unsigned short bubble_sprite_src_x, bubble_sprite_src_y;
    bool bubble_show;

    char ui_breakdowns_counter_text[32];
    int ui_breakdowns_counter_text_x;
    bool ui_breakdowns_counter_text_show;

    bool ui_breakdowns_counter_animation_play;
    int ui_breakdowns_counter_animation_frame_time;
    int ui_breakdowns_counter_animation_frame;
} Neighbour;

Neighbour* neighbour_create(
    g2dImage* spritelists[16],
    const Frame* gfxdata[16],
    const Animation* animations[16],
    int start_x,
    int start_y,
    RoomID start_room,
    int start_bubble,
    hDoor* (*h_doors)[2],
    vDoor* (*v_doors)[3],
    LookObject* (*look_objects)[8],
    UseObject* (*use_objects)[2],
    Woody* woody,
    bool* neighbour_active,
    bool* level_end_active
);

void neighbour_animation_update_frame(Neighbour* neighbour);
void neighbour_animation_set(Neighbour* neighbour, int pack, int animation);
void neighbour_update(Neighbour* neighbour);
void neighbour_draw(const Neighbour* neighbour);
void neighbour_door_draw(const Neighbour* neighbour);
void neighbour_draw_ui(const Neighbour* neighbour);
void neighbour_breakdown_counter_update(Neighbour* neighbour);
void neighbour_unload(Neighbour* neighbour);

#endif // NEIGHBOUR_H