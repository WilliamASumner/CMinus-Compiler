#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "ast.h"
#include "symtable.h"
#include "errors.h"

void gencode_program(struct program_node* prog) {
    gencode_dec_list(prog->decList);
}

void gencode_dec_list(struct dec_list_node* decList) {
    if (decList == NULL)
        return;
    gencode_var_dec(decList->var);
    gencode_func_dec(decList->func);
    gencode_dec_list(decList->nextDeclaration);
}

void gencode_var_dec(struct var_dec_node* var) {
}

void gencode_func_dec(struct func_dec_node* func) {
}

void gencode(struct ast_node* root, FILE* outFile) {
    if (root == NULL) return;
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*)root;
                gencode((struct ast_node*)r->decList,outFile);
                break;
            }

        case DEC_LIST_NODE:
            {
                struct dec_list_node* r = (struct dec_list_node*)root;
                gencode((struct ast_node*)r->var,outFile);
                gencode((struct ast_node*)r->func,outFile);
                gencode((struct ast_node*)r->nextDeclaration,outFile);
                break;
            }

        case VAR_DEC_NODE:
            {
                struct var_dec_node* r = (struct var_dec_node*)root;
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                if (strncmp(r->id,"output",7) == 0) {
                    //gencode(OUTPUT);
                }
                if (strncmp(r->id,"input",6) == 0) {
                    //gencode(INPUT);
                }
                if (r->params != NULL) {
                    gencode((struct ast_node*)r->params,outFile);
                }
                gencode((struct ast_node*)r->params,outFile);
                gencode((struct ast_node*)r->stmt,outFile);
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                fprintf(outFile,"[compound-stmt\n");
                gencode((struct ast_node*)r->local_dec,outFile);
                gencode((struct ast_node*)r->stmt,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                // no printing to be done here
                gencode((struct ast_node*)r->var_dec,outFile);
                gencode((struct ast_node*)r->next,outFile);
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                switch (r->expr_type) {
                    case EXPRST:
                        gencode((struct ast_node*)r->typed_stmt.expr_stmt,outFile);
                        break;
                    case CMP:
                        gencode((struct ast_node*)r->typed_stmt.cmp_stmt,outFile);
                        break;
                    case SEL:
                        gencode((struct ast_node*)r->typed_stmt.sel_stmt,outFile);
                        break;
                    case ITER:
                        gencode((struct ast_node*)r->typed_stmt.iter_stmt,outFile);
                        break;
                    case RET:
                        gencode((struct ast_node*)r->typed_stmt.ret_stmt,outFile);
                        break;
                    default:
                        fprintf(stderr,"error printing ast tree\n");
                        exit(1);
                }
                gencode((struct ast_node*)r->next,outFile);
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                if (r->expr == NULL) {
                    fprintf(outFile,"[;]\n");
                }
                else
                    gencode((struct ast_node*)r->expr,outFile);
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                fprintf(outFile,"[selection-stmt");
                gencode((struct ast_node*)r->if_expr,outFile);
                fprintf(outFile,"\n");
                gencode((struct ast_node*)r->if_stmt,outFile);
                if (r->else_stmt != NULL) {
                    fprintf(outFile,"\n");
                }

                gencode((struct ast_node*)r->else_stmt,outFile);
                fprintf(outFile,"\n");
                fprintf(outFile,"]\n");
                break;
            }

        case ITER_STMT_NODE:
            {
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                fprintf(outFile,"[iteration-stmt");
                gencode((struct ast_node*)r->while_expr,outFile);
                fprintf(outFile,"\n");
                gencode((struct ast_node*)r->while_stmt,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                fprintf(outFile,"[return-stmt");
                if (r->ret_expr != NULL)
                    fprintf(outFile," ");
                gencode((struct ast_node*)r->ret_expr,outFile);
                fprintf(outFile,"]\n");
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL) {
                    fprintf(outFile,"[= ");
                    gencode((struct ast_node*)r->var,outFile);
                    if (r->expr == NULL) {
                        fprintf(stderr,"error: malformed ast, missing expr in expr node");
                        exit(1);
                    }
                    fprintf(outFile," ");
                    gencode((struct ast_node*)r->expr,outFile);
                }
                else {
                    gencode((struct ast_node*)r->smp_expr,outFile);
                }
                if (r->var != NULL) {
                    fprintf(outFile,"]\n");
                }
                break;
            }

        case VAR_NODE:
            {
                struct var_node* r = (struct var_node*)root;
                fprintf(outFile,"[");
                fprintf(outFile,"var [%s]",r->id);
                gencode((struct ast_node*)r->array_expr,outFile);
                fprintf(outFile,"]");
                break;
            }

        case SMP_EXPR_NODE:
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                if (r->op != UNARYREL) {
                    fprintf(outFile,"\n");
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

                gencode((struct ast_node*)r->left,outFile);
                gencode((struct ast_node*)r->right,outFile);
                if (r->op != UNARYREL)
                    fprintf(outFile,"]");
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                if  (r->op != UNARYADD) {
                    fprintf(outFile,"\n");
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
                gencode((struct ast_node*)r->expr,outFile);
                gencode((struct ast_node*)r->term,outFile);
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
                gencode((struct ast_node*)r->term,outFile);
                gencode((struct ast_node*)r->factor,outFile);
                if (r->op != UNARYMULT)
                    fprintf(outFile,"]");
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        gencode((struct ast_node*)r->factor.expr,outFile);
                        break;
                    case VAR:
                        gencode((struct ast_node*)r->factor.var,outFile);
                        break;
                    case CALL:
                        gencode((struct ast_node*)r->factor.call,outFile);
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
                fprintf(outFile,"[call\n");
                fprintf(outFile,"[%s]\n",r->id);
                fprintf(outFile,"[args ");
                gencode((struct ast_node*)r->args,outFile);
                fprintf(outFile,"]");
                fprintf(outFile,"]");
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                gencode((struct ast_node*)r->arg,outFile);
                gencode((struct ast_node*)r->nextArg,outFile);
                break;
            }
        default:
            throw_ast_error(IMPROPER_NODE_TYPE,root);
    }
}
