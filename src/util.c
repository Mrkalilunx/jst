#include "util.h"
#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_usage(const char* prog_name) {
    Language lang = LANG_ZH;
    
    // Try to detect language from environment
    const char* lang_env = getenv("LANG");
    if (lang_env && (strstr(lang_env, "en") == lang_env || strstr(lang_env, "en_") != NULL)) {
        lang = LANG_EN;
    }
    
    if (lang == LANG_ZH) {
        // 中文帮助信息 - 标准格式
        printf("用法: %s [选项] [文件]\n", prog_name);
        printf("JSON 树状查看器 - 以树形结构显示 JSON 文件\n\n");
        printf("选项:\n");
        printf("  -c, --color MODE   颜色模式: none（无色）, simple（简洁，默认）, rich（丰富）\n");
        printf("  -k, --filter KEY   过滤输出，仅显示匹配的键\n");
        printf("  -L, --max-depth N  设置最大显示深度\n");
        printf("  -p, --compact      紧凑输出（减少间距）\n");
        printf("  -s, --path         显示数组/对象项的完整路径\n");
        printf("  -l, --lang LANG    语言: zh（中文）, en（英文）\n");
        printf("  -t, --types        显示值的类型信息\n");
        printf("      --stats        显示 JSON 统计信息\n");
        printf("  -h, --help         显示此帮助信息\n");
        printf("  -v, --version      显示版本信息\n\n");
        printf("示例:\n");
        printf("  %s file.json          # 查看文件\n", prog_name);
        printf("  %s -c rich file.json # 使用丰富颜色模式\n", prog_name);
        printf("  %s -L 2 file.json    # 限制显示深度为 2\n", prog_name);
        printf("  %s -k name file.json # 仅显示包含 'name' 的键\n", prog_name);
        printf("  cat file.json | %s   # 从标准输入读取\n\n", prog_name);
        printf("如果未指定文件，则从标准输入读取\n");
        printf("\n报告错误至: https://github.com/Mrkalilunx/jst/issues\n");
    } else {
        // 英文帮助信息 - 标准格式
        printf("Usage: %s [OPTION]... [FILE]\n", prog_name);
        printf("JSON Tree Viewer - Display JSON files in tree structure\n\n");
        printf("Options:\n");
        printf("  -c, --color MODE   Color mode: none, simple (default), rich\n");
        printf("  -k, --filter KEY   Filter output to show only matching keys\n");
        printf("  -L, --max-depth N  Set maximum display depth\n");
        printf("  -p, --compact      Compact output (reduce spacing)\n");
        printf("  -s, --path         Show full path for array/object items\n");
        printf("  -l, --lang LANG    Language: zh (Chinese), en (English)\n");
        printf("  -t, --types        Show type information for values\n");
        printf("      --stats        Show JSON statistics\n");
        printf("  -h, --help         Show this help message\n");
        printf("  -v, --version      Show version information\n\n");
        printf("Examples:\n");
        printf("  %s file.json          # View a file\n", prog_name);
        printf("  %s -c rich file.json # Use rich color mode\n", prog_name);
        printf("  %s -L 2 file.json    # Limit display depth to 2\n", prog_name);
        printf("  %s -k name file.json # Show only keys containing 'name'\n", prog_name);
        printf("  cat file.json | %s   # Read from stdin\n\n", prog_name);
        printf("If no file is specified, reads from stdin\n");
        printf("\nReport bugs to: https://github.com/Mrkalilunx/jst/issues\n");
    }
}