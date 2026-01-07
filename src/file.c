#include "file.h"
#include "color.h"
#include "json.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define INIT_BUFFER_SIZE 4096

static void handle_file_error(const char* path) {
    fprintf(stderr, "jst: %s: %s\n", path, strerror(errno));
    exit(EXIT_FAILURE);
}

static void* check_alloc(void* ptr) {
    if (!ptr) {
        fprintf(stderr, "jst: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

static char* read_from_stdin() {
    size_t capacity = INIT_BUFFER_SIZE;
    char* buffer = check_alloc(malloc(capacity));
    size_t total_read = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer + total_read, capacity - total_read))) {
        if (bytes_read < 0) {
            if (errno == EINTR) continue;
            free(buffer);
            handle_file_error("-");
        }

        total_read += bytes_read;
        if (total_read == capacity) {
            capacity *= 2;
            buffer = check_alloc(realloc(buffer, capacity));
        }
    }

    if (total_read == 0) {
        free(buffer);
        fprintf(stderr, "jst: stdin: empty input\n");
        exit(EXIT_FAILURE);
    }

    buffer[total_read] = '\0';
    return buffer;
}

static char* read_from_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        handle_file_error(file_path);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        handle_file_error(file_path);
    }
    
    rewind(file);
    char* buffer = check_alloc(malloc(size + 1));
    size_t read_size = fread(buffer, 1, size, file);
    fclose(file);
    
    if (read_size != (size_t)size) {
        free(buffer);
        handle_file_error(file_path);
    }
    
    buffer[size] = '\0';
    return buffer;
}

char* read_and_process_file(const char* file_path) {
    char* json_data = (strcmp(file_path, "-") == 0) ? 
                      read_from_stdin() : 
                      read_from_file(file_path);

    char* processed_data = preprocess_json(json_data);
    free(json_data);
    
    if (!processed_data) {
        fprintf(stderr, "jst: JSON preprocessing failed\n");
        exit(EXIT_FAILURE);
    }
    
    return processed_data;
}

cJSON* parse_json_data(char* json_data) {
    cJSON* root = cJSON_Parse(json_data);
    free(json_data);
    
    if (!root) {
        const char* error_ptr = cJSON_GetErrorPtr();
        fprintf(stderr, "jst: JSON parse error: %s\n", error_ptr ? error_ptr : "Unknown error");
        exit(EXIT_FAILURE);
    }
    
    return root;
}