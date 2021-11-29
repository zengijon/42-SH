#pragma once
#include "../struct/grammar_struct.h"

int exec_list(struct list *l);

int exec_list_next(struct list_next *l_n);

int exec_and_or_next(struct and_or_next *a_o, int p_res);

int exec_and_or(struct and_or *a_o);

int exec_pipeline_next(struct pipeline_next *p);

int exec_pipeline(struct pipeline *p);

int exec_command(struct command *cmd);

int exec_simple_command(struct simple_command *cmd);

int exec_shell_command(struct shell_command *cmd);

int exec_fundec(struct funcdec *cmd);

int exec_redir(struct redirection *cmd);

int exec_prefix(struct prefix *pre);

int exec_element(struct element *elt);

int exec_compound_list(struct compound_list *cp_list);

int exec_rule_for(struct rule_for *r_f);

int exec_rule_while(struct rule_while *r_w);

int exec_rule_until(struct rule_until *r_u);

int exec_rule_case(struct rule_case *r_c);

int exec_rule_if(struct rule_if *r_i);

int exec_else_clause(struct else_clause *e_c);

int exec_do_group(struct do_group *do_gp);

int exe_case_clause(struct case_clause *c_c);

int exec_case_item(struct case_item *c_i);