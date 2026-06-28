#include "../../objects/scene_manager.h"

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"
#include "../../engine/NFHSound/NFHSound.h"

#include <stdbool.h>
#include <math.h>

#define CG_MENU_GRAY G2D_RGB(199, 199, 199)
#define MENU_BLUE G2D_RGB(0, 96, 254)

#define BUTTON_DEFAULT 0
#define BUTTON_SELECTED 1
#define BUTTON_PRESSED 2

extern Scene Tutorial1Scene;
extern Scene LevelPeepScene;

extern g2dImage* SpriteAtlas_MENU_SPRITES;
extern g2dImage* SpriteAtlas_INGAMEUI;

extern g2dImage* SpriteList_BUTTONS;

extern g2dImage* BG_CINEMA;
extern g2dImage* BG_NEW_GAME;

extern g2dImage* Sprite_NFH_LOGO;
extern g2dImage* Sprite_Punkt;

extern g2dImage* Icon_tutorial_1;
extern g2dImage* Icon_level_peep;

extern g2dImage* Button_tut1_normal;
extern g2dImage* Button_tut1_normal_pressed;
extern g2dImage* Button_peep_normal;
extern g2dImage* Button_peep_normal_pressed;

extern intraFont* Font_BLUEHIGH_8;
extern intraFont* Font_BLUEHIGH_10;
extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;
extern intraFont* Font_ACMESA;

// new_game_menu //
static bool tutorial_selected;

static float new_scroll_y;
static float new_scroll_y_target;
static int scroll_y_limit;
static int scroll_y;

static inline int clamp(int x, int min, int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

static inline float lerp(float a, float b, float t) {
    return b + (a - b) * t;
}

static void new_game_menu_init(void) {
    tutorial_selected = true;
    new_scroll_y = 0.0f;
    new_scroll_y_target = 0.0f;
    scroll_y = 0;
    scroll_y_limit = -10;
}

static void new_game_menu_update(void) {
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
        new_scroll_y = lerp(new_scroll_y, new_scroll_y_target, 0.8);
        scroll_y = floor(new_scroll_y);
    }

    // Выбор уровня посредством стрелочек
    if (controls_pressed(PSP_CTRL_UP)) {
        if (!tutorial_selected) {
            tutorial_selected = true;
            new_scroll_y = 0.0f;
            new_scroll_y_target = 0.0f;
            scroll_y = 0;
            scroll_y_limit = -8;
            NFHSoundPlay(SOUND_BUT_HOVER1);
        }
    } else if (controls_pressed(PSP_CTRL_DOWN)) {
        if (tutorial_selected) {
            tutorial_selected = false;
            new_scroll_y = 0.0f;
            new_scroll_y_target = 0.0f;
            scroll_y = 0;
            scroll_y_limit = -8;
            NFHSoundPlay(SOUND_BUT_HOVER1);
        }
    }

    // Выход из меню по кнопке O
    if (controls_pressed(PSP_CTRL_CIRCLE)) {
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CIRCLE)) {
        scene_pop();
    }

    // Заход в уровень по кнопке X
    if (controls_pressed(PSP_CTRL_CROSS)) {
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CROSS)) {
        if (tutorial_selected) {
            scene_change(&Tutorial1Scene);
        } else {
            scene_change(&LevelPeepScene);
        }
    }
}

static void new_game_menu_draw(void) {
    g2dClear(BLACK);

    g2d_DrawImageExt(BG_NEW_GAME, 0, 129, 480, 80, WHITE, 0, 129, 480, 80, 0, 255, G2D_UP_LEFT);

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

    g2d_DrawImageExt(BG_NEW_GAME, 0, 0, 480, 129, WHITE, 0, 0, 480, 129, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(BG_NEW_GAME, 0, 209, 480, 63, WHITE, 0, 209, 480, 63, 0, 255, G2D_UP_LEFT);

    // Подпись к иконке
    intraFontSetStyle(Font_BLUEHIGH_8, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGH_8, 0);

    if (tutorial_selected) {
        intraFontPrint(Font_BLUEHIGH_8, 253, 120 + intraFontTextHeight(Font_BLUEHIGH_8), "Наш герой - Вуди!");
    } else {
        intraFontPrint(Font_BLUEHIGH_8, 253, 120 + intraFontTextHeight(Font_BLUEHIGH_8), "Любопытный сосед");
    }

    intraFontSetStyle(Font_ACMESA, 0.65, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 94, 7 + intraFontTextHeight(Font_ACMESA), "Список серий");
    intraFontPrint(Font_ACMESA, 94, 7 + intraFontTextHeight(Font_ACMESA), "Список серий");

    intraFontSetStyle(Font_ACMESA, 0.65, WHITE, 0, 0, INTRAFONT_ALIGN_RIGHT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 409, 7 + intraFontTextHeight(Font_ACMESA), "Описание серии");
    intraFontPrint(Font_ACMESA, 409, 7 + intraFontTextHeight(Font_ACMESA), "Описание серии");

    intraFontSetStyle(Font_ACMESA, 0.65, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 79, 29 + intraFontTextHeight(Font_ACMESA), "Пилотные серии");
    intraFontPrint(Font_ACMESA, 79, 29 + intraFontTextHeight(Font_ACMESA), "Пилотные серии");

    intraFontPrint(Font_ACMESA, 79, 74 + intraFontTextHeight(Font_ACMESA), "Сезон 1");
    intraFontPrint(Font_ACMESA, 79, 74 + intraFontTextHeight(Font_ACMESA), "Сезон 1");

    intraFontPrint(Font_ACMESA, 79, 148 + intraFontTextHeight(Font_ACMESA), "Сезон 2");
    intraFontPrint(Font_ACMESA, 79, 148 + intraFontTextHeight(Font_ACMESA), "Сезон 2");

    intraFontPrint(Font_ACMESA, 79, 193 + intraFontTextHeight(Font_ACMESA), "Сезон 3");
    intraFontPrint(Font_ACMESA, 79, 193 + intraFontTextHeight(Font_ACMESA), "Сезон 3");

    intraFontPrint(Font_ACMESA, 101, 237 + intraFontTextHeight(Font_ACMESA), "Вернуться в главное меню");
    intraFontPrint(Font_ACMESA, 101, 237 + intraFontTextHeight(Font_ACMESA), "Вернуться в главное меню");

    intraFontPrint(Font_ACMESA, 270, 217 + intraFontTextHeight(Font_ACMESA), "Рекорд серии:");
    intraFontPrint(Font_ACMESA, 270, 217 + intraFontTextHeight(Font_ACMESA), "Рекорд серии:");

    intraFontPrint(Font_ACMESA, 270, 227 + intraFontTextHeight(Font_ACMESA), "Пакостей");
    intraFontPrint(Font_ACMESA, 270, 227 + intraFontTextHeight(Font_ACMESA), "Пакостей");

    intraFontPrint(Font_ACMESA, 326, 227 + intraFontTextHeight(Font_ACMESA), "Оценка");
    intraFontPrint(Font_ACMESA, 326, 227 + intraFontTextHeight(Font_ACMESA), "Оценка");

    intraFontPrint(Font_ACMESA, 370, 226 + intraFontTextHeight(Font_ACMESA), "Съемка");
    intraFontPrint(Font_ACMESA, 370, 226 + intraFontTextHeight(Font_ACMESA), "Съемка");

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
        intraFontPrint(Font_BLUEHIGH_8, 339, 101+6 + intraFontTextHeight(Font_BLUEHIGH_8), "режиссера! Он");
        intraFontPrint(Font_BLUEHIGH_8, 332, 101+6+6 + intraFontTextHeight(Font_BLUEHIGH_8), "подскажет вам, что");
        intraFontPrint(Font_BLUEHIGH_8, 354, 101+6+6+6 + intraFontTextHeight(Font_BLUEHIGH_8), "делать.");
    } else {
        intraFontPrint(Font_BLUEHIGH_8, 330, 104 + intraFontTextHeight(Font_BLUEHIGH_8), "Некоторые пакости в");
        intraFontPrint(Font_BLUEHIGH_8, 340, 104+6 + intraFontTextHeight(Font_BLUEHIGH_8), "сериях можно");
        intraFontPrint(Font_BLUEHIGH_8, 345, 104+6+6 + intraFontTextHeight(Font_BLUEHIGH_8), "пропускать.");
    }

    g2d_DrawImageExt(SpriteList_BUTTONS, 386 + 9, 241 + 9, 23, 23, WHITE, 0, 0, 23, 23, 0, 255, G2D_UP_LEFT); // CROSS
    g2d_DrawImageExt(SpriteList_BUTTONS, 71 + 9, 241 + 9, 23, 23, WHITE, 23, 0, 23, 23, 0, 255, G2D_UP_LEFT); // CIRCLE


    // Название уровня
    intraFontSetStyle(Font_ACMESA, 0.8, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
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

    if (tutorial_selected) {
        g2d_DrawImage(Button_tut1_normal_pressed, 71, 39, WHITE, 0, 255, G2D_UP_LEFT);
        g2d_DrawImage(Button_peep_normal, 71, 84, WHITE, 0, 255, G2D_UP_LEFT);
    } else {
        g2d_DrawImage(Button_tut1_normal, 71, 39, WHITE, 0, 255, G2D_UP_LEFT);
        g2d_DrawImage(Button_peep_normal_pressed, 71, 84, WHITE, 0, 255, G2D_UP_LEFT);
    }

    g2dFlip(G2D_VSYNC);
}

Scene NewGameMenuScene = {
    .init = new_game_menu_init,
    .update = new_game_menu_update,
    .draw = new_game_menu_draw,
    .unload = NULL
};

// authors_menu //

static void authors_menu_update(void) {
    controls_read();

    if (controls_pressed(PSP_CTRL_CIRCLE)) {
        scene_pop();
    }
}

static void authors_menu_draw(void) {
    g2dClear(BLACK);

    g2d_DrawImage(BG_CINEMA, 0, 0, WHITE, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_BLUEHIGB_18, 0.661f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);
    intraFontPrint(Font_BLUEHIGB_18, 62, 42 + intraFontTextHeight(Font_BLUEHIGB_18), "Оригинальная игра: JoWood Productions");
    intraFontPrint(Font_BLUEHIGB_18, 62, 56 + intraFontTextHeight(Font_BLUEHIGB_18), "Версию для PSP сделал: dntrnk");

    intraFontPrint(Font_BLUEHIGB_18, 326, 42 + intraFontTextHeight(Font_BLUEHIGB_18), "вернуться в меню");
    g2d_DrawImageExt(SpriteList_BUTTONS, 300, 35, 23, 23, WHITE, 23, 0, 23, 23, 0, 255, G2D_UP_LEFT); // CIRCLE

    intraFontSetStyle(Font_BLUEHIGB_10, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 0);
    intraFontPrint(Font_BLUEHIGB_10, 62, 188 + intraFontTextHeight(Font_BLUEHIGB_10), "t.me/pspdevlogdntrnk");

    g2dFlip(G2D_VSYNC);
}

Scene AuthorsMenuScene = {
    .init = NULL,
    .update = authors_menu_update,
    .draw = authors_menu_draw,
    .unload = NULL
};

// mainmenu //

typedef struct {
    int clicked_button;
    int selected_button;

    int menu_buttons[3];
    g2dColor menu_buttons_colors[3];
} MainMenuContext;

static MainMenuContext ctx;

static void mainmenu_init(void) {
    ctx.selected_button = 0; // Кнопка, выбранная посредством стрелочек
    ctx.clicked_button = 1; // Кнопка, на которой нажали X

    ctx.menu_buttons[0] = BUTTON_SELECTED;
    ctx.menu_buttons[1] = BUTTON_DEFAULT;
    ctx.menu_buttons[2] = BUTTON_DEFAULT;

    ctx.menu_buttons_colors[0] = WHITE;
    ctx.menu_buttons_colors[1] = CG_MENU_GRAY;
    ctx.menu_buttons_colors[2] = CG_MENU_GRAY;

    /* ^^^ Что значит таблица menu_buttons ^^^
    Это состояния кнопок. Цифры также означают и спрайт для отрисовки:
        0 - кнопка не выбрана
        1 - кнопка выбрана
        2 - кнопка нажата
    
    Индексы:
        0 - Играть
        1 - Авторы
        2 - Выход
    */

    NFHMusicPlay(MUSIC_TITEL, 1);
}

static void mainmenu_buttons_update(void) {
    // Сначала обнуляем все значения
    ctx.menu_buttons[0] = BUTTON_DEFAULT;
    ctx.menu_buttons[1] = BUTTON_DEFAULT;
    ctx.menu_buttons[2] = BUTTON_DEFAULT;

    ctx.menu_buttons_colors[0] = CG_MENU_GRAY;
    ctx.menu_buttons_colors[1] = CG_MENU_GRAY;
    ctx.menu_buttons_colors[2] = CG_MENU_GRAY;

    // А потом устанавливаем выбранной кнопке BUTTON_SELECTED (1) и белый цвет
    ctx.menu_buttons[ctx.selected_button] = BUTTON_SELECTED;
    ctx.menu_buttons_colors[ctx.selected_button] = WHITE;

    NFHSoundPlay(SOUND_BUT_HOVER1);
}

static void mainmenu_update(void) {
    controls_read();

    // Управление кнопками через стрелки вверх/вниз
    if (controls_pressed(PSP_CTRL_UP)) {
        if (ctx.selected_button != 0) {
            ctx.selected_button--;
            
            mainmenu_buttons_update();
        }
    } else if (controls_pressed(PSP_CTRL_DOWN)) {
        if (ctx.selected_button != 2) {
            ctx.selected_button++;

            mainmenu_buttons_update();
        }
    }

    /*
        Нажатие кнопки с "залипанием" как в оригинальной игре
        Действие происходит если:
            Кнопку Х только что отпустили
            Положение кнопки не поменялось
    */

    if (controls_pressed(PSP_CTRL_CROSS)) {
        ctx.menu_buttons[ctx.selected_button] = 2;
        ctx.clicked_button = ctx.selected_button;
        NFHSoundPlay(SOUND_BUT1);
    } else if (controls_released(PSP_CTRL_CROSS) && ctx.menu_buttons[ctx.clicked_button] == BUTTON_PRESSED) {
        switch (ctx.clicked_button) {
            case 0: // Играть
                scene_push(&NewGameMenuScene);

                break;
            case 1: // Авторы
                scene_push(&AuthorsMenuScene);

                break;
            case 2: // Выход
                sceKernelExitGame();
            
                break;
        }

        ctx.menu_buttons[ctx.clicked_button] = BUTTON_SELECTED;
    }
}

static void mainmenu_draw(void) {
    g2dClear(BLACK);

    // BG
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 0, 0, 480, 272, WHITE, 0, 72, 480, 272, 0, 255, G2D_UP_LEFT);

    // Logo
    g2d_DrawImage(Sprite_NFH_LOGO, 186, 7, WHITE, 0, 255, G2D_UP_LEFT);

    // Отрисовка кнопок, состояние кнопки совпадает со спрайтом (не выбран/выбран/нажат)
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 173, 130, 150, 24, WHITE, ctx.menu_buttons[0] * 150, 0, 150, 24, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 173, 155, 150, 24, WHITE, ctx.menu_buttons[1] * 150, 24, 150, 24, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageExt(SpriteAtlas_MENU_SPRITES, 173, 180, 150, 24, WHITE, ctx.menu_buttons[2] * 150, 48, 150, 24, 0, 255, G2D_UP_LEFT);

    intraFontSetStyle(Font_ACMESA, 0.8, ctx.menu_buttons_colors[0], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontActivate(Font_ACMESA, 1);
    intraFontPrint(Font_ACMESA, 248, 137 + intraFontTextHeight(Font_ACMESA), "Начать игру");

    intraFontSetStyle(Font_ACMESA, 0.8, ctx.menu_buttons_colors[1], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_ACMESA, 248, 162 + intraFontTextHeight(Font_ACMESA), "Авторы");

    intraFontSetStyle(Font_ACMESA, 0.8, ctx.menu_buttons_colors[2], 0, 0, INTRAFONT_ALIGN_CENTER);
    intraFontPrint(Font_ACMESA, 248, 187 + intraFontTextHeight(Font_ACMESA), "Выход");

    g2dFlip(G2D_VSYNC);
}

Scene MainMenuScene = {
    .init = mainmenu_init,
    .update = mainmenu_update,
    .draw = mainmenu_draw,
    .unload = NULL
};
