#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/ui/joe.h"

const Animation ui_joe_animations[] = {
    {
        // head
        .frames = (const unsigned short[]) {
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_0,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_0,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 22,

        .loop = true
    },

    {
        // head_talk
        .frames = (const unsigned short[]) {
            FRAME_UI_JOE_HEAD_2,
            FRAME_UI_JOE_HEAD_3,
            FRAME_UI_JOE_HEAD_4,
            FRAME_UI_JOE_HEAD_5,
            FRAME_UI_JOE_HEAD_6,
            FRAME_UI_JOE_HEAD_7,
            FRAME_UI_JOE_HEAD_8,
            FRAME_UI_JOE_HEAD_9,
            FRAME_UI_JOE_HEAD_10,
            FRAME_UI_JOE_HEAD_9,
            FRAME_UI_JOE_HEAD_8,
            FRAME_UI_JOE_HEAD_7,
            FRAME_UI_JOE_HEAD_6,
            FRAME_UI_JOE_HEAD_5,
            FRAME_UI_JOE_HEAD_4,
            FRAME_UI_JOE_HEAD_3,
            FRAME_UI_JOE_HEAD_2,
            
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_0,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_0,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1,
            FRAME_UI_JOE_HEAD_1
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 33,

        .loop = true
    }
};
