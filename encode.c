#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void Encode_tree(TreeNode* node, FILE* file) {
	if (node == NULL) //if node is empty
	{
		fputc(0, file);
		return;
	}
	if (node->left == NULL && node->right == NULL) //if a leaf
	{
		fputc(1, file);
		fputc(node->sym, file);
	}
	else
	{
		fputc(2, file);
		Encode_tree(node->left, file);
		Encode_tree(node->right, file);
	}
}


void Encode(FILE* input, FILE* output, Code* codes, TreeNode* tree, long long size)
{
	fwrite(&size, sizeof(long long), 1, output);
	Encode_tree(tree, output);
	uint8_t sym;
	uint8_t buffer = 0;
	int count = 0;
	while (fread(&sym, sizeof(uint8_t), 1, input) == 1)
	{
		Code code = codes[sym];
		for (int i = 0; i < code.len; i++)
		{
			buffer = (buffer << 1) | ((code.bin_code >> (code.len - i - 1)) & 1);
			count++;
			if (count == 8)
			{
				fwrite(&buffer, sizeof(uint8_t), 1, output);
				
				buffer = 0;
				count = 0;
			}
		}
	}
	if (count > 0)
	{
		buffer = buffer << (8 - count);
		fwrite(&buffer, sizeof(uint8_t), 1, output);
	}
}