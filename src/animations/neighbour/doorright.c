#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/neighbour/doorright.h"

const Animation neighbour_doorright_animations[] = {
    {
        // doorright_leave
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_DOORRIGHT_0,
            FRAME_NEIGHBOUR_DOORRIGHT_1,
            FRAME_NEIGHBOUR_DOORRIGHT_2,
            FRAME_NEIGHBOUR_DOORRIGHT_3,
            FRAME_NEIGHBOUR_DOORRIGHT_4,
            FRAME_NEIGHBOUR_DOORRIGHT_5,
            FRAME_NEIGHBOUR_DOORRIGHT_6,
            FRAME_NEIGHBOUR_DOORRIGHT_7,
            FRAME_NEIGHBOUR_DOORRIGHT_8,
            FRAME_NEIGHBOUR_DOORRIGHT_9,
            FRAME_NEIGHBOUR_DOORRIGHT_10,
            FRAME_NEIGHBOUR_DOORRIGHT_11,
            FRAME_NEIGHBOUR_DOORRIGHT_12,
            FRAME_NEIGHBOUR_DOORRIGHT_13,
            FRAME_NEIGHBOUR_DOORRIGHT_14,
            FRAME_NEIGHBOUR_DOORRIGHT_15,
            FRAME_NEIGHBOUR_DOORRIGHT_16,
            FRAME_NEIGHBOUR_DOORRIGHT_17,
            FRAME_NEIGHBOUR_DOORRIGHT_18,
            FRAME_NEIGHBOUR_DOORRIGHT_19
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
    },

    {
        // doorright_enter
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_DOORRIGHT_19,
            FRAME_NEIGHBOUR_DOORRIGHT_18,
            FRAME_NEIGHBOUR_DOORRIGHT_17,
            FRAME_NEIGHBOUR_DOORRIGHT_16,
            FRAME_NEIGHBOUR_DOORRIGHT_15,
            FRAME_NEIGHBOUR_DOORRIGHT_14,
            FRAME_NEIGHBOUR_DOORRIGHT_13,
            FRAME_NEIGHBOUR_DOORRIGHT_12,
            FRAME_NEIGHBOUR_DOORRIGHT_11,
            FRAME_NEIGHBOUR_DOORRIGHT_10,
            FRAME_NEIGHBOUR_DOORRIGHT_9,
            FRAME_NEIGHBOUR_DOORRIGHT_8,
            FRAME_NEIGHBOUR_DOORRIGHT_7,
            FRAME_NEIGHBOUR_DOORRIGHT_6,
            FRAME_NEIGHBOUR_DOORRIGHT_5,
            FRAME_NEIGHBOUR_DOORRIGHT_4,
            FRAME_NEIGHBOUR_DOORRIGHT_3,
            FRAME_NEIGHBOUR_DOORRIGHT_2,
            FRAME_NEIGHBOUR_DOORRIGHT_1,
            FRAME_NEIGHBOUR_DOORRIGHT_0
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
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 20,

        .loop = false
    }
};
