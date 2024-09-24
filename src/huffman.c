#include "huffman.h"
#include <stdlib.h>
#include <stdbool.h>
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


/*
 *
 *
 *         [*1, *5, *9, *10]
 *
 *          || pull first out
 *          \/
 *
 *        [*5, *9, *10, NULL]
 *
 *         || pull second out
 *         \/
 *
 *        [*9, *10, NULL, NULL]
 *
 *         make new node with sum of those 2 elements and assign the 2 nodes as left and right child
 *
 *         6 = 1 + 5
 *
 *         6->left = 1
 *         6->right = 5
 *
 *         || push *6 into heap
 *         \/
 *
 *        [*6, *9, *10, NULL]
 *
 *        repeat untill one element lefts
 *
 *        [*25, NULL, NULL, NULL]
 *
 *         || pull last out
 *         \/
 *
 *        free [NULL, NULL, NULL, NULL]
 *
 *        return saved element
 *
 *
 */
