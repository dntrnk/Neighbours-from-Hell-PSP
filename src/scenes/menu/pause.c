#include "../../objects/scene_manager.h"

#include <math.h>

#include "../../engine/controls/controls.h"
#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/NFHSound/NFHSound.h"

#define CG_MENU_GRAY G2D_RGB(199, 199, 199)
#define MENU_BLUE G2D_RGB(0, 96, 254)

#define BUTTON_DEFAULT 0
#define BUTTON_SELECTED 1
#define BUTTON_PRESSED 2

extern Scene MainMenuScene;
extern Scene NewGameMenuScene;
extern Scene Tutorial1Scene;

extern g2dImage* SpriteAtlas_MENU_SPRITES;

extern g2dImage* SpriteList_BUTTONS;

extern g2dImage* BG_NEW_GAME;
extern g2dImage* BG_INGAME_MENU;

extern g2dImage* Sprite_Punkt;

extern g2dImage* Icon_tutorial_1;
extern g2dImage* Icon_level_peep;

extern intraFont* Font_ACMESA;
extern intraFont* Font_BLUEHIGH_8;
extern intraFont* Font_BLUEHIGH_10;
extern intraFont* Font_BLUEHIGB_10;

static bool tutorial_selected;

static float new_scroll_y;
static float new_scroll_y_target;
static int scroll_y_limit;
static int scroll_y;

static int selected_button = 0;
static int clicked_button = 1;

static int menu_buttons[4] = {BUTTON_SELECTED, BUTTON_DEFAULT, BUTTON_DEFAULT, BUTTON_DEFAULT}; // Кнопка, выбранная посредством стрелочек
static g2dColor menu_buttons_colors[4] = {WHITE, CG_MENU_GRAY, CG_MENU_GRAY, CG_MENU_GRAY}; // Кнопка, на которой нажали X

/* ^^^ Что значит таблица menu_buttons ^^^
Это состояния кнопок. Цифры также означают и спрайт для отрисовки:
    0 - кнопка не выбрана
    1 - кнопка выбрана
    2 - кнопка нажата

Индексы:
    0 - Продолжить игру
    1 - Начать съемку заново
    2 - Снимать новую серию
    3 - Вернуться в главное меню
*/

static inline int clamp(int x, int min, int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

static inline float lerp(float a, float b, float t) {
    return b + (a - b) * t;
}

static void init(void) {
    NFHHouseMusicPause();
    NFHMusicPlay(MUSIC_TITEL, true);

    tutorial_selected = is_this_scene(&Tutorial1Scene);
    new_scroll_y = 0.0f;
    new_scroll_y_target = 0.0f;
    scroll_y = 0;
    scroll_y_limit = -10;

    selected_button = 0;
    clicked_button = 1;

    menu_buttons[0] = BUTTON_SELECTED;
    menu_buttons[1] = BUTTON_DEFAULT;
    menu_buttons[2] = BUTTON_DEFAULT;
    menu_buttons[3] = BUTTON_DEFAULT;

    menu_buttons_colors[0] = WHITE;
    menu_buttons_colors[1] = CG_MENU_GRAY;
    menu_buttons_colors[2] = CG_MENU_GRAY;
    menu_buttons_colors[3] = CG_MENU_GRAY;
}

static void pause_buttons_update(void) {
    // Сначала обнуляем все значения
    menu_buttons[0] = BUTTON_DEFAULT;
    menu_buttons[1] = BUTTON_DEFAULT;
    menu_buttons[2] = BUTTON_DEFAULT;
    menu_buttons[3] = BUTTON_DEFAULT;

    menu_buttons_colors[0] = CG_MENU_GRAY;
    menu_buttons_colors[1] = CG_MENU_GRAY;
    menu_buttons_colors[2] = CG_MENU_GRAY;
    menu_buttons_colors[3] = CG_MENU_GRAY;

    // А потом устанавливаем выбранной кнопке BUTTON_SELECTED (1) и белый цвет
    menu_buttons[selected_button] = BUTTON_SELECTED;
    menu_buttons_colors[selected_button] = WHITE;

    NFHSoundPlay(SOUND_BUT_HOVER1);
}

static void update(void) {
    controls_read();

    // Через стик можно будет "листать" описание
    int stick_y = controls_AnalogY();

    if (abs(stick_y) > 65) {
        new_scroll_y_target = clamp(new_scroll_y - clamp(stick_y, -10, 10), scroll_y_limit, 0);
    }

    if (abs(new_scroll_y - new_scroll_y_target) < 1) {
        new_scroll_y = new_scroll_y_target;
        scroll_y = floor(new_scroll_y);
    } else {
        new_scroll_y = lerp(new_scroll_y, new_scroll_y_target, 0.8f);
        scroll_y = floor(new_scroll_y);
    }

    // Управление кнопками через стрелки вверх/вниз
    if (controls_pressed(PSP_CTRL_UP)) {
        if (selected_button != 0) {
            selected_button--;
            
            pause_buttons_update();
        }
    } else if (controls_pressed(PSP_CTRL_DOWN)) {
        if (selected_button != 3) {
            selected_button++;

            pause_buttons_update();
        }
    }

    /*
        Нажатие кнопки с "залипанием" как в оригинальной игре
        Действие происходит если:
            Кнопку Х только что отпустили
            Положение кнопки не поменялось
    */

    if (controls_pressed(PSP_CTRL_CROSS)) {
        menu_buttons[selected_button] = 2;
        clicked_button = selected_button;
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CROSS) && menu_buttons[clicked_button] == BUTTON_PRESSED) {
        switch (clicked_button) {
            case 0: // Продолжить игру
                NFHHouseMusicResume();
                NFHMusicStop();
                scene_pop();

                break;
            case 1: // Начать съемку заново
                scene_restart();

                break;
            case 2: // Снимать новую серию
                scene_change(&MainMenuScene);
                scene_handle_requests();
                scene_push(&NewGameMenuScene);
            
                break;
            case 3: // Вернуться в главное меню
                scene_change(&MainMenuScene);
            
                break;
        }

        menu_buttons[clicked_button] = BUTTON_SELECTED;
    }

    // Выход из меню по кнопке O
    if (controls_pressed(PSP_CTRL_CIRCLE)) {
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CIRCLE)) {
        NFHHouseMusicResume();
        NFHMusicStop();
        scene_pop();
    }
}

static void draw(void) {
    // да, копипаста (с мелкими изменениями да да)
    g2d_Clear(BLACK);

    // Правая половина (копирка newgame)
    g2d_DrawImageExt(BG_NEW_GAME, 240, 129, 240, 80, WHITE, 240, 129, 240, 80, 0, 255, G2D_UP_LEFT);

    // Описание уровня
    intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 1);

    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGB_10, 253, 129 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Поначалу всегда тяжело. Если вы");
        intraFontPrint(Font_BLUEHIGB_10, 253, 137 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "прочитали руководство,");
        intraFontPrint(Font_BLUEHIGB_10, 253, 145 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "потренируйтесь с Вуди, пока не");
        intraFontPrint(Font_BLUEHIGB_10, 253, 153 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "прозвучала команда \'Мотор!\'");

        intraFontPrint(Font_BLUEHIGB_10, 253, 165 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Научитесь водить Вуди по дому,");
        intraFontPrint(Font_BLUEHIGB_10, 253, 173 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "путешествовать между комнатами");
        intraFontPrint(Font_BLUEHIGB_10, 253, 181 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "и этажами.");

        intraFontPrint(Font_BLUEHIGB_10, 253, 193 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Советуем пройти это вступление,");
        intraFontPrint(Font_BLUEHIGB_10, 253, 201 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "прежде чем переходить");
        intraFontPrint(Font_BLUEHIGB_10, 253, 209 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "к съемкам.");
    } else {
        intraFontPrint(Font_BLUEHIGB_10, 253, 129 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Как мы скоро увидим, наш сосед");
        intraFontPrint(Font_BLUEHIGB_10, 253, 137 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "тот еще тип.");

        intraFontPrint(Font_BLUEHIGB_10, 253, 149 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Редко застанешь его мирно");
        intraFontPrint(Font_BLUEHIGB_10, 253, 157 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "сидящим в кресле.");
        intraFontPrint(Font_BLUEHIGB_10, 253, 165 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Исключения только");
        intraFontPrint(Font_BLUEHIGB_10, 253, 173 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "подтверждают правило...");
        intraFontPrint(Font_BLUEHIGB_10, 253, 181 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Может, он просто затаился");
        intraFontPrint(Font_BLUEHIGB_10, 253, 189 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "и что то высматривает?");
        intraFontPrint(Font_BLUEHIGB_10, 275, 187 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, ".");
        intraFontPrint(Font_BLUEHIGB_10, 276, 187 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, ".");

        intraFontPrint(Font_BLUEHIGB_10, 253, 201 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "Вуди это не по душе. Придется");
        intraFontPrint(Font_BLUEHIGB_10, 253, 209 + intraFontTextHeight(Font_BLUEHIGB_10) + scroll_y, "навестить соседушку...");
    }

    g2d_DrawImageExt(BG_NEW_GAME, 240, 0, 240, 129, WHITE, 240, 0, 240, 129, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(BG_NEW_GAME, 240, 209, 240, 63, WHITE, 240, 209, 240, 63, 0, 255, G2D_UP_LEFT);

    // Подпись к иконке
    intraFontSetStyle(Font_BLUEHIGH_8, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGH_8, 0);

    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGH_8, 253, 120 + intraFontTextHeight(Font_BLUEHIGH_8), "Наш герой - Вуди!");
    } else {
        intraFontPrint(Font_BLUEHIGH_8, 253, 120 + intraFontTextHeight(Font_BLUEHIGH_8), "Любопытный сосед");
    }

    intraFontSetStyle(Font_ACMESA, 0.65f, WHITE, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 409, 7 + intraFontTextHeight(Font_ACMESA), "Описание серии");
    intraFontPrint(Font_ACMESA, 409, 7 + intraFontTextHeight(Font_ACMESA), "Описание серии");

    intraFontSetStyle(Font_ACMESA, 0.65f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 270, 217 + intraFontTextHeight(Font_ACMESA), "Рекорд серии:");
    intraFontPrint(Font_ACMESA, 270, 217 + intraFontTextHeight(Font_ACMESA), "Рекорд серии:");

    intraFontPrint(Font_ACMESA, 270, 227 + intraFontTextHeight(Font_ACMESA), "Пакостей");
    intraFontPrint(Font_ACMESA, 270, 227 + intraFontTextHeight(Font_ACMESA), "Пакостей");

    intraFontPrint(Font_ACMESA, 326, 227 + intraFontTextHeight(Font_ACMESA), "Оценка");
    intraFontPrint(Font_ACMESA, 326, 227 + intraFontTextHeight(Font_ACMESA), "Оценка");

    intraFontPrint(Font_ACMESA, 373, 222 + intraFontTextHeight(Font_ACMESA), "Назад");
    intraFontPrint(Font_ACMESA, 373, 222 + intraFontTextHeight(Font_ACMESA), "Назад");
    intraFontPrint(Font_ACMESA, 373, 229 + intraFontTextHeight(Font_ACMESA), "в игру");
    intraFontPrint(Font_ACMESA, 373, 229 + intraFontTextHeight(Font_ACMESA), "в игру");

    // Статистика уровня... ну пока что просто заглушка будет
    intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_BLUEHIGB_10, 1);

    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGB_10, 322, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0/1");
        intraFontPrint(Font_BLUEHIGB_10, 322, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0/1");
    } else {
        intraFontPrint(Font_BLUEHIGB_10, 322, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0/4");
        intraFontPrint(Font_BLUEHIGB_10, 322, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0/4");
    }
    
    intraFontPrint(Font_BLUEHIGB_10, 352, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0%");
    intraFontPrint(Font_BLUEHIGB_10, 352, 239 + intraFontTextHeight(Font_BLUEHIGB_10), "0%");

    if (tutorial_selected) {
        g2d_DrawImage(Sprite_Punkt, 255, 240, WHITE, 0, 255, G2D_UP_LEFT);
    } else {
        g2d_DrawImage(Sprite_Punkt, 255, 240, WHITE, 0, 255, G2D_UP_LEFT);
        g2d_DrawImage(Sprite_Punkt, 260, 240, WHITE, 0, 255, G2D_UP_LEFT);
        g2d_DrawImage(Sprite_Punkt, 265, 240, WHITE, 0, 255, G2D_UP_LEFT);
        g2d_DrawImage(Sprite_Punkt, 270, 240, WHITE, 0, 255, G2D_UP_LEFT);
    }

    // Квота
    intraFontSetStyle(Font_BLUEHIGH_10, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGH_10, 1);

    intraFontPrint(Font_BLUEHIGH_10, 329, 80 + intraFontTextHeight(Font_BLUEHIGH_10), "Оценка");
    intraFontPrint(Font_BLUEHIGH_10, 374, 80 + intraFontTextHeight(Font_BLUEHIGH_10), "Время");

    intraFontSetStyle(Font_BLUEHIGB_10, 1, WHITE, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_BLUEHIGB_10, 1);
    
    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGB_10, 362, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "100%");
        intraFontPrint(Font_BLUEHIGB_10, 362, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "100%");

        intraFontPrint(Font_BLUEHIGB_10, 407, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "0:00мин");
        intraFontPrint(Font_BLUEHIGB_10, 407, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "0:00мин");
    } else {
        intraFontPrint(Font_BLUEHIGB_10, 362, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "50%");
        intraFontPrint(Font_BLUEHIGB_10, 362, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "50%");

        intraFontPrint(Font_BLUEHIGB_10, 407, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "5:00мин");
        intraFontPrint(Font_BLUEHIGB_10, 407, 71 + intraFontTextHeight(Font_BLUEHIGB_10), "5:00мин");
    }

    // Подсказка
    intraFontSetStyle(Font_BLUEHIGB_10, 1, MENU_BLUE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 1);
    intraFontPrint(Font_BLUEHIGB_10, 341, 94 + intraFontTextHeight(Font_BLUEHIGB_10), "Подсказка!");

    intraFontSetStyle(Font_BLUEHIGH_8, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGH_8, 0);

    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGH_8, 335, 101 + intraFontTextHeight(Font_BLUEHIGH_8), "Слушайте советы");
        intraFontPrint(Font_BLUEHIGH_8, 339, 107 + intraFontTextHeight(Font_BLUEHIGH_8), "режиссера! Он");
        intraFontPrint(Font_BLUEHIGH_8, 332, 113 + intraFontTextHeight(Font_BLUEHIGH_8), "подскажет вам, что");
        intraFontPrint(Font_BLUEHIGH_8, 354, 119 + intraFontTextHeight(Font_BLUEHIGH_8), "делать.");
    } else {
        intraFontPrint(Font_BLUEHIGH_8, 330, 104 + intraFontTextHeight(Font_BLUEHIGH_8), "Некоторые пакости в");
        intraFontPrint(Font_BLUEHIGH_8, 340, 110 + intraFontTextHeight(Font_BLUEHIGH_8), "сериях можно");
        intraFontPrint(Font_BLUEHIGH_8, 345, 116 + intraFontTextHeight(Font_BLUEHIGH_8), "пропускать.");
    }

    g2d_DrawImageExt(SpriteList_BUTTONS, 386 + 9, 241 + 9, 23, 23, WHITE, 23, 0, 23, 23, 0, 255, G2D_UP_LEFT); // CIRCLE

    // Название уровня
    intraFontSetStyle(Font_ACMESA, 0.8f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 1);

    if (tutorial_selected) {
        intraFontPrint(Font_ACMESA, 255, 35 + intraFontTextHeight(Font_ACMESA), "Первые шаги");
        intraFontPrint(Font_ACMESA, 255, 35 + intraFontTextHeight(Font_ACMESA), "Первые шаги");
    } else {
        intraFontPrint(Font_ACMESA, 255, 29 + intraFontTextHeight(Font_ACMESA), "Сделал гадость - сердцу\n\n\n\nрадость");
        intraFontPrint(Font_ACMESA, 255, 29 + intraFontTextHeight(Font_ACMESA), "Сделал гадость - сердцу\n\n\n\nрадость");
    }

    if (tutorial_selected) {
        g2d_DrawImage(Icon_tutorial_1, 253, 55, WHITE, 0, 255, G2D_UP_LEFT);
    } else {
        g2d_DrawImage(Icon_level_peep, 253, 55, WHITE, 0, 255, G2D_UP_LEFT);
    }

    // Левая половина
    g2d_DrawImage(BG_INGAME_MENU, 0, 0, WHITE, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_ACMESA, 0.8f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 75, 35 + intraFontTextHeight(Font_ACMESA), "Игровое меню");
    intraFontPrint(Font_ACMESA, 75, 35 + intraFontTextHeight(Font_ACMESA), "Игровое меню");

    #define OFFSET_Y -53

    // Отрисовка кнопок, состояние кнопки совпадает со спрайтом (не выбран/выбран/нажат)
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 74, 77, 150, 24, WHITE, menu_buttons[0] * 150, 0, 150, 24, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 74, 102, 150, 24, WHITE, menu_buttons[1] * 150, 24, 150, 24, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 74, 127, 150, 24, WHITE, menu_buttons[2] * 150, 24, 150, 24, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 74, 152, 150, 24, WHITE, menu_buttons[3] * 150, 48, 150, 24, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_ACMESA, 0.8f, menu_buttons_colors[0], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 149, 137 + OFFSET_Y + intraFontTextHeight(Font_ACMESA), "Продолжить игру");

    intraFontSetStyle(Font_ACMESA, 0.8f, menu_buttons_colors[1], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_ACMESA, 149, 162 + OFFSET_Y + intraFontTextHeight(Font_ACMESA), "Начать съемку заново");

    intraFontSetStyle(Font_ACMESA, 0.8f, menu_buttons_colors[2], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_ACMESA, 149, 187 + OFFSET_Y + intraFontTextHeight(Font_ACMESA), "Снимать новую серию");

    intraFontSetStyle(Font_ACMESA, 0.8f, menu_buttons_colors[3], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_ACMESA, 149, 212 + OFFSET_Y + intraFontTextHeight(Font_ACMESA), "Вернуться в меню");

    g2d_Flip(G2D_VSYNC);
}

Scene PauseScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = NULL
};
