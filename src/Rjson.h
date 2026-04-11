#ifndef RJSON_H
#define RJSON_H

#include "cJSON.h"

/**
 * @brief Rjson 错误码枚举
 * 
 * 定义所有可能的错误类型
 */
typedef enum {
    RJSON_SUCCESS = 0,         // 成功
    RJSON_ERROR_INVALID_JSON,  // 无效的 JSON 格式
    RJSON_ERROR_FILE_NOT_FOUND,// 文件未找到
    RJSON_ERROR_READ_FAILED,   // 读取文件失败
    RJSON_ERROR_MEMORY,        // 内存分配失败
    RJSON_ERROR_INVALID_NODE,  // 无效的 JSON 节点
    RJSON_ERROR_NOT_OBJECT,    // 节点不是对象类型
    RJSON_ERROR_NO_KEYS        // 没有找到键
} RjsonError;

/**
 * @brief JSON 类型枚举
 * 
 * 定义 JSON 数据的6种基本类型
 */
typedef enum {
    JSON_NULL = 0,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_OBJECT,
    JSON_ARRAY
} JsonType;

/**
 * @brief JSON 树节点结构体（前置声明）
 */
typedef struct JsonTreeNode JsonTreeNode;

/**
 * @brief JSON 树节点结构体
 * 
 * 用于表示 JSON 的完整层级结构，包括父子关系和兄弟关系
 * 对于 String/Number/Boolean 类型，会存储具体值
 */
struct JsonTreeNode {
    union {
        char* key;        // 对象成员：键名（动态分配）
        int index;        // 数组元素：索引位置
    } identifier;
    
    JsonType type;                    // JSON 类型
    struct JsonTreeNode* parent;      // 父节点指针
    struct JsonTreeNode* first_child; // 第一个子节点指针
    struct JsonTreeNode* next_sibling;// 下一个兄弟节点指针
    int child_count;                  // 子节点数量
    
    union {
        char* string_val;    // 字符串值（仅当 type == JSON_STRING 时有效，需释放）
        double number_val;   // 数字值（仅当 type == JSON_NUMBER 时有效）
        int bool_val;        // 布尔值（仅当 type == JSON_BOOL 时有效）
    } value;
};

/**
 * @brief 读取 JSON 文件到内存
 * 
 * 读取指定路径的 JSON 文件内容到内存缓冲区
 * 
 * @param file_path JSON 文件路径
 * @return char* 文件内容字符串（以 '\0' 结尾），失败返回 NULL
 * @note 调用者负责使用 free() 释放返回的内存
 */
char* loadJson(const char* file_path);

/**
 * @brief 检测文件是否是有效的 JSON
 * 
 * 通过读取文件内容并使用 cJSON 尝试解析来判断文件是否包含有效的 JSON 数据
 * 
 * @param file_path 文件路径
 * @return int 1 表示是有效的 JSON，0 表示不是或解析失败
 */
int validJson(const char* file_path);

/**
 * @brief 获取错误信息字符串
 * 
 * 根据错误码返回对应的错误描述字符串
 * 
 * @param error 错误码
 * @return const char* 错误描述字符串
 */
const char* errmsg(RjsonError error);

/**
 * @brief 解析 JSON 数据字符串为树形结构
 * 
 * 直接解析 JSON 数据字符串并构建完整的树形结构
 * 
 * @param json_data JSON 数据字符串
 * @return JsonTreeNode* 构建的树节点，失败返回 NULL
 * @note 调用者负责使用 freeJson() 释放返回的内存
 */
JsonTreeNode* parseJson(const char* json_data);

/**
 * @brief 构建完整的 JSON 树形结构
 * 
 * 递归解析 JSON 节点及其所有子节点，构建完整的树形结构
 * 
 * @param node cJSON 节点
 * @param parent_key 父节点的键名（用于当前节点）
 * @param parent 父节点指针
 * @param index 当前节点在父节点的索引位置
 * @return JsonTreeNode* 构建的树节点，失败返回 NULL
 * @note 调用者负责使用 freeJson() 释放返回的内存
 */
JsonTreeNode* buildJson(cJSON* node, const char* parent_key, JsonTreeNode* parent, int index);

/**
 * @brief 释放 JSON 树形结构
 * 
 * 递归释放 buildJson 构建的树形结构及其所有子节点和字符串内存
 * 
 * @param root 树的根节点
 */
void freeJson(JsonTreeNode* root);

#endif