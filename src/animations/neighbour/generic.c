#include "../../types/gfx_types.h"
#include "../../types/sfx_types.h"
#include "../../gfxdata/neighbour/generic.h"

const Animation neighbour_generic_animations[] = {
    {
        // ms0
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MS0_0
        },

        .sfx = (const short[]) {
            SOUND_NONE
        },

        .frame_count = 1,

        .loop = true
    },

    {
        // ms1
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MS1_0,
            FRAME_NEIGHBOUR_MS1_4,
            FRAME_NEIGHBOUR_MS1_8,
            FRAME_NEIGHBOUR_MS1_12,
            FRAME_NEIGHBOUR_MS1_16,
            FRAME_NEIGHBOUR_MS1_20
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 6,

        .loop = true
    },

    {
        // ms2
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MS2_0
        },

        .sfx = (const short[]) {
            SOUND_NONE
        },

        .frame_count = 1,

        .loop = true
    },

    {
        // ms3
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MS3_0,
            FRAME_NEIGHBOUR_MS3_4,
            FRAME_NEIGHBOUR_MS3_8,
            FRAME_NEIGHBOUR_MS3_12,
            FRAME_NEIGHBOUR_MS3_16,
            FRAME_NEIGHBOUR_MS3_20
        },

        .sfx = (const short[]) {
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 6,

        .loop = true
    },

    {
        // mg0
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MG0_0,
            FRAME_NEIGHBOUR_MG0_1,
            FRAME_NEIGHBOUR_MG0_2,
            FRAME_NEIGHBOUR_MG0_3,
            FRAME_NEIGHBOUR_MG0_4,
            FRAME_NEIGHBOUR_MG0_5,
            FRAME_NEIGHBOUR_MG0_6,
            FRAME_NEIGHBOUR_MG0_7
        },

        .sfx = (const short[]) {
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 8,

        .loop = true
    },

    {
        // mg1
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MG1_0,
            FRAME_NEIGHBOUR_MG1_1,
            FRAME_NEIGHBOUR_MG1_2,
            FRAME_NEIGHBOUR_MG1_3,
            FRAME_NEIGHBOUR_MG1_4,
            FRAME_NEIGHBOUR_MG1_5,
            FRAME_NEIGHBOUR_MG1_6,
            FRAME_NEIGHBOUR_MG1_7
        },

        .sfx = (const short[]) {
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 8,

        .loop = true
    },

    {
        // mg2
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MG2_0,
            FRAME_NEIGHBOUR_MG2_1,
            FRAME_NEIGHBOUR_MG2_2,
            FRAME_NEIGHBOUR_MG2_3,
            FRAME_NEIGHBOUR_MG2_4,
            FRAME_NEIGHBOUR_MG2_5,
            FRAME_NEIGHBOUR_MG2_6,
            FRAME_NEIGHBOUR_MG2_7
        },

        .sfx = (const short[]) {
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 8,

        .loop = true
    },

    {
        // mg3
        .frames = (const unsigned short[]) {
            FRAME_NEIGHBOUR_MG3_0,
            FRAME_NEIGHBOUR_MG3_1,
            FRAME_NEIGHBOUR_MG3_2,
            FRAME_NEIGHBOUR_MG3_3,
            FRAME_NEIGHBOUR_MG3_4,
            FRAME_NEIGHBOUR_MG3_5,
            FRAME_NEIGHBOUR_MG3_6,
            FRAME_NEIGHBOUR_MG3_7
        },

        .sfx = (const short[]) {
            SOUND_NA_STEP1,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NA_STEP2,
            SOUND_NONE,
            SOUND_NONE,
            SOUND_NONE
        },

        .frame_count = 8,

        .loop = true
    }

};
