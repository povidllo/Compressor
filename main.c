#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tree.h"



void serialize_tree(TreeNode* node, FILE* file) {
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
        serialize_tree(node->left, file); 
        serialize_tree(node->right, file); 
    }
}

TreeNode* deserialize_tree(FILE* file) {
    int marker = fgetc(file);
    if (marker == 0) return NULL; // пустой узел
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (marker == 1) {
        node->sym = fgetc(file); // считываем символ для листового узла
        node->left = node->right = NULL;
    }
    else {
        node->left = deserialize_tree(file); // рекурсивно десериализуем левое поддерево
        node->right = deserialize_tree(file); // рекурсивно десериализуем правое поддерево
    }
    return node;
}


void encode(FILE* input, FILE* output, Code* codes, TreeNode* tree, long long size)
{
    fwrite(&size, sizeof(long long), 1, output);
    serialize_tree(tree, output);
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

void decode(FILE* input, FILE* output, TreeNode* tree, long long size)
{

	TreeNode* cur = tree;
	uint8_t bite;
	int bit = 0;
    while (size > 0 && fread(&bite, sizeof(uint8_t), 1, input) == 1)
	{
        bit = 0;
        while (size > 0 && bit != 8)
        {
            uint8_t temp_bite = (bite >> (7 - bit)) & 1;
            if (temp_bite == 0 && cur->left != NULL)
            {
                cur = cur->left;
            }
            else if (temp_bite == 1 && cur->right != NULL)
            {
                cur = cur->right;
            }

            if (cur->left == NULL && cur->right == NULL)
            {
                fwrite(&(cur->sym), sizeof(uint8_t), 1, output);
                cur = tree;
                size--;
            }
            bit++;
        }
	}

}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Invalid input\n");
        return 1;
    }

    if (strcmp(argv[2], "zip") == 0)
    {
        char* file_name = (char*)malloc(sizeof(char) * 300);
        strcpy(file_name, argv[1]);
        long long size = 0;
        long long symbols[257] = { 0 };
        FILE* file = fopen(file_name , "rb");
        printf("%s", file_name);
        if (file == NULL)
        {
            printf("Invalid file data input1\n");
            return 1;
        }

        uint8_t sym;
        while (fread(&sym, sizeof(uint8_t), 1, file) == 1)
        {
            symbols[sym]++;
            size++;
        }


        fclose(file);
        TreeNode* tree = BuildHuffmanTree(257, symbols);
        Code* codes = (Code*)calloc(257, sizeof(Code));
        dfs_tree(tree, codes, 0, 0);

        file = fopen(file_name, "rb");

        char* file_ext = strtok(argv[1], ".");
        char* new_file = (char*)malloc(sizeof(char) * 300);
        strcpy(new_file, file_ext);
        char new_ext[6] = ".biba";
        //FILE* out = fopen("output.txt", "wb");
        FILE* out = fopen(strcat(new_file, new_ext), "wb");
        if (file == NULL)
        {
            printf("Invalid file data input2\n");
            return 1;
        }
        file_ext = strtok(NULL, ".");
        int len = strlen(file_ext);
        fwrite(&len, sizeof(int), 1, out);
        fwrite(file_ext, sizeof(char), len, out);

        encode(file, out, codes, tree, size);
        fclose(file);
        fclose(out);
        free(codes);
    }
    else if (strcmp(argv[2], "unzip") == 0)
    {
        printf("1\n");
        char* file_name = (char*)malloc(sizeof(char) * 300);
        strcpy(file_name, argv[1]);

        FILE* input = fopen(argv[1], "rb");


        int len;
        char* file_ext = (char*)malloc(sizeof(char) * 10);
        fread(&len, sizeof(int), 1, input);
        fread(file_ext, sizeof(char), len, input);
        char* new_file = strtok(file_name, ".");
        char add[5] = "_new";
        strcat(strcat(new_file, "_new."), file_ext);
        printf("%s, %s", new_file, file_ext);
        FILE* output = fopen(new_file, "wb");
        if (input == NULL)
        {
            printf("Error opening encoded file\n");
            return 1;
        }





        printf("2\n");
        printf("3\n");
        long long new_size;
        fread(&new_size, sizeof(long long), 1, input);
        //fread(&new_size, sizeof(long long), 1, input);
        TreeNode* new_tree = deserialize_tree(input);
        decode(input, output, new_tree, new_size);
        printf("4\n");
    }
    return 0;
}
