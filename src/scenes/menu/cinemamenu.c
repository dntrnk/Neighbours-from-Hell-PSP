#include "../../objects/scene_manager.h"

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"
#include "../../engine/NFHSound/NFHSound.h"

#include <stdbool.h>

extern Scene MainMenuScene;

extern g2dImage* BG_CINEMA;
extern g2dImage* Sprite_NFH_LOGO;

extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;
extern intraFont* Font_ACMESA;

static g2dImage* jwd;
static bool is_jwd;
static bool is_press_start;

static int i;
static int i2;
static int start_text_y;

static void init(void) {
    jwd = g2d_LoadImage("assets_thq/sprites/ui/menu/jowoodpresents.png", G2D_RGBA8888);
    
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
    intraFontPrint(Font_BLUEHIGB_10, 4, 262 + intraFontTextHeight(Font_BLUEHIGB_10), "Version 0.2.1 INDEV Nightly");

    if (is_jwd) {
        g2d_DrawImage(jwd, 168, 80, WHITE, 0, 255, G2D_UP_LEFT);
    } else {
        g2d_DrawImage(Sprite_NFH_LOGO, 181, 48, WHITE, 0, 255, G2D_UP_LEFT);

        intraFontSetStyle(Font_ACMESA, 1, BLACK, 0, 0, INTRAFONT_ALIGN_CENTER);
        intraFontActivate(Font_ACMESA, 0);
        intraFontPrint(Font_ACMESA, 240, 146 + intraFontTextHeight(Font_ACMESA), "сладкая месть");

        if (is_press_start) {
            intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_CENTER);
            intraFontActivate(Font_BLUEHIGB_10, 0);
            intraFontPrint(Font_BLUEHIGB_10, 240.5, start_text_y + intraFontTextHeight(Font_BLUEHIGB_10), "Нажмите Start, чтобы продолжить игру");

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
