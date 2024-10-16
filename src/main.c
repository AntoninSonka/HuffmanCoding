#define MEMEWATCH
//#define MW_STDIO
#include "../memwatch/memwatch.h"

#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>

void printBinary(int depth, uint16_t num){
    for(int i = depth - 1; i >= 0; --i){
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
}

int main(int argc, char* argv[]){
    
    if(argc != 2){
        printf("Bad number of arguments\n");
        printf("Exiting with code: %d\n", 1);
        return 1;
    }
    
    int textSize;

    char* text = readFile(argv[1], &textSize);

    if(text == NULL){
        printf("Exiting with code %d\n", 2);
        return 2;
    }
    
    printf("--------------TEXT-FROM-FILE--------------\n");
    for(int i = 0; i < textSize; ++i){
        printf("%c", text[i]);
    }
    printf("------------------------------------------\n");
    
    int arrSize;

    Node** arr = createNodeArr(text, textSize, &arrSize);

    if(arr == NULL){
        free(text);
        printf("Exiting with code: %d\n", 3);
        return 3;
    }

    printf("----------------NODE-ARRAY----------------\n");

    for(int i = 0; i < arrSize; ++i){
        printf("ch: %c, f: %d\n", arr[i]->ch, arr[i]->frequency);
    }
    
    printf("------------------------------------------\n");
    
    heapify(arr, arrSize);

    printf("-------------------HEAP-------------------\n");

    for(int i = 0; i < arrSize; ++i){
        printf("ch: %c, f: %d\n", arr[i]->ch, arr[i]->frequency);
    }
    printf("\n");
    int multiplicator = 1;
    for(int i = 0; i < arrSize; ++i){
        for(int j = 0; j < multiplicator; ++j){
            if(arr[i]->ch == '\n'){
                printf("ch: \\n, f: %d || ", arr[i]->frequency);
            }
            else{
                printf("ch: %c, f: %d || ", arr[i]->ch, arr[i]->frequency);
            }
            ++i;
            if(i >= arrSize)
                break;
        }
        --i;
        multiplicator *= 2;
        printf("\n");
    }
    
    printf("------------------------------------------\n");
    Node* root = buildHuffmanTree(arr, arrSize);

    printf("---------------HUFFMAN-TREE---------------\n");
    printf("------------------------------------------\n");
    printTree(root, true);
    printf("\n------------------------------------------\n");

    int treeSize = 0;
    Code** codeArr = assignCodes(root, arrSize, &treeSize);
    printf("------------------CODES-------------------\n");
    printf("------------------------------------------\n");
    for(int i = 0; i < arrSize; ++i){
        printf("ch: %c, f: %d, depth: %d, binaryCode: ", codeArr[i]->ch, codeArr[i]->frequency, codeArr[i]->depth);
        printBinary(codeArr[i]->depth, codeArr[i]->code);
        printf("\n");
    }
    printf("------------------------------------------\n");
   
    Tree** treeArr = assignTree(root, treeSize);
    printf("------------------CODES-------------------\n");
    printf("------------------------------------------\n");
    for(int i = 0; i < treeSize; ++i){
        printBinary(2, treeArr[i]->identefire);
        if(treeArr[i]->identefire == 0b00 || treeArr[i]->identefire == 0b11){
            printBinary(8, treeArr[i]->ch);
            /*if(treeArr[i]->ch == '\n'){
                printf("\\n");
            }
            else{
                printf("%c", treeArr[i]->ch);
            }*/
        }
    }
    printf("\n");
    printf("------------------------------------------\n");
    int treeBitSize = 0;
    int treeBinSize = 0;
    uint8_t* treeBinArr = treeToBin(treeArr, treeSize, &treeBitSize, &treeBinSize);
    printf("\n%d\n", treeBinSize);
    for(int i = 0; i < treeBinSize; ++i){
        printBinary(8, treeBinArr[i]);
    }
    free(treeBinArr);
    printf("\n");
    for(int i = 0; i < treeSize; ++i){
        free(treeArr[i]);
    }
    free(treeArr);
    for(int i = 0; i < arrSize; ++i){
        free(codeArr[i]);
    }
    
    
    free(codeArr);

    freeTree(root);
    free(text);

    /*printf("\ntest head\n");
    int treeBitSize = 127;
    int textBitSize = 127;
    uint16_t head = createFileHead(textBitSize, treeBitSize);
    printf("%b", head);*/
    printf("\n");

    return 0;
}
