int a = 100;
int main()
{
    int i;
    int j;
    for(i = 10; i; i = i-1) {
        for(j = 10-i; j; j = j - 1) {
            a = 42;
            out();
        }
        a = 10;
        out();
    }
}
