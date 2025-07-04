#ifndef TREE_H
#define TREE_H

#include "color.h"
#include "args.h"
#include "cJSON.h"

void print_tree(cJSON* item, int depth, int is_last, int current_depth, 
               const int* is_last_list, cJSON* parent, int idx);
void print_tree_with_root(cJSON* root);

#endif