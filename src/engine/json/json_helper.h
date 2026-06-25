#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "cJSON.h"

#include "../../types/room_types.h"

char* json_get_item_string(const cJSON* const parsed_json, const char* const key, int max_length);
cJSON* json_get_item_object(const cJSON* const parsed_json, const char* const key);
cJSON* json_get_item_array(const cJSON* const parsed_json, const char* const key);
RoomID string_to_room_type(const char* room);
int json_get_item_number(const cJSON* const parsed_json, const char* const key);

#endif // JSON_HELPER_H