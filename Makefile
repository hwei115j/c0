TARGET = c

OBJ = main.c lex.c parser.c
ASM = asm.c lex.c parser.c
SEXP = sexp.c lex.c parser.c
CC = gcc

CFLAGS =
OUTDIR = build
SRCDIR = src
INCDIR = include 
INCLUDES = $(addprefix -I,$(INCDIR))

$(OUTDIR)/$(TARGET): $(OUTDIR)/%.o
	$(CC) $(CFLAGS) $@ -o $(TARGET)

$(OUTDIR)/%.o: %.c
	mkdir -p $(dir $@)
	echo "    CC      "$@
	$(CC) $(CFLAGS) $@ -o $@ -c $(INCLUDES) $<
asm:
	${CC} ${ASM} -o c
sexp:
	${CC} ${SEXP} -o c
debug:
	${CC} -g ${OBJ} -o c && gdb c
clean:
	rm -f *.exe *.out *.o *.stackdump
