#ifndef LOCALIZATIONS_H
#define LOCALIZATIONS_H

#include "../engine/json/json_helper.h"

typedef enum {
    LANG_ENGLISH,
    LANG_RUSSIAN
} Language;

extern Language current_lang;
extern char* current_lang_code;

void lang_set(Language new_language);

#endif // LOCALIZATIONS_H