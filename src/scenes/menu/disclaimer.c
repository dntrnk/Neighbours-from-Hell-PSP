#include "../../objects/scene_manager.h"

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"

extern Scene MadeByDntrnkScene;

extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;

static int i;

static void init(void) {
    i = 0;
}

static void update(void) {
    controls_read();

    i++;

    if (controls_pressed(PSP_CTRL_CROSS)) {
        i = 616;
    }

    if (i >= 634) {
        scene_change(&MadeByDntrnkScene);
    }
}

static void draw(void) {
    g2d_Clear(BLACK);

    intraFontSetStyle(Font_BLUEHIGB_18, 1, RED, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);
    intraFontPrint(Font_BLUEHIGB_18, 190, 24 + intraFontTextHeight(Font_BLUEHIGB_18), "ДИСКЛЕЙМЕР");

    intraFontSetStyle(Font_BLUEHIGB_18, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);
    intraFontPrint(Font_BLUEHIGB_18, 16, 64 + intraFontTextHeight(Font_BLUEHIGB_18), "Это некоммерческий фанатский проект.");
    intraFontPrint(Font_BLUEHIGB_18, 16, 86 + intraFontTextHeight(Font_BLUEHIGB_18), "Все права на игру \"Neighbours from Hell\"");
    intraFontPrint(Font_BLUEHIGB_18, 16, 102 + intraFontTextHeight(Font_BLUEHIGB_18), "принадлежат THQ Nordic.");
    intraFontPrint(Font_BLUEHIGB_18, 16, 124 + intraFontTextHeight(Font_BLUEHIGB_18), "Проект не связан с THQ Nordic и не одобрен компанией.");
    intraFontPrint(Font_BLUEHIGB_18, 16, 146 + intraFontTextHeight(Font_BLUEHIGB_18), "Автор не претендует на права на оригинальные");
    intraFontPrint(Font_BLUEHIGB_18, 16, 162 + intraFontTextHeight(Font_BLUEHIGB_18), "игровые ресурсы.");

    intraFontPrint(Font_BLUEHIGB_18, 16, 228 + intraFontTextHeight(Font_BLUEHIGB_18), "По вопросам, связанным с проектом:");
    intraFontPrint(Font_BLUEHIGB_18, 16, 244 + intraFontTextHeight(Font_BLUEHIGB_18), "dntrnk.dev@gmail.com");

    g2d_DrawRectFilled(0, 268, (int) (i * 0.779f), 4, WHITE, 255); // progressbar

    if (i <= 17) {
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK, 255 - (i * 15));
    } else if (i >= 616) {
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK, (i - 616) * 15);
    }

    g2d_Flip(G2D_VSYNC);
}

Scene DisclaimerScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = NULL
};
