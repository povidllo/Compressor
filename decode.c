#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

TreeNode* Decode_tree(FILE* file)
{
    int marker = fgetc(file);
    if (marker == 0) return NULL; // ������ ����
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL)
    {
        printf("Something went wrong\n");
        exit(0);
    }
    if (marker == 1) {
        node->sym = fgetc(file); // ��������� ������ ��� ��������� ����
        node->left = node->right = NULL;
    }
    else {
        node->left = Decode_tree(file); // ���������� ������������� ����� ���������
        node->right = Decode_tree(file); // ���������� ������������� ������ ���������
    }
    return node;
}


void Decode(FILE* input, FILE* output, TreeNode* tree, long long size)
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
    //сделать елси size != 0

}