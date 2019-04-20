#include <stdlib.h>
#include "stack.h"

//#ifdef DEBUG
#include <stdio.h>
//#endif

struct stack_node* make_node(int val, struct stack_node* next) {
    struct stack_node* node = malloc(sizeof(struct stack_node));
    if (node == NULL)
        return NULL; // error allocating stack
    node->val = val;
    node->next_node = next;
    return node;
}

struct stack* new_stack(){
    struct stack* s = malloc(sizeof(struct stack));
    if (s == NULL)
        return NULL;
    s->head = NULL;
    return s;
}



void stack_push(struct stack* stack, int val) {
    if (stack == NULL)
        return;
    struct stack_node* newHead = make_node(val,stack->head);
    stack->head = newHead;
}

int stack_pop(struct stack* stack) {
    if (stack == NULL || stack->head == NULL)
        return -1;
    int val = stack->head->val;
    struct stack_node* oldHead = stack->head;
    stack->head = oldHead->next_node;
    free(oldHead);
    return val;
}

int stack_peek(struct stack* stack) {
    if (stack == NULL || stack->head == NULL)
        return -1;
    return stack->head->val;
}

int return_and_increment(struct stack* stack, int inc) {
    if (stack == NULL || stack->head == NULL)
        return -1;
    int val = stack->head->val;
    stack->head->val += inc;
    return val;
}

int add_stack_nodes(struct stack_node* node) {
    if (node == NULL) return 0;
    return node->val + add_stack_nodes(node->next_node);
}

int sum_all_scopes(struct stack* stack) {
    if (stack == NULL)
        return -1; // error
    return add_stack_nodes(stack->head);
}

void free_stack_list(struct stack_node* node) {
    if (node == NULL)
        return;
    if (node->next_node != NULL) {
        free_stack_list(node->next_node);
        node->next_node = NULL;
    }
    free(node);
}

void free_stack(struct stack* stack) {
    if (stack == NULL)
        return;
    free_stack_list(stack->head);
}

int is_empty(struct stack* stack) {
    if (stack->head == NULL)
        return 1;
    return 0;
}
struct stack* copy_stack(struct stack* stack) {
    if (stack == NULL)
        return NULL;
    struct stack* newStack = new_stack();
    struct stack_node* curr = stack->head;
    if (curr == NULL)
        return newStack;
    struct stack_node* tail = make_node(curr->val,NULL); // new node
    newStack->head = tail;
    curr = curr->next_node;
    while (curr != NULL) {
        tail->next_node = make_node(curr->val,NULL);
        tail = tail->next_node;
        curr = curr->next_node;
    }
    return newStack;
}

struct stack* copy_stack_rev(struct stack* stack) {
    if (stack == NULL)
        return NULL;
    struct stack* newStack = new_stack();
    struct stack_node* curr = stack->head;
    while (curr != NULL) {
        stack_push(newStack,curr->val);
        curr = curr->next_node;
    }
    return newStack;
}
void rev_stack(struct stack* stack){
    if (stack == NULL)
        return;
    struct stack_node* prev = NULL;
    struct stack_node* curr = stack->head;
    struct stack_node* next = curr->next_node;
    while (next != NULL){
        curr->next_node = prev;
        prev = curr;
        curr = next;
        next = curr->next_node;
    }
    curr->next_node = prev;
    stack->head = curr;
}
//#ifdef DEBUG
void print_stack(struct stack* stack) {
    if (stack == NULL)
        return;
    struct stack_node* node = stack->head;
    while (node != NULL) {
        printf("%d ",node->val);
        node = node->next_node;
    }
    printf("\n");
}
//#endif
