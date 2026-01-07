#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include "util.h"
#include "color.h"

typedef enum {
    LANG_EN = 0,
    LANG_ZH
} Language;

typedef struct {
    ColorMode color_mode;
    int max_depth;
    const char* file_path;
    const char* filter_key;
    int compact;
    int show_path;
    int show_stats;
    int show_types;
    Language language;
} optarg_config;

extern int max_depth;

void parse_command_line_arguments(int argc, char* argv[], optarg_config* config);

#endif