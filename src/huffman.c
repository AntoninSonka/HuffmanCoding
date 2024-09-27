#define MEMEWATCH
//#define MW_STDIO
#include "../memwatch/memwatch.h"

#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>

struct Node* createNode(char ch){
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->ch = ch;
    node->frequency = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct Node** createNodeArr(char* text, int textSize, int* arrSize){
    struct Node** arr = (struct Node**) malloc(sizeof(struct Node*) * 256);
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

    arr = (struct Node**) realloc(arr, sizeof(struct Node*) * *arrSize);
    if(arr == NULL){
        printf("Reallocation error\n");
        return NULL;
    }
    return arr;
}

//-------------

void swap(struct Node** x, struct Node** y){
    struct Node* z = *x;
    *x = *y;
    *y = z;
}

void traverseDown(struct Node** arr, int index, int arrSize){
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

void heapify(struct Node** arr, int arrSize){
    for(int i = (arrSize / 2 - 1); i >= 0; --i){
        traverseDown(arr, i, arrSize);
    }
}

//-------------

void traverseUp(struct Node** arr, int index){
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

struct Node* buildHuffmanTree(struct Node** arr, int arrSize){

    while(arrSize != 1){
        struct Node* root = createNode('\0');
        
        
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
    }
    struct Node* root = arr[0];
    arr[0] = NULL;
    free(arr);
    return root;
}

void freeTree(struct Node* root){
    if(root->left == NULL && root->right == NULL){
        free(root);
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void printTree(struct Node* root, bool isLeft){
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
