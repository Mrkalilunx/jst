#include "Rjson.h"
#include "cJSON.h"
#include "clr.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

// 全局配置
static ProgramConfig g_config;

#define PROGRAM_NAME "jst"
#define VERSION "1.0.0"
#define AUTHOR "悠悠的小球球"
#define DESCRIPTION "JSON Tree Tool"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__
#define BUILD_TIMESTAMP BUILD_DATE " " BUILD_TIME

void cmd_help(void) {
    printf("%s%s %s%s\n", Rmkans(CLR_BOLD), Rmkans(CLR_CYAN), PROGRAM_NAME, strest());
    printf("%sUsage:%s %s <file> [options]\n", Rmkans(CLR_YELLOW), strest(), PROGRAM_NAME);
    printf("       %s | %s [options]\n", PROGRAM_NAME, PROGRAM_NAME);
    printf("\n");
    printf("%sOptions:%s\n", Rmkans(CLR_YELLOW), strest());
    printf("  -h, --help         %sShow this help message%s\n", Rmkans(CLR_GREEN), strest());
    printf("  -v, --verbose      %sShow version information%s\n", Rmkans(CLR_GREEN), strest());
    printf("  -d, --detail       %sShow detailed output including timing%s\n", Rmkans(CLR_GREEN), strest());
    printf("  -i, --input        %sInput JSON file (optional)%s\n", Rmkans(CLR_GREEN), strest());
    printf("  -L, --max-depth N  %sMaximum depth to display (default: unlimited)%s\n", Rmkans(CLR_GREEN), strest());
    printf("  -C, --config FILE  %sUse specified config file%s\n", Rmkans(CLR_GREEN), strest());
    printf("      --generate-config [FILE]  %sGenerate default config file (default: .jst.json)%s\n", Rmkans(CLR_GREEN), strest());
    printf("\n");
    printf("%sExamples:%s\n", Rmkans(CLR_YELLOW), strest());
    printf("  %s data.json\n", PROGRAM_NAME);
    printf("  %s -i data.json\n", PROGRAM_NAME);
    printf("  %s data.json -L 2\n", PROGRAM_NAME);
    printf("  %s data.json -C myconfig.json\n", PROGRAM_NAME);
    printf("  %s --generate-config\n", PROGRAM_NAME);
    printf("  %s --generate-config ~/.jst.json\n", PROGRAM_NAME);
    printf("  cat data.json | %s\n", PROGRAM_NAME);
    printf("  echo '{\"key\":\"value\"}' | %s\n", PROGRAM_NAME);
    printf("  %s data.json -d    %s# Show timing details%s\n", PROGRAM_NAME, Rmkans(CLR_BRIGHT_BLACK), strest());
}

void cmd_verbose(void) {
    printf("%s%s %s %s%s\n", Rmkans(CLR_BOLD), Rmkans(CLR_CYAN), PROGRAM_NAME, VERSION, strest());
    printf("%sAuthor:%s %s\n", Rmkans(CLR_YELLOW), strest(), AUTHOR);
    printf("%sBuild:%s %s\n", Rmkans(CLR_GREEN), strest(), BUILD_TIMESTAMP);
}

/**
 * @brief 从标准输入或文件读取JSON数据
 * 
 * @param input_file 输入文件路径，如果为NULL则从标准输入读取
 * @return JsonTreeNode* 解析后的JSON树节点，失败返回NULL
 */
JsonTreeNode* cmd_input(const char* input_file) {
    char* json_data = NULL;
    
    if (input_file) {
        // 从文件读取
        if (!validJson(input_file)) {
            printf("%sError:%s Invalid JSON file\n", Rmkans(CLR_RED), strest());
            return NULL;
        }

        json_data = loadJson(input_file);
        if (!json_data) {
            printf("%sError:%s Failed to read file\n", Rmkans(CLR_RED), strest());
            return NULL;
        }
    } else {
        // 从标准输入读取
        FILE* fp = stdin;
        
        // 动态读取标准输入
        size_t capacity = 1024;
        size_t size = 0;
        json_data = (char*)malloc(capacity);
        if (!json_data) {
            printf("%sError:%s Memory allocation failed\n", Rmkans(CLR_RED), strest());
            return NULL;
        }
        
        int c;
        while ((c = fgetc(fp)) != EOF) {
            if (size >= capacity - 1) {
                capacity *= 2;
                char* new_data = (char*)realloc(json_data, capacity);
                if (!new_data) {
                    printf("%sError:%s Memory allocation failed\n", Rmkans(CLR_RED), strest());
                    free(json_data);
                    return NULL;
                }
                json_data = new_data;
            }
            json_data[size++] = (char)c;
        }
        json_data[size] = '\0';
        
        if (size == 0) {
            // 没有数据，返回NULL
            free(json_data);
            return NULL;
        }
    }

    JsonTreeNode* tree = parseJson(json_data);
    free(json_data);

    if (!tree) {
        printf("%sError:%s Failed to parse JSON\n", Rmkans(CLR_RED), strest());
        return NULL;
    }

    return tree;
}

int cmd_config(const char* config_file, ProgramConfig* config) {
    // 加载配置文件
    // 优先级：用户指定的配置文件 > 当前目录的 .jst.json > 默认配置
    int config_loaded = 0;
    
    if (config_file) {
        // 使用用户指定的配置文件
        if (config_load(config_file, config) == 0) {
            config_loaded = 1;
        }
    } else {
        // 尝试加载当前目录的 .jst.json
        if (config_load(".jst.json", config) == 0) {
            config_loaded = 1;
        }
    }
    
    // 如果没有加载成功，使用默认配置
    if (!config_loaded) {
        ProgramConfig* default_config = config_get_default();
        if (default_config) {
            memcpy(config, default_config, sizeof(ProgramConfig));
        }
    }
    
    return 0;
}

int cmd_generate_config(const char* output_path) {
    // 确定配置文件路径
    const char* path = output_path;
    char default_path[256];
    
    if (!path) {
        // 尝试当前目录的 .jst.json
        snprintf(default_path, sizeof(default_path), ".jst.json");
        path = default_path;
        
        // 检查是否可以写入（但不创建文件）
        const char* home = getenv("HOME");
        char home_path[256];
        if (home) {
            snprintf(home_path, sizeof(home_path), "%s/.jst.json", home);
        }
        
        // 这里不打开文件，只是确定路径
        // 如果当前目录不可写，会在生成时失败，然后尝试 home 目录
    }
    
    // 检查文件是否已存在
    if (access(path, F_OK) == 0) {
        printf("Config file already exists: %s\n", path);
        printf("Overwrite? (y/N): ");
        
        char response[10];
        if (fgets(response, sizeof(response), stdin)) {
            // 移除换行符
            if (response[strlen(response) - 1] == '\n') {
                response[strlen(response) - 1] = '\0';
            }
            
            if (strcmp(response, "y") != 0 && strcmp(response, "Y") != 0) {
                printf("Cancelled.\n");
                return 0;
            }
        } else {
            printf("Cancelled.\n");
            return 0;
        }
    }
    
    // 尝试生成配置文件
    printf("Generating config file: %s\n", path);
    int result = config_generate_default_file(path);
    
    // 如果失败且是默认路径，尝试 home 目录
    if (result != 0 && !output_path) {
        const char* home = getenv("HOME");
        if (home) {
            snprintf(default_path, sizeof(default_path), "%s/.jst.json", home);
            printf("Trying home directory: %s\n", default_path);
            result = config_generate_default_file(default_path);
            if (result == 0) {
                printf("✓ Config file generated successfully!\n");
                return 0;
            }
        }
    }
    
    if (result == 0) {
        printf("✓ Config file generated successfully!\n");
    } else {
        printf("✗ Failed to generate config file!\n");
        return 1;
    }
    
    return 0;
}

/**
 * 递归打印JSON树节点
 * 
 * @param node     当前要打印的节点
 * @param depth    当前节点的深度（根节点的子节点depth=0）
 * @param is_last  状态数组，is_last[i]表示从根到父节点路径上，第i层节点是否是最后一个兄弟
 *                 - is_last[i]=1: 第i层已结束，子节点缩进时输出空格
 *                 - is_last[i]=0: 第i层还有后续节点，子节点缩进时输出 │
 * 
 * 实现原理：
 * 1. 缩进输出：遍历0到depth-1层，根据is_last数组决定输出空格还是 │
 * 2. 连接符：根据当前节点是否是最后一个兄弟，输出 ├── 或 └──
 * 3. 节点信息：输出键名/索引和值
 * 4. 递归处理：对于对象/数组类型，预计算所有子节点的is_last状态，然后递归
 * 
 * 关键设计：
 * - 每次递归前预计算所有子节点的is_last状态，避免在递归过程中修改共享状态
 * - 为每个子节点创建新的is_last数组，传递给递归调用
 * - is_last[depth]表示当前节点本身的状态，子节点使用is_last[depth+1]来判断父节点状态
 * - max_depth表示最大显示深度，-1表示无限制
 */
static void print_node(JsonTreeNode* node, int depth, int* is_last, int max_depth) {
    if (!node) return;
    
    // 定义树形字符
    const char* tree_branch = "├──";  // 有后续兄弟的节点
    const char* tree_leaf = "└──";    // 最后一个节点
    const char* tree_space = "    ";  // 空格
    
    // ===== 第一步：输出缩进 =====
    // 遍历从根到父节点的所有层级，根据每层的状态决定缩进字符
    for (int i = 0; i < depth; i++) {
        if (is_last[i]) {
            // 第i层已经结束（父节点是最后一个兄弟），输出空格
            printf("%s", tree_space);
        } else {
            // 第i层还有后续节点（父节点后面还有兄弟），输出 │
            printf("\033[%d;%dm│   \033[0m", g_config.tree.style, g_config.tree.fg);
        }
    }
    
    // ===== 第二步：确定当前节点是否是最后一个兄弟 =====
    // 直接从is_last数组中读取当前节点的状态
    int is_last_child = is_last[depth];
    
    // ===== 第三步：输出连接符 =====
    // depth>=0时才输出连接符（包括第一层节点）
    if (depth >= 0) {
        printf("\033[%d;%dm%s\033[0m", g_config.tree.style, g_config.tree.fg, is_last_child ? tree_leaf : tree_branch);
    }
    
    // ===== 第四步：输出节点名称 =====
    if (node->parent && node->parent->type == JSON_OBJECT && node->identifier.key) {
        // 对象成员：输出键名
        printf("\033[%d;%dm%s\033[0m: ", g_config.json_object.key.style, g_config.json_object.key.fg, node->identifier.key);
    } else if (node->parent && node->parent->type == JSON_ARRAY) {
        // 数组元素：输出索引
        printf("\033[%d;%dm[%d]\033[0m: ", g_config.json_array.key.style, g_config.json_array.key.fg, node->identifier.index);
    } else {
        // 根节点（理论上不会到这里，因为根节点由json_tree函数单独处理）
        printf("root: ");
    }
    
    // ===== 第五步：根据类型输出值或递归处理子节点 =====
    if (node->type == JSON_OBJECT || node->type == JSON_ARRAY) {
        TypeColorConfig* type_config = (node->type == JSON_OBJECT) ? &g_config.json_object : &g_config.json_array;
        
        // 对象或数组类型
        if (node->child_count == 0) {
            // 空对象或数组
            printf("\033[%d;%dm%s\033[0m\n", type_config->value.style, type_config->value.fg, node->type == JSON_ARRAY ? "[]" : "{}");
        } else {
            // 有子节点，输出类型信息
            printf("\033[%d;%dm%s (%d %s)\033[0m\n", type_config->value.style, type_config->value.fg,
                   node->type == JSON_ARRAY ? "Array" : "Object",
                   node->child_count,
                   node->type == JSON_ARRAY ? "elements" : "children");
            
            // ===== 第六步：递归处理子节点 =====
            // 直接在循环中为每个子节点设置is_last状态并递归
            
            for (JsonTreeNode* child = node->first_child; child; child = child->next_sibling) {
                // 检查是否超过最大深度
                if (max_depth >= 0 && depth >= max_depth) {
                    break;  // 超过最大深度，不再展开子节点
                }
                
                // 为每个子节点创建新的is_last数组
                // 大小为depth+2，因为需要存储depth+1层的状态
                int* new_is_last = malloc(sizeof(int) * (depth + 2));
                
                // 复制父节点的状态（0到depth层）
                for (int i = 0; i < depth + 1; i++) {
                    new_is_last[i] = is_last[i];
                }
                
                // 添加当前子节点的状态（depth+1层）
                // 子节点在递归时会使用is_last[depth+1]来判断当前节点是否是最后一个兄弟
                new_is_last[depth + 1] = (child->next_sibling == NULL);
                
                // 递归打印子节点，深度+1
                print_node(child, depth + 1, new_is_last, max_depth);
                
                // 释放临时数组
                free(new_is_last);
            }
        }
    } else {
        // 基本类型值（STRING, NUMBER, BOOL, NULL）
        switch (node->type) {
            case JSON_STRING:
                printf("\033[%d;%dm\"%s\"\033[0m\n", g_config.json_string.value.style, g_config.json_string.value.fg, node->value.string_val);
                break;
            case JSON_NUMBER:
                printf("\033[%d;%dm%g\033[0m\n", g_config.json_number.value.style, g_config.json_number.value.fg, node->value.number_val);
                break;
            case JSON_BOOL:
                printf("\033[%d;%dm%s\033[0m\n", g_config.json_bool.value.style, g_config.json_bool.value.fg, node->value.bool_val ? "true" : "false");
                break;
            case JSON_NULL:
                printf("\033[%d;%dmnull\033[0m\n", g_config.json_null.value.style, g_config.json_null.value.fg);
                break;
            default:
                printf("\n");
                break;
        }
    }
}

/**
 * 输出JSON树结构
 * 
 * @param tree       JSON树的根节点
 * @param max_depth  最大显示深度（-1表示无限制）
 * @param verbose    是否显示详细信息（包括耗时）
 * 
 * 实现原理：
 * 1. 输出蓝色的"."作为根节点标识
 * 2. 初始化is_last数组，记录每层节点是否是最后一个兄弟
 * 3. 从根节点的子节点开始递归打印
 * 4. 如果verbose为true，显示耗时信息
 * 
 * is_last数组说明：
 * - is_last[i] 表示从根到父节点路径上，第i层节点是否是最后一个兄弟
 * - 初始化为0，然后在遍历子节点时逐个设置
 * - 例如：is_last[0]=1表示根节点的第一个子节点是最后一个兄弟
 */
void json_tree(JsonTreeNode* tree, int max_depth, int verbose) {
    if (!tree) return;
    
    // 记录开始时间
    clock_t start_time = clock();
    
    // 输出根节点标识（使用配置中的树颜色）
    printf("\033[%d;%dm.\033[0m\n", g_config.tree.style, g_config.tree.fg);
    
    // 定义状态数组，记录每层节点是否是最后一个兄弟
    // is_last[i] = 1 表示第i层已经结束，输出空格
    // is_last[i] = 0 表示第i层还有后续节点，输出 │
    int is_last[64] = {0};
    
    // 从根节点的子节点开始打印，depth=0 表示第一层节点
    for (JsonTreeNode* child = tree->first_child; child; child = child->next_sibling) {
        // 设置 is_last[0] 表示当前子节点是否是最后一个兄弟
        // 这个状态会被传递给子节点，用于决定缩进格式
        is_last[0] = (child->next_sibling == NULL);
        print_node(child, 0, is_last, max_depth);
    }
    
    // 如果verbose为true，显示耗时信息
    if (verbose) {
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000;  // 转换为毫秒
        printf("\n%s--- Time taken: %.3f ms ---%s\n", Rmkans(CLR_CYAN), elapsed_time, strest());
    }
}

int main(int argc, char* argv[]) {
    int opt;
    int verbose = 0;
    int help = 0;
    int max_depth = -1;  // -1 表示无限制
    int show_detail = 0;  // 是否显示详细信息（包括时间）
    int generate_config = 0;  // 是否生成配置文件
    const char* input_file = NULL;
    const char* config_file = NULL;  // 配置文件路径
    const char* generate_config_file = NULL;  // 生成配置文件的路径

    static struct option long_options[] = {
        {"verbose",         no_argument,       0, 'v'},
        {"help",            no_argument,       0, 'h'},
        {"detail",          no_argument,       0, 'd'},
        {"max-depth",       required_argument, 0, 'L'},
        {"input",           required_argument, 0, 'i'},
        {"config",          required_argument, 0, 'C'},
        {"generate-config", optional_argument, 0, 1000},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "vhdL:i:C:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'h':
                help = 1;
                break;
            case 'd':
                show_detail = 1;
                break;
            case 'L':
                max_depth = atoi(optarg);
                if (max_depth < 0) max_depth = -1;
                break;
            case 'i':
                input_file = optarg;
                break;
            case 'C':
                config_file = optarg;
                break;
            case 1000:  // --generate-config
                generate_config = 1;
                generate_config_file = optarg;
                break;
            default:
                break;
        }
    }

    // 处理各个命令
    if (help) {
        cmd_help();
        return 0;
    }

    if (verbose) {
        cmd_verbose();
        return 0;
    }

    // 如果是生成配置文件模式
    if (generate_config) {
        return cmd_generate_config(generate_config_file);
    }

    // 获取输入文件
    if (!input_file && optind < argc) {
        input_file = argv[optind];
    }

    // 如果没有输入文件，从标准输入读取
    if (!input_file) {
        // 检查标准输入是否有数据
        if (isatty(STDIN_FILENO)) {
            // 交互式终端，没有输入
            printf("%sError:%s No input file specified\n", Rmkans(CLR_RED), strest());
            printf("Usage: %s <file> | %s\n", PROGRAM_NAME, PROGRAM_NAME);
            printf("       %s <file> [options]\n", PROGRAM_NAME);
            return 1;
        }
        // 从标准输入读取（input_file = NULL）
    }

    // 加载配置
    cmd_config(config_file, &g_config);

    // 处理输入
    JsonTreeNode* tree = cmd_input(input_file);
    if (!tree) {
        return 1;
    }

    // 输出树形结构（show_detail控制是否显示时间）
    json_tree(tree, max_depth, show_detail);

    freeJson(tree);
    return 0;
}