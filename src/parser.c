#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c0.h"
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

static void pfexpect(char punct, int line)
{
    token *tok = read_token();
    if(!tok || !is_punct(tok, punct)) {
        char str[100];
        sprintf(str, "The next token is '%c' (tok = %c) ", punct, tok->punct);
        pferror(str,__FILE__, line);
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

static int is_assig_opt(token *);
static Ast *assignment_expr();
static Ast *conditional_expr();
static Ast *logical_or_expr();
static Ast *logical_and_expr();
static Ast *inclusive_or_expr();
static Ast *exclusive_or_expr();
static Ast *and_expr();
static Ast *equality_expr();
static Ast *relational_expr();
static Ast *shift_expr();
static Ast *additive_expr();
static Ast *multiplicative_expr();
static Ast *cast_expr();
static Ast *postfix_expr();
static Ast *primary_expr();

static int is_assig_opt(token *tok)
{
    if(is_punct(tok, '=') || is_punct(tok, PUNCT_NE) || is_punct(tok, PUNCT_GE) || is_punct(tok, PUNCT_LE))
        return 1;
    return 0;
}
/*
static Ast *expr()
{
    return assignment_expr();
}
*/
static Ast *expr()
{
    Ast *ast = assignment_expr();
    token *tok = read_token();
    List *exprs = make_list();

    list_push(exprs, ast);
    while(is_punct(tok, ',')) {
        ast = assignment_expr();
        list_push(exprs, ast);
        tok = read_token();
    }

    unget_token(tok);
    ast = new_ast();
    ast->type = AST_ASSIGNMENT_EXPR;
    ast->exprs = exprs;
    
    return ast;
}
static Ast *assignment_expr()
{
    Ast *ast = conditional_expr();
    token *tok = read_token();
    
    if(is_assig_opt(tok)) {
        Ast *r = assignment_expr();
        return ast_binop(tok->punct, ast, r);
    }
    unget_token(tok);

    return ast;
}

static Ast *conditional_expr()
{
    Ast *ast = logical_or_expr();
    token *tok = read_token();

    if(is_punct(tok, '?')) {
        //Ast *r = expression();
        Ast *r = expr();
        token *rtok = read_token();
        if(!is_punct(rtok, ':'))
            error("next is ':' ");
        Ast *l = conditional_expr();
        r = ast_binop(rtok->punct, r, l);

        return ast_binop(tok->punct, ast, r);
    }
    unget_token(tok);

    return ast;
}


static Ast *logical_or_expr()
{
    Ast *ast = logical_and_expr();
    token *tok = read_token();

    while(is_punct(tok, PUNCT_LOGOR)) {
        ast = ast_binop(tok->punct, ast, logical_and_expr());
        tok = read_token();
    }
    unget_token(tok);
    return ast;
}

static Ast *logical_and_expr()
{
    Ast *ast = inclusive_or_expr();
    token *tok = read_token();

    while(is_punct(tok, PUNCT_LOGAND)) {
        ast = ast_binop(tok->punct, ast, inclusive_or_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *inclusive_or_expr()
{
    Ast *ast = exclusive_or_expr();
    token *tok = read_token();

    while(is_punct(tok, '|')) {
        ast = ast_binop(tok->punct, ast, exclusive_or_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *exclusive_or_expr()
{
    Ast *ast = and_expr();
    token *tok = read_token();

    while(is_punct(tok, '^')) {
        ast = ast_binop(tok->punct, ast, and_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *and_expr()
{
    Ast *ast = equality_expr();
    token *tok = read_token();

    while(is_punct(tok, '&')) {
        ast = ast_binop(tok->punct, ast, equality_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *equality_expr()
{
    Ast *ast = relational_expr();
    token *tok = read_token();

    while(is_punct(tok, PUNCT_EQ) || is_punct(tok, PUNCT_NE)) {
        ast = ast_binop(tok->punct, ast, relational_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *relational_expr()
{
    Ast *ast = shift_expr();
    token *tok = read_token();

    while(is_punct(tok, '<') || is_punct(tok, '>') || is_punct(tok, PUNCT_LE) || is_punct(tok, PUNCT_GE)) {
        ast = ast_binop(tok->punct, ast, shift_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *shift_expr()
{
    Ast *ast = additive_expr();
    token *tok = read_token();

    while(is_punct(tok, PUNCT_CIL) || is_punct(tok, PUNCT_CIR)) {
        ast = ast_binop(tok->punct, ast, additive_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *additive_expr()
{
    Ast *ast = multiplicative_expr();
    token *tok = read_token();

    while(is_punct(tok, '+') || is_punct(tok, '-')) {
        ast = ast_binop(tok->punct, ast, multiplicative_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *multiplicative_expr()
{
    Ast *ast = cast_expr();
    token *tok = read_token();

    while(is_punct(tok, '*') || is_punct(tok, '/') || is_punct(tok, '%')) {
        ast = ast_binop(tok->punct, ast, cast_expr());
        tok = read_token();
    }

    unget_token(tok);
    return ast;
}

static Ast *cast_expr()
{
    token *tok = read_token();
    Ast *ast;
   
    /*
    if(is_punct(tok, '(')) {
        //Ast *ast = type_name();
        expect(')');
        cast_expr();
    }
*/
    unget_token(tok);
    return unary_expr();
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

static Ast *unary_expr()
{
    token *tok = read_token();

    if(is_punct(tok, PUNCT_INC) || is_punct(tok, PUNCT_DEC)) {
        return ast_binop(tok->punct,NULL, unary_expr());
    }
    else if(is_punct(tok, '&') ||is_punct(tok, '*') ||is_punct(tok, '+')\
            ||is_punct(tok, '-') ||is_punct(tok, '~') ||is_punct(tok, '!'))  {
        return ast_binop(tok->punct, cast_expr(), NULL);
    }
    
    unget_token(tok);
    return postfix_expr();
}

static Ast *postfix_expr()
{
    Ast *ast = primary_expr();
    token *tok = read_token();

    if(is_punct(tok, '[')) {
        ;
    }
    if(is_punct(tok, PUNCT_INC) || is_punct(tok, PUNCT_DEC)) {
        return ast_binop(tok->punct, ast, NULL);
    }
    if(is_punct(tok, '(')) {
        List *args = make_list();

        while(1) {
            list_push(args, assignment_expr());
            tok = peek_token();
            if(is_punct(tok, ','))
                expect(',');
            else if(is_punct(tok, ')'))
                break;
            else {
                error("next is ',' or ')'!");
            }
        }
        expect(')');
        ast->args = args;
        return ast;
    }

    unget_token(tok);
    return ast;
}

static Ast *primary_expr()
{
    token *tok = read_token();

    if(is_punct(tok, '(')) {
        Ast *ast = expr();
        expect(')');
        return ast;
    }
    if(tok->type == Num) {
        Ast *ast = new_ast();
        ast->type = AST_LITERAL;
        ast->ctype = ctype_int;
        ast->ival = atoi(tok->sval);
        return ast;
    }
    if(tok->type == Str) {
        int n = strlen(tok->sval);
        Ctype *r = malloc(sizeof(Ctype));
        *r = (Ctype){CTYPE_ARRAY, n, NULL};

        Ast *ast = new_ast();
        ast->type = AST_STRING;
        ast->ctype = r;
        ast->sval = tok->sval;
        return ast;

    }
    else if(tok->type == Id) {
        return ident_or_func(tok->sval);
    }
    else {
        error("primary_expr");
    }

}
static Ast *ident_or_func(char *id)
{
    Ast *ast = new_ast();

    if(is_punct(peek_token(), '(')) {
        /*
        while(1) {
            list_push(args, expr());
            tok = peek_token();
            if(is_punct(tok, ','))
                expect(',');
            else if(is_punct(tok, ')'))
                break;
            else {
                error("next is ',' or ')'!");
            }
        }
        expect(')');
        */
        ast->type = AST_FUNCALL;
        ast->fname = id;
        return ast;
    }
    ast->type = AST_LVAR;
    ast->ctype = ctype_int;
    ast->varname = id;
    return ast;
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
    tok = peek_token();
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
