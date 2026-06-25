#ifndef WOODY_H
#define WOODY_H

#include <stdbool.h>

#include "../engine/graphics/g2d.h"

#include "sign.h"
#include "level_end.h"

#include "../types/gfx_types.h"
#include "../types/room_types.h"
#include "../types/door_types.h"

#include "../types/look_object_types.h"
#include "../types/hideout_types.h"
#include "../types/storage_types.h"

#include "../animations/woody/generic.h"
#include "../animations/woody/doorleft.h"
#include "../animations/woody/doorright.h"
#include "../animations/woody/doorback.h"
#include "../animations/woody/generic2.h"

typedef enum {
    STATE_H_MOVE,
    STATE_AUTO_H_MOVE,
    STATE_V_MOVE,
    STATE_AUTO_V_MOVE,
    STATE_IN_H_DOOR,
    STATE_IN_V_DOOR,
    STATE_LOOK_OBJECT,
    STATE_HIDEOUT_ENTER,
    STATE_HIDEOUT,
    STATE_HIDEOUT_EXIT,
    STATE_STORAGE_CHECK,
    STATE_STORAGE_FOUND,
    STATE_STORAGE_NOT_FOUND,
    STATE_STORAGE_END,
    STATE_LEVEL_START,
    STATE_LEVEL_ENDING,
    STATE_STOP
} WoodyState;

typedef enum {
    GOAL_LOOK_OBJECT,
    GOAL_HIDEOUT,
    GOAL_STORAGE,
    GOAL_FLOOR,
    GOAL_NONE
} WoodyAutoMoveGoal;

typedef enum {
    SQUARE,
    CROSS,
    CIRCLE
} HintButton;

typedef struct {
    char* text;
    HintButton button_type;
    bool is_active_goal;
    bool show;
} Hint;

typedef struct Woody {
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

    int enter_door_sprite_x;
    int enter_door_sprite_y;

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

    int dest_door_exit_x;

    // Основа Вуди
    int x, y;

    int speed_x, speed_y;

    short velocity_x, velocity_y;

    bool can_move;
    bool in_door;
    bool is_on_floor;
    int floor_y;
    bool hide;

    WoodyState state;

    WoodyAutoMoveGoal auto_move_goal_type;
    int auto_move_goal_x;
    int auto_move_goal_y;

    hDoor* (*h_doors)[2];
    vDoor* (*v_doors)[3];

    LookObject* (*look_objects)[8];

    char* look_object_phrase_text;
    int look_object_phrase_y;
    int look_object_bubble_size;
    bool look_object_phrase_show;

    Hideout* (*hideouts);

    Storage* (*storages)[8];

    RoomID room;
    RoomCollision* room_collisions;

    // Прохождение уровня
    int quota;
    int min_quota;

    int tricks;
    int total_tricks;

    // Инвентарь
    Item inventory[16];
    int selected_item;
    int item_count;

    // Интерфейс
    Hint hints[3];

    float ui_quota;
    char ui_quota_text[32];
    bool ui_quota_animation_play;
    int ui_quota_animation_frame;
    float ui_quota_delta;
    char ui_quota_delta_text[32];
    bool ui_quota_delta_text_show;
    g2dColor ui_quota_text_color;

    char ui_tricks_counter_text[32];
    int ui_tricks_counter_text_x;
    bool ui_tricks_counter_text_show;

    bool inventory_animation_items_show;
    bool inventory_animation_play;
    int inventory_animation_first_index;
    int inventory_animation_last_index;
    int inventory_animation_frame_time;
    int inventory_animation_frame;

    bool ui_tricks_counter_animation_play;
    int ui_tricks_counter_animation_frame_time;
    int ui_tricks_counter_animation_frame;

    // Камера
    int camera_limit_x;
    int camera_limit_y;

    int camera_offset_x;
    int camera_offset_y;

    float new_camera_offset_x;
    float new_camera_offset_y;

    float camera_door_offset_x;
    float camera_door_offset_y;

    LevelEnd* level_end;
    bool* level_end_active;
} Woody;

Woody* woody_create(
    g2dImage* spritelists[16],
    const Frame* gfxdata[16],
    const Animation* animations[16],
    int start_x,
    int start_y,
    hDoor* (*h_doors)[2],
    vDoor* (*v_doors)[3],
    LookObject* (*look_objects)[8],
    Hideout* (*hideouts),
    Storage* (*storages)[8],
    int camera_limit_x,
    int camera_limit_y,
    RoomID room,
    RoomCollision* room_collisions,
    int min_quota,
    int total_tricks,
    LevelEnd* level_end,
    bool* level_end_active
);

void woody_animation_update_frame(Woody* woody);
void woody_animation_set(Woody* woody, int pack, int animation);
void woody_dest_door_animation_update_frame(Woody* woody);
void woody_dest_door_animation_set(Woody* woody, int pack, int animation);
void woody_start_using_h_door(Woody* woody, hDoor* new_enter_door);
void woody_start_using_v_door(Woody* woody, vDoor* new_enter_door);
void woody_update(Woody* woody);
void woody_draw(const Woody* woody);
void woody_door_draw(const Woody* woody);
void woody_draw_ui(const Woody* woody);
void woody_tricks_counter_update(Woody* woody, int trick_quota);
bool woody_check_sign_collision(const Woody* woody, const Sign* sign);
void woody_unload(Woody* woody);

#endif // WOODY_H