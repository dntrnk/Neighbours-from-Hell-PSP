#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"

#include "../../gfxdata/woody/generic2.h"

const Animation woody_generic2_animations[] = {
    {
        // wardrobe_enter
        .frames = (const unsigned short[]) {
            FRAME_WOODY_WARDROBE_0,
            FRAME_WOODY_WARDROBE_1,
            FRAME_WOODY_WARDROBE_2,
            FRAME_WOODY_WARDROBE_3,
            FRAME_WOODY_WARDROBE_4,
            FRAME_WOODY_WARDROBE_5,
            FRAME_WOODY_WARDROBE_6,
            FRAME_WOODY_WARDROBE_7,
            FRAME_WOODY_WARDROBE_8,
            FRAME_WOODY_WARDROBE_9,
            FRAME_WOODY_WARDROBE_10,
            FRAME_WOODY_WARDROBE_11,
            FRAME_WOODY_WARDROBE_12,
            FRAME_WOODY_WARDROBE_13,
            FRAME_WOODY_WARDROBE_14,
            FRAME_WOODY_WARDROBE_15,
            FRAME_WOODY_WARDROBE_16,
            FRAME_WOODY_WARDROBE_17,
            FRAME_WOODY_WARDROBE_18,
            FRAME_WOODY_WARDROBE_19
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_OBJ_OPEN1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE,
            SOUND_WOD_STEP2A,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_OBJ_CLOSE1,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 20,

        .loop = false
    },

    {
        // wardrobe_leave
        .frames = (const unsigned short[]) {
            FRAME_WOODY_WARDROBE_19,
            FRAME_WOODY_WARDROBE_18,
            FRAME_WOODY_WARDROBE_17,
            FRAME_WOODY_WARDROBE_16,
            FRAME_WOODY_WARDROBE_15,
            FRAME_WOODY_WARDROBE_14,
            FRAME_WOODY_WARDROBE_13,
            FRAME_WOODY_WARDROBE_12,
            FRAME_WOODY_WARDROBE_11,
            FRAME_WOODY_WARDROBE_10,
            FRAME_WOODY_WARDROBE_9,
            FRAME_WOODY_WARDROBE_8,
            FRAME_WOODY_WARDROBE_7,
            FRAME_WOODY_WARDROBE_6,
            FRAME_WOODY_WARDROBE_5,
            FRAME_WOODY_WARDROBE_4,
            FRAME_WOODY_WARDROBE_3,
            FRAME_WOODY_WARDROBE_2,
            FRAME_WOODY_WARDROBE_1,
            FRAME_WOODY_WARDROBE_0
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_OBJ_OPEN1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_STEP2A,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_OBJ_CLOSE1,
            SOUND_NONE,
            SOUND_WOD_STEP1A,
            SOUND_NONE
        },

        .frame_count = 20,

        .loop = false
    },

    {
        // open
        .frames = (const unsigned short[]) {
            FRAME_WOODY_USE_MID_0
        },

        .sfx = (const short[]) {
            SOUND_NONE
        },

        .frame_count = 1,

        .loop = false
    },

    {
        // close
        .frames = (const unsigned short[]) {
            FRAME_WOODY_USE_MID_0
        },

        .sfx = (const short[]) {
            SOUND_NONE
        },

        .frame_count = 1,

        .loop = false
    },

    {
        // take0
        .frames = (const unsigned short[]) {
            FRAME_WOODY_ITEMFOUND0_0,
            FRAME_WOODY_ITEMFOUND0_1,
            FRAME_WOODY_ITEMFOUND0_2,
            FRAME_WOODY_ITEMFOUND0_3,
            FRAME_WOODY_ITEMFOUND0_4,
            FRAME_WOODY_ITEMFOUND0_4,
            FRAME_WOODY_ITEMFOUND0_4,
            FRAME_WOODY_ITEMFOUND0_5,
            FRAME_WOODY_GT0_0,
            FRAME_WOODY_GT0_1,
            FRAME_WOODY_GT0_2,
            FRAME_WOODY_GT0_3,
            FRAME_WOODY_GT0_4,
            FRAME_WOODY_GT0_5,
            FRAME_WOODY_GT0_6,
            FRAME_WOODY_GT0_11,
            FRAME_WOODY_GT0_13,
            FRAME_WOODY_GT0_14,
            FRAME_WOODY_GT0_15
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_HA1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_GIVE_TAKE2,
            SOUND_NONE,
            SOUND_GIVE_TAKE1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 19,

        .loop = false
    },

    {
        // lose
        .frames = (const unsigned short[]) {
            FRAME_WOODY_WHATSUP_0,
            FRAME_WOODY_WHATSUP_1,
            FRAME_WOODY_WHATSUP_2,
            FRAME_WOODY_WHATSUP_3,
            FRAME_WOODY_WHATSUP_4,
            FRAME_WOODY_WHATSUP_5,
            FRAME_WOODY_WHATSUP_6,
            FRAME_WOODY_WHATSUP_7,
            FRAME_WOODY_WHATSUP_8,
            FRAME_WOODY_WHATSUP_9,
            FRAME_WOODY_WHATSUP_10,
            FRAME_WOODY_WHATSUP_11,
            FRAME_WOODY_WHATSUP_12,
            FRAME_WOODY_WHATSUP_13
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_WOD_EH1,
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

        .frame_count = 14,

        .loop = false
    },

    {
        // take_high3
        .frames = (const unsigned short[]) {
            FRAME_WOODY_ITEMFOUND3_0,
            FRAME_WOODY_ITEMFOUND3_1,
            FRAME_WOODY_ITEMFOUND3_2,
            FRAME_WOODY_ITEMFOUND3_3,
            FRAME_WOODY_ITEMFOUND3_4,
            FRAME_WOODY_ITEMFOUND3_4,
            FRAME_WOODY_ITEMFOUND3_4,
            FRAME_WOODY_ITEMFOUND3_5,
            FRAME_WOODY_TAKEHIGH3_0,
            FRAME_WOODY_TAKEHIGH3_1,
            FRAME_WOODY_TAKEHIGH3_1,
            FRAME_WOODY_TAKEHIGH3_2,
            FRAME_WOODY_TAKEHIGH3_3,
            FRAME_WOODY_TAKEHIGH3_4,
            FRAME_WOODY_TAKEHIGH3_5
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_WOD_HA1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_USE1A,
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
        // use_mid
        .frames = (const unsigned short[]) {
            FRAME_WOODY_USE_MID_0,
            FRAME_WOODY_USE_MID_2,
            FRAME_WOODY_USE_MID_4,
            FRAME_WOODY_USE_MID_6,
            FRAME_WOODY_USE_MID_8,
            FRAME_WOODY_USE_MID_10,
            FRAME_WOODY_USE_MID_12,
            FRAME_WOODY_USE_MID_14,
            FRAME_WOODY_USE_MID_16,
            FRAME_WOODY_USE_MID_18,
            FRAME_WOODY_USE_MID_20,
            FRAME_WOODY_USE_MID_22,
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_USE1,
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
