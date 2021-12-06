#pragma once
#include "../struct/grammar_struct.h"
#include "../struct/exec_struct.h"

int exec_list(struct list *l, struct exec_struct *ex_l);

int exec_list_next(struct list_next *l_n, struct exec_struct *ex_l);

int exec_and_or_next(struct and_or_next *a_o, int p_res, struct exec_struct *ex_l);

int exec_and_or(struct and_or *a_o, struct exec_struct *ex_l);

int exec_pipeline_next(struct pipeline_next *p, struct exec_struct *ex_l);

int exec_pipeline(struct pipeline *p, struct exec_struct *ex_l);

int exec_command(struct command *cmd, struct exec_struct *ex_l);

int exec_simple_command(struct simple_command *cmd, struct exec_struct *ex_l);

int exec_shell_command(struct shell_command *cmd, struct exec_struct *ex_l);

int exec_fundec(struct funcdec *cmd, struct exec_struct *ex_l);

int exec_redir(struct redirection *cmd, struct exec_struct *ex_l);

int exec_prefix(struct prefix *pre, struct exec_struct *ex_l);

int exec_element(struct element *elt, struct exec_struct *ex_l);

int exec_compound_list(struct compound_list *cp_list, struct exec_struct *ex_l);

int exec_rule_for(struct rule_for *r_f, struct exec_struct *ex_l);

int exec_rule_while(struct rule_while *r_w, struct exec_struct *ex_l);

int exec_rule_until(struct rule_until *r_u, struct exec_struct *ex_l);

int exec_rule_case(struct rule_case *r_c, struct exec_struct *ex_l);

int exec_rule_if(struct rule_if *r_i, struct exec_struct *ex_l);

int exec_else_clause(struct else_clause *e_c, struct exec_struct *ex_l);

int exec_do_group(struct do_group *do_gp, struct exec_struct *ex_l);

int exe_case_clause(struct case_clause *c_c, struct exec_struct *ex_l);

int exec_case_item(struct case_item *c_i, struct exec_struct *ex_l);

int assign_var(char *name, char *value, struct exec_struct *ex_l);
