int func()
{
    int a = 1+2*3;
    return a*10;
}

int main()
{
    int i = 0;
    char i1 = 0;
    long i2 = 0;
    float i3 = 0;
    double i4 = 0;
    int sum = 0;

    if(i)
        int a = 10;
    if(sum) 
        func();
    else {
        10*func()+10;
        func();
    }
    while(i) {
        i = i + 1;
        func();
    }
    for(i = 100; i; i = i -1)
        sum = sum + i;
    return sum + 1;
}
