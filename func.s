BP,     DEC 3999
SP,     DEC 3999
-1,     DEC -1
-4,     DEC -4
2,      DEC 2
1,      DEC 1
R0,     DEC 0
R1,     DEC 0

PUSH,   DEC 0       //將AC PUSH到mem[SP++]
        STA SP
        LDA SP
        ADD -1
        STA SP
        BUN PUSH

POP,    DEC 0       //將mem[--SP]讀到AC
        LDA SP
        INC
        STA SP
        LDA SP I
        BUN POP

CALL,   DEC 0       //呼叫AC儲存的function位址
        STA R0
        LDA CALL
        BSZ PUSH
        LDA BP
        BSZ PUSH
        LDA SP
        STA BP
        BUN R0

//  argc[n]
//  argc[...]
//  argc[0]                     -4
//  ret STA add <- 先不實做     (-4)
//  return add                  -3
//  'BP                         -2
//  'SP                         -1
//  new BP,SP ->  //NULL

RET,    DEC 0       //將return值存到AC，並返回上一函式
        STA R1
        LDA BP
        STA SP
        POP
        STA SP
        POP
        STA BP
        POP
        STA R0
        LDA R1
        BUN R0



//  int add(int a, int b)
//  {
//      return a+b;
//  }
//  int main()
//  {
//      return add(1, 2);
//  }

main,   LDA     2
        BSZ     PUSH
        LDA     1
        BSZ     PUSH
        LDA     add
        BSZ     CALL
        OUT
        BSZ     RET

add,    LDA     BP
        ADD     -4
        STA     R0
        LDA     R0      I
        PUSH
        LDA     R0
        ADD     -1
        POP
        ADD     R0      I
        BSA     RET
        LDA     main
        BSZ     CALL
        HLT
