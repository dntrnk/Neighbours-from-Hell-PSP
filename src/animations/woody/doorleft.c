#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/woody/doorleft.h"

const Animation woody_doorleft_animations[] = {
    {
        // doorleft_enter
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORLEFT_6,
            FRAME_WOODY_DOORLEFT_7,
            FRAME_WOODY_DOORLEFT_8,
            FRAME_WOODY_DOORLEFT_9,
            FRAME_WOODY_DOORLEFT_10,
            FRAME_WOODY_DOORLEFT_11,
            FRAME_WOODY_DOORLEFT_12,
            FRAME_WOODY_DOORLEFT_13,
            FRAME_WOODY_DOORLEFT_14,
            FRAME_WOODY_DOORLEFT_15,
            FRAME_WOODY_DOORLEFT_16,
            FRAME_WOODY_DOORLEFT_17,
            FRAME_WOODY_DOORLEFT_18,
            FRAME_WOODY_DOORLEFT_19,
            FRAME_WOODY_DOORLEFT_20,
            FRAME_WOODY_DOORLEFT_21,
            FRAME_WOODY_DOORLEFT_22,
            FRAME_WOODY_DOORLEFT_23,
            FRAME_WOODY_DOORLEFT_24
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
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
            SOUND_NONE
        },

        .frame_count = 19,

        .loop = false
    },

    {
        // doorleft_leave
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORLEFT_24,
            FRAME_WOODY_DOORLEFT_23,
            FRAME_WOODY_DOORLEFT_22,
            FRAME_WOODY_DOORLEFT_21,
            FRAME_WOODY_DOORLEFT_20,
            FRAME_WOODY_DOORLEFT_19,
            FRAME_WOODY_DOORLEFT_18,
            FRAME_WOODY_DOORLEFT_17,
            FRAME_WOODY_DOORLEFT_16,
            FRAME_WOODY_DOORLEFT_15,
            FRAME_WOODY_DOORLEFT_14,
            FRAME_WOODY_DOORLEFT_13,
            FRAME_WOODY_DOORLEFT_12,
            FRAME_WOODY_DOORLEFT_11,
            FRAME_WOODY_DOORLEFT_10,
            FRAME_WOODY_DOORLEFT_9,
            FRAME_WOODY_DOORLEFT_8,
            FRAME_WOODY_DOORLEFT_7,
            FRAME_WOODY_DOORLEFT_6,
            FRAME_WOODY_DOORLEFT_5,
            FRAME_WOODY_DOORLEFT_4,
            FRAME_WOODY_DOORLEFT_3,
            FRAME_WOODY_DOORLEFT_2,
            FRAME_WOODY_DOORLEFT_1,
            FRAME_WOODY_DOORLEFT_0
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
            SOUND_DOOR_CLOSE1,
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

        .frame_count = 25,

        .loop = false
    }
};
