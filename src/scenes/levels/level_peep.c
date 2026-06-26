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
extern g2dImage* SpriteAtlas_STORAGES;

extern intraFont* Font_BLUEHIGB_18;

extern const Frame woody_generic_frames[];
extern const Animation woody_generic_animations[];

extern const Frame woody_doorleft_frames[];
extern const Animation woody_doorleft_animations[];

extern const Frame woody_doorright_frames[];
extern const Animation woody_doorright_animations[];

extern const Frame woody_doorback_frames[];
extern const Animation woody_doorback_animations[];

extern const Frame woody_generic2_frames[];
extern const Animation woody_generic2_animations[];

extern const Frame neighbour_generic_frames[];
extern const Animation neighbour_generic_animations[];

extern const Frame neighbour_doorleft_frames[];
extern const Animation neighbour_doorleft_animations[];

extern const Frame neighbour_doorright_frames[];
extern const Animation neighbour_doorright_animations[];

extern const Frame neighbour_sofa_frames[];
extern const Animation neighbour_sofa_animations[];

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
static vDoor* v_doors[9][3];

static unsigned short new_hint_id;

static g2dImage* Sprite_SOFA_MS;

static LookObject* look_objects[9][8];
static Hideout* hideouts[9];
static Storage* storages[9][8];

static LevelEnd* level_end;
static bool level_end_active;

static g2dImage* woody_spritelists[16];
static const Frame* woody_gfxdata[16];
static const Animation* woody_animations[16];
static Woody* woody;

static int logic_frame;

static g2dImage* neighbour_spritelists[16];
static const Frame* neighbour_gfxdata[16];
static const Animation* neighbour_animations[16];
static Neighbour* neighbour;
static bool neighbour_active;

static bool pause_button_pressed;

static int get_unique_hint_id(void) {
    new_hint_id++;
    return new_hint_id - 1;
}

static void init(void) {
    FILE* file = fopen("data/levels/level_peep.json", "r");

    if (file == NULL) {
        scene_error("Не удалось открыть файл \"data/levels/level_peep.json\"");
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
        true // move_woody
    );

    intro_active = true;

    // == Progressbar == //
    g2dClear(BLACK);
    intro_draw_loading(0);
    g2dFlip(G2D_VSYNC);

    // Дом
    houseBg1 = g2d_LoadImage(json_get_item_string(parsed_json, "houseBg1", 255), G2D_CLUT8); 
    houseBg2 = g2d_LoadImage(json_get_item_string(parsed_json, "houseBg2", 255), G2D_CLUT8); 

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
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 2; j++) {
            h_doors[i][j] = NULL;
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

        if (!new_door)
            continue;

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
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 3; j++) {
            v_doors[i][j] = NULL;
        }
    }

    // Загрузка дверей из JSON
    cJSON* parsed_v_doors = json_get_item_array(parsed_json, "vDoors");

    int parsed_v_doors_count = cJSON_GetArraySize(parsed_v_doors);

    for (int i = 0; i < parsed_v_doors_count; i++) {
        cJSON* parsed_current_door = cJSON_GetArrayItem(parsed_v_doors, i);
        
        RoomID room = string_to_room_type(json_get_item_string(parsed_current_door, "room", 15));

        int index = 0;

        for (int i = 0; i < 3; i++) {
            if (v_doors[room][i] == NULL) {
                index = i;
                break;
            }
        }

        vDoor* new_door = malloc(sizeof(vDoor));

        if (!new_door)
            continue;

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

    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 8; j++) {
            look_objects[i][j] = NULL;
        }
    }

    LookObject* new_look_object1 = malloc(sizeof(LookObject));
    if (new_look_object1) {
        *new_look_object1 = (LookObject) {
            .spritelist = SpriteList_DOORBACK,
            .sprite_x = 275,
            .sprite_y = 66,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 46,
            .sprite_h = 70,
            .sprite_show = true,
            .collision_x = 246,
            .collision_y = 86,
            .hint_text = "Посмотреть на запертую дверь",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Эта дверь ведет в\n\n\nспальню. Туда мне не\n\n\nпройти.",
            .phrase_y = 0,
            .bubble_size = 18
        };
        look_objects[ROOM_LIR][0] = new_look_object1;
    }

    LookObject* new_look_object2 = malloc(sizeof(LookObject));
    if (new_look_object2) {
        *new_look_object2 = (LookObject) {
            .spritelist = SpriteList_DOORBACK,
            .sprite_x = 429,
            .sprite_y = 208,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 46,
            .sprite_h = 70,
            .sprite_show = true,
            .collision_x = 400,
            .collision_y = 228,
            .hint_text = "Посмотреть на запертую дверь",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Эта дверь ведет в\n\n\nподвал. Мне туда не\n\n\nпройти.",
            .phrase_y = 0,
            .bubble_size = 18
        };
        look_objects[ROOM_ANC][0] = new_look_object2;
    }

    LookObject* new_look_object3 = malloc(sizeof(LookObject));
    if (new_look_object3) {
        *new_look_object3 = (LookObject) {
            .spritelist = NULL,
            .sprite_x = 0,
            .sprite_y = 0,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 0,
            .sprite_h = 0,
            .sprite_show = true,
            .collision_x = 404,
            .collision_y = 102,
            .hint_text = "Посмотреть на микроволновку",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Микроволновка. Говорят,\n\n\nесли засунуть туда\n\n\nсырое яйцо, будет весело.",
            .phrase_y = 0,
            .bubble_size = 18
        };
        look_objects[ROOM_KIT][0] = new_look_object3;
    }

    LookObject* new_look_object4 = malloc(sizeof(LookObject));
    if (new_look_object4) {
        *new_look_object4 = (LookObject) {
            .spritelist = NULL,
            .sprite_x = 0,
            .sprite_y = 0,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 0,
            .sprite_h = 0,
            .sprite_show = true,
            .collision_x = 599,
            .collision_y = 115,
            .hint_text = "Посмотреть на бинокль",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "В этот бинокль\n\n\nсоседушка следит за\n\n\nтем, как раздевается\n\n\nдевушка в окне напротив!",
            .phrase_y = -6,
            .bubble_size = 22
        };
        look_objects[ROOM_KIT][1] = new_look_object4;
    }

    Sprite_SOFA_MS = g2d_LoadImage("assets/sprites/lir/sofa/sofa_ms.png", G2D_CLUT8);

    LookObject* new_look_object5 = malloc(sizeof(LookObject));
    if (new_look_object5) {
        *new_look_object5 = (LookObject) {
            .spritelist = Sprite_SOFA_MS,
            .sprite_x = 173,
            .sprite_y = 107,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 61,
            .sprite_h = 56,
            .sprite_show = true,
            .collision_x = 161,
            .collision_y = 111,
            .hint_text = "Посмотреть на кресло",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Здесь он сидит и\n\n\nсмотрит телевизор.",
            .phrase_y = 8,
            .bubble_size = 10
        };
        look_objects[ROOM_LIR][1] = new_look_object5;
    }

    // Загрузка укрытий из JSON
    cJSON* parsed_hideouts = json_get_item_array(parsed_json, "hideouts");

    int parsed_hideouts_count = cJSON_GetArraySize(parsed_hideouts);

    for (int i = 0; i < parsed_hideouts_count; i++) {
        cJSON* parsed_current_hideout = cJSON_GetArrayItem(parsed_hideouts, i);
        
        RoomID room = string_to_room_type(json_get_item_string(parsed_current_hideout, "room", 15));

        Hideout* new_hideout = malloc(sizeof(Hideout));

        if (!new_hideout)
            continue;

        *new_hideout = (Hideout) {
            .type = HIDEOUT_WARDROBE, // TODO: ДОБАВИТЬ ПАРС ЭТОГО ЗНАЧЕНИЯ
            .collision_x = json_get_item_number(parsed_current_hideout, "collision_x"),
            .collision_y = json_get_item_number(parsed_current_hideout, "collision_y"),
            .hint_id = get_unique_hint_id()
        };

        strcpy(new_hideout->hint_enter_text, json_get_item_string(parsed_current_hideout, "hint_enter_text", 255));
        strcpy(new_hideout->hint_exit_text, json_get_item_string(parsed_current_hideout, "hint_exit_text", 255));

        hideouts[room] = new_hideout;
    }

    // Хранилища
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 8; j++) {
            storages[i][j] = NULL;
        }
    }

    Storage* new_storage1 = malloc(sizeof(Storage));
    if (new_storage1) {
        *new_storage1 = (Storage) {
            .spritelist = SpriteAtlas_STORAGES,
            .sprite_x = 476,
            .sprite_y = 246,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 33,
            .sprite_h = 18,
            .sprite_type = SPRITE_TYPE_OPENS,
            .sprite_show = false,
            .collision_x = 431,
            .collision_y = 239,
            .hint_text = "Открыть комод",
            .hint_id = get_unique_hint_id(),
            .items = {ITEM_FARTBAG, ITEM_NONE, ITEM_NONE, ITEM_NONE},
            .opened = false,
            .side = SIDE_UP_MID
        };
        storages[ROOM_ANC][0] = new_storage1;
    }

    Storage* new_storage2 = malloc(sizeof(Storage));
    if (new_storage2) {
        *new_storage2 = (Storage) {
            .spritelist = SpriteAtlas_STORAGES,
            .sprite_x = 58,
            .sprite_y = 228,
            .sprite_src_x = 33,
            .sprite_src_y = 0,
            .sprite_w = 10,
            .sprite_h = 27,
            .sprite_type = SPRITE_TYPE_OPENS,
            .sprite_show = false,
            .collision_x = 18,
            .collision_y = 254,
            .hint_text = "Открыть аптечку",
            .hint_id = get_unique_hint_id(),
            .items = {ITEM_SUPERGLUE, ITEM_NONE, ITEM_NONE, ITEM_NONE},
            .opened = false,
            .side = SIDE_LEFT_HIGH
        };
        storages[ROOM_TOI][0] = new_storage2;
    }

    Storage* new_storage3 = malloc(sizeof(Storage));
    if (new_storage3) {
        *new_storage3 = (Storage) {
            .spritelist = SpriteAtlas_STORAGES,
            .sprite_x = 622,
            .sprite_y = 91,
            .sprite_src_x = 43,
            .sprite_src_y = 0,
            .sprite_w = 45,
            .sprite_h = 59,
            .sprite_type = SPRITE_TYPE_OPENS,
            .sprite_show = false,
            .collision_x = 576,
            .collision_y = 100,
            .hint_text = "Открыть холодильник",
            .hint_id = get_unique_hint_id(),
            .items = {ITEM_EGG, ITEM_NONE, ITEM_NONE, ITEM_NONE},
            .opened = false,
            .side = SIDE_UP_MID
        };
        storages[ROOM_KIT][0] = new_storage3;
    }

    // LevelEnd
    level_end = level_end_create();

    level_end_active = false;

    // Вуди
    woody_spritelists[0] = SpriteList_WOODY_GENERIC;
    woody_spritelists[1] = SpriteList_DOORLEFT;
    woody_spritelists[2] = SpriteList_DOORRIGHT;
    woody_spritelists[3] = SpriteList_DOORBACK;
    woody_spritelists[4] = g2d_LoadImage("assets/sprites/woody/generic2.png", G2D_CLUT8); 

    woody_gfxdata[0] = woody_generic_frames;
    woody_gfxdata[1] = woody_doorleft_frames;
    woody_gfxdata[2] = woody_doorright_frames;
    woody_gfxdata[3] = woody_doorback_frames;
    woody_gfxdata[4] = woody_generic2_frames;
    
    woody_animations[0] = woody_generic_animations;
    woody_animations[1] = woody_doorleft_animations;
    woody_animations[2] = woody_doorright_animations;
    woody_animations[3] = woody_doorback_animations;
    woody_animations[4] = woody_generic2_animations;

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

        ROOM_FRO, // room
        room_collisions, // room_collisions

        json_get_item_number(parsed_json, "min_quota"), // min_quota
        json_get_item_number(parsed_json, "total_tricks"), // total_tricks

        level_end, // level_end
        &level_end_active // level_end_active
    );

    neighbour_spritelists[0] = SpriteList_NEIGHBOUR_GENERIC;
    neighbour_spritelists[1] = SpriteList_DOORLEFT;
    neighbour_spritelists[2] = SpriteList_DOORRIGHT;
    // neighbour_spritelists[3] = SpriteList_DOORBACK;
    neighbour_spritelists[4] = g2d_LoadImage("assets/sprites/lir/sofa/level_peep.png", G2D_CLUT8);

    neighbour_gfxdata[0] = neighbour_generic_frames;
    neighbour_gfxdata[1] = neighbour_doorleft_frames;
    neighbour_gfxdata[2] = neighbour_doorright_frames;
    // neighbour_gfxdata[3] = neighbour_doorback_frames;
    neighbour_gfxdata[4] = neighbour_sofa_frames;

    neighbour_animations[0] = neighbour_generic_animations;
    neighbour_animations[1] = neighbour_doorleft_animations;
    neighbour_animations[2] = neighbour_doorright_animations;
    // neighbour_animations[3] = neighbour_doorback_animations;
    neighbour_animations[4] = neighbour_sofa_animations;

    neighbour = neighbour_create(
        neighbour_spritelists, // spritelists
        neighbour_gfxdata, // gfxdata
        neighbour_animations, // animations

        22, -56, // start_x, start_y
        ROOM_LIR, // start_room
        BUBBLE_SOFA, // start_bubble
        h_doors, v_doors, // h_doors, v_doors
        look_objects // look_objects
    );
    neighbour_active = false;

    logic_frame = 0;

    // Интерфейс
    pause_button_pressed = false;

    cJSON_Delete(parsed_json);

    NFHMusicPlay(MUSIC_JINGLE_LEVELSTART, 0);
}

static void update(void) {
    controls_read();

    woody_update(woody);

    logic_frame++;

    if (logic_frame == 5) {
        if (neighbour_active) {
            neighbour_update(neighbour);
        }
        logic_frame = 0;
    }

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
    if (intro_active) {
        int intro_exit_code = intro_update(intro);

        switch (intro_exit_code) {
            case INTRO_WOODY_START_MOVE_TO_HOUSE:
                woody->state = STATE_LEVEL_START;

                break;
            case INTRO_NEIGHBOUR_CAN_MOVE:
                neighbour_active = true;
                neighbour->bubble_show = true;

                break;
            case INTRO_WOODY_START_ANIMATION:
                woody->state = STATE_STOP;
                woody->can_move = false;
                woody_animation_set(woody, ANIMATION_PACK_WOODY_GENERIC, ANIMATION_WOODY_START);

                break;
            case INTRO_WOODY_CAN_MOVE:
                woody->state = STATE_H_MOVE;
                woody->can_move = true;

                break;
            case INTRO_DELETE:
                intro_active = false;
                    
                break;
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
    g2d_DrawImageEasy(houseBg1, -camera_x, -camera_y, WHITE, 0, 255, G2D_UP_LEFT);

    if (camera_x > 32)
        g2d_DrawImageEasy(houseBg2, 512 - camera_x, -camera_y, WHITE, 0, 255, G2D_UP_LEFT);

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
        hDoor* current_door = h_doors[room][1]; // Правая дверь всегда под индексом 1

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

    if (woody->in_door) {
        woody_door_draw(woody);
    }

    if (neighbour->in_door) {
        neighbour_door_draw(neighbour);
    }

    // look_objects
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < 8; i++) {
            LookObject* current_look_object = look_objects[room][i];

            if (current_look_object == NULL)
                break; // Важно, чтобы look_objects по порядку ставили
            
            if (current_look_object->spritelist == NULL)
                continue;

            if (!current_look_object->sprite_show)
                continue;

            if (current_look_object->sprite_x + current_look_object->sprite_w <= camera_x || 
                current_look_object->sprite_x >= camera_right ||
                current_look_object->sprite_y + current_look_object->sprite_h <= camera_y ||
                current_look_object->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImage(current_look_object->spritelist, current_look_object->sprite_x - camera_x, current_look_object->sprite_y - camera_y, current_look_object->sprite_w, current_look_object->sprite_h, WHITE, current_look_object->sprite_src_x, current_look_object->sprite_src_y, current_look_object->sprite_w, current_look_object->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // storages
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < 8; i++) {
            Storage* current_storage = storages[room][i];

            if (current_storage == NULL)
                break; // Важно, чтобы storages по порядку ставили
            
            if (current_storage->spritelist == NULL)
                continue;

            if (!current_storage->sprite_show)
                continue;

            if (current_storage->sprite_x + current_storage->sprite_w <= camera_x || 
                current_storage->sprite_x >= camera_right ||
                current_storage->sprite_y + current_storage->sprite_h <= camera_y ||
                current_storage->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImage(current_storage->spritelist, current_storage->sprite_x - camera_x, current_storage->sprite_y - camera_y, current_storage->sprite_w, current_storage->sprite_h, WHITE, current_storage->sprite_src_x, current_storage->sprite_src_y, current_storage->sprite_w, current_storage->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    if (!woody->in_door) {
        woody_draw(woody);
    }

    if (!neighbour->in_door) {
        neighbour_draw(neighbour);
    }

    // Отрисовка UI

    // UI BG
    g2d_DrawImage(SpriteAtlas_INGAMEUI, 0, 192, 480, 80, WHITE, 0, 0, 480, 80, 0, 255, G2D_UP_LEFT);

    woody_draw_ui(woody);

    neighbour_draw_ui(neighbour);

    if (pause_button_pressed && !intro->draw_intro) {
        // Pause Button
        g2d_DrawImage(SpriteAtlas_INGAMEUI, 450, 208, 20, 20, WHITE, 480, 0, 20, 20, 0, 255, G2D_UP_LEFT);
    }

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
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 2; j++) {
            if (h_doors[i][j] != NULL) {
                free(h_doors[i][j]);
                h_doors[i][j] = NULL;
            }
        }
    }

    // v_doors_unload
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 3; j++) {
            if (v_doors[i][j] != NULL) {
                free(v_doors[i][j]);
                v_doors[i][j] = NULL;
            }
        }
    }

    g2d_FreeImage(Sprite_SOFA_MS);

    // look_objects_unload
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 8; j++) {
            if (look_objects[i][j] != NULL) {
                free(look_objects[i][j]);
                look_objects[i][j] = NULL;
            }
        }
    }

    // hideouts unload
    for (int i = 0; i < room_count; i++) {
        if (hideouts[i] != NULL) {
            free(hideouts[i]);
        }
    }

    // storages unload
    for (int i = 0; i < room_count; i++) {
        for (int j = 0; j < 8; j++) {
            if (storages[i][j] != NULL) {
                free(storages[i][j]);
                storages[i][j] = NULL;
            }
        }
    }

    g2d_FreeImage(woody->spritelists[4]);
    woody_unload(woody);

    g2d_FreeImage(neighbour_spritelists[4]);
    neighbour_unload(neighbour);
    intro_unload(intro);
    level_end_unload(level_end);

    NFHHouseMusicStop();
}

Scene LevelPeepScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = unload
};
