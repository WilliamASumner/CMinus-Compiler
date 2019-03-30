#include <stdlib.h>
#include "symtable.h"

int hashSymbol(char *symbol) {
    int hash = 37;
    char* i;
    while (*symbol) {
        hash = hash * 54059;
    return hash;
}

int find(char *someSymbol) {
    return -1;
}

void add(char *someSymbol) {
    return;
}

int inScope(char *someSymbol) {
    return 0; // 0 = false
}

void enterScope() {
    return;
}

void exitScope() {
    return;
}
