#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/neighbour/sofa.h"

const Animation neighbour_sofa_animations[] = {
    {
        // sit_remo
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_SIT_REMO_0,
            FRAME_NEIGHBOUR_SIT_REMO_1,
            FRAME_NEIGHBOUR_SIT_REMO_2,
            FRAME_NEIGHBOUR_SIT_REMO_3,
            FRAME_NEIGHBOUR_SIT_REMO_4,
            FRAME_NEIGHBOUR_SIT_REMO_5,
            FRAME_NEIGHBOUR_SIT_REMO_6,
            FRAME_NEIGHBOUR_SIT_REMO_7,
            FRAME_NEIGHBOUR_SIT_REMO_8,
            FRAME_NEIGHBOUR_SIT_REMO_9,
            FRAME_NEIGHBOUR_SIT_REMO_10,
            FRAME_NEIGHBOUR_SIT_REMO_12,
            FRAME_NEIGHBOUR_SIT_REMO_13,
            FRAME_NEIGHBOUR_SIT_REMO_12,
            FRAME_NEIGHBOUR_SIT_REMO_11,
            FRAME_NEIGHBOUR_SIT_REMO_10,
            FRAME_NEIGHBOUR_SIT_REMO_9,
            FRAME_NEIGHBOUR_SIT_REMO_8,
            FRAME_NEIGHBOUR_SIT_REMO_7,
            FRAME_NEIGHBOUR_SIT_REMO_6,
            FRAME_NEIGHBOUR_SIT_REMO_5,
            FRAME_NEIGHBOUR_SIT_REMO_4,
            FRAME_NEIGHBOUR_SIT_REMO_3,
            FRAME_NEIGHBOUR_SIT_REMO_2,
            FRAME_NEIGHBOUR_SIT_REMO_1,
            FRAME_NEIGHBOUR_SIT_REMO_0
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_REMOTE1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 26,

        .loop = false
    },

    {
        // sit
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_2,
			FRAME_NEIGHBOUR_SIT_2,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_2,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1,
			FRAME_NEIGHBOUR_SIT_1
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
            SOUND_NONE
        },

        .frame_count = 32,

        .loop = false
    },

    {
        // sofa_enter
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_SITDOWN_5,
			FRAME_NEIGHBOUR_SITDOWN_4,
			FRAME_NEIGHBOUR_SITDOWN_3,
			FRAME_NEIGHBOUR_SITDOWN_2,
			FRAME_NEIGHBOUR_SITDOWN_1,
			FRAME_NEIGHBOUR_SITDOWN_0
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NA_SITSDOWN1,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 6,

        .loop = false
    },

    {
        // sofa_leave
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_SITDOWN_0,
			FRAME_NEIGHBOUR_SITDOWN_1,
			FRAME_NEIGHBOUR_SITDOWN_2,
			FRAME_NEIGHBOUR_SITDOWN_3,
			FRAME_NEIGHBOUR_SITDOWN_4,
			FRAME_NEIGHBOUR_SITDOWN_5
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_GETSUP1,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE
        },

        .frame_count = 6,

        .loop = false
    }
};
