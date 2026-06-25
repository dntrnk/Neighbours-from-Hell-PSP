#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/neighbour/doorleft.h"

const Animation neighbour_doorleft_animations[] = {
    {
        // doorleft_enter
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_DOORLEFT_0,
            FRAME_NEIGHBOUR_DOORLEFT_1,
            FRAME_NEIGHBOUR_DOORLEFT_2,
            FRAME_NEIGHBOUR_DOORLEFT_3,
            FRAME_NEIGHBOUR_DOORLEFT_4,
            FRAME_NEIGHBOUR_DOORLEFT_5,
            FRAME_NEIGHBOUR_DOORLEFT_6,
            FRAME_NEIGHBOUR_DOORLEFT_7,
            FRAME_NEIGHBOUR_DOORLEFT_8,
            FRAME_NEIGHBOUR_DOORLEFT_9,
            FRAME_NEIGHBOUR_DOORLEFT_10,
            FRAME_NEIGHBOUR_DOORLEFT_11,
            FRAME_NEIGHBOUR_DOORLEFT_12,
            FRAME_NEIGHBOUR_DOORLEFT_13,
            FRAME_NEIGHBOUR_DOORLEFT_14,
            FRAME_NEIGHBOUR_DOORLEFT_15,
            FRAME_NEIGHBOUR_DOORLEFT_16,
            FRAME_NEIGHBOUR_DOORLEFT_17,
            FRAME_NEIGHBOUR_DOORLEFT_18,
            FRAME_NEIGHBOUR_DOORLEFT_19
        },

        .sfx = {
            SOUND_NONE,
            SOUND_DOOR_OPEN1,
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

        .frame_count = 20,

        .loop = false
    },

    {
        // doorleft_leave
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_DOORLEFT_19,
            FRAME_NEIGHBOUR_DOORLEFT_18,
            FRAME_NEIGHBOUR_DOORLEFT_17,
            FRAME_NEIGHBOUR_DOORLEFT_16,
            FRAME_NEIGHBOUR_DOORLEFT_15,
            FRAME_NEIGHBOUR_DOORLEFT_14,
            FRAME_NEIGHBOUR_DOORLEFT_13,
            FRAME_NEIGHBOUR_DOORLEFT_12,
            FRAME_NEIGHBOUR_DOORLEFT_11,
            FRAME_NEIGHBOUR_DOORLEFT_10,
            FRAME_NEIGHBOUR_DOORLEFT_9,
            FRAME_NEIGHBOUR_DOORLEFT_8,
            FRAME_NEIGHBOUR_DOORLEFT_7,
            FRAME_NEIGHBOUR_DOORLEFT_6,
            FRAME_NEIGHBOUR_DOORLEFT_5,
            FRAME_NEIGHBOUR_DOORLEFT_4,
            FRAME_NEIGHBOUR_DOORLEFT_3,
            FRAME_NEIGHBOUR_DOORLEFT_2,
            FRAME_NEIGHBOUR_DOORLEFT_1,
            FRAME_NEIGHBOUR_DOORLEFT_0
        },

        .sfx = {
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
            SOUND_DOOR_CLOSE1,
            SOUND_NONE
        },

        .frame_count = 20,

        .loop = false
    }
};
