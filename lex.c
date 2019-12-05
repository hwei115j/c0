#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "c0.h"

#define MAX_STR 150

static token *ungotten = NULL;

static token *read_punct(int punct)
{
    token *r = malloc(sizeof(token));
    r->type = TTYPE_PUNCT;
    r->punct = punct;
    return r;
}

static token *read_id()
{
    int i, ch;
    char *lexeme = malloc(sizeof(char)*MAX_STR);
    token *r = malloc(sizeof(token));

    ch = getchar();
    for(i = 0; isalpha(ch) || isdigit(ch) || ch == '_'; i++) {
        lexeme[i] = ch;
        ch = getchar();
    }
    ungetc(ch, stdin);
    lexeme[i] = '\0';
    r->type = Id;
    r->sval = lexeme;

    return r;
}

static token *read_num()
{
    int i = 0, ch;
    char *lexeme = malloc(sizeof(char)*MAX_STR);
    token *r = malloc(sizeof(token));

    for(i = 0; isdigit(ch = getchar()); i++)
        lexeme[i] = ch;
    lexeme[i] = '\0';
    ungetc(ch, stdin);
    r->type = Num;
    r->sval = lexeme;

    return r;
}

static token *read_str()
{
    char *lexeme = malloc(sizeof(char)*MAX_STR);
    token *r = malloc(sizeof(token));
    int ch, i;

    for(i = 0; (ch = getchar()) != '"'; i++)
        lexeme[i] = ch;
    lexeme[i] = '\0';
    r->type = Str;
    r->sval = lexeme;

    return r;
}
static token *read_char()
{
    token *r = malloc(sizeof(token));
    r->type = Char;
    r->ch = getchar();

    return r;
}
static token *gettoken()
{
    int ch;

    while((ch = getchar()) != '\n' && isspace(ch));
    /*
     * case Low ... high: 的語法是GCC的擴展語法，並非標準C，使用時請多加注意
     * https://gcc.gnu.org/onlinedocs/gcc-7.4.0/gcc/Case-Ranges.html
     */
    while(ch == '\n' || ch == ' ')
        ch = getchar();
    switch(ch) {
        case '0' ... '9':
            ungetc(ch, stdin);
            return read_num();
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
            ungetc(ch, stdin);
            return read_id();
        case '"':
            return read_str();
        case '\'':
            return read_char();
        case '&':
        case '|':
        case '>':
        case '<':
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
        case '=':
        case ',':
        case '\\':
        case ';':
        case '{':
        case '}':
        case '\n':
            return read_punct(ch);
        case  EOF:
            return NULL;
        default:
            return read_punct(ch);
    }
}

int is_punct(token *tok, int c)
{
    return tok && (tok->type == TTYPE_PUNCT) && (tok->punct == c);
}

void unget_token(token *tok)
{
    if (!tok)
        return;
    if (ungotten)
        printf("Push back buffer is already full");
    ungotten = tok;
}

token *peek_token(void)
{
    token *tok = read_token();
    unget_token(tok);
    return tok;
}

token *read_token(void)
{
    if (ungotten) {
        token *tok = ungotten;
        ungotten = NULL;
        return tok;
    }
    return gettoken();
}
