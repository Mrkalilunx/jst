#include "Rjson.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// 将 cJSON 类型转换为 JsonType
static JsonType cJSON_to_JsonType(int cjson_type) {
    switch (cjson_type) {
        case cJSON_NULL:
            return JSON_NULL;
        case cJSON_True:
        case cJSON_False:
            return JSON_BOOL;
        case cJSON_Number:
            return JSON_NUMBER;
        case cJSON_String:
            return JSON_STRING;
        case cJSON_Object:
            return JSON_OBJECT;
        case cJSON_Array:
            return JSON_ARRAY;
        default:
            return JSON_NULL;
    }
}

char* loadJson(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        return NULL;
    }
    
    rewind(file);
    
    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t read_size = fread(buffer, 1, size, file);
    fclose(file);
    
    if (read_size != (size_t)size) {
        free(buffer);
        return NULL;
    }
    
    buffer[size] = '\0';
    return buffer;
}

int validJson(const char* file_path) {
    if (!file_path) {
        return 0;
    }
    
    char* content = loadJson(file_path);
    if (!content) {
        return 0;
    }
    
    cJSON* root = cJSON_Parse(content);
    free(content);
    
    if (root) {
        cJSON_Delete(root);
        return 1;
    }
    
    return 0;
}

const char* errmsg(RjsonError error) {
    switch (error) {
        case RJSON_SUCCESS:
            return "Success";
        case RJSON_ERROR_INVALID_JSON:
            return "Invalid JSON format";
        case RJSON_ERROR_FILE_NOT_FOUND:
            return "File not found";
        case RJSON_ERROR_READ_FAILED:
            return "Failed to read file";
        case RJSON_ERROR_MEMORY:
            return "Memory allocation failed";
        case RJSON_ERROR_INVALID_NODE:
            return "Invalid JSON node";
        case RJSON_ERROR_NOT_OBJECT:
            return "Node is not an object";
        case RJSON_ERROR_NO_KEYS:
            return "No keys found";
        default:
            return "Unknown error";
    }
}

JsonTreeNode* parseJson(const char* json_data) {
    if (!json_data) {
        return NULL;
    }
    
    cJSON* root = cJSON_Parse(json_data);
    if (!root) {
        return NULL;
    }
    
    JsonTreeNode* tree = buildJson(root, NULL, NULL, 0);
    
    cJSON_Delete(root);
    
    return tree;
}

JsonTreeNode* buildJson(cJSON* node, const char* parent_key, JsonTreeNode* parent, int index) {
    if (!node) {
        return NULL;
    }
    
    JsonTreeNode* tree_node = malloc(sizeof(JsonTreeNode));
    if (!tree_node) {
        return NULL;
    }
    
    // 初始化节点
    tree_node->type = cJSON_to_JsonType(node->type);
    tree_node->parent = parent;
    tree_node->first_child = NULL;
    tree_node->next_sibling = NULL;
    tree_node->child_count = 0;
    
    // 设置标识符（key 或 index）
    if (parent && parent->type == JSON_ARRAY) {
        // 数组元素：存储索引
        tree_node->identifier.index = index;
    } else {
        // 对象成员：存储键名
        tree_node->identifier.key = parent_key ? strdup(parent_key) : NULL;
    }
    
    // 存储值（仅针对 String/Number/Boolean 类型）
    if (cJSON_IsString(node)) {
        tree_node->value.string_val = node->valuestring ? strdup(node->valuestring) : NULL;
    } else if (cJSON_IsNumber(node)) {
        tree_node->value.number_val = node->valuedouble;
    } else if (cJSON_IsTrue(node)) {
        tree_node->value.bool_val = 1;
    } else if (cJSON_IsFalse(node)) {
        tree_node->value.bool_val = 0;
    } else {
        tree_node->value.string_val = NULL;
    }
    
    if (cJSON_IsObject(node) || cJSON_IsArray(node)) {
        int child_count = cJSON_GetArraySize(node);
        tree_node->child_count = child_count;
        
        if (child_count > 0) {
            JsonTreeNode** child_array = malloc(sizeof(JsonTreeNode*) * child_count);
            if (!child_array) {
                if (tree_node->type == JSON_STRING && tree_node->value.string_val) {
                    free(tree_node->value.string_val);
                }
                if (parent && parent->type != JSON_ARRAY) {
                    free(tree_node->identifier.key);
                }
                free(tree_node);
                return NULL;
            }
            
            cJSON* child = node->child;
            int child_index = 0;
            JsonTreeNode* prev_child = NULL;
            
            while (child && child_index < child_count) {
                const char* child_key = NULL;
                if (cJSON_IsObject(node) && child->string) {
                    child_key = child->string;
                }
                
                JsonTreeNode* child_node = buildJson(child, child_key, tree_node, child_index);
                
                if (child_node) {
                    child_array[child_index] = child_node;
                    
                    if (!tree_node->first_child) {
                        tree_node->first_child = child_node;
                    }
                    
                    if (prev_child) {
                        prev_child->next_sibling = child_node;
                    }
                    
                    prev_child = child_node;
                }
                
                child = child->next;
                child_index++;
            }
            
            free(child_array);
        }
    }
    
    return tree_node;
}

void freeJson(JsonTreeNode* root) {
    if (!root) {
        return;
    }
    
    JsonTreeNode* child = root->first_child;
    while (child) {
        JsonTreeNode* next = child->next_sibling;
        freeJson(child);
        child = next;
    }
    
    // 释放字符串值
    if (root->type == JSON_STRING && root->value.string_val) {
        free(root->value.string_val);
    }
    
    // 释放键名（仅对象成员有键名）
    if (root->parent && root->parent->type != JSON_ARRAY) {
        free(root->identifier.key);
    }
    
    free(root);
}