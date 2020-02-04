#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "c0.h"

#define MAX_STR 150

static token *ungotten = NULL;

static token *read_punct(int punct)
{
    int ch;
    int reg = 0;

    switch(punct) {
        case '=':
            reg = PUNCT_EQ;
            break;
        case '>':
            reg = PUNCT_CIR;
            break;
        case '<':
            reg = PUNCT_CIL;
            break;
        case '+':
            reg = PUNCT_INC;
            break;
        case '-':
            reg = PUNCT_DEC;
            break;
    }
    if((ch = getchar()) == punct && reg)
        punct = reg;
    else
        ungetc(ch, stdin);
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
/*
static int getnext()
{
    int ch = getchar();

    while(ch != '\n' && isspace(ch) && ch != ' ') {

        //while((ch = getchar()) == '\n' && isspace(ch));
        while(ch == '\n' || ch == ' ' && isspace(ch))
            ch = getchar();

        if(ch == '/') {
            ch = getchar();

            if(ch == '/') {
                while((ch = getchar()) != '\n' && ch != EOF);
            } else if(ch == '*') {
                while(ch != EOF && (ch = getchar()) != EOF) {
                    if(ch == '*' && (ch = getchar()) == '/') {
                        break;
                    }
                }
            } else {
                ungetc(ch, stdin);
            }
        }
    }
    return ch;
}
*/
static token *gettoken()
{
    int ch;

    while(((ch = getchar()) == '\n' ||  isspace(ch)) && ch != EOF);

    /*
     * case Low ... high: 的語法是GCC的擴展語法，並非標準C，使用時請多加注意
     * https://gcc.gnu.org/onlinedocs/gcc-7.4.0/gcc/Case-Ranges.html
     */
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
        case '/': {
            ch = getchar();
            if(ch == '/') {
                while((ch = getchar()) != '\n' && ch != EOF);
                return gettoken();
            }
            else if(ch == '*') {
                while(1) {
                    if((ch = getchar()) == '*') {
                        if((ch = getchar()) == '/')
                            break;
                        else 
                            ungetc(ch, stdin);
                    }
                }
                return gettoken();
            }
            else {
                ungetc(ch, stdin);
                ch = '/';
            }
        }
        case '\\':
        case '!':
        case '&':
        case '|':
        case '>':
        case '<':
        case '+':
        case '-':
        case '*':
        case '(':
        case ')':
        case '=':
        case ',':
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
