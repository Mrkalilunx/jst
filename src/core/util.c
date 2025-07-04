#include "util.h"
#include <stdio.h>

void display_usage(const char* prog_name) {
    printf("Usage: %s [options] [file]\n\n", prog_name);
    printf("Options:\n");
    printf("  -n, --no-color    Simplified color mode (default)\n");
    printf("  -C, --color       Rich color mode\n");
    printf("  -L, --max-depth N Set maximum display depth\n");
    printf("  -h, --help        Show this help message\n");
    printf("  -v, --version     Show version information\n\n");
    printf("If no file is specified, reads from stdin\n");
}