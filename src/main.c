#include <pspkernel.h>
#include <pspdisplay.h>
#include <psppower.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "engine/graphics/g2d.h"
#include "engine/fonts/intraFont.h"
#include "engine/controls/controls.h"
#include "engine/audio/pspaalib.h"
#include "engine/NFHSound/NFHSound.h"

#include "objects/scene_manager.h"

PSP_MODULE_INFO("Neighbours from Hell", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

char list[0x20000] __attribute__((aligned(64)));

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

g2dImage* SpriteAtlas_MENU_SPRITES = NULL;
g2dImage* SpriteAtlas_INTRO = NULL;
g2dImage* SpriteAtlas_INGAMEUI = NULL;
g2dImage* SpriteAtlas_STORAGES = NULL;

g2dImage* BG_CINEMA = NULL;
g2dImage* BG_NEW_GAME = NULL;
g2dImage* BG_INGAME_MENU = NULL;

g2dImage* SpriteList_WOODY_GENERIC = NULL;

g2dImage* SpriteList_NEIGHBOUR_GENERIC = NULL;

g2dImage* SpriteList_DOORLEFT = NULL;
g2dImage* SpriteList_DOORRIGHT = NULL;
g2dImage* SpriteList_DOORBACK = NULL;

g2dImage* SpriteList_BUBBLES = NULL;
g2dImage* SpriteList_BUTTONS = NULL;
g2dImage* SpriteList_ITEMS_1 = NULL;
g2dImage* SpriteList_ITEMS_2 = NULL;

g2dImage* Sprite_NFH_LOGO = NULL;
g2dImage* Sprite_LOADING_TEXT_DATA = NULL;
g2dImage* Sprite_Punkt = NULL;

g2dImage* Icon_tutorial_1 = NULL;
g2dImage* Icon_level_peep = NULL;

g2dImage* Button_tut1_normal = NULL;
g2dImage* Button_tut1_normal_pressed = NULL;
g2dImage* Button_peep_normal = NULL;
g2dImage* Button_peep_normal_pressed = NULL;

intraFont* Font_BLUEHIGC_11 = NULL;
intraFont* Font_BLUEHIGC_24 = NULL;
intraFont* Font_BLUEHIGH_8 = NULL;
intraFont* Font_BLUEHIGH_10 = NULL;
intraFont* Font_BLUEHIGB_10 = NULL;
intraFont* Font_BLUEHIGB_18 = NULL;
intraFont* Font_ACMESA = NULL;
intraFont* Font_ACMESA_17_9 = NULL;
intraFont* Font_ACMESAI_13 = NULL;

extern Scene MadeByDntrnkScene;

int camera_x = 0;
int camera_y = 0;
int camera_right = 0;
int camera_bottom = 0;

int get_freeRam(void) {
    void* buf[128];
    int i, j;

    for (i = 0; i < 128; i++) {
        buf[i] = malloc(512 * 1024);
        if (!buf[i])
            break;
    }
    
    int result = i;

    for (j = result - 1; j >= 0; j--) {
        free(buf[j]);
    }

    return (result * 512 * 1024);
}

int main(void) {
    setup_callbacks();

    srand(time(NULL));

    g2dInit();
    intraFontInit();
    controls_init();
    AalibInit();

    SpriteAtlas_MENU_SPRITES = g2d_LoadImage("assets/sprites/ui/menu/menusprites.png", G2D_RGBA8888);
    SpriteAtlas_INTRO = g2d_LoadImage("assets/sprites/ui/menu/intro.png", G2D_RGBA8888);
    SpriteAtlas_INGAMEUI = g2d_LoadImage("assets/sprites/ui/ingame/ingameui.png", G2D_CLUT8); 
    SpriteAtlas_STORAGES = g2d_LoadImage("assets/sprites/storages/storages.png", G2D_RGBA8888);

    BG_CINEMA = g2d_LoadImage("assets/sprites/ui/menu/credits.png", G2D_CLUT8);
    BG_NEW_GAME = g2d_LoadImage("assets/sprites/ui/menu/newgame.png", G2D_RGBA8888);
    BG_INGAME_MENU = g2d_LoadImage("assets/sprites/ui/menu/ingamemenu.png", G2D_RGBA8888);

    SpriteList_WOODY_GENERIC = g2d_LoadImage("assets/sprites/woody/generic.png", G2D_CLUT8); 

    SpriteList_NEIGHBOUR_GENERIC = g2d_LoadImage("assets/sprites/neighbour/generic.png", G2D_CLUT8); 

    SpriteList_DOORLEFT = g2d_LoadImage("assets/sprites/doorleft/generic.png", G2D_CLUT8); 
    SpriteList_DOORRIGHT = g2d_LoadImage("assets/sprites/doorright/generic.png", G2D_CLUT8); 
    SpriteList_DOORBACK = g2d_LoadImage("assets/sprites/doorback/generic.png", G2D_CLUT8); 

    SpriteList_BUBBLES = g2d_LoadImage("assets/sprites/ui/ingame/bubbles.png", G2D_CLUT8); 
    SpriteList_BUTTONS = g2d_LoadImage("assets/sprites/ui/ingame/buttons.png", G2D_CLUT8); 
    SpriteList_ITEMS_1 = g2d_LoadImage("assets/sprites/ui/ingame/items1.png", G2D_CLUT8); 
    SpriteList_ITEMS_2 = g2d_LoadImage("assets/sprites/ui/ingame/items2.png", G2D_CLUT8); 

    Sprite_NFH_LOGO = g2d_LoadImage("assets/sprites/ui/menu/logo.png", G2D_CLUT8);
    Sprite_LOADING_TEXT_DATA = g2d_LoadImage("assets/sprites/ui/menu/loadingTextData.png", G2D_CLUT4);
    Sprite_Punkt = g2d_LoadImage("assets/sprites/ui/menu/punkt.png", G2D_RGBA8888);

    Icon_tutorial_1 = g2d_LoadImage("assets/sprites/ui/menu/tutorial_1.png", G2D_CLUT8);
    Icon_level_peep = g2d_LoadImage("assets/sprites/ui/menu/level_peep.png", G2D_CLUT8);

    Button_tut1_normal = g2d_LoadImage("assets/sprites/ui/menu/button_tut1_normal.png", G2D_CLUT8);
    Button_tut1_normal_pressed = g2d_LoadImage("assets/sprites/ui/menu/button_tut1_normal_pressed.png", G2D_CLUT8);
    Button_peep_normal = g2d_LoadImage("assets/sprites/ui/menu/button_peep_normal.png", G2D_CLUT8);
    Button_peep_normal_pressed = g2d_LoadImage("assets/sprites/ui/menu/button_peep_normal_pressed.png", G2D_CLUT8);

    Font_BLUEHIGC_11 = intraFontLoadTTF("assets/fonts/bluehigc.ttf", INTRAFONT_STRING_UTF8, 11);
    Font_BLUEHIGC_24 = intraFontLoadTTF("assets/fonts/bluehigc.ttf", INTRAFONT_STRING_UTF8, 24);
    Font_BLUEHIGH_8 = intraFontLoadTTF("assets/fonts/bluehigh.ttf", INTRAFONT_STRING_UTF8, 8);
    Font_BLUEHIGH_10 = intraFontLoadTTF("assets/fonts/bluehigh.ttf", INTRAFONT_STRING_UTF8, 10);
    Font_BLUEHIGB_10 = intraFontLoadTTF("assets/fonts/bluehigb.ttf", INTRAFONT_STRING_UTF8, 10);
    Font_BLUEHIGB_18 = intraFontLoadTTF("assets/fonts/bluehigb.ttf", INTRAFONT_STRING_UTF8, 18);
    Font_ACMESA = intraFontLoadTTF("assets/fonts/acmesa.ttf", INTRAFONT_STRING_UTF8, 11.f);
    Font_ACMESA_17_9 = intraFontLoadTTF("assets/fonts/acmesa.ttf", INTRAFONT_STRING_UTF8, 17.9);
    Font_ACMESAI_13 = intraFontLoadTTF("assets/fonts/acmesai.ttf", INTRAFONT_STRING_UTF8, 13);

    NFHSoundPreload(SOUND_BUT1);
    NFHSoundPreload(SOUND_BUT_HOVER1);
    NFHSoundPreload(SOUND_DOOR_CLOSE1);
    NFHSoundPreload(SOUND_DOOR_OPEN1);
    NFHSoundPreload(SOUND_ILLEGAL);
    NFHSoundPreload(SOUND_LEVEL_START);
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
    NFHSoundPreload(SOUND_NA_USE1A);
    NFHSoundPreload(SOUND_NA_STEP1);
    NFHSoundPreload(SOUND_NA_STEP2);
    NFHSoundPreload(SOUND_NA_SITSDOWN1);
    NFHSoundPreload(SOUND_NA_REMOTE1);
    NFHSoundPreload(SOUND_NA_GETSUP1);

    scene_change(&MadeByDntrnkScene);
    scene_handle_requests();

    while (true) {
        scene_update();
        scene_handle_requests();
        scene_draw();
    }

    g2d_FreeImage(SpriteAtlas_MENU_SPRITES);
    g2d_FreeImage(SpriteAtlas_INTRO);
    g2d_FreeImage(SpriteAtlas_INGAMEUI);
    g2d_FreeImage(SpriteAtlas_STORAGES);

    g2d_FreeImage(BG_CINEMA);
    g2d_FreeImage(BG_NEW_GAME);
    g2d_FreeImage(BG_INGAME_MENU);

    g2d_FreeImage(SpriteList_WOODY_GENERIC);

    g2d_FreeImage(SpriteList_NEIGHBOUR_GENERIC);

    g2d_FreeImage(SpriteList_DOORLEFT);
    g2d_FreeImage(SpriteList_DOORRIGHT);
    g2d_FreeImage(SpriteList_DOORBACK);

    g2d_FreeImage(SpriteList_BUBBLES);
    g2d_FreeImage(SpriteList_BUTTONS);
    g2d_FreeImage(SpriteList_ITEMS_1);
    g2d_FreeImage(SpriteList_ITEMS_2);

    g2d_FreeImage(Sprite_NFH_LOGO);
    g2d_FreeImage(Sprite_LOADING_TEXT_DATA);
    g2d_FreeImage(Sprite_Punkt);

    g2d_FreeImage(Icon_tutorial_1);
    g2d_FreeImage(Icon_level_peep);

    g2d_FreeImage(Button_tut1_normal);
    g2d_FreeImage(Button_tut1_normal_pressed);
    g2d_FreeImage(Button_peep_normal);
    g2d_FreeImage(Button_peep_normal_pressed);

    g2dTerm();

    return 0;
}
