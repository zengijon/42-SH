#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../exec_builtins/echo.h"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(simple, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo coucou");
    my_echo(test);
    cr_assert_stdout_eq_str(("coucou\n"));
    free(test);
}

Test(endnewline, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -n coucou");
    my_echo(test);
    cr_assert_stdout_eq_str(("coucou"));
    free(test);
}

Test(endnewlinee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -e cou\\ncou");
    my_echo(test);
    cr_assert_stdout_eq_str(("cou\ncou\n"));
    free(test);
}


Test(endnewlinereee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo cou\\ncou");
    my_echo(test);
    cr_assert_stdout_eq_str(("cou\\ncou\n"));
    free(test);
}

Test(endnewlineretgee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo cou\\nco\\nu");
    my_echo(test);
    cr_assert_stdout_eq_str(("cou\\nco\\nu\n"));
    free(test);
}

Test(endnineretgee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -ne cou\\nco\\nu");
    my_echo(test);
    cr_assert_stdout_eq_str(("cou\nco\nu"));
    free(test);
}

Test(endninereee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -nel cou\\nco\\nu");
    my_echo(test);
    cr_assert_stdout_eq_str(("-nel cou\\nco\\nu\n"));
    free(test);
}

Test(endnineireee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -e \\tech\\toooo");
    my_echo(test);
    cr_assert_stdout_eq_str(("\tech\toooo\n"));
    free(test);
}

Test(endniireee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -e -e \\tech\\toooo");
    my_echo(test);
    cr_assert_stdout_eq_str(("\tech\toooo\n"));
    free(test);
}

Test(endniigbreee, exit_code, .init = redirect_all_stdout)
{
    char *test = strdup("echo -e -a -e \\tech\\toooo");
    my_echo(test);
    cr_assert_stdout_eq_str(("-a -e \tech\toooo\n"));
    free(test);
}
