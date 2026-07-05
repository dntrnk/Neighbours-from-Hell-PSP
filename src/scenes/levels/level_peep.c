#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

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
#include "../../types/use_object_types.h"

#include "../../objects/woody.h"
#include "../../objects/neighbour.h"
#include "../../objects/intro.h"
#include "../../objects/sign.h"
#include "../../objects/tutorial.h"
#include "../../objects/level_end.h"

#define CG_ORANGE_BREAKDOWNS G2D_RGB(254, 159, 0)

extern Scene PauseScene;

extern g2dImage* SpriteList_WOODY_GENERIC;
extern g2dImage* SpriteList_DOORLEFT;
extern g2dImage* SpriteList_DOORRIGHT;
extern g2dImage* SpriteList_DOORBACK;

extern g2dImage* SpriteList_NEIGHBOUR_GENERIC;

extern g2dImage* SpriteAtlas_INGAMEUI;
extern g2dImage* SpriteAtlas_STORAGES;

extern intraFont* Font_BLUEHIGC_24;
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

extern const Frame woody_generic3_frames[];
extern const Animation woody_generic3_animations[];

extern const Frame neighbour_generic_frames[];
extern const Animation neighbour_generic_animations[];

extern const Frame neighbour_doorleft_frames[];
extern const Animation neighbour_doorleft_animations[];

extern const Frame neighbour_doorright_frames[];
extern const Animation neighbour_doorright_animations[];

extern const Frame neighbour_sofa_frames[];
extern const Animation neighbour_sofa_animations[];

extern const Frame neighbour_binoculars_frames[];
extern const Animation neighbour_binoculars_animations[];

extern const Frame neighbour_generic2_frames[];
extern const Animation neighbour_generic2_animations[];

extern const Frame neighbour_generic3_frames[];
extern const Animation neighbour_generic3_animations[];

extern const Frame neighbour_look_frames[];
extern const Animation neighbour_look_animations[];

extern const Frame neighbour_shout0_frames[];
extern const Animation neighbour_shout0_animations[];

extern const Frame neighbour_sofa1_frames[];
extern const Animation neighbour_sofa1_animations[];

extern const Frame neighbour_gameover_frames[];
extern const Animation neighbour_gameover_animations[];

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

static g2dImage* Sprite_DIRTYMICROWAVE;
static g2dImage* Sprite_MICROWAVE_COOK;

static g2dImage* Sprite_BINOCULARS_MS;
static g2dImage* Sprite_BINOCULARS_GLUE_MS;

static g2dImage* Sprite_SOFA_MS;

static LookObject* look_objects[9][MAX_LOOK_OBJECTS_IN_ROOM];
static Hideout* hideouts[9];
static Storage* storages[9][MAX_STORAGES_IN_ROOM];
static UseObject* use_objects[9][MAX_USE_OBJECTS_IN_ROOM];

g2dImage* SpriteList_TV;
g2dImage* Sprite_TWISTEDANTENNA;

static int tv_animation_frame;
static int tv_animation_length;
static int tv_animation_frame_time;

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

static void microwave_on_making_trick(void) {
    look_objects[ROOM_KIT][0]->spritelist = Sprite_MICROWAVE_COOK;
    look_objects[ROOM_KIT][0]->sprite_x = 442;
    look_objects[ROOM_KIT][0]->sprite_y = 100;
    look_objects[ROOM_KIT][0]->sprite_w = 28;
    look_objects[ROOM_KIT][0]->sprite_h = 19;
    look_objects[ROOM_KIT][0]->sprite_show = true;
}

static void microwave_on_stop_making_trick(void) {
    look_objects[ROOM_KIT][0]->sprite_show = false;
}

static void microwave_on_trick(void) {
    look_objects[ROOM_KIT][0]->spritelist = Sprite_DIRTYMICROWAVE;
    look_objects[ROOM_KIT][0]->sprite_x = 440;
    look_objects[ROOM_KIT][0]->sprite_y = 100;
    look_objects[ROOM_KIT][0]->sprite_w = 30;
    look_objects[ROOM_KIT][0]->sprite_h = 19;
    look_objects[ROOM_KIT][0]->sprite_show = true;
}

static void microwave_on_untrick(void) {
    look_objects[ROOM_KIT][0]->sprite_show = false;
}

static void binoculars_on_trick(void) {
    look_objects[ROOM_KIT][1]->spritelist = Sprite_BINOCULARS_GLUE_MS;
}

static void binoculars_on_untrick(void) {
    look_objects[ROOM_KIT][1]->sprite_show = false;
    look_objects[ROOM_KIT][1]->collision_x = 2048;
    look_objects[ROOM_KIT][1]->alt_action = true;
}

static void tv_on_trick(void) {
    use_objects[ROOM_LIR][0]->sprite_src_x = 0;
    use_objects[ROOM_LIR][0]->sprite_src_y = 22;
    use_objects[ROOM_LIR][0]->collision_x = -128;
    look_objects[ROOM_LIR][2]->collision_x = 49;
    look_objects[ROOM_LIR][2]->sprite_show = true;
    tv_animation_frame = 0;
    tv_animation_length = 2;
}

static void tv_on_untrick(void) {
    use_objects[ROOM_LIR][0]->sprite_src_x = 0;
    use_objects[ROOM_LIR][0]->sprite_src_y = 0;
    use_objects[ROOM_LIR][0]->collision_x = 49;
    use_objects[ROOM_LIR][0]->first_time = false;
    look_objects[ROOM_LIR][2]->collision_x = -128;
    look_objects[ROOM_LIR][2]->sprite_show = false;
    tv_animation_frame = 0;
    tv_animation_length = 8;
}

static inline int clamp(int x, int min, int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

static int get_unique_hint_id(void) {
    new_hint_id++;
    return new_hint_id - 1;
}

static void init(void) {
    FILE* file = fopen("data/levels/level_peep.json", "r");

    if (!file) {
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
        NULL, // woody
        true, // move_woody
        -234, -147 // camera_extra_x, camera_extra_y
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

    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            look_objects[room][i] = NULL;
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
            .above_door = false,
            .collision_x = 246,
            .collision_y = 86,
            .hint_text = "Посмотреть на запертую дверь",
            .use_hint_text = "запертой двери",
            .tricked_hint_text = "",
            .tricked_use_hint_text = "",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Эта дверь ведет в\n\n\nспальню. Туда мне не\n\n\nпройти.",
            .phrase_y = 0,
            .bubble_size = 18,
            .tricked = false,
            .item_to_trick = ITEM_NONE,
            .alt_action = false,
            .trick_making_length = 0,
            .trick_tv_rating = 0,
            .tricked_phrase_text = "",
            .tricked_phrase_y = 0,
            .tricked_bubble_size = 0,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = NULL,
            .on_untrick = NULL
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
            .above_door = false,
            .collision_x = 400,
            .collision_y = 228,
            .hint_text = "Посмотреть на запертую дверь",
            .use_hint_text = "запертой двери",
            .tricked_hint_text = "",
            .tricked_use_hint_text = "",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Эта дверь ведет в\n\n\nподвал. Мне туда не\n\n\nпройти.",
            .phrase_y = 0,
            .bubble_size = 18,
            .tricked = false,
            .item_to_trick = ITEM_NONE,
            .alt_action = false,
            .trick_making_length = 0,
            .trick_tv_rating = 0,
            .tricked_phrase_text = "",
            .tricked_phrase_y = 0,
            .tricked_bubble_size = 0,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = NULL,
            .on_untrick = NULL
        };
        look_objects[ROOM_ANC][0] = new_look_object2;
    }

    Sprite_MICROWAVE_COOK = g2d_LoadImage("assets_thq/sprites/kit/microwave/microwave_cook.png", G2D_RGBA8888);
    Sprite_DIRTYMICROWAVE = g2d_LoadImage("assets_thq/sprites/kit/microwave/dirtymicrowave.png", G2D_RGBA8888);

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
            .sprite_show = false,
            .above_door = false,
            .collision_x = 404,
            .collision_y = 102,
            .hint_text = "Посмотреть на микроволновку",
            .use_hint_text = "микроволновку",
            .tricked_hint_text = "Посмотреть на грязную микроволновку",
            .tricked_use_hint_text = "грязной микроволновке",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Микроволновка. Говорят,\n\n\nесли засунуть туда\n\n\nсырое яйцо, будет весело.",
            .phrase_y = 0,
            .bubble_size = 18,
            .tricked = false,
            .item_to_trick = ITEM_EGG,
            .alt_action = false,
            .trick_making_length = 108,
            .trick_tv_rating = 16,
            .tricked_phrase_text = "Кажется, яйцо уже\n\n\nспеклось",
            .tricked_phrase_y = 8,
            .tricked_bubble_size = 10,
            .on_making_trick = microwave_on_making_trick,
            .on_stop_making_trick = microwave_on_stop_making_trick,
            .on_trick = microwave_on_trick,
            .on_untrick = microwave_on_untrick
        };
        look_objects[ROOM_KIT][0] = new_look_object3;
    }

    Sprite_BINOCULARS_MS = g2d_LoadImage("assets_thq/sprites/kit/binoculars/binoculars_ms.png", G2D_CLUT8);
    Sprite_BINOCULARS_GLUE_MS = g2d_LoadImage("assets_thq/sprites/kit/binoculars/binoculars_glue_ms.png", G2D_CLUT8);

    LookObject* new_look_object4 = malloc(sizeof(LookObject));
    if (new_look_object4) {
        *new_look_object4 = (LookObject) {
            .spritelist = Sprite_BINOCULARS_MS,
            .sprite_x = 661,
            .sprite_y = 126,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 11,
            .sprite_h = 19,
            .sprite_show = true,
            .above_door = false,
            .collision_x = 599,
            .collision_y = 115,
            .hint_text = "Посмотреть на бинокль",
            .use_hint_text = "бинокль",
            .tricked_hint_text = "Посмотреть на самоклеющийся бинокль",
            .tricked_use_hint_text = "самоклеющемся бинокле",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "В этот бинокль\n\n\nсоседушка следит за\n\n\nтем, как раздевается\n\n\nдевушка в окне напротив!",
            .phrase_y = -6,
            .bubble_size = 22,
            .tricked = false,
            .item_to_trick = ITEM_SUPERGLUE,
            .alt_action = false,
            .trick_making_length = 108,
            .trick_tv_rating = 30,
            .tricked_phrase_text = "Самоклеющийся бинокль\n\n\n- контактные линзы\n\n\nбудущего.",
            .tricked_phrase_y = 2,
            .tricked_bubble_size = 14,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = binoculars_on_trick,
            .on_untrick = binoculars_on_untrick
        };
        look_objects[ROOM_KIT][1] = new_look_object4;
    }

    Sprite_SOFA_MS = g2d_LoadImage("assets_thq/sprites/lir/sofa/sofa_ms.png", G2D_CLUT8);

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
            .above_door = true,
            .collision_x = 161,
            .collision_y = 111,
            .hint_text = "Посмотреть на кресло",
            .use_hint_text = "кресло",
            .tricked_hint_text = "Посмотреть на кресло с подвохом",
            .tricked_use_hint_text = "кресле с подвохом",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Здесь он сидит и\n\n\nсмотрит телевизор.",
            .phrase_y = 8,
            .bubble_size = 10,
            .tricked = false,
            .item_to_trick = ITEM_FARTBAG,
            .alt_action = false,
            .trick_making_length = 54,
            .trick_tv_rating = 25,
            .tricked_phrase_text = "Вот он удивится, когда\n\n\nплюхнется в кресло!",
            .tricked_phrase_y = 8,
            .tricked_bubble_size = 10,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = NULL,
            .on_untrick = NULL
        };
        look_objects[ROOM_LIR][1] = new_look_object5;
    }

    Sprite_TWISTEDANTENNA = g2d_LoadImage("assets_thq/sprites/lir/tv/twistedantenna.png", G2D_RGBA8888);

    LookObject* new_look_object6 = malloc(sizeof(LookObject));
    if (new_look_object6) {
        *new_look_object6 = (LookObject) {
            .spritelist = Sprite_TWISTEDANTENNA,
            .sprite_x = 81,
            .sprite_y = 103,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 33,
            .sprite_h = 21,
            .sprite_show = false,
            .above_door = true,
            .collision_x = -128,
            .collision_y = 111,
            .hint_text = "Посмотреть на скрученную антенну",
            .use_hint_text = "антенну",
            .tricked_hint_text = "",
            .tricked_use_hint_text = "",
            .hint_id = get_unique_hint_id(),
            .phrase_text = "Теперь ловит даже\n\n\nкабельное!",
            .phrase_y = 8,
            .bubble_size = 10,
            .tricked = false,
            .item_to_trick = ITEM_NONE,
            .alt_action = false,
            .trick_making_length = 0,
            .trick_tv_rating = 0,
            .tricked_phrase_text = "",
            .tricked_phrase_y = 8,
            .tricked_bubble_size = 10,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = NULL,
            .on_untrick = NULL
        };
        look_objects[ROOM_LIR][2] = new_look_object6;
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
            .use_hint_text = "шкаф",
            .hint_id = get_unique_hint_id()
        };

        strcpy(new_hideout->hint_enter_text, json_get_item_string(parsed_current_hideout, "hint_enter_text", 255));
        strcpy(new_hideout->hint_exit_text, json_get_item_string(parsed_current_hideout, "hint_exit_text", 255));

        hideouts[room] = new_hideout;
    }

    // Хранилища
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_STORAGES_IN_ROOM; i++) {
            storages[room][i] = NULL;
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
            .use_hint_text = "комод",
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
            .use_hint_text = "аптечку",
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
            .use_hint_text = "холодильник",
            .hint_id = get_unique_hint_id(),
            .items = {ITEM_EGG, ITEM_NONE, ITEM_NONE, ITEM_NONE},
            .opened = false,
            .side = SIDE_UP_MID
        };
        storages[ROOM_KIT][0] = new_storage3;
    }

    // UseObjects
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_USE_OBJECTS_IN_ROOM; i++) {
            use_objects[room][i] = NULL;
        }
    }

    SpriteList_TV = g2d_LoadImage("assets_thq/sprites/lir/tv/tv.png", G2D_RGBA8888);

    tv_animation_frame = 0;
    tv_animation_length = 8;
    tv_animation_frame_time = 0;

    UseObject* new_use_object1 = malloc(sizeof(UseObject));
    if (new_use_object1) {
        *new_use_object1 = (UseObject) {
            .spritelist = SpriteList_TV,
            .sprite_x = 99,
            .sprite_y = 119,
            .sprite_src_x = 0,
            .sprite_src_y = 0,
            .sprite_w = 14,
            .sprite_h = 22,
            .sprite_show = true,
            .collision_x = 49,
            .collision_y = 111,
            .hint_text = "Использовать телевизионную антенну",
            .use_hint_text = "телевизионную антенну",
            .hint_id = get_unique_hint_id(),
            .tricked = false,
            .first_time = true,
            .trick_making_length = 54,
            .trick_tv_rating = 20,
            .on_making_trick = NULL,
            .on_stop_making_trick = NULL,
            .on_trick = tv_on_trick,
            .on_untrick = tv_on_untrick
        };
        use_objects[ROOM_LIR][0] = new_use_object1;
    }

    // LevelEnd
    level_end = level_end_create();

    level_end_active = false;

    // Вуди
    woody_spritelists[0] = SpriteList_WOODY_GENERIC;
    woody_spritelists[1] = SpriteList_DOORLEFT;
    woody_spritelists[2] = SpriteList_DOORRIGHT;
    woody_spritelists[3] = SpriteList_DOORBACK;
    woody_spritelists[4] = g2d_LoadImage("assets_thq/sprites/woody/generic2.png", G2D_CLUT8);
    woody_spritelists[5] = g2d_LoadImage("assets_thq/sprites/woody/generic3.png", G2D_CLUT8);

    woody_gfxdata[0] = woody_generic_frames;
    woody_gfxdata[1] = woody_doorleft_frames;
    woody_gfxdata[2] = woody_doorright_frames;
    woody_gfxdata[3] = woody_doorback_frames;
    woody_gfxdata[4] = woody_generic2_frames;
    woody_gfxdata[5] = woody_generic3_frames;
    
    woody_animations[0] = woody_generic_animations;
    woody_animations[1] = woody_doorleft_animations;
    woody_animations[2] = woody_doorright_animations;
    woody_animations[3] = woody_doorback_animations;
    woody_animations[4] = woody_generic2_animations;
    woody_animations[5] = woody_generic3_animations;

    woody = woody_create(
        woody_spritelists, // spritelists
        woody_gfxdata, // gfxdata
        woody_animations, // animations

        json_get_item_number(parsed_json, "woody_start_x"), json_get_item_number(parsed_json, "woody_start_y"), // start_x, start_y

        h_doors, v_doors, // h_doors, v_doors
        look_objects, // look_objects
        hideouts, // hideouts
        storages, // storages
        use_objects, // use_objects

        json_get_item_number(parsed_json, "camera_limit_x"), json_get_item_number(parsed_json, "camera_limit_y"), // camera_limit_x, camera_limit_y

        ROOM_FRO, // room
        room_collisions, // room_collisions

        json_get_item_number(parsed_json, "min_quota"), // min_quota
        json_get_item_number(parsed_json, "total_tricks"), // total_tricks

        NULL, // neighbour
        &neighbour_active, // neighbour_active
        level_end, // level_end
        &level_end_active // level_end_active
    );
    woody->neighbour_active = &neighbour_active;
    intro->woody = woody;

    neighbour_spritelists[0] = SpriteList_NEIGHBOUR_GENERIC;
    neighbour_spritelists[1] = SpriteList_DOORLEFT;
    neighbour_spritelists[2] = SpriteList_DOORRIGHT;
    // neighbour_spritelists[3] = SpriteList_DOORBACK;
    neighbour_spritelists[4] = g2d_LoadImage("assets_thq/sprites/lir/sofa/level_peep.png", G2D_CLUT8);
    neighbour_spritelists[5] = g2d_LoadImage("assets_thq/sprites/kit/binoculars/level_peep.png", G2D_CLUT8);
    neighbour_spritelists[6] = g2d_LoadImage("assets_thq/sprites/neighbour/generic2.png", G2D_CLUT8);
    neighbour_spritelists[7] = g2d_LoadImage("assets_thq/sprites/neighbour/generic3.png", G2D_CLUT8);
    neighbour_spritelists[8] = g2d_LoadImage("assets_thq/sprites/neighbour/look.png", G2D_CLUT8);
    neighbour_spritelists[9] = g2d_LoadImage("assets_thq/sprites/neighbour/shout0.png", G2D_CLUT8);
    neighbour_spritelists[10] = g2d_LoadImage("assets_thq/sprites/lir/sofa/level_peep1.png", G2D_CLUT8);
    neighbour_spritelists[11] = g2d_LoadImage("assets_thq/sprites/neighbour/gameover.png", G2D_CLUT8);

    neighbour_gfxdata[0] = neighbour_generic_frames;
    neighbour_gfxdata[1] = neighbour_doorleft_frames;
    neighbour_gfxdata[2] = neighbour_doorright_frames;
    // neighbour_gfxdata[3] = neighbour_doorback_frames;
    neighbour_gfxdata[4] = neighbour_sofa_frames;
    neighbour_gfxdata[5] = neighbour_binoculars_frames;
    neighbour_gfxdata[6] = neighbour_generic2_frames;
    neighbour_gfxdata[7] = neighbour_generic3_frames;
    neighbour_gfxdata[8] = neighbour_look_frames;
    neighbour_gfxdata[9] = neighbour_shout0_frames;
    neighbour_gfxdata[10] = neighbour_sofa1_frames;
    neighbour_gfxdata[11] = neighbour_gameover_frames;

    neighbour_animations[0] = neighbour_generic_animations;
    neighbour_animations[1] = neighbour_doorleft_animations;
    neighbour_animations[2] = neighbour_doorright_animations;
    // neighbour_animations[3] = neighbour_doorback_animations;
    neighbour_animations[4] = neighbour_sofa_animations;
    neighbour_animations[5] = neighbour_binoculars_animations;
    neighbour_animations[6] = neighbour_generic2_animations;
    neighbour_animations[7] = neighbour_generic3_animations;
    neighbour_animations[8] = neighbour_look_animations;
    neighbour_animations[9] = neighbour_shout0_animations;
    neighbour_animations[10] = neighbour_sofa1_animations;
    neighbour_animations[11] = neighbour_gameover_animations;

    neighbour = neighbour_create(
        neighbour_spritelists, // spritelists
        neighbour_gfxdata, // gfxdata
        neighbour_animations, // animations

        22, -56, // start_x, start_y
        ROOM_LIR, // start_room
        BUBBLE_SOFA, // start_bubble
        h_doors, v_doors, // h_doors, v_doors
        look_objects, // look_objects
        use_objects, // use_objects
        woody, // woody,
        &neighbour_active, // neighbour_active
        &level_end_active // level_end_active
    );
    woody->neighbour = neighbour;
    neighbour_active = false;

    logic_frame = 0;

    // Интерфейс
    pause_button_pressed = false;

    cJSON_Delete(parsed_json);

    NFHSoundPreload(SOUND_DOOR_CLOSE1);
    NFHSoundPreload(SOUND_DOOR_OPEN1);
    NFHSoundPreload(SOUND_ILLEGAL);
    NFHSoundPreload(SOUND_LEVELSTART);
    NFHSoundPreload(SOUND_WOD_JUHU1);
    NFHSoundPreload(SOUND_WOD_LAUGH1);
    NFHSoundPreload(SOUND_WOD_STEP1A);
    NFHSoundPreload(SOUND_WOD_STEP2A);
    NFHSoundPreload(SOUND_OBJ_OPEN1);
    NFHSoundPreload(SOUND_OBJ_CLOSE1);
    NFHSoundPreload(SOUND_WOD_HA1);
    NFHSoundPreload(SOUND_GIVE_TAKE1);
    NFHSoundPreload(SOUND_GIVE_TAKE2);
    NFHSoundPreload(SOUND_WOD_EH1);
    NFHSoundPreload(SOUND_NA_STEP1);
    NFHSoundPreload(SOUND_NA_STEP2);
    NFHSoundPreload(SOUND_NA_SITSDOWN1);
    NFHSoundPreload(SOUND_NA_REMOTE1);
    NFHSoundPreload(SOUND_NA_GETSUP1);
    NFHSoundPreload(SOUND_NA_PEEP1);
    NFHSoundPreload(SOUND_NA_PEEP2);
    NFHSoundPreload(SOUND_NA_PEEP3);
    NFHSoundPreload(SOUND_NA_PEEP4);
    NFHSoundPreload(SOUND_WOD_NONO);
    NFHSoundPreload(SOUND_WOD_LAUGH2);

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

    // Вот это всё надо будет в соседа засунуть. Сейчас он обновляется один раз в шесть кадров. А пусть он сам свои logic_frame
    // считает, а не уровень. Тогда будет куда деть весь код соседа ниже
    if (neighbour->ui_breakdowns_counter_animation_play) {
        neighbour->ui_breakdowns_counter_animation_frame_time++;
        if (neighbour->ui_breakdowns_counter_animation_frame_time == 10) {
            neighbour->ui_breakdowns_counter_animation_frame_time = 0;
            neighbour->ui_breakdowns_counter_animation_frame++;
            
            if (neighbour->ui_breakdowns_counter_animation_frame == 13) {
                neighbour->breakdowns++;
                neighbour->woody->tv_rating += 3;

                woody->ui_tv_rating_delta = 3.0f;
                strcpy(woody->ui_tv_rating_delta_text, "+3");
                woody->ui_tv_rating_delta_text_color = CG_ORANGE_BREAKDOWNS;

                sprintf(neighbour->ui_breakdowns_counter_text, "%d", neighbour->breakdowns);

                intraFontSetStyle(Font_BLUEHIGC_24, 0.583f, CG_ORANGE_BREAKDOWNS, 0, 0, INTRAFONT_ALIGN_LEFT);
                intraFontActivate(Font_BLUEHIGC_24, 1);
                neighbour->ui_breakdowns_counter_text_x = 459 - floor(intraFontMeasureText(Font_BLUEHIGC_24, neighbour->ui_breakdowns_counter_text) * 0.5);
                neighbour->ui_breakdowns_counter_text_show = false;
            }

            if (neighbour->ui_breakdowns_counter_animation_frame >= 14) {
                if (neighbour->ui_breakdowns_counter_animation_frame == 17) {
                    woody->ui_tv_rating_delta_text_show = true;
                    neighbour->ui_breakdowns_counter_text_show = true;
                    woody->ui_tv_rating_animation_play = true;
                    woody->ui_tv_rating_animation_frame = 0;
                    neighbour->ui_breakdowns_counter_animation_play = false;
                } else {
                    neighbour->ui_breakdowns_counter_text_show = !neighbour->ui_breakdowns_counter_text_show;
                    woody->ui_tv_rating_delta_text_show = !woody->ui_tv_rating_delta_text_show;
                }
            }
        }
    }

    if (neighbour->jingle_joke_playing) {
        neighbour->jingle_joke_timer--;
        if (neighbour->jingle_joke_timer == 0) {
            NFHHouseMusicResume();
        }
    }
    // ---

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
            case INTRO_WOODY_START_MOVE_TO_HOUSE: {
                woody->state = STATE_LEVEL_START;

                break;
            }

            case INTRO_NEIGHBOUR_CAN_MOVE: {
                neighbour_active = true;
                neighbour->bubble_show = true;

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

                break;
            }

            case INTRO_DELETE: {
                intro_active = false;
                    
                break;
            }
        }

        camera_x = clamp(camera_x + intro->camera_extra_x, 0, woody->camera_limit_x);
        camera_y = clamp(camera_y + intro->camera_extra_y, 0, woody->camera_limit_y);
    }

    // TV
    tv_animation_frame_time++;
    if (tv_animation_frame_time == 5) {
        tv_animation_frame++;
        if (tv_animation_frame == tv_animation_length) {
            tv_animation_frame = 0;
        }
        use_objects[ROOM_LIR][0]->sprite_src_x = tv_animation_frame * 14;
        tv_animation_frame_time = 0;
    }

    if (level_end_active) {
        level_end_update(level_end);
    }
        
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
        hDoor* current_door = h_doors[room][1]; // Правая дверь всегда под индексом 1

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

            if (!current_look_object) break; // Важно, чтобы look_objects по порядку ставили. А они будут по порядку заполнять массив при загрузке уровня
            
            if (current_look_object->above_door) continue;

            if (!current_look_object->spritelist) continue;

            if (!current_look_object->sprite_show) continue;

            if (current_look_object->sprite_x + current_look_object->sprite_w <= camera_x || 
                current_look_object->sprite_x >= camera_right ||
                current_look_object->sprite_y + current_look_object->sprite_h <= camera_y ||
                current_look_object->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_look_object->spritelist, current_look_object->sprite_x - camera_x, current_look_object->sprite_y - camera_y, current_look_object->sprite_w, current_look_object->sprite_h, WHITE, current_look_object->sprite_src_x, current_look_object->sprite_src_y, current_look_object->sprite_w, current_look_object->sprite_h, 0, 255, G2D_UP_LEFT);
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
        for (int i = 0; i < MAX_LOOK_OBJECTS_IN_ROOM; i++) {
            LookObject* current_look_object = look_objects[room][i];

            if (!current_look_object) break; // Важно, чтобы look_objects по порядку ставили. А они будут по порядку заполнять массив при загрузке уровня
            
            if (!current_look_object->above_door) continue;

            if (!current_look_object->spritelist) continue;

            if (!current_look_object->sprite_show) continue;

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

            if (!current_storage) break; // Хранилища идут подряд без пропусков
            
            if (!current_storage->spritelist) continue;

            if (!current_storage->sprite_show) continue;

            if (current_storage->sprite_x + current_storage->sprite_w <= camera_x || 
                current_storage->sprite_x >= camera_right ||
                current_storage->sprite_y + current_storage->sprite_h <= camera_y ||
                current_storage->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_storage->spritelist, current_storage->sprite_x - camera_x, current_storage->sprite_y - camera_y, current_storage->sprite_w, current_storage->sprite_h, WHITE, current_storage->sprite_src_x, current_storage->sprite_src_y, current_storage->sprite_w, current_storage->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    // use_objects
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_USE_OBJECTS_IN_ROOM; i++) {
            UseObject* current_use_object = use_objects[room][i];

            if (!current_use_object) break; // Хранилища идут подряд без пропусков
            
            if (!current_use_object->spritelist) continue;

            if (!current_use_object->sprite_show) continue;

            if (current_use_object->sprite_x + current_use_object->sprite_w <= camera_x || 
                current_use_object->sprite_x >= camera_right ||
                current_use_object->sprite_y + current_use_object->sprite_h <= camera_y ||
                current_use_object->sprite_y >= camera_bottom)
                continue;

            g2d_DrawImageExt(current_use_object->spritelist, current_use_object->sprite_x - camera_x, current_use_object->sprite_y - camera_y, current_use_object->sprite_w, current_use_object->sprite_h, WHITE, current_use_object->sprite_src_x, current_use_object->sprite_src_y, current_use_object->sprite_w, current_use_object->sprite_h, 0, 255, G2D_UP_LEFT);
        }
    }

    if (!woody->in_door && neighbour->game_over_state != STATE_LOSE_ANIMATION) {
        woody_draw(woody);
    }

    if (!neighbour->in_door) {
        neighbour_draw(neighbour);
    }

    // Отрисовка UI

    woody_draw_ui(woody);

    neighbour_draw_ui(neighbour);

    if (pause_button_pressed && !intro->draw_intro) {
        // Pause Button
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 450, 208, 20, 20, WHITE, 480, 0, 20, 20, 0, 255, G2D_UP_LEFT);
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

    g2d_FreeImage(Sprite_DIRTYMICROWAVE);
    g2d_FreeImage(Sprite_MICROWAVE_COOK);

    g2d_FreeImage(Sprite_BINOCULARS_MS);
    g2d_FreeImage(Sprite_BINOCULARS_GLUE_MS);

    g2d_FreeImage(Sprite_SOFA_MS);

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
    for (int i = 0; i < room_count; i++) {
        if (hideouts[i]) {
            free(hideouts[i]);
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

    // use_objects unload
    for (int room = 0; room < room_count; room++) {
        for (int i = 0; i < MAX_USE_OBJECTS_IN_ROOM; i++) {
            if (use_objects[room][i]) {
                free(use_objects[room][i]);
                use_objects[room][i] = NULL;
            }
        }
    }

    g2d_FreeImage(SpriteList_TV);
    g2d_FreeImage(Sprite_TWISTEDANTENNA);

    g2d_FreeImage(woody->spritelists[4]);
    g2d_FreeImage(woody->spritelists[5]);
    woody_unload(woody);

    g2d_FreeImage(neighbour_spritelists[4]);
    g2d_FreeImage(neighbour_spritelists[5]);
    g2d_FreeImage(neighbour_spritelists[6]);
    g2d_FreeImage(neighbour_spritelists[7]);
    g2d_FreeImage(neighbour_spritelists[8]);
    g2d_FreeImage(neighbour_spritelists[9]);
    g2d_FreeImage(neighbour_spritelists[10]);
    g2d_FreeImage(neighbour_spritelists[11]);
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
