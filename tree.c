#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//parent = (id - 1)/1
//left = id * 2 + 1
//right = id * 2 + 2

typedef struct TreeNode {
	int count;
	int sym;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;

typedef struct Heap {
	TreeNode** arr;
	int size;
	int cap;
}Heap;

Heap create_heap(int cap)//creating a heap
{
	Heap heap;
	heap.cap = cap;
	heap.arr = (TreeNode**)malloc((heap.cap + 1) * sizeof(TreeNode*));
	heap.size = 0;
	return heap;
}

void heapify(Heap* heap, int id)//from up to down
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
		heapify(heap, smallest);
	}
}

void insert_heap(Heap* heap, TreeNode* val)//insert into heap
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

TreeNode* extract_min_heap(Heap* heap)
{
	TreeNode* min = heap->arr[0];
	heap->arr[0] = heap->arr[heap->size - 1];
	heap->size--;

	heapify(heap, 0);
	return min;
}


TreeNode* NewTreeFromSym(int sym, int f)
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	res->count = f;
	res->sym = sym;
	res->left = res->right = NULL;
	return res;
}

TreeNode* NewTreeFromTwoTrees(TreeNode* tree0, TreeNode* tree1)
{
	TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));
	res->count = tree0->count + tree1->count;
	res->left = tree0;
	res->right = tree1;
	return res;
}

TreeNode* BuildHuffmanTree(int n, int* f)
{
	Heap heap = create_heap(256);
	for (int i = 0; i < n; i++)
	{
		if(f[i] != 0)
			insert_heap(&heap, NewTreeFromSym(i, f[i]));
	}
	for (int i = 1; i < n; i++)
	{
		for (int j = 0; j < heap.size; j++)
		{
			TreeNode* a = heap.arr[j];
		}
		if (heap.size > 1)
		{
			TreeNode* tree0 = extract_min_heap(&heap);
			TreeNode* tree1 = extract_min_heap(&heap);
			insert_heap(&heap, NewTreeFromTwoTrees(tree0, tree1));
		}
	}
	return extract_min_heap(&heap);
}
