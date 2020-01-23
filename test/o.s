        BSA     main     
        BSA     CALL     
        HLT              
mul,    DEC     0        
        BSA     GETF     
        BUN     mul     I
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
        LDA     3        
        BSA     PUSH     
        BSA     RET      
main,   DEC     0        
        BSA     GETF     
        BUN     main    I
        LDA     1        
        BSA     PUSH     
        LDA     2        
        BSA     PUSH     
        BSA     mul      
        BSA     CALL     
        BSA     RET      
        ORG     1000     
out,    DEC     0        
        BSA     GETF     
        BUN     out     I
        LDA     3        
        BSA     OSET     
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
0,      DEC     0        
3,      DEC     3        
-1,     DEC     -1       
4,      DEC     4        
1,      DEC     1        
2,      DEC     2        
        END              
