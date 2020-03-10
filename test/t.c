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
//    int num = fib(23);
    out_num(fib(10));
//    out_num(num);
}

