// Symtable for variable names
// Written by Will Sumner
// CS 1622: Intro to Compilers
// University of Pittsburgh, Spring 2019
// This file contains functions for manipulating the symbol table,
// which is a separate chaining hashtable with the djb2 hash from Dan Bernstein
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtable.h"
#include "id.h"
#include "stack.h"

int hashSymbol(const char *symbol) { // djb2 algorithm
    unsigned long hash = 5381;
    int c;
    while ((c = *(symbol++)))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

struct sym_table* new_table() {
    struct sym_table* table = malloc(sizeof(struct sym_table));
    int i;
    for(i = 0;i < TABLE_SIZE; i++)
        table->hashtable[i] = NULL; // zero out the table
    table->currScope = 0;
    table->currVarNoStack = new_stack();
    return table;
}

struct sym_node* table_find(struct sym_table* table, const char *symbol) {
    struct sym_node* entry = table->hashtable[hashSymbol(symbol)];
    if (entry == NULL)
        return NULL;
    // if the first entry doesn't match, search LL
    if (!(strncmp(entry->symbol,symbol,100) == 0)) { // only compare 100 chars
        entry = entry->nextSymbol; // goto next
        while (entry != NULL && !(strncmp(entry->symbol,symbol,100) == 0))
            entry = entry->nextSymbol; // while it doesn't match, go forward
    }
    return entry;
}

// Add a symbol to the symbol table
// Criteria are:
// if no variable dec for scope and
// current scope

struct sym_node* table_add(
        struct sym_table *table,
        const char *symbol,
        enum id_type idType,
        enum type_spec valType,
        struct params_node* params,
        int isParamVar)
{
    int key = hashSymbol(symbol);
    struct sym_node *entry = table->hashtable[key];

    if (entry == NULL) { // if empty spot was found
        entry = malloc(sizeof(struct sym_node)); // add a new first entry
        entry->symbol = strdup(symbol); // copy in the symbol
        entry->nextSymbol = NULL;
        entry->prevScope = NULL;
        entry->scope = table->currScope;
        entry->idType = idType;
        entry->valType = valType;
        entry->params = params;
        entry->varNo = return_and_increment(table->currVarNoStack);
        entry->isParamVar = isParamVar;
        table->hashtable[key] = entry; // add to hash table directly
        return entry;
    }
    // there was no empty space
    struct sym_node *prevEntry = NULL;
    if (!(strncmp(entry->symbol,symbol,100) == 0)) // if the first element is not a match
    {
        prevEntry = entry;
        entry = entry->nextSymbol;
        while (entry != NULL && !(strncmp(entry->symbol,symbol,100) == 0)) {
            prevEntry = entry;
            entry = entry->nextSymbol;
        }
    }

    // entry is only null when there was no match found
    if (entry == NULL) { 
        entry = malloc(sizeof(struct sym_node)); // add a new first entry
        entry->symbol = strdup(symbol); // copy in the symbol
        entry->nextSymbol = NULL;
        entry->prevScope = NULL;
        entry->scope = table->currScope; // new entry with current scope
        entry->idType = idType;
        entry->valType = valType;
        entry->params = params;
        entry->varNo = return_and_increment(table->currVarNoStack);
        entry->isParamVar = isParamVar;
        prevEntry->nextSymbol = entry; // hook up this entry
        return entry;
    }
    else { 
        if (table->currScope > entry->scope) // if we're on a new scope
            if (prevEntry == NULL) { // if at the top of a chaining list
                prevEntry = entry; // save this top of the stack
                entry = malloc(sizeof(struct sym_node)); // add a new first entry
                entry->symbol = strdup(symbol); // copy in the symbol
                entry->nextSymbol = prevEntry->nextSymbol;
                prevEntry->nextSymbol = NULL; // disconnect
                entry->prevScope = prevEntry;
                entry->scope = table->currScope;
                entry->idType = idType;
                entry->valType = valType;
                entry->params = params;
                entry->varNo = return_and_increment(table->currVarNoStack);
                entry->isParamVar = isParamVar;
                table->hashtable[key] = entry; // add to hash table directly
                return entry;
            } else { // if somewhere in the chaining list
                struct sym_node* newNode =  malloc(sizeof(struct sym_node)); // add a new first entry
                newNode->symbol = strdup(symbol);
                newNode->nextSymbol = entry->nextSymbol;
                entry->nextSymbol = NULL;
                prevEntry->nextSymbol = newNode; // chain in the new node
                newNode->prevScope = entry; // set the old scope to be the entry
                newNode->idType = idType;
                newNode->valType = valType;
                newNode->params = params;
                newNode->scope = table->currScope;
                newNode->varNo = return_and_increment(table->currVarNoStack);
                newNode->isParamVar = isParamVar;
                return newNode; // return the new guy
            }
        else if (table->currScope == entry->scope) // redeclaring an entr in same scope
            return NULL;
        else /* if currScope < scope */ { // undeleted item
            return NULL;
        }
    }
    return NULL; // some magical error
}

int table_in_scope(struct sym_table *table, const char *symbol) {
    struct sym_node* entry = table_find(table, symbol); // find the current symbol
    if (entry == NULL || entry->scope < table->currScope) // if not found
        return 0;
    return 1;
}


void table_enter_scope(struct sym_table *table) {
    table->currScope++;
    stack_push(table->currVarNoStack,1); // now start counting at 1
    return;
}

void free_sym_node(struct sym_node *node) {
    if (node->symbol != NULL)
        free(node->symbol);
    free(node);
}

static struct sym_node* pop_to_scope(struct sym_node* node, int currentScope) {
    if (node == NULL)
        return NULL;
    struct sym_node* connectingNode = node->nextSymbol;
    struct sym_node* old = node;
    while (node != NULL && node->scope > currentScope) {
        old = node;
        node = node->prevScope;
        free_sym_node(old);
    }
    if (node == NULL) // if we deleted a whole list
        return pop_to_scope(connectingNode,currentScope); // return the top of the next list
    node->nextSymbol = connectingNode;
    return node; // otherwise return the last node that wasn't popped
}

//Clear all entries >= the scope number that is being exited
void table_exit_scope(struct sym_table *table) {
    table->currScope -= 1;
    stack_pop(table->currVarNoStack); // just pop off and restore the oldest var no
    int i;
    struct sym_node* node;
    for (i = 0; i < TABLE_SIZE; i++) {
        node = pop_to_scope(table->hashtable[i],table->currScope);
        table->hashtable[i] = node; // set the new top
        while(node!=NULL) {
            node->nextSymbol = pop_to_scope(node->nextSymbol,table->currScope);
            node = node->nextSymbol;
        }
    }
    return;
}

void free_list(struct sym_node *node) { // free recursive lists
    if (node == NULL)
        return;
    if (node->nextSymbol != NULL)
        free_list(node->nextSymbol);
    if (node->prevScope != NULL)
        free_list(node->prevScope);
    free_sym_node(node);
}

void free_table(struct sym_table *table) {
    if (table == NULL)
        return;
    int i;
    for(i = 0;i < TABLE_SIZE; i++)
        free_list(table->hashtable[i]); // free the nodes
    if (table->currVarNoStack != NULL)
        free_stack(table->currVarNoStack);
    free(table);
}

#ifdef DEBUG

void print_node(struct sym_node* n) {
    if (n == NULL) {
        printf("NULL node\n");
        return;
    }
    printf("address: %p\n",n);
    int key = hashSymbol(n->symbol);
    printf("key: %d\n",key);
    printf("symbol: %s\n",n->symbol);
    printf("id type : %d\n",n->idType);
    printf("value type : %d\n",n->valType);
    printf("next symbol: %p\n",n->nextSymbol);
    if (n->nextSymbol != NULL)
        printf("next symbol value: %s\n",n->nextSymbol->symbol);
    printf("previous scope: %p\n",n->prevScope);
    printf("variable scope: %d\n",n->scope);
}

void print_nodes(struct sym_node* n) {
    if (n == NULL)
        return;
    print_node(n);
    printf("\n");
    print_nodes(n->nextSymbol);
}


void print_table(struct sym_table* t) {
    if (t == NULL) {
        printf("NULL table\n");
        return;
    }
    int i;
    struct sym_node* n;
    for (i = 0; i < TABLE_SIZE; i++) {
        n = t->hashtable[i];
        if (n == NULL)
            continue;
        printf("########\n");
        printf("index %d\n",i);
        printf("########\n");
        print_nodes(t->hashtable[i]);
    }
}
#endif
