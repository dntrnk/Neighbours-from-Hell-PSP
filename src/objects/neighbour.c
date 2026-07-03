#include "neighbour.h"

#include <stdlib.h>
#include <math.h>

#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"

#include "woody.h"

#define CG_ORANGE_BREAKDOWNS G2D_RGB(254, 159, 0)

extern g2dImage* SpriteAtlas_INGAMEUI;
extern g2dImage* SpriteList_BUBBLES;

extern g2dImage* SpriteList_NEIGHBOUR_GENERIC;

extern g2dImage* Sprite_Angrybar;

extern intraFont* Font_BLUEHIGC_24;

extern const Frame ui_bubble_frames[];

extern int camera_x;
extern int camera_y;

typedef enum {
    WALK_TO_X,
    WALK_TO_Y,
    BUBBLE_SET,
    POSITION_SET,
    START_USING_H_DOOR,
    END_USING_H_DOOR,
    ANIMATION_PLAY_TILL_THE_END,
    LOOK_OBJECT_VISIBILITY_SET,
    LOOK_OBJECT_CHECK_TO_TRICK,
    LOOK_OBJECT_MAKE_UNTRICKED,
    LOOK_OBJECT_TRICK_COUNT,
    LOOK_OBJECT_CHECK_TO_ALT_ACTION
} ActionType;

typedef struct {
    int x;
    int next_state;
} WalkToXArgs;

typedef struct {
    int y;
    int next_state;
} WalkToYArgs;

typedef struct {
    int new_bubble_id;
    int next_state;
} BubbleSetArgs;

typedef struct {
    int x, y;
    int next_state;
} PositionSetArgs;

typedef struct {
    RoomID room;
    int side;
    int next_state;
} StartUsingHDoorArgs;

typedef struct {
    RoomID room;
    int side;
    int next_state;
} EndUsingHDoorArgs;

typedef struct {
    int animation_pack, animation_index;
    int next_state;
} AnimationPlayTillTheEndArgs;

typedef struct {
    int room, id;
    bool visibility;
    int next_state;
} LookObjectVisibilitySetArgs;

typedef struct {
    int room, id;
    int trick_state;
    int no_trick_state;
} LookObjectCheckToTrickArgs;

typedef struct {
    int room, id;
    int next_state;
} LookObjectMakeUntrickedArgs;

typedef struct {
    int room, id;
    int breakdown_state;
    int no_breakdown_state;
} LookObjectTrickCountArgs;

typedef struct {
    int room, id;
    int alt_state;
    int no_alt_state;
} LookObjectCheckToAltActionArgs;

typedef struct {
    ActionType action;
    const void* args;
    bool instant;
} Action;

// Подойти к креслу
static const BubbleSetArgs action_args_0 = {BUBBLE_SOFA, 1};
static const WalkToXArgs action_args_1 = {-66, 2};
static const WalkToYArgs action_args_2 = {-60, 3};

// Сесть в кресло
static const PositionSetArgs action_args_3 = {2, -53, 4};
static const LookObjectVisibilitySetArgs action_args_4 = {ROOM_LIR, 1, false, 5};
static const AnimationPlayTillTheEndArgs action_args_5 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_ENTER, 6};

// Проверить, есть ли пакость в кресле
static const LookObjectCheckToTrickArgs action_args_6 = {ROOM_LIR, 1, 7, 17};

// В кресле есть пакость
static const AnimationPlayTillTheEndArgs action_args_7 = {ANIMATION_PACK_NEIGHBOUR_SOFA1, ANIMATION_NEIGHBOUR_SOFA1_NEIGHBOUR_SURPRISE, 8};
static const AnimationPlayTillTheEndArgs action_args_8 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_LEAVE, 9};
static const BubbleSetArgs action_args_9 = {BUBBLE_WUT, 10};
static const PositionSetArgs action_args_10 = {-66, -60, 11};
static const LookObjectVisibilitySetArgs action_args_11 = {ROOM_LIR, 1, true, 12};
static const LookObjectTrickCountArgs action_args_12 = {ROOM_LIR, 1, 14, 13};
static const AnimationPlayTillTheEndArgs action_args_13 = {ANIMATION_PACK_NEIGHBOUR_SHOUT0, ANIMATION_NEIGHBOUR_SHOUT0, 15};
static const AnimationPlayTillTheEndArgs action_args_14 = {ANIMATION_PACK_NEIGHBOUR_GENERIC2, ANIMATION_NEIGHBOUR_SHOUT2_EXTRA, 15};
static const LookObjectMakeUntrickedArgs action_args_15 = {ROOM_LIR, 1, 16};
static const BubbleSetArgs action_args_16 = {BUBBLE_SOFA, 3};

// Сидеть в кресле (без пакости)
static const AnimationPlayTillTheEndArgs action_args_17 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 18};
static const AnimationPlayTillTheEndArgs action_args_18 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT_REMO, 19};
static const AnimationPlayTillTheEndArgs action_args_19 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 20};
static const AnimationPlayTillTheEndArgs action_args_20 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT_REMO, 21};
static const AnimationPlayTillTheEndArgs action_args_21 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 22};

// Встать с кресла
static const BubbleSetArgs action_args_22 = {BUBBLE_BINOCULARS, 23};
static const AnimationPlayTillTheEndArgs action_args_23 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_LEAVE, 24};
static const PositionSetArgs action_args_24 = {-66, -60, 25};
static const LookObjectVisibilitySetArgs action_args_25 = {ROOM_LIR, 1, true, 26};

// Уйти от кресла
static const WalkToYArgs action_args_26 = {-56, 27};

// Идти к двери с гостиной на кухню
static const WalkToXArgs action_args_27 = {154, 28};

// Начать использовать дверь с гостиной на кухню
static const StartUsingHDoorArgs action_args_28 = {ROOM_LIR, 1, 29};
static const PositionSetArgs action_args_29 = {108, -53, 30};
static const AnimationPlayTillTheEndArgs action_args_30 = {ANIMATION_PACK_NEIGHBOUR_DOORRIGHT, ANIMATION_NEIGHBOUR_DOORRIGHT_ENTER, 31};

// Закончить использовать дверь с гостиной на кухню
static const EndUsingHDoorArgs action_args_31 = {ROOM_LIR, 1, 32};
static const PositionSetArgs action_args_32 = {208, -54, 33};

// Идти мимо микроволновки
static const WalkToXArgs action_args_33 = {228, 34};

// Проверить, грязная ли микроволновка
static const LookObjectCheckToTrickArgs action_args_34 = {ROOM_KIT, 0, 35, 45};

// Микроволновка грязная
static const AnimationPlayTillTheEndArgs action_args_35 = {ANIMATION_PACK_NEIGHBOUR_GENERIC3, ANIMATION_NEIGHBOUR_DOUBLETAKE1, 36};
static const BubbleSetArgs action_args_36 = {BUBBLE_WUT, 37};
static const LookObjectTrickCountArgs action_args_37 = {ROOM_KIT, 0, 39, 38};
static const AnimationPlayTillTheEndArgs action_args_38 = {ANIMATION_PACK_NEIGHBOUR_SHOUT0, ANIMATION_NEIGHBOUR_SHOUT0, 40};
static const AnimationPlayTillTheEndArgs action_args_39 = {ANIMATION_PACK_NEIGHBOUR_GENERIC2, ANIMATION_NEIGHBOUR_SHOUT2_EXTRA, 40};

// Подойти к микроволновке
static const WalkToYArgs action_args_40 = {-66, 41};

// Почистить микроволновку
static const AnimationPlayTillTheEndArgs action_args_41 = {ANIMATION_PACK_NEIGHBOUR_GENERIC3, ANIMATION_NEIGHBOUR_USE_MID, 42};
static const LookObjectMakeUntrickedArgs action_args_42 = {ROOM_KIT, 0, 43};
static const BubbleSetArgs action_args_43 = {BUBBLE_BINOCULARS, 44};

// Уйти от микроволновки
static const WalkToYArgs action_args_44 = {-54, 45};

// Идти к биноклю
static const WalkToXArgs action_args_45 = {410, 46};

// Проверить, есть ли пакость на бинокле
static const LookObjectCheckToTrickArgs action_args_46 = {ROOM_KIT, 1, 54, 47};

// Смотреть в бинокль (без пакости)
static const LookObjectCheckToAltActionArgs action_args_47 = {ROOM_KIT, 1, 53, 48};

// Проверить, есть ли бинокль

// Бинокль на месте
static const PositionSetArgs action_args_48 = {309, -50, 49};
static const LookObjectVisibilitySetArgs action_args_49 = {ROOM_KIT, 1, false, 50};
static const AnimationPlayTillTheEndArgs action_args_50 = {ANIMATION_PACK_NEIGHBOUR_BINOCULARS, ANIMATION_NEIGHBOUR_BINOCULARS_PEEP, 51};
static const LookObjectVisibilitySetArgs action_args_51 = {ROOM_KIT, 1, true, 52};
static const PositionSetArgs action_args_52 = {410, -54, 64};

// Бинокля нет
static const AnimationPlayTillTheEndArgs action_args_53 = {ANIMATION_PACK_NEIGHBOUR_LOOK, ANIMATION_NEIGHBOUR_LOOK1, 64};

// Смотреть в бинокль (пакость)
static const PositionSetArgs action_args_54 = {309, -50, 55};
static const LookObjectVisibilitySetArgs action_args_55 = {ROOM_KIT, 1, false, 56};
static const AnimationPlayTillTheEndArgs action_args_56 = {ANIMATION_PACK_NEIGHBOUR_BINOCULARS, ANIMATION_NEIGHBOUR_BINOCULARS_PEEP_GLUE, 57};
static const PositionSetArgs action_args_57 = {410, -54, 58};
static const BubbleSetArgs action_args_58 = {BUBBLE_WUT, 59};
static const LookObjectTrickCountArgs action_args_59 = {ROOM_KIT, 1, 61, 60};
static const AnimationPlayTillTheEndArgs action_args_60 = {ANIMATION_PACK_NEIGHBOUR_GENERIC2, ANIMATION_NEIGHBOUR_SHOUT2, 62};
static const AnimationPlayTillTheEndArgs action_args_61 = {ANIMATION_PACK_NEIGHBOUR_GENERIC2, ANIMATION_NEIGHBOUR_SHOUT2_EXTRA, 62};
static const LookObjectMakeUntrickedArgs action_args_62 = {ROOM_KIT, 1, 63};
static const BubbleSetArgs action_args_63 = {BUBBLE_SOFA, 64};

// Идти к двери с кухни в гостиную
static const BubbleSetArgs action_args_64 = {BUBBLE_SOFA, 65};
static const WalkToXArgs action_args_65 = {210, 66};

// Начать использовать дверь с кухни на гостиную
static const StartUsingHDoorArgs action_args_66 = {ROOM_KIT, 0, 67};
static const PositionSetArgs action_args_67 = {248, -53, 68};
static const AnimationPlayTillTheEndArgs action_args_68 = {ANIMATION_PACK_NEIGHBOUR_DOORLEFT, ANIMATION_NEIGHBOUR_DOORLEFT_ENTER, 69};

// Закончить использовать дверь с кухни на гостиную
static const EndUsingHDoorArgs action_args_69 = {ROOM_KIT, 0, 70};
static const PositionSetArgs action_args_70 = {148, -56, 1};

const Action actions[] = {
    // Подойти к креслу
    {BUBBLE_SET, &action_args_0, true},
    {WALK_TO_X, &action_args_1, false},
    {WALK_TO_Y, &action_args_2, false},

    // Сесть в кресло
    {POSITION_SET, &action_args_3, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_4, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_5, false},

    // Проверить, есть ли пакость в кресле
    {LOOK_OBJECT_CHECK_TO_TRICK, &action_args_6, true},

    // В кресле есть пакость
    {ANIMATION_PLAY_TILL_THE_END, &action_args_7, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_8, false},
    {BUBBLE_SET, &action_args_9, true},
    {POSITION_SET, &action_args_10, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_11, true},
    {LOOK_OBJECT_TRICK_COUNT, &action_args_12, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_13, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_14, false},
    {LOOK_OBJECT_MAKE_UNTRICKED, &action_args_15, true},
    {BUBBLE_SET, &action_args_16, true},

    // Сидеть в кресле (без пакости)
    {ANIMATION_PLAY_TILL_THE_END, &action_args_17, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_18, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_19, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_20, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_21, false},

    // Встать с кресла
    {BUBBLE_SET, &action_args_22, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_23, false},
    {POSITION_SET, &action_args_24, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_25, true},

    // Уйти от кресла
    {WALK_TO_Y, &action_args_26, false},

    // Идти к двери с гостиной на кухню
    {WALK_TO_X, &action_args_27, false},

    // Начать использовать дверь с гостиной на кухню
    {START_USING_H_DOOR, &action_args_28, true},
    {POSITION_SET, &action_args_29, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_30, false},

    // Закончить использовать дверь с гостиной на кухню
    {END_USING_H_DOOR, &action_args_31, true},
    {POSITION_SET, &action_args_32, true},

    // Идти мимо микроволновки
    {WALK_TO_X, &action_args_33, false},

    // Проверить, грязная ли микроволновка
    {LOOK_OBJECT_CHECK_TO_TRICK, &action_args_34, true},

    // Микроволновка грязная
    {ANIMATION_PLAY_TILL_THE_END, &action_args_35, false},
    {BUBBLE_SET, &action_args_36, true},
    {LOOK_OBJECT_TRICK_COUNT, &action_args_37, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_38, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_39, false},

    // Подойти к микроволновке
    {WALK_TO_Y, &action_args_40, false},

    // Почистить микроволновку
    {ANIMATION_PLAY_TILL_THE_END, &action_args_41, false},
    {LOOK_OBJECT_MAKE_UNTRICKED, &action_args_42, true},
    {BUBBLE_SET, &action_args_43, true},

    // Уйти от микроволновки
    {WALK_TO_Y, &action_args_44, false},

    // Идти к биноклю
    {WALK_TO_X, &action_args_45, false},

    // Проверить, есть ли пакость на бинокле
    {LOOK_OBJECT_CHECK_TO_TRICK, &action_args_46, true},

    // Смотреть в бинокль (без пакости)

    // Проверить, есть ли бинокль
    {LOOK_OBJECT_CHECK_TO_ALT_ACTION, &action_args_47, true},

    // Бинокль на месте
    {POSITION_SET, &action_args_48, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_49, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_50, false},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_51, true},
    {POSITION_SET, &action_args_52, true},

    // Бинокля нет
    {ANIMATION_PLAY_TILL_THE_END, &action_args_53, false},

    // Смотреть в бинокль (пакость)
    {POSITION_SET, &action_args_54, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_55, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_56, false},
    {POSITION_SET, &action_args_57, true},
    {BUBBLE_SET, &action_args_58, true},
    {LOOK_OBJECT_TRICK_COUNT, &action_args_59, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_60, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_61, false},
    {LOOK_OBJECT_MAKE_UNTRICKED, &action_args_62, true},
    {BUBBLE_SET, &action_args_63, true},

    // Идти к двери с кухни в гостиную
    {BUBBLE_SET, &action_args_64, true},
    {WALK_TO_X, &action_args_65, false},

    // Начать использовать дверь с кухни на гостиную
    {START_USING_H_DOOR, &action_args_66, true},
    {POSITION_SET, &action_args_67, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_68, false},

    // Закончить использовать дверь с кухни на гостиную
    {END_USING_H_DOOR, &action_args_69, true},
    {POSITION_SET, &action_args_70, true}
};

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
    Woody* woody
) {
    Neighbour* neighbour = malloc(sizeof(Neighbour));
    memset(neighbour, 0, sizeof(Neighbour));

    // Анимации
    for (int i = 0; i < 16; i++) {
        neighbour->spritelists[i] = spritelists[i];
        neighbour->gfxdata[i] = gfxdata[i];
        neighbour->animations[i] = animations[i];
    }

    neighbour->current_spritelist = 0;

    neighbour->current_animation_pack = -1;
    neighbour->current_animation_index = -1;

    neighbour->animation_loop = false;
    neighbour->animation_frame = 0;
    neighbour->animation_length = 0;

    neighbour->sprite_src_x = 362;
    neighbour->sprite_src_y = 0;
    neighbour->sprite_w = 63;
    neighbour->sprite_h = 59;
    neighbour->sprite_offset_x = 207;
    neighbour->sprite_offset_y = 165;

    neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MS2);

    // Анимации дверей
    neighbour->enter_door = NULL;
    neighbour->dest_door = NULL;

    neighbour->dest_door_animation_play = false;

    neighbour->dest_door_current_spritelist = 0;

    neighbour->dest_door_current_animation_pack = -1;
    neighbour->dest_door_current_animation_index = -1;

    neighbour->dest_door_animation_frame = 0;
    neighbour->dest_door_animation_frame_time = 0;
    neighbour->dest_door_animation_length = 0;

    neighbour->dest_door_sprite_x = 0;
    neighbour->dest_door_sprite_y = 0;
    neighbour->dest_door_sprite_src_x = 0;
    neighbour->dest_door_sprite_src_y = 0;
    neighbour->dest_door_sprite_w = 0;
    neighbour->dest_door_sprite_h = 0;
    neighbour->dest_door_sprite_offset_x = 0;
    neighbour->dest_door_sprite_offset_y = 0;

    // Основа соседа
    neighbour->x = start_x;
    neighbour->y = start_y;

    neighbour->action_state = 0;
    neighbour->action_done = false;

    neighbour->in_door = false;

    neighbour->h_doors = h_doors;
    neighbour->v_doors = v_doors;

    neighbour->look_objects = look_objects;

    neighbour->room = start_room;

    neighbour->woody = woody;

    // Прохождение уровня
    neighbour->emotion = 0;
    neighbour->breakdowns = 0;
    neighbour->angry = 0.0f;
    neighbour->angry_cooldown = 0;

    neighbour->jingle_joke_playing = false;
    neighbour->jingle_joke_timer = 0;

    // Интерфейс
    neighbour->head_icon_src_x = 47 * neighbour->emotion;
    neighbour->head_icon_show = true;
    neighbour->head_icon_animation_play = false;
    neighbour->head_icon_animation_frame = 0;
    
    neighbour->current_bubble = start_bubble;

    {
        int src_x = neighbour->current_bubble * 46;
        int src_y = 0;

        src_y = (src_x / 506) * 37;
        src_x = src_x % 506;

        neighbour->bubble_sprite_src_x = src_x;
        neighbour->bubble_sprite_src_y = src_y;
    }

    neighbour->bubble_show = false;

    strcpy(neighbour->ui_breakdowns_counter_text, "0");
    neighbour->ui_breakdowns_counter_text_x = 459 - floor(intraFontMeasureText(Font_BLUEHIGC_24, neighbour->ui_breakdowns_counter_text) * 0.5);;
    neighbour->ui_breakdowns_counter_text_show = true;

    neighbour->ui_breakdowns_counter_animation_play = false;
    neighbour->ui_breakdowns_counter_animation_frame_time = 0;
    neighbour->ui_breakdowns_counter_animation_frame = 0;

    return neighbour;
}

void neighbour_animation_update_frame(Neighbour* neighbour) {
    const Animation* current_animation = &neighbour->animations[neighbour->current_animation_pack][neighbour->current_animation_index];
    int animation_frame = current_animation->frames[neighbour->animation_frame];
    const Frame* frame = &neighbour->gfxdata[neighbour->current_animation_pack][animation_frame];

    neighbour->sprite_src_x = frame->src_x;
    neighbour->sprite_src_y = frame->src_y;
    neighbour->sprite_w = frame->w;
    neighbour->sprite_h = frame->h;
    neighbour->sprite_offset_x = frame->offset_x;
    neighbour->sprite_offset_y = frame->offset_y;
    neighbour->current_spritelist = frame->spritelist_index;

    int current_sound = current_animation->sfx[neighbour->animation_frame];

    if (current_sound != SOUND_NONE)
        NFHSoundPlay(current_sound);
}


void neighbour_animation_set(Neighbour* neighbour, int pack, int animation) {
    if (neighbour->current_animation_pack == pack && neighbour->current_animation_index == animation) {
        return;
    }

    neighbour->current_animation_pack = pack;
    neighbour->current_animation_index = animation;
    neighbour->animation_frame = 0;

    const Animation* new_animation = &neighbour->animations[pack][animation];
    neighbour->animation_loop = new_animation->loop;
    neighbour->animation_length = new_animation->frame_count;

    neighbour_animation_update_frame(neighbour);
}

void neighbour_dest_door_animation_update_frame(Neighbour* neighbour) {
    const Animation* current_animation = &neighbour->animations[neighbour->dest_door_current_animation_pack][neighbour->dest_door_current_animation_index];
    int animation_frame = current_animation->frames[neighbour->dest_door_animation_frame];
    const Frame* frame = &neighbour->gfxdata[neighbour->dest_door_current_animation_pack][animation_frame];

    neighbour->dest_door_sprite_src_x = frame->src_x;
    neighbour->dest_door_sprite_src_y = frame->src_y;
    neighbour->dest_door_sprite_w = frame->w;
    neighbour->dest_door_sprite_h = frame->h;
    neighbour->dest_door_sprite_offset_x = frame->offset_x;
    neighbour->dest_door_sprite_offset_y = frame->offset_y;
    neighbour->dest_door_current_spritelist = frame->spritelist_index;

    int current_sound = current_animation->sfx[neighbour->dest_door_animation_frame];

    if (current_sound != SOUND_NONE)
        NFHSoundPlay(current_sound);
}

void neighbour_dest_door_animation_set(Neighbour* neighbour, int pack, int animation) {
    if (neighbour->dest_door_current_animation_pack == pack && neighbour->dest_door_current_animation_index == animation) {
        return;
    }

    neighbour->dest_door_current_animation_pack = pack;
    neighbour->dest_door_current_animation_index = animation;
    neighbour->dest_door_animation_frame = 0;

    const Animation* new_animation = &neighbour->animations[pack][animation];
    neighbour->dest_door_animation_length = new_animation->frame_count;

    neighbour_dest_door_animation_update_frame(neighbour);
}

void neighbour_update(Neighbour* neighbour) {
    bool need_to_execute = true;
    while (need_to_execute) {
        Action current_action = actions[neighbour->action_state];
        need_to_execute = current_action.instant;
        switch (current_action.action) {
            case WALK_TO_X: {
                const WalkToXArgs* args = (const WalkToXArgs*) current_action.args;

                int goal_x = args->x;
                bool action_ended = false;

                if (goal_x > neighbour->x) {
                    neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MG1);

                    neighbour->x += 4;

                    if (goal_x <= neighbour->x) {
                        action_ended = true;
                    }
                } else if (goal_x < neighbour->x) {
                    neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MG3);

                    neighbour->x -= 4;

                    if (goal_x >= neighbour->x) {
                        action_ended = true;
                    }
                } else {
                    action_ended = true;
                }

                if (action_ended) {
                    neighbour->x = goal_x;
                    neighbour->action_state = args->next_state;
                }

                break;
            }

            case WALK_TO_Y: {
                const WalkToYArgs* args = (const WalkToYArgs*) current_action.args;

                int goal_y = args->y;
                bool action_ended = false;

                if (goal_y > neighbour->y) {
                    neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MG2);

                    neighbour->y += 1;

                    if (goal_y <= neighbour->y) {
                        action_ended = true;
                    }
                } else if (goal_y < neighbour->y) {
                    neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MG0);

                    neighbour->y -= 1;

                    if (goal_y >= neighbour->y) {
                        action_ended = true;
                    }
                } else {
                    action_ended = true;
                }

                if (action_ended) {
                    neighbour->y = goal_y;
                    neighbour->action_state = args->next_state;
                }

                break;
            }

            case BUBBLE_SET: {
                const BubbleSetArgs* args = (const BubbleSetArgs*) current_action.args;
                
                int new_bubble_id = args->new_bubble_id;
                int src_x = new_bubble_id * 46;
                int src_y = 0;

                src_y = (src_x / 506) * 37;
                src_x = src_x % 506;

                neighbour->current_bubble = new_bubble_id;
                neighbour->bubble_sprite_src_x = src_x;
                neighbour->bubble_sprite_src_y = src_y;

                neighbour->action_state = args->next_state;

                break;
            }

            case POSITION_SET: {
                const PositionSetArgs* args = (const PositionSetArgs*) current_action.args;
                
                neighbour->x = args->x;
                neighbour->y = args->y;

                neighbour->action_state = args->next_state;

                break;
            }

            case START_USING_H_DOOR: {
                const StartUsingHDoorArgs* args = (const StartUsingHDoorArgs*) current_action.args;

                hDoor* current_door = neighbour->h_doors[args->room][args->side];

                if (current_door->using_by == USING_NONE) {
                    hDoor* new_dest_door = neighbour->h_doors[current_door->dest_door_room][current_door->dest_door_id];

                    neighbour->room = current_door->dest_door_room;
                    neighbour->in_door = true;

                    current_door->using_by = USING_NEIGHBOUR;
                    new_dest_door->using_by = USING_NEIGHBOUR;

                    if (args->side == 0) {
                        // Оффсеты...
                        neighbour->dest_door_sprite_x = new_dest_door->sprite_x - 293;
                        neighbour->dest_door_sprite_y = new_dest_door->sprite_y - 129;

                        neighbour_dest_door_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_DOORRIGHT, ANIMATION_NEIGHBOUR_DOORRIGHT_LEAVE);
                    } else {
                        // Оффсеты...
                        neighbour->dest_door_sprite_x = new_dest_door->sprite_x - 174;
                        neighbour->dest_door_sprite_y = new_dest_door->sprite_y - 129;

                        neighbour_dest_door_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_DOORLEFT, ANIMATION_NEIGHBOUR_DOORLEFT_LEAVE);
                    }

                    neighbour->dest_door_animation_play = true;

                    neighbour->action_state = args->next_state;
                } else {
                    if (args->side == 0) {
                        neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MS3);
                    } else {
                        neighbour_animation_set(neighbour, ANIMATION_PACK_NEIGHBOUR_GENERIC, ANIMATION_NEIGHBOUR_MS1);
                    }
                    need_to_execute = false;
                }

                break;
            }

            case END_USING_H_DOOR: {
                const EndUsingHDoorArgs* args = (const EndUsingHDoorArgs*) current_action.args;

                hDoor* enter_door = neighbour->h_doors[args->room][args->side];
                hDoor* dest_door = neighbour->h_doors[enter_door->dest_door_room][enter_door->dest_door_id];

                neighbour->dest_door_animation_play = false;
                neighbour->in_door = false;

                enter_door->using_by = USING_NONE;
                dest_door->using_by = USING_NONE;

                neighbour->action_state = args->next_state;

                break;
            }

            case ANIMATION_PLAY_TILL_THE_END: {
                const AnimationPlayTillTheEndArgs* args = (const AnimationPlayTillTheEndArgs*) current_action.args;
                
                if (neighbour->current_animation_pack != args->animation_pack || neighbour->current_animation_index != args->animation_index) {
                    neighbour_animation_set(neighbour, args->animation_pack, args->animation_index);
                } else {
                    if (neighbour->animation_frame == neighbour->animation_length - 2) { // Так надо, иначе последний кадр два раза рисуется лол
                        neighbour->action_state = args->next_state;
                    }
                }

                break;
            }
            
            case LOOK_OBJECT_VISIBILITY_SET: {
                const LookObjectVisibilitySetArgs* args = (const LookObjectVisibilitySetArgs*) current_action.args;

                neighbour->look_objects[args->room][args->id]->sprite_show = args->visibility;

                neighbour->action_state = args->next_state;

                break;
            }

            case LOOK_OBJECT_CHECK_TO_TRICK: {
                const LookObjectCheckToTrickArgs* args = (const LookObjectCheckToTrickArgs*) current_action.args;

                const LookObject* current_look_object = neighbour->look_objects[args->room][args->id];

                if (current_look_object->tricked) {
                    neighbour->head_icon_animation_frame = 0;
                    neighbour->head_icon_animation_play = true;
                }

                neighbour->action_state = (current_look_object->tricked) ? args->trick_state : args->no_trick_state;

                break;
            }

            case LOOK_OBJECT_MAKE_UNTRICKED: {
                const LookObjectMakeUntrickedArgs* args = (const LookObjectMakeUntrickedArgs*) current_action.args;

                LookObject* current_look_object = neighbour->look_objects[args->room][args->id];

                current_look_object->tricked = false;

                if (current_look_object->on_untrick) {
                    neighbour->look_objects[args->room][args->id]->on_untrick();
                }

                neighbour->action_state = args->next_state;

                break;
            }

            case LOOK_OBJECT_TRICK_COUNT: {
                const LookObjectTrickCountArgs* args = (const LookObjectTrickCountArgs*) current_action.args;

                if (neighbour->angry == 0.0f) {
                    woody_tricks_counter_update(neighbour->woody, neighbour->look_objects[args->room][args->id]->trick_tv_rating);

                    neighbour->emotion = 2;
                    neighbour->head_icon_src_x = 47 * neighbour->emotion;

                    neighbour->action_state = args->no_breakdown_state;
                } else {
                    woody_tricks_counter_update(neighbour->woody, neighbour->look_objects[args->room][args->id]->trick_tv_rating);

                    neighbour->emotion = 3;
                    neighbour->head_icon_src_x = 47 * neighbour->emotion;

                    neighbour_breakdown_counter_update(neighbour);
                    neighbour->action_state = args->breakdown_state;
                }
                
                neighbour->angry = 100.0f;
                neighbour->angry_cooldown = 53;

                NFHHouseMusicPause();
                NFHSoundPlay(SOUND_JINGLE_JOKE);

                if (rand() % 2 == 0) {
                    NFHSoundPlay(SOUND_BIG2);
                } else {
                    NFHSoundPlay(SOUND_BIG3);
                }

                neighbour->jingle_joke_playing = true;
                neighbour->jingle_joke_timer = 120;

                break;
            }

            case LOOK_OBJECT_CHECK_TO_ALT_ACTION: {
                const LookObjectCheckToAltActionArgs* args = (const LookObjectCheckToAltActionArgs*) current_action.args;

                const LookObject* current_look_object = neighbour->look_objects[args->room][args->id];

                neighbour->action_state = (current_look_object->alt_action) ? args->alt_state : args->no_alt_state;

                break;
            }
        }
    }

    neighbour->animation_frame++;
    if (neighbour->animation_frame == neighbour->animation_length) {
        if (neighbour->animation_loop) {
            neighbour->animation_frame = 0;
        } else {
            neighbour->animation_frame = neighbour->animation_length - 1;
        }
    }
    neighbour_animation_update_frame(neighbour);

    if (neighbour->dest_door_animation_play) {
        neighbour->dest_door_animation_frame++;
        if (neighbour->dest_door_animation_frame == neighbour->dest_door_animation_length) {
            neighbour->dest_door_animation_frame = neighbour->dest_door_animation_length - 1;
        }
        neighbour_dest_door_animation_update_frame(neighbour);
    }

    if (neighbour->head_icon_animation_play) {
        neighbour->head_icon_animation_frame++;
        if (neighbour->head_icon_animation_frame % 2 == 0) {
            neighbour->head_icon_show = !neighbour->head_icon_show;
        }

        if (neighbour->head_icon_animation_frame == 12) {
            neighbour->head_icon_animation_play = false;
        }
    }

    if (neighbour->angry_cooldown == 0) {
        if (neighbour->angry != 0.0f) {
            neighbour->angry -= 0.36f;
            if (neighbour->angry <= 0.0f) {
                neighbour->angry = 0.0f;

                neighbour->emotion = 0;
                neighbour->head_icon_src_x = 47 * neighbour->emotion;
            }
        }
    } else {
        neighbour->angry_cooldown--;
        if (neighbour->angry_cooldown == 0) {
            neighbour->emotion = 1;
            neighbour->head_icon_src_x = 47 * neighbour->emotion;
        }
    }
}

void neighbour_draw(const Neighbour* neighbour) {
    g2d_DrawImageExt(neighbour->spritelists[neighbour->current_spritelist], neighbour->x + neighbour->sprite_offset_x - camera_x, neighbour->y + neighbour->sprite_offset_y - camera_y, neighbour->sprite_w, neighbour->sprite_h, WHITE, neighbour->sprite_src_x, neighbour->sprite_src_y, neighbour->sprite_w, neighbour->sprite_h, 0, 255, G2D_UP_LEFT);
}

void neighbour_door_draw(const Neighbour* neighbour) {
    // Enter Door
    g2d_DrawImageExt(neighbour->spritelists[neighbour->current_spritelist], neighbour->x + neighbour->sprite_offset_x - camera_x, neighbour->y + neighbour->sprite_offset_y - camera_y, neighbour->sprite_w, neighbour->sprite_h, WHITE, neighbour->sprite_src_x, neighbour->sprite_src_y, neighbour->sprite_w, neighbour->sprite_h, 0, 255, G2D_UP_LEFT);

    // Destination Door
    g2d_DrawImageExt(neighbour->spritelists[neighbour->dest_door_current_spritelist], neighbour->dest_door_sprite_x + neighbour->dest_door_sprite_offset_x - camera_x, neighbour->dest_door_sprite_y + neighbour->dest_door_sprite_offset_y - camera_y, neighbour->dest_door_sprite_w, neighbour->dest_door_sprite_h, WHITE, neighbour->dest_door_sprite_src_x, neighbour->dest_door_sprite_src_y, neighbour->dest_door_sprite_w, neighbour->dest_door_sprite_h, 0, 255, G2D_UP_LEFT);
}

void neighbour_draw_ui(const Neighbour* neighbour) {
    // Head Icon
    if (neighbour->head_icon_show) {
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 3, 230, 44, 42, WHITE, neighbour->head_icon_src_x, 80, 44, 42, 0, 255, G2D_UP_LEFT);
    }

    const int angrybar_height = 65 * (neighbour->angry * 0.01f);
    g2d_DrawImageExt(Sprite_Angrybar, 51, 268 - angrybar_height, 16, angrybar_height, WHITE, 0, 65 - angrybar_height, 16, angrybar_height, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_BLUEHIGC_24, 0.583, CG_ORANGE_BREAKDOWNS, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGC_24, 1);

    if (neighbour->ui_breakdowns_counter_text_show) {
        intraFontPrint(Font_BLUEHIGC_24, neighbour->ui_breakdowns_counter_text_x, 257 + intraFontTextHeight(Font_BLUEHIGC_24), neighbour->ui_breakdowns_counter_text);
    }

    // Bubble Icon
    if (neighbour->bubble_show) {
        g2d_DrawImageExt(SpriteList_BUBBLES, 0, 193, 46, 37, WHITE, neighbour->bubble_sprite_src_x, neighbour->bubble_sprite_src_y, 46, 37, 0, 255, G2D_UP_LEFT);
    }
}

void neighbour_breakdown_counter_update(Neighbour* neighbour) {
    neighbour->ui_breakdowns_counter_animation_play = true;
    neighbour->ui_breakdowns_counter_animation_frame_time = 0;
    neighbour->ui_breakdowns_counter_animation_frame = 0;
}

void neighbour_unload(Neighbour* neighbour) {
    free(neighbour);
}
