#ifndef LEVEL_END_H
#define LEVEL_END_H

#include "../engine/graphics/g2d.h"

#define BUTTON_RESTART 0
#define BUTTON_OK 1

typedef struct {
    int counter;
    int selected_button;
    int clicked_button;

    char end_text[64];
    
    char tricks_text[32];
    char tv_rating_text[32];

    g2dColor buttons_colors[2];
} LevelEnd;

LevelEnd* level_end_create(void);
void level_end_update(LevelEnd* level_end);
void level_end_draw(const LevelEnd* level_end);
void level_end_unload(LevelEnd* level_end);

#endif // LEVEL_END_H