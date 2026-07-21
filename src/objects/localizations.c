#include "localizations.h"

const char* language_codes[] = {
    [LANG_RUSSIAN] = "ru",
    [LANG_ENGLISH] = "en"
};

Language current_lang = LANG_RUSSIAN;
char* current_lang_code = "ru";
