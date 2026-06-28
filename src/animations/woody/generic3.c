#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/woody/generic3.h"

const Animation woody_generic3_animations[] = {
    {
        // decline
        .frames = (const unsigned short[]) {
            FRAME_WOODY_NONO_0,
            FRAME_WOODY_NONO_1,
            FRAME_WOODY_NONO_2,
            FRAME_WOODY_NONO_3,
            FRAME_WOODY_NONO_4,
            FRAME_WOODY_NONO_5,
            FRAME_WOODY_NONO_6,
            FRAME_WOODY_NONO_7,
            FRAME_WOODY_NONO_8,
            FRAME_WOODY_NONO_9,
            FRAME_WOODY_NONO_10,
            FRAME_WOODY_NONO_11,
            FRAME_WOODY_NONO_12,
            FRAME_WOODY_NONO_13,
            FRAME_WOODY_NONO_14,
            FRAME_WOODY_NONO_15,
            FRAME_WOODY_NONO_16,
            FRAME_WOODY_NONO_17,
            FRAME_WOODY_NONO_18
        },

        .sfx = (const short[]) {
            SOUND_WOD_NONO,
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
            SOUND_WOD_NONO,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 19,

        .loop = false
    },

    {
        // smile
        .frames = (const unsigned short[]) {
            FRAME_WOODY_SMILE_0,
            FRAME_WOODY_SMILE_1,
            FRAME_WOODY_SMILE_2,
            FRAME_WOODY_SMILE_3,
            FRAME_WOODY_SMILE_4,
            FRAME_WOODY_SMILE_5,
            FRAME_WOODY_SMILE_6,
            FRAME_WOODY_SMILE_7,
            FRAME_WOODY_SMILE_8,
            FRAME_WOODY_SMILE_9,
            FRAME_WOODY_SMILE_10,
            FRAME_WOODY_SMILE_11,
            FRAME_WOODY_SMILE_12
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_WOD_LAUGH2,
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

        .frame_count = 13,

        .loop = false
    },

    {
        // fear1
        .frames = (const unsigned short[]) {
            FRAME_WOODY_FEAR1_1,
            FRAME_WOODY_FEAR1_2,
            FRAME_WOODY_FEAR1_3,
            FRAME_WOODY_FEAR1_4,
            FRAME_WOODY_FEAR1_5,
            FRAME_WOODY_FEAR1_6,
            FRAME_WOODY_FEAR1_7,
            FRAME_WOODY_FEAR1_8,
            FRAME_WOODY_FEAR1_9,
        },

        .sfx = (const short[]) {
            SOUND_WOD_FEAR1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP2A,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 9,

        .loop = false
    },

    {
        // fear3
        .frames = (const unsigned short[]) {
            FRAME_WOODY_FEAR3_1,
            FRAME_WOODY_FEAR3_2,
            FRAME_WOODY_FEAR3_3,
            FRAME_WOODY_FEAR3_4,
            FRAME_WOODY_FEAR3_5,
            FRAME_WOODY_FEAR3_6,
            FRAME_WOODY_FEAR3_7,
            FRAME_WOODY_FEAR3_8,
            FRAME_WOODY_FEAR3_9,
        },

        .sfx = (const short[]) {
            SOUND_WOD_FEAR1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 9,

        .loop = false
    },

    {
        // fear1_loop
        .frames = (const unsigned short[]) {
            FRAME_WOODY_FEAR1_10,
            FRAME_WOODY_FEAR1_11
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 2,

        .loop = true
    },

    {
        // fear3_loop
        .frames = (const unsigned short[]) {
            FRAME_WOODY_FEAR3_10,
            FRAME_WOODY_FEAR3_11
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 2,

        .loop = true
    }
};
