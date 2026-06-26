#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/woody/doorright.h"

const Animation woody_doorright_animations[] = {
    {
        // doorright_enter
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORRIGHT_15,
            FRAME_WOODY_DOORRIGHT_14,
            FRAME_WOODY_DOORRIGHT_13,
            FRAME_WOODY_DOORRIGHT_12,
            FRAME_WOODY_DOORRIGHT_11,
            FRAME_WOODY_DOORRIGHT_10,
            FRAME_WOODY_DOORRIGHT_9,
            FRAME_WOODY_DOORRIGHT_8,
            FRAME_WOODY_DOORRIGHT_7,
            FRAME_WOODY_DOORRIGHT_6,
            FRAME_WOODY_DOORRIGHT_5,
            FRAME_WOODY_DOORRIGHT_4,
            FRAME_WOODY_DOORRIGHT_3,
            FRAME_WOODY_DOORRIGHT_2,
            FRAME_WOODY_DOORRIGHT_1,
            FRAME_WOODY_DOORRIGHT_0
        },

        .sfx = (const short[]) {
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

        .frame_count = 16,

        .loop = false
    },

    {
        // doorright_leave
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORRIGHT_0,
            FRAME_WOODY_DOORRIGHT_0,
            FRAME_WOODY_DOORRIGHT_0,
            FRAME_WOODY_DOORRIGHT_1,
            FRAME_WOODY_DOORRIGHT_2,
            FRAME_WOODY_DOORRIGHT_3,
            FRAME_WOODY_DOORRIGHT_4,
            FRAME_WOODY_DOORRIGHT_5,
            FRAME_WOODY_DOORRIGHT_6,
            FRAME_WOODY_DOORRIGHT_7,
            FRAME_WOODY_DOORRIGHT_8,
            FRAME_WOODY_DOORRIGHT_9,
            FRAME_WOODY_DOORRIGHT_10,
            FRAME_WOODY_DOORRIGHT_11,
            FRAME_WOODY_DOORRIGHT_12,
            FRAME_WOODY_DOORRIGHT_13,
            FRAME_WOODY_DOORRIGHT_14,
            FRAME_WOODY_DOORRIGHT_15,
            FRAME_WOODY_DOORRIGHT_16,
            FRAME_WOODY_DOORRIGHT_17,
            FRAME_WOODY_DOORRIGHT_18,
            FRAME_WOODY_DOORRIGHT_19,
            FRAME_WOODY_DOORRIGHT_20,
            FRAME_WOODY_DOORRIGHT_21
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
            SOUND_DOOR_CLOSE1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 24,

        .loop = false
    }
};
