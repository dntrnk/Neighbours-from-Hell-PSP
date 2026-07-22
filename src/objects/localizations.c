#include "localizations.h"

char* language_codes[] = {
    [LANG_ENGLISH] = "en",
    [LANG_RUSSIAN] = "ru"
};

Language current_lang = LANG_RUSSIAN;
char* current_lang_code = "ru";

void lang_set(Language new_language) {
    current_lang = new_language;
    current_lang_code = language_codes[current_lang];
}
