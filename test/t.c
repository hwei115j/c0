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

int main()
{
    int a[5];
    int i = 0;
    for(i = 0; i < 5; i++)
        a[i] = 10 * (i + 1);
    int *b;
    b = a;
    out_num(*a);
    /*
    for(i = 0; i < 5; i++) {
        out_num(b[i]);
        out('\n');
    }
    */
}
