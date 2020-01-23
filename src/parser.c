#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c0.h"
#define error(STR) pferror(STR, __LINE__)
#define expect(STR) pfexpect(STR, __LINE__)

static Ctype *ctype_int = &(Ctype){CTYPE_INT, 2, NULL};
static Ctype *ctype_long = &(Ctype){CTYPE_LONG, 4, NULL};
static Ctype *ctype_char = &(Ctype){CTYPE_CHAR, 1, NULL};
static Ctype *ctype_float = &(Ctype){CTYPE_FLOAT, 4, NULL};
static Ctype *ctype_double = &(Ctype){CTYPE_DOUBLE, 8, NULL};

static Ast *new_ast()
{
    Ast *r = malloc(sizeof(Ast));
    memset(r, 0, sizeof(Ast));
    return r;
}

static void pferror(char *s, int line)
{
    fprintf(stderr, "parser %d:%s error!\n",line, s);
    exit(1);
}

static void pfexpect(char punct, int line)
{
    token *tok = read_token();
    if(!tok || !is_punct(tok, punct)) {
        char str[100];
        sprintf(str, "The next token is '%c' (tok = %c) ", punct, tok->punct);
        pferror(str, line);
    }
}
static Ctype *get_ctype(token *);
static int priority(token *);
static int is_type_keyword(token *);
static Ast *stmt_expr(int);
static Ast *unary_expr();
static Ast *ast_binop(int, Ast *, Ast *);
static Ast *expr();
static Ast *decl_or_func();
static Ast *func_def(Ctype *, char *);
static Ast *compound_stmt();
static Ast *stmt();
static Ast *decl_or_stmt();
static Ast *decl();
static Ast *ast_compound_stmt(List *);
static Ast *ast_var(Ctype *, char *, int);
static Ast *decl_init(Ast *);
static Ast *ast_decl(Ast *, Ast *);
static Ast *selection_stmt();
static Ast *ret_stmt();
static Ast *ident_or_func(char *);
static Ast *iteration_stmt();
static Ast *while_stmt();

static Ast *expr()
{
    return stmt_expr(18);
}

List *statement()
{
    List *list = make_list();
    
    while(1) {
        Ast *ast = decl_or_func();
        if(!ast)
            return list;
        list_push(list, ast);
    }
}

static Ast *stmt_expr(int prec)
{
    Ast *ast = unary_expr();

    while(1) {
        token *tok = read_token();
        if(!tok) expect(';');
        if(tok->type != TTYPE_PUNCT) {
            unget_token(tok);
            return ast;
        }
        int prec2 = priority(tok);
        if(prec2 < 0 || prec <= prec2) {
            unget_token(tok);
            return ast;
        }
        Ast *rest = stmt_expr(prec2);
        ast = ast_binop(tok->punct, ast, rest);
    }
}

static Ast *unary_expr()
{
    token *tok = read_token();

    if(tok->type == TTYPE_PUNCT) {
        if(is_punct(tok, '(')) {
            Ast *ast = expr();
            expect(')');
            return ast;
        }
        else {
            unget_token(tok);
            return NULL;
        }
    }
    if(tok->type == Num) {
        Ast *ast = new_ast();
        ast->type = AST_LITERAL;
        ast->ctype = ctype_int;
        ast->ival = atoi(tok->sval);
        return ast;
    }
    else if(tok->type == Id) {
        return ident_or_func(tok->sval);
    }
    else {
        printf("%d\n", tok->type);
        printf("%c\n", tok->punct);
        error("unary_expr");
    }

}

static Ast *ident_or_func(char *id)
{
    Ast *ast = new_ast();
    token *tok = read_token();
    List *args = make_list();

    if(is_punct(tok, '(')) {
        while(1) {
            list_push(args, expr());
            tok = peek_token();
            if(is_punct(tok, ','))
                expect(',');
            else if(is_punct(tok, ')'))
                break;
            else
                error("next is ',' or ')'!");
        }
        expect(')');
        ast->type = AST_FUNCALL;
        ast->args = args;
        ast->fname = id;
        return ast;
    }
    unget_token(tok);
    ast->type = AST_LVAR;
    ast->ctype = ctype_int;
    ast->varname = id;
    return ast;
}
static int priority(token *tok)
{
    switch(tok->punct) {
        case '*':
        case '/':
        case '%':
            return 3;
        case '+':
        case '-':
            return 4;
        case '=':
            return 16;
        defaule:
            return -1;
    }
}

static Ast *ast_binop(int punct, Ast *left, Ast *right)
{
    Ast *ast = new_ast();

    ast->type = TTYPE_PUNCT;
    ast->ival = punct;
    ast->left = left;
    ast->right = right;

    return ast;
}

static Ast *decl_or_func()
{
    token *tok = read_token();
    if(!tok) 
        return NULL;
    Ctype *ctype = get_ctype(tok);
    token *name  = read_token();
    if(name->type != Id)
        error("next is not Id");
    tok = peek_token();
    if(is_punct(tok, '=') || is_punct(tok, ';')) {
        Ast *var = ast_var(ctype, name->sval, AST_GVAR);
        return decl_init(var);
    }
    if(is_punct(tok, '(')) {
        return func_def(ctype, name->sval); 
    }
    expect(';');
    return NULL;
}

static Ast *func_def(Ctype *ctype, char *name)
{
    List *args = make_list();    
    expect('(');
    while(1) {
        token *tok = peek_token();
        if(is_punct(tok, ','))
            error("next is not ','!");
        if(is_punct(tok, ')'))
            break;
        list_push(args, decl());
    }
    expect(')');
    expect('{');
    Ast *func = new_ast();
    func->type = AST_FUNC;
    func->ctype = ctype;
    func->fname = name;
    func->args = args;
    func->body = compound_stmt();
    return func;
}

static Ast *compound_stmt()
{
    List *list = make_list();

    while(1) {
        Ast *ast = decl_or_stmt();
        if(!ast)
            break;
        list_push(list, ast);
        token *tok = read_token();
        if(is_punct(tok, '}'))
            break;
        unget_token(tok);
    }
    return ast_compound_stmt(list);
}

static Ctype *get_ctype(token *tok)
{
    if (!tok)
        return NULL;
    if (tok->type != Id)
        return NULL;
    if (!strcmp(tok->sval, "int"))
        return ctype_int;
    if (!strcmp(tok->sval, "long"))
        return ctype_long;
    if (!strcmp(tok->sval, "char"))
        return ctype_char;
    if (!strcmp(tok->sval, "float"))
        return ctype_float;
    if (!strcmp(tok->sval, "double"))
        return ctype_double;
    return NULL;
}

static int is_type_keyword(token *tok)
{
    return !!get_ctype(tok);
}

static Ast *decl()
{
    token *tok   = read_token();
    Ctype *ctype = get_ctype(tok);
    token *name  = read_token();
    if(name->type != Id) error("next token is not Id\n");
    Ast *var = ast_var(ctype, name->sval, AST_LVAR);

    return decl_init(var);
}

static Ast *decl_or_stmt()
{
    token *tok = peek_token();
    if(!tok) 
        return NULL;
    return is_type_keyword(tok)?decl():stmt();
}

static int idcmp(token *tok, char *str)
{
    return tok->type == Id && !strcmp(tok->sval, str);
}
static Ast *stmt()
{
    token *tok = read_token();

    if(idcmp(tok, "if")) {
        return selection_stmt();
    }
    else if(idcmp(tok, "for")) {
        return iteration_stmt();
    }
    else if(idcmp(tok, "while")) {
        return while_stmt();
    }
    else if(idcmp(tok, "return")) {
        return ret_stmt();
    }
    else if(is_punct(tok, '{')) {
        return compound_stmt();
    }
    if(get_ctype(tok) != NULL)
        error("decl");

    unget_token(tok);
    Ast *r = expr();
    expect(';');
    return r;
}
static Ast *ast_compound_stmt(List *list)
{
    Ast *ast = new_ast();

    ast->type = AST_COMPOUND_STMT;
    ast->stmts = list;

    return ast;
}

static Ast *ast_var(Ctype *ctype, char *sval, int AST)
{
    Ast *r = new_ast();
    r->type = AST;
    r->ctype = ctype;
    r->varname = sval;
    return r;
}
static Ast *decl_init(Ast *var)
{
    token *tok = read_token();

    if(is_punct(tok, ',') || is_punct(tok, ';')) {
        return ast_decl(var, NULL);
    }
    if(is_punct(tok, ')')) {
        unget_token(tok);
        return ast_decl(var, NULL);
    }
    if(is_punct(tok, '=')) {
        Ast *init = expr();
        expect(';');
        return ast_decl(var, init);
    }
    error("next is ',' or ';' or '='!");
}

static Ast *ast_decl(Ast *var, Ast *init)
{
    Ast *ast = new_ast();
    ast->type = AST_DECL;
    ast->ctype = NULL;
    ast->declvar = var;
    ast->declinit = init;
    return ast;
}
static Ast *selection_stmt()
{
    Ast *r = new_ast();

    r->type = AST_IF;
    expect('(');
    r->cond = expr();
    expect(')');
    r->then = stmt();
    if(idcmp(peek_token(), "else")) {
        read_token();
        r->els = stmt();
    }

    return r;
}
static Ast *ret_stmt()
{
    Ast *r = new_ast();

    r->type = AST_RETURN;
    r->retval = expr(); 
    expect(';');

    return r;
}
static Ast *iteration_stmt()
{
    Ast *r = new_ast();

    r->type = AST_FOR;
    expect('(');
    token *tok = read_token();
    if(is_punct(tok, ';')) {
        unget_token(tok);
        r->forinit = NULL;
        expect(';');
    }
    else if(get_ctype(tok)) {
        unget_token(tok);
        r->forinit = decl();
    }
    else {
        unget_token(tok);
        r->forinit = expr();
        expect(';');
    }
    tok = read_token();
    if(is_punct(tok, ';')) {
        unget_token(tok);
        r->forcond = NULL;
    }
    else {
        unget_token(tok);
        r->forcond = expr();
    }
    expect(';');
    tok = read_token();
    if(is_punct(tok, ')')) {
        unget_token(tok);
        r->forstep = NULL;
    }
    else {
        unget_token(tok);
        r->forstep = expr();
    }
    expect(')');
    r->forbody = stmt();

    return r;
}

static Ast *while_stmt()
{
    Ast *r = new_ast();

    r->type = AST_WHILE;
    expect('(');
    r->whilecond = expr();
    expect(')');
    r->whilebody = stmt();

    return r;
}
