#include "g2d.h"

g2dImage* g2d_LoadImage(const char* filename, g2dTexFormat pallete) {
    return g2dTexLoad(filename, NULL, 0, pallete | G2D_SWIZZLE);
}

void g2d_FreeImage(g2dImage* image) {
    if (image != NULL)
        g2dTexFree(&image);
}

void g2d_DrawImage(const g2dImage* tex, int x, int y, int w, int h, g2dColor color, int srcx, int srcy, int srcw, int srch, int r, int a, g2dCoord_Mode mode) {
    g2dBeginRects(tex);
    g2dSetCoordMode(mode);
    g2dSetCoordXY(x, y);
    g2dSetCropXY(srcx, srcy);
    g2dSetCropWH(srcw, srch);
    g2dSetScaleWH(w, h);
    g2dSetRotation(r);
    g2dSetColor(color);
    g2dSetAlpha(a);
    g2dAdd();
    g2dEnd();
}

void g2d_DrawImageEasy(const g2dImage* tex, int x, int y, g2dColor color, int r, int a, g2dCoord_Mode mode) {
    g2dBeginRects(tex);
    g2dSetCoordMode(mode);
    g2dSetCoordXY(x, y);
    g2dSetRotation(r);
    g2dSetColor(color);
    g2dSetAlpha(a);
    g2dAdd();
    g2dEnd();
}

void g2d_DrawRectFilled(int x, int y, int w, int h, g2dColor color) {
    g2dBeginRects(NULL);
    g2dSetCoordXY(x, y);
    g2dSetScaleWH(w, h);
    g2dSetColor(color);
    g2dResetAlpha();
    g2dAdd();
    g2dEnd();
}

Sprite* g2d_CreateSprite(g2dImage* tex, int src_x, int src_y, int w, int h, int offset_x, int offset_y) {
    Sprite* spr = malloc(sizeof(Sprite));
    
    if (spr == NULL)
        return NULL;
    
    spr->tex = tex;
    spr->src_x = src_x;
    spr->src_y = src_y;
    spr->w = w;
    spr->h = h;
    spr->offset_x = offset_x;
    spr->offset_y = offset_y;

    return spr;
}

void g2d_DrawSprite(const Sprite* spr, int x, int y) {
    g2dBeginRects(spr->tex);
    g2dSetCoordMode(G2D_UP_LEFT); // DEFAULT
    g2dSetTexLinear(false);
    g2dSetCoordXY(x, y);
    g2dSetCropXY(spr->src_x, spr->src_y);
    g2dSetCropWH(spr->w, spr->h);
    g2dSetScaleWH(spr->w, spr->h);
    g2dSetRotation(0); // DEFAULT
    g2dSetColor(WHITE); // DEFAULT
    g2dSetAlpha(255); // DEFAULT
    g2dAdd();
    g2dEnd();
}
