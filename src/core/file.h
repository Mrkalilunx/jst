#ifndef FILE_H
#define FILE_H

#include "cJSON.h"

char* read_and_process_file(const char* file_path);
cJSON* parse_json_data(const char* json_data);

#endif