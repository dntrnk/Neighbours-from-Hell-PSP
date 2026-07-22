#include "../../objects/scene_manager.h"

#include <math.h>

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"

#include "../../objects/localizations.h"

extern Scene DisclaimerScene;

extern g2dImage* Sprite_NFH_LOGO;

extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;

static char* language_names[] = {
    "English",
    "Русский"
};

static int siner;
static int selected;

static void init(void) {
    siner = 0;
    selected = 0;
}

static void update(void) {
    controls_read();

    siner++;
    if (siner >= 1440) {
        siner = 0;
    }

    if (controls_pressed(PSP_CTRL_UP)) {
        if (selected != 0) {
            selected--;
        }
    }

    if (controls_pressed(PSP_CTRL_DOWN)) {
        if (selected != 1) {
            selected++;
        }
    }

    if (controls_pressed(PSP_CTRL_CROSS)) {
        switch (selected) {
            case 0:
                lang_set(LANG_ENGLISH);
                Sprite_NFH_LOGO = g2d_LoadImage("assets_thq/sprites/ui/menu/logo_en.png", G2D_CLUT8);
                break;
            case 1:
                lang_set(LANG_RUSSIAN);
                Sprite_NFH_LOGO = g2d_LoadImage("assets_thq/sprites/ui/menu/logo_ru.png", G2D_CLUT8);
                break;
        }

        scene_change(&DisclaimerScene);
    }
}

static void draw(void) {
    g2d_Clear(BLACK);

    int text_y = 120;

    intraFontSetStyle(Font_BLUEHIGB_18, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);

    for (int i = 0; i < 2; i++) {
        intraFontPrint(Font_BLUEHIGB_18, 200, text_y + intraFontTextHeight(Font_BLUEHIGB_18), language_names[i]);
        text_y += 24;
    }

    intraFontPrint(Font_BLUEHIGB_18, 185 + sin(siner * 0.25) * 2, 130 + (selected * 24), ">");

    g2d_Flip(G2D_VSYNC);
}

Scene LanguageSelectScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = NULL
};
