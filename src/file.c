#define MEMEWATCH
//#define MW_STDIO
#include "../memwatch/memwatch.h"

#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* readFile(const char* path, int* fileSize){
    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("No such a file or directory\n");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    *fileSize = ftell(file);

    
    if(fileSize == 0){
        fclose(file);
        printf("There is nothing to do\n");
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    
    char* text = (char*) malloc(*fileSize);

    for(int i = 0; i < *fileSize; ++i){
        text[i] = fgetc(file);
    }

    fclose(file);

    return text;
}

//void writeCodeToFile(uint16_t head, ){
//}
