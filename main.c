#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tree.h"

#define file_name "input.txt"


void encode(FILE* input, FILE* output, Code* codes, TreeNode* tree, long long size)
{
	//fwrite(&size, sizeof(long long), 1, output);
	//fwrite(tree, sizeof(TreeNode), 256, output);
	uint8_t sym;
	uint8_t buffer = 0;
	int count = 0;
	while (fread(&sym, sizeof(uint8_t), 1, input) == 1) 
	{
		Code code = codes[sym];
		for (int i = 0; i < code.len; i++)
		{
			//buffer |= (code.bin_code <<= count;)
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
    long long size = 0;
    long long symbols[257] = { 0 };
    FILE* file = fopen(file_name, "rb");
    if (file == NULL)
    {
        printf("Invalid file data input\n");
        return 1;
    }


    uint8_t sym;
    while (fread(&sym, sizeof(uint8_t), 1, file) == 1)
    {
        symbols[sym]++;
        size++;
    }

    TreeNode* tree = BuildHuffmanTree(257, symbols);
    Code* codes = (Code*)calloc(257, sizeof(Code));
    dfs_tree(tree, codes, 0, 0);

    file = fopen(file_name, "rb");
    FILE* out = fopen("output.txt", "wb");
    if (file == NULL)
    {
        printf("Invalid file data input\n");
        return 1;
    }

    encode(file, out, codes, tree, size);
    fclose(out);

    // Decode encoded data and write to output_decode.txt
    FILE* encoded_file = fopen("output.txt", "rb");
    FILE* decoded_out = fopen("output_decode.txt", "wb");
    if (encoded_file == NULL)
    {
        printf("Error opening encoded file\n");
        return 1;
    }

    // Read size and tree from encoded file
    //long long encoded_size;
    //TreeNode* encoded_tree = (TreeNode*)malloc(sizeof(TreeNode) * 256);
    //fread(&encoded_size, sizeof(long long), 1, encoded_file);
    //fread(encoded_tree, sizeof(TreeNode), 256, encoded_file);

    // Decode and write to output_decode.txt
    decode(encoded_file, decoded_out, tree, size);

    // Cleanup
    fclose(encoded_file);
    fclose(decoded_out);
    //free(encoded_tree);
    free(codes);

    return 0;
}
