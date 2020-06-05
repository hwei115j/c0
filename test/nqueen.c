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

int getchar()
{
    return in();
}

int putchar(int ch)
{
    out(ch);
    return ch;
}

int out_num(int n)
{

    int max;
    int r;
    int f = 0;
    int count = 0;
    
    if(n == 0) {
        count++;
        putchar(48);
    }
    for(max = 10000; max; max = div(max, 10)) {
        r = div(n, max);
        n = n - (r * max);
        if(r > 0)
            f = 1;
        if(f) {
            putchar(r + 48);
            count++;
        }
    }
    return count;
}

int puts(char *str)
{
    int count = 0;
    for(int i = 0; str[i]; i++) {
        putchar(str[i]);
        count++;
    }

    return count;
}

int printf(char *fmt, ...)
{
    char *va_list = va_start(&fmt);
    int p = 0;
    int count;

    while(*fmt) {
        if(*fmt != '%') {
            putchar(*fmt);
            count++;
        } else if(*fmt == '\\') {
            fmt++;
            if(*fmt == '\\')
                putchar('\\');
            else if(*fmt == 'n')
                putchar('\n');
            else if(*fmt == 't')
                putchar('\t');
            count++;
        } else {
            fmt++;
            if(*fmt == '%')
                putchar('%');
            else if(*fmt == 'd')
                count = count + out_num(va_list[p++]);
            else if(*fmt == 's')
                count = count + puts(va_list[p++]);
            else if(*fmt == 'c') {
                count++;
                putchar(va_list[p++]);
            }
        } 
        fmt++;
    }

    return count;
}
/*
int conflict(int (*board)[8], int row, int col)
{
    for (int i = 0; i < row; i++) {
        if (board[i][col])
            return 1;
        int j = row - i;
        if (0 < col - j + 1 && board[i][col - j])
            return 1;
        if (col + j < 8 && board[i][col + j])
            return 1;
    }
    return 0;
}

int print_board(int (*board)[8])
{
    int j;
    for (int i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if(board[i][i])
                printf("Q ");
            else 
                printf(". ");
        }
        printf("\n");
    }
    printf("\n\n");
}
*/
int print_board(int a)
{
    return 0;
}
int conflict(int a, int b, int c)
{
    return 0;
}
int solve(int (*board)[8], int row)
{
    /*
    if (row == 8) {
        print_board(board);
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!conflict(board, row, i)) {
            board[row][i] = 1;
            //solve(board, row + 1);
            board[row][i] = 0;
        }
    }
    */
    return 0;
}

int main()
{
    /*
    int board[64];
    int i;
    for (i = 0; i < 64; i++)
        board[i] = 0;
    solve(board, 0);
    */
    return 0;
}
