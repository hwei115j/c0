/*
int out(int a)
{
    putchar(a);
}
*/
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

int out_str(char *str)
{
    for(int i = 0; str[i]; i++)
        out(str[i]);
    return 0;
}

int spa(int n)
{
    out_num(n);
    out(' ');
    out(' ');
    if(div(n, 10) == 0)
        out(' ');
    return 0;
}
int fib(int n)
{
    if(n == 0)
        return 0;
    if(n < 3)
        return 1;
    return fib(n - 1) + fib(n - 2);
}
int main()
{
    int a = 10;
    int *p = &a;

}
