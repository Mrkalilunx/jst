#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include "util.h"
#include "color.h"

typedef struct {
    ColorMode color_mode;
    int max_depth;
    const char* file_path;
} optarg_config;

extern int max_depth;

void parse_command_line_arguments(int argc, char *argv[], optarg_config* config);

#endif