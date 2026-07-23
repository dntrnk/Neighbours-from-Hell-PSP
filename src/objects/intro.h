#ifndef INTRO_H
#define INTRO_H

#include <stdbool.h>

#include "../engine/fonts/intraFont.h"

#include "woody.h"


#define INTRO_WOODY_START_MOVE_TO_HOUSE 1
#define INTRO_WOODY_START_ANIMATION 2
#define INTRO_NEIGHBOUR_CAN_MOVE 3
#define INTRO_WOODY_CAN_MOVE 4
#define INTRO_DELETE 5

typedef struct Woody Woody;

typedef struct {
    intraFont* episode_name_font;
    char episode_name[128];
    bool move_woody;

    bool draw_intro;
    int timer;
    int song_timer;

    char presents_text[32];

    int presents_text_x;

    int jwd_logo_y;
    int jwd_logo_speed;

    int nfh_logo_x;
    int nfh_logo_y;
    int nfh_logo_offset_x;
    int nfh_logo_offset_y;
    int nfh_logo_speed_x;
    int nfh_logo_speed_y;

    char in_episode_text[32];

    int in_episode_x;
    int in_episode_speed;

    int episode_x;
    int episode_speed;

    int text1_offset;
    int text2_offset;

    int camera_extra_x;
    int camera_extra_y;

    int camera_limit_x;
    int camera_limit_y;

    Woody* woody;
} Intro;

Intro* intro_create(const char* episode_name, Woody* woody, bool move_woody, int camera_extra_x, int camera_extra_y);
int intro_update(Intro* intro);
void intro_draw_loading(int progress);
void intro_draw(const Intro* intro);
void intro_unload(Intro* intro);

#endif // INTRO_H