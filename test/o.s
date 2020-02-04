        BSA     main     
        BSA     CALL     
        HLT              
div,    DEC     0        
        BSA     GETF     
        BUN     div     I
        CLA              
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R1       
        LDA     3        
        BSA     OSET     
        STA     R1      I
        CLA              
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R1       
        LDA     4        
        BSA     OSET     
        STA     R1      I
        LDA     0        
        BSA     PUSH     
        BUN     .L0      
.L1,    LDA     BP       
        ADD     -2       
        STA     R0       
        LDA     R0      I
        BSA     PUSH     
        LDA     R0      I
        INC              
        STA     R0      I
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     -1       
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
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
.L0,    LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        CMA              
        INC              
        STA     R1       
        BSA     POP      
        ADD     R1       
        CLE              
        SPA              
        CME              
        CME              
        CLA              
        CIL              
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L1      
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        CMA              
        INC              
        ADD     R1       
        CLE              
        SZA              
        CME              
        CME              
        CLA              
        CIL              
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L2      
        BUN     .L3      
.L2,    LDA     BP       
        ADD     -2       
        STA     R0       
        LDA     R0      I
        BSA     PUSH     
        LDA     R0      I
        INC              
        STA     R0      I
.L3,    LDA     -2       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        BSA     RET      
        CMA              
        BSA     RET      
out_num,DEC     0        
        BSA     GETF     
        BUN     out_num I
        CLA              
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R1       
        LDA     3        
        BSA     OSET     
        STA     R1      I
        CLA              
        BSA     PUSH     
        CLA              
        BSA     PUSH     
        LDA     0        
        BSA     PUSH     
        LDA     10       
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R0       
        BSA     POP      
        STA     R0      I
        BUN     .L4      
.L5,    LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     div      
        BSA     CALL     
        STA     R1       
        LDA     SP       
        ADD     2        
        STA     SP       
        LDA     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     -2       
        STA     R0       
        BSA     POP      
        STA     R0      I
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     -2       
        BSA     OSET     
        BSA     PUSH     
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        STA     .R2      
        BSA     POP      
        STA     .R3      
        CLA              
        STA     R1       
        LDA     -8       
        STA     .R4      
        BSA     .MUL     
        LDA     R1       
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
        LDA     -2       
        BSA     OSET     
        BSA     PUSH     
        LDA     0        
        BSA     PUSH     
        BSA     POP      
        CMA              
        INC              
        STA     R1       
        BSA     POP      
        ADD     R1       
        CLE              
        SPA              
        CME              
        CME              
        CLA              
        CIL              
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L6      
        BUN     .L7      
.L6,    LDA     1        
        BSA     PUSH     
        LDA     BP       
        ADD     -3       
        STA     R0       
        BSA     POP      
        STA     R0      I
.L7,    LDA     -3       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L8      
        BUN     .L9      
.L8,    LDA     -2       
        BSA     OSET     
        BSA     PUSH     
        LDA     48       
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        BSA     out      
        BSA     CALL     
        STA     R1       
        LDA     SP       
        ADD     1        
        STA     SP       
        LDA     R1       
        BSA     PUSH     
.L9,    LDA     10       
        BSA     PUSH     
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     div      
        BSA     CALL     
        STA     R1       
        LDA     SP       
        ADD     2        
        STA     SP       
        LDA     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R0       
        BSA     POP      
        STA     R0      I
.L4,    LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L5      
        LDA     0        
        BSA     PUSH     
        BSA     POP      
        BSA     RET      
        CMA              
        BSA     RET      
main,   DEC     0        
        BSA     GETF     
        BUN     main    I
        LDA     13       
        BSA     PUSH     
        BSA     out_num  
        BSA     CALL     
        STA     R1       
        LDA     SP       
        ADD     1        
        STA     SP       
        LDA     R1       
        BSA     PUSH     
        CMA              
        BSA     RET      
        ORG     1000     
MCIR,   DEC     0        
        BUN     ._L0     
._L1,   LDA     .R2      
        CIR              
        STA     .R2      
._L0,   ISZ     .R3      
        BUN     ._L1     
        BUN     MCIR    I
MCIL,   DEC     0        
        BUN     ._L2     
._L3,   LDA     .R2      
        CIL              
        STA     .R2      
._L2,   ISZ     .R3      
        BUN     ._L3     
        BUN     MCIL    I
out,    DEC     0        
        BSA     GETF     
        BUN     out     I
        LDA     3        
        BSA     OSET     
        OUT              
        BSA     RET      
in,     DEC     0        
        BSA     GETF     
        BUN     in      I
        INP              
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
.MUL,   DEC     0        
.LOP,   CLE              
        LDA     .R3      
        CIR              
        STA     .R3      
        SZE              
        BUN     .ONE     
        BUN     .ZRO     
.ONE,   LDA     .R2      
        ADD     R1       
        STA     R1       
        CLE              
.ZRO,   LDA     .R2      
        CIL              
        STA     .R2      
        ISZ     .R4      
        BUN     .LOP     
        BUN     .MUL    I
NUM,    DEC     48       
SP,     DEC     3999     
BP,     DEC     3999     
R0,     DEC     0        
R1,     DEC     0        
.R2,    DEC     0        
.R3,    DEC     0        
.R4,    DEC     0        
N1,     DEC     -1       
0,      DEC     0        
3,      DEC     3        
-1,     DEC     -1       
4,      DEC     4        
-2,     DEC     -2       
10,     DEC     10       
2,      DEC     2        
-8,     DEC     -8       
1,      DEC     1        
-3,     DEC     -3       
48,     DEC     48       
13,     DEC     13       
        END              
