#include "neighbour.h"

#include <stdlib.h>

#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"

#include "woody.h"

#define CG_ORANGE_BREAKDOWNS G2D_RGB(254, 159, 0)

extern g2dImage* SpriteAtlas_INGAMEUI;
extern g2dImage* SpriteList_BUBBLES;

extern g2dImage* SpriteList_NEIGHBOUR_GENERIC;

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
    LOOK_OBJECT_TRICK_COUNT
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
    int next_state;
} LookObjectTrickCountArgs;

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

// Сидеть в кресле
static const AnimationPlayTillTheEndArgs action_args_6 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 7};
static const AnimationPlayTillTheEndArgs action_args_7 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT_REMO, 8};
static const AnimationPlayTillTheEndArgs action_args_8 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 9};
static const AnimationPlayTillTheEndArgs action_args_9 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT_REMO, 10};
static const AnimationPlayTillTheEndArgs action_args_10 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_SIT, 11};

// Встать с кресла
static const BubbleSetArgs action_args_11 = {BUBBLE_BINOCULARS, 12};
static const AnimationPlayTillTheEndArgs action_args_12 = {ANIMATION_PACK_NEIGHBOUR_SOFA, ANIMATION_NEIGHBOUR_SOFA_LEAVE, 13};
static const PositionSetArgs action_args_13 = {-66, -60, 14};
static const LookObjectVisibilitySetArgs action_args_14 = {ROOM_LIR, 1, true, 15};

// Уйти от кресла
static const WalkToYArgs action_args_15 = {-56, 16};

// Идти к двери с гостиной на кухню
static const WalkToXArgs action_args_16 = {154, 17};

// Начать использовать дверь с гостиной на кухню
static const StartUsingHDoorArgs action_args_17 = {ROOM_LIR, 1, 18};
static const PositionSetArgs action_args_18 = {108, -53, 19};
static const AnimationPlayTillTheEndArgs action_args_19 = {ANIMATION_PACK_NEIGHBOUR_DOORRIGHT, ANIMATION_NEIGHBOUR_DOORRIGHT_ENTER, 20};

// Закончить использовать дверь с гостиной на кухню
static const EndUsingHDoorArgs action_args_20 = {ROOM_LIR, 1, 21};
static const PositionSetArgs action_args_21 = {208, -54, 22};

// Идти к биноклю
static const WalkToXArgs action_args_22 = {410, 23};

// Проверить, есть ли пакость на бинокле
static const LookObjectCheckToTrickArgs action_args_23 = {ROOM_KIT, 1, 29, 24};

// ---
// Смотреть в бинокль (без пакости)
static const PositionSetArgs action_args_24 = {309, -50, 25};
static const LookObjectVisibilitySetArgs action_args_25 = {ROOM_KIT, 1, false, 26};
static const AnimationPlayTillTheEndArgs action_args_26 = {ANIMATION_PACK_NEIGHBOUR_BINOCULARS, ANIMATION_NEIGHBOUR_BINOCULARS_PEEP, 27};
static const LookObjectVisibilitySetArgs action_args_27 = {ROOM_KIT, 1, true, 28};
static const PositionSetArgs action_args_28 = {410, -54, 38};

// Смотреть в бинокль (пакость)
static const PositionSetArgs action_args_29 = {309, -50, 30};
static const LookObjectVisibilitySetArgs action_args_30 = {ROOM_KIT, 1, false, 31};
static const AnimationPlayTillTheEndArgs action_args_31 = {ANIMATION_PACK_NEIGHBOUR_BINOCULARS, ANIMATION_NEIGHBOUR_BINOCULARS_PEEP_GLUE, 32};
static const PositionSetArgs action_args_32 = {410, -54, 33};
static const BubbleSetArgs action_args_33 = {BUBBLE_WUT, 34};
static const LookObjectTrickCountArgs action_args_34 = {ROOM_KIT, 1, 35};
static const AnimationPlayTillTheEndArgs action_args_35 = {ANIMATION_PACK_NEIGHBOUR_GENERIC2, ANIMATION_NEIGHBOUR_SHOUT2, 36};
static const LookObjectMakeUntrickedArgs action_args_36 = {ROOM_KIT, 1, 37};
static const BubbleSetArgs action_args_37 = {BUBBLE_SOFA, 38};
// ---

// Идти к двери с кухни в гостиную
static const BubbleSetArgs action_args_38 = {BUBBLE_SOFA, 39};
static const WalkToXArgs action_args_39 = {210, 40};

// Начать использовать дверь с кухни на гостиную
static const StartUsingHDoorArgs action_args_40 = {ROOM_KIT, 0, 41};
static const PositionSetArgs action_args_41 = {248, -53, 42};
static const AnimationPlayTillTheEndArgs action_args_42 = {ANIMATION_PACK_NEIGHBOUR_DOORLEFT, ANIMATION_NEIGHBOUR_DOORLEFT_ENTER, 43};

// Закончить использовать дверь с кухни на гостиную
static const EndUsingHDoorArgs action_args_43 = {ROOM_KIT, 0, 44};
static const PositionSetArgs action_args_44 = {148, -56, 1};

const Action actions[] = {
    // Подойти к креслу
    {BUBBLE_SET, &action_args_0, true},
    {WALK_TO_X, &action_args_1, false},
    {WALK_TO_Y, &action_args_2, false},

    // Сесть в кресло
    {POSITION_SET, &action_args_3, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_4, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_5, false},

    // Сидеть в кресле
    {ANIMATION_PLAY_TILL_THE_END, &action_args_6, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_7, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_8, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_9, false},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_10, false},

    // Встать с кресла
    {BUBBLE_SET, &action_args_11, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_12, false},
    {POSITION_SET, &action_args_13, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_14, true},

    // Уйти от кресла
    {WALK_TO_Y, &action_args_15, false},

    // Идти к двери с гостиной на кухню
    {WALK_TO_X, &action_args_16, false},

    // Начать использовать дверь с гостиной на кухню
    {START_USING_H_DOOR, &action_args_17, true},
    {POSITION_SET, &action_args_18, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_19, false},

    // Закончить использовать дверь с гостиной на кухню
    {END_USING_H_DOOR, &action_args_20, true},
    {POSITION_SET, &action_args_21, true},

    // Идти к биноклю
    {WALK_TO_X, &action_args_22, false},

    // Проверить, есть ли пакость на бинокле
    {LOOK_OBJECT_CHECK_TO_TRICK, &action_args_23, true},

    // --
    // Смотреть в бинокль (без пакости)
    {POSITION_SET, &action_args_24, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_25, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_26, false},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_27, true},
    {POSITION_SET, &action_args_28, true},

    // Смотреть в бинокль (пакость)
    {POSITION_SET, &action_args_29, true},
    {LOOK_OBJECT_VISIBILITY_SET, &action_args_30, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_31, false},
    {POSITION_SET, &action_args_32, true},
    {BUBBLE_SET, &action_args_33, true},
    {LOOK_OBJECT_TRICK_COUNT, &action_args_34, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_35, false},
    {LOOK_OBJECT_MAKE_UNTRICKED, &action_args_36, true},
    {BUBBLE_SET, &action_args_37, true},
    // --

    // Идти к двери с кухни в гостиную
    {BUBBLE_SET, &action_args_38, true},
    {WALK_TO_X, &action_args_39, false},

    // Начать использовать дверь с кухни на гостиную
    {START_USING_H_DOOR, &action_args_40, true},
    {POSITION_SET, &action_args_41, true},
    {ANIMATION_PLAY_TILL_THE_END, &action_args_42, false},

    // Закончить использовать дверь с кухни на гостиную
    {END_USING_H_DOOR, &action_args_43, true},
    {POSITION_SET, &action_args_44, true}
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

    // Интерфейс
    neighbour->head_icon_src_x = 47 * neighbour->emotion;
    neighbour->head_icon_show = true;
    neighbour->head_icon_animation_play = false;
    neighbour->head_icon_animation_frame = 0;
    // local breakdownsText = tostring(breakdowns)
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

                neighbour->look_objects[args->room][args->id]->tricked = false;
                neighbour->look_objects[args->room][args->id]->on_untrick();

                neighbour->action_state = args->next_state;

                break;
            }

            case LOOK_OBJECT_TRICK_COUNT: {
                const LookObjectTrickCountArgs* args = (const LookObjectTrickCountArgs*) current_action.args;

                neighbour->head_icon_src_x = 94;

                woody_tricks_counter_update(neighbour->woody, neighbour->look_objects[args->room][args->id]->trick_tv_rating);

                neighbour->action_state = args->next_state;

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
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 3, 230, 47, 42, WHITE, neighbour->head_icon_src_x, 80, 47, 42, 0, 255, G2D_UP_LEFT);
    }

    intraFontSetStyle(Font_BLUEHIGC_24, 0.583, CG_ORANGE_BREAKDOWNS, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGC_24, 1);
    intraFontPrint(Font_BLUEHIGC_24, 457, 257 + intraFontTextHeight(Font_BLUEHIGC_24), "0");

    // Bubble Icon
    if (neighbour->bubble_show) {
        g2d_DrawImageExt(SpriteList_BUBBLES, 0, 193, 46, 37, WHITE, neighbour->bubble_sprite_src_x, neighbour->bubble_sprite_src_y, 46, 37, 0, 255, G2D_UP_LEFT);
    }
}

void neighbour_unload(Neighbour* neighbour) {
    free(neighbour);
}
