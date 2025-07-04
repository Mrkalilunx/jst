#include "tree.h"
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
    buffer[0] = '\0';
    for (int i = 0; i < path_count; i++) {
        if (path_stack[i].key) {
            snprintf(buffer + strlen(buffer), buffer_size - strlen(buffer), 
                    i ? ".%s" : "%s", path_stack[i].key);
        } else {
            snprintf(buffer + strlen(buffer), buffer_size - strlen(buffer), 
                    "[%d]", path_stack[i].index);
        }
    }
}

static void print_indent(int depth, const int* is_last_list) {
    for (int i = 0; i < depth - 1; i++) {
        printf(is_last_list[i] ? "    " : "│   ");
    }
}

static void print_connector(int depth, int is_last) {
    if (depth > 0) printf(is_last ? "└── " : "├── ");
}

static void print_node_name(cJSON* item) {
    if (item->string) {
        printf("%s%s%s", get_color_key(item), item->string, get_color_reset());
    } else if (cJSON_IsArray(item) || cJSON_IsObject(item)) {
        char path[256];
        get_full_path(path, sizeof(path));
        printf("%s%s%s", get_color_key(item), path, get_color_reset());
    }
}

static void print_value(cJSON* item) {
    if (cJSON_IsString(item)) {
        printf(" %s\"%s\"%s\n", get_color_value(item), item->valuestring, get_color_reset());
    } else if (cJSON_IsNumber(item)) {
        printf(" %s%g%s\n", get_color_value(item), item->valuedouble, get_color_reset());
    } else if (cJSON_IsBool(item)) {
        printf(" %s%s%s\n", get_color_value(item), 
               cJSON_IsTrue(item) ? "true" : "false", get_color_reset());
    } else if (cJSON_IsNull(item)) {
        printf(" %snull%s\n", get_color_value(item), get_color_reset());
    }
}

static void print_children(cJSON* item, int depth, int current_depth, 
                          const int* is_last_list, cJSON* parent, int idx) {
    if (current_depth > max_depth) return;

    cJSON* child = item->child;
    int child_count = 0;
    for (cJSON* c = child; c; c = c->next) child_count++;

    int* new_last_list = calloc(depth + 1, sizeof(int));
    memcpy(new_last_list, is_last_list, depth * sizeof(int));
    
    int child_idx = 0;
    while (child) {
        new_last_list[depth] = (child_idx == child_count - 1);
        push_path(child, child->string, child_idx);
        print_tree(child, depth + 1, new_last_list[depth], current_depth + 1, 
                  new_last_list, item, child_idx);
        pop_path();
        child = child->next;
        child_idx++;
    }
    
    free(new_last_list);
}

void print_tree(cJSON* item, int depth, int is_last, int current_depth, 
               const int* is_last_list, cJSON* parent, int idx) {
    if (current_depth > max_depth) return;

    print_indent(depth, is_last_list);
    print_connector(depth, is_last);
    print_node_name(item);

    if (cJSON_IsObject(item) || cJSON_IsArray(item)) {
        printf("\n");
        if (cJSON_GetArraySize(item) > 0) {
            print_children(item, depth, current_depth, is_last_list, parent, idx);
        }
    } else {
        print_value(item);
    }
}

void print_tree_with_root(cJSON* root) {
    printf(".\n");
    
    int child_count = 0;
    for (cJSON* c = root->child; c; c = c->next) child_count++;
    
    int* last_list = calloc(1, sizeof(int));
    int idx = 0;
    
    for (cJSON* child = root->child; child; child = child->next) {
        last_list[0] = (idx == child_count - 1);
        push_path(child, child->string, idx);
        print_tree(child, 1, last_list[0], 1, last_list, root, idx);
        pop_path();
        idx++;
    }
    
    free(last_list);
}