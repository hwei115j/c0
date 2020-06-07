                BSA     main             
                BSA     .CALL            
                HLT                      
div,            DEC     0                
                BSA     .GETF            
                BUN     div             I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R1              
                LDA     4                
                BSA     .OSET            
                STA     .R1             I
                LDA     0                
                BSA     .PUSH            
                BUN     .L0              
.L1,            LDA     .BP              
                ADD     -2               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
.L0,            LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SPA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L1              
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                CMA                      
                INC                      
                ADD     .R1              
                CLE                      
                SZA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L2              
                BUN     .L3              
.L2,            LDA     .BP              
                ADD     -2               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L3,            LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
out_num,        DEC     0                
                BSA     .GETF            
                BUN     out_num         I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                CLA                      
                BSA     .PUSH            
                CLA                      
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                LDA     10000            
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L4              
.L5,            LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                BSA     div              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     2                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -2               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                STA     .R2              
                BSA     .POP             
                STA     .R3              
                CLA                      
                STA     .R1              
                LDA     -8               
                STA     .R4              
                BSA     .MUL             
                LDA     .R1              
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SPA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L6              
                BUN     .L7              
.L6,            LDA     1                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -3               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
.L7,            LDA     -3               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L8              
                BUN     .L9              
.L8,            LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     48               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
.L9,            LDA     10               
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     div              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     2                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
.L4,            LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L5              
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
out_str,        DEC     0                
                BSA     .GETF            
                BUN     out_str         I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                LDA     0                
                BSA     .PUSH            
                BUN     .L10             
.L11,           LDA     .BP              
                ADD     0                
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
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
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L10,           LDA     .BP              
                ADD     0                
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
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
                BSA     .POP             
                SZA                      
                BUN     .L11             
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
spa,            DEC     0                
                BSA     .GETF            
                BUN     spa             I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     32               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     32               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                BSA     div              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     2                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                CMA                      
                INC                      
                ADD     .R1              
                CLE                      
                SZA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L12             
                BUN     .L13             
.L12,           LDA     32               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
.L13,           LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
fib,            DEC     0                
                BSA     .GETF            
                BUN     fib             I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                CMA                      
                INC                      
                ADD     .R1              
                CLE                      
                SZA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L14             
                BUN     .L15             
.L14,           LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
.L15,           LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     3                
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L16             
                BUN     .L17             
.L16,           LDA     1                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
.L17,           LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     1                
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                BSA     fib              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     2                
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                BSA     fib              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
swap,           DEC     0                
                BSA     .GETF            
                BUN     swap            I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                STA     .R1              
                LDA     3                
                BSA     .OSET            
                STA     .R1             I
                CLA                      
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R1              
                LDA     4                
                BSA     .OSET            
                STA     .R1             I
                LDA     .BP              
                ADD     0                
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                LDA     0                
                BSA     .PUSH            
                BSA     .POP             
                BSA     .RET             
                CMA                      
                BSA     .RET             
main,           DEC     0                
                BSA     .GETF            
                BUN     main            I
                LDA     0                
                BSA     .PUSH            
                CLA                      
                BSA     .PUSH            
                CLA                      
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                LDA     20               
                BSA     .PUSH            
                BSA     .s0              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L18             
.L19,           LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     0                
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                LDA     .BP              
                ADD     -1               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L18,           LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     101              
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L19             
                LDA     0                
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     0                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L20             
.L21,           LDA     0                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -2               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L22             
.L23,           LDA     42               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -2               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L22,           LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L23             
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L20,           LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L21             
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                CLA                      
                BSA     .PUSH            
                LDA     1                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L24             
.L25,           LDA     1                
                BSA     .PUSH            
                LDA     .BP              
                ADD     -2               
                BSA     .PUSH            
                BSA     .POP             
                STA     .R0              
                BSA     .POP             
                STA     .R0             I
                BUN     .L26             
.L27,           LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     42               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     61               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     .POP             
                STA     .R2              
                BSA     .POP             
                STA     .R3              
                CLA                      
                STA     .R1              
                LDA     -8               
                STA     .R4              
                BSA     .MUL             
                LDA     .R1              
                BSA     .PUSH            
                BSA     spa              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -2               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L26,           LDA     -2               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L27             
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -1               
                STA     .R0              
                LDA     .R0             I
                BSA     .PUSH            
                LDA     .R0             I
                INC                      
                STA     .R0             I
.L24,           LDA     -1               
                BSA     .OSET            
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     .POP             
                CMA                      
                INC                      
                STA     .R1              
                BSA     .POP             
                ADD     .R1              
                CLE                      
                SNA                      
                CME                      
                CME                      
                CLA                      
                CIL                      
                BSA     .PUSH            
                BSA     .POP             
                SZA                      
                BUN     .L25             
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s1              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     23               
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s2              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     23               
                BSA     .PUSH            
                BSA     fib              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     10               
                BSA     .PUSH            
                BSA     out              
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s3              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -3               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s4              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -4               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s5              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     .BP              
                ADD     -4               
                BSA     .PUSH            
                LDA     .BP              
                ADD     -3               
                BSA     .PUSH            
                BSA     swap             
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     2                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s6              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -3               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s7              
                BSA     .PUSH            
                BSA     out_str          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                LDA     -4               
                BSA     .OSET            
                BSA     .PUSH            
                BSA     out_num          
                BSA     .CALL            
                STA     .R1              
                LDA     .SP              
                ADD     1                
                STA     .SP              
                LDA     .R1              
                BSA     .PUSH            
                BSA     .s8              
                BSA     .PUSH            
                BSA     out_str          
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
.SP,            DEC     4095             
.BP,            DEC     4095             
.R0,            DEC     0                
.R1,            DEC     0                
.R2,            DEC     0                
.R3,            DEC     0                
.R4,            DEC     0                
.N1,            DEC     -1               
.N2,            DEC     -2               
0,              DEC     0                
3,              DEC     3                
-1,             DEC     -1               
4,              DEC     4                
-2,             DEC     -2               
10000,          DEC     10000            
2,              DEC     2                
-8,             DEC     -8               
1,              DEC     1                
-3,             DEC     -3               
48,             DEC     48               
10,             DEC     10               
32,             DEC     32               
20,             DEC     20               
.s0,            DEC     0                
                BSA     .GETF            
                BUN     .s0             I
                DEC     104              
                DEC     101              
                DEC     108              
                DEC     108              
                DEC     111              
                DEC     44               
                DEC     32               
                DEC     119              
                DEC     111              
                DEC     114              
                DEC     108              
                DEC     100              
                DEC     10               
                DEC     10               
                DEC     0                
101,            DEC     101              
42,             DEC     42               
61,             DEC     61               
.s1,            DEC     0                
                BSA     .GETF            
                BUN     .s1             I
                DEC     102              
                DEC     105              
                DEC     98               
                DEC     40               
                DEC     0                
23,             DEC     23               
.s2,            DEC     0                
                BSA     .GETF            
                BUN     .s2             I
                DEC     41               
                DEC     32               
                DEC     61               
                DEC     32               
                DEC     0                
.s3,            DEC     0                
                BSA     .GETF            
                BUN     .s3             I
                DEC     32               
                DEC     32               
                DEC     32               
                DEC     32               
                DEC     40               
                DEC     97               
                DEC     44               
                DEC     32               
                DEC     98               
                DEC     41               
                DEC     32               
                DEC     61               
                DEC     32               
                DEC     40               
                DEC     0                
.s4,            DEC     0                
                BSA     .GETF            
                BUN     .s4             I
                DEC     44               
                DEC     32               
                DEC     0                
-4,             DEC     -4               
.s5,            DEC     0                
                BSA     .GETF            
                BUN     .s5             I
                DEC     41               
                DEC     10               
                DEC     0                
.s6,            DEC     0                
                BSA     .GETF            
                BUN     .s6             I
                DEC     115              
                DEC     119              
                DEC     97               
                DEC     112              
                DEC     40               
                DEC     97               
                DEC     44               
                DEC     32               
                DEC     98               
                DEC     41               
                DEC     32               
                DEC     61               
                DEC     32               
                DEC     40               
                DEC     0                
.s7,            DEC     0                
                BSA     .GETF            
                BUN     .s7             I
                DEC     44               
                DEC     32               
                DEC     0                
.s8,            DEC     0                
                BSA     .GETF            
                BUN     .s8             I
                DEC     41               
                DEC     10               
                DEC     0                
                END                      
