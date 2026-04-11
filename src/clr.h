/**
 * @file clr.h
 * @brief 终端ANSI颜色样式定义
 */

#ifndef CLR_H
#define CLR_H

#include <stdint.h>
#include <unistd.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ANSI颜色代码
 */
typedef enum {
    CLR_BLACK   = 30,    // 黑色前景
    CLR_RED     = 31,    // 红色前景
    CLR_GREEN   = 32,    // 绿色前景
    CLR_YELLOW  = 33,    // 黄色前景
    CLR_BLUE    = 34,    // 蓝色前景
    CLR_MAGENTA = 35,    // 品红前景
    CLR_CYAN    = 36,    // 青色前景
    CLR_WHITE   = 37,    // 白色前景
    CLR_DEFAULT = 39,    // 默认前景
    
    CLR_BRIGHT_BLACK   = 90,    // 亮黑色前景
    CLR_BRIGHT_RED     = 91,    // 亮红色前景
    CLR_BRIGHT_GREEN   = 92,    // 亮绿色前景
    CLR_BRIGHT_YELLOW  = 93,    // 亮黄色前景
    CLR_BRIGHT_BLUE    = 94,    // 亮蓝色前景
    CLR_BRIGHT_MAGENTA = 95,    // 亮品红前景
    CLR_BRIGHT_CYAN    = 96,    // 亮青色前景
    CLR_BRIGHT_WHITE   = 97,    // 亮白色前景
    
    CLR_BG_BLACK   = 40,        // 黑色背景
    CLR_BG_RED     = 41,        // 红色背景
    CLR_BG_GREEN   = 42,        // 绿色背景
    CLR_BG_YELLOW  = 43,        // 黄色背景
    CLR_BG_BLUE    = 44,        // 蓝色背景
    CLR_BG_MAGENTA = 45,        // 品红背景
    CLR_BG_CYAN    = 46,        // 青色背景
    CLR_BG_WHITE   = 47,        // 白色背景
    CLR_BG_DEFAULT = 49,        // 默认背景
    
    CLR_BG_BRIGHT_BLACK   = 100,       // 亮黑色背景
    CLR_BG_BRIGHT_RED     = 101,       // 亮红色背景
    CLR_BG_BRIGHT_GREEN   = 102,       // 亮绿色背景
    CLR_BG_BRIGHT_YELLOW  = 103,       // 亮黄色背景
    CLR_BG_BRIGHT_BLUE    = 104,       // 亮蓝色背景
    CLR_BG_BRIGHT_MAGENTA = 105,       // 亮品红背景
    CLR_BG_BRIGHT_CYAN    = 106,       // 亮青色背景
    CLR_BG_BRIGHT_WHITE   = 107,       // 亮白色背景
} clr_color_t;

/**
 * @brief RGB颜色结构体（0-255范围）
 */
typedef struct {
    uint8_t r;    // 红色分量 (0-255)
    uint8_t g;    // 绿色分量 (0-255)
    uint8_t b;    // 蓝色分量 (0-255)
} clr_rgb_t;

/**
 * @brief xterm-256color 颜色索引范围
 */
typedef enum {
    // 基本颜色 (0-7)
    CLR_256_BLACK   = 0,     // 黑色
    CLR_256_RED     = 1,     // 红色
    CLR_256_GREEN   = 2,     // 绿色
    CLR_256_YELLOW  = 3,     // 黄色
    CLR_256_BLUE    = 4,     // 蓝色
    CLR_256_MAGENTA = 5,     // 品红
    CLR_256_CYAN    = 6,     // 青色
    CLR_256_WHITE   = 7,     // 白色
    
    // 亮色 (8-15)
    CLR_256_BRIGHT_BLACK   = 8,     // 亮黑色
    CLR_256_BRIGHT_RED     = 9,     // 亮红色
    CLR_256_BRIGHT_GREEN   = 10,    // 亮绿色
    CLR_256_BRIGHT_YELLOW  = 11,    // 亮黄色
    CLR_256_BRIGHT_BLUE    = 12,    // 亮蓝色
    CLR_256_BRIGHT_MAGENTA = 13,    // 亮品红
    CLR_256_BRIGHT_CYAN    = 14,    // 亮青色
    CLR_256_BRIGHT_WHITE   = 15,    // 亮白色
    
    // 216色立方体 (16-231)
    CLR_256_CUBE_START = 16,         // 6x6x6 RGB立方体起始
    CLR_256_CUBE_END   = 231,        // 6x6x6 RGB立方体结束
    
    // 灰度渐变 (232-255)
    CLR_256_GRAY_START = 232,        // 灰度起始（从暗到亮）
    CLR_256_GRAY_END   = 255,        // 灰度结束
} clr_256_t;

/**
 * @brief ANSI常见文本样式
 */
typedef enum {
    CLR_RESET       = 0,    // 重置所有样式
    CLR_BOLD        = 1,    // 粗体
    CLR_DIM         = 2,    // 暗淡
    CLR_ITALIC      = 3,    // 斜体
    CLR_UNDERLINE   = 4,    // 下划线
    CLR_BLINK       = 5,    // 闪烁
    CLR_REVERSE     = 7,    // 反显
    CLR_HIDDEN      = 8,    // 隐藏
    CLR_STRIKETHROUGH = 9,  // 删除线
    CLR_NORMAL      = 22,   // 正常亮度
} clr_style_t;

/**
 * @brief 颜色序列类型枚举（用于获取固定长度）
 */
typedef enum {
    CLR_SEQ_ANSI_FG      = 0,  // ANSI前景色 (30-39, 90-97) - 5字节
    CLR_SEQ_ANSI_BG      = 1,  // ANSI背景色 (40-49) - 5字节
    CLR_SEQ_ANSI_BG_BRIGHT = 2, // ANSI背景色 (100-107) - 6字节
    CLR_SEQ_256_FG       = 3,  // 256色前景色 - 11字节
    CLR_SEQ_256_BG       = 4,  // 256色背景色 - 11字节
    CLR_SEQ_RGB_FG       = 5,  // RGB前景色 - 19字节
    CLR_SEQ_RGB_BG       = 6,  // RGB背景色 - 19字节
    CLR_SEQ_STYLE        = 7,  // 样式 - 5字节
    CLR_SEQ_RESET        = 8,  // 重置 - 5字节
} clr_seq_type_t;

/**
 * @brief 获取颜色序列的固定长度
 * @param type 颜色序列类型
 * @return 序列长度（字节）
 */
int clrlen(clr_seq_type_t type);

/**
 * @brief 创建前景色ANSI转义序列
 * @param fg_color 前景色枚举值 (CLR_*)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mkans(int fg_color);

/**
 * @brief 创建背景色ANSI转义序列
 * @param bg_color 背景色枚举值 (CLR_BG_*)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mkansbg(int bg_color);

/**
 * @brief 查表获取前景色ANSI转义序列
 * @param fg_color 前景色枚举值 (CLR_*)
 * @return ANSI转义序列字符串（静态表）
 */
const char* Rmkans(int fg_color);

/**
 * @brief 查表获取背景色ANSI转义序列
 * @param bg_color 背景色枚举值 (CLR_BG_*)
 * @return ANSI转义序列字符串（静态表）
 */
const char* Rmkansbg(int bg_color);

/**
 * @brief 动态生成RGB前景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mkrgb(clr_rgb_t rgb);

/**
 * @brief 动态生成RGB背景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mkrgbbg(clr_rgb_t rgb);

/**
 * @brief 查表拼接生成RGB前景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* Rmkrgb(clr_rgb_t rgb);

/**
 * @brief 查表拼接生成RGB背景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* Rmkrgbbg(clr_rgb_t rgb);

/**
 * @brief 哈希表生成RGB前景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态表）
 */
const char* Hmkrgb(clr_rgb_t rgb);

/**
 * @brief 哈希表生成RGB背景色ANSI转义序列
 * @param rgb RGB颜色结构体
 * @return ANSI转义序列字符串（静态表）
 */
const char* Hmkrgbbg(clr_rgb_t rgb);

/**
 * @brief 直接写入前景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param fg_color 前景色枚举值 (CLR_*)
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void writeans(int fd, int fg_color) {
    // 前景色边界检查
    if (!((fg_color >= 30 && fg_color <= 39) || (fg_color >= 90 && fg_color <= 97))) {
        fg_color = 39;
    }
    
    // 查表返回静态字符串和长度
    const char* seq = Rmkans(fg_color);
    int len = (fg_color >= 30 && fg_color <= 39) ? 5 : 5;
    write(fd, seq, len);
}

/**
 * @brief 直接写入背景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param bg_color 背景色枚举值 (CLR_BG_*)
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void writeansbg(int fd, int bg_color) {
    // 背景色边界检查
    if (!((bg_color >= 40 && bg_color <= 49) || (bg_color >= 100 && bg_color <= 107))) {
        bg_color = 49;
    }
    
    // 查表返回静态字符串和长度
    const char* seq = Rmkansbg(bg_color);
    int len = (bg_color >= 40 && bg_color <= 49) ? 5 : 6;
    write(fd, seq, len);
}

/**
 * @brief 直接写入RGB前景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param rgb RGB颜色结构体
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void writergb(int fd, clr_rgb_t rgb) {
    static char buffer[32];
    
    // 查表拼接：一次 memcpy 组装
    memcpy(buffer, "\033[38;2;", 7);
    
    // RGB数字表
    static const char* rgb_num[256] = {
        "000", "001", "002", "003", "004", "005", "006", "007", "008", "009",
        "010", "011", "012", "013", "014", "015", "016", "017", "018", "019",
        "020", "021", "022", "023", "024", "025", "026", "027", "028", "029",
        "030", "031", "032", "033", "034", "035", "036", "037", "038", "039",
        "040", "041", "042", "043", "044", "045", "046", "047", "048", "049",
        "050", "051", "052", "053", "054", "055", "056", "057", "058", "059",
        "060", "061", "062", "063", "064", "065", "066", "067", "068", "069",
        "070", "071", "072", "073", "074", "075", "076", "077", "078", "079",
        "080", "081", "082", "083", "084", "085", "086", "087", "088", "089",
        "090", "091", "092", "093", "094", "095", "096", "097", "098", "099",
        "100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
        "110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
        "120", "121", "122", "123", "124", "125", "126", "127", "128", "129",
        "130", "131", "132", "133", "134", "135", "136", "137", "138", "139",
        "140", "141", "142", "143", "144", "145", "146", "147", "148", "149",
        "150", "151", "152", "153", "154", "155", "156", "157", "158", "159",
        "160", "161", "162", "163", "164", "165", "166", "167", "168", "169",
        "170", "171", "172", "173", "174", "175", "176", "177", "178", "179",
        "180", "181", "182", "183", "184", "185", "186", "187", "188", "189",
        "190", "191", "192", "193", "194", "195", "196", "197", "198", "199",
        "200", "201", "202", "203", "204", "205", "206", "207", "208", "209",
        "210", "211", "212", "213", "214", "215", "216", "217", "218", "219",
        "220", "221", "222", "223", "224", "225", "226", "227", "228", "229",
        "230", "231", "232", "233", "234", "235", "236", "237", "238", "239",
        "240", "241", "242", "243", "244", "245", "246", "247", "248", "249",
        "250", "251", "252", "253", "254", "255"
    };
    
    memcpy(buffer + 7, rgb_num[rgb.r], 3);
    memcpy(buffer + 10, ";", 1);
    memcpy(buffer + 11, rgb_num[rgb.g], 3);
    memcpy(buffer + 14, ";", 1);
    memcpy(buffer + 15, rgb_num[rgb.b], 3);
    memcpy(buffer + 18, "m", 1);
    
    // 一次系统调用
    write(fd, buffer, 19);
}

/**
 * @brief 直接写入RGB背景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param rgb RGB颜色结构体
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void writergbbg(int fd, clr_rgb_t rgb) {
    static char buffer[32];
    
    // 查表拼接：一次 memcpy 组装
    memcpy(buffer, "\033[48;2;", 7);
    
    // RGB数字表
    static const char* rgb_num[256] = {
        "000", "001", "002", "003", "004", "005", "006", "007", "008", "009",
        "010", "011", "012", "013", "014", "015", "016", "017", "018", "019",
        "020", "021", "022", "023", "024", "025", "026", "027", "028", "029",
        "030", "031", "032", "033", "034", "035", "036", "037", "038", "039",
        "040", "041", "042", "043", "044", "045", "046", "047", "048", "049",
        "050", "051", "052", "053", "054", "055", "056", "057", "058", "059",
        "060", "061", "062", "063", "064", "065", "066", "067", "068", "069",
        "070", "071", "072", "073", "074", "075", "076", "077", "078", "079",
        "080", "081", "082", "083", "084", "085", "086", "087", "088", "089",
        "090", "091", "092", "093", "094", "095", "096", "097", "098", "099",
        "100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
        "110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
        "120", "121", "122", "123", "124", "125", "126", "127", "128", "129",
        "130", "131", "132", "133", "134", "135", "136", "137", "138", "139",
        "140", "141", "142", "143", "144", "145", "146", "147", "148", "149",
        "150", "151", "152", "153", "154", "155", "156", "157", "158", "159",
        "160", "161", "162", "163", "164", "165", "166", "167", "168", "169",
        "170", "171", "172", "173", "174", "175", "176", "177", "178", "179",
        "180", "181", "182", "183", "184", "185", "186", "187", "188", "189",
        "190", "191", "192", "193", "194", "195", "196", "197", "198", "199",
        "200", "201", "202", "203", "204", "205", "206", "207", "208", "209",
        "210", "211", "212", "213", "214", "215", "216", "217", "218", "219",
        "220", "221", "222", "223", "224", "225", "226", "227", "228", "229",
        "230", "231", "232", "233", "234", "235", "236", "237", "238", "239",
        "240", "241", "242", "243", "244", "245", "246", "247", "248", "249",
        "250", "251", "252", "253", "254", "255"
    };
    
    memcpy(buffer + 7, rgb_num[rgb.r], 3);
    memcpy(buffer + 10, ";", 1);
    memcpy(buffer + 11, rgb_num[rgb.g], 3);
    memcpy(buffer + 14, ";", 1);
    memcpy(buffer + 15, rgb_num[rgb.b], 3);
    memcpy(buffer + 18, "m", 1);
    
    // 一次系统调用
    write(fd, buffer, 19);
}

/**
 * @brief 动态生成256色前景色ANSI转义序列
 * @param index 颜色索引 (0-255)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mk256(int index);

/**
 * @brief 动态生成256色背景色ANSI转义序列
 * @param index 颜色索引 (0-255)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mk256bg(int index);

/**
 * @brief 查表生成256色前景色ANSI转义序列
 * @param index 颜色索引 (0-255)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* Rmk256(int index);

/**
 * @brief 查表生成256色背景色ANSI转义序列
 * @param index 颜色索引 (0-255)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* Rmk256bg(int index);

/**
 * @brief 直接写入256色前景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param index 颜色索引 (0-255)
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void write256(int fd, int index) {
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    // 查表返回，固定长度11
    const char* seq = Rmk256(index);
    write(fd, seq, 11);
}

/**
 * @brief 直接写入256色背景色到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param index 颜色索引 (0-255)
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void write256bg(int fd, int index) {
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    // 查表返回，固定长度11
    const char* seq = Rmk256bg(index);
    write(fd, seq, 11);
}

/**
 * @brief 动态生成文本样式ANSI转义序列
 * @param style 样式枚举值 (CLR_STYLE_*)
 * @return ANSI转义序列字符串（静态缓冲区）
 */
const char* mkstyle(int style);

/**
 * @brief 查表生成文本样式ANSI转义序列
 * @param style 样式枚举值 (CLR_STYLE_*)
 * @return ANSI转义序列字符串（静态表）
 */
const char* Rmkstyle(int style);

/**
 * @brief 直接写入文本样式到文件描述符（单线程优化）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @param style 样式枚举值 (CLR_STYLE_*)
 * @note 使用静态缓冲区，单线程使用，性能最优
 */
static inline void writestyle(int fd, int style) {
    // 样式表
    static const char* style_table[23] = {
        [0]  = "\033[00m",  // CLR_RESET
        [1]  = "\033[01m",  // CLR_BOLD
        [2]  = "\033[02m",  // CLR_DIM
        [3]  = "\033[03m",  // CLR_ITALIC
        [4]  = "\033[04m",  // CLR_UNDERLINE
        [5]  = "\033[05m",  // CLR_BLINK
        [7]  = "\033[07m",  // CLR_REVERSE
        [8]  = "\033[08m",  // CLR_HIDDEN
        [9]  = "\033[09m",  // CLR_STRIKETHROUGH
        [22] = "\033[22m",  // CLR_NORMAL
    };
    
    if (style >= 0 && style < 23 && style_table[style] != NULL) {
        write(fd, style_table[style], 7);  // 固定长度7
    } else {
        write(fd, style_table[0], 7);
    }
}

/**
 * @brief 重置所有样式和颜色（返回字符串）
 * @return ANSI重置序列字符串
 * @note 等价于 "\033[0m"
 */
static inline const char* strest(void) {
    return "\033[0m";
}

/**
 * @brief 重置所有样式和颜色（直接写入）
 * @param fd 文件描述符（通常为STDOUT_FILENO）
 * @note 重置前景色、背景色和所有文本样式
 */
static inline void clrest(int fd) {
    write(fd, "\033[0m", 4);
}

#ifdef __cplusplus
}
#endif

#endif /* CLR_H */