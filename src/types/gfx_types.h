#ifndef GFX_TYPES_H
#define GFX_TYPES_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    unsigned short src_x, src_y;
    short w, h;
    short offset_x, offset_y;
    unsigned short spritelist_index;
} Frame;

typedef struct {
    const unsigned short int* frames;
    const short* sfx;
    unsigned short frame_count;
    bool loop;
} Animation;

#endif // GFX_TYPES_H