#include "scene_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_SCENE_STACK 8

static Scene* scene_stack[MAX_SCENE_STACK];
static int scene_stack_count = 0;

static Scene* current_scene;

typedef enum {
    SCENE_PUSH,
    SCENE_POP,
    SCENE_CHANGE,
    SCENE_RESTART
} SceneRequestType; 

static bool scene_is_requesting = false;
static SceneRequestType scene_request;

static Scene* next_scene;

#include "../engine/controls/controls.h"
#include "../engine/graphics/g2d.h"   
#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"

char error_screen_text[1024];
extern intraFont* Font_BLUEHIGB_18;

int get_freeRam(void);

// костыль который надо будет убрать (мне прост как-то надо в меню паузы узнать, какой уровень запущен)
bool is_this_scene(const Scene* this_scene) {
    return (scene_stack[0] == this_scene);
}

void scene_push(Scene* new_scene) {
    scene_is_requesting = true;
    scene_request = SCENE_PUSH;
    next_scene = new_scene;
}

void scene_pop(void) {
    scene_is_requesting = true;
    scene_request = SCENE_POP;
}

void scene_change(Scene* new_scene) {
    scene_is_requesting = true;
    scene_request = SCENE_CHANGE;
    next_scene = new_scene;
}

void scene_restart(void) {
    scene_is_requesting = true;
    scene_request = SCENE_RESTART;
}

static void scene_push_process(void) {
    if (scene_stack_count >= MAX_SCENE_STACK) return;

    scene_stack_count++;
    scene_stack[scene_stack_count - 1] = next_scene;

    current_scene = scene_stack[scene_stack_count - 1];

    if (current_scene->init) {
        current_scene->init();
    }

    printf("Free RAM: %.1fMB\n", get_freeRam() / 1048576.0f);
}

static void scene_pop_process(void) {
    if (scene_stack_count <= 0) return;

    Scene* top = scene_stack[scene_stack_count - 1];
    if (top) {
        if (top->unload) {
            top->unload();
        }
    }
    scene_stack_count--;

    current_scene = scene_stack[scene_stack_count - 1];
}

static void scene_change_process(void) {
    while (scene_stack_count > 0) {
        scene_pop_process();
    }

    scene_push_process();
}

static void scene_restart_process(void) {
    while (scene_stack_count > 0) {
        scene_pop_process();
    }

    scene_stack_count = 1;

    current_scene = scene_stack[0];

    if (current_scene->init) {
        current_scene->init();
    }
}

void scene_update(void) {
    current_scene->update();
}

void scene_draw(void) {
    current_scene->draw();
}

inline void scene_handle_requests(void) {
    if (scene_is_requesting) {
        switch (scene_request) {
            case SCENE_PUSH: scene_push_process(); break;
            case SCENE_POP: scene_pop_process(); break;
            case SCENE_CHANGE: scene_change_process(); break;
            case SCENE_RESTART: scene_restart_process(); break;
        }
        scene_is_requesting = false;
    }
}

void scene_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(error_screen_text, sizeof(error_screen_text), format, args);
    va_end(args);

    NFHMusicStop();
    NFHHouseMusicStop();

    while (true) {
        controls_read();

        if (controls_pressed(PSP_CTRL_CROSS)) {
            sceKernelExitGame();
        }
            
        g2d_Clear(WHITE);

        intraFontSetStyle(Font_BLUEHIGB_18, 1, RED, 0, 0, INTRAFONT_ALIGN_LEFT);
        intraFontActivate(Font_BLUEHIGB_18, 0);
        intraFontPrint(Font_BLUEHIGB_18, 32, 32 + intraFontTextHeight(Font_BLUEHIGB_18), "ОШИБКА");
        intraFontPrint(Font_BLUEHIGB_18, 240, 216 + intraFontTextHeight(Font_BLUEHIGB_18), "Нажмите X чтобы\n\n\nвыйти в XMB");

        intraFontSetStyle(Font_BLUEHIGB_18, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
        intraFontActivate(Font_BLUEHIGB_18, 0);
        intraFontPrint(Font_BLUEHIGB_18, 32, 64 + intraFontTextHeight(Font_BLUEHIGB_18), error_screen_text);

        g2d_Flip(G2D_VSYNC);
    }
}
