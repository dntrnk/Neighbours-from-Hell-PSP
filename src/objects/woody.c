#include "woody.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../engine/controls/controls.h"
#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"

#include "../objects/scene_manager.h"

#define CG_YELLOW_TRICKS G2D_RGB(239, 239, 0)
#define CG_BLUE_TIMER G2D_RGB(121, 181, 181)
#define COLOR_GREEN_PERCENT G2D_RGB(132, 175, 109)
#define COLOR_WHITE_PERCENT G2D_RGB(254, 254, 254)
#define COLOR_GRAY_BUBBLE G2D_RGB(158, 158, 158)

#define MAX_HINTS 3
#define MAX_ITEMS_IN_INVENTORY 16

#define WOODY_INTERACT_DISTANCE 36
#define CAMERA_STICK_DEADZONE 65

#define IS_LAST_ANIMATION_FRAME (woody->animation_frame == woody->animation_length - 1 && woody->animation_frame_time == 4)

extern Scene Tutorial1Scene;

extern g2dImage* SpriteAtlas_INGAMEUI;

extern g2dImage* SpriteList_BUTTONS;
extern g2dImage* SpriteList_ITEMS_1;
extern g2dImage* SpriteList_ITEMS_2;
extern g2dImage* SpriteList_PROGRESSBAR;

extern intraFont* Font_ACMESA;
extern intraFont* Font_BLUEHIGH_8;
extern intraFont* Font_BLUEHIGC_11;
extern intraFont* Font_BLUEHIGC_24;
extern intraFont* Font_BLUEHIGB_18;

extern int camera_x;
extern int camera_y;

static inline int clamp(int x, int min, int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

static inline float lerp(float a, float b, float t) {
    return b + (a - b) * t;
}

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
    Neighbour* neighbour,
    LevelEnd* level_end,
    bool* level_end_active
) {
    Woody* woody = malloc(sizeof(Woody));
    memset(woody, 0, sizeof(Woody));

    // Анимации
    for (int i = 0; i < 16; i++) {
        woody->spritelists[i] = spritelists[i];
        woody->gfxdata[i] = gfxdata[i];
        woody->animations[i] = animations[i];
    }

    woody->current_spritelist = 0;

    woody->current_animation_pack = -1;
    woody->current_animation_index = -1;

    woody->animation_loop = false;
    woody->animation_frame = 0;
    woody->animation_frame_time = 0;
    woody->animation_length = 0;

    woody->sprite_src_x = 0;
    woody->sprite_src_y = 0;
    woody->sprite_w = 0;
    woody->sprite_h = 0;
    woody->sprite_offset_x = 0;
    woody->sprite_offset_y = 0;

    woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);

    // Анимации дверей
    woody->enter_door = NULL;
    woody->dest_door = NULL;

    woody->enter_door_sprite_x = 0;
    woody->enter_door_sprite_y = 0;

    woody->dest_door_current_spritelist = 0;

    woody->dest_door_current_animation_pack = -1;
    woody->dest_door_current_animation_index = -1;

    woody->dest_door_animation_frame = 0;
    woody->dest_door_animation_frame_time = 0;
    woody->dest_door_animation_length = 0;

    woody->dest_door_sprite_x = 0;
    woody->dest_door_sprite_y = 0;
    woody->dest_door_sprite_src_x = 0;
    woody->dest_door_sprite_src_y = 0;
    woody->dest_door_sprite_w = 0;
    woody->dest_door_sprite_h = 0;
    woody->dest_door_sprite_offset_x = 0;
    woody->dest_door_sprite_offset_y = 0;

    woody->dest_door_exit_x = 0;

    // Основа Вуди
    woody->x = start_x;
    woody->y = start_y;

    woody->speed_x = 2;
    woody->speed_y = 1;

    woody->velocity_x = 0;
    woody->velocity_y = 0;

    woody->can_move = false;
    woody->in_door = false;
    woody->is_on_floor = true;
    woody->floor_y = start_y;
    woody->hide = false;
    woody->caught = false;

    woody->state = STATE_STOP;

    woody->auto_move_goal_type = GOAL_NONE;
    woody->auto_move_goal_x = 0;
    woody->auto_move_goal_y = 0;

    woody->h_doors = h_doors;
    woody->v_doors = v_doors;

    woody->neighbour = neighbour;

    woody->look_objects = look_objects;
    
    // strcpy(woody->look_object_phrase_text, "");
    woody->look_object_phrase_y = 0;
    woody->look_object_bubble_size = 0;
    woody->look_object_phrase_show = false;

    woody->hideouts = hideouts;

    woody->storages = storages;

    woody->room = room;
    woody->room_collisions = room_collisions;

    woody->level_end = level_end;
    woody->level_end_active = level_end_active;

    // Прохождение уровня
    woody->tv_rating = 0;
    woody->min_quota = min_quota;

    woody->tricks = 0;
    woody->total_tricks = total_tricks;

    // Инвентарь
    for (int i = 0; i < MAX_ITEMS_IN_INVENTORY; i++) {
        woody->inventory[i] = ITEM_NONE;
    }

    woody->selected_item = 0;
    woody->item_count = 0;
    woody->inventory_using = false;

    // Пакости
    woody->trick_making_progress = 0;
    woody->trick_making_length = 0;

    // Интерфейс
    for (int i = 0; i < MAX_HINTS; i++) {
        woody->hints[i].show = false;
        woody->hints[i].is_active_goal = false;
    }

    woody->ui_tv_rating = 0.0f;
    sprintf(woody->ui_tv_rating_text, "%d", (int) woody->ui_tv_rating);
    woody->ui_tv_rating_text_color = COLOR_GREEN_PERCENT;
    woody->ui_tv_rating_animation_play = false;
    woody->ui_tv_rating_animation_frame = 0;

    woody->ui_tv_rating_delta = 0.0f;
    strcpy(woody->ui_tv_rating_delta_text, "+0");
    woody->ui_tv_rating_delta_text_color = CG_YELLOW_TRICKS;
    woody->ui_tv_rating_delta_text_show = false;

    sprintf(woody->ui_tricks_counter_text, "%d/%d", woody->tricks, woody->total_tricks);
    intraFontSetStyle(Font_BLUEHIGC_24, 0.583, CG_YELLOW_TRICKS, 0, 0, INTRAFONT_ALIGN_LEFT);
    woody->ui_tricks_counter_text_x = 460 - floor(intraFontMeasureText(Font_BLUEHIGC_24, woody->ui_tricks_counter_text) * 0.5);
    woody->ui_tricks_counter_text_show = true;

    woody->inventory_animation_items_show = true;
    woody->inventory_animation_play = false;
    woody->inventory_animation_first_index = 0;
    woody->inventory_animation_last_index = 0;
    woody->inventory_animation_frame = 0;
    woody->inventory_animation_frame_time = 0;

    woody->ui_tricks_counter_animation_play = false;
    woody->ui_tricks_counter_animation_frame = 0;
    woody->ui_tricks_counter_animation_frame_time = 0;

    // Камера
    woody->camera_limit_x = camera_limit_x;
    woody->camera_limit_y = camera_limit_y;

    woody->camera_offset_x = 0;
    woody->camera_offset_y = 0;

    woody->new_camera_offset_x = 0;
    woody->new_camera_offset_y = 0;

    woody->camera_door_offset_x = 0;
    woody->camera_door_offset_y = 0;

    return woody;
}

void woody_animation_update_frame(Woody* woody) {
    const Animation* current_animation = &woody->animations[woody->current_animation_pack][woody->current_animation_index];
    int animation_frame = current_animation->frames[woody->animation_frame];
    const Frame* frame = &woody->gfxdata[woody->current_animation_pack][animation_frame];

    woody->sprite_src_x = frame->src_x;
    woody->sprite_src_y = frame->src_y;
    woody->sprite_w = frame->w;
    woody->sprite_h = frame->h;
    woody->sprite_offset_x = frame->offset_x;
    woody->sprite_offset_y = frame->offset_y;
    woody->current_spritelist = frame->spritelist_index;

    int current_sound = current_animation->sfx[woody->animation_frame];

    if (current_sound != SOUND_NONE)
        NFHSoundPlay(current_sound);

}

void woody_animation_set(Woody* woody, int pack, int animation) {
    if (woody->current_animation_pack == pack && woody->current_animation_index == animation) {
        return;
    }

    woody->current_animation_pack = pack;
    woody->current_animation_index = animation;
    woody->animation_frame = 0;
    woody->animation_frame_time = 0;

    const Animation* new_animation = &woody->animations[pack][animation];
    woody->animation_loop = new_animation->loop;
    woody->animation_length = new_animation->frame_count;

    woody_animation_update_frame(woody);
}

void woody_dest_door_animation_update_frame(Woody* woody) {
    const Animation* current_animation = &woody->animations[woody->dest_door_current_animation_pack][woody->dest_door_current_animation_index];
    int animation_frame = current_animation->frames[woody->dest_door_animation_frame];
    const Frame* frame = &woody->gfxdata[woody->dest_door_current_animation_pack][animation_frame];

    woody->dest_door_sprite_src_x = frame->src_x;
    woody->dest_door_sprite_src_y = frame->src_y;
    woody->dest_door_sprite_w = frame->w;
    woody->dest_door_sprite_h = frame->h;
    woody->dest_door_sprite_offset_x = frame->offset_x;
    woody->dest_door_sprite_offset_y = frame->offset_y;
    woody->dest_door_current_spritelist = frame->spritelist_index;

    int current_sound = current_animation->sfx[woody->dest_door_animation_frame];

    if (current_sound != SOUND_NONE) {
        NFHSoundPlay(current_sound);
    }
}

void woody_dest_door_animation_set(Woody* woody, int pack, int animation) {
    woody->dest_door_current_animation_pack = pack;
    woody->dest_door_current_animation_index = animation;
    woody->dest_door_animation_length = woody->animations[pack][animation].frame_count;
    woody->dest_door_animation_frame = 0;
    woody->dest_door_animation_frame_time = 0;

    woody_dest_door_animation_update_frame(woody);
}

void woody_start_using_h_door(Woody* woody, hDoor* new_enter_door) {
    hDoor* current_door = new_enter_door;
    hDoor* new_dest_door = woody->h_doors[current_door->dest_door_room][current_door->dest_door_id];

    woody->enter_door = new_enter_door;
    woody->dest_door = new_dest_door;

    woody->room = current_door->dest_door_room;
    woody->in_door = true;

    current_door->using_by = USING_WOODY;
    woody->enter_door_sprite_x = current_door->sprite_x;
    woody->enter_door_sprite_y = current_door->sprite_y;

    new_dest_door->using_by = USING_WOODY;
    woody->dest_door_exit_x = new_dest_door->exit_x;
    woody->dest_door_sprite_x = new_dest_door->sprite_x;
    woody->dest_door_sprite_y = new_dest_door->sprite_y;

    if (current_door->dest_door_id == 0) {
        woody_dest_door_animation_set(woody, ANIMATION_PACK_WOODY_DOORLEFT, ANIMATION_WOODY_DOORLEFT_LEAVE);
    } else {
        woody_dest_door_animation_set(woody, ANIMATION_PACK_WOODY_DOORRIGHT, ANIMATION_WOODY_DOORRIGHT_LEAVE);
    }

    woody->state = STATE_IN_H_DOOR;
}

void woody_start_using_v_door(Woody* woody, vDoor* new_enter_door) {
    vDoor* current_door = new_enter_door;
    vDoor* new_dest_door = woody->v_doors[current_door->dest_door_room][current_door->dest_door_id];

    woody->enter_door = new_enter_door;
    woody->dest_door = new_dest_door;

    woody->room = current_door->dest_door_room;
    woody->in_door = true;

    current_door->using_by = USING_WOODY;
    woody->enter_door_sprite_x = current_door->sprite_x;
    woody->enter_door_sprite_y = current_door->sprite_y;

    new_dest_door->using_by = USING_WOODY;
    woody->dest_door_sprite_x = new_dest_door->sprite_x;
    woody->dest_door_sprite_y = new_dest_door->sprite_y;

    woody_animation_set(woody, ANIMATION_PACK_WOODY_DOORBACK, ANIMATION_WOODY_DOORBACK_ENTER);
    woody_dest_door_animation_set(woody, ANIMATION_PACK_WOODY_DOORBACK, ANIMATION_WOODY_DOORBACK_LEAVE);

    woody->state = STATE_IN_V_DOOR;
}

static void woody_auto_move_check(Woody* woody) {
    // Авто-движение к LookObject
    if (controls_pressed(PSP_CTRL_SQUARE) && ((woody->state != STATE_LOOK_OBJECT || woody->inventory_using) && woody->state != STATE_MAKING_TRICK)) {
        LookObject** look_object_in_room = woody->look_objects[woody->room];
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            LookObject* current_look_object = look_object_in_room[i];
            if (current_look_object) {
                if (((abs(current_look_object->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) && woody->y == woody->floor_y) || (current_look_object->collision_x == woody->x)) {
                    woody->state = STATE_AUTO_H_MOVE;
                    woody->auto_move_goal_type = GOAL_LOOK_OBJECT;
                    woody->auto_move_goal_x = current_look_object->collision_x;
                    woody->auto_move_goal_y = current_look_object->collision_y;
                    if (current_look_object->tricked) {
                        woody->look_object_phrase_text = current_look_object->tricked_phrase_text;
                        woody->look_object_phrase_y = current_look_object->tricked_phrase_y;
                        woody->look_object_bubble_size = current_look_object->tricked_bubble_size;
                    } else {
                        woody->look_object_phrase_text = current_look_object->phrase_text;
                        woody->look_object_phrase_y = current_look_object->phrase_y;
                        woody->look_object_bubble_size = current_look_object->bubble_size;
                    }
                    woody->look_object_phrase_show = false;

                    break;
                }
            } else {
                break;
            }
        }
    }

    // Авто-движение к Hideout
    if (controls_pressed(PSP_CTRL_CIRCLE)) {
        Hideout* current_hideout = woody->hideouts[woody->room];
        if (current_hideout) {
            if (((abs(current_hideout->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) && woody->y == woody->floor_y) || (current_hideout->collision_x == woody->x)) {
                woody->state = STATE_AUTO_H_MOVE;
                woody->auto_move_goal_type = GOAL_HIDEOUT;
                woody->auto_move_goal_x = current_hideout->collision_x;
                woody->auto_move_goal_y = current_hideout->collision_y;
                woody->look_object_phrase_show = false;
            }
        }
    }

    // Авто-движение к Storages
    if (controls_pressed(PSP_CTRL_CROSS)) {
        Storage** storages_in_room = woody->storages[woody->room];
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            Storage* current_storage = storages_in_room[i];
            if (current_storage) {
                if (((abs(current_storage->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) && woody->y == woody->floor_y) || (current_storage->collision_x == woody->x)) {
                    woody->state = STATE_AUTO_H_MOVE;
                    woody->auto_move_goal_type = GOAL_STORAGE;
                    woody->auto_move_goal_x = current_storage->collision_x;
                    woody->auto_move_goal_y = current_storage->collision_y;
                    woody->look_object_phrase_show = false;

                    break;
                }
            } else {
                break;
            }
        }
    }
}

static void woody_auto_move_complete(Woody* woody) {
    switch (woody->auto_move_goal_type) {
        case GOAL_LOOK_OBJECT:
            if (!woody->inventory_using) {
                woody->state = STATE_LOOK_OBJECT;
                woody->look_object_phrase_show = true;

                woody->auto_move_goal_type = GOAL_NONE;

                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);
            } else {
                // Ищем текущий LookObject
                for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
                    LookObject* current_look_object = woody->look_objects[woody->room][i];
                    if (current_look_object) {
                        if (current_look_object->collision_x == woody->x) {
                            if (current_look_object->item_to_trick == woody->inventory[woody->selected_item]) {
                                woody->state = STATE_MAKING_TRICK;
                                woody->trick_making_length = current_look_object->trick_making_length;
                                woody->trick_making_progress = 0;

                                if (current_look_object->on_making_trick) {
                                    current_look_object->on_making_trick();
                                }

                                if (woody->room == ROOM_KIT && i == 0) {
                                    woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_USE_MID_MICROWAVE);
                                } else {
                                    woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_USE_MID);
                                }
                                //
                            } else {
                                woody->state = STATE_NONO;
                                woody->inventory_using = false;

                                woody->auto_move_goal_type = GOAL_NONE;

                                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_DECLINE);

                            }

                            break;
                        }
                    }
                }
            }

            break;
        case GOAL_HIDEOUT:
            // Пока что нет пакостей, связанных с укрытиями
            if (!woody->inventory_using) {
                woody->state = STATE_HIDEOUT_ENTER;
                woody->hide = true;

                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_WARDROBE_ENTER);
            } else {
                woody->state = STATE_NONO;
                woody->inventory_using = false;

                woody->auto_move_goal_type = GOAL_NONE;

                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_DECLINE);
            }

            woody->auto_move_goal_type = GOAL_NONE;

            break;
        case GOAL_FLOOR:
            woody->state = STATE_H_MOVE;

            woody->auto_move_goal_type = GOAL_NONE;

            break;
        case GOAL_STORAGE:
            // Пока что нет пакостей, связанных с храналищами
            if (!woody->inventory_using) {
                woody->state = STATE_STORAGE_CHECK;

                Storage** storages_in_room = woody->storages[woody->room];
                for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
                    Storage* current_storage = storages_in_room[i];
                    if (current_storage) {
                        if (woody->x == current_storage->collision_x) {
                            switch (current_storage->sprite_type) {
                                case SPRITE_TYPE_OPENS:
                                    current_storage->sprite_show = true;

                                    break;
                                case SPRITE_TYPE_DISAPPEARS:

                                    break;
                            }
                        }
                    } else {
                        break;
                    }
                }
                woody->auto_move_goal_type = GOAL_NONE;

                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_OPEN);
            } else {
                woody->state = STATE_NONO;
                woody->inventory_using = false;

                woody->auto_move_goal_type = GOAL_NONE;

                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_DECLINE);
            }

            break;
        case GOAL_NONE:

            break;
    }
}

static void woody_hints_update(Woody* woody) {
    // Отключаем существующие подсказки
    for (int i = 0; i < MAX_HINTS; i++) {
        Hint* current_hint = &woody->hints[i];
        current_hint->show = false;
        current_hint->is_active_goal = false;
    }

    // Смотрим LookObjects
    for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
        LookObject* current_look_object = woody->look_objects[woody->room][i];
        if (current_look_object) {
            if (abs(current_look_object->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) {
                woody->hints[2] = woody->hints[1];
                woody->hints[1] = woody->hints[0];

                Hint* current_hint = &woody->hints[0];
                if (woody->inventory_using) {
                    if (current_look_object->tricked) {
                        sprintf(current_hint->text, "Использовать %s на %s", item_use_names[woody->inventory[woody->selected_item]], current_look_object->tricked_use_hint_text);
                    } else {
                        sprintf(current_hint->text, "Использовать %s на %s", item_use_names[woody->inventory[woody->selected_item]], current_look_object->use_hint_text);
                    }
                } else {
                    if (current_look_object->tricked) {
                        strcpy(current_hint->text, current_look_object->tricked_hint_text);
                    } else {
                        strcpy(current_hint->text, current_look_object->hint_text);
                    }
                }
                current_hint->button_type = SQUARE;
                current_hint->show = true;
                current_hint->is_active_goal = false;

                if (woody->auto_move_goal_type == GOAL_LOOK_OBJECT && woody->auto_move_goal_x == current_look_object->collision_x) {
                    current_hint->is_active_goal = true;
                }
            }
        } else {
            break;
        }
    }

    // Смотрим Hideouts
    Hideout* current_hideout = woody->hideouts[woody->room];
    if (current_hideout) {
        if (abs(current_hideout->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) {
            woody->hints[2] = woody->hints[1];
            woody->hints[1] = woody->hints[0];

            Hint* current_hint = &woody->hints[0];
            if (woody->inventory_using) {
                sprintf(current_hint->text, "Использовать %s на %s", item_use_names[woody->inventory[woody->selected_item]], current_hideout->use_hint_text);
            } else {
                strcpy(current_hint->text, current_hideout->hint_enter_text);
            }
            current_hint->button_type = CIRCLE;
            current_hint->show = true;
            current_hint->is_active_goal = false;

            if ((woody->auto_move_goal_type == GOAL_HIDEOUT || woody->state == STATE_HIDEOUT_ENTER) && woody->auto_move_goal_x == current_hideout->collision_x) {
                current_hint->is_active_goal = true;
            } else {
                if (woody->state == STATE_HIDEOUT) {
                    strcpy(current_hint->text, current_hideout->hint_exit_text);
                    current_hint->is_active_goal = false;
                } else if (woody->state == STATE_HIDEOUT_EXIT) {
                    strcpy(current_hint->text, current_hideout->hint_exit_text);
                    current_hint->is_active_goal = true;
                }
            }
        }
    }

    // Смотрим Storages
    for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
        Storage* current_storage = woody->storages[woody->room][i];
        if (current_storage) {
            if (abs(current_storage->collision_x - woody->x) < WOODY_INTERACT_DISTANCE) {
                woody->hints[2] = woody->hints[1];
                woody->hints[1] = woody->hints[0];

                Hint* current_hint = &woody->hints[0];
                if (woody->inventory_using) {
                    sprintf(current_hint->text, "Использовать %s на %s", item_use_names[woody->inventory[woody->selected_item]], current_storage->use_hint_text);
                } else {
                    strcpy(current_hint->text, current_storage->hint_text);
                }
                current_hint->button_type = CROSS;
                current_hint->show = true;
                current_hint->is_active_goal = false;

                if (woody->auto_move_goal_x == current_storage->collision_x && (woody->auto_move_goal_type == GOAL_STORAGE || woody->state == STATE_STORAGE_CHECK || woody->state == STATE_STORAGE_FOUND || woody->state == STATE_STORAGE_NOT_FOUND)) {
                    current_hint->is_active_goal = true;
                }
            }
        } else {
            break;
        }
    }

    // Сдвиг active_goal в начало подсказок
    for (int i = 0; i < MAX_HINTS; i++) {
        Hint* current_hint = &woody->hints[i];
        if (current_hint->is_active_goal) {
            switch (i) {
                case 0: {

                    break;
                }
                
                case 1: {
                    Hint active_goal = *current_hint;

                    woody->hints[1] = woody->hints[0];
                    woody->hints[0] = active_goal;

                    break;
                }

                case 2: {
                    Hint active_goal = *current_hint;

                    woody->hints[2] = woody->hints[1];
                    woody->hints[1] = woody->hints[0];
                    woody->hints[0] = active_goal;

                    break;
                }
            }
        }
    }
    
}

// Update

// Хелперы
static bool woody_can_toggle_inventory(const Woody* woody) {
    switch (woody->state) {
        case STATE_AUTO_H_MOVE:
        case STATE_AUTO_V_MOVE:
        case STATE_STORAGE_CHECK:
        case STATE_MAKING_TRICK:
        case STATE_HIDEOUT_ENTER:
        case STATE_HIDEOUT:
        case STATE_HIDEOUT_EXIT:
        case STATE_STORAGE_FOUND:
        case STATE_CAUGHT_GO_TO_FLOOR:
        case STATE_CAUGHT_START:
        case STATE_CAUGHT_LOOP:
        case STATE_LEVEL_START:
        case STATE_LEVEL_ENDING:
        case STATE_STOP:
            return false;
        default:
            return true;
    }
}

static bool woody_input_wants_cancel_held(void) {
    return controls_held(PSP_CTRL_LEFT) || controls_held(PSP_CTRL_DOWN) || controls_held(PSP_CTRL_RIGHT);
}

static bool woody_input_wants_cancel_pressed(void) {
    return controls_pressed(PSP_CTRL_LEFT) || controls_pressed(PSP_CTRL_DOWN) || controls_pressed(PSP_CTRL_RIGHT);
}

static void woody_cancel_to_floor(Woody* woody) {
    woody->state = STATE_AUTO_V_MOVE;
    woody->auto_move_goal_type = GOAL_FLOOR;
    woody->auto_move_goal_y = woody->floor_y;
}

static void woody_stop_making_trick(Woody* woody) {
    if (woody->state == STATE_MAKING_TRICK) {
        switch (woody->auto_move_goal_type) {
            case GOAL_LOOK_OBJECT:
                // Ищем текущий LookObject
                for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
                    LookObject* current_look_object = woody->look_objects[woody->room][i];
                    if (current_look_object) {
                        if (current_look_object->collision_x == woody->x) {

                            if (current_look_object->on_stop_making_trick) {
                                current_look_object->on_stop_making_trick();
                            }

                            break;
                        }
                    }
                }

                break;
            case GOAL_HIDEOUT:
                // Пока что нет пакостей, связанных с укрытиями

                break;
            case GOAL_FLOOR:

                break;
            case GOAL_STORAGE:
                // Пока что нет пакостей, связанных с храналищами

                break;
            case GOAL_NONE:

                break;
        }
    }
}

static void woody_update_h_move(Woody* woody) {
    woody->velocity_x = 0;

    if (controls_held(PSP_CTRL_LEFT)) {
        woody->velocity_x = -woody->speed_x;
        if (woody->x + woody->velocity_x < woody->room_collisions[woody->room].x1) {
            // Проверка на дверь
            hDoor* current_door = woody->h_doors[woody->room][0]; // Левая дверь всегда под индексом 0!

            if (current_door) {
                if (woody->x + woody->velocity_x <= current_door->collision_x && current_door->using_by == USING_NONE) {
                    woody_start_using_h_door(woody, current_door);
                    woody_animation_set(woody, ANIMATION_PACK_WOODY_DOORLEFT, ANIMATION_WOODY_DOORLEFT_ENTER);
                }   
            } else {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);
            }

            woody->velocity_x = 0;
        }
    } else if (controls_held(PSP_CTRL_RIGHT)) {
        woody->velocity_x = woody->speed_x;
        if (woody->x + woody->velocity_x > woody->room_collisions[woody->room].x2) {
        // Проверка на дверь
        hDoor* current_door = woody->h_doors[woody->room][1]; // Правая дверь всегда под индексом 1!

            if (current_door) {
                if (woody->x + woody->velocity_x >= current_door->collision_x && current_door->using_by == USING_NONE) {
                    woody_start_using_h_door(woody, current_door);
                    woody_animation_set(woody, ANIMATION_PACK_WOODY_DOORRIGHT, ANIMATION_WOODY_DOORRIGHT_ENTER);
                }
            } else {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS1);
            }

            woody->velocity_x = 0;
        }
    }

    // Ассист движения к вертикальным дверям
    if (controls_held(PSP_CTRL_UP)) {
        for (int door = 0; door < MAX_V_DOORS_IN_ROOM; door++) {
            if (woody->v_doors[woody->room][door]) {
                int current_collision_x = woody->v_doors[woody->room][door]->collision_x;
                int distance = abs(current_collision_x - woody->x);

                if (distance == 0) {
                    woody->is_on_floor = false;
                    woody->velocity_y = -woody->speed_y;
                    woody->state = STATE_V_MOVE;
                } else {
                    if (distance < WOODY_INTERACT_DISTANCE) {
                        if (woody->x < current_collision_x) {
                            woody->velocity_x = woody->speed_x;
                            if (woody->x + woody->velocity_x >= current_collision_x) {
                                woody->x = current_collision_x;
                                woody->velocity_x = 0;
                                woody->is_on_floor = false;
                                woody->velocity_y = -woody->speed_y;
                                woody->state = STATE_V_MOVE;
                            }
                        } else {
                            woody->velocity_x = -woody->speed_x;
                            if (woody->x + woody->velocity_x <= current_collision_x) {
                                woody->x = current_collision_x;
                                woody->velocity_x = 0;
                                woody->is_on_floor = false;
                                woody->velocity_y = -woody->speed_y;
                                woody->state = STATE_V_MOVE;
                            }
                        }
                    }
                }
            } 
        }
    }

    woody_auto_move_check(woody);

    if (woody->velocity_x == 0) {
        if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC && woody->current_animation_index == ANIMATION_WOODY_MG3) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);
        }
        else if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC && woody->current_animation_index == ANIMATION_WOODY_MG1) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS1);
        }
    } else {
        if (woody->velocity_x > 0) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG1);
        } else {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG3);
        }

        woody->x += woody->velocity_x;
    }
}

static void woody_update_v_move(Woody* woody) {
    // Вертикальное движение
    if (controls_held(PSP_CTRL_UP)) {
        woody->velocity_y = -woody->speed_y;
    } else if (controls_held(PSP_CTRL_DOWN)) {
        woody->velocity_y = woody->speed_y;
    }

    if (woody->velocity_y > 0) {
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG2);
    } else {
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG0);
    }

    woody->y += woody->velocity_y;

    // Вход в вертикальную дверь
    for (int door = 0; door < MAX_V_DOORS_IN_ROOM; door++) {
        vDoor* current_door = woody->v_doors[woody->room][door];
        if (current_door) {
            if (current_door->using_by == USING_NONE && woody->x == current_door->collision_x && woody->y <= current_door->collision_y) {
                woody_start_using_v_door(woody, current_door);
                break;
            }
        }
    }

    if (woody->y >= woody->floor_y) {
        woody->y = woody->floor_y;
        woody->is_on_floor = true;
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);
        woody->state = STATE_H_MOVE;
    }
}

static void woody_update_auto_h_move(Woody* woody) {
    woody->velocity_x = 0;

    if (woody->auto_move_goal_x > woody->x) {
        woody->velocity_x = woody->speed_x;
    } else if (woody->auto_move_goal_x < woody->x) {
        woody->velocity_x = -woody->speed_x;
    } else {
        if (woody->y == woody->auto_move_goal_y) {
            woody_auto_move_complete(woody);
        } else {
            woody->state = STATE_AUTO_V_MOVE;
        }
    }

    if (woody->velocity_x == 0) {
        if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC && woody->current_animation_index == ANIMATION_WOODY_MG3) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);
        }
        else if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC && woody->current_animation_index == ANIMATION_WOODY_MG1) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS1);
        } 
    } else {
        if (woody_input_wants_cancel_pressed()) {
            woody->state = STATE_H_MOVE;
            woody->auto_move_goal_type = GOAL_NONE;

            NFHSoundPlay(SOUND_ILLEGAL);
        }

        if (woody->velocity_x > 0) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG1);
        } else {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG3);
        }

        woody->x += woody->velocity_x;

        if (woody->velocity_x > 0) {
            if (woody->auto_move_goal_x < woody->x) {
                woody->x = woody->auto_move_goal_x;
            }
        } else {
            if (woody->auto_move_goal_x > woody->x) {
                woody->x = woody->auto_move_goal_x;
            }
        }
    }
}

static void woody_update_auto_v_move(Woody* woody) {
    woody->velocity_y = 0;

    if (woody->auto_move_goal_y > woody->y) {
        woody->velocity_y = woody->speed_y;

        woody_auto_move_check(woody);

    } else if (woody->auto_move_goal_y < woody->y) {
        woody->velocity_y = -woody->speed_y;

        if (woody_input_wants_cancel_pressed()) {
            woody_cancel_to_floor(woody);

            NFHSoundPlay(SOUND_ILLEGAL);
        }
    } else {
        woody_auto_move_complete(woody);
    }

    if (woody->velocity_y != 0) {
        if (woody->velocity_y > 0) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG2);
        } else {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG0);
        }

        woody->y += woody->velocity_y;
    }

    if (woody->y >= woody->floor_y) {
        woody->y = woody->floor_y;
        woody->is_on_floor = true;
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);
        woody->state = STATE_H_MOVE;
    }
}

static void woody_update_look_object(Woody* woody) {
    if (woody_input_wants_cancel_pressed()) {
        woody_cancel_to_floor(woody);

        woody->look_object_phrase_show = false;
    }

    woody_auto_move_check(woody);
}

static void woody_update_hideout_enter(Woody* woody) {
    // Ждём когда кончится анимация, а потом пихаем Вуди в состояние STATE_HIDEOUT
    if (IS_LAST_ANIMATION_FRAME) {
        woody->state = STATE_HIDEOUT;
    }
}

static void woody_update_hideout(Woody* woody) {
    if (controls_pressed(PSP_CTRL_CIRCLE)) {
        woody->state = STATE_HIDEOUT_EXIT;
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_WARDROBE_LEAVE);
    }
}

static void woody_update_hideout_exit(Woody* woody) {
    // Ждём когда кончится анимация, а потом заставляем Вуди идти на пол
    if (IS_LAST_ANIMATION_FRAME) {
        woody_cancel_to_floor(woody);

        woody->hide = false;
    }
}

static void woody_update_storage_check(Woody* woody) {
    if (IS_LAST_ANIMATION_FRAME) {
        Storage* current_storage = NULL;

        Storage** storages_in_room = woody->storages[woody->room];
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            current_storage = storages_in_room[i];
            if (current_storage) {
                if (woody->x == current_storage->collision_x) {
                    if (!current_storage->opened) {
                        current_storage->opened = true;
                        woody->state = STATE_STORAGE_FOUND;

                        switch (current_storage->side) {
                            case SIDE_LEFT_HIGH:
                                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_TAKE_HIGH3);

                                break;
                            case SIDE_LEFT_MID:
                                break;
                            case SIDE_LEFT_LOW:
                                break;
                            case SIDE_UP_HIGH:
                                break;
                            case SIDE_UP_MID:
                                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_TAKE0);

                                break;
                            case SIDE_UP_LOW:
                                break;
                            case SIDE_RIGHT_HIGH:
                                break;
                            case SIDE_RIGHT_MID:
                                break;
                            case SIDE_RIGHT_LOW:
                                break;
                        }
                    } else {
                        woody->state = STATE_STORAGE_NOT_FOUND;
                        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC2, ANIMATION_WOODY_LOSE);
                    }

                    break;
                }
            } else {
                break;
            }
        }
    }
}

static void woody_update_storage_found(Woody* woody) {
    if (IS_LAST_ANIMATION_FRAME) {
        Storage* current_storage = NULL;

        Storage** storages_in_room = woody->storages[woody->room];
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            current_storage = storages_in_room[i];
            if (current_storage) {
                if (woody->x == current_storage->collision_x) {
                    int first_item_index = woody->item_count;
                    woody->inventory_animation_first_index = first_item_index;
                    woody->inventory_animation_last_index = first_item_index;
                    for (int i = 0; i < MAX_ITEMS_IN_STORAGE; i++) {
                        Item current_item = current_storage->items[i];
                        if (current_item != ITEM_NONE) {
                            woody->inventory[first_item_index + i] = current_item;
                            woody->item_count++;
                            woody->inventory_animation_last_index++;
                        } else {
                            break;
                        }
                    }

                    woody->inventory_animation_items_show = true;
                    woody->inventory_animation_play = true;
                    woody->inventory_animation_frame = 0;
                    woody->inventory_animation_frame_time = 0;

                    woody->state = STATE_STORAGE_END;

                    switch (current_storage->side) {
                        case SIDE_LEFT_HIGH:
                        case SIDE_LEFT_MID:
                        case SIDE_LEFT_LOW:
                            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);

                            break;
                        case SIDE_UP_HIGH:
                        case SIDE_UP_MID:
                        case SIDE_UP_LOW:
                            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS0);

                            break;
                        case SIDE_RIGHT_HIGH:
                        case SIDE_RIGHT_MID:
                        case SIDE_RIGHT_LOW:
                            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS1);

                            break;
                    }

                    current_storage->sprite_show = false;

                }

                break;
            } else {
                break;
            }
        }
    }
}

static void woody_update_storage_not_found(Woody* woody) {
    if (IS_LAST_ANIMATION_FRAME) {
        woody->state = STATE_STORAGE_END;

        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);

        Storage* current_storage = NULL;

        Storage** storages_in_room = woody->storages[woody->room];
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            current_storage = storages_in_room[i];
            if (current_storage) {
                if (woody->x == current_storage->collision_x) {
                    current_storage->sprite_show = false;
                }

                break;
            } else {
                break;
            }
        }
    }
}

static void woody_update_storage_end(Woody* woody) {
    if (woody_input_wants_cancel_held()) {
        woody_cancel_to_floor(woody);
    }

    woody_auto_move_check(woody);
}

static void woody_update_in_h_door(Woody* woody) {
    // Camera Door Offset
    woody->camera_door_offset_x = lerp(woody->camera_door_offset_x, woody->dest_door_exit_x - woody->x, 0.95f);
    woody->camera_door_offset_y = lerp(woody->camera_door_offset_y, woody->room_collisions[woody->room].floor - woody->y, 0.95f);

    // Анимация Dest Door
    woody->dest_door_animation_frame_time++;
    if (woody->dest_door_animation_frame_time == 5) {
        woody->dest_door_animation_frame_time = 0;
        woody->dest_door_animation_frame++;
        if (woody->dest_door_animation_frame == woody->dest_door_animation_length) {
            woody->in_door = false;
            woody->x = woody->dest_door_exit_x;
            woody->y = woody->room_collisions[woody->room].floor;
            woody->floor_y = woody->y;
            woody->camera_door_offset_x = 0;
            woody->camera_door_offset_y = 0;

            woody->state = STATE_H_MOVE;

            if (woody->dest_door_current_animation_pack == ANIMATION_PACK_WOODY_DOORRIGHT && woody->dest_door_current_animation_index == ANIMATION_WOODY_DOORRIGHT_LEAVE) {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);
            } else {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS1);
            }

            hDoor* enter_door = (hDoor*) woody->enter_door;
            enter_door->using_by = USING_NONE;

            hDoor* dest_door = (hDoor*) woody->dest_door;
            dest_door->using_by = USING_NONE;

            woody_check_caught(woody, woody->neighbour);

            if (woody->room == ROOM_FRO) {
                // Уровень пройден
                woody->can_move = false;
                woody->state = STATE_LEVEL_ENDING;

                NFHHouseMusicStop();

                if (woody->tv_rating >= woody->min_quota || is_this_scene(&Tutorial1Scene)) {
                    NFHMusicPlay(MUSIC_JINGLE_SUCCESS_NORMAL, 0);
                } else {
                    strcpy(woody->level_end->end_text, "Провал");
                    NFHMusicPlay(MUSIC_JINGLE_FAILED, 0);
                }
            }
        } else {
            woody_dest_door_animation_update_frame(woody);
        }
    }
}

static void woody_update_in_v_door(Woody* woody) {
    // Camera Door Offset
    vDoor* dest_door = (vDoor*)woody->dest_door;

    if (woody->dest_door_animation_frame >= 10) {
        woody->camera_door_offset_x = lerp(woody->camera_door_offset_x, dest_door->collision_x - woody->x, 0.92f);
        woody->camera_door_offset_y = lerp(woody->camera_door_offset_y, woody->room_collisions[woody->room].floor - woody->floor_y, 0.92f);
    }

    // Анимация Dest Door
    woody->dest_door_animation_frame_time++;
    if (woody->dest_door_animation_frame_time == 5) {
        woody->dest_door_animation_frame_time = 0;
        woody->dest_door_animation_frame++;
        if (woody->dest_door_animation_frame == woody->dest_door_animation_length) {
            woody->in_door = false;
            woody->x = dest_door->collision_x;
            woody->y = dest_door->collision_y;
            woody->floor_y = woody->room_collisions[woody->room].floor;
            woody->camera_door_offset_x = 0;
            woody->camera_door_offset_y = 0;

            woody->state = STATE_V_MOVE;
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG2);

            woody->velocity_y = woody->speed_y;

            vDoor* enter_door = (vDoor*) woody->enter_door;
            enter_door->using_by = USING_NONE;

            dest_door->using_by = USING_NONE;

            woody_check_caught(woody, woody->neighbour);
        } else {
            woody_dest_door_animation_update_frame(woody);
        }
    }
}

static void woody_update_nono(Woody* woody) {
    if (woody_input_wants_cancel_pressed()) {
        woody_cancel_to_floor(woody);
    }

    if (IS_LAST_ANIMATION_FRAME) {
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);

        if (woody->y == woody->floor_y) {
            woody->state = STATE_H_MOVE;
        } else {
            woody->state = STATE_NONO_END;
        }
    }
}

static void woody_update_nono_end(Woody* woody) {
    if (woody_input_wants_cancel_held()) {
        woody_cancel_to_floor(woody);
    }

    woody_auto_move_check(woody);
}

static void woody_update_making_trick(Woody* woody) {
    woody->trick_making_progress++;

    if (woody->trick_making_progress == woody->trick_making_length) {
        woody->state = STATE_SMILE;

        // Удаляем предмет из инвентаря
        woody->inventory[woody->selected_item] = ITEM_NONE;
        if (woody->selected_item != 15) {
            woody->inventory[woody->selected_item] = woody->inventory[woody->selected_item+1];
        }
        for (int item = woody->selected_item+1; item < (MAX_ITEMS_IN_INVENTORY - 1); item++) {
            if (woody->inventory[item] == ITEM_NONE) break;
            woody->inventory[item] = woody->inventory[item+1];
        }
        woody->inventory[15] = ITEM_NONE;
        woody->item_count--;
                        
        if (woody->selected_item > woody->item_count - 1) {
            woody->selected_item = woody->item_count - 1;
        }

        if (woody->selected_item < 0) {
            woody->selected_item = 0;
        }

        switch (woody->auto_move_goal_type) {
            case GOAL_LOOK_OBJECT:
                // Ищем текущий LookObject
                for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
                    LookObject* current_look_object = woody->look_objects[woody->room][i];
                    if (current_look_object) {
                        if (current_look_object->collision_x == woody->x) {
                            current_look_object->tricked = true;

                            if (current_look_object->on_trick) {
                                current_look_object->on_trick();
                            }

                            break;
                        }
                    }
                }

                break;
            case GOAL_HIDEOUT:
                // Пока что нет пакостей, связанных с укрытиями

                break;
            case GOAL_FLOOR:

                break;
            case GOAL_STORAGE:
                // Пока что нет пакостей, связанных с храналищами

                break;
            case GOAL_NONE:

                break;
        }

        woody->inventory_using = false;
        woody->auto_move_goal_type = GOAL_NONE;

        int install_sound = (rand() % 2) ? SOUND_INSTALL1 : SOUND_INSTALL2;
        NFHSoundPlay(install_sound);
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_LAUGH);
    } else {
        if (woody_input_wants_cancel_held()) {
            woody_stop_making_trick(woody);
        
            woody_cancel_to_floor(woody);
        }

        woody_auto_move_check(woody);
    }
}

static void woody_update_smile(Woody* woody) {
    if (woody_input_wants_cancel_held()) {
        woody_cancel_to_floor(woody);
    }

    woody_auto_move_check(woody);
}

static void woody_update_caught_go_to_floor(Woody* woody) {
    woody->y += woody->speed_y;

    woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG2);

    if (woody->y >= woody->floor_y) {
        woody->y = woody->floor_y;
        woody->is_on_floor = true;
        if (woody->x > (woody->room_collisions[woody->room].x2 - woody->room_collisions[woody->room].x1) * 0.5 + woody->room_collisions[woody->room].x1) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR3);
        } else {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR1);
        }

        woody->state = STATE_CAUGHT_START;
    }
}

static void woody_update_caught_start(Woody* woody) {
    if (IS_LAST_ANIMATION_FRAME) {
        if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC3 && woody->current_animation_index == ANIMATION_WOODY_FEAR1) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR1_LOOP);
        } else if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC3 && woody->current_animation_index == ANIMATION_WOODY_FEAR3) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR3_LOOP);
        }

        woody->state = STATE_CAUGHT_LOOP;
    }
}

static void woody_update_caught_loop(Woody* woody) {

}

static void woody_update_level_start(Woody* woody) {
    // Типо как STATE_H_MOVE, но с автодвижением
    woody->velocity_x = -woody->speed_x;

    if (woody->x + woody->velocity_x < woody->room_collisions[woody->room].x1) {
        // Проверка на дверь
        hDoor* current_door = woody->h_doors[woody->room][0]; // Левая дверь всегда под индексом 0!

        if (current_door) {
            if (woody->x + woody->velocity_x <= current_door->collision_x && current_door->using_by == USING_NONE) {
                woody_start_using_h_door(woody, current_door);
                woody_animation_set(woody, ANIMATION_PACK_WOODY_DOORLEFT, ANIMATION_WOODY_DOORLEFT_ENTER);
            }
        }
        woody->velocity_x = 0;
    }

    if (woody->velocity_x == 0) {
        if (woody->current_animation_pack == ANIMATION_PACK_WOODY_GENERIC && woody->current_animation_index == ANIMATION_WOODY_MG3) {
            woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS3);
        }
    } else {
        woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MG3);

        woody->x += woody->velocity_x;
    }
}

static void woody_update_level_ending(Woody* woody) {
    if (woody->is_on_floor) {
        if (!*woody->level_end_active) {
            if (woody->tv_rating >= woody->min_quota || is_this_scene(&Tutorial1Scene)) {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_TRIUMPH);
            }
            *woody->level_end_active = true;
        }

        if (woody->level_end->counter == 0) {
            if (woody->tv_rating >= woody->min_quota) {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_MS2);
                NFHSoundPlay(SOUND_APPLAUSE);
            }
            woody->animation_loop = false;
            woody->state = STATE_STOP;            
        } else if (woody->level_end->counter == 1) {
            // Обновляем данные ЗА КАДР до того, как level_end появится
            strcpy(woody->level_end->tricks_text, woody->ui_tricks_counter_text);
            sprintf(woody->level_end->tv_rating_text, "%d", woody->tv_rating);
        }
    } else {
        // потом
    }
}

void woody_update(Woody* woody) {
    // Управление инвентарём
    if (woody->inventory_using) {
        if (controls_pressed(PSP_CTRL_LTRIGGER) && woody->selected_item > 0) {
            woody->selected_item--;
            NFHSoundPlay(SOUND_BUT_HOVER1);
        } else if (controls_pressed(PSP_CTRL_RTRIGGER) && woody->selected_item < woody->item_count - 1) {
            woody->selected_item++;
            NFHSoundPlay(SOUND_BUT_HOVER1);
        }
    }

    if (controls_pressed(PSP_CTRL_TRIANGLE) && woody->item_count != 0 && woody_can_toggle_inventory(woody)) {
        woody->inventory_using = !woody->inventory_using;
        if (woody->inventory_using) {
            NFHSoundPlay(SOUND_BUT_HOVER1);
        }
    }

    switch (woody->state) {
        case STATE_H_MOVE: woody_update_h_move(woody); break;
        case STATE_V_MOVE: woody_update_v_move(woody); break;
        case STATE_AUTO_H_MOVE: woody_update_auto_h_move(woody); break;
        case STATE_AUTO_V_MOVE: woody_update_auto_v_move(woody); break;
        case STATE_LOOK_OBJECT: woody_update_look_object(woody); break;
        case STATE_HIDEOUT_ENTER: woody_update_hideout_enter(woody); break;
        case STATE_HIDEOUT: woody_update_hideout(woody); break;
        case STATE_HIDEOUT_EXIT: woody_update_hideout_exit(woody); break;
        case STATE_STORAGE_CHECK: woody_update_storage_check(woody); break;
        case STATE_STORAGE_FOUND: woody_update_storage_found(woody); break;
        case STATE_STORAGE_NOT_FOUND: woody_update_storage_not_found(woody); break;
        case STATE_STORAGE_END: woody_update_storage_end(woody); break;
        case STATE_IN_H_DOOR: woody_update_in_h_door(woody); break;
        case STATE_IN_V_DOOR: woody_update_in_v_door(woody); break;
        case STATE_NONO: woody_update_nono(woody); break;
        case STATE_NONO_END: woody_update_nono_end(woody); break;
        case STATE_MAKING_TRICK: woody_update_making_trick(woody); break;
        case STATE_SMILE: woody_update_smile(woody); break;
        case STATE_CAUGHT_GO_TO_FLOOR: woody_update_caught_go_to_floor(woody); break;
        case STATE_CAUGHT_START: woody_update_caught_start(woody); break;
        case STATE_CAUGHT_LOOP: woody_update_caught_loop(woody); break;
        case STATE_LEVEL_START: woody_update_level_start(woody); break;
        case STATE_LEVEL_ENDING: woody_update_level_ending(woody); break;
        case STATE_STOP: break;
    }

    woody_hints_update(woody);

    // Анимации
    woody->animation_frame_time++;
    if (woody->animation_frame_time == 5) {
        woody->animation_frame_time = 0;
        woody->animation_frame++;
        if (woody->animation_frame == woody->animation_length) {
            if (woody->animation_loop) {
                woody->animation_frame = 0;
            } else {
                woody->animation_frame = woody->animation_length - 1;
            }
        }
        woody_animation_update_frame(woody);
    }

    // UI
    if (woody->inventory_animation_play) {
        woody->inventory_animation_frame_time++;
        if (woody->inventory_animation_frame_time == 10) {
            woody->inventory_animation_frame_time = 0;
            woody->inventory_animation_frame++;

            if (woody->inventory_animation_frame == 5) {
                woody->inventory_animation_frame = 0;
                woody->inventory_animation_play = false;
            } else {
                woody->inventory_animation_items_show = !woody->inventory_animation_items_show;
            }
        }
    }

    if (woody->ui_tricks_counter_animation_play) {
        woody->ui_tricks_counter_animation_frame_time++;
        if (woody->ui_tricks_counter_animation_frame_time == 10) {
            woody->ui_tricks_counter_animation_frame_time = 0;
            woody->ui_tricks_counter_animation_frame++;
            
            if (woody->ui_tricks_counter_animation_frame == 6) {
                woody->ui_tv_rating_animation_play = true;
                woody->ui_tv_rating_animation_frame = 0;
                woody->ui_tricks_counter_animation_play = false;
            } else {
                woody->ui_tricks_counter_text_show = !woody->ui_tricks_counter_text_show;
            }
        }
    }

    if (woody->ui_tv_rating_animation_play) {
        float add = woody->ui_tv_rating_delta / (60 - woody->ui_tv_rating_animation_frame);

        woody->ui_tv_rating += add;
        woody->ui_tv_rating_delta -= add;

        woody->ui_tv_rating_animation_frame++;

        if (woody->ui_tv_rating_animation_frame == 60) {
            woody->ui_tv_rating_animation_play = false;
            woody->ui_tv_rating = woody->tv_rating;
            woody->ui_tv_rating_delta = 0;
            woody->ui_tv_rating_animation_frame = 0;
            woody->ui_tv_rating_delta_text_show = false;
        }

        int ui_tv_rating_round = (int) (woody->ui_tv_rating);

        if (ui_tv_rating_round >= woody->min_quota)
            woody->ui_tv_rating_text_color = COLOR_WHITE_PERCENT;

        sprintf(woody->ui_tv_rating_text, "%d", ui_tv_rating_round);
        sprintf(woody->ui_tv_rating_delta_text, "+%d", woody->tv_rating - ui_tv_rating_round);
    }
    
    // Движение камерой на стик
    int stick_x = controls_AnalogX();
    int stick_y = controls_AnalogY();
        
    if ((abs(stick_x) > CAMERA_STICK_DEADZONE) || (abs(stick_y) > CAMERA_STICK_DEADZONE)) {
        woody->new_camera_offset_x = stick_x;
        woody->new_camera_offset_y = stick_y;
    } else {
        woody->new_camera_offset_x = 0;
        woody->new_camera_offset_y = 0;
    }

    if (abs(woody->camera_offset_x - woody->new_camera_offset_x) < 1) {
        woody->camera_offset_x = woody->new_camera_offset_x;
    } else {
        woody->camera_offset_x = lerp(woody->camera_offset_x, woody->new_camera_offset_x, 0.8f);
    }

    if (abs(woody->camera_offset_y - woody->new_camera_offset_y) < 1) {
        woody->camera_offset_y = woody->new_camera_offset_y;
    } else {
        woody->camera_offset_y = lerp(woody->camera_offset_y, woody->new_camera_offset_y, 0.8f);
    }

    // Clamp камеры по Вуди
    camera_x = clamp(woody->x - 186 + floor(woody->camera_door_offset_x + 0.5), 0, woody->camera_limit_x);
    camera_y = clamp(woody->floor_y - 110 + floor(woody->camera_door_offset_y + 0.5), 0, woody->camera_limit_y);
    
    // Добавление оффсета стиком
    camera_x = clamp(camera_x + floor(woody->camera_offset_x + 0.5), 0, woody->camera_limit_x);
    camera_y = clamp(camera_y + floor(woody->camera_offset_y + 0.5), 0, woody->camera_limit_y);
}

void woody_draw(const Woody* woody) {
    g2d_DrawImageExt(woody->spritelists[woody->current_spritelist], woody->x + woody->sprite_offset_x - 179 - camera_x, woody->y + woody->sprite_offset_y - 165 - camera_y, woody->sprite_w, woody->sprite_h, WHITE, woody->sprite_src_x, woody->sprite_src_y, woody->sprite_w, woody->sprite_h, 0, 255, G2D_UP_LEFT);

    // Мысли Вуди
    if (woody->look_object_phrase_show) {
        // Bubble Bottom
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, woody->x - camera_x - 14, woody->y - camera_y - 27, 120, 27, WHITE, 0, 398, 120, 27, 0, 235, G2D_UP_LEFT);

        // Bubble Center
        g2d_DrawRectFilled(woody->x - camera_x - 14, woody->y - camera_y - woody->look_object_bubble_size - 27, 1, woody->look_object_bubble_size, BLACK, 235);
        g2d_DrawRectFilled(woody->x - camera_x - 13, woody->y - camera_y - woody->look_object_bubble_size - 27, 1, woody->look_object_bubble_size, COLOR_GRAY_BUBBLE, 235);

        g2d_DrawRectFilled(woody->x - camera_x - 12, woody->y - camera_y - woody->look_object_bubble_size - 27, 116, woody->look_object_bubble_size, WHITE, 235);

        g2d_DrawRectFilled(woody->x - camera_x + 104, woody->y - camera_y - woody->look_object_bubble_size - 27, 1, woody->look_object_bubble_size, COLOR_GRAY_BUBBLE, 235);
        g2d_DrawRectFilled(woody->x - camera_x + 105, woody->y - camera_y - woody->look_object_bubble_size - 27, 1, woody->look_object_bubble_size, BLACK, 235);

        // Bubble Top
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, woody->x - camera_x - 14, woody->y - camera_y - woody->look_object_bubble_size - 39, 120, 12, WHITE, 275, 293, 120, 12, 0, 235, G2D_UP_LEFT);

        intraFontSetStyle(Font_ACMESA, 0.65f, BLACK, 0, 0, INTRAFONT_ALIGN_CENTER);
        intraFontActivate(Font_ACMESA, 1);
        intraFontPrint(Font_ACMESA, woody->x - camera_x + 46, woody->y - camera_y - 48 + woody->look_object_phrase_y + intraFontTextHeight(Font_ACMESA), woody->look_object_phrase_text);
        intraFontPrint(Font_ACMESA, woody->x - camera_x + 46, woody->y - camera_y - 48 + woody->look_object_phrase_y + intraFontTextHeight(Font_ACMESA), woody->look_object_phrase_text);
    }

    // Прогрессбар пакости
    if (woody->state == STATE_MAKING_TRICK) {
        // BG
        g2d_DrawImageExt(SpriteList_PROGRESSBAR, woody->x - camera_x + 33, woody->y - camera_y - 19, 49, 12, WHITE, 0, 0, 49, 12, 0, 235, G2D_UP_LEFT);

        // Прогресс
        int progressbar_width = (49.0f * (float) woody->trick_making_progress / (float) woody->trick_making_length);
        g2d_DrawImageExt(SpriteList_PROGRESSBAR, woody->x - camera_x + 33, woody->y - camera_y - 19, progressbar_width, 12, WHITE, 0, 12, progressbar_width, 12, 0, 235, G2D_UP_LEFT);

        int percentage = (100.0f * (float) woody->trick_making_progress / (float) woody->trick_making_length);

        char percentage_text[8];
        sprintf(percentage_text, "%d%%", percentage);

        // Outline
        intraFontSetStyle(Font_BLUEHIGB_18, 0.4f, BLACK, 0, 0, INTRAFONT_ALIGN_CENTER);
        intraFontActivate(Font_BLUEHIGB_18, 0);

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
                intraFontPrint(Font_BLUEHIGB_18, woody->x - camera_x + 57-i, woody->y - camera_y - 15-j + intraFontTextHeight(Font_BLUEHIGB_18), percentage_text);
        }

        // Main Text
        intraFontSetStyle(Font_BLUEHIGB_18, 0.4f, WHITE, 0, 0, INTRAFONT_ALIGN_CENTER);
        intraFontPrint(Font_BLUEHIGB_18, woody->x - camera_x + 57, woody->y - camera_y - 15 + intraFontTextHeight(Font_BLUEHIGB_18), percentage_text);
    }
}

void woody_door_draw(const Woody* woody) {
    // Enter Door
    g2d_DrawImageExt(woody->spritelists[woody->current_spritelist], woody->enter_door_sprite_x + woody->sprite_offset_x - camera_x, woody->enter_door_sprite_y + woody->sprite_offset_y - camera_y, woody->sprite_w, woody->sprite_h, WHITE, woody->sprite_src_x, woody->sprite_src_y, woody->sprite_w, woody->sprite_h, 0, 255, G2D_UP_LEFT);

    // Destination Door
    g2d_DrawImageExt(woody->spritelists[woody->dest_door_current_spritelist], woody->dest_door_sprite_x + woody->dest_door_sprite_offset_x - camera_x, woody->dest_door_sprite_y + woody->dest_door_sprite_offset_y - camera_y, woody->dest_door_sprite_w, woody->dest_door_sprite_h, WHITE, woody->dest_door_sprite_src_x, woody->dest_door_sprite_src_y, woody->dest_door_sprite_w, woody->dest_door_sprite_h, 0, 255, G2D_UP_LEFT);
}

void woody_draw_ui(const Woody* woody) {
    // Подсказки
    if (!woody->in_door) {
        int hint_text_x = 93;

        for (int i = 0; i < MAX_HINTS; i++) {
            const Hint* current_hint = &woody->hints[i];
            if (!current_hint->show) break;

            const g2dColor hint_text_color = current_hint->is_active_goal ? CG_YELLOW_TRICKS : WHITE;

            // Кнопка подсказки
            int src_x = 0;
            int src_y = 0;
            switch (current_hint->button_type) {
                case CROSS: src_x = 0; src_y = 0; break;
                case CIRCLE: src_x = 23; src_y = 0; break;
                case SQUARE: src_x = 0; src_y = 23; break;
            }

            g2d_DrawImageExt(SpriteList_BUTTONS, hint_text_x - 23, 215, 23, 23, WHITE, src_x, src_y, 23, 23, 0, 255, G2D_UP_LEFT);

            // Текст подсказки
            intraFontSetStyle(Font_BLUEHIGC_11, 1, hint_text_color, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_BLUEHIGC_11, 0);
            intraFontPrint(Font_BLUEHIGC_11, hint_text_x, 224 + intraFontTextHeight(Font_BLUEHIGC_11), current_hint->text);
            hint_text_x += floor(intraFontMeasureText(Font_BLUEHIGC_11, current_hint->text)) + 28;
        }
    }

    // Пакости
    if (woody->ui_tricks_counter_text_show) {
        intraFontSetStyle(Font_BLUEHIGC_24, 0.583f, CG_YELLOW_TRICKS, 0, 0, INTRAFONT_ALIGN_LEFT);
        intraFontActivate(Font_BLUEHIGC_24, 1);
        intraFontPrint(Font_BLUEHIGC_24, woody->ui_tricks_counter_text_x, 236 + intraFontTextHeight(Font_BLUEHIGC_24), woody->ui_tricks_counter_text);
    }

    // Инвентарь
    if (woody->item_count > 0) {
        const bool is_auto_moving = (woody->state == STATE_AUTO_H_MOVE || woody->state == STATE_AUTO_V_MOVE);
        const bool is_targeted_action = (woody->auto_move_goal_type == GOAL_LOOK_OBJECT || woody->auto_move_goal_type == GOAL_HIDEOUT || woody->auto_move_goal_type == GOAL_STORAGE);

        for (int i = 0; i < woody->item_count; i++) {
            const int item = woody->inventory[i];
            if (item == ITEM_NONE) break;

            // Анимация новых предметов
            if (!woody->inventory_animation_items_show && woody->inventory_animation_first_index <= i && i <= woody->inventory_animation_last_index) continue;

            // Определение состояния предмета
            int item_state = 1;
            if (woody->inventory_using && i == woody->selected_item) {
                item_state = (is_auto_moving && is_targeted_action) ? 2 : 0;
            }

            // Вычисление позиции в спрайт-листе
            const int base_src_x = (item * 3 + item_state) * 45;
            const int src_x = base_src_x % 495;
            const int sprite_index = base_src_x / 495;
            
            g2dImage* current_spritelist;
            int src_y;
            
            if (sprite_index >= 15) {
                current_spritelist = SpriteList_ITEMS_2;
                src_y = (sprite_index - 15) * 34;
            } else {
                current_spritelist = SpriteList_ITEMS_1;
                src_y = sprite_index * 34;
            }

            g2d_DrawImageExt(current_spritelist, 99 + i * 45, 235, 45, 34, WHITE, src_x, src_y, 45, 34, 0, 255, G2D_UP_LEFT);
        }
    }

    // Таймер
    intraFontSetStyle(Font_BLUEHIGC_24, 1, CG_BLUE_TIMER, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_BLUEHIGC_24, 0);
    intraFontPrint(Font_BLUEHIGC_24, 438, 209 + intraFontTextHeight(Font_BLUEHIGC_24), "--:--");

    // Проценты
    intraFontSetStyle(Font_BLUEHIGC_24, 1, woody->ui_tv_rating_text_color, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_BLUEHIGC_24, 0);
    intraFontPrint(Font_BLUEHIGC_24, 419, 249 + intraFontTextHeight(Font_BLUEHIGC_24), woody->ui_tv_rating_text);

    // Над-проценты
    if (woody->ui_tv_rating_delta_text_show && woody->ui_tricks_counter_text_show) {
        intraFontSetStyle(Font_BLUEHIGC_24, 0.7f, woody->ui_tv_rating_delta_text_color, 0, 0, INTRAFONT_ALIGN_RIGHT);
        intraFontActivate(Font_BLUEHIGC_24, 0);
        intraFontPrint(Font_BLUEHIGC_24, 418, 236 + intraFontTextHeight(Font_BLUEHIGC_24), woody->ui_tv_rating_delta_text);

        intraFontSetStyle(Font_BLUEHIGB_18, 0.8f, woody->ui_tv_rating_delta_text_color, 0, 0, INTRAFONT_ALIGN_LEFT);
        intraFontActivate(Font_BLUEHIGB_18, 0);
        intraFontPrint(Font_BLUEHIGB_18, 419, 238 + intraFontTextHeight(Font_BLUEHIGB_18), "%");
    }

    intraFontSetStyle(Font_BLUEHIGB_18, 1, woody->ui_tv_rating_text_color, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 0);
    intraFontPrint(Font_BLUEHIGB_18, 418, 253 + intraFontTextHeight(Font_BLUEHIGB_18), "%");
}

void woody_tricks_counter_update(Woody* woody, int trick_tv_rating) {
    woody->tricks++;
    woody->tv_rating += trick_tv_rating;

    sprintf(woody->ui_tricks_counter_text, "%d/%d", woody->tricks, woody->total_tricks);

    intraFontSetStyle(Font_BLUEHIGC_24, 0.583f, CG_YELLOW_TRICKS, 0, 0, INTRAFONT_ALIGN_LEFT);
    woody->ui_tricks_counter_text_x = 460 - floor(intraFontMeasureText(Font_BLUEHIGC_24, woody->ui_tricks_counter_text) * 0.5);
    woody->ui_tricks_counter_text_show = false;

    woody->ui_tricks_counter_animation_play = true;
    woody->ui_tricks_counter_animation_frame_time = 0;
    woody->ui_tricks_counter_animation_frame = 0;

    woody->ui_tv_rating_delta = (float) trick_tv_rating;
    sprintf(woody->ui_tv_rating_delta_text, "+%d", trick_tv_rating);
    woody->ui_tv_rating_delta_text_show = true;
    woody->ui_tv_rating_delta_text_color = CG_YELLOW_TRICKS;
}

void woody_check_caught(Woody* woody, Neighbour* neighbour) {
    if (woody->room == neighbour->room && !woody->in_door && !neighbour->in_door && !woody->hide) {
        neighbour->game_over_state = STATE_GAME_OVER_START;
        neighbour->woody_caught = true;

        woody->look_objects[ROOM_LIR][1]->sprite_show = true; // на всякий случай
        if (!woody->look_objects[ROOM_KIT][1]->alt_action) {
            woody->look_objects[ROOM_KIT][1]->sprite_show = true; // тоже на всякий случай
        }

        woody->caught = true;

        woody->look_object_phrase_show = false;

        if (woody->state == STATE_STORAGE_CHECK || woody->state == STATE_STORAGE_FOUND || woody->state == STATE_STORAGE_NOT_FOUND) {
            Storage* current_storage = NULL;

            Storage** storages_in_room = woody->storages[woody->room];
            for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
                current_storage = storages_in_room[i];
                if (current_storage) {
                    if (woody->x == current_storage->collision_x) {
                        current_storage->sprite_show = false;
                    }
                    break;
                } else {
                    break;
                }
            }
        }

        woody_stop_making_trick(woody);

        woody->auto_move_goal_type = GOAL_NONE;
        woody->inventory_using = false;

        RoomCollision* current_room = &woody->room_collisions[woody->room];

        if (woody->x > clamp(neighbour->origin_x + 181, current_room->x1 + 30, current_room->x2 - 30)) {
            neighbour->game_over_goal_x = woody->x - 206;
        } else {
            neighbour->game_over_goal_x = woody->x - 156;
        }

        if (woody->y == woody->floor_y) {
            woody->state = STATE_CAUGHT_START;

            if (woody->x > clamp(neighbour->origin_x + 181, current_room->x1 + 30, current_room->x2 - 30)) {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR3);
            } else {
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC3, ANIMATION_WOODY_FEAR1);
            }
        } else {
            woody->state = STATE_CAUGHT_GO_TO_FLOOR;
        }

        NFHHouseMusicStop();
        NFHSoundPlay(SOUND_JINGLE_CAUGHT);
    }
}

bool woody_check_sign_collision(const Woody* woody, const Sign* sign) {
    int woody_collision_x = woody->x + 54;
    return (woody->room == sign->room && sign->collision_x1 < woody_collision_x && woody_collision_x < sign->collision_x2);
}

void woody_unload(Woody* woody) {
    free(woody);
}
