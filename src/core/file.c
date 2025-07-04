#include "file.h"
#include "color.h"
#include "json.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define INIT_BUFFER_SIZE 4096

static void handle_file_error(const char* msg, const char* path) {
    fprintf(stderr, "Error: %s '%s': %s\n", msg, path, strerror(errno));
    exit(EXIT_FAILURE);
}

static char* read_from_stdin() {
    size_t capacity = INIT_BUFFER_SIZE;
    char* buffer = malloc(capacity);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t total_read = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer + total_read, capacity - total_read))) {
        if (bytes_read < 0) {
            if (errno == EINTR) continue;
            free(buffer);
            handle_file_error("Error reading stdin", "-");
        }

        total_read += bytes_read;
        if (total_read == capacity) {
            capacity *= 2;
            char* new_buffer = realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                fprintf(stderr, "Error: Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
            buffer = new_buffer;
        }
    }

    if (total_read == 0) {
        free(buffer);
        fprintf(stderr, "Error: Empty input from stdin\n");
        exit(EXIT_FAILURE);
    }

    buffer[total_read] = '\0';
    return buffer;
}

static char* read_from_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        handle_file_error("Could not open file", file_path);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        handle_file_error("Could not determine size of file", file_path);
    }
    
    rewind(file);
    char* buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(buffer, 1, size, file);
    fclose(file);
    
    if (read_size != (size_t)size) {
        free(buffer);
        handle_file_error("Could not read entire file", file_path);
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
        fprintf(stderr, "Error: JSON preprocessing failed\n");
        exit(EXIT_FAILURE);
    }
    
    return processed_data;
}

cJSON* parse_json_data(const char* json_data) {
    cJSON* root = cJSON_Parse(json_data);
    free((char*)json_data);
    
    if (!root) {
        const char* error_ptr = cJSON_GetErrorPtr();
        fprintf(stderr, "JSON parse error: %s\n", error_ptr ? error_ptr : "Unknown error");
        exit(EXIT_FAILURE);
    }
    
    return root;
}