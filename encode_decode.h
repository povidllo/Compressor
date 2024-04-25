
#include <stdio.h>
#include "tree.h"

TreeNode* Decode_tree(FILE* file);

void Decode(FILE* input, FILE* output, TreeNode* tree, long long size);

void Encode_tree(TreeNode* node, FILE* file);

void Encode(FILE* input, FILE* output, Code* codes, TreeNode* tree, long long size);
