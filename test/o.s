                BSA     main             
                BSA     CALL             
                HLT                      
main,           DEC     0                
                BSA     GETF             
                BUN     main            I
                CMA                      
                BSA     RET              
                ORG     1000             
MCIR,           DEC     0                
                BUN     ._L0             
._L1,           LDA     .R2              
                CIR                      
                STA     .R2              
._L0,           ISZ     .R3              
                BUN     ._L1             
                BUN     MCIR            I
MCIL,           DEC     0                
                BUN     ._L2             
._L3,           LDA     .R2              
                CIL                      
                STA     .R2              
._L2,           ISZ     .R3              
                BUN     ._L3             
                BUN     MCIL            I
out,            DEC     0                
                BSA     GETF             
                BUN     out             I
                LDA     3                
                BSA     OSET             
                OUT                      
                BSA     RET              
in,             DEC     0                
                BSA     GETF             
                BUN     in              I
                INP                      
                BSA     RET              
OSET,           DEC     0                
                ADD     BP               
                STA     R0               
                LDA     R0              I
                BUN     OSET            I
PUSH,           DEC     0                
                STA     SP              I
                LDA     SP               
                ADD     N1               
                STA     SP               
                BUN     PUSH            I
POP,            DEC     0                
                LDA     SP               
                INC                      
                STA     SP               
                LDA     SP              I
                BUN     POP             I
RET,            DEC     0                
                STA     R1               
                LDA     BP               
                STA     SP               
                BSA     POP              
                STA     BP               
                BSA     POP              
                STA     R0               
                LDA     R1               
                BUN     R0              I
CALL,           DEC     0                
                STA     R0               
                LDA     CALL             
                BSA     PUSH             
                LDA     BP               
                BSA     PUSH             
                LDA     SP               
                STA     BP               
                BUN     R0              I
GETF,           DEC     0                
                LDA     GETF             
                INC                      
                BUN     GETF            I
.MUL,           DEC     0                
.LOP,           CLE                      
                LDA     .R3              
                CIR                      
                STA     .R3              
                SZE                      
                BUN     .ONE             
                BUN     .ZRO             
.ONE,           LDA     .R2              
                ADD     R1               
                STA     R1               
                CLE                      
.ZRO,           LDA     .R2              
                CIL                      
                STA     .R2              
                ISZ     .R4              
                BUN     .LOP             
                BUN     .MUL            I
NUM,            DEC     48               
SP,             DEC     3999             
BP,             DEC     3999             
R0,             DEC     0                
R1,             DEC     0                
.R2,            DEC     0                
.R3,            DEC     0                
.R4,            DEC     0                
N1,             DEC     -1               
.s0,            DEC     97               
                DEC     108              
                DEC     100              
                DEC     106              
                DEC     102              
                DEC     0                
3,              DEC     3                
                END                      
