#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lexer/lexer.h"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

int is_good(struct lexer *test, int len, enum token_type *res)
{
    for (int i = 0; i < len; ++i)
    {
        if (lexer_pop(test)->type != res[i])
            return 1;
    }
    return 0;
}

Test(just_word, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo coucou");
    struct lexer *test = lexer_new(input);
    enum token_type expected[] = {TOKEN_WORDS, TOKEN_WORDS, TOKEN_EOF};
    cr_assert_eq(is_good(test, 3, expected), 0);
    free(test);
}

Test(just_word_2, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo coucou toi la bas    ");
    struct lexer *test = lexer_new(input);
    enum token_type expected[] = {TOKEN_WORDS, TOKEN_WORDS, TOKEN_WORDS,
                                   TOKEN_WORDS, TOKEN_WORDS, TOKEN_EOF};
    cr_assert_eq(is_good(test, 6, expected), 0);
    free(test);
}

Test(ifed, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("if echo fi");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_IF);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_FI);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(ifed_2, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("if echo then yes else no fi");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_IF);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_THEN);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_ELSE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_FI);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_0, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\n");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_1, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\nClavier");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_2, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\nClavier\nDe\nMerde\n");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_2bis, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\n Clavier \n De \n Merde \n");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_3, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\n \n");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(new_lined_4, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("\n\n");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_NEWLINE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(quote_1, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo 'ceci est un test'");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    struct token *tmp = lexer_pop(test);
    cr_assert_eq(tmp->type, TOKEN_WORDS);
    cr_assert_str_eq(tmp->value, "ceci est un test");
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(quote_error, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo 'ceci est un test");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_ERROR);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(quote_multiple, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo ''''''");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(if_bait, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("ifons");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}

Test(peek_rec_0, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("echo coucou if then else");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_peek_rec(test, 1)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_peek_rec(test, 2)->type, TOKEN_IF);
    cr_assert_eq(lexer_peek_rec(test, 3)->type, TOKEN_THEN);
    cr_assert_eq(lexer_peek_rec(test, 4)->type, TOKEN_ELSE);
    cr_assert_eq(lexer_peek_rec(test, 5)->type, TOKEN_EOF);
    free(test);
}

Test(while_1, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("while chocolat");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WHILE);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}
Test(plz, exit_code, .init = redirect_all_stdout)
{
    const char *input = strdup("if echo fi; then echo then; fi; echo test");
    struct lexer *test = lexer_new(input);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_IF);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_PTCOMA);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_THEN);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_PTCOMA);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_FI);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_PTCOMA);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_WORDS);
    cr_assert_eq(lexer_pop(test)->type, TOKEN_EOF);
    free(test);
}
