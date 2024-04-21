#include <stdint.h>

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

void dfs_tree(TreeNode* tree, Code* codes, uint32_t bin_code, uint32_t len);