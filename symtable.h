// Custom Symbol table using a hash table and separate chaining
#ifndef SYMTABLE_H
#define SYMTABLE_H
#include "types.h"
#define TABLESIZE 1000 // 1000 symbols should be ok for our compiler proj
typedef struct symnode {
    char *symbol;
    var_type varType;
    struct symnode* nextSymbol; // for getting to next symbol
    struct symnode* oldContext; // for accessing old contexts
} SymNode;

typedef struct symtable {
    struct symnode table[TABLESIZE];
} SymTable;
#endif
