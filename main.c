#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tree.h"

int main(int argc, char* argv[])
{
	// if (argc != 3)
	// {
	// 	printf("Invalid data input format\n");
	// 	exit(0);
	// }

	int symbols[256] = { 0 };
	int codes[256] = { 0 };
	// FILE* file = fopen(argv[1], "r");	
	FILE* file = fopen("input.txt", "rb");	
	if (file == NULL)
	{
		printf("Invalid file data input\n");
	}

	uint8_t sym;
	while (fread(&sym, sizeof(uint8_t), 1, file) == 1) {
		symbols[sym]++;
	}

	TreeNode* tree = BuildHuffmanTree(256, symbols);
	return 0;
}