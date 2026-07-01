#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/neighbour/generic3.h"

const Animation neighbour_generic3_animations[] = {
    {
        // doubletake1
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_FIND1_2,
            FRAME_NEIGHBOUR_FIND1_3,
            FRAME_NEIGHBOUR_FIND1_4,
            FRAME_NEIGHBOUR_FIND1_5,
            FRAME_NEIGHBOUR_FIND1_6,
            FRAME_NEIGHBOUR_FIND1_7,
            FRAME_NEIGHBOUR_FIND1_8,
            FRAME_NEIGHBOUR_FIND1_9,
            FRAME_NEIGHBOUR_FIND1_10,
            FRAME_NEIGHBOUR_FIND1_11,
            FRAME_NEIGHBOUR_FIND1_12,
            FRAME_NEIGHBOUR_FIND1_13,
            FRAME_NEIGHBOUR_FIND1_14,
            FRAME_NEIGHBOUR_FIND1_15,
            FRAME_NEIGHBOUR_FIND1_16
        },

        .sfx = (const short[]) {
			SOUND_NONE,
			SOUND_NA_HUH1,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NA_SUP_HUH1,
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

        .frame_count = 15,

        .loop = false
    },

	{
        // discover3
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_FIND3_0,
			FRAME_NEIGHBOUR_FIND3_1,
			FRAME_NEIGHBOUR_FIND3_2,
            FRAME_NEIGHBOUR_FIND3_3,
            FRAME_NEIGHBOUR_FIND3_4,
            FRAME_NEIGHBOUR_FIND3_5,
            FRAME_NEIGHBOUR_FIND3_6
        },

        .sfx = (const short[]) {
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NA_SUP_HUH1,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE
        },

        .frame_count = 7,

        .loop = false
    },

    {
        // mr3
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_MR3_0,
            FRAME_NEIGHBOUR_MR3_1,
            FRAME_NEIGHBOUR_MR3_2,
            FRAME_NEIGHBOUR_MR3_3,
            FRAME_NEIGHBOUR_MR3_4,
            FRAME_NEIGHBOUR_MR3_5,
            FRAME_NEIGHBOUR_MR3_6,
            FRAME_NEIGHBOUR_MR3_7,
            FRAME_NEIGHBOUR_MR3_8,
            FRAME_NEIGHBOUR_MR3_9,
            FRAME_NEIGHBOUR_MR3_10,
            FRAME_NEIGHBOUR_MR3_11
        },

        .sfx = (const short[]) {
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NA_RUN1,
            SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
            SOUND_NONE,
			SOUND_NA_RUN2,
			SOUND_NONE,
			SOUND_NONE,
        },

        .frame_count = 12,

        .loop = true
    },

    {
        // use_mid
        .frames = (const unsigned short[]) {
			FRAME_NEIGHBOUR_USE_MID_0,
            FRAME_NEIGHBOUR_USE_MID_1,
            FRAME_NEIGHBOUR_USE_MID_2,
            FRAME_NEIGHBOUR_USE_MID_3,
            FRAME_NEIGHBOUR_USE_MID_4,
            FRAME_NEIGHBOUR_USE_MID_5,
            FRAME_NEIGHBOUR_USE_MID_6,
            FRAME_NEIGHBOUR_USE_MID_7,
            FRAME_NEIGHBOUR_USE_MID_8,
            FRAME_NEIGHBOUR_USE_MID_9,
            FRAME_NEIGHBOUR_USE_MID_10,
            FRAME_NEIGHBOUR_USE_MID_11,
            FRAME_NEIGHBOUR_USE_MID_12,
            FRAME_NEIGHBOUR_USE_MID_13,
            FRAME_NEIGHBOUR_USE_MID_12,
            FRAME_NEIGHBOUR_USE_MID_11,
            FRAME_NEIGHBOUR_USE_MID_10,
            FRAME_NEIGHBOUR_USE_MID_9,
            FRAME_NEIGHBOUR_USE_MID_7,
            FRAME_NEIGHBOUR_USE_MID_6,
            FRAME_NEIGHBOUR_USE_MID_5,
            FRAME_NEIGHBOUR_USE_MID_4,
            FRAME_NEIGHBOUR_USE_MID_3,
            FRAME_NEIGHBOUR_USE_MID_2
        },

        .sfx = (const short[]) {
			SOUND_NONE,
			SOUND_NA_USE1A,
			SOUND_NONE,
			SOUND_NONE,
            SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
			SOUND_NONE,
            SOUND_NA_USE1,
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

        .frame_count = 24,

        .loop = true
    }
};
