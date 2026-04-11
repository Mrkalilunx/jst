#include "config.h"
#include "cJSON.h"
#include "clr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int color_name_to_code(const char* color_name) {
    if (!color_name) return CLR_DEFAULT;
    
    // 基本颜色
    if (strcmp(color_name, "black") == 0) return CLR_BLACK;
    if (strcmp(color_name, "red") == 0) return CLR_RED;
    if (strcmp(color_name, "green") == 0) return CLR_GREEN;
    if (strcmp(color_name, "yellow") == 0) return CLR_YELLOW;
    if (strcmp(color_name, "blue") == 0) return CLR_BLUE;
    if (strcmp(color_name, "magenta") == 0) return CLR_MAGENTA;
    if (strcmp(color_name, "cyan") == 0) return CLR_CYAN;
    if (strcmp(color_name, "white") == 0) return CLR_WHITE;
    
    // 亮色
    if (strcmp(color_name, "bright_black") == 0) return CLR_BRIGHT_BLACK;
    if (strcmp(color_name, "bright_red") == 0) return CLR_BRIGHT_RED;
    if (strcmp(color_name, "bright_green") == 0) return CLR_BRIGHT_GREEN;
    if (strcmp(color_name, "bright_yellow") == 0) return CLR_BRIGHT_YELLOW;
    if (strcmp(color_name, "bright_blue") == 0) return CLR_BRIGHT_BLUE;
    if (strcmp(color_name, "bright_magenta") == 0) return CLR_BRIGHT_MAGENTA;
    if (strcmp(color_name, "bright_cyan") == 0) return CLR_BRIGHT_CYAN;
    if (strcmp(color_name, "bright_white") == 0) return CLR_BRIGHT_WHITE;
    
    // 背景色
    if (strcmp(color_name, "none") == 0) return CLR_BG_DEFAULT;
    
    return CLR_DEFAULT;
}

static int style_name_to_code(const char* style_name) {
    if (!style_name) return CLR_NORMAL;
    
    if (strcmp(style_name, "normal") == 0) return CLR_NORMAL;
    if (strcmp(style_name, "bold") == 0) return CLR_BOLD;
    if (strcmp(style_name, "dim") == 0) return CLR_DIM;
    if (strcmp(style_name, "italic") == 0) return CLR_ITALIC;
    if (strcmp(style_name, "underline") == 0) return CLR_UNDERLINE;
    if (strcmp(style_name, "blink") == 0) return CLR_BLINK;
    if (strcmp(style_name, "reverse") == 0) return CLR_REVERSE;
    if (strcmp(style_name, "hidden") == 0) return CLR_HIDDEN;
    
    return CLR_NORMAL;
}

static int color_mode_name_to_code(const char* mode_name) {
    if (!mode_name) return COLOR_MODE_ANSI;
    
    if (strcmp(mode_name, "ansi") == 0) return COLOR_MODE_ANSI;
    if (strcmp(mode_name, "256") == 0) return COLOR_MODE_256;
    if (strcmp(mode_name, "rgb") == 0) return COLOR_MODE_RGB;
    
    return COLOR_MODE_ANSI;
}

static int parse_color_config(cJSON* json, ColorConfig* config) {
    if (!json || !config) return -1;
    
    cJSON* fg_item = cJSON_GetObjectItem(json, "fg");
    cJSON* bg_item = cJSON_GetObjectItem(json, "bg");
    cJSON* style_item = cJSON_GetObjectItem(json, "style");
    
    if (fg_item && cJSON_IsString(fg_item)) {
        config->fg = color_name_to_code(fg_item->valuestring);
    } else {
        config->fg = CLR_DEFAULT;
    }
    
    if (bg_item && cJSON_IsString(bg_item)) {
        config->bg = color_name_to_code(bg_item->valuestring) + 10;  // 背景色代码 = 前景色 + 10
    } else {
        config->bg = CLR_BG_DEFAULT;
    }
    
    if (style_item && cJSON_IsString(style_item)) {
        config->style = style_name_to_code(style_item->valuestring);
    } else {
        config->style = CLR_NORMAL;
    }
    
    return 0;
}

static int parse_type_color_config(cJSON* json, TypeColorConfig* config) {
    if (!json || !config) return -1;
    
    cJSON* key_item = cJSON_GetObjectItem(json, "key");
    cJSON* value_item = cJSON_GetObjectItem(json, "value");
    
    if (key_item) {
        parse_color_config(key_item, &config->key);
    }
    
    if (value_item) {
        parse_color_config(value_item, &config->value);
    }
    
    return 0;
}

int config_load(const char* filename, ProgramConfig* config) {
    if (!filename || !config) return -1;
    
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer) {
        fclose(fp);
        return -1;
    }
    
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);
    
    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    
    if (!root) return -1;
    
    memset(config, 0, sizeof(ProgramConfig));
    
    cJSON* color_mode_item = cJSON_GetObjectItem(root, "color_mode");
    if (color_mode_item && cJSON_IsString(color_mode_item)) {
        config->color_mode = color_mode_name_to_code(color_mode_item->valuestring);
    } else {
        config->color_mode = COLOR_MODE_ANSI;
    }
    
    cJSON* json_null_item = cJSON_GetObjectItem(root, "JSON_NULL");
    if (json_null_item) {
        parse_type_color_config(json_null_item, &config->json_null);
    }
    
    cJSON* json_bool_item = cJSON_GetObjectItem(root, "JSON_BOOL");
    if (json_bool_item) {
        parse_type_color_config(json_bool_item, &config->json_bool);
    }
    
    cJSON* json_number_item = cJSON_GetObjectItem(root, "JSON_NUMBER");
    if (json_number_item) {
        parse_type_color_config(json_number_item, &config->json_number);
    }
    
    cJSON* json_string_item = cJSON_GetObjectItem(root, "JSON_STRING");
    if (json_string_item) {
        parse_type_color_config(json_string_item, &config->json_string);
    }
    
    cJSON* json_object_item = cJSON_GetObjectItem(root, "JSON_OBJECT");
    if (json_object_item) {
        parse_type_color_config(json_object_item, &config->json_object);
    }
    
    cJSON* json_array_item = cJSON_GetObjectItem(root, "JSON_ARRAY");
    if (json_array_item) {
        parse_type_color_config(json_array_item, &config->json_array);
    }
    
    cJSON* tree_item = cJSON_GetObjectItem(root, "tree");
    if (tree_item) {
        parse_color_config(tree_item, &config->tree);
    }
    
    cJSON_Delete(root);
    return 0;
}

void config_free(ProgramConfig* config) {
    if (!config) return;
    memset(config, 0, sizeof(ProgramConfig));
}

void config_init_default(ProgramConfig* config) {
    if (!config) return;
    
    config->color_mode = COLOR_MODE_ANSI;
    
    // JSON_NULL: 红色
    config->json_null.key.fg = CLR_RED;
    config->json_null.key.bg = CLR_BG_DEFAULT;
    config->json_null.key.style = CLR_NORMAL;
    config->json_null.value.fg = CLR_RED;
    config->json_null.value.bg = CLR_BG_DEFAULT;
    config->json_null.value.style = CLR_NORMAL;
    
    // JSON_BOOL: 品红
    config->json_bool.key.fg = CLR_MAGENTA;
    config->json_bool.key.bg = CLR_BG_DEFAULT;
    config->json_bool.key.style = CLR_NORMAL;
    config->json_bool.value.fg = CLR_MAGENTA;
    config->json_bool.value.bg = CLR_BG_DEFAULT;
    config->json_bool.value.style = CLR_NORMAL;
    
    // JSON_NUMBER: 蓝色
    config->json_number.key.fg = CLR_BLUE;
    config->json_number.key.bg = CLR_BG_DEFAULT;
    config->json_number.key.style = CLR_NORMAL;
    config->json_number.value.fg = CLR_BLUE;
    config->json_number.value.bg = CLR_BG_DEFAULT;
    config->json_number.value.style = CLR_NORMAL;
    
    // JSON_STRING: 键绿色，值黄色
    config->json_string.key.fg = CLR_GREEN;
    config->json_string.key.bg = CLR_BG_DEFAULT;
    config->json_string.key.style = CLR_NORMAL;
    config->json_string.value.fg = CLR_YELLOW;
    config->json_string.value.bg = CLR_BG_DEFAULT;
    config->json_string.value.style = CLR_NORMAL;
    
    // JSON_OBJECT: 键粗体青色，值青色
    config->json_object.key.fg = CLR_CYAN;
    config->json_object.key.bg = CLR_BG_DEFAULT;
    config->json_object.key.style = CLR_BOLD;
    config->json_object.value.fg = CLR_CYAN;
    config->json_object.value.bg = CLR_BG_DEFAULT;
    config->json_object.value.style = CLR_NORMAL;
    
    // JSON_ARRAY: 键粗体青色，值青色
    config->json_array.key.fg = CLR_CYAN;
    config->json_array.key.bg = CLR_BG_DEFAULT;
    config->json_array.key.style = CLR_BOLD;
    config->json_array.value.fg = CLR_CYAN;
    config->json_array.value.bg = CLR_BG_DEFAULT;
    config->json_array.value.style = CLR_NORMAL;
    
    // Tree: 蓝色
    config->tree.fg = CLR_BLUE;
    config->tree.bg = CLR_BG_DEFAULT;
    config->tree.style = CLR_NORMAL;
}

ProgramConfig* config_get_default(void) {
    static ProgramConfig default_config = {0};
    static int initialized = 0;
    
    if (!initialized) {
        config_init_default(&default_config);
        initialized = 1;
    }
    
    return &default_config;
}

/**
 * @brief 辅助函数：创建颜色配置的 JSON 对象
 */
static cJSON* create_color_json(int fg, int bg, int style) {
    cJSON* color_obj = cJSON_CreateObject();
    if (!color_obj) return NULL;
    
    // 将颜色代码转换为名称
    const char* fg_name = "default";
    const char* bg_name = "none";
    const char* style_name = "normal";
    
    // 前景色名称映射
    switch (fg) {
        case CLR_BLACK: fg_name = "black"; break;
        case CLR_RED: fg_name = "red"; break;
        case CLR_GREEN: fg_name = "green"; break;
        case CLR_YELLOW: fg_name = "yellow"; break;
        case CLR_BLUE: fg_name = "blue"; break;
        case CLR_MAGENTA: fg_name = "magenta"; break;
        case CLR_CYAN: fg_name = "cyan"; break;
        case CLR_WHITE: fg_name = "white"; break;
        case CLR_BRIGHT_BLACK: fg_name = "bright_black"; break;
        case CLR_BRIGHT_RED: fg_name = "bright_red"; break;
        case CLR_BRIGHT_GREEN: fg_name = "bright_green"; break;
        case CLR_BRIGHT_YELLOW: fg_name = "bright_yellow"; break;
        case CLR_BRIGHT_BLUE: fg_name = "bright_blue"; break;
        case CLR_BRIGHT_MAGENTA: fg_name = "bright_magenta"; break;
        case CLR_BRIGHT_CYAN: fg_name = "bright_cyan"; break;
        case CLR_BRIGHT_WHITE: fg_name = "bright_white"; break;
        default: fg_name = "default"; break;
    }
    
    // 背景色名称映射
    switch (bg) {
        case CLR_BG_BLACK: bg_name = "black"; break;
        case CLR_BG_RED: bg_name = "red"; break;
        case CLR_BG_GREEN: bg_name = "green"; break;
        case CLR_BG_YELLOW: bg_name = "yellow"; break;
        case CLR_BG_BLUE: bg_name = "blue"; break;
        case CLR_BG_MAGENTA: bg_name = "magenta"; break;
        case CLR_BG_CYAN: bg_name = "cyan"; break;
        case CLR_BG_WHITE: bg_name = "white"; break;
        default: bg_name = "none"; break;
    }
    
    // 样式名称映射
    switch (style) {
        case CLR_BOLD: style_name = "bold"; break;
        case CLR_DIM: style_name = "dim"; break;
        case CLR_ITALIC: style_name = "italic"; break;
        case CLR_UNDERLINE: style_name = "underline"; break;
        case CLR_BLINK: style_name = "blink"; break;
        case CLR_REVERSE: style_name = "reverse"; break;
        case CLR_HIDDEN: style_name = "hidden"; break;
        default: style_name = "normal"; break;
    }
    
    cJSON_AddStringToObject(color_obj, "fg", fg_name);
    cJSON_AddStringToObject(color_obj, "bg", bg_name);
    cJSON_AddStringToObject(color_obj, "style", style_name);
    
    return color_obj;
}

/**
 * @brief 辅助函数：创建类型颜色配置的 JSON 对象
 */
static cJSON* create_type_color_json(TypeColorConfig* config) {
    cJSON* type_obj = cJSON_CreateObject();
    if (!type_obj) return NULL;
    
    cJSON* key_obj = create_color_json(config->key.fg, config->key.bg, config->key.style);
    cJSON* value_obj = create_color_json(config->value.fg, config->value.bg, config->value.style);
    
    if (key_obj) cJSON_AddItemToObject(type_obj, "key", key_obj);
    if (value_obj) cJSON_AddItemToObject(type_obj, "value", value_obj);
    
    return type_obj;
}

int config_generate_default_file(const char* filename) {
    if (!filename) return -1;
    
    // 获取默认配置
    ProgramConfig* default_config = config_get_default();
    if (!default_config) return -1;
    
    // 创建根对象
    cJSON* root = cJSON_CreateObject();
    if (!root) return -1;
    
    // 添加颜色模式
    cJSON_AddStringToObject(root, "color_mode", "ansi");
    
    // 添加各类型的颜色配置
    cJSON* json_null_obj = create_type_color_json(&default_config->json_null);
    if (json_null_obj) cJSON_AddItemToObject(root, "JSON_NULL", json_null_obj);
    
    cJSON* json_bool_obj = create_type_color_json(&default_config->json_bool);
    if (json_bool_obj) cJSON_AddItemToObject(root, "JSON_BOOL", json_bool_obj);
    
    cJSON* json_number_obj = create_type_color_json(&default_config->json_number);
    if (json_number_obj) cJSON_AddItemToObject(root, "JSON_NUMBER", json_number_obj);
    
    cJSON* json_string_obj = create_type_color_json(&default_config->json_string);
    if (json_string_obj) cJSON_AddItemToObject(root, "JSON_STRING", json_string_obj);
    
    cJSON* json_object_obj = create_type_color_json(&default_config->json_object);
    if (json_object_obj) cJSON_AddItemToObject(root, "JSON_OBJECT", json_object_obj);
    
    cJSON* json_array_obj = create_type_color_json(&default_config->json_array);
    if (json_array_obj) cJSON_AddItemToObject(root, "JSON_ARRAY", json_array_obj);
    
    // 添加树形符号颜色配置
    cJSON* tree_obj = create_color_json(default_config->tree.fg, default_config->tree.bg, default_config->tree.style);
    if (tree_obj) cJSON_AddItemToObject(root, "tree", tree_obj);
    
    // 生成 JSON 字符串
    char* json_str = cJSON_Print(root);
    cJSON_Delete(root);
    
    if (!json_str) return -1;
    
    // 写入文件
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        free(json_str);
        return -1;
    }
    
    fprintf(fp, "%s\n", json_str);
    fclose(fp);
    free(json_str);
    
    return 0;
}