#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//parent = (id - 1)/1
//left = id * 2 + 1
//right = id * 2 + 2

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

Heap Create_heap(int cap)//Create a heap
{
	Heap heap;
	heap.cap = cap;
	heap.arr = (TreeNode**)malloc((heap.cap + 1) * sizeof(TreeNode*));
	heap.size = 0;
	return heap;
}

void Heapify(Heap* heap, int id)//Heapify up to bottom
{
	int left = id * 2 + 1;
	int right = id * 2 + 2;
	int smallest = id;

	if (left < heap->size && heap->arr[left]->count < heap->arr[smallest]->count)
	{
		smallest = left;
	}
	if (right < heap->size && heap->arr[right]->count < heap->arr[smallest]->count)
	{
		smallest = right;
	}
	if (smallest != id)
	{
		TreeNode* temp = heap->arr[smallest];
		heap->arr[smallest] = heap->arr[id];
		heap->arr[id] = temp;
		Heapify(heap, smallest);
	}
}

void Insert_heap(Heap* heap, TreeNode* val)//Instert in heap
{
	heap->arr[heap->size] = val;

	int cur = heap->size;
	int parent = (cur - 1) / 2;

	while (cur != 0 && val->count < heap->arr[parent]->count)
	{

		TreeNode* temp = heap->arr[parent];
		heap->arr[parent] = heap->arr[cur];
		heap->arr[cur] = temp;
		cur = parent;
		parent = (cur - 1) / 2;
	}

	heap->size++;
}

TreeNode* Extract_min_heap(Heap* heap)//Extract min from heap
{
	TreeNode* min = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size - 1];
	heap->size--;

	Heapify(heap, 0);
	return min;
}


TreeNode* NewTreeFromSym(int sym, long long size)//Create a new tree with one element
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	res->count = size;
	res->sym = sym;
	res->left = res->right = NULL;
	return res;
}

TreeNode* NewTreeFromTwoTrees(TreeNode* tree0, TreeNode* tree1)//Create a new tree from two trees
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	res->count = tree0->count + tree1->count;
	res->left = tree0;
	res->right = tree1;
	res->sym = 0;
	return res;
}

TreeNode* BuildHuffmanTree(int n, long long* symbols)//Build a Huffman Tree
{
	Heap heap = Create_heap(256);
	for (int i = 0; i < n; i++)
	{
		if(symbols[i] != 0)
			Insert_heap(&heap, NewTreeFromSym(i, symbols[i]));
	}
	for (int i = 1; i < n; i++)
	{
		// for (int j = 0; j < heap.size; j++)
		// {
		// 	TreeNode* a = heap.arr[j];
		// }
		if (heap.size > 1)
		{
			TreeNode* tree0 = Extract_min_heap(&heap);
			TreeNode* tree1 = Extract_min_heap(&heap);
			Insert_heap(&heap, NewTreeFromTwoTrees(tree0, tree1));
		}
	}
	return Extract_min_heap(&heap);
}


typedef struct Code {
	uint32_t bin_code;
	uint32_t len;
}Code;

void DFS_tree(TreeNode* tree, Code* codes, uint32_t bin_code, int len) //Give binary code for each symbol using Huffman Tree
{
	if (tree == NULL)
		return;

	if (tree->left == NULL && tree->right == NULL)
	{
		codes[tree->sym].bin_code = bin_code;
		codes[tree->sym].len = len;
		// printf("%d: ", tree->sym);
		// for (int i = len - 1; i >= 0; i--)
		// 	printf("%d", (bin_code >> i) & 1);
		// printf("\n");
	}
	else
	{
		DFS_tree(tree->left, codes, (bin_code << 1) | 0, len + 1);
		DFS_tree(tree->right, codes, (bin_code << 1) | 1, len + 1);
	}
}
