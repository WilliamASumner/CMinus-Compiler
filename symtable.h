// Custom Symbol table using a hash table and separate chaining
#ifndef SYMTABLE_H
#define SYMTABLE_H
#define TABLE_SIZE 10 // 100000 symbols should be ok for our compiler proj

enum idType {FUNCTION,VARIABLE,ARRAY};

struct sym_node {
    char *symbol; // symbol this node holds
    struct sym_node* nextSymbol; // for getting to next symbol
    struct sym_node* prevScope; // for accessing old contexts
    enum idType type; // whether it is a function, variable or other
    int scope;
};

struct sym_table {
    struct sym_node* hashtable[TABLE_SIZE]; // array of symnode pointers
    int currScope;
};

struct sym_table*  newTable(void);

int hashSymbol(char *symbol);

struct sym_node* table_find(struct sym_table *table, char *someSymbol);

struct sym_node* table_add(struct sym_table *table, char *someSymbol, enum idType type);

int table_inScope(struct sym_table *table, char *someSymbol);

void table_enterScope(struct sym_table* table);

void table_exitScope(struct sym_table* table);

void free_table(struct sym_table* table);

#ifdef DEBUG
void printNode(struct sym_node* n);
#endif

#endif
