#include <stdio.h>
#include <stdlib.h>

#ifdef LL_IMPL
typedef struct Node {
    void *value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *ptr;
    int size;
} Llist;
Llist LlInit(void){
    return (Llist){NULL, 0};
}

int LlFree(Llist *a){
    Node *node = a->ptr;
    while(node->next!=NULL){
        node = node->next;
    }
    while(node->prev!=NULL){
        free(node->value);
        node = node->prev;
        free(node);
    }
    return 0;
}

Node *NodeGetPtrAt(Node *node, int n){
    if(n==0){
        return node;
    }
    return NodeGetPtrAt(node->next, n-1);
}

void LlAppend(Llist *a, void *value){
    Node *tmpNode = malloc(sizeof(*tmpNode));
    tmpNode->value = value;
    tmpNode->next = NULL;
    if(a->size==0){
        a->ptr = tmpNode;
        a->size+=1;
    } else {
        Node *lastNode = NodeGetPtrAt(a->ptr, a->size-1);
        lastNode->next = tmpNode;
        tmpNode->prev = lastNode; 
        a->size+=1;
    }
}

int LlPop(Llist *a, int index){
    if (index<0 || index>a->size){
        return -1;
    }
    Node *nodeCurr=NodeGetPtrAt(a->ptr, index);
    if(index==0){
        a->ptr = nodeCurr->next;
        a->ptr->prev = NULL;
    } else {
        nodeCurr->prev->next = nodeCurr->next;
        nodeCurr->next->prev = nodeCurr->prev;
    }
    free(nodeCurr);
    a->size--;
    return 0;
}

void *LlGetAt(Llist *a, int index){
    return NodeGetPtrAt(a->ptr, index)->value;
}
#endif
