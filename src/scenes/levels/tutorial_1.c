#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../engine/controls/controls.h"
#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/NFHSound/NFHSound.h"
#include "../../engine/json/json_helper.h"

#include "../../objects/scene_manager.h"

#include "../../types/room_types.h"
#include "../../types/gfx_types.h"
#include "../../types/door_types.h"

#include "../../types/look_object_types.h"
#include "../../types/hideout_types.h"
#include "../../types/storage_types.h"

#include "../../objects/woody.h"
#include "../../objects/neighbour.h"
#include "../../objects/intro.h"
#include "../../objects/sign.h"
#include "../../objects/tutorial.h"
#include "../../objects/level_end.h"

extern Scene PauseScene;

extern g2dImage* SpriteList_WOODY_GENERIC;
extern g2dImage* SpriteList_DOORLEFT;
extern g2dImage* SpriteList_DOORRIGHT;
extern g2dImage* SpriteList_DOORBACK;

extern g2dImage* SpriteList_NEIGHBOUR_GENERIC;

extern g2dImage* SpriteAtlas_INGAMEUI;

extern intraFont* Font_BLUEHIGB_18;

extern const Frame woody_generic_frames[];
extern const Animation woody_generic_animations[];

extern const Frame woody_doorleft_frames[];
extern const Animation woody_doorleft_animations[];

extern const Frame woody_doorright_frames[];
extern const Animation woody_doorright_animations[];

extern const Frame woody_doorback_frames[];
extern const Animation woody_doorback_animations[];

extern const Frame neighbour_generic_frames[];
extern const Animation neighbour_generic_animations[];

extern int camera_x;
extern int camera_y;
extern int camera_right;
extern int camera_bottom;

// void scene_tutorial_1(void) {
//     while (intro->song_timer < 79) {
//         g2dClear(BLACK);

//         intro_update(intro);

//         intro_draw(intro);

//         g2dFlip(G2D_VSYNC);
//     }
// }

static const char* current_episode_name;
static Intro* intro;
static bool intro_active;

static g2dImage* houseBg1;
static g2dImage* houseBg2;

static int room_count;
static RoomCollision room_collisions[9];

static hDoor* h_doors[9][2];
static vDoor* v_doors[9][MAX_V_DOORS_IN_ROOM];

static unsigned short new_hint_id;

static LookObject* look_objects[9][MAX_LOOK_OBJECTS_IN_ROOM];
static Hideout* hideouts[9];
static Storage* storages[9][MAX_STORAGES_IN_ROOM];

static LevelEnd* level_end;
static bool level_end_active;

static g2dImage* woody_spritelists[16];
static const Frame* woody_gfxdata[16];
static const Animation* woody_animations[16];
static Woody* woody;

static g2dImage* neighbour_spritelists[16];
static const Frame* neighbour_gfxdata[16];
static const Animation* neighbour_animations[16];
static Neighbour* neighbour;

static Tutorial* tutorial;
static bool tutorial_active;

static Sign* sign;

static bool pause_button_pressed;

// не используется
// static int get_unique_hint_id(void) {
//     new_hint_id++;
//     return new_hint_id - 1;
// }

static void init(void) {
    FILE* file = fopen("data/levels/tutorial_1.json", "r");

    if (!file) {
        scene_error("Не удалось открыть файл \"data/levels/tutorial_1.json\"");
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* json = malloc(size + 1);
    fread(json, 1, size, file);
    json[size] = '\0';
    fclose(file);
    cJSON* parsed_json = cJSON_Parse(json);
    free(json);

    // Интро
    current_episode_name = json_get_item_string(parsed_json, "episode_name", 127);

    intro = intro_create(
        current_episode_name, // episode_name
        NULL, // woody
        false, // move_woody
        0, 0 // camera_extra_x, camera_extra_y
    );

    intro_active = true;

    // == Progressbar == //
    g2dClear(BLACK);
    intro_draw_loading(0);
    g2dFlip(G2D_VSYNC);

    // Дом
    houseBg1 = g2d_LoadImage(json_get_item_string(parsed_json, "houseBg1", 255), G2D_CLUT8); 
    // houseBg2 = g2d_LoadImage(json_get_item_string(parsed_json, "houseBg2", 255), G2D_CLUT8); 
    houseBg2 = g2d_LoadImage(json_get_item_string(parsed_json, "houseBg2", 255), G2D_RGBA8888); // тут пока что ргба8888, т.к при клат8 появляются артефакты в виде "пепла" на полу

    // Комнаты
    cJSON* rooms = json_get_item_array(parsed_json, "rooms");

    room_count = cJSON_GetArraySize(rooms);

    // Загрузка коллизий из JSON
    for (int i = 0; i < room_count; i++) {
        cJSON* current_room = cJSON_GetArrayItem(rooms, i);
        
        RoomID room_type = string_to_room_type(json_get_item_string(current_room, "room", 15));

        RoomCollision* current_room_collision = &room_collisions[room_type];

        current_room_collision->x1 = json_get_item_number(current_room, "x1");
        current_room_collision->x2 = json_get_item_number(current_room, "x2");
        current_room_collision->floor = json_get_item_number(current_room, "floor");
    }

    // == Progressbar == //
    g2dClear(BLACK);
    intro_draw_loading(40);
    g2dFlip(G2D_VSYNC);

    // Горизонтальные двери
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < 2; i++) {
            h_doors[room][i] = NULL;
        }
    }

    // Загрузка дверей из JSON
    cJSON* parsed_h_doors = json_get_item_array(parsed_json, "hDoors");

    int parsed_h_doors_count = cJSON_GetArraySize(parsed_h_doors);

    for (int i = 0; i < parsed_h_doors_count; i++) {
        cJSON* parsed_current_door = cJSON_GetArrayItem(parsed_h_doors, i);
        
        RoomID room = string_to_room_type(json_get_item_string(parsed_current_door, "room", 15));

        int side = json_get_item_number(parsed_current_door, "side");

        if (side < 0) side = 0;
        if (side > 1) side = 1;

        g2dImage* current_spritelist = (side == 0) ? SpriteList_DOORLEFT : SpriteList_DOORRIGHT;

        hDoor* new_door = malloc(sizeof(hDoor));

        if (!new_door) continue;

        *new_door = (hDoor) {
            .spritelist = current_spritelist,
            .sprite_x = json_get_item_number(parsed_current_door, "sprite_x"),
            .sprite_y = json_get_item_number(parsed_current_door, "sprite_y"),
            .sprite_src_x = json_get_item_number(parsed_current_door, "sprite_src_x"),
            .sprite_src_y = json_get_item_number(parsed_current_door, "sprite_src_y"),
            .sprite_w = json_get_item_number(parsed_current_door, "sprite_w"),
            .sprite_h = json_get_item_number(parsed_current_door, "sprite_h"),
            .collision_x = json_get_item_number(parsed_current_door, "collision_x"),
            .exit_x = json_get_item_number(parsed_current_door, "exit_x"),
            .dest_door_room = string_to_room_type(json_get_item_string(parsed_current_door, "dest_door_room", 15)),
            .dest_door_id = json_get_item_number(parsed_current_door, "dest_door_id"),
            .using_by = USING_NONE
        };

        h_doors[room][side] = new_door;
    }

    // Вертикальные двери
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_V_DOORS_IN_ROOM; i++) {
            v_doors[room][i] = NULL;
        }
    }

    // Загрузка дверей из JSON
    cJSON* parsed_v_doors = json_get_item_array(parsed_json, "vDoors");

    int parsed_v_doors_count = cJSON_GetArraySize(parsed_v_doors);

    for (int i = 0; i < parsed_v_doors_count; i++) {
        cJSON* parsed_current_door = cJSON_GetArrayItem(parsed_v_doors, i);
        
        RoomID room = string_to_room_type(json_get_item_string(parsed_current_door, "room", 15));

        int index = 0;

        for (int i = 0; i < MAX_V_DOORS_IN_ROOM; i++) {
            if (!v_doors[room][i]) {
                index = i;
                break;
            }
        }

        vDoor* new_door = malloc(sizeof(vDoor));

        if (!new_door) continue;

        *new_door = (vDoor) {
            .spritelist = SpriteList_DOORBACK,
            .sprite_x = json_get_item_number(parsed_current_door, "sprite_x"),
            .sprite_y = json_get_item_number(parsed_current_door, "sprite_y"),
            .sprite_src_x = json_get_item_number(parsed_current_door, "sprite_src_x"),
            .sprite_src_y = json_get_item_number(parsed_current_door, "sprite_src_y"),
            .sprite_w = json_get_item_number(parsed_current_door, "sprite_w"),
            .sprite_h = json_get_item_number(parsed_current_door, "sprite_h"),
            .collision_x = json_get_item_number(parsed_current_door, "collision_x"),
            .collision_y = json_get_item_number(parsed_current_door, "collision_y"),
            .dest_door_room = string_to_room_type(json_get_item_string(parsed_current_door, "dest_door_room", 15)),
            .dest_door_id = json_get_item_number(parsed_current_door, "dest_door_id"),
            .using_by = USING_NONE
        };

        v_doors[room][index] = new_door;
    }

    // Интерактивные объекты
    new_hint_id = 0;

    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            look_objects[room][i] = NULL;
        }
    }

    // Hideouts
    for (int i = 0; i < room_count; i++) {
        hideouts[i] = NULL;
    }

    // Хранилища
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            storages[room][i] = NULL;
        }
    }

    // LevelEnd
    level_end = level_end_create();

    level_end_active = false;

    // Вуди
    woody_spritelists[0] = SpriteList_WOODY_GENERIC;
    woody_spritelists[1] = SpriteList_DOORLEFT;
    woody_spritelists[2] = SpriteList_DOORRIGHT;
    woody_spritelists[3] = SpriteList_DOORBACK;

    woody_gfxdata[0] = woody_generic_frames;
    woody_gfxdata[1] = woody_doorleft_frames;
    woody_gfxdata[2] = woody_doorright_frames;
    woody_gfxdata[3] = woody_doorback_frames;
    
    woody_animations[0] = woody_generic_animations;
    woody_animations[1] = woody_doorleft_animations;
    woody_animations[2] = woody_doorright_animations;
    woody_animations[3] = woody_doorback_animations;

    woody = woody_create(
        woody_spritelists, // spritelists
        woody_gfxdata, // gfxdata
        woody_animations, // animations

       json_get_item_number(parsed_json, "woody_start_x"), json_get_item_number(parsed_json, "woody_start_y"), // start_x, start_y

        h_doors, v_doors, // h_doors, v_doors
        look_objects, // look_objects
        hideouts, // hideouts
        storages, // storages

        json_get_item_number(parsed_json, "camera_limit_x"), json_get_item_number(parsed_json, "camera_limit_y"), // camera_limit_x, camera_limit_y

        ROOM_KIT, // room
        room_collisions, // room_collisions

        json_get_item_number(parsed_json, "min_quota"), // min_quota
        json_get_item_number(parsed_json, "total_tricks"), // total_tricks

        NULL, // neighbour
        NULL, // neighbour_active
        level_end, // level_end
        &level_end_active // level_end_active
    );
    intro->woody = woody;

    neighbour_spritelists[0] = SpriteList_NEIGHBOUR_GENERIC;
    neighbour_spritelists[1] = SpriteList_DOORLEFT;
    neighbour_spritelists[2] = SpriteList_DOORRIGHT;
    neighbour_spritelists[3] = SpriteList_DOORBACK;

    neighbour_gfxdata[0] = neighbour_generic_frames;

    neighbour_animations[0] = neighbour_generic_animations;

    neighbour = neighbour_create(
        neighbour_spritelists, // spritelists
        neighbour_gfxdata, // gfxdata
        neighbour_animations, // animations

        0, 0, // start_x, start_y
        ROOM_NONE, // start_room
        BUBBLE_SOFA, // start_bubble
        NULL, NULL, // h_doors, v_doors
        NULL, // look_objects
        woody, // woody
        NULL, // neighbour_active
        NULL // level_end_active
    );
    woody->neighbour = neighbour;

    // Туториал
    cJSON* tutorial_phrases = json_get_item_array(parsed_json, "tutorial_phrases");

    int tutorial_phrases_count = cJSON_GetArraySize(tutorial_phrases);

    if (tutorial_phrases_count > TUTORIAL_MAX_PHRASES) {
        scene_error("Количество фраз в туториале больше %d", TUTORIAL_MAX_PHRASES);
    }

    Phrase phrases[tutorial_phrases_count];

    for (int i = 0; i < tutorial_phrases_count; i++) {
        cJSON* current_tutorial_phrase = cJSON_GetArrayItem(tutorial_phrases, i);

        // Отрабатываем фразы
        cJSON* current_phrase_texts = json_get_item_array(current_tutorial_phrase, "phrase_texts");
        int phrase_text_count = cJSON_GetArraySize(current_phrase_texts);

        if (phrase_text_count > TUTORIAL_MAX_PHRASE_TEXTS) {
            scene_error("Количество строк в фразе больше %d", TUTORIAL_MAX_PHRASE_TEXTS);
        }

        Phrase* current_phrase = &phrases[i];

        // Отрабатываем строки
        for (int j = 0; j < phrase_text_count; j++) {
            cJSON* parsed_current_phrase_text = cJSON_GetArrayItem(current_phrase_texts, j);
            const char* current_text = json_get_item_string(parsed_current_phrase_text, "text", 255);
            PhraseText* current_phrase_text = &current_phrase->phrase_texts[j];
            strcpy(current_phrase_text->text, current_text);
            current_phrase_text->y = json_get_item_number(parsed_current_phrase_text, "y");
        }

        current_phrase->phrase_text_count = phrase_text_count;
        current_phrase->window_size = json_get_item_number(current_tutorial_phrase, "window_size");
    }

    tutorial = tutorial_create(phrases, tutorial_phrases_count);

    tutorial_active = false;

    sign = sign_create(
        h_doors, v_doors, // h_doors, v_doors
        tutorial, // tutorial
        woody // woody
    );

    // Интерфейс
    pause_button_pressed = false;

    cJSON_Delete(parsed_json);

    NFHMusicPlay(MUSIC_JINGLE_LEVELSTART, 0);
}

static void update(void) {
    controls_read();

    woody_update(woody);

    sign_update(sign, woody_check_sign_collision(woody, sign));

    // Пауза
    if (controls_released(PSP_CTRL_START)) {
        if (!intro->draw_intro) {
            scene_push(&PauseScene);
        }

        pause_button_pressed = false;
    }

    if (controls_pressed(PSP_CTRL_START) && !intro->draw_intro)
        NFHSoundPlay(SOUND_BUT1);

    if (controls_held(PSP_CTRL_START))
        pause_button_pressed = true;

    // Интерфейс
    if (tutorial_active)
        tutorial_update(tutorial);

    if (intro_active) {
        int intro_exit_code = intro_update(intro);

        switch (intro_exit_code) {
            case INTRO_WOODY_START_MOVE_TO_HOUSE: {
                woody->state = STATE_LEVEL_START;

                break;
            }

            case INTRO_WOODY_START_ANIMATION: {
                woody->state = STATE_STOP;
                woody->can_move = false;
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_START);

                break;
            }

            case INTRO_WOODY_CAN_MOVE: {
                woody->state = STATE_H_MOVE;
                woody->can_move = true;

                sign_show(sign);
                tutorial_active = true;

                break;
            }

            case INTRO_DELETE: {
                intro_active = false;
                    
                break;
            }
        }
    }

    if (level_end_active)
        level_end_update(level_end);
        
    camera_right = camera_x + 480;
    camera_bottom = camera_y + 272;
}

static void draw(void) {
    g2dClear(BLACK);

    // Отрисовка дома
    g2d_DrawImage(houseBg1, -camera_x, -camera_y, WHITE, 0, 255, G2D_UP_LEFT);

    if (camera_x > 32)
        g2d_DrawImage(houseBg2, 512 - camera_x, -camera_y, WHITE, 0, 255, G2D_UP_LEFT);

    // Отрисовка дверей

    // h_doors

    // Левые двери
    for (int room = 0; room < room_count; room++) {
        hDoor* current_door = h_doors[room][0]; // Левая дверь всегда под индексом 0

        if (!current_door) continue;

        if (current_door->using_by == USING_NONE) {
            if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                current_door->sprite_x >= camera_right ||
                current_door->sprite_y + current_door->sprite_h <= camera_y ||
                current_door->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // Правые двери
    for (int room = 0; room < room_count; room++) {
        hDoor* current_door = h_doors[room][1]; // Левая дверь всегда под индексом 1

        if (!current_door) continue;

        if (current_door->using_by == USING_NONE) {
            if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                current_door->sprite_x >= camera_right ||
                current_door->sprite_y + current_door->sprite_h <= camera_y ||
                current_door->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // v_doors
    for (int room = 0; room < room_count; room++) {
        for (int door = 0; door < MAX_V_DOORS_IN_ROOM; door++) {
            vDoor* current_door = v_doors[room][door];

            if (!current_door) continue;

            if (current_door->using_by == USING_NONE) {
                if (current_door->sprite_x + current_door->sprite_w <= camera_x || 
                    current_door->sprite_x >= camera_right ||
                    current_door->sprite_y + current_door->sprite_h <= camera_y ||
                    current_door->sprite_y >= camera_bottom)
                    continue;

                g2d_DrawImageExt(current_door->spritelist, current_door->sprite_x - camera_x, current_door->sprite_y - camera_y, current_door->sprite_w, current_door->sprite_h, WHITE, current_door->sprite_src_x, current_door->sprite_src_y, current_door->sprite_w, current_door->sprite_h, 0, 255, G2D_UP_LEFT);
            }
        }
    }

    // look_objects
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            LookObject* current_look_object = look_objects[room][i];

            if (!current_look_object) break; // Важно, чтобы look_objects по порядку ставили

            if (current_look_object->sprite_x + current_look_object->sprite_w <= camera_x || 
                current_look_object->sprite_x >= camera_right ||
                current_look_object->sprite_y + current_look_object->sprite_h <= camera_y ||
                current_look_object->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_look_object->spritelist, current_look_object->sprite_x - camera_x, current_look_object->sprite_y - camera_y, current_look_object->sprite_w, current_look_object->sprite_h, WHITE, current_look_object->sprite_src_x, current_look_object->sprite_src_y, current_look_object->sprite_w, current_look_object->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // storages
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            Storage* current_storage = storages[room][i];

            if (!current_storage) break; // Важно, чтобы storages по порядку ставили
            
            if (!current_storage->spritelist) continue;

            if (current_storage->sprite_x + current_storage->sprite_w <= camera_x || 
                current_storage->sprite_x >= camera_right ||
                current_storage->sprite_y + current_storage->sprite_h <= camera_y ||
                current_storage->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_storage->spritelist, current_storage->sprite_x - camera_x, current_storage->sprite_y - camera_y, current_storage->sprite_w, current_storage->sprite_h, WHITE, current_storage->sprite_src_x, current_storage->sprite_src_y, current_storage->sprite_w, current_storage->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    if (woody->in_door) {
        woody_door_draw(woody);
    }

    sign_draw(sign);

    if (!woody->in_door) {
        woody_draw(woody);
    }

    // Отрисовка UI

    // UI BG
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 0, 192, 480, 80, WHITE, 0, 0, 480, 80, 0, 255, G2D_UP_LEFT);

    woody_draw_ui(woody);

    neighbour_draw_ui(neighbour);

    if (pause_button_pressed && !intro->draw_intro) {
        // Pause Button
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 450, 208, 20, 20, WHITE, 480, 0, 20, 20, 0, 255, G2D_UP_LEFT);
    }

    if (tutorial_active)
        tutorial_draw(tutorial);

    if (intro_active)
        intro_draw(intro);

    if (level_end_active)
        level_end_draw(level_end);

    g2dFlip(G2D_VSYNC);
}

static void unload(void) {
    g2d_FreeImage(houseBg1);
    g2d_FreeImage(houseBg2);

    // h_doors_unload
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < 2; i++) {
            if (h_doors[room][i]) {
                free(h_doors[room][i]);
                h_doors[room][i] = NULL;
            }
        }
    }

    // v_doors_unload
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_V_DOORS_IN_ROOM; i++) {
            if (v_doors[room][i]) {
                free(v_doors[room][i]);
                v_doors[room][i] = NULL;
            }
        }
    }

    // look_objects_unload
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            if (look_objects[room][i]) {
                free(look_objects[room][i]);
                look_objects[room][i] = NULL;
            }
        }
    }

    // hideouts unload
    for (int room = 0; room < room_count; room++) {
        if (hideouts[room]) {
            free(hideouts[room]);
        }
    }

    // storages unload
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            if (storages[room][i]) {
                free(storages[room][i]);
                storages[room][i] = NULL;
            }
        }
    }

    woody_unload(woody);
    neighbour_unload(neighbour);
    intro_unload(intro);
    sign_unload(sign);
    tutorial_unload(tutorial);
    level_end_unload(level_end);

    NFHHouseMusicStop();
}

Scene Tutorial1Scene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = unload
};
