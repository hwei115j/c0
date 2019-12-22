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

## 目前進度：

- 完成部份前端，且能輸出S表達式AST
- 可以解析char、int、long、float、double等型態
- 可以解析if、while、for、return等關鍵字
- 可以宣告function並可以解析function call
- 目前表達式判斷僅可判斷是否為0
- func.s為轉換組合語言嘗試
- 有限的錯誤處理————在程式寫錯時能開心一點

## demo：

```bash
make sexp
./sexp < in.c | ./style > out.c
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
