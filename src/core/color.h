#ifndef COLOR_H
#define COLOR_H

#include "cJSON.h"

typedef enum {
    COLOR_MODE_SIMPLIFIED = 0,
    COLOR_MODE_RICH,
    COLOR_MODE_NONE
} ColorMode;

#define COLOR_SIMPLIFIED_KEY      "\033[38;5;141m"

#define COLOR_RICH_ARRAY_KEY      "\033[38;5;208m"
#define COLOR_RICH_OBJECT_KEY     "\033[38;5;33m"
#define COLOR_RICH_STRING_KEY     "\033[38;5;34m"
#define COLOR_RICH_NUMBER_KEY     "\033[38;5;99m"
#define COLOR_RICH_BOOL_KEY       "\033[38;5;160m"
#define COLOR_RICH_NULL_KEY       "\033[38;5;245m"

#define COLOR_RICH_STRING_VALUE   "\033[38;5;42m"
#define COLOR_RICH_NUMBER_VALUE   "\033[38;5;141m"
#define COLOR_RICH_BOOL_VALUE     "\033[38;5;203m"
#define COLOR_RICH_NULL_VALUE     "\033[38;5;247m"

#define COLOR_RESET               "\033[0m"

extern ColorMode current_color_mode;

const char* get_color_key(cJSON* item);
const char* get_color_value(cJSON* item);
const char* get_color_reset();

#endif