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
    if (root != NULL) {
        root->next = attachee;
    }
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

struct args_node* ast_link_args_node(struct args_node* root, struct args_node* attachee)
{
    root->nextArg = attachee;
    return attachee;
}

struct stmt_node* reverse_stmt_list(struct stmt_node* root) {
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

struct local_decs_node* reverse_local_dec_list(struct local_decs_node* root) {
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

void print_indent(int indent, FILE* outFile)
{
    int i = 0;
    while (i++ < indent*2)
       fprintf(outFile," ");
}

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
                char* t = r->type == INT ? "int" : "void";
                fprintf(outFile,"[var-declaration [%s] [%s]]\n",t,r->id->symbol);
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                char* t = r->type == INT ? "int" : "void";
                fprintf(outFile,"[fun-declaration\n");
                indent++;
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",t);
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",r->id->symbol);

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
                char* t = r->type == INT ? "int" : "void";
                fprintf(outFile,"[param [%s] [%s]",t,r->id->symbol);
                if (r->id->type == ARRAY)
                    fprintf(outFile," \\[\\]");
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
                if (r->var != NULL) {
                    if (r->expr != NULL)
                        print_indent(indent,outFile);
                    fprintf(outFile,"]\n");
                }
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                fprintf(outFile,"[");
                fprintf(outFile,"var [%s]",r->id->symbol);
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
                fprintf(outFile,"[call\n");
                indent++;
                print_indent(indent,outFile);
                fprintf(outFile,"[%s]\n",r->id->symbol);
                print_indent(indent,outFile);
                fprintf(outFile,"[args ");
                print_ast_tree((struct ast_node*)r->args,outFile);
                indent--;
                fprintf(outFile,"]\n");
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
            fprintf(stderr,"error printing ast: improper nodeType\n");
            exit(1);
    }
}

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
                        exit(1);
                }
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
