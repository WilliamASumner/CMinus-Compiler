#ifndef CODEGEN_H
#define CODEGEN_H
#include "ast.h"
void gencode_program(struct program_node* node);
void gencode_dec_list(struct dec_list_node* node);
void gencode_var_dec(struct var_dec_node* node);
void gencode_func_dec(struct func_dec_node* node);
void gencode_params(struct params_node* node);
void gencode_cmp_stmt(struct cmp_stmt_node* node);
void gencode_local_decs(struct local_decs_node* node);
void gencode_stmt_node(struct stmt_node* node);
void gencode_expr_stmt(struct expr_stmt_node* node);
void gencode_sel_stmt(struct sel_stmt_node* node);
void gencode_iter_stmt(struct iter_stmt_node* node);
void gencode_ret_stmt(struct ret_stmt_node* node);
void gencode_expr(struct expr_node* node);
void gencode_var(struct var_node* node);
void gencode_smp_expr(struct smp_expr_node* node);
void gencode_add_expr(struct add_expr_node* node);
void gencode_term(struct term_node* node);
void gencode_factor_node(struct factor_node* node);
void gencode_call(struct call_node* node);
void gencode_args(struct args_node* node);
#endif
