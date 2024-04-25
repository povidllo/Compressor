#pragma once
#include <stdint.h>
#include <stdio.h>


typedef struct TreeNode {
	long long count;
	uint8_t sym;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;

typedef struct Heap {
	TreeNode** arr;
	int size;
	int cap;
}Heap;

TreeNode* BuildHuffmanTree(int n, long long* f);

typedef struct Code {
	uint32_t bin_code;
	uint32_t len;
}Code;

void DFS_tree(TreeNode* tree, Code* codes, uint32_t bin_code, uint32_t len);

TreeNode* Decode_tree(FILE* file);

void Decode(FILE* input, FILE* output, TreeNode* tree, long long size);

void Encode_tree(TreeNode* node, FILE* file);

void Encode(FILE* input, FILE* output, Code* codes, TreeNode* tree, long long size);
