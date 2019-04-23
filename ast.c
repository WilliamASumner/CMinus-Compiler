// The Almighty AST
// Written by Will Sumner
// CS 1622: Intro to Compilers
// University of Pittsburgh, Spring 2019
// This file contains functions for manipulating the AST
// The code here is not clean
// but it will be cleaned at a later date
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "codegen.h"
#include "errors.h"

struct program_node* ast_new_program_node(
        struct dec_list_node* decList)
{
    struct program_node* node = malloc(sizeof(struct program_node));
    node->nodeType = PROGRAM_NODE;
    node->decList = (struct dec_list_node*) decList;
    return node;
}

struct dec_list_node* ast_new_dec_list_node(
        struct var_dec_node* var,
        struct func_dec_node* func,
        struct dec_list_node* nextDeclaration)
{
    struct dec_list_node* node = malloc(sizeof(struct dec_list_node));
    node->nodeType = DEC_LIST_NODE;
    node->var = var;
    node->func = func;
    node->nextDeclaration = nextDeclaration;
    return node;
}

struct dec_list_node* ast_link_dec_list_node(
        struct dec_list_node* root,
        struct dec_list_node* attachee)
{
    root->nextDeclaration = attachee;
    return attachee;
}

struct var_dec_node* ast_new_var_dec_node(
        enum id_type idType,
        enum type_spec valType,
        char* id,
        int arraySize)
{
    struct var_dec_node* node = malloc(sizeof(struct var_dec_node));
    node->nodeType = VAR_DEC_NODE;
    node->valType = valType;
    node->idType = idType;
    node->id = id;
    node->arraySize = arraySize;
    return node;
}

struct func_dec_node* ast_new_func_dec_node(
        enum id_type idType,
        enum type_spec valType,
        char* id,
        struct params_node* params,
        struct cmp_stmt_node* stmt)
{
    struct func_dec_node* node = malloc(sizeof(struct func_dec_node));
    node->nodeType = FUNC_DEC_NODE;
    node->idType = idType;
    node->valType = valType;
    node->id = id;
    node->params = params;
    node->stmt = stmt;
    return node;
}

struct params_node* ast_new_params_node(
        enum id_type idType,
        enum type_spec valType,
        char* id,
        struct params_node* next)
{
    struct params_node* node = malloc(sizeof(struct params_node));
    node->nodeType = PARAMS_NODE;
    node->idType = idType;
    node->valType = valType;
    node->next = next;
    node->id = id;
    return node;
}

struct params_node* ast_link_params_node(
        struct params_node* root,
        struct params_node*attachee) {
    root->next = attachee;
    return attachee;
}

struct cmp_stmt_node* ast_new_cmp_stmt_node(
        struct local_decs_node* local_dec,
        struct stmt_node* stmt)
{
    struct cmp_stmt_node* node = malloc(sizeof(struct cmp_stmt_node));
    node->nodeType = CMP_STMT_NODE;
    node->local_dec = local_dec;
    node->stmt = stmt;
    return node;
}

struct local_decs_node* ast_new_local_decs_node(
        struct var_dec_node* var_dec,
        struct local_decs_node* next)
{
    struct local_decs_node* node = malloc(sizeof(struct local_decs_node));
    node->nodeType = LOCAL_DECS_NODE;
    node->var_dec = var_dec;
    node->next = next;
    return node;
}

struct stmt_node* ast_new_stmt_node(
        enum stmt_type type, 
        struct stmt_node* next,
        union sub_stmt typed_stmt)
{
    struct stmt_node* node = malloc(sizeof(struct stmt_node));
    node->nodeType = STMT_NODE;
    node->expr_type = type;
    node->next = next;
    node->typed_stmt = typed_stmt;
    return node;
}

struct stmt_node* ast_link_stmt_node(
        struct stmt_node* root,
        struct stmt_node* attachee) {
    if (root != NULL) {
        root->next = attachee;
    }
    return root;
}

struct expr_stmt_node* ast_new_expr_stmt_node(
        struct expr_node* expr)
{
    struct expr_stmt_node* node = malloc(sizeof(struct expr_stmt_node));
    node->nodeType = EXPR_STMT_NODE;
    node->expr = expr;
    return node;
}

struct sel_stmt_node* ast_new_sel_stmt_node(
        struct expr_node* if_expr,
        struct stmt_node* if_stmt,
        struct stmt_node* else_stmt)
{
    struct sel_stmt_node* node = malloc(sizeof(struct sel_stmt_node));
    node->nodeType = SEL_STMT_NODE;
    node->if_expr = if_expr;
    node->if_stmt = if_stmt;
    node->else_stmt = else_stmt;
    return node;
}

struct iter_stmt_node* ast_new_iter_stmt_node(
        struct expr_node* while_expr,
        struct stmt_node* while_stmt)
{
    struct iter_stmt_node* node = malloc(sizeof(struct iter_stmt_node));
    node->nodeType = ITER_STMT_NODE;
    node->while_expr = while_expr;
    node->while_stmt = while_stmt;
    return node;
}

struct ret_stmt_node* ast_new_ret_stmt_node(
        struct expr_node* ret_expr)
{
    struct ret_stmt_node* node = malloc(sizeof(struct ret_stmt_node));
    node->nodeType = RET_STMT_NODE;
    node->ret_expr = ret_expr;
    return node;
}

struct expr_node* ast_new_expr_node(
        struct var_node* var,
        struct expr_node* expr,
        struct smp_expr_node* smp_expr)
{
    struct expr_node* node = malloc(sizeof(struct expr_node));
    node->nodeType = EXPR_NODE;
    node->var = var;
    node->expr = expr;
    node->smp_expr = smp_expr;
    return node;
}

struct var_node* ast_new_var_node(
        enum id_type idType,
        enum type_spec valType,
        char* id,
        struct expr_node* array_expr)
{
    struct var_node* node = malloc(sizeof(struct var_node));
    node->nodeType = VAR_NODE;
    node->idType = idType;
    node->valType = valType;
    node->id = id;
    node->array_expr = array_expr;
    return node;
}

struct smp_expr_node* ast_new_smp_expr_node(
        enum relop op,
        struct add_expr_node* left,
        struct add_expr_node* right)
{
    struct smp_expr_node* node = malloc(sizeof(struct smp_expr_node));
    node->nodeType = SMP_EXPR_NODE;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

struct add_expr_node* ast_new_add_expr_node(
        enum addop op,
        struct add_expr_node* expr,
        struct term_node* term)
{
    struct add_expr_node* node = malloc(sizeof(struct add_expr_node));
    node->nodeType = ADD_EXPR_NODE;
    node->op = op;
    node->expr = expr;
    node->term = term;
    return node;
}

struct term_node* ast_new_term_node(
        enum mulop op,
        struct term_node* term,
        struct factor_node* factor)
{
    struct term_node* node = malloc(sizeof(struct term_node));
    node->nodeType = TERM_NODE;
    node->op = op;
    node->term = term;
    node->factor = factor;
    return node;
}

struct factor_node* ast_new_factor_node(
        enum fact_type factor_type,
        union fact_union factor)
{
    struct factor_node* node = malloc(sizeof(struct factor_node));
    node->nodeType = FACT_NODE;
    node->factor_type = factor_type;
    node->factor = factor;
    return node;
}

struct call_node* ast_new_call_node(
        char* id,
        struct args_node* args)
{
    struct call_node* node = malloc(sizeof(struct call_node));
    node->nodeType = CALL_NODE;
    node->id = id;
    node->args = args;
    return node;
}

struct args_node* ast_new_args_node(
        struct expr_node* arg,
        struct args_node* nextArg)
{
    struct args_node* node = malloc(sizeof(struct args_node));
    node->nodeType = ARGS_NODE;
    node->arg = arg;
    node->nextArg = nextArg;
    return node;
}

struct args_node* ast_link_args_node(
        struct args_node* root,
        struct args_node* attachee)
{
    root->nextArg = attachee;
    return attachee;
}

struct stmt_node* reverse_stmt_list(
        struct stmt_node* root)
{
    struct stmt_node* curr = root;
    struct stmt_node* prev = NULL;
    struct stmt_node* next = NULL;
    while(curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

struct local_decs_node* reverse_local_dec_list(
        struct local_decs_node* root)
{
    struct local_decs_node* curr = root;
    struct local_decs_node* prev = NULL;
    struct local_decs_node* next = NULL;
    while(curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev; // return the new head
}

void organize_global_decs(struct ast_node* ast) { // push all global var decs to front
    if (ast == NULL || ast->nodeType != PROGRAM_NODE) return;
    struct program_node* prog = (struct program_node*)ast;
    struct dec_list_node* tail = prog->decList;
    struct dec_list_node* lead = tail->nextDeclaration;
    struct dec_list_node* prev = tail;

    // first get tail to be a var dec
    if (tail->var == NULL) {
        while (lead != NULL) {
            if (lead->var != NULL) // if a var dec found
                break;
            prev = lead;
            lead = lead->nextDeclaration;
        }
        if (lead != NULL) { // if a var dec was found
            prev->nextDeclaration = lead->nextDeclaration;
            lead->nextDeclaration = tail;
            prog->decList = lead;
            tail = lead;
            lead = tail->nextDeclaration;
            prev = tail;
        }
        else { return; } // just leave, only one func declaration
    }

    // by now we have a var in the first part of the declist
    while (lead != NULL) { // loop through entire structure
        if (lead->var != NULL) {
            prev->nextDeclaration = lead->nextDeclaration;
            lead->nextDeclaration = tail->nextDeclaration;
            tail->nextDeclaration = lead;
            tail = lead;
            lead = tail->nextDeclaration;
            prev = tail;
        }
        else {
            prev = lead;
            lead = lead->nextDeclaration;
        }
    }

}

void gencode_ast_tree(struct ast_node* root) { // prep AST tree and gen code
    if (root->nodeType != PROGRAM_NODE)
        return;
    organize_global_decs(root); // for easier globals
    gencode((struct ast_node*)root); // generate that code!
}

// Helper function to control indenting
void print_indent(int indent, FILE* outFile) {
    int i = 0;
    while (i++ < indent*2)
        fprintf(outFile," ");
}

// Semi-pretty printing function for AST
void print_ast_tree(struct ast_node* root, FILE* outFile) {
    if (root == NULL) return;
    static int indent = 0; // controlling how much indent there is
    switch(root->nodeType) {
        case ARGS_NODE:
        case DEC_LIST_NODE:
        case ADD_EXPR_NODE:
        case VAR_NODE:
        case STMT_NODE:
        case EXPR_STMT_NODE:
        case LOCAL_DECS_NODE:
        case EXPR_NODE:
        case SMP_EXPR_NODE:
        case FACT_NODE:
        case TERM_NODE:
        case CALL_NODE:
        case VAR_DEC_NODE:
        case FUNC_DEC_NODE:
            break;
        default:
            print_indent(indent,outFile);
    }
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*)root;
                fprintf(outFile,"[program\n");
                indent = 1; // reset on a program node (a root)
                print_ast_tree((struct ast_node*)r->decList,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case DEC_LIST_NODE:
            {
                struct dec_list_node* r = (struct dec_list_node*)root;
                print_ast_tree((struct ast_node*)r->var,outFile);
                print_ast_tree((struct ast_node*)r->func,outFile);
                print_ast_tree((struct ast_node*)r->nextDeclaration,outFile);
                break;
            }

        case VAR_DEC_NODE:
            {
                struct var_dec_node* r = (struct var_dec_node*)root;
                char* t = r->valType == INT ? "int" : "void";
                print_indent(indent,outFile);
                fprintf(outFile,"[var-declaration [%s] [%s]",t,r->id);
                if (r->arraySize > 0)
                    fprintf(outFile," [%d]",r->arraySize);
                fprintf(outFile,"]\n");
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                if (strncmp(r->id,"output",7) == 0 || strncmp(r->id,"input",6) == 0)
                    break;
                print_indent(indent,outFile);
                char* t = r->valType == INT ? "int" : "void";
                fprintf(outFile,"[fun-declaration\n");
                indent++;
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",t);
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",r->id);

                print_indent(indent,outFile);
                fprintf(outFile,"[params");
                if (r->params != NULL) {
                    fprintf(outFile,"\n");
                }
                indent++;
                print_ast_tree((struct ast_node*)r->params,outFile);
                fprintf(outFile,"]\n"); // close params
                indent--;

                print_ast_tree((struct ast_node*)r->stmt,outFile);
                indent--;
                print_indent(indent,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                char* t = r->valType == INT ? "int" : "void";
                fprintf(outFile,"[param [%s] [%s]",t,r->id);
                if (r->idType == ARRAY)
                    fprintf(outFile," [\\[\\]]");
                fprintf(outFile,"]");
                if (r->next != NULL) {
                    fprintf(outFile,"\n");
                    print_ast_tree((struct ast_node*)r->next,outFile);
                }
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                fprintf(outFile,"[compound-stmt\n");
                indent++;
                print_ast_tree((struct ast_node*)r->local_dec,outFile);
                print_ast_tree((struct ast_node*)r->stmt,outFile);
                indent--;
                printf("\n");
                print_indent(indent,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                // no printing to be done here
                print_ast_tree((struct ast_node*)r->var_dec,outFile);
                print_ast_tree((struct ast_node*)r->next,outFile);
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                switch (r->expr_type) {
                    case EXPRST:
                        print_ast_tree((struct ast_node*)r->typed_stmt.expr_stmt,outFile);
                        break;
                    case CMP:
                        print_ast_tree((struct ast_node*)r->typed_stmt.cmp_stmt,outFile);
                        break;
                    case SEL:
                        print_ast_tree((struct ast_node*)r->typed_stmt.sel_stmt,outFile);
                        break;
                    case ITER:
                        print_ast_tree((struct ast_node*)r->typed_stmt.iter_stmt,outFile);
                        break;
                    case RET:
                        print_ast_tree((struct ast_node*)r->typed_stmt.ret_stmt,outFile);
                        break;
                    default:
                        fprintf(stderr,"error printing ast tree\n");
                        exit(1);
                }
                print_ast_tree((struct ast_node*)r->next,outFile);
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                if (r->expr == NULL) {
                    print_indent(indent,outFile);
                    fprintf(outFile,"[;]\n");
                }
                else
                    print_ast_tree((struct ast_node*)r->expr,outFile);
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                fprintf(outFile,"[selection-stmt");
                indent++;
                print_indent(indent,outFile);
                print_ast_tree((struct ast_node*)r->if_expr,outFile);
                fprintf(outFile,"\n");
                print_indent(indent,outFile);
                print_ast_tree((struct ast_node*)r->if_stmt,outFile);
                if (r->else_stmt != NULL) {
                    print_indent(indent,outFile);
                    fprintf(outFile,"\n");
                }

                print_ast_tree((struct ast_node*)r->else_stmt,outFile);
                fprintf(outFile,"\n");
                indent--;
                print_indent(indent,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case ITER_STMT_NODE:
            {
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                fprintf(outFile,"[iteration-stmt");
                indent++;
                print_ast_tree((struct ast_node*)r->while_expr,outFile);
                fprintf(outFile,"\n");
                print_ast_tree((struct ast_node*)r->while_stmt,outFile);
                indent--;
                print_indent(indent,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                fprintf(outFile,"[return-stmt");
                if (r->ret_expr != NULL)
                    fprintf(outFile," ");
                print_ast_tree((struct ast_node*)r->ret_expr,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL) {
                    print_indent(indent,outFile);
                    fprintf(outFile,"[= ");
                    print_ast_tree((struct ast_node*)r->var,outFile);
                    if (r->expr == NULL) {
                        fprintf(stderr,"error: malformed ast, missing expr in expr node");
                        exit(1);
                    }
                    fprintf(outFile," ");
                    print_ast_tree((struct ast_node*)r->expr,outFile);
                }
                else {
                    print_ast_tree((struct ast_node*)r->smp_expr,outFile);
                }
                if (r->var != NULL)
                    fprintf(outFile,"]\n");
                else
                    fprintf(outFile,"\n");
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                fprintf(outFile,"[");
                fprintf(outFile,"var [%s]",r->id);
                print_ast_tree((struct ast_node*)r->array_expr,outFile);
                fprintf(outFile,"]");
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                if (r->op != UNARYREL) {
                    fprintf(outFile,"\n");
                    print_indent(indent,outFile);
                }
                switch(r->op) {
                    case LT:
                        fprintf(outFile,"[< ");
                        break;
                    case GT:
                        fprintf(outFile,"[> ");
                        break;
                    case GTE:
                        fprintf(outFile,"[>= ");
                        break;
                    case LTE:
                        fprintf(outFile,"[<= ");
                        break;
                    case EQ:
                        fprintf(outFile,"[== ");
                        break;
                    case NEQ:
                        fprintf(outFile,"[!= ");
                        break;
                    case UNARYREL:
                        break;
                    default:
                        fprintf(stderr,"error printing ast: improper relop\n");
                }

                print_ast_tree((struct ast_node*)r->left,outFile);
                print_ast_tree((struct ast_node*)r->right,outFile);
                if (r->op != UNARYREL)
                    fprintf(outFile,"]");
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                if  (r->op != UNARYADD) {
                    fprintf(outFile,"\n");
                    print_indent(indent,outFile);
                }
                switch (r->op) {
                    case PLUS:
                        fprintf(outFile,"[+ ");
                        break;
                    case MINUS:
                        fprintf(outFile,"[- ");
                        break;
                    case UNARYADD:
                        break;
                    default:
                        fprintf(stderr,"error printing ast: improper addop\n");
                }
                print_ast_tree((struct ast_node*)r->expr,outFile);
                print_ast_tree((struct ast_node*)r->term,outFile);
                if (r->op != UNARYADD)
                    fprintf(outFile,"]");
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                switch (r->op) {
                    case MULT:
                        fprintf(outFile,"[* ");
                        break;
                    case DIV:
                        fprintf(outFile,"[/ ");
                    case UNARYMULT:
                        break;
                    default:
                        fprintf(stderr,"error printing ast: improper multop\n");
                }
                print_ast_tree((struct ast_node*)r->term,outFile);
                print_ast_tree((struct ast_node*)r->factor,outFile);
                if (r->op != UNARYMULT)
                    fprintf(outFile,"]");
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        print_ast_tree((struct ast_node*)r->factor.expr,outFile);
                        break;
                    case VAR:
                        print_ast_tree((struct ast_node*)r->factor.var,outFile);
                        break;
                    case CALL:
                        print_ast_tree((struct ast_node*)r->factor.call,outFile);
                        break;
                    case NUMFACT: // nothing to do on a number
                        fprintf(outFile,"[%d]",r->factor.number);
                        break;
                    default:
                        fprintf(stderr,"error: malformed AST, improper factor_type %d in fact node\n",r->factor_type);
                        exit(1);
                }
                break;
            }
        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                fprintf(outFile,"\n");
                print_indent(indent,outFile);
                fprintf(outFile,"[call\n");
                indent++;
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",r->id);
                print_indent(indent,outFile);
                fprintf(outFile,"[args ");
                print_ast_tree((struct ast_node*)r->args,outFile);
                fprintf(outFile,"]");
                indent--;
                fprintf(outFile,"]");
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                print_ast_tree((struct ast_node*)r->arg,outFile);
                print_ast_tree((struct ast_node*)r->nextArg,outFile);
                break;
            }
        default:
            throw_ast_error(IMPROPER_NODE_TYPE,root);
    }
}



//Type checking helper
//Returns the type of an expr

static enum type_spec get_type(struct ast_node * root) {
    if (root == NULL) {
        throw_ast_error(TYPE_CHECK_ON_NULL_NODE,root);
        return NOTYPE; // never gets here
    }
    switch(root->nodeType) {
        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL && r->expr != NULL) { // var = expr 
                    enum type_spec type = get_type((struct ast_node*)r->var);
                    if (type == get_type((struct ast_node*)r->expr))
                        return type;
                    else
                        throw_semantic_error(TYPE_MISMATCH,root);
                }

                if (r->smp_expr != NULL)
                    return get_type((struct ast_node*)r->smp_expr);
                else
                    throw_ast_error(INCOMPLETE_EXPR_NODE,root);
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                struct sym_node* n = table_find(table,r->id);
                if (n == NULL)
                    throw_semantic_error(USE_BEFORE_DEC,root);

                if (r->array_expr != NULL) {// check array expr
                    if (r->idType != ARRAY)
                        throw_semantic_error(ID_TYPE_MISMATCH,root);
                    if (get_type((struct ast_node*)r->array_expr) != INT)
                        throw_semantic_error(INVALID_ARR_ACCESS,root);
                }
                if (n->idType == ARRAY && r->idType == VARIABLE)
                    return INTARRAY; // if declared as array but used as variable
                else if (n->valType == INT && r->valType == INT)
                    return INT;
                else
                    throw_semantic_error(TYPE_MISMATCH,root);
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                enum type_spec leftType = get_type((struct ast_node*)r->left);
                if ((r->right != NULL) &&
                        (INT != get_type((struct ast_node*)r->right) ||
                         leftType != INT))
                    throw_semantic_error(NON_INT_SMP_EXPR,root);
                return leftType;
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                enum type_spec exprType;
                if (r->expr != NULL) // if it is a binary addexpr
                    exprType = get_type((struct ast_node*)r->expr);
                else
                    return get_type((struct ast_node*)r->term);
                if (exprType != get_type((struct ast_node*)r->term)
                        || exprType != INT)
                    throw_semantic_error(NON_INT_ADD_EXPR,root);
                return INT;
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                enum type_spec exprType;
                if (r->term != NULL) // if it is a binary term
                    exprType = get_type((struct ast_node*)r->term);
                else
                    return get_type((struct ast_node*)r->factor);
                if (exprType != get_type((struct ast_node*)r->factor)
                        || exprType != INT)
                    throw_semantic_error(NON_INT_TERM,root);
                return INT;
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        return get_type((struct ast_node*)r->factor.expr);
                        break;
                    case VAR:
                        return get_type((struct ast_node*)r->factor.var);
                        break;
                    case CALL:
                        return get_type((struct ast_node*)r->factor.call);
                        break;
                    case NUMFACT: // nothing to do on a number
                        return INT;
                    default:
                        throw_ast_error(INVALID_FACTOR_TYPE,root);
                }
                break;
            }

        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                struct sym_node* sym = table_find(table,r->id);
                if (sym == NULL)
                    throw_semantic_error(USE_BEFORE_DEC,root);
                return sym->valType;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                return get_type((struct ast_node*)r->arg);
            }
        default:
            throw_ast_error(IMPROPER_NODE_TYPE,root);
    }
    return VOID; // never gets here
}

int check_args(struct args_node* args, struct params_node* params) {
    if (args == NULL && params==NULL) {return 1;}
    while (args != NULL && params != NULL) {
        enum type_spec type = get_type((struct ast_node*)args->arg);
        if (type != params->valType || params->idType == ARRAY) {
            if (!(type == INTARRAY && params->idType == ARRAY))
                throw_semantic_error(CALL_MISMATCH,(struct ast_node *)args);
        }
        args = args->nextArg;
        params = params->next;
    }
    if ((struct ast_node*)args != (struct ast_node*)params) // if one is not null
        throw_semantic_error(CALL_MISMATCH, (args==NULL) ? (struct ast_node*)args : (struct ast_node*)params);
    return 0;
}


int check_declaration_main(struct func_dec_node* func) {
    if (func == NULL)
        return 0;
    if (func->nodeType != FUNC_DEC_NODE)
        return 0;
    else if (strncmp(func->id,"main",4) != 0)
        return 0;
    else if (func->params != NULL)
        return 0;
    else if (func->idType != FUNCTION)
        return 0;
    else if (func->valType != VOID)
        return 0;
    return 1;
}

//Add in I/O functions for the AST, so that redefs cause an error
void ast_add_io(struct ast_node* ast) {
    if (ast == NULL) return;
    struct params_node* output_param = ast_new_params_node(VARIABLE,INT,strdup("x"),NULL);
    if (ast->nodeType != PROGRAM_NODE)
        return; // called on invalid program
    struct program_node* prog = (struct program_node*)ast;
    if (prog->decList == NULL)
        throw_ast_error(EMPTY_DECLIST,ast);
    //create function definitions, but leave null as we'll add in our own code
    char* outName = strdup("output");
    char* inName = strdup("input");
    struct func_dec_node* outputDec = ast_new_func_dec_node(FUNCTION,VOID,outName,output_param,NULL);
    struct func_dec_node* inputDec = ast_new_func_dec_node(FUNCTION,INT,inName,NULL,NULL);
    struct dec_list_node* tempNode = ast_new_dec_list_node(NULL,inputDec,prog->decList); // input -> firstDec ... -> NULL
    tempNode = ast_new_dec_list_node(NULL,outputDec,tempNode); // output -> input -> NULL
    prog->decList = tempNode; //output -> input -> firstDec ... -> NULL, finish linking
}





int generateOutput = 0;
int generateInput = 0;

// Semantic checking for the AST,
// general flow is check node->check children->finish
void analyze_ast_tree(struct ast_node * root) {
    if (root == NULL) return;
    static char* currFunc = NULL;
    static int foundReturn = 0;
    static int didParams = 0;
    switch(root->nodeType) {
        case PROGRAM_NODE:
            { // just analyze decList, NULL returns empty prog
                struct program_node* r = (struct program_node*) root;
                analyze_ast_tree((struct ast_node*)r->decList);
                break;
            }

        case DEC_LIST_NODE:
            { // start at the first dec, check the next one if possible
                struct dec_list_node* r = (struct dec_list_node*)root;
                if (r->var != NULL && r->func == NULL)
                    analyze_ast_tree((struct ast_node*)r->var);
                else if (r->func != NULL && r->var == NULL)
                    analyze_ast_tree((struct ast_node*)r->func);
                else // double check that there is only one func/var dec
                    throw_ast_error(VAR_AND_FUNC_DEC,root);
                if (r->nextDeclaration == NULL && !(check_declaration_main(r->func))) // if last declaration
                        throw_semantic_error(LAST_DEC_NOT_VALID_MAIN,root);
                else
                    analyze_ast_tree((struct ast_node*)r->nextDeclaration); // analyze next declaration
                break;
            }

        case VAR_DEC_NODE:
            { // just check that this var dec has 
                struct var_dec_node* r = (struct var_dec_node*)root;
                if (r->id == NULL) { throw_ast_error(EMPTY_VAR_DEC,root);};
                if (r->valType == VOID)
                    throw_semantic_error(NON_INT_VAR,root);
                struct sym_node* newSym = table_add(table,r->id,r->idType,r->valType,NULL,0,r->arraySize);
                if (newSym == NULL) // redef or invalid scope
                    throw_semantic_error(REDECLARATION_OR_BAD_SCOPE,root);
                if (newSym->idType != r->idType || newSym->valType != INT)
                    throw_semantic_error(TYPE_MISMATCH,root);

                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                if (r->id == NULL) { throw_ast_error(EMPTY_FUNC_DEC,root);}
                struct sym_node* newSym = table_add(table,r->id,r->idType,r->valType,r->params,0,0);
                if (newSym == NULL) // redef or invalid scope
                    throw_semantic_error(REDECLARATION_OR_BAD_SCOPE,root);
                currFunc = r->id;
                foundReturn = 0;
                didParams = 1;
                table_enter_scope(table);
                analyze_ast_tree((struct ast_node*)r->params);
                analyze_ast_tree((struct ast_node*)r->stmt);
                table_exit_scope(table);
                if (r->valType != VOID && foundReturn != 1 && strncmp(r->id,"input",6) != 0)
                    throw_semantic_error(MISSING_RETURN,root);
                currFunc = NULL;
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                if (r->id == NULL)
                    throw_ast_error(EMPTY_VAR_DEC,root);
                struct sym_node* n = table_add(table,r->id,r->idType,r->valType,NULL,1,1); // CHECK THIS
                if (n == NULL)
                    throw_semantic_error(REDECLARATION_OR_BAD_SCOPE,root);
                if (n->valType == VOID)
                    throw_semantic_error(TYPE_MISMATCH,root);

                analyze_ast_tree((struct ast_node*)r->next);
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                if (didParams != 1)
                    table_enter_scope(table);
                int oldValue = didParams;
                didParams = 0;
                analyze_ast_tree((struct ast_node*)r->local_dec);
                analyze_ast_tree((struct ast_node*)r->stmt);
                didParams = oldValue;
                if (didParams == 1)
                    didParams = 0;
                else
                    table_exit_scope(table);
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                analyze_ast_tree((struct ast_node*)r->var_dec);
                analyze_ast_tree((struct ast_node*)r->next);
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                switch (r->expr_type) {
                    case EXPRST:
                        analyze_ast_tree((struct ast_node*)r->typed_stmt.expr_stmt);
                        break;
                    case CMP:
                        analyze_ast_tree((struct ast_node*)r->typed_stmt.cmp_stmt);
                        break;
                    case SEL:
                        analyze_ast_tree((struct ast_node*)r->typed_stmt.sel_stmt);
                        break;
                    case ITER:
                        analyze_ast_tree((struct ast_node*)r->typed_stmt.iter_stmt);
                        break;
                    case RET:
                        analyze_ast_tree((struct ast_node*)r->typed_stmt.ret_stmt);
                        break;
                    default:
                        throw_ast_error(INVALID_EXPR_TYPE,root);
                }
                analyze_ast_tree((struct ast_node*)r->next);
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                analyze_ast_tree((struct ast_node*)r->expr);
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                if (r->if_expr == NULL || r->if_stmt == NULL)
                    throw_ast_error(INCOMPLETE_SELE_NODE,root);

                if (get_type((struct ast_node*)r->if_expr) != INT)
                    throw_semantic_error(TYPE_MISMATCH,root);

                analyze_ast_tree((struct ast_node*)r->if_expr);
                analyze_ast_tree((struct ast_node*)r->if_stmt);
                analyze_ast_tree((struct ast_node*)r->else_stmt);
                break;
            }

        case ITER_STMT_NODE:
            {
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                if (r->while_expr == NULL || r->while_stmt == NULL)
                    throw_ast_error(INCOMPLETE_SELE_NODE,root);
                if (get_type((struct ast_node*)r->while_expr) != INT)
                    throw_semantic_error(TYPE_MISMATCH,root);


                analyze_ast_tree((struct ast_node*)r->while_expr);
                analyze_ast_tree((struct ast_node*)r->while_stmt);
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                struct sym_node* n = table_find(table,currFunc);
                if (n->valType == VOID && r->ret_expr != NULL)
                    throw_semantic_error(BAD_RETURN_TYPE,root);
                if(r->ret_expr == NULL && n->valType != VOID)
                    throw_semantic_error(BAD_RETURN_TYPE,root);
                else if (r->ret_expr != NULL) {
                    if (get_type((struct ast_node*)r->ret_expr) == VOID &&
                            n->valType == INT)
                    throw_semantic_error(BAD_RETURN_TYPE,root);
                }
                else if (r->ret_expr != NULL && n->valType != INT)
                    throw_semantic_error(BAD_RETURN_TYPE,root);
                analyze_ast_tree((struct ast_node*)r->ret_expr); // check that the expr is valid 
                foundReturn = 1;
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL) {
                    if (r->smp_expr != NULL)
                        throw_ast_error(INCOMPLETE_EXPR_NODE,root);
                    if (get_type((struct ast_node*)r->expr) != INT)
                        throw_semantic_error(TYPE_MISMATCH,root);
                }
                analyze_ast_tree((struct ast_node*)r->var);
                analyze_ast_tree((struct ast_node*)r->expr);
                analyze_ast_tree((struct ast_node*)r->smp_expr);
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                struct sym_node* n = table_find(table,r->id);
                if (n == NULL)
                    throw_semantic_error(USE_BEFORE_DEC,root);
                else if (n->idType != r->idType || n->valType != r->valType) {
                    throw_semantic_error(TYPE_MISMATCH,root);
                }
                if (r->array_expr != NULL ) {
                    if (get_type((struct ast_node*)r->array_expr) != INT)
                        throw_semantic_error(INVALID_ARR_ACCESS,root);
                    analyze_ast_tree((struct ast_node*)r->array_expr);
                }
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                if (r->left != NULL && r->right != NULL) { // a relop is in play
                    if (get_type((struct ast_node*)r->left) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                    if (get_type((struct ast_node*)r->right) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                }
                analyze_ast_tree((struct ast_node*)r->left);
                analyze_ast_tree((struct ast_node*)r->right);
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                if (r->expr != NULL && r->term != NULL) { // a addop is in play
                    if (get_type((struct ast_node*)r->expr) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                    if (get_type((struct ast_node*)r->term) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                }
                analyze_ast_tree((struct ast_node*)r->expr);
                analyze_ast_tree((struct ast_node*)r->term);
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                if (r->term != NULL && r->factor != NULL) { // a addop is in play
                    if (get_type((struct ast_node*)r->term) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                    if (get_type((struct ast_node*)r->factor) != INT) // if not an int
                        throw_semantic_error(TYPE_MISMATCH,root);
                }
                analyze_ast_tree((struct ast_node*)r->term);
                analyze_ast_tree((struct ast_node*)r->factor);
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        analyze_ast_tree((struct ast_node*)r->factor.expr);
                        break;
                    case VAR:
                        analyze_ast_tree((struct ast_node*)r->factor.var);
                        break;
                    case CALL:
                        analyze_ast_tree((struct ast_node*)r->factor.call);
                        break;
                    case NUMFACT: // nothing to do on a number
                        break;
                    default:
                        throw_ast_error(INVALID_FACTOR_TYPE,root);
                }
                break;
            }
        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                struct sym_node* func = table_find(table,r->id); // find this function
                if (strncmp("output",r->id,7) == 0 )
                    generateOutput = 1;
                else if (strncmp("input",r->id,6) == 0)
                    generateInput = 1;
                else if (func == NULL || func->idType != FUNCTION)
                    throw_semantic_error(USE_BEFORE_DEC,root);
                check_args(r->args,func->params);
                //analyze_ast_tree((struct ast_node*)r->args);
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                analyze_ast_tree((struct ast_node*)r->arg);
                analyze_ast_tree((struct ast_node*)r->nextArg);
                break;
            }
        default:
            throw_ast_error(IMPROPER_NODE_TYPE,root);
    }
}

//Free AST Function
//Cleanup function for the AST
void free_ast_tree(struct ast_node * root) {
    if (root == NULL) return;
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*) root;
                free_ast_tree((struct ast_node*)r->decList);
                break;
            }

        case DEC_LIST_NODE:
            {
                struct dec_list_node* r = (struct dec_list_node*)root;
                free_ast_tree((struct ast_node*)r->var);
                free_ast_tree((struct ast_node*)r->func);
                free_ast_tree((struct ast_node*)r->nextDeclaration);
                break;
            }

        case VAR_DEC_NODE:
            {
                struct var_dec_node* r = (struct var_dec_node*)root;
                if (r->id !=NULL) {
                    free(r->id);
                    r->id = NULL;
                }
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                if (r->id !=NULL) {
                    free(r->id);
                    r->id = NULL;
                }
                free_ast_tree((struct ast_node*)r->params);
                free_ast_tree((struct ast_node*)r->stmt);
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                free_ast_tree((struct ast_node*)r->next);
                if (r->id !=NULL) {
                    free(r->id);
                    r->id = NULL;
                }
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                free_ast_tree((struct ast_node*)r->local_dec);
                free_ast_tree((struct ast_node*)r->stmt);
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                free_ast_tree((struct ast_node*)r->var_dec);
                free_ast_tree((struct ast_node*)r->next);
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                free_ast_tree((struct ast_node*)r->next);
                switch (r->expr_type) {
                    case EXPRST:
                        free_ast_tree((struct ast_node*)r->typed_stmt.expr_stmt);
                        break;
                    case CMP:
                        free_ast_tree((struct ast_node*)r->typed_stmt.cmp_stmt);
                        break;
                    case SEL:
                        free_ast_tree((struct ast_node*)r->typed_stmt.sel_stmt);
                        break;
                    case ITER:
                        free_ast_tree((struct ast_node*)r->typed_stmt.iter_stmt);
                        break;
                    case RET:
                        free_ast_tree((struct ast_node*)r->typed_stmt.ret_stmt);
                        break;
                    default:
                        fprintf(stderr,"error freeing ast tree\n");
                        exit(1);
                }
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                if (r->expr != NULL) {
                    free_ast_tree((struct ast_node*)r->expr);
                    r->expr = NULL;
                }
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                free_ast_tree((struct ast_node*)r->if_expr);
                free_ast_tree((struct ast_node*)r->if_stmt);
                free_ast_tree((struct ast_node*)r->else_stmt);
                break;
            }

        case ITER_STMT_NODE:
            {
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                free_ast_tree((struct ast_node*)r->while_expr);
                free_ast_tree((struct ast_node*)r->while_stmt);
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                free_ast_tree((struct ast_node*)r->ret_expr);
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                free_ast_tree((struct ast_node*)r->var);
                free_ast_tree((struct ast_node*)r->expr);
                free_ast_tree((struct ast_node*)r->smp_expr);
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                if (r->id !=NULL) {
                    free(r->id);
                    r->id = NULL;
                }
                free_ast_tree((struct ast_node*)r->array_expr);
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                free_ast_tree((struct ast_node*)r->left);
                free_ast_tree((struct ast_node*)r->right);
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                free_ast_tree((struct ast_node*)r->expr);
                free_ast_tree((struct ast_node*)r->term);
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                free_ast_tree((struct ast_node*)r->term);
                free_ast_tree((struct ast_node*)r->factor);
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        free_ast_tree((struct ast_node*)r->factor.expr);
                        break;
                    case VAR:
                        free_ast_tree((struct ast_node*)r->factor.var);
                        break;
                    case CALL:
                        free_ast_tree((struct ast_node*)r->factor.call);
                        break;
                    case NUMFACT: // nothing to do on a number
                        break;
                    default:
                        fprintf(stderr,"error: malformed AST, improper factor_type in fact node\n");
                        exit(1);
                }
                break;
            }
        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                if (r->id !=NULL) {
                    free(r->id);
                    r->id = NULL;
                }
                free_ast_tree((struct ast_node*)r->args);
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                free_ast_tree((struct ast_node*)r->arg);
                free_ast_tree((struct ast_node*)r->nextArg);
                break;
            }
        default:
            fprintf(stderr,"error freeing ast, improper nodeType\n");
            exit(1);
    }
    free(root);
}
