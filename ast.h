#ifndef AST_H
#define AST_H

#include "symtable.h"

enum type_spec {INT,VOID};
enum fact_type {EXPR,VAR,CALL,NUMFACT};
enum relop {LT,GT,GTE,LTE,EQ,NEQ,UNARYREL = 0};
enum mulop {MULT,DIV,UNARYMULT=0};
enum addop {PLUS,MINUS,UNARYADD=0};

enum ast_node_type {
    PROGRAM_NODE,
    DEC_LIST_NODE,
    VAR_DEC_NODE,
    FUNC_DEC_NODE,
    PARAMS_NODE,
    PARAM_NODE,
    CMP_STMT_NODE,
    LOCAL_DECS_NODE,
    STMT_LIST_NODE,
    STMT_NODE,
    EXPR_STMT_NODE,
    SEL_STMT_NODE,
    ITER_STMT_NODE,
    RET_STMT_NODE,
    EXPR_NODE,
    VAR_NODE,
    SMP_EXPR_NODE,
    RELOP_NODE,
    ADD_EXPR_NODE,
    TERM_NODE,
    FACT_NODE,
    CALL_NODE,
    ARGS_NODE,
    UNKNOWN_NODE = 0
};


struct program_node {
    enum ast_node_type nodeType;
    struct dec_list_node* decList;
};

struct dec_list_node {
    enum ast_node_type nodeType;
    struct var_dec_node* var;
    struct func_dec_node* func;
    struct dec_list_node* nextDeclaration;
}; // keeps a list of all the nodes

struct var_dec_node {
    enum ast_node_type nodeType;
    enum type_spec type; // INT or VOID
    struct sym_node* id;
    int arraySize;
};

struct func_dec_node {
    enum ast_node_type nodeType;
    enum type_spec type;
    struct sym_node* id;
    struct params_node* params;
    struct cmp_stmt_node* stmt;
};

struct params_node { // LL of parameters with ID's and types
    enum ast_node_type nodeType;
    enum type_spec type;
    struct params_node* next;
    struct sym_node* id;
};

struct cmp_stmt_node {
    enum ast_node_type nodeType;
    struct local_decs_node* local_dec;
    struct stmt_node* stmt;
};

struct local_decs_node {
    enum ast_node_type nodeType;
    struct var_dec_node* var_dec;
    struct local_decs_node* next;
};

struct stmt_node {
    enum ast_node_type nodeType;
    enum stmt_type {EXPRST,CMP,SEL,ITER,RET,UNARYSTMT=0} expr_type;
    struct stmt_node* next;
    union sub_stmt {
        struct expr_stmt_node* expr_stmt;
        struct cmp_stmt_node* cmp_stmt;
        struct sel_stmt_node* sel_stmt;
        struct iter_stmt_node* iter_stmt;
        struct ret_stmt_node* ret_stmt;
    } typed_stmt;
};

struct expr_stmt_node { // var = sth or 3+3*5
    enum ast_node_type nodeType;
    struct expr_node* expr;
};

struct sel_stmt_node { // for if then else
    enum ast_node_type nodeType;
    struct expr_node* if_expr;
    struct stmt_node* if_stmt;
    struct stmt_node* else_stmt;
};

struct iter_stmt_node { // for while
    enum ast_node_type nodeType;
    struct expr_node* while_expr;
    struct stmt_node* while_stmt;
};

struct ret_stmt_node { // return statement
    enum ast_node_type nodeType;
    struct expr_node* ret_expr;
};

struct expr_node {
    enum ast_node_type nodeType;
    struct var_node* var;
    struct expr_node* expr;
    struct smp_expr_node* smp_expr;
};

struct var_node {
    enum ast_node_type nodeType;
    struct sym_node* id;
    struct expr_node* array_expr;
};

struct smp_expr_node {
    enum ast_node_type nodeType;
    enum relop op;
    struct add_expr_node* left;
    struct add_expr_node* right;
};

struct add_expr_node {
    enum ast_node_type nodeType;
    enum addop op;
    struct add_expr_node* expr;
    struct term_node* term;
};

struct term_node {
    enum ast_node_type nodeType;
    enum mulop op;
    struct term_node* term;
    struct factor_node* factor;
};

struct factor_node {
    enum ast_node_type nodeType;
    enum fact_type factor_type;
    union fact_union {
        struct expr_node* expr;
        struct var_node* var;
        struct call_node* call;
        int number;
    } factor;
};

struct call_node {
    enum ast_node_type nodeType;
    struct sym_node* id;
    struct args_node* args;
};

struct args_node {
    enum ast_node_type nodeType;
    struct expr_node* arg;
    struct args_node* nextArg;
};

struct ast_node {
    enum ast_node_type nodeType;
};

struct program_node* ast_new_program_node(struct dec_list_node* decList);

struct dec_list_node* ast_new_dec_list_node(struct var_dec_node* var, struct func_dec_node* func, struct dec_list_node* nextDeclaration);

struct dec_list_node* ast_link_dec_list_node(struct dec_list_node* root, struct dec_list_node* attachee);

struct var_dec_node* ast_new_var_dec_node(enum type_spec type, struct sym_node* id, int arraySize);

struct func_dec_node* ast_new_func_dec_node(enum type_spec type, struct sym_node* id, struct params_node* params, struct cmp_stmt_node* stmt);


struct params_node* ast_new_params_node(enum type_spec type, struct sym_node* id, struct params_node* next);

struct params_node* ast_link_params_node(struct params_node* root, struct params_node*attachee);

struct cmp_stmt_node* ast_new_cmp_stmt_node(struct local_decs_node* local_dec, struct stmt_node* stmt);

struct local_decs_node* ast_new_local_decs_node(struct var_dec_node* var_dec, struct local_decs_node* next);


struct stmt_node* ast_new_stmt_node(enum stmt_type type, struct stmt_node* next, union sub_stmt typed_stmt);

struct stmt_node* ast_link_stmt_node(struct stmt_node* root, struct stmt_node* attachee);

struct expr_stmt_node* ast_new_expr_stmt_node(struct expr_node* expr);

struct sel_stmt_node* ast_new_sel_stmt_node(struct expr_node* if_expr, struct stmt_node* if_stmt, struct stmt_node* else_stmt);

struct iter_stmt_node* ast_new_iter_stmt_node(struct expr_node* while_expr, struct stmt_node* while_stmt);

struct ret_stmt_node* ast_new_ret_stmt_node(struct expr_node* ret_expr);

struct expr_node* ast_new_expr_node(struct var_node* var, struct expr_node* expr, struct smp_expr_node* smp_expr);

struct var_node* ast_new_var_node(struct sym_node* id, struct expr_node* array_expr);
struct smp_expr_node* ast_new_smp_expr_node(enum relop op, struct add_expr_node* left, struct add_expr_node* right);

struct add_expr_node* ast_new_add_expr_node(enum addop op, struct add_expr_node* expr, struct term_node* term);

struct term_node* ast_new_term_node(enum mulop op, struct term_node* term, struct factor_node* factor);

struct factor_node* ast_new_factor_node(enum fact_type factor_type, union fact_union factor);

struct call_node* ast_new_call_node(struct sym_node* id, struct args_node* args);

struct args_node* ast_new_args_node(struct expr_node* arg, struct args_node* nextArg);

void print_ast_tree(struct ast_node* root);

void free_ast_tree(struct ast_node* root); // freeing function

#endif
