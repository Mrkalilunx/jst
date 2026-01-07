#include <args.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int max_depth = INT_MAX;

void parse_command_line_arguments(int argc, char* argv[], optarg_config* config) {
    // Validate input parameters
    if (!config) {
        fprintf(stderr, "jst: internal error - NULL config\n");
        exit(EXIT_FAILURE);
    }

    config->color_mode = COLOR_MODE_SIMPLIFIED;
    config->max_depth = INT_MAX;
    config->file_path = NULL;
    config->filter_key = NULL;
    config->compact = 0;
    config->show_path = 0;
    config->show_stats = 0;
    config->show_types = 0;

    // Detect default language from environment
    const char* lang_env = getenv("LANG");
    if (lang_env && (strstr(lang_env, "en") == lang_env || strstr(lang_env, "en_") != NULL)) {
        config->language = LANG_EN;
    } else {
        config->language = LANG_ZH;
    }

    const char* const short_options = "c:k:L:psl:tvh";
    struct option long_options[] = {
        {"color", required_argument, NULL, 'c'},
        {"filter", required_argument, NULL, 'k'},
        {"max-depth", required_argument, NULL, 'L'},
        {"compact", no_argument, NULL, 'p'},
        {"path", no_argument, NULL, 's'},
        {"lang", required_argument, NULL, 'l'},
        {"stats", no_argument, NULL, 1},
        {"types", no_argument, NULL, 't'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
        case 'c':
            if (strcmp(optarg, "none") == 0) {
                config->color_mode = COLOR_MODE_NONE;
            } else if (strcmp(optarg, "simple") == 0) {
                config->color_mode = COLOR_MODE_SIMPLIFIED;
            } else if (strcmp(optarg, "rich") == 0) {
                config->color_mode = COLOR_MODE_RICH;
            } else {
                fprintf(stderr, "%s: invalid color mode '%s'\n", argv[0], optarg);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        case 'k':
            // Validate filter key - should not be empty
            if (strlen(optarg) == 0) {
                fprintf(stderr, "%s: filter key cannot be empty\n", argv[0]);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            config->filter_key = optarg;
            break;
        case 'L': {
            char* endptr;
            long depth = strtol(optarg, &endptr, 10);
            if (endptr == optarg || *endptr != '\0' || depth <= 0 || depth > INT_MAX) {
                fprintf(stderr, "%s: invalid depth '%s'\n", argv[0], optarg);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            config->max_depth = (int)depth;
            break;
        }
        case 'p':
            config->compact = 1;
            break;
        case 's':
            config->show_path = 1;
            break;
        case 'l':
            if (strcmp(optarg, "en") == 0 || strcmp(optarg, "english") == 0) {
                config->language = LANG_EN;
            } else if (strcmp(optarg, "zh") == 0 || strcmp(optarg, "chinese") == 0) {
                config->language = LANG_ZH;
            } else {
                fprintf(stderr, "%s: invalid language '%s'\n", argv[0], optarg);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        case 1:
            config->show_stats = 1;
            break;
        case 't':
            config->show_types = 1;
            break;
        case 'h':
            display_usage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'v':
            printf("%s 1.0\n", argv[0]);
            exit(EXIT_SUCCESS);
        case '?':
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            exit(EXIT_FAILURE);
        default:
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int file_count = 0;
    for (int i = optind; i < argc; i++) {
        if (file_count++ > 0) {
            fprintf(stderr, "%s: too many arguments\n", argv[0]);
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        config->file_path = argv[i];
    }

    if (!config->file_path) {
        if (isatty(fileno(stdin))) {
            fprintf(stderr, "%s: missing file operand\n", argv[0]);
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        config->file_path = "-";
    }
}