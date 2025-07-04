#include "color.h"
#include <stdio.h>

ColorMode current_color_mode = COLOR_MODE_SIMPLIFIED;

const char* get_color_key(cJSON* item) {
    if (current_color_mode != COLOR_MODE_RICH) {
        return COLOR_SIMPLIFIED_KEY;
    }

    if (cJSON_IsArray(item)) return COLOR_RICH_ARRAY_KEY;
    if (cJSON_IsObject(item)) return COLOR_RICH_OBJECT_KEY;
    if (cJSON_IsString(item)) return COLOR_RICH_STRING_KEY;
    if (cJSON_IsNumber(item)) return COLOR_RICH_NUMBER_KEY;
    if (cJSON_IsBool(item)) return COLOR_RICH_BOOL_KEY;
    if (cJSON_IsNull(item)) return COLOR_RICH_NULL_KEY;
    
    return "";
}

const char* get_color_value(cJSON* item) {
    if (current_color_mode != COLOR_MODE_RICH) {
        return "";
    }

    if (cJSON_IsString(item)) return COLOR_RICH_STRING_VALUE;
    if (cJSON_IsNumber(item)) return COLOR_RICH_NUMBER_VALUE;
    if (cJSON_IsBool(item)) return COLOR_RICH_BOOL_VALUE;
    if (cJSON_IsNull(item)) return COLOR_RICH_NULL_VALUE;
    
    return "";
}

const char* get_color_reset() {
    return (current_color_mode != COLOR_MODE_NONE) ? COLOR_RESET : "";
}