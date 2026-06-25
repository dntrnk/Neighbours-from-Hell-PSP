#include "../../objects/scene_manager.h"

#include "../../engine/graphics/g2d.h"
#include "../../engine/NFHSound/NFHSound.h"

extern Scene CinemaMenuScene;

static g2dImage* dntrnk;
static g2dImage* dntrnk_text;

static int i;

static void init(void) {
    dntrnk = g2d_LoadImage("assets/sprites/ui/menu/dntrnk.png", G2D_CLUT4);
    dntrnk_text = g2d_LoadImage("assets/sprites/ui/menu/dntrnkText.png", G2D_CLUT4);

    i = 0;

    NFHMusicPlay(MUSIC_DNTRNK, 0);
}

static void update(void) {
    i++;

    if (i >= 200) {
        scene_change(&CinemaMenuScene);
    }
}

static void draw(void) {
    g2dClear(BLACK);

    g2d_DrawImageEasy(dntrnk, 176, 72, WHITE, 0, 255, G2D_UP_LEFT);
    g2d_DrawImageEasy(dntrnk_text, 198, 216, WHITE, 0, 255, G2D_UP_LEFT);

    g2dFlip(G2D_VSYNC);
}

static void unload(void) {
    g2d_FreeImage(dntrnk);
    g2d_FreeImage(dntrnk_text);
}

Scene MadeByDntrnkScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = unload
};
