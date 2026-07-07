#include "g2d.h"

static struct {
    g2dImage* current_tex;
    bool is_batching;
} _batch = {
    .current_tex = NULL,
    .is_batching = false
};

void g2d_FlushBatch(void) {
    if (_batch.is_batching) {
        g2dEnd();
        _batch.current_tex = NULL;
        _batch.is_batching = false;
    }
}

static void _check_batch(g2dImage* tex) {
    if (_batch.is_batching && _batch.current_tex != tex) {
        g2d_FlushBatch();
    }
    
    if (!_batch.is_batching) {
        g2dBeginRects(tex);
        _batch.current_tex = tex;
        _batch.is_batching = true;
    }
}

g2dImage* g2d_LoadImage(const char* filename, g2dTexFormat pallete) {
    return g2dTexLoad(filename, NULL, 0, pallete | G2D_SWIZZLE);
}

void g2d_FreeImage(g2dImage* image) {
    if (image)
        g2dTexFree(&image);
}

void g2d_Clear(g2dColor color) {
    g2d_FlushBatch();
    _batch.current_tex = NULL;
    _batch.is_batching = false;
    g2dClear(color);
}

void g2d_DrawImageExt(g2dImage* tex, int x, int y, int w, int h, g2dColor color, int srcx, int srcy, int srcw, int srch, int r, int a, g2dCoord_Mode mode) {
    _check_batch(tex);
    g2dReset();
    g2dSetCoordMode(mode);
    g2dSetCoordXY(x, y);
    g2dSetCropXY(srcx, srcy);
    g2dSetCropWH(srcw, srch);
    g2dSetScaleWH(w, h);
    if (r != 0) g2dSetRotation(r);
    if (color != WHITE) g2dSetColor(color);
    if (a != 255) g2dSetAlpha(a);
    g2dAdd();
}

void g2d_DrawImage(g2dImage* tex, int x, int y, g2dColor color, int r, int a, g2dCoord_Mode mode) {
    _check_batch(tex);
    g2dReset();
    g2dSetCoordMode(mode);
    g2dSetCoordXY(x, y);
    g2dSetCropXY(0, 0);
    g2dSetCropWH(tex->w, tex->h);
    g2dSetScaleWH(tex->w, tex->h);
    if (r != 0) g2dSetRotation(r);
    if (color != WHITE) g2dSetColor(color);
    if (a != 255) g2dSetAlpha(a);
    g2dAdd();
}

void g2d_DrawRectFilled(int x, int y, int w, int h, g2dColor color, int a) {
    _check_batch(NULL);
    g2dReset();
    g2dSetCoordXY(x, y);
    g2dSetScaleWH(w, h);
    g2dSetColor(color);
    if (a != 255) g2dSetAlpha(a);
    g2dAdd();
}

void g2d_Flip(g2dFlip_Mode mode) {
    g2d_FlushBatch();
    g2dFlip(mode);
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

void g2d_DrawSprite(Sprite* spr, int x, int y) {
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
