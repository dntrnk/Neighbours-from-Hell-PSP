#include "intro.h"

#include <stdlib.h>
#include <string.h>

#include "../engine/graphics/g2d.h"
#include "../engine/NFHSound/NFHSound.h"
#include "../engine/controls/controls.h"

#include "scene_manager.h"

#include "localizations.h"

#define GRAY G2D_RGB(128, 128, 128)
#define BLUE_EPISODE_NAME G2D_RGB(65, 92, 134)

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
extern intraFont* Font_ACMESAI_17_9;
extern intraFont* Font_ACMESAI_13;

Intro* intro_create(
    const char* episode_name,
    Woody* woody,
    bool move_woody,
    int camera_extra_x,
    int camera_extra_y
) {
    Intro* intro = malloc(sizeof(Intro));
    memset(intro, 0, sizeof(Intro));

    char loc_filename[512];
    sprintf(loc_filename, "data/localizations/%s/intro.json", current_lang_code);

    FILE* loc_file = fopen(loc_filename, "r");

    if (!loc_file) {
        scene_error("Не удалось открыть файл %s", loc_filename);
    }

    fseek(loc_file, 0, SEEK_END);
    long loc_size = ftell(loc_file);
    fseek(loc_file, 0, SEEK_SET);
    char* loc_json = malloc(loc_size + 1);
    fread(loc_json, 1, loc_size, loc_file);
    loc_json[loc_size] = '\0';
    fclose(loc_file);
    cJSON* parsed_loc_json = cJSON_Parse(loc_json);
    free(loc_json);

    NFHHouseMusicLoad();

    switch (current_lang) {
        case LANG_ENGLISH:
            intro->episode_name_font = Font_ACMESAI_17_9;

            break;
        case LANG_RUSSIAN:
            intro->episode_name_font = Font_ACMESA_17_9;

            break;
    }
    strcpy(intro->episode_name, episode_name);
    intro->move_woody = move_woody;

    intro->draw_intro = true;
    intro->timer = 0;
    intro->song_timer = 0;

    strcpy(intro->presents_text, json_get_item_string(parsed_loc_json, "presents", 31));

    intraFontSetStyle(Font_ACMESAI_13, 0.7f, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_ACMESAI_13, 0);
    intro->presents_text_x = 240 - (intraFontMeasureText(Font_ACMESAI_13, intro->presents_text) * 0.5);

    intro->jwd_logo_y = 21;
    intro->jwd_logo_speed = 0;

    intro->nfh_logo_x = -139;
    intro->nfh_logo_y = 48;

    switch (current_lang) {
        case LANG_RUSSIAN:
            intro->nfh_logo_offset_x = 0;
            intro->nfh_logo_offset_y = 0;

            break;
        case LANG_ENGLISH:
            intro->nfh_logo_offset_x = -22;
            intro->nfh_logo_offset_y = 10;

            break;
    }

    intro->nfh_logo_speed_x = 0;
    intro->nfh_logo_speed_y = 0;

    strcpy(intro->in_episode_text, json_get_item_string(parsed_loc_json, "in_episode", 31));

    intro->in_episode_speed = 0;
    intro->in_episode_x = 749;

    intro->episode_speed = 0;
    intro->episode_x = -270;

    intraFontSetStyle(Font_ACMESAI_13, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intro->text1_offset = (int) (intraFontMeasureText(Font_ACMESAI_13, intro->in_episode_text) * 0.5);

    intraFontSetStyle(intro->episode_name_font, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
    intro->text2_offset = (int) (intraFontMeasureText(intro->episode_name_font, intro->episode_name) * 0.5);

    intro->camera_extra_x = camera_extra_x;
    intro->camera_extra_y = camera_extra_y;

    intro->woody = woody;

    cJSON_Delete(parsed_loc_json);

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
        NFHSoundPlay(SOUND_LEVELSTART);
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

    if (intro->timer > TIMER_INTRO_END) {
        if (intro->camera_extra_x < 0) {
            intro->camera_extra_x += 10;
            if (intro->camera_extra_x > 0) {
                intro->camera_extra_x = 0;
            }
        }
        if (intro->camera_extra_y < 0) {
            intro->camera_extra_y += 10;
            if (intro->camera_extra_y > 0) {
                intro->camera_extra_y = 0;
            }
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
        if (!intro->woody->caught) {
            NFHHouseMusicPlay();
        }
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
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK, 80);

        // JWD Logo
        if (intro->timer < 250) {
            g2d_DrawImageExt(SpriteAtlas_INTRO, 120, intro->jwd_logo_y, 240, 129, WHITE, 0, 324, 240, 129, 0, 255, G2D_UP_LEFT);

            // in_episode shadow
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);
            intraFontPrint(Font_ACMESAI_13, intro->presents_text_x + 2, intro->jwd_logo_y + 142 + intraFontTextHeight(Font_ACMESAI_13), intro->presents_text);

            // in_episode outline
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(Font_ACMESAI_13, intro->presents_text_x-i, intro->jwd_logo_y + 139-j + intraFontTextHeight(Font_ACMESAI_13), intro->presents_text);
            }

            // in_episode text
            intraFontSetStyle(Font_ACMESAI_13, 0.7f, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(Font_ACMESAI_13, intro->presents_text_x, intro->jwd_logo_y + 139 + intraFontTextHeight(Font_ACMESAI_13), intro->presents_text);
        }

        if (intro->timer > 192)
            g2d_DrawImage(Sprite_NFH_LOGO, intro->nfh_logo_x + intro->nfh_logo_offset_x, intro->nfh_logo_y + intro->nfh_logo_offset_y, WHITE, 0, 255, G2D_UP_LEFT);

        if (intro->timer > 340) {
            // а может всё-таки сделать функции-обёртки
            // вместо того чтобы всегда писать SetStyle(), Activate() и т.д?

            // in_episode //

            // in_episode shadow
            intraFontSetStyle(Font_ACMESAI_13, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);
            intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset + 3, 141 + intraFontTextHeight(Font_ACMESAI_13), intro->in_episode_text);

            // in_episode outline
            intraFontSetStyle(Font_ACMESAI_13, 1, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(Font_ACMESAI_13, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset-i, 138-j + intraFontTextHeight(Font_ACMESAI_13), intro->in_episode_text);
            }

            // in_episode text
            intraFontSetStyle(Font_ACMESAI_13, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(Font_ACMESAI_13, intro->in_episode_x - intro->text1_offset, 138 + intraFontTextHeight(Font_ACMESAI_13), intro->in_episode_text);

            // episode_name //

            // episode_name shadow
            intraFontSetStyle(intro->episode_name_font, 1, BLACK, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(intro->episode_name_font, 0);
            intraFontPrint(intro->episode_name_font, intro->episode_x - intro->text2_offset + 3, 162 + intraFontTextHeight(intro->episode_name_font), intro->episode_name);

            // episode_name outline
            intraFontSetStyle(intro->episode_name_font, 1, BLUE_EPISODE_NAME, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontActivate(intro->episode_name_font, 0);

            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                    intraFontPrint(intro->episode_name_font, intro->episode_x - intro->text2_offset-i, 159-j + intraFontTextHeight(intro->episode_name_font), intro->episode_name);
            }

            // episode_name text   
            intraFontSetStyle(intro->episode_name_font, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
            intraFontPrint(intro->episode_name_font, intro->episode_x - intro->text2_offset, 159 + intraFontTextHeight(intro->episode_name_font), intro->episode_name);
        }
    }
}

void intro_unload(Intro* intro) {
    free(intro);
}
