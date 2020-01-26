int func()
{
    int a = 1+2*3;
    return a*10;
}

int main()
{
    int i = 0;
    int sum = 0;

    if(i)
        i = 100;
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
