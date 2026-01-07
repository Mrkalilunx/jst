#ifndef COLOR_H
#define COLOR_H

#include "cJSON.h"

typedef enum {
    COLOR_MODE_SIMPLIFIED = 0,
    COLOR_MODE_RICH,
    COLOR_MODE_NONE
} ColorMode;

// Simplified mode colors - clean and subtle
#define COLOR_SIMPLIFIED_KEY      "\033[38;5;39m"      // Bright blue for keys
#define COLOR_SIMPLIFIED_STRING   "\033[38;5;120m"     // Soft green for strings
#define COLOR_SIMPLIFIED_NUMBER   "\033[38;5;228m"     // Warm yellow for numbers
#define COLOR_SIMPLIFIED_BOOL     "\033[38;5;213m"     // Soft magenta for booleans
#define COLOR_SIMPLIFIED_NULL     "\033[38;5;245m"     // Gray for null
#define COLOR_SIMPLIFIED_INDEX    "\033[38;5;151m"     // Soft cyan for array indices
#define COLOR_SIMPLIFIED_TYPE     "\033[38;5;244m"     // Subtle gray for type info

// Rich mode colors - carefully designed palette
// Keys - cool tones
#define COLOR_RICH_OBJECT_KEY     "\033[38;5;33m"      // Deep blue for object keys
#define COLOR_RICH_ARRAY_KEY      "\033[38;5;38m"      // Cyan for array keys
#define COLOR_RICH_STRING_KEY     "\033[38;5;75m"      // Light blue for string keys
#define COLOR_RICH_NUMBER_KEY     "\033[38;5;99m"      // Purple for number keys
#define COLOR_RICH_BOOL_KEY       "\033[38;5;140m"     // Muted magenta for bool keys
#define COLOR_RICH_NULL_KEY       "\033[38;5;245m"     // Gray for null keys

// Values - warm and vibrant tones
#define COLOR_RICH_STRING_VALUE   "\033[38;5;42m"      // Bright green for strings
#define COLOR_RICH_NUMBER_VALUE   "\033[38;5;208m"     // Orange for numbers
#define COLOR_RICH_BOOL_VALUE     "\033[38;5;196m"     // Red for booleans
#define COLOR_RICH_NULL_VALUE     "\033[38;5;241m"     // Dark gray for null
#define COLOR_RICH_INDEX          "\033[38;5;45m"      // Bright cyan for array indices
#define COLOR_RICH_TYPE           "\033[38;5;244m"     // Subtle gray for type info

#define COLOR_RESET               "\033[0m"

extern ColorMode current_color_mode;

const char* get_color_key(cJSON* item);
const char* get_color_value(cJSON* item);
const char* get_color_index();
const char* get_color_type();
const char* get_color_reset();

#endif