#include "../../objects/scene_manager.h"

#include "../../engine/graphics/g2d.h"
#include "../../engine/fonts/intraFont.h"
#include "../../engine/controls/controls.h"

#include "../../objects/localizations.h"

extern Scene MadeByDntrnkScene;

extern intraFont* Font_BLUEHIGB_10;
extern intraFont* Font_BLUEHIGB_18;

static int i;

static char title[64];
static char line1[128];
static char line2[128];
static char line3[128];
static char line4[128];
static char line5[128];
static char line6[128];
static char line7[128];

static void init(void) {
    char loc_filename[512];
    sprintf(loc_filename, "data/localizations/%s/disclaimer.json", current_lang_code);

    FILE* loc_file = fopen(loc_filename, "r");

    if (!loc_file) {
        scene_error("Не удалось открыть файл %s", loc_filename);
    }

    fseek(loc_file, 0, SEEK_END);
    long size = ftell(loc_file);
    fseek(loc_file, 0, SEEK_SET);
    char* json = malloc(size + 1);
    fread(json, 1, size, loc_file);
    json[size] = '\0';
    fclose(loc_file);
    cJSON* parsed_loc_json = cJSON_Parse(json);
    free(json);

    i = 0;

    strcpy(title, json_get_item_string(parsed_loc_json, "title", 63));
    strcpy(line1, json_get_item_string(parsed_loc_json, "line1", 127));
    strcpy(line2, json_get_item_string(parsed_loc_json, "line2", 127));
    strcpy(line3, json_get_item_string(parsed_loc_json, "line3", 127));
    strcpy(line4, json_get_item_string(parsed_loc_json, "line4", 127));
    strcpy(line5, json_get_item_string(parsed_loc_json, "line5", 127));
    strcpy(line6, json_get_item_string(parsed_loc_json, "line6", 127));
    strcpy(line7, json_get_item_string(parsed_loc_json, "line7", 127));

    cJSON_Delete(parsed_loc_json);
}

static void update(void) {
    controls_read();

    i++;

    if (controls_pressed(PSP_CTRL_CROSS)) {
        i = 616;
    }

    if (i >= 634) {
        scene_change(&MadeByDntrnkScene);
    }
}

static void draw(void) {
    g2d_Clear(BLACK);

    intraFontSetStyle(Font_BLUEHIGB_18, 1, RED, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);
    intraFontPrint(Font_BLUEHIGB_18, 190, 24 + intraFontTextHeight(Font_BLUEHIGB_18), title);

    intraFontSetStyle(Font_BLUEHIGB_18, 1, WHITE, 0, 0, INTRAFONT_ALIGN_LEFT);
    intraFontActivate(Font_BLUEHIGB_18, 1);
    intraFontPrint(Font_BLUEHIGB_18, 16, 64 + intraFontTextHeight(Font_BLUEHIGB_18), line1);
    intraFontPrint(Font_BLUEHIGB_18, 16, 86 + intraFontTextHeight(Font_BLUEHIGB_18), line2);
    intraFontPrint(Font_BLUEHIGB_18, 16, 102 + intraFontTextHeight(Font_BLUEHIGB_18), line3);
    intraFontPrint(Font_BLUEHIGB_18, 16, 124 + intraFontTextHeight(Font_BLUEHIGB_18), line4);
    intraFontPrint(Font_BLUEHIGB_18, 16, 146 + intraFontTextHeight(Font_BLUEHIGB_18), line5);
    intraFontPrint(Font_BLUEHIGB_18, 16, 162 + intraFontTextHeight(Font_BLUEHIGB_18), line6);

    intraFontPrint(Font_BLUEHIGB_18, 16, 228 + intraFontTextHeight(Font_BLUEHIGB_18), line7);
    intraFontPrint(Font_BLUEHIGB_18, 16, 244 + intraFontTextHeight(Font_BLUEHIGB_18), "dntrnk.dev@gmail.com");

    g2d_DrawRectFilled(0, 268, (int) (i * 0.779f), 4, WHITE, 255); // progressbar

    if (i <= 17) {
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK, 255 - (i * 15));
    } else if (i >= 616) {
        g2d_DrawRectFilled(0, 0, 480, 272, BLACK, (i - 616) * 15);
    }

    g2d_Flip(G2D_VSYNC);
}

Scene DisclaimerScene = {
    .init = init,
    .update = update,
    .draw = draw,
    .unload = NULL
};
