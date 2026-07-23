#include "../../objects/scene_manager.h"

#include <stdbool.h>

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"
#include "../../engine/NFHSound/NFHSound.h"

#include "../../objects/localizations.h"

extern Scene MainMenuScene;

extern g2dImage* BG_CINEMA;
extern g2dImage* Sprite_NFH_LOGO;

extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;
extern intraFont* Font_ACMESA;

static g2dImage* jwd;

static int game_logo_x;
static int game_logo_y;

static int presents_text_x;
static int press_start_text_x;

static bool is_jwd;
static bool is_press_start;

static int i;
static int i2;
static int start_text_y;

static char presents_text[64];
static char game_subtitle_text[64];
static char press_start_text[64];

static void init(void) {
    char loc_filename[512];
    sprintf(loc_filename, "data/localizations/%s/cinemamenu.json", current_lang_code);

    FILE* loc_file = fopen(loc_filename, "r");

    if (!loc_file) {
        scene_error("Не удалось открыть файл %s", loc_filename);
    }

    fseek(loc_file, 0, SEEK_END);
    long loc_size = ftell(loc_file);
    fseek(loc_file, 0, SEEK_SET);
    char* loc_json = malloc(loc_size + 1);
    fread(loc_json, 1, loc_size, loc_file);
    loc_json[loc_size] = '\0';
    fclose(loc_file);
    cJSON* parsed_loc_json = cJSON_Parse(loc_json);
    free(loc_json);

    strcpy(game_subtitle_text, json_get_item_string(parsed_loc_json, "game_subtitle", 63));
    strcpy(presents_text, json_get_item_string(parsed_loc_json, "presents", 63));
    strcpy(press_start_text, json_get_item_string(parsed_loc_json, "press_start", 63));

    cJSON_Delete(parsed_loc_json);

    jwd = g2d_LoadImage("assets_thq/sprites/ui/menu/jowoodpresents.png", G2D_RGBA8888);
    
    switch (current_lang) {
        case LANG_RUSSIAN: game_logo_x = 181; game_logo_y = 48; break;
        case LANG_ENGLISH: game_logo_x = 159; game_logo_y = 58; break;
    }

    intraFontSetStyle(Font_ACMESA, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 0);
    presents_text_x = 240 - (intraFontMeasureText(Font_ACMESA, presents_text) * 0.5);

    intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 0);
    press_start_text_x = 240 - (intraFontMeasureText(Font_BLUEHIGB_10, press_start_text) * 0.5);

    is_jwd = true; // Показывать ли "JoWood представляет"
    is_press_start = false; // Показывать ли "Нажмите Start"

    i = 0; // Таймер показа "JoWood представляет"
    i2 = 0; // Таймер показа "Нажмите Start"
    start_text_y = 180;

    NFHMusicPlay(MUSIC_TITEL, true);
}

static void update(void) {
    controls_read();

    // Увеличение таймера если "JoWood представляет" показывается
    if (is_jwd) {
        if (i != 170) { // На 170 кадре отключаем это
            i++;
        } else {
            is_jwd = false;
        }
    } else if (!is_press_start) {
        if (i2 != 84) {
            i2++;
        } else {
            is_press_start = true;
        }
    } else if (start_text_y != 170) {
        start_text_y -= 2;
    }

    if (controls_pressed(PSP_CTRL_CROSS) || controls_pressed(PSP_CTRL_START))
        scene_change(&MainMenuScene);
}

static void draw(void) {
    g2d_Clear(BLACK);

    g2d_DrawImage(BG_CINEMA, 0, 0, WHITE, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_BLUEHIGB_10, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 0);
    intraFontPrint(Font_BLUEHIGB_10, 4, 262 + intraFontTextHeight(Font_BLUEHIGB_10), "Version 0.2.2 INDEV Nightly");

    if (is_jwd) {
        g2d_DrawImage(jwd, 168, 80, WHITE, 0, 255, G2D_UP_LEFT);

        intraFontSetStyle(Font_ACMESA, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
        intraFontActivate(Font_ACMESA, 0);
        intraFontPrint(Font_ACMESA, presents_text_x, 137 + intraFontTextHeight(Font_ACMESA), presents_text);
    } else {
        g2d_DrawImage(Sprite_NFH_LOGO, game_logo_x, game_logo_y, WHITE, 0, 255, G2D_UP_LEFT);

        intraFontSetStyle(Font_ACMESA, 1, BLACK, 0, 0, INTRAFONT_ALIGN_CENTER);
        intraFontActivate(Font_ACMESA, 0);
        intraFontPrint(Font_ACMESA, 240, 146 + intraFontTextHeight(Font_ACMESA), game_subtitle_text);

        if (is_press_start) {
            intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_BLUEHIGB_10, 0);
            intraFontPrint(Font_BLUEHIGB_10, press_start_text_x, start_text_y + intraFontTextHeight(Font_BLUEHIGB_10), press_start_text);

            g2d_DrawRectFilled(100, 178, 240, 21, WHITE, 255);
        }
    }

    g2d_Flip(G2D_VSYNC);
}

static void unload(void) {
    g2d_FreeImage(jwd);
}

Scene CinemaMenuScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = unload
};
