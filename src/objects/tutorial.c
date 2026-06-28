#include "tutorial.h"

#include <stdlib.h>

#include "../engine/fonts/intraFont.h"

extern g2dImage* SpriteAtlas_INGAMEUI;

extern intraFont* Font_BLUEHIGB_10;

extern const Frame ui_joe_frames[];
extern const Animation ui_joe_animations[];

Tutorial* tutorial_create(Phrase* phrases, int phrase_count) {
    Tutorial* tutorial = malloc(sizeof(Tutorial));
    memset(tutorial, 0, sizeof(Tutorial));

    tutorial->current_animation_index = 0;

    tutorial->animation_frame = 0;
    tutorial->animation_frame_time = 0;
    tutorial->animation_length = 0;

    tutorial->sprite_src_x = 0;
    tutorial->sprite_src_y = 0;
    tutorial->sprite_w = 0;
    tutorial->sprite_h = 0;
    tutorial->sprite_offset_x = 0;
    tutorial->sprite_offset_y = 0;

    tutorial_animation_set(tutorial, ANIMATION_UI_JOE_HEAD_TALK);

    tutorial->phrase_count = phrase_count;
    memcpy(tutorial->phrases, phrases, sizeof(Phrase) * phrase_count);

    tutorial->phrase_number = -1;
    tutorial->phrase_text_count = 0;
    tutorial->window_size = 0;
    tutorial->current_phrase = NULL;

    tutorial_update_phrase(tutorial);

    return tutorial;
}

void tutorial_update_phrase(Tutorial* tutorial) {
    tutorial->phrase_number++;

    if (tutorial->phrase_number >= tutorial->phrase_count) {
        tutorial->phrase_number = tutorial->phrase_count - 1;
        return;
    }

    tutorial->current_phrase = &tutorial->phrases[tutorial->phrase_number];
    tutorial->phrase_text_count = tutorial->current_phrase->phrase_text_count;
    tutorial->window_size = tutorial->current_phrase->window_size;
}

void tutorial_animation_update_frame(Tutorial* tutorial) {
    int animation_frame = ui_joe_animations[tutorial->current_animation_index].frames[tutorial->animation_frame];
    const Frame* frame = &ui_joe_frames[animation_frame];

    tutorial->sprite_src_x = frame->src_x;
    tutorial->sprite_src_y = frame->src_y;
    tutorial->sprite_w = frame->w;
    tutorial->sprite_h = frame->h;
    tutorial->sprite_offset_x = frame->offset_x;
    tutorial->sprite_offset_y = frame->offset_y;
}

void tutorial_animation_set(Tutorial* tutorial, int animation) {
    tutorial->current_animation_index = animation;
    tutorial->animation_frame = 0;
    tutorial->animation_frame_time = 0;
    tutorial->animation_length = ui_joe_animations[animation].frame_count;
    tutorial_animation_update_frame(tutorial);
}

void tutorial_update(Tutorial* tutorial) {
    tutorial->animation_frame_time++;
    if (tutorial->animation_frame_time == 5) {
        tutorial->animation_frame_time = 0;
        tutorial->animation_frame++;
        if (tutorial->animation_frame == tutorial->animation_length) {
            tutorial->animation_frame = 0;
        }
        tutorial_animation_update_frame(tutorial);
    }
}

void tutorial_draw(const Tutorial* tutorial) {
    // tutorial_base
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 0, 0, 480, 59, WHITE, 0, 320, 480, 59, 0, 255, G2D_UP_LEFT);

    // tutorial_center
    int draw_y = 36;

    for (int i = 0; i < tutorial->window_size; i++) {
        g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 62, draw_y, 418, 3, WHITE, 0, 379, 418, 3, 0, 255, G2D_UP_LEFT);
        draw_y += 3;
    }

    // tutorial_bottom
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 61, draw_y, 419, 16, WHITE, 0, 382, 419, 16, 0, 255, G2D_UP_LEFT);

    // Text
    intraFontSetStyle(Font_BLUEHIGB_10, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_10, 1);

    for (int i = 0; i < tutorial->phrase_text_count; i++) {
        const PhraseText* current_phrase_text = &tutorial->current_phrase->phrase_texts[i];
        intraFontPrint(Font_BLUEHIGB_10, 78, current_phrase_text->y + intraFontTextHeight(Font_BLUEHIGB_10), current_phrase_text->text);
    }

    // Joe
    g2d_DrawImageExt(SpriteAtlas_INGAMEUI, 8 + tutorial->sprite_offset_x, 2 + tutorial->sprite_offset_y, tutorial->sprite_w, tutorial->sprite_h, WHITE, tutorial->sprite_src_x, tutorial->sprite_src_y, tutorial->sprite_w, tutorial->sprite_h, 0, 255, G2D_UP_LEFT);
}

void tutorial_unload(Tutorial* tutorial) {
    free(tutorial);
}
