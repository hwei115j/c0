#ifndef __C0_H_
#define __C0_H_
#include <stdarg.h>
#include "dict.h"
#include "list.h"

#define error(...) pferror(__FILE__, __LINE__, __VA_ARGS__)

#define swap(a, b)         \
    {                      \
        typeof(a) tmp = b; \
        b = a;             \
        a = tmp;           \
    }

static inline void pferror(char *file, int line, char *fmt, ...)
{
    fprintf(stderr, "%s:%d: ", file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}


enum {
   Unkonwn = 128,
   TTYPE_PUNCT,
   Num, End, Id, Str,
   Addsub, Muldiv, Assign, Lparen, Rparen, Dq, Comma, Slash, Bslash, Lbraces, Rbraces, Semicolon,
   If, Int, Char, While, Ret
};

enum {
    AST_LITERAL = 256,
    AST_STRING,
    AST_LVAR,
    AST_GVAR,
    AST_FUNCALL,
    AST_FUNC,
    AST_DECL,
    AST_ARRAY_INIT,
    AST_ADDR,
    AST_DEREF,
    AST_IF,
    AST_TERNARY,
    AST_FOR,
    AST_WHILE,
    AST_RETURN,
    AST_COMPOUND_STMT,
    AST_STRUCT_REF,
    AST_ASSIGNMENT_EXPR,
    PUNCT_EQ,
    PUNCT_INC,
    PUNCT_DEC,
    PUNCT_LOGAND,
    PUNCT_LOGOR,
    PUNCT_ARROW,

    PUNCT_CIR,
    PUNCT_CIL,
    PUNCT_NE,
    PUNCT_GE,
    PUNCT_LE,
    PUNCT_VLA
};

enum {
    CTYPE_VOID,
    CTYPE_CHAR,
    CTYPE_INT,
    CTYPE_LONG,
    CTYPE_FLOAT,
    CTYPE_DOUBLE,
    CTYPE_ARRAY,
    CTYPE_PTR,
    CTYPE_STRUCT,
};

typedef struct __Ctype {
    int type;
    int size;
    struct __Ctype *ptr; /* pointer or array */
    int len;             /* array length */
    /* struct */
    int offset;
    struct dict *dict;
} Ctype;

typedef struct {
    int type;
    union {
        char *sval;
        int  punct;
        int ch;
    };
}token;

typedef struct __Ast {
    int type;
    Ctype *ctype;
    /* char, int, or long */
    long ival;
    union {

        /* float or double */
        struct {
            double fval;
            char *flabel;
        };

        /* string literal */
        struct {
            char *sval;
            char *slabel;
        };

        /* Local/global variable */
        struct {
            char *varname;
            struct {
                int loff;
                char *glabel;
            };
        };

        /* Binary operator */
        struct {
            struct __Ast *left;
            struct __Ast *right;
        };

        /* Unary operator */
        struct {
            struct __Ast *operand;
        };

        /* Function call or function declaration */
        struct {
            char *fname;
            struct {
                List *args;
                struct {
                    List *params;
                    List *localvars;
                    struct __Ast *body;
                };
            };
        };

        /* Declaration */
        struct {
            struct __Ast *declvar;
            struct __Ast *declinit;
        };

        /* Array initializer */
        List *arrayinit;

        /* if statement or ternary operator */
        struct {
            struct __Ast *cond;
            struct __Ast *then;
            struct __Ast *els;
        };

        /* for statement */
        struct {
            struct __Ast *forinit;
            struct __Ast *forcond;
            struct __Ast *forstep;
            struct __Ast *forbody;
        };
        /* while statement */
        struct {
            struct __Ast *whilecond;
            struct __Ast *whilebody;
        }; 
        /* return statement */
        struct __Ast *retval;

        /* Compound statement */
        List *stmts;
        
        List *exprs;
        /* Struct reference */
        struct {
            struct __Ast *struc;
            char *field; /* specific to ast_to_string only */
        };
    };
} Ast;

static char *getype(Ctype *ctype)
{
    char *str = malloc(sizeof(char) * 100);
    char *s = str;
    while(true) {
        switch(ctype->type) {
            case CTYPE_VOID:
                s += sprintf(s, "void"); 
                return str;
            case CTYPE_CHAR:
                s += sprintf(s, "char"); 
                return str;
            case CTYPE_INT:
                s += sprintf(s, "int");
                return str;
            case CTYPE_LONG:
                s += sprintf(s, "long"); 
                return str;
            case CTYPE_FLOAT:
                s += sprintf(s, "float"); 
                return str;
            case CTYPE_DOUBLE:
                s += sprintf(s, "double"); 
                return str;
            case CTYPE_ARRAY:
                s += sprintf(s, "[%d]", ctype->len); 
                ctype = ctype->ptr;
                break;
            case CTYPE_PTR:
                s += sprintf(s, "*");
                ctype = ctype->ptr;
                break;
            case CTYPE_STRUCT:
                s += sprintf(s, "struct"); 
                return str;
            default:
                fprintf(stderr, "zzzz\n");
                return NULL;
        }
    }
    return NULL;
}


/* lex.c */
extern int is_punct(token *tok, int c); /*判斷tok是否是punct且符合c*/
extern void unget_token(token *tok);    /*將tok還回去*/
extern token *peek_token(void);         /*預先得知下個token，但不讀入*/
extern token *read_token(void);         /*讀入下個token*/

/*parser.c */
extern List *statement();
#endif
