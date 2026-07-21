#ifndef LOCALIZATIONS_H
#define LOCALIZATIONS_H

#include "../engine/json/json_helper.h"

typedef enum {
    LANG_RUSSIAN,
    LANG_ENGLISH
} Language;

extern Language current_lang;
extern char* current_lang_code;

#endif // LOCALIZATIONS_H