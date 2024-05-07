#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "tree.h"
#include <libgen.h> //для имени файла

//gcc main.c encode.c decode.c tree.c -o compressor

int check_file(char* file_name) //проверяет есть ли такой файл уже или нет
{
    if (access(file_name, F_OK) == 0)
    {
        printf("Are you sure you want to overwrite this file? \"%s\"  [y/n] : ", file_name);
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
    return 0;
}


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
            printf("    Something went wrong\n");
            return 1;
        }
        long long symbols[257] = { 0 }; // количество символов

        strcat(argv[argc - 1], ".biba");
        
        if(check_file(argv[argc - 1])){return 1;} //проверяем есть ли такой файл уже
        FILE* out = fopen(argv[argc - 1], "wb"); // открываем архивируемый файл
        if (out == NULL){
            printf("    Something went wrong\n");
            return 1;
        }
        
        fwrite(&files_count, sizeof(int), 1, out); // записать количество файлов

        /*Считаем все байты и размеры*/
        for(int f = 0; f < files_count; f++) {
            files_names[f] = (char*)malloc(sizeof(char) * 400);
            if (files_names[f] == NULL) { 
                printf("    Something went wrong\n");
                return 1;
            }
            size[f] = 0; 

            strcpy(files_names[f], basename(argv[f + 1]));
            // printf("%s ", files_names[f]);

            FILE* file = fopen(argv[f + 1], "rb");
            if (file == NULL) {
                printf("    Can't open file %s\n", files_names[f]);
                return 1;
            }

            uint8_t sym;
            while (fread(&sym, sizeof(uint8_t), 1, file) == 1) {
                symbols[sym]++;
                size[f]++; 
            }

            // printf("%s %lld\n", files_names[f], size[f]);
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
            printf("    Something went wrong\n");
            return 1;;
        }
        DFS_tree(tree, codes, 0, 0);
        Encode_tree(tree, out);

        /*Кодируем файлы*/
        for(int f = 0; f < files_count; f++)
        {
            FILE* input = fopen(argv[f + 1], "rb");
            if(input == NULL){
                printf("    Can't open file %s", argv[f + 1]);
                return 1;
            }
            // printf("%s %lld", argv[f + 1],size[f]);
            Encode(input, out, codes, tree, size[f]);
            // printf("    f\n");
            fclose(input);
        }
        
        /*free and close files*/
        free(size);
        for(int f = 0; f < files_count; f++)
        {
            free(files_names[f]);
        }
        free(files_names);
        fclose(out);
        printf("    Your compression was successful\n");
    }
    else if (argc == 3 && strcmp(argv[argc - 1], "-unzip") == 0)
    {
        if(strstr(argv[1], ".biba") == NULL)
        {
            printf("    Your file must have [.biba] extention:)\n");
            return 1;
        }
        FILE* input = fopen(argv[1], "rb");//декодируемый файл
        int files_count = 0;
        fread(&files_count, sizeof(int), 1, input);

        char** files_names = (char**)malloc(sizeof(char*) * files_count); // имена файлов
        long long *size = (long long*)malloc(sizeof(long long) * files_count); // размеры файлов
        if (files_names == NULL || size == NULL) {
            printf("    Something went wrong\n");
            return 1;
        }
        for(int f = 0; f < files_count; f++) // инициализация размеров и названий файлов
        {
            files_names[f] = (char*)malloc(sizeof(char) * 400);
            if (files_names[f] == NULL) { 
                printf("    Something went wrong\n");
                return 1;
            }
            int filename_size;
            size[f] = 0; 
            fread(&filename_size, sizeof(int), 1, input);
            fread(files_names[f], sizeof(char), filename_size, input);
            fread(&size[f], sizeof(long long), 1, input);
            // printf("%s %lld %lld\n", files_names[f], filename_size, size[f]);
            
        }
        // printf("-----------\n");
        TreeNode* new_tree = Decode_tree(input);
        for(int f = 0; f < files_count; f++) // декодирование
        {
            if(check_file(files_names[f])){continue;;} //проверяем есть ли такой файл уже
            FILE* output = fopen(files_names[f], "wb");
            Decode(input, output, new_tree, size[f]);
            // printf("5\n");
            fclose(output);
        }

        /*free and close files*/
        free(size);
        for(int f = 0; f < files_count; f++)
        {
            free(files_names[f]); 
        }
        free(files_names);
        fclose(input);
        printf("    Your decompression was successful\n");
    }
    else
    {
        printf("    Invalid input\n    You can execute \"./app --help\" in Linux system or \"app --help\" in Windows\n");
        // return 1;
    }
    return 0;
}
