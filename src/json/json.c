#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char* illegal;
    const char* replacement;
} ValueReplacement;

static const ValueReplacement REPLACEMENTS[] = {
    {"-Infinity", "-1.7976931348623157e+308"},
    {"Infinity", "1.7976931348623157e+308"},
    {"NaN", "null"},
    {"nan", "null"}
};

static const size_t REPLACEMENT_COUNT = sizeof(REPLACEMENTS) / sizeof(REPLACEMENTS[0]);

static int is_valid_boundary(const char* data, size_t pos, size_t len) {
    return (pos == 0 || !isalnum((unsigned char)data[pos-1])) &&
           (pos + len == strlen(data) || !isalnum((unsigned char)data[pos+len]));
}

char* preprocess_json(const char* json_data) {
    if (!json_data) return NULL;

    size_t input_len = strlen(json_data);
    size_t output_size = input_len * 2;
    char* output = malloc(output_size);
    if (!output) return NULL;

    size_t out_pos = 0;
    int in_string = 0, escape = 0;

    for (size_t i = 0; i < input_len; i++) {
        if (out_pos + 50 >= output_size) {
            output_size *= 2;
            char* new_output = realloc(output, output_size);
            if (!new_output) {
                free(output);
                return NULL;
            }
            output = new_output;
        }

        if (in_string) {
            if (escape) {
                escape = 0;
            } else if (json_data[i] == '\\') {
                escape = 1;
            } else if (json_data[i] == '"') {
                in_string = 0;
            }
            output[out_pos++] = json_data[i];
            continue;
        }

        int replaced = 0;
        for (size_t j = 0; j < REPLACEMENT_COUNT; j++) {
            const char* illegal = REPLACEMENTS[j].illegal;
            size_t illegal_len = strlen(illegal);
            
            if (i + illegal_len > input_len) continue;
            if (strncmp(&json_data[i], illegal, illegal_len) != 0) continue;
            if (!is_valid_boundary(json_data, i, illegal_len)) continue;
            
            const char* replacement = REPLACEMENTS[j].replacement;
            size_t replacement_len = strlen(replacement);
            
            if (out_pos + replacement_len >= output_size) {
                output_size = out_pos + replacement_len + 1024;
                char* new_output = realloc(output, output_size);
                if (!new_output) {
                    free(output);
                    return NULL;
                }
                output = new_output;
            }
            
            memcpy(&output[out_pos], replacement, replacement_len);
            out_pos += replacement_len;
            i += illegal_len - 1;
            replaced = 1;
            break;
        }
        
        if (replaced) continue;
        
        if (json_data[i] == '"') {
            in_string = 1;
        }
        output[out_pos++] = json_data[i];
    }

    output[out_pos] = '\0';
    return realloc(output, out_pos + 1);
}