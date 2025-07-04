/*
  版权所有 (c) 2009-2017 Dave Gamble 和 cJSON 贡献者

  特此免费授予任何获得本软件及相关文档文件（“软件”）副本的人，
  无限制地处理本软件，包括但不限于使用、复制、修改、合并、发布、分发、再授权和/或销售
  本软件的副本，并允许向其提供本软件的人这样做，但须符合以下条件：

  上述版权声明和本许可声明应包含在
  本软件的所有副本或主要部分中。

  本软件按“原样”提供，不提供任何形式的明示或暗示保证，包括但不限于适销性保证、
  特定用途适用性和非侵权保证。在任何情况下，作者或版权持有人均不对
  任何索赔、损害或其他责任负责，无论是在合同诉讼、侵权行为还是其他方面，
  由本软件引起或与本软件的使用或其他交易相关。
*/

#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C"
{
#endif

// Windows 平台检测宏
#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* 为 Windows 编译时，我们指定特定的调用约定，以避免从使用不同默认调用约定的项目中调用时出现问题。
   对于 Windows，你有 3 个定义选项：

   CJSON_HIDE_SYMBOLS - 如果你永远不想 dllexport 符号，请定义此选项
   CJSON_EXPORT_SYMBOLS - 在库构建时，如果你想 dllexport 符号，请定义此选项 (默认)
   CJSON_IMPORT_SYMBOLS - 如果你想 dllimport 符号，请定义此选项

   对于支持 visibility 属性的 *nix 构建，你可以通过以下方式定义类似行为：

   通过添加以下内容将默认可见性设置为隐藏：
     -fvisibility=hidden (对于 gcc)
     或
     -xldscope=hidden (对于 sun cc)
   到 CFLAGS

   然后使用 CJSON_API_VISIBILITY 标志以与 CJSON_EXPORT_SYMBOLS 相同的方式“导出”相同的符号
*/

#define CJSON_CDECL __cdecl
#define CJSON_STDCALL __stdcall

/* 默认导出符号，这对于复制粘贴 C 和头文件是必要的 */
#if !defined(CJSON_HIDE_SYMBOLS) && !defined(CJSON_IMPORT_SYMBOLS) && !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type)   type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllimport) type CJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define CJSON_CDECL
#define CJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
#endif

/* 项目版本 */
#define CJSON_VERSION_MAJOR 1  // 主版本号
#define CJSON_VERSION_MINOR 7  // 次版本号
#define CJSON_VERSION_PATCH 18 // 修订号

#include <stddef.h> // 包含 size_t 定义

/* cJSON 类型: */
#define cJSON_Invalid (0)     // 无效
#define cJSON_False  (1 << 0) // 假
#define cJSON_True   (1 << 1) // 真
#define cJSON_NULL   (1 << 2) // 空
#define cJSON_Number (1 << 3) // 数字
#define cJSON_String (1 << 4) // 字符串
#define cJSON_Array  (1 << 5) // 数组
#define cJSON_Object (1 << 6) // 对象
#define cJSON_Raw    (1 << 7) // 原始 JSON 字符串

#define cJSON_IsReference 256   // 表示该项是引用（非拥有）
#define cJSON_StringIsConst 512 // 表示字符串值是常量（不应被修改/释放）

/* cJSON 结构体: */
typedef struct cJSON
{
    /* next/prev 允许你遍历数组/对象链。或者，使用 GetArraySize/GetArrayItem/GetObjectItem */
    struct cJSON *next; // 指向同级的下一个项
    struct cJSON *prev; // 指向同级的上一个项
    /* 数组或对象项将有一个 child 指针，指向该数组/对象中所有项的链。 */
    struct cJSON *child; // 指向第一个子项（如果是数组/对象）

    /* 项的类型，如上所述。 */
    int type; // 存储上述类型标志 (cJSON_Invalid, cJSON_False 等)

    /* 项的值字符串（如果 type == cJSON_String 或 type == cJSON_Raw） */
    char *valuestring;
    /* 写入 valueint 已弃用，请改用 cJSON_SetNumberValue */
    int valueint;       // 整数值（已弃用，优先使用 valuedouble）
    /* 项的数字值（如果 type == cJSON_Number） */
    double valuedouble; // 双精度浮点数值

    /* 项的名称字符串（如果此项是对象的子项或在对象的子项列表中）。 */
    char *string; // 键名（Key）
} cJSON;

/* 钩子（Hooks）结构体，用于自定义内存管理 */
typedef struct cJSON_Hooks
{
      /* malloc/free 在 Windows 上始终是 CDECL，无论编译器的默认调用约定如何，因此确保钩子允许直接传递这些函数。 */
      void *(CJSON_CDECL *malloc_fn)(size_t sz); // 自定义内存分配函数指针
      void (CJSON_CDECL *free_fn)(void *ptr);    // 自定义内存释放函数指针
} cJSON_Hooks;

/* cJSON 布尔类型 */
typedef int cJSON_bool;

/* 限制数组/对象在 cJSON 拒绝解析它们之前可以嵌套的深度。
 * 这是为了防止栈溢出。 */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000 // 默认嵌套深度限制
#endif

/* 限制循环引用在 cJSON 拒绝解析它们之前的长度。
 * 这是为了防止栈溢出。 */
#ifndef CJSON_CIRCULAR_LIMIT
#define CJSON_CIRCULAR_LIMIT 10000 // 默认循环引用检测限制
#endif

/* 将 cJSON 的版本作为字符串返回 */
CJSON_PUBLIC(const char*) cJSON_Version(void);

/* 向 cJSON 提供 malloc、realloc 和 free 函数 */
CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks* hooks);

/* 内存管理：调用者始终负责释放所有 cJSON_Parse 变体（使用 cJSON_Delete）和 cJSON_Print（使用 stdlib free、cJSON_Hooks.free_fn 或 cJSON_free，视情况而定）的结果。
   例外是 cJSON_PrintPreallocated，调用者对该缓冲区负全部责任。 */
/* 提供一个 JSON 字符串块，此函数返回一个你可以查询的 cJSON 对象。 */
CJSON_PUBLIC(cJSON *) cJSON_Parse(const char *value); // 解析以 null 结尾的 JSON 字符串
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLength(const char *value, size_t buffer_length); // 解析带显式长度的 JSON 数据
/* ParseWithOpts 允许你要求（并检查）JSON 是否以 null 结尾，并检索指向解析的最后字节的指针。 */
/* 如果在 return_parse_end 中提供了一个指针并且解析失败，那么 return_parse_end 将包含指向错误的指针，因此将与 cJSON_GetErrorPtr() 匹配。 */
CJSON_PUBLIC(cJSON *) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, cJSON_bool require_null_terminated);
CJSON_PUBLIC(cJSON *) cJSON_ParseWithLengthOpts(const char *value, size_t buffer_length, const char **return_parse_end, cJSON_bool require_null_terminated);

/* 将 cJSON 实体渲染为文本以进行传输/存储。 */
CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item); // 格式化的 JSON 字符串
/* 将 cJSON 实体渲染为文本以进行传输/存储，不带任何格式。 */
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item); // 未格式化的 JSON 字符串
/* 使用缓冲策略将 cJSON 实体渲染为文本。prebuffer 是对最终大小的预估。预估准确可减少重新分配。fmt=0 表示未格式化，=1 表示格式化 */
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON *item, int prebuffer, cJSON_bool fmt);
/* 使用内存中已分配的给定长度的缓冲区将 cJSON 实体渲染为文本。成功返回 1，失败返回 0。 */
/* 注意：cJSON 在估计将使用多少内存时并不总是 100% 准确，因此为了安全起见，请分配比你实际需要多 5 个字节 */
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length, const cJSON_bool format);
/* 删除一个 cJSON 实体及其所有子实体。 */
CJSON_PUBLIC(void) cJSON_Delete(cJSON *item);

/* 返回数组（或对象）中的项数。 */
CJSON_PUBLIC(int) cJSON_GetArraySize(const cJSON *array);
/* 从数组 "array" 中检索序号为 "index" 的项。如果不成功则返回 NULL。 */
CJSON_PUBLIC(cJSON *) cJSON_GetArrayItem(const cJSON *array, int index);
/* 从对象中获取键为 "string" 的项。不区分大小写。(已弃用，推荐使用 cJSON_GetObjectItemCaseSensitive) */
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string);
/* 从对象中获取键为 "string" 的项。区分大小写。(推荐) */
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItemCaseSensitive(const cJSON * const object, const char * const string);
/* 检查对象是否包含键为 "string" 的项。 */
CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string);
/* 用于分析失败的解析。这返回一个指向解析错误的指针。你可能需要回看几个字符才能理解它。当 cJSON_Parse() 返回 0 时定义。当 cJSON_Parse() 成功时为 0。 */
CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void);

/* 检查项类型并返回其值 */
CJSON_PUBLIC(char *) cJSON_GetStringValue(const cJSON * const item); // 获取字符串值（如果是字符串类型）
CJSON_PUBLIC(double) cJSON_GetNumberValue(const cJSON * const item); // 获取数值（如果是数字类型）

/* 这些函数检查项的类型 */
CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON * const item); // 是否无效
CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON * const item);   // 是否假值
CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON * const item);    // 是否真值
CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON * const item);    // 是否布尔值（真或假）
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON * const item);    // 是否空值
CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON * const item);  // 是否数字
CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON * const item);  // 是否字符串
CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON * const item);   // 是否数组
CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON * const item);  // 是否对象
CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON * const item);     // 是否原始 JSON 字符串

/* 这些调用创建相应类型的 cJSON 项。 */
CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void);    // 创建 Null 项
CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void);    // 创建 True 项
CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void);   // 创建 False 项
CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool boolean); // 创建布尔项
CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num);      // 创建数字项
CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string); // 创建字符串项
/* 原始 json */
CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw); // 创建原始 JSON 字符串项
CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void);  // 创建空数组项
CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void); // 创建空对象项

/* 创建一个字符串项，其中 valuestring 引用一个外部字符串，
 * 因此它不会被 cJSON_Delete 释放 */
CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string);
/* 创建一个仅引用其元素的对象/数组项，
 * 因此这些元素不会被 cJSON_Delete 释放 */
CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const cJSON *child); // 创建对象引用项
CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const cJSON *child);  // 创建数组引用项

/* 这些实用函数创建包含 count 个项的数组。
 * 参数 count 不能大于数字数组中的元素数量，否则数组访问将越界。*/
CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count);    // 创建整数数组
CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count); // 创建浮点数数组
CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count); // 创建双精度浮点数数组
CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char *const *strings, int count); // 创建字符串数组

/* 将项追加到指定的数组/对象。 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToArray(cJSON *array, cJSON *item); // 添加项到数组
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item); // 添加键值对到对象
/* 当 string 绝对是常量（即字面量，或等效的）且肯定比 cJSON 对象生命周期长时使用此函数。
 * 警告：使用此函数时，务必在写入 `item->string` 之前检查 (item->type & cJSON_StringIsConst) 是否为零 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item);
/* 将项的引用追加到指定的数组/对象。当你想要将现有的 cJSON 项添加到新的 cJSON 中，但又不想破坏你现有的 cJSON 时使用此函数。 */
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item); // 添加引用到数组
CJSON_PUBLIC(cJSON_bool) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item); // 添加引用到对象

/* 从数组/对象中移除/分离项。 */
CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON * const item); // 通过指针分离项
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which); // 从数组中分离指定索引的项
CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which);     // 从数组中删除指定索引的项（包含分离和删除）
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string); // 从对象中分离指定键的项（不区分大小写）
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string); // 从对象中分离指定键的项（区分大小写）
CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string); // 从对象中删除指定键的项（不区分大小写，包含分离和删除）
CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string); // 从对象中删除指定键的项（区分大小写，包含分离和删除）

/* 更新数组项。 */
CJSON_PUBLIC(cJSON_bool) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem); /* 将新项插入数组指定位置，将原有项向右移动。 */
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON * const parent, cJSON * const item, cJSON * replacement); // 通过指针替换项
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem); // 替换数组中指定索引的项
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem); // 替换对象中指定键的项（不区分大小写）
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object, const char *string, cJSON *newitem); // 替换对象中指定键的项（区分大小写）

/* 复制一个 cJSON 项 */
CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse);
/* Duplicate 将创建一个与你传递的项相同的新 cJSON 项，位于需要释放的新内存中。
 * 当 recurse!=0 时，它将复制连接到该项的任何子项。
 * 从 Duplicate 返回时，item->next 和 ->prev 指针始终为零。 */
/* 递归比较两个 cJSON 项是否相等。如果 a 或 b 为 NULL 或无效，它们将被视为不相等。
 * case_sensitive 决定对象键是否区分大小写（1 区分，0 不区分） */
CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON * const a, const cJSON * const b, const cJSON_bool case_sensitive);

/* 最小化字符串，从字符串中移除空白字符（如 ' ', '\t', '\r', '\n'）。
 * 输入指针 json 不能指向只读地址区域（例如字符串常量），
 * 而应指向可读写的地址区域。 */
CJSON_PUBLIC(void) cJSON_Minify(char *json);

/* 用于同时创建项并将其添加到对象的辅助函数。
 * 成功时返回添加的项，失败时返回 NULL。 */
CJSON_PUBLIC(cJSON*) cJSON_AddNullToObject(cJSON * const object, const char * const name);    // 添加 Null 项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddTrueToObject(cJSON * const object, const char * const name);    // 添加 True 项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddFalseToObject(cJSON * const object, const char * const name);   // 添加 False 项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean); // 添加布尔项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number);    // 添加数字项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string); // 添加字符串项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw);      // 添加原始 JSON 字符串项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddObjectToObject(cJSON * const object, const char * const name); // 添加空对象项到对象
CJSON_PUBLIC(cJSON*) cJSON_AddArrayToObject(cJSON * const object, const char * const name);  // 添加空数组项到对象

/* 当分配整数值时，它也需要传播到 valuedouble。 */
#define cJSON_SetIntValue(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* cJSON_SetNumberValue 宏的辅助函数 */
CJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number);
#define cJSON_SetNumberValue(object, number) ((object != NULL) ? cJSON_SetNumberHelper(object, (double)number) : (number))
/* 更改 cJSON_String 对象的 valuestring，仅当对象类型为 cJSON_String 时才生效 */
CJSON_PUBLIC(char*) cJSON_SetValuestring(cJSON *object, const char *valuestring);

/* 如果对象不是布尔类型，此宏不执行任何操作并返回 cJSON_Invalid，否则返回新类型 */
#define cJSON_SetBoolValue(object, boolValue) ( \
    (object != NULL && ((object)->type & (cJSON_False|cJSON_True))) ? /* 检查是布尔类型 */ \
    (object)->type=((object)->type &(~(cJSON_False|cJSON_True)))|((boolValue)?cJSON_True:cJSON_False) : /* 设置新布尔值 */ \
    cJSON_Invalid /* 非布尔类型返回无效 */ \
)

/* 用于遍历数组或对象的宏 */
#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)

/* 使用通过 cJSON_InitHooks 设置的 malloc/free 函数分配/释放对象 */
CJSON_PUBLIC(void *) cJSON_malloc(size_t size); // 使用注册的分配函数
CJSON_PUBLIC(void) cJSON_free(void *object);    // 使用注册的释放函数

#ifdef __cplusplus
}
#endif

#endif /* cJSON__h */