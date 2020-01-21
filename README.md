# C0

## 介紹：

c0是一個簡單的學習用C編譯器，輸入為單一C原始檔，輸出為[Mano\_machine](https://en.wikipedia.org/wiki/Mano_machine)組合語言
也可以輸出S-expressions形式之AST

大量參考成大jserv的[MazuCC](https://github.com/jserv/MazuCC)

## 用法：

```bash
    make            #輸出為AST樹形式(計畫刪除)
    make sexp       #輸出為S表達式形式之AST
    make asm        #輸出Mano_machine之組合語言
```
以上輸入皆為stdin，輸出為stdout
輸出s表達式AST為未格式化之形式，可以用[sexp](https://github.com/hwei115j/sexp)格式化

如果輸出組合語言，可以用[as](https://github.com/hwei115j/asm)組譯，並用[vm](https://github.com/hwei115j/vm)執行

## 目前進度：

- 完成部份前端，且能輸出S表達式AST
- 可以解析char、int、long、float、double等型態
- 可以解析if、while、for、return等關鍵字
- 可以宣告function並可以解析function call
- 目前表達式判斷僅可判斷是否為0
- func.s為轉換組合語言嘗試
- 有限的錯誤處理————在程式寫錯時能開心一點

20190122更新

- 完成部份後端，已經可以編譯成.s檔
- if、while、for、return、function call皆可處理
- out()函式可以輸出ASCII
- 目前僅可做+-運算

## 未來規劃

- 完成後端運算
- 解析所有運算符號（==、<、>、&&、||....）
- 支援函式參數
- 支援指標
- 後端ISA變更，預定使用RSIC-V(RV32I)
- 重新設計堆疊管理
- 重構
- 理論知識嚴重不足，可能要先啃幾本書

## demo：

### S表達式輸出
```bash
cd c0/test
make sexp
./c < in.c | ./style > out.c
```

```C
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
```

```lisp
(AST_FUNC ((int 2) func)
          (AST_DECL (= (int a) (+ 1 (* 2 3))))
          (AST_RETURN (* (int a) 10)))
(AST_FUNC ((int 2) main)
          (AST_DECL (= (int i) 0))
          (AST_DECL (= (float i1) 0))
          (AST_DECL (= (char i2) 0))
          (AST_DECL (= (double i3) 0))
          (AST_DECL (= (int sum) 0))
          (AST_IF (COND (int sum))
                  (THEN (AST_FUNCALL func))
                  (ELSE (+ (* 10 (AST_FUNCALL func)) 10)
                        (AST_FUNCALL func)))
          (AST_WHILE (COND (int i))
                     (BODY (= (int i) (+ (int i) 1))
                           (AST_FUNCALL func)))
          (AST_FOR (INIT (= (int i) 100))
                   (COND (int i))
                   (STEP (= (int i) (- (int i) 1)))
                   (BODY (= (int sum) (+ (int sum) (int i)))))
          (AST_RETURN (int sum))) 
```

### 組合語言輸出

```bash
cd c0/test
make asm
./c < t.c
```

```c
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
```

```asm
        BSA     main     
        BSA     CALL     
        HLT              
main,   DEC     0        
        BSA     GETF     
        BUN     main    I
        BSA     PUSH     
        BSA     PUSH     
        LDA     10       
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
        BUN     .R0      
.R1,    LDA     10       
        BSA     PUSH     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        CMA              
        INC              
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R0       
        BSA     POP      
        STA     R0      I
        BUN     .R2      
.R3,    LDA     42       
        BSA     PUSH     
        BSA     POP      
        STA     a        
        BSA     out      
        BSA     CALL     
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        LDA     1        
        BSA     PUSH     
        BSA     POP      
        CMA              
        INC              
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R0       
        BSA     POP      
        STA     R0      I
.R2,    LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .R3      
        LDA     10       
        BSA     PUSH     
        BSA     POP      
        STA     a        
        BSA     out      
        BSA     CALL     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     1        
        BSA     PUSH     
        BSA     POP      
        CMA              
        INC              
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
.R0,    LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .R1      
        BSA     RET      
        ORG     1000     
out,    DEC     0        
        BSA     GETF     
        BUN     out     I
        LDA     a        
        OUT              
        BSA     RET      
OSET,   DEC     0        
        ADD     BP       
        STA     R0       
        LDA     R0      I
        BUN     OSET    I
PUSH,   DEC     0        
        STA     SP      I
        LDA     SP       
        ADD     N1       
        STA     SP       
        BUN     PUSH    I
POP,    DEC     0        
        LDA     SP       
        INC              
        STA     SP       
        LDA     SP      I
        BUN     POP     I
RET,    DEC     0        
        STA     R1       
        LDA     BP       
        STA     SP       
        BSA     POP      
        STA     BP       
        BSA     POP      
        STA     R0       
        LDA     R1       
        BUN     R0      I
CALL,   DEC     0        
        STA     R0       
        LDA     CALL     
        BSA     PUSH     
        LDA     BP       
        BSA     PUSH     
        LDA     SP       
        STA     BP       
        BUN     R0      I
GETF,   DEC     0        
        LDA     GETF     
        INC              
        BUN     GETF    I
NUM,    DEC     48       
SP,     DEC     3999     
BP,     DEC     3999     
R0,     DEC     0        
R1,     DEC     0        
N1,     DEC     -1       
a,      DEC     100      
10,     DEC     10       
0,      DEC     0        
-1,     DEC     -1       
42,     DEC     42       
1,      DEC     1        
        END              

```
