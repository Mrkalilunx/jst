#include <tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    cJSON* node;
    const char* key;
    int index;
} NodePath;

static NodePath path_stack[100];
static int path_count = 0;

static void push_path(cJSON* node, const char* key, int index) {
    if (path_count < 100) {
        path_stack[path_count] = (NodePath){node, key, index};
        path_count++;
    }
}

static void pop_path() {
    if (path_count > 0) path_count--;
}

static void get_full_path(char* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return;

    size_t pos = 0;
    buffer[0] = '\0';

    for (int i = 0; i < path_count && pos < buffer_size - 1; i++) {
        if (path_stack[i].key) {
            pos += snprintf(buffer + pos, buffer_size - pos,
                          i ? ".%s" : "%s", path_stack[i].key);
        } else {
            pos += snprintf(buffer + pos, buffer_size - pos,
                          "[%d]", path_stack[i].index);
        }

        // Prevent buffer overflow
        if (pos >= buffer_size - 1) break;
    }
}

static void print_prefix(int depth, int is_last, const int* is_last_list) {
    for (int i = 0; i < depth - 1; i++) {
        printf(is_last_list[i] ? "    " : "│   ");
    }
    if (depth > 0) {
        printf(is_last ? "└── " : "├── ");
    }
}

static void print_node_name(cJSON* item, const optarg_config* config, int idx) {
    if (item->string) {
        printf("%s%s%s", get_color_key(item), item->string, get_color_reset());
    } else if ((cJSON_IsArray(item) || cJSON_IsObject(item)) && config->show_path) {
        char path[256];
        get_full_path(path, sizeof(path));
        printf("%s%s%s", get_color_key(item), path, get_color_reset());
    } else {
        // 数组元素显示索引
        printf("%s[%d]%s", get_color_index(), idx, get_color_reset());
    }
}

static void print_value(cJSON* item, const optarg_config* config) {
    const char* sep = config->compact ? "" : " ";
    const char* type_str = "";
    const char* type_color = "";

    if (config->show_types) {
        if (cJSON_IsString(item)) {
            type_str = " (string)";
            type_color = get_color_type();
        } else if (cJSON_IsNumber(item)) {
            type_str = " (number)";
            type_color = get_color_type();
        } else if (cJSON_IsBool(item)) {
            type_str = " (boolean)";
            type_color = get_color_type();
        } else if (cJSON_IsNull(item)) {
            type_str = " (null)";
            type_color = get_color_type();
        }
    }

    if (cJSON_IsString(item)) {
        printf("%s: %s\"%s\"%s%s%s%s\n", sep, get_color_value(item), item->valuestring, get_color_reset(), type_color, type_str, get_color_reset());
    } else if (cJSON_IsNumber(item)) {
        printf("%s: %s%g%s%s%s%s\n", sep, get_color_value(item), item->valuedouble, get_color_reset(), type_color, type_str, get_color_reset());
    } else if (cJSON_IsBool(item)) {
        printf("%s: %s%s%s%s%s%s\n", sep, get_color_value(item),
               cJSON_IsTrue(item) ? "true" : "false", get_color_reset(), type_color, type_str, get_color_reset());
    } else if (cJSON_IsNull(item)) {
        printf("%s: %snull%s%s%s%s\n", sep, get_color_value(item), get_color_reset(), type_color, type_str, get_color_reset());
    }
}

static void print_children(cJSON* item, int depth, int current_depth, 
                          const int* is_last_list, const optarg_config* config) {
    if (current_depth > max_depth) return;

    cJSON* child = item->child;
    int child_count = 0;
    
    for (cJSON* c = child; c; c = c->next) child_count++;

    int* new_last_list = calloc(depth + 1, sizeof(int));
    if (depth > 0) {
        memcpy(new_last_list, is_last_list, depth * sizeof(int));
    }
    
    int child_idx = 0;
    int actual_idx = 0;
    while (child) {
        if (config->filter_key && child->string && 
            strstr(child->string, config->filter_key) == NULL) {
            child = child->next;
            child_idx++;
            continue;
        }
        
        new_last_list[depth] = (child_idx == child_count - 1);
        push_path(child, child->string, child_idx);
        print_tree(child, depth + 1, new_last_list[depth], current_depth + 1, 
                  new_last_list, item, actual_idx, config);
        pop_path();
        child = child->next;
        child_idx++;
        actual_idx++;
    }
    
    free(new_last_list);
}

void print_tree(cJSON* item, int depth, int is_last, int current_depth,
               const int* is_last_list, cJSON* parent, int idx, const optarg_config* config) {
    (void)parent;  // Reserved for future use
    if (current_depth > max_depth) return;

    print_prefix(depth, is_last, is_last_list);
    print_node_name(item, config, idx);

    if (cJSON_IsObject(item) || cJSON_IsArray(item)) {
        int size = cJSON_GetArraySize(item);
        if (size == 0) {
            printf(" %s%s%s\n", get_color_key(item), 
                   cJSON_IsArray(item) ? "[]" : "{}", get_color_reset());
        } else {
            printf("\n");
            print_children(item, depth, current_depth, is_last_list, config);
        }
    } else {
        print_value(item, config);
    }
}

void print_tree_with_root(cJSON* root, const optarg_config* config) {
    int child_count = 0;
    for (cJSON* c = root->child; c; c = c->next) child_count++;
    
    int last_list = 0;
    int idx = 0;
    int printed_count = 0;
    
    for (cJSON* child = root->child; child; child = child->next) {
        if (config->filter_key && child->string && 
            strstr(child->string, config->filter_key) == NULL) {
            idx++;
            continue;
        }
        
        last_list = (idx == child_count - 1);
        push_path(child, child->string, idx);
        print_tree(child, 1, last_list, 1, &last_list, root, idx, config);
        pop_path();
        idx++;
        printed_count++;
    }
    
    if (config->show_stats) {
        if (config->language == LANG_ZH) {
            printf("\n统计: %d 项", child_count);
            if (config->filter_key) {
                printf("（显示 %d 项）", printed_count);
            }
            printf("\n");
        } else {
            printf("\nStatistics: %d item%s", child_count, child_count == 1 ? "" : "s");
            if (config->filter_key) {
                printf(" (%d shown)", printed_count);
            }
            printf("\n");
        }
    }
}