                BSA     main             
                BSA     .CALL            
                HLT                      
func,           DEC     0                
                BSA     .GETF            
                BUN     func            I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                LDA     .BP              
                ADD     0                
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
main,           DEC     0                
                BSA     .GETF            
                BUN     main            I
                LDA     9                
                BSA     .PUSH            
                LDA     8                
                BSA     .PUSH            
                LDA     7                
                BSA     .PUSH            
                LDA     6                
                BSA     .PUSH            
                LDA     5                
                BSA     .PUSH            
                LDA     4                
                BSA     .PUSH            
                LDA     3                
                BSA     .PUSH            
                LDA     2                
                BSA     .PUSH            
                LDA     1                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -8               
                BSA     .PUSH            
                BSA     func             
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                CMA                      
                BSA     .RET             
                ORG     2000             
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
                BSA     .GETF            
                BUN     out             I
                LDA     3                
                BSA     .OSET            
                OUT                      
                BSA     .RET             
in,             DEC     0                
                BSA     .GETF            
                BUN     in              I
                INP                      
                BSA     .RET             
va_start,       DEC     0                
                BSA     .GETF            
                BUN     va_start        I
                LDA     .BP              
                ADD     3                
                STA     .R0              
                LDA     .R0             I
                ADD     4                
                BSA     .RET             
.OSET,          DEC     0                
                ADD     .BP              
                STA     .R0              
                LDA     .R0             I
                BUN     .OSET           I
.PUSH,          DEC     0                
                STA     .SP             I
                LDA     .SP              
                ADD     .N1              
                STA     .SP              
                BUN     .PUSH           I
.POP,           DEC     0                
                LDA     .SP              
                INC                      
                STA     .SP              
                LDA     .SP             I
                BUN     .POP            I
.RET,           DEC     0                
                STA     .R1              
                LDA     .BP              
                STA     .SP              
                BSA     .POP             
                STA     .BP              
                BSA     .POP             
                STA     .R0              
                LDA     .R1              
                BUN     .R0             I
.CALL,          DEC     0                
                STA     .R0              
                LDA     .CALL            
                BSA     .PUSH            
                LDA     .BP              
                BSA     .PUSH            
                LDA     .SP              
                STA     .BP              
                BUN     .R0             I
.GETF,          DEC     0                
                LDA     .GETF            
                INC                      
                BUN     .GETF           I
.MUL,           DEC     0                
.LOP,           CLE                      
                LDA     .R3              
                CIR                      
                STA     .R3              
                SZE                      
                BUN     .ONE             
                BUN     .ZRO             
.ONE,           LDA     .R2              
                ADD     .R1              
                STA     .R1              
                CLE                      
.ZRO,           LDA     .R2              
                CIL                      
                STA     .R2              
                ISZ     .R4              
                BUN     .LOP             
                BUN     .MUL            I
.NUM,           DEC     48               
.SP,            DEC     3999             
.BP,            DEC     3999             
.R0,            DEC     0                
.R1,            DEC     0                
.R2,            DEC     0                
.R3,            DEC     0                
.R4,            DEC     0                
.N1,            DEC     -1               
.N2,            DEC     -2               
0,              DEC     0                
3,              DEC     3                
9,              DEC     9                
8,              DEC     8                
7,              DEC     7                
6,              DEC     6                
5,              DEC     5                
4,              DEC     4                
2,              DEC     2                
1,              DEC     1                
-8,             DEC     -8               
                END                      
