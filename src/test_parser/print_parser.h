//
// Created by clousty8 on 26/11/2021.
//

#ifndef INC_42SHEEEE_PRINT_PARSER_H
#define INC_42SHEEEE_PRINT_PARSER_H

#include "../struct/grammar_struct.h"

void print_list_next(struct list_next *l);

void print_list(struct list *l);

void print_and_or_next(struct and_or_next *a_o_n);

void print_and_or(struct and_or *a_o);

void print_pipeline(struct pipeline *p);

void print_command(struct command *c);

void print_simple_command(struct simple_command *s_c);

void print_shell_command(struct shell_command *s_c, int flag);

void print_funcdec(struct funcdec *f);

void print_redirection(struct redirection *r);

void print_prefix(struct prefix *p);

void print_element(struct element *p);

void print_compound_list(struct compound_list *c_l);

void print_rule_for(struct rule_for *r_f); // for later

void print_rule_while(struct rule_while *r_w); // for later

void print_rule_until(struct rule_until *r_u); // for later

void print_rule_case(struct rule_case *r_c); // for later

void print_rule_if(struct rule_if *r_i);

void print_else_clause(struct else_clause *e_c);

void print_do_group(struct do_group *d_g); // for later

void print_case_clause(struct case_clause *c_c); // for later

void print_case_item(struct case_item *c_i); // for later

#endif // INC_42SHEEEE_PRINT_PARSER_H
