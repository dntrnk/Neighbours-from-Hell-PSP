#include "json_helper.h"

#include "../../objects/scene_manager.h"

#include <stdio.h>
#include <string.h>

extern Scene ErrorScreenScene;

char* json_get_item_string(const cJSON* const parsed_json, const char* key, int max_length) {
    cJSON* item = cJSON_GetObjectItem(parsed_json, key);

    if (!item) {
        scene_error("Ключ \"%s\" не найден", key);
    } else if (!cJSON_IsString(item)) {
        scene_error("Значение \"%s\" должно быть строкой", key);
    } else {
        if (strlen(item->valuestring) > max_length) {
            scene_error("Строка \"%s\" слишком длинная\n\n\n\nМаксимальная длина: %d", key, max_length);
        } else {
            return item->valuestring;
        }
    }

    return NULL;
}

cJSON* json_get_item_object(const cJSON* const parsed_json, const char* key) {
    cJSON* item = cJSON_GetObjectItem(parsed_json, key);

    if (!item) {
        scene_error("Ключ \"%s\" не найден", key);
    } else if (!cJSON_IsObject(item)) {
        scene_error("Значение \"%s\" должно быть объектом", key);
    } else {
        return item;
    }

    return NULL;
}

cJSON* json_get_item_array(const cJSON* const parsed_json, const char* key) {
    cJSON* item = cJSON_GetObjectItem(parsed_json, key);

    if (!item) {
        scene_error("Ключ \"%s\" не найден", key);
    } else if (!cJSON_IsArray(item)) {
        scene_error("Значение \"%s\" должно быть массивом", key);
    } else {
        return item;
    }

    return NULL;
}

RoomID string_to_room_type(const char* room) {
    if (strcmp(room, "ROOM_KIT") == 0) return ROOM_KIT;
    if (strcmp(room, "ROOM_LIR") == 0) return ROOM_LIR;
    if (strcmp(room, "ROOM_ANC") == 0) return ROOM_ANC;
    if (strcmp(room, "ROOM_FRO") == 0) return ROOM_FRO;
    if (strcmp(room, "ROOM_TOI") == 0) return ROOM_TOI;
    
    scene_error("Комнаты \"%s\" не существует", room);

    return 0;
}

int json_get_item_number(const cJSON* const parsed_json, const char* key) {
    cJSON* item = cJSON_GetObjectItem(parsed_json, key);

    if (!item) {
        scene_error("Ключ \"%s\" не найден", key);
    } else if (!cJSON_IsNumber(item)) {
        scene_error("Значение \"%s\" должно быть числом", key);
    } else {
        return item->valueint;
    }

    return 0;
}
