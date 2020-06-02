int div(int a, int b)
{
    int sum = 0;

    while(a > b) {
        sum++;
        a = a - b;
    }
    if(a == b)
        sum++;
    return sum;
}

int out_num(int n)
{

    int max;
    int r;
    int f = 0;

    if(n == 0)
        out(48);
    for(max = 10000; max; max = div(max, 10)) {
        r = div(n, max);
        n = n - (r * max);
        if(r > 0)
            f = 1;
        if(f)
            out(r + 48);
    }
    out('\n');
    return 0;
}

int printf(char *fmt, ...)
{
    char *list = va_start(&fmt);
    char *str = list[0];
    str;
    /*
    int i = 0;
    for(i = 0; str[i]; i++)
        out(str[i]);
        */
    return 0;
}
int main()
{
    printf("aaaa", "123456", '2', '3');
}

