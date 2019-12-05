OBJ = main.c lex.c parser.c
ASM = asm.c lex.c parser.c
SEXP = sexp.c lex.c parser.c
CC = gcc

all:
	${CC} ${OBJ} -o c
asm:
	${CC} ${ASM} -o c
sexp:
	${CC} ${SEXP} -o c
debug:
	${CC} -g ${OBJ} -o c && gdb c
clean:
	rm -f *.exe *.out *.o *.stackdump
