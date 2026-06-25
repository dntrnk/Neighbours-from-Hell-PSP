#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "../engine/graphics/g2d.h"

#include "../types/gfx_types.h"

#include "../animations/ui/joe.h"

#define MAX_PHRASES 32

typedef struct {
    char text[256];
    int y;
} PhraseText;

typedef struct {
    PhraseText phrase_texts[8];
    int phrase_text_count;
    int window_size;
} Phrase;

typedef struct {
    int current_animation_index;
    
    int animation_frame;
    int animation_frame_time;
    int animation_length;

    unsigned short sprite_src_x;
    unsigned short sprite_src_y;
    unsigned short sprite_w;
    unsigned short sprite_h;
    int sprite_offset_x;
    int sprite_offset_y;

    Phrase phrases[MAX_PHRASES];
    int phrase_number;
    int phrase_count;
    int phrase_text_count;
    int window_size;

    Phrase* current_phrase;
} Tutorial;

Tutorial* tutorial_create(Phrase* phrases, int phrase_count);
void tutorial_update_phrase(Tutorial* tutorial);
void tutorial_animation_update_frame(Tutorial* tutorial);
void tutorial_animation_set(Tutorial* tutorial, int animation);
void tutorial_update(Tutorial* tutorial);
void tutorial_draw(const Tutorial* tutorial);
void tutorial_unload(Tutorial* tutorial);

#endif // TUTORIAL_H