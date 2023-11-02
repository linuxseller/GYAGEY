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
    void (*free)(void*);
} Llist;

Llist LlInit(void){
    return (Llist){NULL, 0, NULL};
}

Node *NodeGetPtrAt(Node *node, int n){
    if(n==0){
        return node;
    }
    return NodeGetPtrAt(node->next, n-1);
}

void LlAppend(Llist *ll, void *value){
    Node *tmpNode = malloc(sizeof(*tmpNode));
    tmpNode->value = value;
    tmpNode->next = NULL;
    if(ll->size==0){
        ll->ptr = tmpNode;
        ll->size+=1;
    } else {
        Node *lastNode = NodeGetPtrAt(ll->ptr, ll->size-1);
        lastNode->next = tmpNode;
        tmpNode->prev = lastNode; 
        ll->size+=1;
    }
}

int LlPop(Llist *ll, int index){
    if (index<0 || index>ll->size){
        return -1;
    }
    Node *nodeCurr=NodeGetPtrAt(ll->ptr, index);
    if(index==0){
        ll->ptr = nodeCurr->next;
        ll->ptr->prev = NULL;
    } else {
        nodeCurr->prev->next = nodeCurr->next;
        nodeCurr->next->prev = nodeCurr->prev;
    }
    free(nodeCurr);
    ll->size--;
    return 0;
}

void *LlGetAt(Llist *ll, int index){
    return NodeGetPtrAt(ll->ptr, index)->value;
}

void LlFree(Llist *ll){
    Node* node = ll->ptr;
    while(node!=NULL){
        ll->free(node->value);
        Node *curnode = node;
        node=node->next;
        free(curnode);
    }
}

#endif
