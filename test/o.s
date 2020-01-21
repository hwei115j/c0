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
