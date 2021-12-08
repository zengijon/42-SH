//
// Created by clousty8 on 08/12/2021.
//

char *my_itoa(int n, char *s)
{
    int k = 0;
    if (n < 0)
    {
        s[k++] = '-';
        n = -1 * n;
    }
    int i = 1;
    for (int j = n; j >= 10; j /= 10)
        i *= 10;
    for (; i > 0; i /= 10)
    {
        s[k++] = '0' + (n / i);
        n %= i;
    }
    s[k] = 0;
    return s;
}