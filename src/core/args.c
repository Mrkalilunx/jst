#include "args.h"
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int max_depth = INT_MAX;

static void handle_error(const char* msg, const char* prog_name) {
    fprintf(stderr, "Error: %s\n", msg);
    display_usage(prog_name);
    exit(EXIT_FAILURE);
}

void parse_command_line_arguments(int argc, char *argv[], optarg_config *config) {
    config->color_mode = COLOR_MODE_SIMPLIFIED;
    config->max_depth = INT_MAX;
    config->file_path = NULL;
    
    int opt;
    const char* const short_options = "nChvL:";
    struct option long_options[] = {
        {"no-color", no_argument, NULL, 'n'},
        {"color", no_argument, NULL, 'C'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"max-depth", required_argument, NULL, 'L'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
        case 'n':
            config->color_mode = COLOR_MODE_SIMPLIFIED;
            break;
        case 'C':
            config->color_mode = COLOR_MODE_RICH;
            break;
        case 'h':
            display_usage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'v':
            printf("%s version 1.0\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'L':
            config->max_depth = atoi(optarg);
            if (config->max_depth <= 0) {
                handle_error("Depth must be a positive integer", argv[0]);
            }
            break;
        case '?':
            fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
            display_usage(argv[0]);
            exit(EXIT_FAILURE);
        default:
            display_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int file_count = 0;
    for (int i = optind; i < argc; i++) {
        if (file_count++ > 0) {
            handle_error("Multiple files specified", argv[0]);
        }
        config->file_path = argv[i];
    }

    if (!config->file_path) {
        if (isatty(fileno(stdin))) {
            handle_error("No input file specified", argv[0]);
        }
        config->file_path = "-";
    }
}