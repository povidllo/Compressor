#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "tree.h"
#include <libgen.h> //для имени файла
// #include "encode_decode.h"

//gcc main.c encode.c decode.c tree.c -o compressor

int main(int argc, char* argv[])
{
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "--h") == 0))
    {
        printf("    Enter into terminal:\n");
        printf("    ./app [file_path] -zip [archive_name] #if you want zip files (You can choose some files)\n");
        printf("    ./app [file_path] -unzip              #unzip file (only if file has .biba extention)\n");
        printf("    notise: you can't zip file with space in name, for example \"in pu.txt\"\n");
        return 0;
    }

    if (argc >= 3 && strcmp(argv[argc - 2], "-zip") == 0) {
        /*Инициализация*/
        int files_count = argc - 3; // количество файлов
        char** files_names = (char**)malloc(sizeof(char*) * files_count); // имена файлов
        long long *size = (long long*)malloc(sizeof(long long) * files_count); // размеры файлов
        if (files_names == NULL || size == NULL) {
            printf("Something went wrong\n");
            return 1;
        }
        long long symbols[257] = { 0 }; // количество символов

        strcat(argv[argc - 1], ".biba");
        FILE* out = fopen(argv[argc - 1], "wb"); // архивируемый файл
        if (out == NULL){
            printf("Something went wrong\n");
            return 1;
        }
        
        fwrite(&files_count, sizeof(int), 1, out); // записать количество файлов
        /*Считаем все байты и размеры*/
        for(int f = 0; f < files_count; f++) {
            files_names[f] = (char*)malloc(sizeof(char) * 400);
            if (files_names[f] == NULL) { 
                printf("Something went wrong\n");
                return 1;
            }
            size[f] = 0; 

            strcpy(files_names[f], basename(argv[f + 1]));
            printf("%s ", files_names[f]);

            FILE* file = fopen(argv[f + 1], "rb");
            if (file == NULL) {
                printf("Can't open file %s\n", files_names[f]);
                return 1;
            }

            uint8_t sym;
            while (fread(&sym, sizeof(uint8_t), 1, file) == 1) {
                symbols[sym]++;
                size[f]++; 
            }

            printf("%s %lld\n", files_names[f], size[f]);
            int len = strlen(files_names[f]);
            fwrite(&len, sizeof(int), 1, out); // записать длину названия файла
            fwrite(files_names[f], sizeof(char), len, out); // записать название файла
            fwrite(&size[f], sizeof(long long), 1, out); // записать количество байт в файле

            fclose(file);
        }

        /*Создаем дерево и коды*/
        TreeNode* tree = BuildHuffmanTree(257, symbols);
        Code* codes = (Code*)calloc(257, sizeof(Code));
        if (codes == NULL){
            printf("Something went wrong\n");
            return 1;;
        }
        DFS_tree(tree, codes, 0, 0);
        Encode_tree(tree, out);

        /*Кодируем файлы*/
        for(int f = 0; f < files_count; f++)
        {
            FILE* input = fopen(argv[f + 1], "rb");
            if(input == NULL){
                printf("Can't open file %s", argv[f + 1]);
                return 1;
            }
            printf("%s %lld", argv[f + 1],size[f]);
            Encode(input, out, codes, tree, size[f]);
            printf("    f\n");
        }
        // file = fopen(file_name, "rb"); //открываем файл, который архивируем
        // if (file == NULL)
        // {
        //     printf("Can't open file %s\n", file_name);
        //     return 1;
        // }

        // char* file_ext = strtok(argv[1], ".");
        // char* new_file = (char*)malloc(sizeof(char) * 300);
        // strcpy(new_file, file_ext);
        // char new_ext[6] = ".biba";
        // strcat(new_file, new_ext);
        // if (access(new_file, F_OK) == 0)
        // {
        //     printf("Are you sure you want to overwrite this file? \"%s\"  [y/n] : ", new_file);
        //     char ans[2];
        //     while (1)
        //     {
        //         scanf("%s", ans);
        //         if (strcmp(ans, "n") == 0)
        //         {
        //             printf("The procces was itterupted :(\n");
        //             return 1;
        //         }
        //         if (strcmp(ans, "y") == 0)
        //         {
        //             break;
        //         }
        //     }
        // }
        // FILE* out = fopen(new_file, "wb"); // создаю файл для записи
        // if (out == NULL)
        // {
        //     printf("Can't create file\n");
        //     return 1;
        // }
        // file_ext = strtok(NULL, ".");
        // int len = strlen(file_ext);
        // fwrite(&len, sizeof(int), 1, out);
        // fwrite(file_ext, sizeof(char), len, out);

        // Encode(file, out, codes, tree, size);
        // fclose(file);
        // fclose(out);
        // free(codes);
    }
    else if (argc == 3 && strcmp(argv[argc - 1], "-unzip") == 0)
    {
        FILE* input = fopen(argv[1], "rb");//декодируемый файл
        int files_count = 0;
        fread(&files_count, sizeof(int), 1, input);

        char** files_names = (char**)malloc(sizeof(char*) * files_count); // имена файлов
        long long *size = (long long*)malloc(sizeof(long long) * files_count); // размеры файлов
        if (files_names == NULL || size == NULL) {
            printf("Something went wrong\n");
            return 1;
        }
        for(int f = 0; f < files_count; f++)
        {
            files_names[f] = (char*)malloc(sizeof(char) * 400);
            if (files_names[f] == NULL) { 
                printf("Something went wrong\n");
                return 1;
            }
            int filename_size;
            size[f] = 0; 
            fread(&filename_size, sizeof(int), 1, input);
            fread(files_names[f], sizeof(char), filename_size, input);
            fread(&size[f], sizeof(long long), 1, input);
            printf("%s %lld %lld\n", files_names[f], filename_size, size[f]);
            
        }
        printf("-----------\n");
        TreeNode* new_tree = Decode_tree(input);
        for(int f = 0; f < files_count; f++)
        {
            FILE* output = fopen(files_names[f], "wb");
            Decode(input, output, new_tree, size[f]);
            printf("5\n");
        }

        // //printf("1\n");
        // char* file_name = (char*)malloc(sizeof(char) * 300);
        // strcpy(file_name, argv[1]);

        // FILE* input = fopen(argv[1], "rb"); //открываем файл, который дешифруем
        // if (input == NULL)
        // {
        //     printf("Error opening encoded file\n");
        //     return 1;
        // }

        // int len;
        // char* file_ext = (char*)malloc(sizeof(char) * 10);
        // if (file_ext == NULL)
        // {
        //     printf("Something went wrong\n");
        //     return 1;;
        // }
        // fread(&len, sizeof(int), 1, input);
        // fread(file_ext, sizeof(char), len, input);
        // char* new_file = strtok(file_name, ".");
        // char add[5] = "_new";
        // strcat(strcat(new_file, "_new."), file_ext);

        // if (access(new_file, F_OK) == 0) //проверка на перезапись
        // {
        //     printf("Are you sure you want to overwrite this file? \"%s\"  [y/n] : ", new_file);
        //     char ans[2];
        //     while (1)
        //     {
        //         scanf("%s", ans);
        //         if (strcmp(ans, "n") == 0)
        //         {
        //             printf("The procces was itterupted :(\n");
        //             return 1;
        //         }
        //         if (strcmp(ans, "y") == 0)
        //         {
        //             break;
        //         }
        //     }
        // }

        // FILE* output = fopen(new_file, "wb"); //открываем файл для шифрования
        // if (output == NULL)
        // {
        //     printf("Can't create file\n");
        //     return 1;
        // }
        // //printf("2\n");
        // //printf("3\n");
        // long long new_size;
        // fread(&new_size, sizeof(long long), 1, input);
        // TreeNode* new_tree = Decode_tree(input);
        // Decode(input, output, new_tree, new_size);
        // //printf("4\n");
    }
    else
    {
        printf("    Invalid input\n    You can execute \"./app --help\" in Linux system or \"app --help\" in Windows\n");
        // return 1;
    }
    return 0;
}
