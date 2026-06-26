#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/woody/doorback.h"

const Animation woody_doorback_animations[] = {
    {
        // doorback_enter
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORBACK_LEAVE_0,
            FRAME_WOODY_DOORBACK_LEAVE_1,
            FRAME_WOODY_DOORBACK_LEAVE_2,
            FRAME_WOODY_DOORBACK_LEAVE_3,
            FRAME_WOODY_DOORBACK_LEAVE_4,
            FRAME_WOODY_DOORBACK_LEAVE_5,
            FRAME_WOODY_DOORBACK_LEAVE_6,
            FRAME_WOODY_DOORBACK_LEAVE_7,
            FRAME_WOODY_DOORBACK_LEAVE_8,
            FRAME_WOODY_DOORBACK_LEAVE_9
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
            SOUND_NONE
        },

        .frame_count = 10,

        .loop = false
    },

    {
        // doorback_leave
        .frames = (const unsigned short[]) {
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_15,
            FRAME_WOODY_DOORBACK_ENTER_14,
            FRAME_WOODY_DOORBACK_ENTER_13,
            FRAME_WOODY_DOORBACK_ENTER_12,
            FRAME_WOODY_DOORBACK_ENTER_11,
            FRAME_WOODY_DOORBACK_ENTER_10,
            FRAME_WOODY_DOORBACK_ENTER_9,
            FRAME_WOODY_DOORBACK_ENTER_8,
            FRAME_WOODY_DOORBACK_ENTER_7,
            FRAME_WOODY_DOORBACK_ENTER_6,
            FRAME_WOODY_DOORBACK_ENTER_5,
            FRAME_WOODY_DOORBACK_ENTER_4,
            FRAME_WOODY_DOORBACK_ENTER_3,
            FRAME_WOODY_DOORBACK_ENTER_2,
            FRAME_WOODY_DOORBACK_ENTER_1,
            FRAME_WOODY_DOORBACK_ENTER_0
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
            SOUND_DOOR_CLOSE1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 26,

        .loop = false
    }
};
