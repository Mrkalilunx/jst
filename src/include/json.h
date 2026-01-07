#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include "color.h"

extern int max_depth;

char* preprocess_json(const char* json_data);

#endif