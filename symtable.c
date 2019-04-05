#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtable.h"

struct sym_table* newTable() {
    struct sym_table* table = malloc(sizeof(struct sym_table));
    int i;
    for(i = 0;i < TABLE_SIZE; i++)
        table->hashtable[i] = NULL; // zero out the table
    table->currScope = 0;
    return table;
}

int hashSymbol(char *symbol) {
    unsigned long hash = 5381;
    int c;
    while ((c = *(symbol++)))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

struct sym_node* table_find(struct sym_table* table, char *symbol) {
    struct sym_node *entry = table->hashtable[hashSymbol(symbol)];
    // if the first entry doesn't match, search LL
    if (!(strncmp(entry->symbol,symbol,100) == 0)) { // only compare 100 chars
        entry = entry->nextSymbol; // goto next
        while (entry != NULL && !(strncmp(entry->symbol,symbol,100) == 0))
            entry = entry->nextSymbol; // while it doesn't match, go forward
    }
    return entry;
}

struct sym_node* table_add(struct sym_table *table, char *symbol,enum idType type) {
    int key = hashSymbol(symbol);
    struct sym_node *entry = table->hashtable[key];

    if (entry == NULL) { // if 
        entry = malloc(sizeof(struct sym_node)); // add a new first entry
        entry->symbol = strdup(symbol); // copy in the symbol
        entry->nextSymbol = NULL;
        entry->prevScope = NULL;
        entry-> scope = 0;
        entry->type = type;
        table->hashtable[key] = entry; // add to hash table directly
        return entry;
    }
    // there was no empty space
    struct sym_node *prevEntry = entry;
    if (!(strncmp(entry->symbol,symbol,100) == 0)) { // only compare 100 chars
        entry = entry->nextSymbol;
        while (entry != NULL && !(strncmp(entry->symbol,symbol,100) == 0)) {
            prevEntry = entry;
            entry = entry->nextSymbol;
        }
    }

    if (entry == NULL) { // if it found an empty spot
        entry = malloc(sizeof(struct sym_node)); // add a new first entry
        entry->symbol = strdup(symbol); // copy in the symbol
        entry->nextSymbol = NULL;
        entry->prevScope = NULL;
        entry->scope = table->currScope;
        entry->type = type;
        prevEntry->nextSymbol = entry; // hook up this entry
        return entry;
    }
    else
        return entry; // found it 
    /*
    else if (entry->scope <  { // it found the same symbol name
        entry = malloc(sizeof(struct sym_node)); // add a new first entry
        entry->symbol = strdup(symbol); // copy in the symbol
        entry->nextSymbol = prevEntry->nextSymbol;
        entry->prevScope = prevEntry; // save the previous entry as the scope
        entry->scope = table->currScope;
        prevEntry->nextSymbol = NULL; // unhook
        return entry;
    }
    return NULL; // if the entry was found, error*/
}

int table_inScope(struct sym_table *table, char *symbol) {
    struct sym_node* entry = table_find(table, symbol); // find the current symbol
    if (entry == NULL || entry->scope != table->currScope) // if not found
        return 0;
    return 0; // 0 = false
}

void table_enterScope(struct sym_table *table) {
    table->currScope++;
    return;
}

void table_exitScope(struct sym_table *table) {
    table->currScope--;
    return;
}

void free_list(struct sym_node *node) { // free recursive lists
    if (node == NULL)
        return;
    if (node->nextSymbol != NULL)
        free_list(node->nextSymbol);
    if (node->prevScope != NULL)
        free_list(node->prevScope);
    free(node);
}
void free_table(struct sym_table *table) {
    if (table == NULL)
        return;
    int i;
    for(i = 0;i < TABLE_SIZE; i++)
        free_list(table->hashtable[i]); // free the nodes
    free(table);
}



#ifdef DEBUG
void printNode(struct sym_node* n) {
    int key = hashSymbol(n->symbol);
    printf("key: %d\n",key);
    printf("symbol: %s\n",n->symbol);
    printf("next symbol: %p\n",n->nextSymbol);
    if (n->nextSymbol != NULL)
        printf("next symbol value: %s\n",n->nextSymbol->symbol);
    printf("previous scope: %p\n",n->prevScope);
}
#endif
