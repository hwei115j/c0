        LDA 10
        BSA PUSH
        BSA fib
        HLT
        ORG 1000
//fib(n) = fib(n-1) + fib(n-2)
//n = 0 | n = 1 return n 
fib,    DEC 0
        LDA fib
        BSA CALL
        LDA BP
        ADD N4
        STA R0
        LDA R0 I
        SZA
        BUN L1
        BSA RET
L1,     BSA PUSH
        ADD N1
        SZA
        BUN L2
        INC
        BSA RET
L2,     LDA BP
        STA R0
        LDA R0 I
        ADD N1
        BSA PUSH
        BSA fib
        BSA PUSH
        LDA BP
        STA R0
        LDA R0 I
        ADD N2
        BSA PUSH
        BSA fib
        BSA PUSH
        LDA SP
        ADD N3
        STA R0
        BSA POP
        ADD R0 I
        BSA RET


BP,     DEC 3999
SP,     DEC 3999
N1,     DEC -1
N2,     DEC -2
N4,     DEC -4
P2,     DEC 2
P1,     DEC 1
P10,    DEC 10
R0,     DEC 0
R1,     DEC 0
NUM,    DEC 48

OOO,    DEC 0
        LDA OOO
        BSA CALL
        LDA P10
        SZA
        BUN J1
        BUN J2
J1,     ADD N1
        STA P10
        ADD NUM
        OUT
        BSA OOO
J2,     BSA RET


PUSH,   DEC 0       //將AC PUSH到mem[SP++]
        STA SP I
        LDA SP
        ADD N1
        STA SP
        LDA SP I
        BUN PUSH I
POP,    DEC 0       //將mem[--SP]讀到AC
        LDA SP
        INC
        STA SP
        LDA SP I
        BUN POP I

CALL,   DEC 0       //呼叫AC儲存的function位址
        BSA PUSH
        LDA BP
        BSA PUSH
        LDA SP
        STA BP
        BSA PUSH
        BUN CALL I
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
        BSA POP
        STA SP
        BSA POP
        STA BP
        BSA POP
        STA R0
        LDA R1
        BUN R0 I
        END
