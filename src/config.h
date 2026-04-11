/**
 * @file config.h
 * @brief 配置管理头文件 - 负责加载和管理程序的颜色配置
 * 
 * 本模块提供以下功能：
 * - 从 JSON 配置文件加载颜色配置
 * - 提供默认配置
 * - 管理配置的生命周期（初始化和释放）
 * 
 * 配置文件格式示例：
 * {
 *   "color_mode": "ansi",  // 或 "256" 或 "rgb"
 *   "JSON_STRING": {
 *     "key": {"fg": "green", "bg": "none", "style": "normal"},
 *     "value": {"fg": "yellow", "bg": "none", "style": "normal"}
 *   },
 *   "tree": {"fg": "blue", "bg": "none", "style": "normal"}
 * }
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>

/**
 * @brief 颜色模式枚举
 * 
 * 定义程序支持的三种颜色模式：
 * - ANSI: 标准16色（black, red, green, yellow, blue, magenta, cyan, white）
 * - 256: 256色模式（0-255索引）
 * - RGB: 真彩色模式（RGB值）
 */
typedef enum {
    COLOR_MODE_ANSI = 0,  // ANSI 16色模式
    COLOR_MODE_256 = 1,   // 256色模式
    COLOR_MODE_RGB = 2    // RGB真彩色模式
} ColorMode;

/**
 * @brief 颜色配置结构体
 * 
 * 存储单个颜色配置，包含前景色、背景色和样式
 */
typedef struct {
    int fg;    //*< 前景色代码（ANSI: 30-37, 90-97; 256: 0-255; RGB: [r,g,b]） 
    int bg;    //*< 背景色代码（ANSI: 40-47, 100-107; 256: 0-255; RGB: [r,g,b]） 
    int style; //*< 样式代码（0-9: normal, bold, dim, italic, underline, blink, reverse, hidden） 
} ColorConfig;

/**
 * @brief 类型颜色配置结构体
 * 
 * 存储某个 JSON 类型的键和值的颜色配置
 */
typedef struct {
    ColorConfig key;    //*< 键名/索引的颜色配置 
    ColorConfig value;  //*< 值的颜色配置 
} TypeColorConfig;

/**
 * @brief 程序配置结构体
 * 
 * 存储整个程序的所有配置信息
 */
typedef struct {
    ColorMode color_mode;          //*< 颜色模式（ansi/256/rgb） 
    TypeColorConfig json_null;     //*< JSON_NULL 类型的颜色配置 
    TypeColorConfig json_bool;     //*< JSON_BOOL 类型的颜色配置 
    TypeColorConfig json_number;   //*< JSON_NUMBER 类型的颜色配置 
    TypeColorConfig json_string;   //*< JSON_STRING 类型的颜色配置 
    TypeColorConfig json_object;   //*< JSON_OBJECT 类型的颜色配置 
    TypeColorConfig json_array;    //*< JSON_ARRAY 类型的颜色配置 
    ColorConfig tree;              //*< 树形符号（│、├──、└──）的颜色配置 
} ProgramConfig;

/**
 * @brief 从配置文件加载配置
 * 
 * @param filename 配置文件路径
 * @param config 输出配置结构体指针
 * @return int 成功返回0，失败返回-1
 * 
 * @note 如果配置文件不存在或格式错误，会返回-1
 * @note 调用者应该检查返回值，失败时可以使用 config_init_default 初始化默认配置
 */
int config_load(const char* filename, ProgramConfig* config);

/**
 * @brief 释放配置资源
 * 
 * @param config 要释放的配置结构体指针
 * 
 * @note 当前实现只是清零内存，没有动态分配的资源
 * @note 保留此函数以备将来扩展
 */
void config_free(ProgramConfig* config);

/**
 * @brief 初始化默认配置
 * 
 * @param config 要初始化的配置结构体指针
 * 
 * @note 设置所有类型的默认颜色和样式
 * @note 默认配置使用 ANSI 16色模式
 */
void config_init_default(ProgramConfig* config);

/**
 * @brief 获取默认配置
 * 
 * @return ProgramConfig* 指向静态默认配置的指针
 * 
 * @note 返回的是静态分配的内存，不需要释放
 * @note 可以直接用作配置加载失败时的后备配置
 */
ProgramConfig* config_get_default(void);

/**
 * @brief 生成默认配置文件
 * 
 * @param filename 要生成的配置文件路径
 * @return int 成功返回0，失败返回-1
 * 
 * @note 会覆盖已存在的文件
 * @note 生成的配置文件使用 ANSI 16色模式和默认颜色值
 * @note 文件格式为 JSON，包含所有类型的颜色配置
 */
int config_generate_default_file(const char* filename);

#endif // CONFIG_H 