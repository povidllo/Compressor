#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "tree.h"
#include "encode_decode.h"

int main(int argc, char* argv[])
{
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "--h") == 0))
    {
        printf("Enter into terminal: main [file_path] [zip/unzip]\n");
        printf("zip - zip file\n");
        printf("unzip - unzip file (only if file has .biba extention\n");
        return 0;
    }
    if(argc != 3)
    {
        printf("Invalid input\n");
        return 1;
    }

    if (strcmp(argv[2], "zip") == 0)
    {
        char* file_name = (char*)malloc(sizeof(char) * 300);
        if (file_name == NULL)
        {
            printf("Something went wrong\n");
            exit(0);
        }
        strcpy(file_name, argv[1]);
        long long size = 0;
        long long symbols[257] = { 0 };
        FILE* file = fopen(file_name , "rb");
        //printf("%s", file_name);
        if (file == NULL)
        {
            printf("Can't open file %s\n", file_name);
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
        if (codes == NULL)
        {
            printf("Something went wrong\n");
            exit(0);
        }
        DFS_tree(tree, codes, 0, 0);

        file = fopen(file_name, "rb"); //открываем файл, который архивируем
        if (file == NULL)
        {
            printf("Can't open file %s\n", file_name);
            return 1;
        }

        char* file_ext = strtok(argv[1], ".");
        char* new_file = (char*)malloc(sizeof(char) * 300);
        strcpy(new_file, file_ext);
        char new_ext[6] = ".biba";
        strcat(new_file, new_ext);
        if (access(new_file, F_OK) == 0)
        {
            printf("Are you sure you want to overwrite this file? \"%s\"  [y/n] : ", new_file);
            char ans[2];
            while (1)
            {
                scanf("%s", ans);
                if (strcmp(ans, "n") == 0)
                {
                    printf("The procces was itterupted :(\n");
                    return 1;
                }
                if (strcmp(ans, "y") == 0)
                {
                    break;
                }
            }
        }
        FILE* out = fopen(new_file, "wb"); // создаю файл для записи
        if (out == NULL)
        {
            printf("Can't create file\n");
            return 1;
        }
        file_ext = strtok(NULL, ".");
        int len = strlen(file_ext);
        fwrite(&len, sizeof(int), 1, out);
        fwrite(file_ext, sizeof(char), len, out);

        Encode(file, out, codes, tree, size);
        fclose(file);
        fclose(out);
        free(codes);
    }
    else if (strcmp(argv[2], "unzip") == 0)
    {
        //printf("1\n");
        char* file_name = (char*)malloc(sizeof(char) * 300);
        strcpy(file_name, argv[1]);

        FILE* input = fopen(argv[1], "rb"); //открываем файл, который дешифруем
        if (input == NULL)
        {
            printf("Error opening encoded file\n");
            return 1;
        }

        int len;
        char* file_ext = (char*)malloc(sizeof(char) * 10);
        if (file_ext == NULL)
        {
            printf("Something went wrong\n");
            exit(0);
        }
        fread(&len, sizeof(int), 1, input);
        fread(file_ext, sizeof(char), len, input);
        char* new_file = strtok(file_name, ".");
        char add[5] = "_new";
        strcat(strcat(new_file, "_new."), file_ext);

        if (access(new_file, F_OK) == 0) //проверка на перезапись
        {
            printf("Are you sure you want to overwrite this file? \"%s\"  [y/n] : ", new_file);
            char ans[2];
            while (1)
            {
                scanf("%s", ans);
                if (strcmp(ans, "n") == 0)
                {
                    printf("The procces was itterupted :(\n");
                    return 1;
                }
                if (strcmp(ans, "y") == 0)
                {
                    break;
                }
            }
        }

        FILE* output = fopen(new_file, "wb"); //открываем файл для шифрования
        if (output == NULL)
        {
            printf("Can't create file\n");
            return 1;
        }
        //printf("2\n");
        //printf("3\n");
        long long new_size;
        fread(&new_size, sizeof(long long), 1, input);
        TreeNode* new_tree = Decode_tree(input);
        Decode(input, output, new_tree, new_size);
        //printf("4\n");
    }
    return 0;
}
