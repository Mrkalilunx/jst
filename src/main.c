#include "args.h"
#include "color.h"
#include "file.h"
#include "json.h"
#include "tree.h"
#include "util.h"
#include <limits.h>

int main(int argc, char* argv[]) {
    optarg_config config = { 
        .color_mode = COLOR_MODE_SIMPLIFIED, 
        .max_depth = INT_MAX, 
        .file_path = NULL 
    };

    parse_command_line_arguments(argc, argv, &config);
    current_color_mode = config.color_mode;
    max_depth = config.max_depth;

    char* processed_data = read_and_process_file(config.file_path);
    cJSON* root = parse_json_data(processed_data);
    
    print_tree_with_root(root);
    cJSON_Delete(root);
    
    return EXIT_SUCCESS;
}