#include "intro.h"

#include <stdlib.h>
#include <string.h>

#include "../engine/graphics/g2d.h"
#include "../engine/fonts/intraFont.h"
#include "../engine/NFHSound/NFHSound.h"
#include "../engine/controls/controls.h"

#define GRAY G2D_RGBA(128, 128, 128, 255)
#define BLACK_ALPHA G2D_RGBA(0, 0, 0, 80)
#define BLUE_EPISODE_NAME G2D_RGBA(65, 92, 134, 255)

#define TIMER_LOGO_START_MOVE_IN 192
#define TIMER_LOGO_STOP_MOVE_IN 250
#define TIMER_EPISODE_START_MOVE_IN 340
#define TIMER_EPISODE_START_MOVE_OUT 480
#define TIMER_LOGO_START_MOVE_OUT 500
#define TIMER_INTRO_END 552
#define TIMER_SONG_END 882

extern g2dImage* SpriteAtlas_INTRO;
extern g2dImage* Sprite_NFH_LOGO;
extern g2dImage* Sprite_LOADING_TEXT_DATA;

extern intraFont* Font_ACMESA_17_9;
extern intraFont* Font_ACMESAI_13;

Intro* intro_create(const char* episode_name, bool move_woody) {
    Intro* intro = malloc(sizeof(Intro));
    memset(intro, 0, sizeof(Intro));

    NFHHouseMusicLoad();

    strcpy(intro->episode_name, episode_name);
    intro->move_woody = move_woody;

    intro->draw_intro = true;
    intro->timer = 0;
    intro->song_timer = 0;

    intro->jwd_logo_y = 21;
    intro->jwd_logo_speed = 0;

    intro->nfh_logo_x = -139;
    intro->nfh_logo_y = 48;
    intro->nfh_logo_speed_x = 0;
    intro->nfh_logo_speed_y = 0;

    intro->in_episode_speed = 0;
    intro->in_episode_x = 749;

    intro->episode_speed = 0;
    intro->episode_x = -270;

    intraFontSetStyle(Font_ACMESAI_13, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intro->text1_offset = (int) (intraFontMeasureText(Font_ACMESAI_13, "в серии") * 0.5);

    intraFontSetStyle(Font_ACMESA_17_9, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intro->text2_offset = (int) (intraFontMeasureText(Font_ACMESA_17_9, intro->episode_name) * 0.5);

    return intro;
}

int intro_update(Intro* intro) {
    int exit_code = 0;

    intro->timer++;

    if ((controls_released(PSP_CTRL_START) || controls_pressed(PSP_CTRL_CROSS)) && intro->draw_intro && intro->song_timer >= 80) {
        intro->timer = TIMER_INTRO_END;
    }

    if (intro->timer == TIMER_LOGO_START_MOVE_IN) {
        intro->jwd_logo_speed = -5;
        intro->nfh_logo_speed_x = 10;
    } else if (intro->timer == TIMER_LOGO_STOP_MOVE_IN) {
        intro->jwd_logo_speed = 0;
    } else if (intro->timer == TIMER_EPISODE_START_MOVE_IN) {
        intro->in_episode_speed = -15;
        intro->episode_speed = 15;
    } else if (intro->timer == TIMER_EPISODE_START_MOVE_OUT) {
        intro->in_episode_speed = -15;
        intro->episode_speed = 15;
    } else if (intro->timer == TIMER_LOGO_START_MOVE_OUT) {
        intro->nfh_logo_speed_y = -5;
    } else if (intro->timer == TIMER_INTRO_END) {
        intro->draw_intro = false;
        NFHSoundPlay(SOUND_LEVEL_START);
        if (intro->move_woody) {
            exit_code = INTRO_WOODY_START_MOVE_TO_HOUSE;
        } else {
            exit_code = INTRO_WOODY_START_ANIMATION;
        }
    } else if (intro->timer == 597 && !intro->move_woody) {
        exit_code = INTRO_WOODY_CAN_MOVE;
    }

    if (intro->move_woody) {
        if (intro->timer == 706) {
            exit_code = INTRO_NEIGHBOUR_CAN_MOVE;
        } else if (intro->timer == 756) {
            exit_code = INTRO_WOODY_START_ANIMATION;
        } else if (intro->timer == 801) {
            exit_code = INTRO_WOODY_CAN_MOVE;
        }
    }

    intro->jwd_logo_y += intro->jwd_logo_speed;
    intro->nfh_logo_x += intro->nfh_logo_speed_x;
    intro->nfh_logo_y += intro->nfh_logo_speed_y;
    intro->in_episode_x += intro->in_episode_speed;
    intro->episode_x += intro->episode_speed;
        
    if (intro->nfh_logo_x == 181) {
        intro->nfh_logo_speed_x = 0;
    }

    if (intro->in_episode_x == 239) {
        intro->in_episode_speed = 0;
        intro->episode_speed = 0;
    }

    intro->song_timer++;

    if (intro->song_timer == TIMER_SONG_END) {
        NFHHouseMusicPlay();
        exit_code = INTRO_DELETE;
    }

    return exit_code;
}

void intro_draw_loading(int progress) {
    // Loading BG
    g2d_DrawImageExt(SpriteAtlas_INTRO, 0, 0, 480, 272, WHITE, 0, 0, 480, 272, 0, 255, G2D_UP_LEFT);

    // Progressbar
    g2d_DrawImageExt(SpriteAtlas_INTRO, 147, 216, 186, 26, WHITE, 0, 272, 186, 26, 0, 255, G2D_UP_LEFT);

    if (progress != 0)
        g2d_DrawImageExt(SpriteAtlas_INTRO, 147, 216, progress, 26, WHITE, 0, 298, progress, 26, 0, 255, G2D_UP_LEFT);

    g2d_DrawImage(Sprite_LOADING_TEXT_DATA, 175, 250, WHITE, 0, 255, G2D_UP_LEFT);
}

void intro_draw(const Intro* intro) {
    if (intro->song_timer < 80) {
        intro_draw_loading(186);
    } else if (intro->draw_intro) {
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK_ALPHA);

        // JWD Logo
        if (intro->timer < 250) {
            g2d_DrawImageExt(SpriteAtlas_INTRO, 120, intro->jwd_logo_y, 240, 129, WHITE, 0, 324, 240, 129, 0, 255, G2D_UP_LEFT);

            // in_episode shadow
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);
            intraFontPrint(Font_ACMESAI_13, 206, intro->jwd_logo_y + 152 + intraFontTextHeight(Font_ACMESAI_13), "представляет");

            // in_episode outline
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(Font_ACMESAI_13, 203-i, intro->jwd_logo_y + 149-j + intraFontTextHeight(Font_ACMESAI_13), "представляет");
            }

            // in_episode text
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(Font_ACMESAI_13, 203, intro->jwd_logo_y + 149 + intraFontTextHeight(Font_ACMESAI_13), "представляет");
        }

        if (intro->timer > 192)
            g2d_DrawImage(Sprite_NFH_LOGO, intro->nfh_logo_x, intro->nfh_logo_y, WHITE, 0, 255, G2D_UP_LEFT);

        if (intro->timer > 340) {
            // а может всё-таки сделать функции-обёртки
            // вместо того чтобы всегда писать SetStyle(), Activate() и т.д?

            // in_episode //

            // in_episode shadow
            intraFontSetStyle(Font_ACMESAI_13, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);
            intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset + 3, 141 + intraFontTextHeight(Font_ACMESAI_13), "в серии");

            // in_episode outline
            intraFontSetStyle(Font_ACMESAI_13, 1, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset-i, 138-j + intraFontTextHeight(Font_ACMESAI_13), "в серии");
            }

            // in_episode text
            intraFontSetStyle(Font_ACMESAI_13, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset, 138 + intraFontTextHeight(Font_ACMESAI_13), "в серии");

            // episode_name //

            // episode_name shadow
            intraFontSetStyle(Font_ACMESA_17_9, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESA_17_9, 0);
            intraFontPrint(Font_ACMESA_17_9, intro->episode_x - intro->text2_offset + 3, 162 + intraFontTextHeight(Font_ACMESA_17_9), intro->episode_name);

            // episode_name outline
            intraFontSetStyle(Font_ACMESA_17_9, 1, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESA_17_9, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(Font_ACMESA_17_9, intro->episode_x - intro->text2_offset-i, 159-j + intraFontTextHeight(Font_ACMESA_17_9), intro->episode_name);
            }

            // episode_name text   
            intraFontSetStyle(Font_ACMESA_17_9, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(Font_ACMESA_17_9, intro->episode_x - intro->text2_offset, 159 + intraFontTextHeight(Font_ACMESA_17_9), intro->episode_name);
        }
    }
}

void intro_unload(Intro* intro) {
    free(intro);
}
