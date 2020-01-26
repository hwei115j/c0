        BSA     main     
        BSA     CALL     
        HLT              
func,   DEC     0        
        BSA     GETF     
        BUN     func    I
        LDA     1        
        BSA     PUSH     
        LDA     2        
        BSA     PUSH     
        LDA     3        
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
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     10       
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
        BSA     RET      
main,   DEC     0        
        BSA     GETF     
        BUN     main    I
        LDA     0        
        BSA     PUSH     
        LDA     0        
        BSA     PUSH     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L0      
        BUN     .L1      
.L0,    NOP              
        LDA     100      
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
.L1,    NOP              
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L2      
        LDA     10       
        BSA     PUSH     
        BSA     func     
        BSA     CALL     
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
        LDA     10       
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        BSA     func     
        BSA     CALL     
        BUN     .L3      
.L2,    NOP              
        BSA     func     
        BSA     CALL     
.L3,    NOP              
        BUN     .L4      
.L5,    NOP              
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        LDA     1        
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
        BSA     func     
        BSA     CALL     
.L4,    NOP              
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L5      
        LDA     100      
        BSA     PUSH     
        LDA     BP       
        ADD     0        
        STA     R0       
        BSA     POP      
        STA     R0      I
        BUN     .L6      
.L7,    NOP              
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        LDA     BP       
        ADD     -1       
        STA     R0       
        BSA     POP      
        STA     R0      I
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
.L6,    NOP              
        LDA     0        
        BSA     OSET     
        BSA     PUSH     
        BSA     POP      
        SZA              
        BUN     .L7      
        LDA     -1       
        BSA     OSET     
        BSA     PUSH     
        LDA     1        
        BSA     PUSH     
        BSA     POP      
        STA     R1       
        BSA     POP      
        ADD     R1       
        BSA     PUSH     
        BSA     POP      
        BSA     RET      
        ORG     1000     
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
.LOP,   LDA     .R3      
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
1,      DEC     1        
2,      DEC     2        
3,      DEC     3        
-8,     DEC     -8       
0,      DEC     0        
10,     DEC     10       
100,    DEC     100      
-1,     DEC     -1       
        END              
