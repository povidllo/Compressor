#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <stdint.h>

/*
./compressor generated_tests/file_0.test generated_tests/file_1.test generated_tests/file_2.test generated_tests/file_3.test generated_tests/file_4.test generated_tests/file_5.test generated_tests/file_6.test generated_tests/file_7.test generated_tests/file_8.test generated_tests/file_9.test -zip zippa
./compressor zippa.biba -unzip

cmp file_1.test generated_tests/file_1.test 
cmp file_2.test generated_tests/file_2.test 
cmp file_3.test generated_tests/file_3.test 
cmp file_4.test generated_tests/file_4.test 
cmp file_5.test generated_tests/file_5.test 
cmp file_6.test generated_tests/file_6.test 
cmp file_7.test generated_tests/file_7.test 
cmp file_8.test generated_tests/file_8.test 
cmp file_9.test generated_tests/file_9.test 

*/
int main(void) {
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        int file_size = rand() % 100000000; //~500 mb
        char name[300] = "generated_tests/file_";
        char char_size[20];
        sprintf(char_size, "%d", i);
        strcat(name, char_size);
        strcat(name, ".test");
        printf("%s %d\n", name, file_size);

        FILE* input = fopen(name, "wb");
        for(int j = 0; j < file_size; j++)
        {
            uint8_t sym = rand() % 256;
            fwrite(&sym, sizeof(uint8_t), 1, input);
        }
        fclose(input);
    }
}