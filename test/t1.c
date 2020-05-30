int main()
{
    struct t {
        struct t1{
            int x;
            int y;
        }*c;
        int a;
        int b;
    }t;
    t.c;
}
