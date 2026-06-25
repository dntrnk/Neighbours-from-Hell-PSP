#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/ui/sign.h"

const Animation ui_sign_animations[] = {
    {
        // ms0
        .frames = (const unsigned short[]) {
            FRAME_UI_SIGN_SIGN1,
            FRAME_UI_SIGN_SIGN2,
            FRAME_UI_SIGN_SIGN3,
            FRAME_UI_SIGN_SIGN4,
            FRAME_UI_SIGN_SIGN5,
            FRAME_UI_SIGN_SIGN6,
            FRAME_UI_SIGN_SIGN6,
            FRAME_UI_SIGN_SIGN5,
            FRAME_UI_SIGN_SIGN4,
            FRAME_UI_SIGN_SIGN3,
            FRAME_UI_SIGN_SIGN2,
            FRAME_UI_SIGN_SIGN1,
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
            SOUND_NONE
        },

        .frame_count = 12,

        .loop = true
    }
};
