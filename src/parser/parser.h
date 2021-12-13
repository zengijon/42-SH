#pragma once

#include <err.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <string.h>

#include "../exec/variable_expention.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../memory/hmalloc.h"
#include "../struct/grammar_struct.h"

struct list_next *build_list_next(struct lexer *lex);

struct list *build_list(struct lexer *lex);

struct and_or_next *build_and_or_next(struct lexer *lex);

struct and_or *build_and_or(struct lexer *lex);

struct pipeline *build_pipeline(struct lexer *lex);

struct command *build_command(struct lexer *lex);

struct simple_command *build_simple_command(struct lexer *lex);

struct shell_command *build_shell_command(struct lexer *lex);

struct funcdec *build_funcdec(struct lexer *lex);

struct redirection *build_redirection(struct lexer *lex);

struct prefix *build_prefix(struct lexer *lex);

struct element *build_element(struct lexer *lex);

struct compound_list *build_compound_list(struct lexer *lex);

struct rule_for *build_rule_for(struct lexer *lex); // for later

struct rule_while *build_rule_while(struct lexer *lex);

struct rule_until *build_rule_until(struct lexer *lex); // for later

struct rule_case *build_rule_case(struct lexer *lex); // for later

struct rule_if *build_rule_if(struct lexer *lex);

struct else_clause *build_else_clause(struct lexer *lex);

struct do_group *build_do_group(struct lexer *lex); // for later

struct case_clause *build_case_clause(struct lexer *lex); // for later

struct case_item *build_case_item(struct lexer *lex); // for later
