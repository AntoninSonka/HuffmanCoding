#include <stdint.h>
#define MEMEWATCH
//#define MW_STDIO
#include "../memwatch/memwatch.h"

#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Node* createNode(char ch){
    Node* node = (Node*) malloc(sizeof(Node));
    node->ch = ch;
    node->frequency = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node** createNodeArr(char* text, int textSize, int* arrSize){
    Node** arr = (Node**) malloc(sizeof(Node*) * 256);
    if(arr == NULL){
        printf("Allocation error\n");
        return NULL;
    }
    
    *arrSize = 0;
    arr[0] = createNode(text[0]);
    ++(*arrSize);

    for(int i = 1; i < textSize; ++i){
        bool hasBeenCreated = false;
        for(int j = 0; j < *arrSize; ++j){
            if(arr[j]->ch == text[i]){
                ++arr[j]->frequency;
                hasBeenCreated = true;
                break;
            }
        }
        if(hasBeenCreated)
            continue;
        arr[*arrSize] = createNode(text[i]);
        ++(*arrSize);
    }

    arr = (Node**) realloc(arr, sizeof(Node*) * *arrSize);
    if(arr == NULL){
        printf("Reallocation error\n");
        return NULL;
    }
    return arr;
}

//-------------

void swap(Node** x, Node** y){
    Node* z = *x;
    *x = *y;
    *y = z;
}

void traverseDown(Node** arr, int index, int arrSize){
    if(index > (arrSize / 2 - 1)){
        return;
    }
    
    if((index * 2 + 2) <= (arrSize - 1)){
        if(arr[index * 2 + 1]->frequency < arr[index * 2 + 2]->frequency){
            if(arr[index]->frequency >= arr[index * 2 + 1]->frequency){
                swap(&arr[index], &arr[index * 2 + 1]);
                traverseDown(arr, index * 2 + 1, arrSize);
            }
        }
        else{
            if(arr[index]->frequency >= arr[index * 2 + 2]->frequency){
                swap(&arr[index], &arr[index * 2 + 2]);
                traverseDown(arr, index * 2 + 2, arrSize);
            }
        }
    }
    else if((index * 2 + 1) <= (arrSize - 1)){
        if(arr[index]->frequency >= arr[index * 2 + 1]->frequency){
            swap(&arr[index], &arr[index * 2 + 1]);
            traverseDown(arr, index * 2 + 1, arrSize);
        }
    }
}

void heapify(Node** arr, int arrSize){
    for(int i = (arrSize / 2 - 1); i >= 0; --i){
        traverseDown(arr, i, arrSize);
    }
}

//-------------

void traverseUp(Node** arr, int index){
    if(index >= 0){
        return;
    }
    
    if(((index - 1) / 2) >= 0){
        if(arr[index]->frequency <= arr[(index - 1) / 2]->frequency){
            swap(&arr[index], &arr[(index - 1) / 2]);
            traverseUp(arr, (index - 1) / 2);
        }
    }
}

Node* buildHuffmanTree(Node** arr, int arrSize){

    while(arrSize != 1){
        Node* root = createNode('\0');
        
        
        traverseDown(arr, 0, arrSize);
        root->left = arr[0];
        swap(&arr[0], &arr[arrSize - 1]);
        arr[arrSize - 1] = NULL;
        --arrSize;

        traverseDown(arr, 0, arrSize);

        root->right = arr[0];
        swap(&arr[0], &arr[arrSize - 1]);
        arr[arrSize - 1] = NULL;

        root->frequency = root->left->frequency + root->right->frequency;
        arr[arrSize - 1] = root;
        traverseUp(arr, arrSize - 1);
    }
    Node* root = arr[0];
    arr[0] = NULL;
    free(arr);
    return root;
}

void freeTree(Node* root){
    if(root->left == NULL && root->right == NULL){
        free(root);
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printTree(Node* root, bool isLeft){
    if(isLeft){
        printf("\nleft: ");
    }
    else{
        printf("\nright: ");
    }
    if(root->left == NULL && root->right == NULL){
        printf("leaf: %c, frequency: %d   ", root->ch, root->frequency);
        return;
    }
    printf("brench: frequency: %d\n", root->frequency);
    printTree(root->left, true);
    printTree(root->right, false);
}

void assignNewCode(Node* root, Code** arr, int* index, uint16_t* currentCode, int* depth, int* treeSize){
    if(root->left == NULL && root->right == NULL){
        arr[*index] = (Code*) malloc(sizeof(Code));
        arr[*index]->ch = root->ch;
        arr[*index]->frequency = root->frequency;
        arr[*index]->code = *currentCode;
        arr[*index]->depth = *depth;
        (*currentCode) >>= 1;
        --(*depth);
        ++(*index);
        return;
    }
    ++(*treeSize);
    (*currentCode) <<= 1;
    ++(*depth);
    assignNewCode(root->left, arr, index, currentCode, depth, treeSize);

    ++(*treeSize);
    (*currentCode) <<= 1;
    (*currentCode) |= 0b1;
    ++(*depth);
    assignNewCode(root->right, arr, index, currentCode, depth, treeSize);
    
    --(*depth);
    (*currentCode) >>= 1;
}

int compare(const void* x, const void* y){
    Code* codeX = *((Code**) x);
    Code* codeY = *((Code**) y);
    if(codeX->frequency < codeY->frequency){
        return 1;
    }
    if(codeX->frequency > codeY->frequency){
        return -1;
    }
    return 0;
}

Code** assignCodes(Node* root, int arrSize, int* treeSize){
    Code** arr = (Code**) malloc(sizeof(Code*) * arrSize);
    int index = 0;
    uint16_t currentCode = 0;
    int depth = 0;
    assignNewCode(root, arr, &index, &currentCode, &depth, treeSize);
    qsort(arr, arrSize, sizeof(Code*), *compare);
    return arr;
}

void assignNewTree(Node* root, Tree** arr, int* index, bool isLeft){
    if(root->left == NULL && root->right == NULL){
        --(*index);
        if(isLeft){
            arr[*index]->identefire = 0b00;
        }
        else{
            arr[*index]->identefire = 0b11;
        }
        arr[*index]->ch = root->ch;
        ++(*index);
        return;
    }
    arr[*index] = (Tree*) malloc(sizeof(Tree));
    arr[*index]->identefire = 0b10;
    ++(*index);
    assignNewTree(root->left, arr, index, 1);
    arr[*index] = (Tree*) malloc(sizeof(Tree));
    arr[*index]->identefire = 0b01;
    ++(*index);
    assignNewTree(root->right, arr, index, 0);
}

Tree** assignTree(Node* root, int treeSize){
    Tree** arr = (Tree**) malloc(sizeof(Tree*) * treeSize);
    int index = 0;
    assignNewTree(root, arr, &index, 0);
    return arr;
}

uint16_t createFileHead(int textBitSize, int treeBitSize){ // I don't even know whats happening here, it just works, okay?

    // textBitSize and treeBitSize is how large are they in bits, not how many bits you need to represent them

    uint16_t head = 0b0;
    head <<= 1;
    head |= 0b1; // It is a text file, so 1 on the start
    head <<= 5;  // Just blank bits, because I needed 2 bytes. I will probably use it for flags or something

    int roundedTextBitSize = 0;
    if(textBitSize % 8 == 0) // If its not by bytes exacly, it does that thing and is is by bytes now
        roundedTextBitSize = textBitSize;
    else
        roundedTextBitSize = textBitSize / 8 * 8 + 8;


    uint8_t textPadding = roundedTextBitSize - textBitSize;
    head <<= 3; // Adds how many zeros will fill the rest of text file
    head |= textPadding;

    printf("%d x %d\n", textBitSize, roundedTextBitSize);
    //uint8_t bytesNeededText = ((((int)log2(textBitSize)) + 1) + 7) / 8; // This somehow works, I dont really know how or why, but yah
    uint8_t bytesNeededText = (((int)log2((int)(roundedTextBitSize / 8)))); // This somehow works, I dont really know how or why, but yah
    bytesNeededText /= 8;
    //--bytesNeededText;
    head <<= 2;
    head |= bytesNeededText;
    
    // The same thing as before, but for the tree

    int roundedTreeBitSize = 0;
    if(treeBitSize % 8 == 0)
        roundedTreeBitSize = treeBitSize;
    else
        roundedTreeBitSize = treeBitSize / 8 * 8 + 8;

    uint8_t treePadding = roundedTreeBitSize - treeBitSize;
    head <<= 3;
    head |= treePadding;

    //uint8_t bytesNeededTree = ((((int)log2(textBitSize)) + 1) + 7) / 8;
    //printf("%d x %d\n", treeBitSize, roundedTreeBitSize);
    uint8_t bytesNeededTree = ((int)log2((int)(roundedTreeBitSize / 8)));
    bytesNeededTree /= 8;
    //--bytesNeededTree;
    head <<= 2;
    head |= bytesNeededTree;
    return head;
} // This thing can support text up to 4 Gigs long after compression, so yeah. I think...

uint8_t* treeToBin(Tree** treeArr, int treeSize, int* treeBitSize, int* treeBinSize){
    int bitSize = 0;
    for(int i = 0; i < treeSize; ++i){
        switch (treeArr[i]->identefire) {
            case 0b00:
            case 0b11:
                bitSize += 10;
                break;
            default:
                bitSize += 2;
                break;
        }
    }
    int roundedBit = 0;
    if(bitSize % 8 == 0)
        roundedBit = bitSize;
    else
        roundedBit = bitSize / 8 * 8 + 8;

    int byteSize = roundedBit / 8;
    uint8_t* arr = (uint8_t*) calloc(byteSize, sizeof(uint8_t));
    
    int bitIndex = 0;
    int arrIndex = -1;
    for(int i = 0; i < treeSize; ++i){
        if(bitIndex == 0){
            bitIndex = 8;
            ++arrIndex;
        }
        bitIndex -= 2;
        int crumb = treeArr[i]->identefire << bitIndex;
        arr[arrIndex] |= crumb;
        if(treeArr[i]->identefire == 0b00 || treeArr[i]->identefire == 0b11){
            uint8_t ch = 0;
            switch (bitIndex) {
                case 0:
                    ++arrIndex;
                    arr[arrIndex] = treeArr[i]->ch;
                    break;
                case 2:
                    ch = treeArr[i]->ch >> 6;
                    arr[arrIndex] |= ch;
                    ch = treeArr[i]->ch << 2;
                    ++arrIndex;
                    arr[arrIndex] |= ch;
                    bitIndex = 2;
                    break;
                case 4:
                    ch = treeArr[i]->ch >> 4;
                    arr[arrIndex] |= ch;
                    ch = treeArr[i]->ch << 4;
                    ++arrIndex;
                    arr[arrIndex] |= ch;
                    bitIndex = 4;
                    break;
                case 6:
                    ch = treeArr[i]->ch >> 2;
                    arr[arrIndex] |= ch;
                    ch = treeArr[i]->ch << 6;
                    ++arrIndex;
                    arr[arrIndex] |= ch;
                    bitIndex = 6;
                    break;
            }
        }
    }
    *treeBitSize = bitSize;
    *treeBinSize = byteSize;
    return arr;
}

uint8_t* textToBin(char* text, int textSize, Code** codes, int treeSize, int* textBitSize, int* textBinSize){

    int bitSize = 0;
    for(int i = 0; i < textSize; ++i){
        for(int j = 0; j < treeSize; ++j){
            if(text[i] == codes[j]->ch){
                bitSize += codes[j]->depth;
                break;
            }
        }
    }
    *textBitSize = bitSize;


    int roundedBit = 0;
    if(bitSize % 8 == 0)
        roundedBit = bitSize;
    else
        roundedBit = bitSize / 8 * 8 + 8;

    int byteSize = roundedBit / 8;
    
    *textBinSize = byteSize;

    uint8_t* arr = (uint8_t*) calloc(byteSize, sizeof(uint8_t));
    int bitLeft = 8;
    
    int codeIndex = 0;
    for(int i = 0; i < treeSize; ++i){
        if(text[0] == codes[i]->ch){
            codeIndex = i;
            break;
        }
    }
    uint16_t currentCode = codes[codeIndex]->code;
    int currentDepth = codes[codeIndex]->depth;
    int textIndex = 0;
    for(int i = 0; i < byteSize; ++i){
        if(currentDepth < bitLeft){
            ++textIndex;
            if(textIndex >= textSize){
                --textIndex;
            }

            bitLeft -= currentDepth;
            currentCode <<= bitLeft;
            arr[i] |= currentCode;
            for(int j = 0; j < treeSize; ++j){
                if(text[textIndex] == codes[j]->ch){
                    codeIndex = j;
                    break;
                }
            }
            currentCode = codes[codeIndex]->code;
            currentDepth = codes[codeIndex]->depth;
            --i;
        }
        else if(currentDepth == bitLeft){
            arr[i] |= currentCode;
            bitLeft = 8;
            ++textIndex;
            for(int j = 0; j < treeSize; ++j){
                if(text[textIndex] == codes[j]->ch){
                    codeIndex = j;
                    break;
                }
            }
            currentCode = codes[codeIndex]->code;
            currentDepth = codes[codeIndex]->depth;
        }
        else{
            uint16_t previousCode = currentCode;
            currentCode >>= (currentDepth - bitLeft);
            arr[i] |= currentCode;
            currentDepth -= bitLeft;
            currentCode = previousCode;
            uint16_t mask = pow(2, currentDepth) - 1;
            currentCode &= mask;
            bitLeft = 8;
        }
    }

    return arr;
}
