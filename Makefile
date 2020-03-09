TARGET = c

PAR = n_parser
OBJ = lex.o $(PAR).o
ASM = asm.c lex.c $(PAR).c
SEXP = sexp.c lex.c $(PAR).c
CC = gcc

CFLAGS = 
OUTDIR = build
SRCDIR = src
INCDIR = include 
TESTDIR = test

INCLUDES = $(addprefix -I ,$(INCDIR))
OBJ := $(patsubst %.o, $(OUTDIR)/%.o, $(OBJ))
TARGET := $(TESTDIR)/$(TARGET)

all: $(OBJ) $(OUTDIR)/main.o
	@echo "    CC      "$(TARGET)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)

asm: $(OBJ) $(OUTDIR)/asm.o
	@echo "    CC      "$(TARGET)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)

sexp: $(OBJ) $(OUTDIR)/sexp.o
	@echo "    CC      "$(TARGET)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)
	
$(OUTDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CC) $(CFLAGS) $< -o $@ -c $(INCLUDES)
clean:
	rm -rf *.exe *.out *.o *.stackdump $(OUTDIR) $(TARGET)
