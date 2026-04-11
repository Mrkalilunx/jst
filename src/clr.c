/**
 * @file clr.c
 */

#include "clr.h"
#include <stdio.h>
#include <string.h>

// ============== 静态查表定义 ==============

// RGB分量表（0-255，三位数字字符串）
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

// ANSI基础色前景查表（索引0-67，对应颜色代码30-97）
static const char* fg_table[68] = {
    [30-30] = "\033[30m",    // CLR_BLACK
    [31-30] = "\033[31m",    // CLR_RED
    [32-30] = "\033[32m",    // CLR_GREEN
    [33-30] = "\033[33m",    // CLR_YELLOW
    [34-30] = "\033[34m",    // CLR_BLUE
    [35-30] = "\033[35m",    // CLR_MAGENTA
    [36-30] = "\033[36m",    // CLR_CYAN
    [37-30] = "\033[37m",    // CLR_WHITE
    [39-30] = "\033[39m",    // CLR_DEFAULT
    [90-30] = "\033[90m",    // CLR_BRIGHT_BLACK
    [91-30] = "\033[91m",    // CLR_BRIGHT_RED
    [92-30] = "\033[92m",    // CLR_BRIGHT_GREEN
    [93-30] = "\033[93m",    // CLR_BRIGHT_YELLOW
    [94-30] = "\033[94m",    // CLR_BRIGHT_BLUE
    [95-30] = "\033[95m",    // CLR_BRIGHT_MAGENTA
    [96-30] = "\033[96m",    // CLR_BRIGHT_CYAN
    [97-30] = "\033[97m",    // CLR_BRIGHT_WHITE
};

// ANSI基础色前景长度表（5字节）
static const int fg_len[68] = {
    [30-30] = 5,  // \033[30m
    [31-30] = 5,  // \033[31m
    [32-30] = 5,  // \033[32m
    [33-30] = 5,  // \033[33m
    [34-30] = 5,  // \033[34m
    [35-30] = 5,  // \033[35m
    [36-30] = 5,  // \033[36m
    [37-30] = 5,  // \033[37m
    [39-30] = 5,  // \033[39m
    [90-30] = 5,  // \033[90m
    [91-30] = 5,  // \033[91m
    [92-30] = 5,  // \033[92m
    [93-30] = 5,  // \033[93m
    [94-30] = 5,  // \033[94m
    [95-30] = 5,  // \033[95m
    [96-30] = 5,  // \033[96m
    [97-30] = 5,  // \033[97m
};

// ANSI基础色背景查表（索引0-67，对应颜色代码40-107）
static const char* bg_table[68] = {
    [40-40] = "\033[40m",    // CLR_BG_BLACK
    [41-40] = "\033[41m",    // CLR_BG_RED
    [42-40] = "\033[42m",    // CLR_BG_GREEN
    [43-40] = "\033[43m",    // CLR_BG_YELLOW
    [44-40] = "\033[44m",    // CLR_BG_BLUE
    [45-40] = "\033[45m",    // CLR_BG_MAGENTA
    [46-40] = "\033[46m",    // CLR_BG_CYAN
    [47-40] = "\033[47m",    // CLR_BG_WHITE
    [49-40] = "\033[49m",    // CLR_BG_DEFAULT
    [100-40] = "\033[100m",  // CLR_BG_BRIGHT_BLACK
    [101-40] = "\033[101m",  // CLR_BG_BRIGHT_RED
    [102-40] = "\033[102m",  // CLR_BG_BRIGHT_GREEN
    [103-40] = "\033[103m",  // CLR_BG_BRIGHT_YELLOW
    [104-40] = "\033[104m",  // CLR_BG_BRIGHT_BLUE
    [105-40] = "\033[105m",  // CLR_BG_BRIGHT_MAGENTA
    [106-40] = "\033[106m",  // CLR_BG_BRIGHT_CYAN
    [107-40] = "\033[107m",  // CLR_BG_BRIGHT_WHITE
};

// ANSI基础色背景长度表（5或6字节）
static const int bg_len[68] = {
    [40-40] = 5,   // \033[40m
    [41-40] = 5,   // \033[41m
    [42-40] = 5,   // \033[42m
    [43-40] = 5,   // \033[43m
    [44-40] = 5,   // \033[44m
    [45-40] = 5,   // \033[45m
    [46-40] = 5,   // \033[46m
    [47-40] = 5,   // \033[47m
    [49-40] = 5,   // \033[49m
    [100-40] = 6,  // \033[100m
    [101-40] = 6,  // \033[101m
    [102-40] = 6,  // \033[102m
    [103-40] = 6,  // \033[103m
    [104-40] = 6,  // \033[104m
    [105-40] = 6,  // \033[105m
    [106-40] = 6,  // \033[106m
    [107-40] = 6,  // \033[107m
};

// RGB哈希表缓存（存储最近使用的RGB颜色）
#define RGB_HASH_SIZE 256
static char rgb_fg_hash_cache[RGB_HASH_SIZE][32] = {0};
static char rgb_bg_hash_cache[RGB_HASH_SIZE][32] = {0};
static uint32_t rgb_fg_hash_rgb[RGB_HASH_SIZE] = {0};
static uint32_t rgb_bg_hash_rgb[RGB_HASH_SIZE] = {0};
static int rgb_hash_initialized = 0;

// 初始化RGB哈希表
static void init_rgb_hash(void) {
    if (rgb_hash_initialized) return;
    
    for (int i = 0; i < RGB_HASH_SIZE; i++) {
        rgb_fg_hash_rgb[i] = 0xFFFFFFFF;
        rgb_bg_hash_rgb[i] = 0xFFFFFFFF;
    }
    
    rgb_hash_initialized = 1;
}

// 256色前景表（0-255）
static char color256_fg_table[256][16];
static char color256_bg_table[256][16];
static int color256_initialized = 0;

// 初始化256色表
static void init_color256_table(void) {
    if (color256_initialized) return;
    
    for (int i = 0; i < 256; i++) {
        // 前景色：\033[38;5;000m (补零到3位，固定长度11)
        snprintf(color256_fg_table[i], 16, "\033[38;5;%03dm", i);
        // 背景色：\033[48;5;000m (补零到3位，固定长度11)
        snprintf(color256_bg_table[i], 16, "\033[48;5;%03dm", i);
    }
    
    color256_initialized = 1;
}

// 样式查表
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

// ============== ANSI基础色函数 ==============

const char* mkans(int fg_color) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    // 前景色边界检查：只允许 30-39 或 90-97
    if (!((fg_color >= 30 && fg_color <= 39) || (fg_color >= 90 && fg_color <= 97))) {
        fg_color = 39;  // 无效时使用默认前景色
    }
    
    snprintf(buffer, 32, "\033[%dm", fg_color);
    return buffer;
}

const char* mkansbg(int bg_color) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    // 背景色边界检查：只允许 40-49 或 100-107
    if (!((bg_color >= 40 && bg_color <= 49) || (bg_color >= 100 && bg_color <= 107))) {
        bg_color = 49;  // 无效时使用默认背景色
    }
    
    snprintf(buffer, 32, "\033[%dm", bg_color);
    return buffer;
}

const char* Rmkans(int fg_color) {
    if (fg_color >= 30 && fg_color <= 39) {
        return fg_table[fg_color - 30];
    }
    if (fg_color >= 90 && fg_color <= 97) {
        return fg_table[fg_color - 30];
    }
    return "\033[39m";  // 默认返回重置
}

const char* Rmkansbg(int bg_color) {
    if (bg_color >= 40 && bg_color <= 49) {
        return bg_table[bg_color - 40];
    }
    if (bg_color >= 100 && bg_color <= 107) {
        return bg_table[bg_color - 40];
    }
    return "\033[49m";  // 默认返回重置
}

// 动态生成RGB前景色
const char* mkrgb(clr_rgb_t rgb) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    snprintf(buffer, 32, "\033[38;2;%d;%d;%dm", rgb.r, rgb.g, rgb.b);
    return buffer;
}

// 动态生成RGB背景色
const char* mkrgbbg(clr_rgb_t rgb) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    snprintf(buffer, 32, "\033[48;2;%d;%d;%dm", rgb.r, rgb.g, rgb.b);
    return buffer;
}

// 查表拼接生成RGB前景色
const char* Rmkrgb(clr_rgb_t rgb) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    // 合并拷贝：预计算偏移量，减少指针操作
    memcpy(buffer, "\033[38;2;", 7);
    memcpy(buffer + 7, rgb_num[rgb.r], 3);
    memcpy(buffer + 10, ";", 1);
    memcpy(buffer + 11, rgb_num[rgb.g], 3);
    memcpy(buffer + 14, ";", 1);
    memcpy(buffer + 15, rgb_num[rgb.b], 3);
    memcpy(buffer + 18, "m", 1);
    buffer[19] = '\0';
    
    return buffer;
}

// 查表拼接生成RGB背景色
const char* Rmkrgbbg(clr_rgb_t rgb) {
    static char buffers[4][32];
    static int index = 0;
    char* buffer = buffers[index];
    index = (index + 1) % 4;
    
    // 合并拷贝：预计算偏移量，减少指针操作
    memcpy(buffer, "\033[48;2;", 7);
    memcpy(buffer + 7, rgb_num[rgb.r], 3);
    memcpy(buffer + 10, ";", 1);
    memcpy(buffer + 11, rgb_num[rgb.g], 3);
    memcpy(buffer + 14, ";", 1);
    memcpy(buffer + 15, rgb_num[rgb.b], 3);
    memcpy(buffer + 18, "m", 1);
    buffer[19] = '\0';
    
    return buffer;
}

// 哈希表生成RGB前景色
const char* Hmkrgb(clr_rgb_t rgb) {
    // 确保哈希表已初始化
    init_rgb_hash();
    
    // 使用RGB值作为哈希键（位运算优化）
    uint32_t hash = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
    int idx = hash & (RGB_HASH_SIZE - 1);  // 位运算代替取模
    
    // 检查缓存中的RGB值是否匹配
    if (rgb_fg_hash_rgb[idx] != hash) {
        // 哈希冲突或缓存为空，重新生成并覆盖
        rgb_fg_hash_rgb[idx] = hash;
        snprintf(rgb_fg_hash_cache[idx], 32, "\033[38;2;%d;%d;%dm", rgb.r, rgb.g, rgb.b);
    }
    
    return rgb_fg_hash_cache[idx];
}

// 哈希表生成RGB背景色
const char* Hmkrgbbg(clr_rgb_t rgb) {
    // 确保哈希表已初始化
    init_rgb_hash();
    
    // 使用RGB值作为哈希键（位运算优化）
    uint32_t hash = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
    int idx = hash & (RGB_HASH_SIZE - 1);  // 位运算代替取模
    
    // 检查缓存中的RGB值是否匹配
    if (rgb_bg_hash_rgb[idx] != hash) {
        // 哈希冲突或缓存为空，重新生成并覆盖
        rgb_bg_hash_rgb[idx] = hash;
        snprintf(rgb_bg_hash_cache[idx], 32, "\033[48;2;%d;%d;%dm", rgb.r, rgb.g, rgb.b);
    }
    
    return rgb_bg_hash_cache[idx];
}

// 动态生成256色前景色
const char* mk256(int index) {
    static char buffers[4][16];
    static int buf_idx = 0;
    char* buffer = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    snprintf(buffer, 16, "\033[38;5;%03dm", index);  // 补零到3位
    return buffer;
}

// 动态生成256色背景色
const char* mk256bg(int index) {
    static char buffers[4][16];
    static int buf_idx = 0;
    char* buffer = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    snprintf(buffer, 16, "\033[48;5;%03dm", index);  // 补零到3位
    return buffer;
}

// 查表生成256色前景色
const char* Rmk256(int index) {
    init_color256_table();
    
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    return color256_fg_table[index];
}

// 查表生成256色背景色
const char* Rmk256bg(int index) {
    init_color256_table();
    
    // 边界检查
    if (index < 0 || index > 255) {
        index = 0;
    }
    
    return color256_bg_table[index];
}

// ============== 样式函数 ==============

// 动态生成样式
const char* mkstyle(int style) {
    static char buffers[4][16];
    static int buf_idx = 0;
    char* buffer = buffers[buf_idx];
    buf_idx = (buf_idx + 1) % 4;
    
    snprintf(buffer, 16, "\033[%dm", style);
    return buffer;
}

// 查表生成样式
const char* Rmkstyle(int style) {
    if (style >= 0 && style < (int)(sizeof(style_table) / sizeof(style_table[0]))) {
        if (style_table[style] != NULL) {
            return style_table[style];
        }
    }
    return style_table[0];  // 默认返回重置
}

// 获取颜色序列的固定长度
int clrlen(clr_seq_type_t type) {
    switch (type) {
        case CLR_SEQ_ANSI_FG:
            return 5;   // \033[30m (30-39, 90-97)
        case CLR_SEQ_ANSI_BG:
            return 5;   // \033[40m (40-49)
        case CLR_SEQ_ANSI_BG_BRIGHT:
            return 6;   // \033[100m (100-107)
        case CLR_SEQ_256_FG:
            return 11;  // \033[38;5;000m
        case CLR_SEQ_256_BG:
            return 11;  // \033[48;5;000m
        case CLR_SEQ_RGB_FG:
            return 19;  // \033[38;2;RRR;GGG;BBBm
        case CLR_SEQ_RGB_BG:
            return 19;  // \033[48;2;RRR;GGG;BBBm
        case CLR_SEQ_STYLE:
            return 5;   // \033[00m
        case CLR_SEQ_RESET:
            return 4;   // \033[0m
        default:
            return 0;
    }
}