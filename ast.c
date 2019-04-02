#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

struct program_node* ast_new_program_node(struct dec_list_node* decList)
{
    struct program_node* node = malloc(sizeof(struct program_node));
    node->nodeType = PROGRAM_NODE;
    node->decList = (struct dec_list_node*) decList;
    return node;
}

struct dec_list_node* ast_new_dec_list_node(struct var_dec_node* var, struct func_dec_node* func, struct dec_list_node* nextDeclaration)
{
    struct dec_list_node* node = malloc(sizeof(struct dec_list_node));
    node->nodeType = DEC_LIST_NODE;
    node->var = var;
    node->func = func;
    node->nextDeclaration = nextDeclaration;
    return node;
}

struct dec_list_node* ast_link_dec_list_node(struct dec_list_node* root, struct dec_list_node* attachee)
{
    printf("linking dec node ");
    if (root->func != NULL)
        printf("%s with ",root->func->id->symbol);
    else
        printf("%s with ",root->var->id->symbol);

    if (attachee->func != NULL)
        printf("%s ",attachee->func->id->symbol);
    else
        printf("%s ",attachee->var->id->symbol);

    root->nextDeclaration = attachee;
    return attachee;
}

struct var_dec_node* ast_new_var_dec_node(enum type_spec type, struct sym_node* id, int arraySize)
{
    struct var_dec_node* node = malloc(sizeof(struct var_dec_node));
    node->nodeType = VAR_DEC_NODE;
    node->type = type;
    node->id = id;
    node->arraySize = arraySize;
    return node;
}

struct func_dec_node* ast_new_func_dec_node(enum type_spec type, struct sym_node* id, struct params_node* params, struct cmp_stmt_node* stmt)
{
    struct func_dec_node* node = malloc(sizeof(struct func_dec_node));
    node->nodeType = FUNC_DEC_NODE;
    node->type = type;
    node->id = id;
    node->params = params;
    node->stmt = stmt;
    return node;
}

struct params_node* ast_new_params_node(enum type_spec type, struct sym_node* id, struct params_node* next)
{
    struct params_node* node = malloc(sizeof(struct params_node));
    node->nodeType = PARAMS_NODE;
    node->type = type;
    node->next = next;
    node->id = id;
    return node;
}

struct params_node* ast_link_params_node(struct params_node* root, struct params_node*attachee) {
    root->next = attachee;
    return attachee;
}

struct cmp_stmt_node* ast_new_cmp_stmt_node(struct local_decs_node* local_dec, struct stmt_node* stmt)
{
    struct cmp_stmt_node* node = malloc(sizeof(struct cmp_stmt_node));
    node->nodeType = CMP_STMT_NODE;
    node->local_dec = local_dec;
    node->stmt = stmt;
    return node;
}

struct local_decs_node* ast_new_local_decs_node(struct var_dec_node* var_dec, struct local_decs_node* next)
{
    struct local_decs_node* node = malloc(sizeof(struct local_decs_node));
    node->nodeType = LOCAL_DECS_NODE;
    node->var_dec = var_dec;
    node->next = next;
    return node;
}

struct stmt_node* ast_new_stmt_node(enum stmt_type type, struct stmt_node* next, union sub_stmt typed_stmt)
{
    struct stmt_node* node = malloc(sizeof(struct stmt_node));
    node->nodeType = STMT_NODE;
    node->expr_type = type;
    node->next = next;
    node->typed_stmt = typed_stmt;
    return node;
}

struct stmt_node* ast_link_stmt_node(struct stmt_node* root, struct stmt_node* attachee) {
    root->next = attachee;
    return root;
}

struct expr_stmt_node* ast_new_expr_stmt_node(struct expr_node* expr)
{
    struct expr_stmt_node* node = malloc(sizeof(struct expr_stmt_node));
    node->nodeType = EXPR_STMT_NODE;
    node->expr = expr;
    return node;
}

struct sel_stmt_node* ast_new_sel_stmt_node(struct expr_node* if_expr, struct stmt_node* if_stmt, struct stmt_node* else_stmt)
{
    struct sel_stmt_node* node = malloc(sizeof(struct sel_stmt_node));
    node->nodeType = SEL_STMT_NODE;
    node->if_expr = if_expr;
    node->if_stmt = if_stmt;
    node->else_stmt = else_stmt;
    return node;
}

struct iter_stmt_node* ast_new_iter_stmt_node(struct expr_node* while_expr, struct stmt_node* while_stmt)
{
    struct iter_stmt_node* node = malloc(sizeof(struct iter_stmt_node));
    node->nodeType = ITER_STMT_NODE;
    node->while_expr = while_expr;
    node->while_stmt = while_stmt;
    return node;
}

struct ret_stmt_node* ast_new_ret_stmt_node(struct expr_node* ret_expr)
{
    struct ret_stmt_node* node = malloc(sizeof(struct ret_stmt_node));
    node->nodeType = RET_STMT_NODE;
    node->ret_expr = ret_expr;
    return node;
}

struct expr_node* ast_new_expr_node(struct var_node* var, struct expr_node* expr, struct smp_expr_node* smp_expr)
{
    struct expr_node* node = malloc(sizeof(struct expr_node));
    node->nodeType = EXPR_NODE;
    node->var = var;
    node->expr = expr;
    node->smp_expr = smp_expr;
    return node;
}

struct var_node* ast_new_var_node(struct sym_node* id, struct expr_node* array_expr)
{
    struct var_node* node = malloc(sizeof(struct var_node));
    node->nodeType = VAR_NODE;
    node->id = id;
    node->array_expr = array_expr;
    return node;
}

struct smp_expr_node* ast_new_smp_expr_node(enum relop op, struct add_expr_node* left, struct add_expr_node* right)
{
    struct smp_expr_node* node = malloc(sizeof(struct smp_expr_node));
    node->nodeType = SMP_EXPR_NODE;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

struct add_expr_node* ast_new_add_expr_node(enum addop op, struct add_expr_node* expr, struct term_node* term)
{
    struct add_expr_node* node = malloc(sizeof(struct add_expr_node));
    node->nodeType = ADD_EXPR_NODE;
    node->op = op;
    node->expr = expr;
    node->term = term;
    return node;
}

struct term_node* ast_new_term_node(enum mulop op, struct term_node* term, struct factor_node* factor)
{
    struct term_node* node = malloc(sizeof(struct term_node));
    node->nodeType = TERM_NODE;
    node->op = op;
    node->term = term;
    node->factor = factor;
    return node;
}

struct factor_node* ast_new_factor_node(enum fact_type factor_type, union fact_union factor)
{
    struct factor_node* node = malloc(sizeof(struct factor_node));
    node->nodeType = FACT_NODE;
    node->factor_type = factor_type;
    node->factor = factor;
    return node;
}

struct call_node* ast_new_call_node(struct sym_node* id, struct args_node* args)
{
    struct call_node* node = malloc(sizeof(struct call_node));
    node->nodeType = CALL_NODE;
    node->id = id;
    node->args = args;
    return node;
}

struct args_node* ast_new_args_node(struct expr_node* arg, struct args_node* nextArg)
{
    struct args_node* node = malloc(sizeof(struct args_node));
    node->nodeType = ARGS_NODE;
    node->arg = arg;
    node->nextArg = nextArg;
    return node;
}

void print_ast_tree(struct ast_node* root){
    if (root == NULL) return;
    switch (root->nodeType){
        case DEC_LIST_NODE:
        case VAR_DEC_NODE:
        case LOCAL_DECS_NODE:
        case STMT_NODE:
        case EXPR_STMT_NODE:
            break;
        default:
            printf("[");
    }
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*)root;
                printf("program\n");
                print_ast_tree((struct ast_node*)r->decList);
                break;
            }

        case DEC_LIST_NODE:
            {
                struct dec_list_node* r = (struct dec_list_node*)root;
                print_ast_tree((struct ast_node*)r->var);
                print_ast_tree((struct ast_node*)r->func);
                print_ast_tree((struct ast_node*)r->nextDeclaration);
                break;
            }

        case VAR_DEC_NODE:
            {
                struct var_dec_node* r = (struct var_dec_node*)root;
                char* t = r->type == INT ? "int" : "void";
                printf("var-declaration [%s] [%s]",t,r->id->symbol);
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                char* t = r->type == INT ? "int" : "void";
                printf("fun-declaration\n[%s]\n[%s]\n[params]\n",t,r->id->symbol);
                print_ast_tree((struct ast_node*)r->params);
                print_ast_tree((struct ast_node*)r->stmt);
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                char* t = r->type == INT ? "int" : "void";
                printf("param [%s] [%s]",t,r->id->symbol);
                if (r->id->type == ARRAY)
                    printf("\\\[\\\[");
                print_ast_tree((struct ast_node*)r->next);
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                printf("compound-stmt\n");
                print_ast_tree((struct ast_node*)r->local_dec);
                print_ast_tree((struct ast_node*)r->stmt);
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                // no printing to be done here
                print_ast_tree((struct ast_node*)r->var_dec);
                print_ast_tree((struct ast_node*)r->next);
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                switch (r->expr_type) {
                    case EXPRST:
                        print_ast_tree((struct ast_node*)r->typed_stmt.expr_stmt);
                        break;
                    case CMP:
                        print_ast_tree((struct ast_node*)r->typed_stmt.cmp_stmt);
                        break;
                    case SEL:
                        print_ast_tree((struct ast_node*)r->typed_stmt.sel_stmt);
                        break;
                    case ITER:
                        print_ast_tree((struct ast_node*)r->typed_stmt.iter_stmt);
                        break;
                    case RET:
                        print_ast_tree((struct ast_node*)r->typed_stmt.ret_stmt);
                        break;
                    default:
                        fprintf(stderr,"error printing ast tree\n");
                        exit(1);
                }
                print_ast_tree((struct ast_node*)r->next);
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                if (r->expr == NULL)
                    printf("[;]");
                else
                    print_ast_tree((struct ast_node*)r->expr);
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                printf("selection-stmt\n");
                print_ast_tree((struct ast_node*)r->if_expr);
                print_ast_tree((struct ast_node*)r->if_stmt);
                print_ast_tree((struct ast_node*)r->else_stmt);
                break;
            }

        case ITER_STMT_NODE:
            {
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                printf("iteration-stmt\n");
                print_ast_tree((struct ast_node*)r->while_expr);
                print_ast_tree((struct ast_node*)r->while_stmt);
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                printf("return-stmt");
                print_ast_tree((struct ast_node*)r->ret_expr);
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL)
                    printf("= ");
                print_ast_tree((struct ast_node*)r->var);
                print_ast_tree((struct ast_node*)r->expr);
                print_ast_tree((struct ast_node*)r->smp_expr);
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                printf("var [%s]",r->id->symbol);
                print_ast_tree((struct ast_node*)r->array_expr);
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                char *t = "";
                switch(r->op) {
                    case LT:
                        t = "<";
                        break;
                    case GT:
                        t = ">";
                        break;
                    case GTE:
                        t = ">=";
                        break;
                    case LTE:
                        t = "<=";
                        break;
                    case EQ:
                        t = "==";
                        break;
                    case NEQ:
                        t = "!=";
                        break;
                }
                printf("%s",t);

                print_ast_tree((struct ast_node*)r->left);
                print_ast_tree((struct ast_node*)r->right);
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                char *t = "";
                switch (r->op) {
                    case PLUS:
                        t = "+";
                        break;
                    case MINUS:
                        t = "-";
                }
                printf("%s",t);

                print_ast_tree((struct ast_node*)r->expr);
                print_ast_tree((struct ast_node*)r->term);
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                char *t = "";
                switch (r->op) {
                    case MULT:
                        t = "*";
                        break;
                    case DIV:
                        t = "/";
                }
                printf("%s",t);
                print_ast_tree((struct ast_node*)r->term);
                print_ast_tree((struct ast_node*)r->factor);
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        print_ast_tree((struct ast_node*)r->factor.expr);
                        break;
                    case VAR:
                        print_ast_tree((struct ast_node*)r->factor.var);
                        break;
                    case CALL:
                        print_ast_tree((struct ast_node*)r->factor.call);
                        break;
                    case NUMFACT: // nothing to do on a number
                        printf("%d",r->factor.number);
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
                printf("call\n[%s]\n[args ",r->id->symbol);
                print_ast_tree((struct ast_node*)r->args);
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                print_ast_tree((struct ast_node*)r->arg);
                print_ast_tree((struct ast_node*)r->nextArg);
                break;
            }
        default:
            fprintf(stderr,"error printing ast, improper nodeType\n");
            exit(1);
    }
    switch (root->nodeType){ // nodes that don't get brackets
        case DEC_LIST_NODE:
        case VAR_DEC_NODE:
        case LOCAL_DECS_NODE:
        case STMT_NODE:
        case EXPR_STMT_NODE:
            break;
        default:
            printf("]");
    }
}

void free_ast_tree(struct ast_node * root) {
    if (root == NULL) return;
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*) root;
                free_ast_tree((struct ast_node*)r->decList);
                free(root);
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
                r->id = NULL; // don't clean up here, clear symbol table after
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                r->id = NULL; // don't cleanup here, clear sym table after
                free_ast_tree((struct ast_node*)r->params);
                free_ast_tree((struct ast_node*)r->stmt);
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                free_ast_tree((struct ast_node*)r->next);
                r->id = NULL;
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
                r->id = NULL;
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
                }
                        exit(1);
                break;
            }
        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                r->id = NULL; // lose the link to the id
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
