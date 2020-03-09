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
    int max = 0;
    int i;
    int j; 

    //1+2+3+....+100
    for(i = 0; i < 101; i++)
        max = max + i;
    out_num(max);

#if 0
    //星星
    for(i = 0; i < 10; i++) {
        for(j = 0; j < i; j++)
            out('*');
        out('\n');
    }
    //99乘法表
    int r;
    for(i = 1; i < 10; i++) {
        for(j = 1; j < 10; j++) {
            out_num(j);
            out('*');
            out_num(i);
            out('=');
            spa(i*j);
        }
        out('\n');
    }

    //費氏數列
    out('f');
    out('i');
    out('b');
    out('(');
    out_num(23);
    out(')');
    out('=');
    out_num(fib(23));
    out('\n');
    //out_num(3452);
#endif
}
