#ifndef G2D_H
#define G2D_H

#include "glib2d.h"

#include <stdlib.h>
#include <pspuser.h>

typedef struct {
    g2dImage* tex;
    int src_x, src_y;
    int w, h;
    int offset_x, offset_y;
} Sprite;

void g2d_FlushBatch(void);

g2dImage* g2d_LoadImage(const char* filename, g2dTexFormat pallete);
void g2d_FreeImage(g2dImage* image);

void g2d_Clear(g2dColor color);
void g2d_DrawImageExt(g2dImage* tex, int x, int y, int w, int h, g2dColor color, int srcx, int srcy, int srcw, int srch, int r, int a, g2dCoord_Mode mode);
void g2d_DrawImage(g2dImage* tex, int x, int y, g2dColor color, int r, int a, g2dCoord_Mode mode);
void g2d_DrawRectFilled(int x, int y, int w, int h, g2dColor color, int a);
void g2d_Flip(g2dFlip_Mode mode);

Sprite* g2d_CreateSprite(g2dImage* tex, int src_x, int src_y, int w, int h, int offset_x, int offset_y);
void g2d_DrawSprite(Sprite* spr, int x, int y);

#endif // G2D_H