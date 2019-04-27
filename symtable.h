// Custom Symbol table using a hash table and separate chaining
#ifndef SYMTABLE_H
#define SYMTABLE_H
#define TABLE_SIZE 10000 // 100000 symbols should be ok for our compiler proj
#include "id.h"
#include "ast.h"
#include "stack.h"

struct sym_node {
    enum id_type idType; // whether it is a function, variable or array
    enum type_spec valType; // whether it is a void or int
    char *symbol; // symbol this node holds
    struct sym_node* nextSymbol; // for getting to next symbol
    struct sym_node* prevScope; // for accessing old contexts
    struct params_node* params; // for func parameters
    int scope;
    int varNo; // number associated with the id
    int isParamVar;
    int varSize; // amount of space the variable takes
};

struct sym_table {
    struct sym_node* hashtable[TABLE_SIZE]; // array of symnode pointers
    int currScope;
    struct stack* currVarNoStack;
};

int hashSymbol(const char *symbol);

struct sym_table* new_table(void);

struct sym_node* table_find(struct sym_table *table, const char *someSymbol);

struct sym_node* table_add(
        struct sym_table *table,
        const char *symbol,
        enum id_type idType,
        enum type_spec valType,
        struct params_node* params,
        int isParamVar,
        int varSize);

int table_in_scope(struct sym_table* table, const char* id);

void table_enter_scope(struct sym_table* table);

void table_exit_scope(struct sym_table* table);

void table_end_param_decs(struct sym_table *table);

void free_table(struct sym_table* table);

void print_table(struct sym_table* t);

void print_node(struct sym_node* n);

#endif
