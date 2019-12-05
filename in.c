int func()
{
    int a = 1+2*3;
    return a*10;
}

int main()
{
    int i = 0;
    int sum = 0;
    if(sum) {
        func();
    }
    for(i = 100; i; i = i -1)
        sum = sum + i;
    return sum;
}
