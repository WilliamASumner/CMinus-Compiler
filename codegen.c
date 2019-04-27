#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "ast.h"
#include "symtable.h"
#include "errors.h"
#include "yacc_header.h"

void push_stack_machine(char* reg) {
#ifdef DEBUG
    fprintf(yyout,"\n#--- Stack Push: %s ---\n",reg);
    fprintf(yyout,"sw %s, 0($sp)\n",reg); // store the value at the stack
    fprintf(yyout,"addiu $sp, $sp, -4\n"); // update the stack
    fprintf(yyout,"#--- Stack Push: %s ---\n\n",reg);
#else
    fprintf(yyout,"sw %s, 0($sp)\n",reg); // store the value at the stack
    fprintf(yyout,"addiu $sp, $sp, -4\n"); // update the stack
#endif

}

void pop_stack_machine(char* reg) {
#ifdef DEBUG
    fprintf(yyout,"\n#--- Stack Pop: %s ---\n",reg);
    fprintf(yyout,"addiu $sp, $sp, 4\n"); // move stack pointer
    fprintf(yyout,"lw %s, 0($sp)\n",reg); // get data from stack
    fprintf(yyout,"#--- Stack Pop: %s ---\n\n",reg);
#else
    fprintf(yyout,"addiu $sp, $sp, 4\n"); // move stack pointer
    fprintf(yyout,"lw %s, 0($sp)\n",reg); // get data from stack
#endif
}

void gencode_output(void) { // void output(int x)
    push_stack_machine("$ra"); // push ra
    fprintf(yyout,"lw $a0, 4($fp)\n"); // load variable x
    fprintf(yyout,"li $v0, 1\n"); // load print int function
    fprintf(yyout,"syscall\n"); // print
    fprintf(yyout,"li $v0, 11\n"); // load \n
    fprintf(yyout,"li $a0, 0x0a\n");
    fprintf(yyout,"syscall\n"); // print newline
    fprintf(yyout,"li $a0, 0\n"); // return value of 0
    fprintf(yyout,"lw $ra 4($sp)\n"); // restore sp
    fprintf(yyout,"addiu $sp, $sp, 12\n"); // restore sp
    fprintf(yyout,"lw $fp 0($sp)\n"); // restore fp
    fprintf(yyout,"jr $ra\n");
}

void gencode_input(void) { // int input(void)
    push_stack_machine("$ra");
    fprintf(yyout,"li $v0, 5\n"); // load read int function
    fprintf(yyout,"syscall\n"); // wait for int
    fprintf(yyout,"move $a0, $v0\n"); // return value
    fprintf(yyout,"lw $ra 4($sp)\n"); // restore sp
    fprintf(yyout,"addiu $sp, $sp, 8\n"); // restore sp
    fprintf(yyout,"lw $fp 0($sp)\n"); // restore fp
    fprintf(yyout,"jr $ra\n");
}

void gencode_array_offset(struct expr_node* expr) {
    gencode((struct ast_node*)expr); // generate offset value
    fprintf(yyout,"li $t2, 4\n");
    fprintf(yyout,"mult $a0, $t2\n"); // turn into a multiple of 4
    fprintf(yyout,"mflo $a0\n"); // move result to $a0
}

int get_var_offset(struct sym_table* table, struct sym_node* node,int params) {
    int varNo = node->varNo;
    int varScope = node->scope;
    int scope = table->currScope;
    struct stack* varStack = copy_stack_rev(table->currVarNoStack); // start from top
    stack_pop(varStack); // no need for globals
    int direction = node->isParamVar ? 1 : -1;
    int offset = 0;
    if (node->isParamVar) {
#ifdef DEBUG
        printf("calculating offset of param %s: offset = (%d - %d)\n",node->symbol,params,varNo);
#endif
        offset = (params - varNo); // add on the offset at the current scope
    }
    else {
#ifdef DEBUG
        printf("scope of table is: %d and scope of var %s is %d\n",scope,node->symbol,varScope);
    print_stack(varStack);
#endif
        scope = 1;
        while (scope < varScope) { // while not at curr scope
            offset += stack_pop(varStack); // add all vars from block
            scope += 1;
        }
#ifdef DEBUG
        printf("calculating offset of var %s (%d): %d +=  %d --> ",node->symbol,varScope,offset,varNo+1);
#endif
        offset += varNo+1; // add on the offset at the current scope
#ifdef DEBUG
        printf("%d\n",offset);
#endif
    }
    free_stack(varStack); // cleanup
    return offset*4*direction; // 4 for alignment
}

int get_total_var_offset(struct sym_table* table, struct sym_node* node) {
    struct stack* varStack = copy_stack(table->currVarNoStack);
    int scope = table->currScope;
    int val = 0;
    while (scope > 0) { // global
        val += stack_pop(varStack);
        scope -= 1;
    }
    free_stack(varStack); // cleanup
    return val*4; // aligned total offset
}

int count_params(struct params_node* p) {
    int n = 0;
    while (p != NULL) {
        n++;
        p = p->next;
    }
    return n;
}



int cleanup_func_offset(struct sym_table* table, struct sym_node* funcNode,int params) {
    int n = 8; // 4 for $fp and 4 for old $ra
    n += get_total_var_offset(table,funcNode);
    n += (4*params);
    return n; // TODO FILL IN
}

void gencode_cleanup_scope() {
    if (stack_peek(table->currVarNoStack) != 0) {
#ifdef DEBUG
        fprintf(yyout,"addiu $sp, $sp, %d # cleaning up scope\n",4*stack_peek(table->currVarNoStack));
#else
        fprintf(yyout,"addiu $sp, $sp, %d\n",4*stack_peek(table->currVarNoStack));
#endif
    }
}

void gencode(struct ast_node* root) {
    if (root == NULL) return;
    static char* currFunc = NULL;
    static int doneWithData = 0;
    static int keepScopeForParams = 0;
    static int conditionalNumber = 1;
    static int loopNum = 0;
    static int params = -1;
    switch(root->nodeType) {
        case PROGRAM_NODE:
            {
                struct program_node* r = (struct program_node*) root;
                fprintf(yyout,".data\n"); // start the data section
                gencode((struct ast_node*)r->decList);
                fprintf(yyout,"main: \n");
                fprintf(yyout,"jal _f_main\n");
                fprintf(yyout,"li $v0, 10\n");
                fprintf(yyout,"syscall\n");
                break;
            }

        case DEC_LIST_NODE:
            {
                struct dec_list_node* r = (struct dec_list_node*)root;
                if (r->var != NULL)
                    gencode((struct ast_node*)r->var);
                else
                    gencode((struct ast_node*)r->func);
                gencode((struct ast_node*)r->nextDeclaration); //gen next dec
                break;
            }

        case VAR_DEC_NODE:
            {
                struct var_dec_node* r = (struct var_dec_node*)root;
                //struct sym_node* newSym = table_add(table,r->id,r->idType,r->valType,NULL,0);
                int varSize = r->arraySize;
                printf("variable %s of size: %d\n",r->id,varSize);

                table_add(table,r->id,r->idType,r->valType,NULL,0,varSize);
                if (table->currScope == 0) { // global variable declaration
                    fprintf(yyout, "_g_%s: .word 0",r->id);
                    int i;
                    for (i = 1; i < r->arraySize; i++)
                        fprintf(yyout,", 0");
                    fprintf(yyout,"\n");
                }
                else {
                    fprintf(yyout,"addiu $sp, $sp, -%d\n",r->arraySize*4); // add stack space
                }
                break;
            }

        case FUNC_DEC_NODE:
            {
                struct func_dec_node* r = (struct func_dec_node*)root;
                if (!doneWithData) { // this is the first function
                    doneWithData = 1;
#if DEBUG
                    fprintf(yyout,"\n.text\nj main\n"); // print text section header
#else
                    fprintf(yyout,"\n.text\n"); // print text section header
#endif
                }

                if (strncmp(r->id,"output",7) == 0) {
                    if (!generateOutput)
                        break; // don't do anything if ouput not used
                    fprintf(yyout,"\n_f_%s:\nmove $fp $sp\n",r->id); // start
                    gencode_output();

                    break;
                } else if (strncmp(r->id,"input",6) == 0 ) {
                    if (!generateInput)
                        break; // don't do anything if input not used
                    fprintf(yyout,"\n_f_%s:\nmove $fp $sp\n",r->id); // start writing function
                    gencode_input();
                    break;
                }
                fprintf(yyout,"\n_f_%s:\nmove $fp $sp\n",r->id); // label
                push_stack_machine("$ra"); // save ret addr

                struct sym_node* funcNode = table_add(table,r->id,r->idType,r->valType,r->params,0,-1);
                currFunc = r->id;
                keepScopeForParams = 1;
                table_enter_scope(table);
                gencode((struct ast_node*)r->params);
                params = count_params(r->params);
                printf("params of func: %s are %d\n",currFunc,params); // REM
                table_end_param_decs(table); // prepare for local decs
                gencode((struct ast_node*)r->stmt);
                currFunc = NULL;

                // cleanup
                fprintf(yyout,"\n_f_%s_exit:\n",r->id); // label
 
                gencode_cleanup_scope(); // generates the needed cleanup
#ifdef DEBUG
                fprintf(yyout,"#^^^cleanup of scope: %d\n",table->currScope);
#endif
                table_exit_scope(table); // update table
                fprintf(yyout,"lw $ra, %d($sp)\n",4);
                fprintf(yyout,"addiu $sp, $sp, %d\n",cleanup_func_offset(table,funcNode,params)); // AND THIS
                fprintf(yyout,"lw $fp 0($sp)\n");
                fprintf(yyout,"jr $ra\n");
                fprintf(yyout,"\n\n");
                params = -1;
                break;
            }

        case PARAMS_NODE:
            {
                struct params_node* r = (struct params_node*)root;
                gencode((struct ast_node*)r->next); // generate last params node 1st
                //struct sym_node* n = table_add(table,r->id,r->idType,r->valType,NULL,1); // add in positions of args
                table_add(table,r->id,r->idType,r->valType,NULL,1,1); // add in positions of args
                break;
            }

        case CMP_STMT_NODE:
            {
                struct cmp_stmt_node* r = (struct cmp_stmt_node*)root;
                if (keepScopeForParams != 1) // check if already added function parameters in this scope
                    table_enter_scope(table);
                int oldValue = keepScopeForParams;
                keepScopeForParams = 0; // disable for future compound statements

                gencode((struct ast_node*)r->local_dec);
                gencode((struct ast_node*)r->stmt);

                if (table->currScope > 1) // if higher than curr func scope
                    gencode_cleanup_scope(); // cleanup the scope

                keepScopeForParams = oldValue;
                if (keepScopeForParams == 1)
                    keepScopeForParams = 0;
                else
                    table_exit_scope(table);
                break;
            }

        case LOCAL_DECS_NODE:
            {
                struct local_decs_node* r = (struct local_decs_node*)root;
                gencode((struct ast_node*)r->var_dec);
                gencode((struct ast_node*)r->next); // generate last variable declaration first
                break;
            }

        case STMT_NODE:
            {
                struct stmt_node* r = (struct stmt_node*)root;
                switch (r->expr_type) {
                    case EXPRST:
                        gencode((struct ast_node*)r->typed_stmt.expr_stmt);
                        break;
                    case CMP:
                        gencode((struct ast_node*)r->typed_stmt.cmp_stmt);
                        break;
                    case SEL:
                        gencode((struct ast_node*)r->typed_stmt.sel_stmt);
                        break;
                    case ITER:
                        gencode((struct ast_node*)r->typed_stmt.iter_stmt);
                        break;
                    case RET:
                        gencode((struct ast_node*)r->typed_stmt.ret_stmt);
                        break;
                    default:
                        throw_ast_error(INVALID_EXPR_TYPE,root);
                }
                gencode((struct ast_node*)r->next);
                break;
            }

        case EXPR_STMT_NODE:
            {
                struct expr_stmt_node* r = (struct expr_stmt_node*)root;
                gencode((struct ast_node*)r->expr);
                break;
            }

        case SEL_STMT_NODE:
            {
                struct sel_stmt_node* r = (struct sel_stmt_node*)root;
                int thisConditionalNumber = conditionalNumber++; // save for this specific if, but increase for the next if

                gencode((struct ast_node*)r->if_expr);
                fprintf(yyout,"bne $a0, 0, _if_branch%d\n",thisConditionalNumber); // complete relative operator

                fprintf(yyout,"_else_branch%d:\n",thisConditionalNumber); // add header for else
                gencode((struct ast_node*)r->else_stmt);
                fprintf(yyout,"j _end_if%d\n",thisConditionalNumber);

                fprintf(yyout,"_if_branch%d:\n",thisConditionalNumber); // true branch
                gencode((struct ast_node*)r->if_stmt);
                fprintf(yyout,"_end_if%d:\n",thisConditionalNumber); // end if
                break;
            }

        case ITER_STMT_NODE:
            {
                int thisLoopNum = loopNum;
                struct iter_stmt_node* r = (struct iter_stmt_node*)root;
                fprintf(yyout,"_loop%d:\n",loopNum++);
                gencode((struct ast_node*)r->while_expr);
                fprintf(yyout,"beq $a0, 0, _end_loop%d\n",thisLoopNum);
                gencode((struct ast_node*)r->while_stmt);
                fprintf(yyout,"j _loop%d\n",thisLoopNum);
                fprintf(yyout,"_end_loop%d:\n",thisLoopNum);
                break;
            }

        case RET_STMT_NODE:
            {
                struct ret_stmt_node* r = (struct ret_stmt_node*)root;
                //struct sym_node* n = table_find(table,currFunc);
                gencode((struct ast_node*)r->ret_expr); // check that the expr is valid
                if (table->currScope > 1) // if higher than curr func scope
                    gencode_cleanup_scope();
                fprintf(yyout,"j _f_%s_exit\n",currFunc); // jump to end of function
                break;
            }

        case EXPR_NODE:
            {
                struct expr_node* r = (struct expr_node*)root;
                if (r->var != NULL) { // assignment: var = expr
                    struct sym_node* n = table_find(table,r->var->id);
                    int varOffset = get_var_offset(table,n,params);

                    gencode((struct ast_node*)r->expr); // compute expr val
                    if (n->scope == 0) { // if global
                        if (n->idType == VAR) { // just store variable
                            fprintf(yyout,"sw $a0, _g_%s\n",n->symbol);
                         } else { // load global addr, add offset then store
                            push_stack_machine("$a0"); // save result of expr
                            gencode_array_offset(r->var->array_expr); // get offset
                            fprintf(yyout,"la $t1, _g_%s\n",n->symbol); // load var
                            fprintf(yyout,"add $t1, $a0, $t1\n"); // add expr
                            pop_stack_machine("$a0"); // pop to $a0
                            fprintf(yyout,"sw $a0, 0($t1)\n");
                        }

                    }
                    else if (n->idType == VAR) { // local/parameter variable
                        fprintf(yyout,"sw $a0, %d($fp)\n",varOffset); // save $a0
                    }
                    else if (n->isParamVar) { // parameter array
                        push_stack_machine("$a0"); // save expr
                        gencode_array_offset(r->var->array_expr); // get offset
                        fprintf(yyout,"lw $t1, %d($fp)\n",varOffset); // get addr
                        fprintf(yyout,"add $t1, $t1, $a0\n"); // add offset
                        pop_stack_machine("$a0"); // restore expr
                        fprintf(yyout,"sw $a0, 0($t1)\n"); // save result at addr

                    }
                    else { // local array
                        push_stack_machine("$a0");
                        gencode_array_offset(r->var->array_expr); // offset in $a0
                        fprintf(yyout,"add $t1, $fp, %d\n",varOffset); // load var
                        fprintf(yyout,"add $t1, $t1, $a0\n"); // inc addr
                        pop_stack_machine("$a0"); // pop to $a0
                        fprintf(yyout,"sw $a0, 0($t1)\n"); // save result at addr
                    }
                } else { // a simple expression, e.g. 3+4 == 1*9
                    gencode((struct ast_node*)r->smp_expr);
                }
                break;
            }

        case VAR_NODE:
            { // now a usage of value, see above for assignment
                struct var_node* r = (struct var_node*)root;
                struct sym_node* n = table_find(table,r->id);
#ifdef DEBUG
                fprintf(yyout,"\n#--- var: %s varNo: %d---\n",r->id,n->varNo);
#endif
                int varOffset = get_var_offset(table,n,params);
                if (n->scope == 0) { // if global
                    if (r->idType == VAR) {
                        fprintf(yyout,"lw $a0, _g_%s\n",r->id); // load global val
                    } else {
                        gencode_array_offset(r->array_expr);
                        fprintf(yyout,"la $t1, _g_%s\n",n->symbol); // load var
                        fprintf(yyout,"add $t1, $a0, $t1\n"); // add expr
                        fprintf(yyout,"lw $a0, 0($t1)\n"); // read the value from the var
                    }
                }
                else if (r->array_expr != NULL) { // array access
                    gencode_array_offset(r->array_expr); // arrayexpr -> $a0
                    if (n->isParamVar) { // param array
                        fprintf(yyout,"lw $t1, %d($fp)\n",varOffset); // load addr
                        fprintf(yyout,"add $a0, $a0, $t1\n"); // add offset to it
                        fprintf(yyout,"lw $a0, 0($a0)\n"); // load from addr
                    } else { // local array
                        fprintf(yyout,"add $t1, $fp, %d\n",varOffset);
                        fprintf(yyout,"add $a0, $a0, $t1\n"); // add var
                        fprintf(yyout,"lw $a0, 0($a0)\n"); // load from addr
                    }
                } else if (r->idType == ARRAY) { // regular array name (for calls)
                    fprintf(yyout,"la $a0, %d($fp)\n",varOffset); // load address 
                } else { // var access
                    fprintf(yyout,"lw $a0, %d($fp)\n",varOffset);
                }

#ifdef DEBUG
                fprintf(yyout,"#--- var: %s---\n\n",r->id);
#endif
                break;
            }

        case SMP_EXPR_NODE: // comparison statement
            {
                struct smp_expr_node* r = (struct smp_expr_node*)root;
                gencode((struct ast_node*)r->left); // generate val of left
                if (r->right == NULL)
                    break; // just return the left value
                push_stack_machine("$a0"); // save acc
                gencode((struct ast_node*)r->right);
                pop_stack_machine("$t1"); // pop into $t1
                // using the set if less than
                switch (r->op) {
                    case LT:
                        fprintf(yyout,"slt $a0, $t1, $a0\n"); // left < right
                        break;
                    case GT:
                        fprintf(yyout,"slt $a0, $a0, $t1\n"); // left > right
                        break;

                    case GTE:
                        fprintf(yyout,"slt $a0, $t1, $a0\n"); // left >= right
                        fprintf(yyout,"xori $a0, $a0, 1\n"); // not op
                        break;
                    case LTE:
                        fprintf(yyout,"slt $a0, $a0, $t1\n"); // left <= right
                        fprintf(yyout,"xori $a0, $a0, 1\n"); // not op
                        break;

                    case NEQ:
                        fprintf(yyout,"slt $t2, $a0, $t1\n"); // left > right
                        fprintf(yyout,"slt $t3, $t1, $a0\n"); // left < right
                        fprintf(yyout,"xor $a0, $t2, $t3\n"); // if either one is 1, 0, else 1
                        break;
                    case EQ:
                        fprintf(yyout,"slt $t2, $a0, $t1\n"); // left > right
                        fprintf(yyout,"slt $t3, $t1, $a0\n"); // left < right
                        fprintf(yyout,"xor $a0, $t2, $t3\n"); // if either one is 1, 0, else 1
                        fprintf(yyout,"xori $a0, $a0, 1\n"); // not op
                        break;
                    default: // do nothing
                        break;
                }
                break;
            }

        case ADD_EXPR_NODE:
            {
                struct add_expr_node* r = (struct add_expr_node*)root;
                if (r->expr == NULL) {
                    gencode((struct ast_node*)r->term); // just do term
                } else {
                    gencode((struct ast_node*)r->expr); // $t1
                    push_stack_machine("$a0"); // save result
                    gencode((struct ast_node*)r->term); // $a0
                    pop_stack_machine("$t1");
                    switch (r->op) {
                        case PLUS:
                            fprintf(yyout,"add $a0, $t1, $a0\n");
                            break;
                        case MINUS:
                            fprintf(yyout,"sub $a0, $t1, $a0\n");
                            break;
                        default:
                            break;
                    }
                }
                break;
            }

        case TERM_NODE:
            {
                struct term_node* r = (struct term_node*)root;
                if (r->term == NULL) {
                    gencode((struct ast_node*)r->factor); // just do factor
                } else {
                    gencode((struct ast_node*)r->term); // $t1
                    push_stack_machine("$a0"); // save result
                    gencode((struct ast_node*)r->factor); // $a0
                    pop_stack_machine("$t1");
                    switch (r->op) {
                        case MULT:
                            fprintf(yyout,"mul $a0, $t1, $a0\n");
                            break;
                        case DIV:
                            fprintf(yyout,"div $a0, $t1, $a0\n");
                            break;
                        default:
                            break;
                    }
                    fprintf(yyout,"mflo $a0\n"); // move result
                }
                break;
            }

        case FACT_NODE:
            {
                struct factor_node* r = (struct factor_node*)root;
                switch (r->factor_type) {
                    case EXPR:
                        gencode((struct ast_node*)r->factor.expr);
                        break;
                    case VAR:
                        gencode((struct ast_node*)r->factor.var);
                        break;
                    case CALL:
                        gencode((struct ast_node*)r->factor.call);
                        break;
                    case NUMFACT:
                        fprintf(yyout,"li $a0 %d\n",r->factor.number);
                        break;
                }
                break;
            }
        case CALL_NODE:
            {
                struct call_node* r = (struct call_node*)root;
                //struct sym_node* func = table_find(table,r->id); // find this function
                push_stack_machine("$fp"); // save frame pointer
                gencode((struct ast_node*)r->args); // add on the args
                fprintf(yyout,"jal _f_%s\n",r->id); // jump to function
                break;
            }

        case ARGS_NODE:
            {
                struct args_node* r = (struct args_node*)root;
                gencode((struct ast_node*)r->nextArg);
                gencode((struct ast_node*)r->arg);
                push_stack_machine("$a0"); // push on the arg
                break;
            }
        default:
            break;
    }
}
