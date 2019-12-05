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
- 變數類型只有int一種
- for、if部份完成，判斷只有變數為零為false其餘皆為true
- 函式宣告和呼叫僅能解析回傳類型，不能解析傳入變數

## demo：

```bash
make sexp
./sexp < in.c | ./style > out.c
```

in.c
```C
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
```

out.c
```C
(AST_FUNC ((size 4) func)
          (AST_COMPOUND (AST_DECL (= ((size 4) a) (+ 1 (* 2 3))))
                        (AST_RETURN (* ((size 4) a) 10))))
(AST_FUNC ((size 4) main)
          (AST_COMPOUND (AST_DECL (= ((size 4) i) 0))
                        (AST_DECL (= ((size 4) sum) 0))
                        (AST_IF (COND ((size 4) sum))
                                (THEN (AST_COMPOUND (AST_FUNCALL func)))
                                (ELSE))
                        (AST_FOR (INIT (= ((size 4) i) 100))
                                 (COND ((size 4) i))
                                 (STEP (= ((size 4) i) (- ((size 4) i) 1)))
                                 (BODY (= ((size 4) sum) (+ ((size 4) sum) ((size 4) i)))))
                        (AST_RETURN ((size 4) sum)))) 
```

