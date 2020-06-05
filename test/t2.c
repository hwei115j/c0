int func(int (*a)[3])
{
    a[0];
   /* 
    for(int i = 0; i < 3; i++)
        out(a[i] + '0');
        */
    return 0;
}
int main()
{
    int a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    func(a);
    
}
