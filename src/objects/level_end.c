#include "level_end.h"

#include <stdlib.h>
#include <string.h>

#include "scene_manager.h"

#include "../engine/controls/controls.h"
#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"

#define LEVEL_END_GREEN G2D_RGB(32, 240, 0)
#define LEVEL_END_YELLOW G2D_RGB(239, 239, 0)

extern Scene MainMenuScene;
extern Scene NewGameMenuScene;

extern g2dImage* SpriteAtlas_INGAMEUI;

extern intraFont* Font_BLUEHIGC_24;
extern intraFont* Font_BLUEHIGB_18;
extern intraFont* Font_ACMESA;
extern intraFont* Font_ACMESA_17_9;

LevelEnd* level_end_create(void) {
    LevelEnd* level_end = malloc(sizeof(LevelEnd));
    memset(level_end, 0, sizeof(LevelEnd));

    level_end->counter = 117;

    strcpy(level_end->end_text, "Отлично!");

    level_end->selected_button = BUTTON_OK;
    level_end->clicked_button = BUTTON_RESTART;

    level_end->buttons_colors[0] = LEVEL_END_GREEN;
    level_end->buttons_colors[1] = LEVEL_END_YELLOW;

    strcpy(level_end->tricks_text, "");
    strcpy(level_end->tv_rating_text, "");

    return level_end;
}

static void update_buttons(LevelEnd* level_end) {
    level_end->buttons_colors[BUTTON_RESTART] = (level_end->selected_button == BUTTON_RESTART) ? LEVEL_END_YELLOW : LEVEL_END_GREEN; // Кнопка Переснять жёлтая, если выбрана, иначе зелёная
    level_end->buttons_colors[BUTTON_OK] = (level_end->selected_button == BUTTON_OK) ? LEVEL_END_YELLOW : LEVEL_END_GREEN; // Кнопка Ок жёлтая, если выбрана, иначе зелёная

    NFHSoundPlay(SOUND_BUT_HOVER1);
}

void level_end_update(LevelEnd* level_end) {
    if (level_end->counter != 0) {
        level_end->counter--;
        return;
    }

    if (controls_pressed(PSP_CTRL_LEFT)) {
        if (level_end->selected_button != BUTTON_RESTART) {
            level_end->selected_button = BUTTON_RESTART;
            update_buttons(level_end);
        }
    } else if (controls_pressed(PSP_CTRL_RIGHT)) {
        if (level_end->selected_button != BUTTON_OK) {
            level_end->selected_button = BUTTON_OK;
            update_buttons(level_end);
        }
    }

    /*
        Нажатие кнопки с "залипанием" как в оригинальной игре
        Действие происходит если:
            Кнопку Х только что отпустили
            Положение кнопки не поменялось
    */

    if (controls_pressed(PSP_CTRL_CROSS)) {
        level_end->clicked_button = level_end->selected_button;
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CROSS) && level_end->selected_button == level_end->clicked_button) {
        if (level_end->selected_button == BUTTON_RESTART) {
            scene_restart();
        } else {
            scene_change(&MainMenuScene);
            scene_handle_requests();
            scene_push(&NewGameMenuScene);
        }
    }
}

void level_end_draw(const LevelEnd* level_end) {
    if (level_end->counter != 0) return;

    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 103, 18, 275, 198, WHITE, 0, 122, 275, 198, 0, 240, G2D_UP_LEFT); // TV BG

    intraFontSetStyle(Font_ACMESA_17_9, 0.73, LEVEL_END_GREEN, 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontActivate(Font_ACMESA_17_9, 0);
    intraFontPrint(Font_ACMESA_17_9, 273, 80 + intraFontTextHeight(Font_ACMESA_17_9), level_end->end_text);

    // Статистика
    intraFontSetStyle(Font_ACMESA, 0.65, LEVEL_END_GREEN, 0, 0, INTRAFONT_ALIGN_LEFT); // Потом на ALIGN_CENTER сменить надо бы
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 253, 112 + intraFontTextHeight(Font_ACMESA), "Пакостей:");
    intraFontPrint(Font_ACMESA, 230, 142 + intraFontTextHeight(Font_ACMESA), "Зрительская оценка:");

    // Пакости
    // Тут я сделал искусственный ALIGN_CENTER, потому что при настоящем "ломался" (не было видно некоторых линий у букв), так как позиция была скорее всего не int-числом.
    intraFontSetStyle(Font_BLUEHIGC_24, 0.9, LEVEL_END_GREEN, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGC_24, 0);
    intraFontPrint(Font_BLUEHIGC_24, 274 - (int) ((intraFontMeasureText(Font_BLUEHIGC_24, level_end->tricks_text) * 0.5) + 0.5), 122 + intraFontTextHeight(Font_BLUEHIGC_24), level_end->tricks_text);

    // Квота
    // При ALIGN_RIGHT текст выглядит "ломаным", поэтому я сделал отрисовку через ALIGN_CENTER, но с вычислением X, как будто это ALIGN_RIGHT (мда)
    intraFontSetStyle(Font_BLUEHIGC_24, 0.926, LEVEL_END_GREEN, 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_BLUEHIGC_24, 279 - (int) ((intraFontMeasureText(Font_BLUEHIGC_24, level_end->tv_rating_text) * 0.5) + 0.5), 153 + intraFontTextHeight(Font_BLUEHIGC_24), level_end->tv_rating_text);

    intraFontSetStyle(Font_BLUEHIGB_18, 0.975, LEVEL_END_GREEN, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 0);
    intraFontPrint(Font_BLUEHIGB_18, 278, 156 + intraFontTextHeight(Font_BLUEHIGB_18), "%");

    // Кнопки
    // Переснять
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 198, 173, 68, 12, level_end->buttons_colors[BUTTON_RESTART], 185, 80, 68, 12, 0, 255, G2D_UP_LEFT);
        
    intraFontSetStyle(Font_ACMESA, 0.65, level_end->buttons_colors[BUTTON_RESTART], 0, 0, INTRAFONT_ALIGN_CENTER); // Потом на ALIGN_CENTER сменить надо бы
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 232, 175 + intraFontTextHeight(Font_ACMESA), "Переснять");

    // Ок
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 274, 173, 68, 12, level_end->buttons_colors[BUTTON_OK], 185, 80, 68, 12, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_ACMESA, 0.65, level_end->buttons_colors[BUTTON_OK], 0, 0, INTRAFONT_ALIGN_CENTER); // Потом на ALIGN_CENTER сменить надо бы тоже
    intraFontPrint(Font_ACMESA, 308, 175 + intraFontTextHeight(Font_ACMESA), "Ок");
}

void level_end_unload(LevelEnd* level_end) {
    free(level_end);
}
