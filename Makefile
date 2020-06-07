TARGET = c
SCGET = sc

PAR = parser
OBJ = lex.o $(PAR).o
ASM = nasm.c lex.c $(PAR).c
SEXP = sexp.c lex.c $(PAR).c


CC = gcc

CFLAGS = 
OUTDIR = build
SRCDIR = src
INCDIR = include 
TESTDIR = test

INCLUDES = $(addprefix -I ,$(INCDIR))
OBJ := $(patsubst %.o, $(OUTDIR)/%.o, $(OBJ))
TARGET := $(OUTDIR)/$(TARGET)
SCGET := $(OUTDIR)/$(SCGET)

.PHONY:	all
all: asm sexp
#all: $(OBJ) $(OUTDIR)/main.o
#	@echo "    CC      "$(TARGET)
#	@$(CC) $(CFLAGS) $^ -o $(TARGET)
#	@cp $(TARGET) $(TESTDIR)/

asm: $(OBJ) $(OUTDIR)/nasm.o
	@echo "    CC      "$(TARGET)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)
	@cp $(TARGET) $(TESTDIR)/

sexp: $(OBJ) $(OUTDIR)/sexp.o
	@echo "    CC      "$(SCGET)
	@$(CC) $(CFLAGS) $^ -o $(SCGET)
	@cp $(SCGET) $(TESTDIR)/

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CC) $(CFLAGS) $< -o $@ -c $(INCLUDES)
.PHONY: clean
clean:
	rm -rf *.exe *.out *.o *.stackdump $(OUTDIR) $(TARGET) $(SCGET)
