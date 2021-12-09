#include <err.h>
#include <stddef.h>

#include "expression.h"

int eval_expr(struct my_expr *expr)
{
    if (expr != NULL)
    {
        if (expr->type == EXPR_NUMBER)
            return expr->data.value;
        int a = eval_expr(expr->data.children.left);
        if (expr->type == EXPR_NEGATION)
            return -a;
        int b = eval_expr(expr->data.children.right);
        switch (expr->type)
        {
        case EXPR_ADDITION:
            return a + b;
        case EXPR_SUBTRACTION:
            return a - b;
        case EXPR_MULTIPLICATION:
            return a * b;
        case EXPR_DIVISION:
            if (b == 0)
            {
                errx(1, "Division by zero not allowed!");
            }
            return a / b;
        default:
            break;
        }
    }
    return 0;
}