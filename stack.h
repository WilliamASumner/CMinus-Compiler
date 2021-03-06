// Stack for the Symtable
// Written by Will Sumner
// CS 1622: Intro to Compilers
// University of Pittsburgh, Spring 2019
// This file contains definitions for the stack
#ifndef STACK_H
#define STACK_H


struct stack_node {
    int val;
    struct stack_node* next_node;
};

struct stack {
    struct stack_node* head;
};

struct stack_node* make_node(int val, struct stack_node* next);
struct stack* new_stack();
struct stack* copy_stack(struct stack* stack);
struct stack* copy_stack_rev(struct stack* stack);
void stack_push(struct stack* stack, int val);
int stack_pop(struct stack* stack);
int stack_peek(struct stack* stack);
int return_and_increment(struct stack* stack,int inc);
void stack_reset(struct stack* stack, int i);
int sum_all_scopes(struct stack* stack);
void free_stack_list(struct stack_node* node);
void free_stack(struct stack* stack);
int is_empty(struct stack* stack);
void rev_stack(struct stack* stack);

//#ifdef DEBUG
void print_stack(struct stack* stack);
//#endif



#endif
