int func()
{
    int a = 1+2*3;
    return a*10;
}

int main()
{
    int i = 0;
    float i1 = 0;
    char i2 = 0;
    double i3 = 0;

    int sum = 0;
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
    return sum;
}
