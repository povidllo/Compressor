#pragma once
typedef struct TreeNode {
	int count;
	uint8_t sym;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;

typedef struct Heap {
	TreeNode** arr;
	int size;
	int cap;
}Heap;

TreeNode* BuildHuffmanTree(int n, int* f);