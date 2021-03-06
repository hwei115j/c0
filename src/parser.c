#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "symbol.h"
#include "c0.h"
#define expect(STR) pfexpect(STR, __LINE__)

static Ctype *ctype_int = &(Ctype){CTYPE_INT, 1, NULL};
static Ctype *ctype_void = &(Ctype){CTYPE_VOID, 1, NULL};
static Ctype *ctype_long = &(Ctype){CTYPE_LONG, 2, NULL};
static Ctype *ctype_char = &(Ctype){CTYPE_CHAR, 1, NULL};
static Ctype *ctype_float = &(Ctype){CTYPE_FLOAT, 4, NULL};
static Ctype *ctype_double = &(Ctype){CTYPE_DOUBLE, 8, NULL};

static struct sym_obj *gctype = NULL;
static struct sym_obj *lctype = NULL;
static struct dict *struct_sym = NULL;
static char *struct_name;

static char *gname;

static struct sym_obj *to_symbol(struct dict *struct_sym)
{
    struct sym_obj *sym = sym_init();
   
    for (Iter i = list_iter(struct_sym->dict); !iter_end(i);) {
        struct __dict *r = iter_next(&i);
        struct symbol *a = malloc(sizeof(struct symbol));
        a->name = r->name;
        a->type = *((Ctype *)r->data);
        sym->add(sym, a);
    }

    return sym;
}

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
        
//        error("The next token is '%c' (tok = %c) ", punct, tok->punct);
        sprintf(str, "The next token is '%c' (tok = %c) ", punct, tok->punct);
        pferror(__FILE__, line, str);
    }
}
static Ctype *get_ctype(token *);
static int priority(token *);
static int is_type_keyword(token *);
static Ast *stmt_expr(int);
static Ast *unary_expr();
static Ast *ast_binop(int, Ast *, Ast *);
static Ast *ast_uop(int, Ast *);
static Ast *expr();
static Ast *decl_or_func();
static Ast *func_def(Ctype *, char *);
static Ast *compound_stmt();
static Ast *stmt();
static Ast *decl_or_stmt();
static Ast *ast_compound_stmt(List *);
static Ast *ast_var(Ctype *, char *, int);
static Ast *decl_init(Ast *);
static Ast *ast_decl(Ast *, Ast *);
static Ast *selection_stmt();
static Ast *ret_stmt();
static Ast *ident_or_func(char *);
static Ast *iteration_stmt();
static Ast *while_stmt();
//expr
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
static Ast *unary_expr();
static int is_assig_opt(token *);

//decl
static Ast *declaration();
static Ast *decl();
static void declarator(List *list);
static Ctype *pointer(Ctype *ctype);
static char *getype(Ctype *);
//struct
static Ctype *read_struct();
static Ctype *read_union();

static int calc()
{
    token *tok = read_token();
    if(tok->type != Num)
        error("");
    return atoi(tok->sval);
}

//static Ast *declaration()
static void dectype(Ctype *ctype)
{
    if(ctype->type == CTYPE_ARRAY)
        fprintf(stderr, "arr, size = %d\n", ctype->size);
    else if(ctype->type == CTYPE_PTR)
        fprintf(stderr, "ptr, size = %d\n", ctype->size);
    else if(ctype->type == CTYPE_INT)
        fprintf(stderr, "int, size = %d\n", ctype->size);
    else if(ctype->type == CTYPE_CHAR)
        fprintf(stderr, "char, size = %d\n", ctype->size);
    if(ctype->ptr != NULL)
        dectype(ctype->ptr);
}

static Ctype *create_ctype(List *list, Ctype *ctype)
{
    list = list_reverse(list);
    for (Iter i = list_iter(list); !iter_end(i);) {
        Ctype *r = iter_next(&i);
        r->ptr = ctype;
        ctype = r;
    }

    return ctype;
}

static Ctype *is_struct(token *tok)
{
    if (!strcmp(tok->sval, "struct"))
        return read_struct();
    if (!strcmp(tok->sval, "union"))
        return read_union();
    return NULL;
}

static Ast *decl()
{
    token *tok = read_token();
    List *list = make_list();
    Ctype *ctype = is_struct(tok);
    ctype = ctype?ctype:get_ctype(tok);
    char *s = malloc(sizeof(char) * 100);

    if(ctype == NULL) 
        error("next token is type specifier\n");
    declarator(list);
    ctype = create_ctype(list, ctype);
 
    Ast *var = ast_var(ctype, gname, AST_LVAR);
    lctype->add(lctype, &(struct symbol){.name = strcpy(s, gname), .type = *ctype});
     
    return decl_init(var);
}

static void declarator(List *list)
{
    List *l = make_list();
    token *tok = read_token();

    while(is_punct(tok, '*')) {
        Ctype *r = malloc(sizeof(Ctype));
        r->type = CTYPE_PTR;
        r->size = 1;
        list_push(l, r);
        tok = read_token();
    }

    if(tok->type == Id) {
        gname = tok->sval;
        tok = read_token();
    }
    else if(is_punct(tok, '(')) {
        declarator(list);
        expect(')');
        tok = read_token();
    }

    while(is_punct(tok, '[')) {
        int n = calc();
        expect(']');
        Ctype *r = malloc(sizeof(Ctype));
        r->type = CTYPE_ARRAY;
        r->size = 2;
        r->len = n;
        list_push(list, r);
        tok = read_token();
    }
    if(is_punct(tok, '(')) {
        error("function not implemented yet;;\n");
        //parameter-type-list
        expect(')');
    }
    else 
        unget_token(tok);
    
    l = list_reverse(l);
    for(void *I; (I = list_pop(l)) != NULL;)
        list_push(list, I);
    return ;
}

static Ctype *pointer(Ctype *ctype)
{
    token *tok = read_token();

    while(is_punct(tok, '*')) {
        Ctype *r = malloc(sizeof(Ctype));
        r->type = CTYPE_PTR;
        r->size = 1;
        r->ptr = ctype;
        ctype = r;
        tok = read_token();
    }

    unget_token(tok);
    return ctype;
}

static int is_assig_opt(token *tok)
{
    if(is_punct(tok, '=') || is_punct(tok, PUNCT_NE) || is_punct(tok, PUNCT_GE) || is_punct(tok, PUNCT_LE))
        return 1;
    return 0;
}
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
    Iter i = list_iter(exprs);
    Ast *r = iter_next(&i);
    
    unget_token(tok);
    ast = new_ast();
    ast->ctype = r->ctype;
    
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
        Ast *r = multiplicative_expr();
        ast = ast_binop(tok->punct, ast, r);
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
    gctype = sym_init();
    gctype->add(gctype, &(struct symbol){.name = "out", .type = *ctype_int});
    gctype->add(gctype, &(struct symbol){.name = "in", .type = *ctype_int});
    gctype->add(gctype, &(struct symbol){.name = "va_start", .type = *ctype_int});
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
        return ast_binop(tok->punct, unary_expr(), NULL);
    }
    else if(is_punct(tok, '*')) {
        return ast_uop(AST_DEREF, cast_expr());
    }
    else if(is_punct(tok, '&')) {
        return ast_uop(AST_ADDR, cast_expr());
    }
    else if(is_punct(tok, '!')) {
        return ast_binop(tok->punct, cast_expr(), NULL);
    }
    else if(is_punct(tok, '+') || is_punct(tok, '-') ||is_punct(tok, '~'))  {
        error("");
        return ast_uop(tok->punct, cast_expr());
    }
    
    unget_token(tok);
    return postfix_expr();
}

static Ast *postfix_expr()
{
    Ast *ast = primary_expr();
    token *tok;
    char *id = malloc(sizeof(char) * 100);
    char *s = id;
    int flag = 0;

    while(1) {
        tok = read_token();
    
        if(is_punct(tok, PUNCT_ARROW))
            fprintf(stderr, "ARROW\n");
        if(is_punct(tok, '[')) {
            Iter i = list_iter(expr()->exprs);
            Ast *r = iter_next(&i);
            expect(']');
            ast = ast_binop('+', ast, r);
            ast = ast_uop(AST_DEREF, ast);
        }
        else if(is_punct(tok, PUNCT_INC) || is_punct(tok, PUNCT_DEC)) {
            ast = ast_binop(tok->punct, ast, NULL);
        }
        else if(is_punct(tok, '(')) {
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
        }
        else if(is_punct(tok, '.')) {
            tok = read_token();
            if(tok->type == Id) {
                struct sym_obj *r = lctype;  

                if(!flag) {
                    s += sprintf(s, "%s", struct_name);
                    flag = 1;
                }
                lctype = to_symbol(struct_sym);
                s += sprintf(s, ".%s", tok->sval);
                ast = ident_or_func(id);
                lctype = r;
            }
            else
                error("error next is not ID %c", tok->ch);
            
        }
        else if(is_punct(tok, PUNCT_ARROW)) {
            tok = read_token();
            if(tok->type == Id) {
            
            }
            error("->");
        }
        else {
            unget_token(tok);
            break;
        }
    }
    return ast;
}

static Ast *primary_expr()
{
    token *tok = read_token();

    if(is_punct(tok,'(')) {
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
    unget_token(tok);
    return NULL;

}
static Ast *ident_or_func(char *id)
{
    Ast *ast = new_ast();
    struct symbol *r = lctype->read(lctype, id);

    if(strchr(id, '.')) {
        char name[100];
        char *tok, *reg;

        strcpy(name, id);
        tok = strtok(name, ".");
   
        while(tok){
            reg = tok;
            tok = strtok(NULL, ".");
        }
        r = lctype->read(lctype, reg);
    }

    if(r == NULL && (r = gctype->read(gctype, id)) == NULL) {
        error("not decl %s", id);
    }
    if(is_punct(peek_token(), '(')) {
        ast->ctype = &r->type;
        ast->type = AST_FUNCALL;
        ast->fname = id;
        return ast;
    }
    if(r->type.type == CTYPE_STRUCT) {
        struct_sym = r->type.dict;
        struct_name = r->name;
    }
    if(r->type.type == CTYPE_PTR && r->type.ptr->type == CTYPE_STRUCT) {
        struct_sym = r->type.ptr->dict;
        struct_name = r->name;
    }
    ast->type = AST_LVAR;
    ast->ctype = &r->type;
    ast->varname = id;

    return ast;
}

static Ast *ast_uop(int type, Ast *u)
{
    Ast *ast = new_ast();

    ast->type = type;
    ast->operand = u;
    ast->ctype = u->ctype;

    return ast;
}

static bool is_inttype(Ctype *ctype)
{
    return ctype->type == CTYPE_CHAR || ctype->type == CTYPE_INT ||
           ctype->type == CTYPE_LONG;
}

static Ctype *result_type(jmp_buf *jmpbuf, int op, Ctype *a, Ctype *b)
{
    if (a->type > b->type) {
        swap(a, b);
    }
    if(a == NULL || b == NULL)
        error("ctype is NULL a=%p b=%p\n", a, b);
    if (b->type == CTYPE_PTR) {
        if (op == '=')
            return a;
        /*
        if (op != '+' && op != '-')
            goto err;
            */
        if (!is_inttype(a))
            goto err;
        return b;
    }
    switch (a->type) {
    case CTYPE_VOID:    
        goto err;
    case CTYPE_CHAR:
    case CTYPE_INT:
        switch (b->type) {
        case CTYPE_CHAR:
        case CTYPE_INT:
            return ctype_int;
        case CTYPE_LONG:
            return ctype_long;
        case CTYPE_FLOAT:
        case CTYPE_DOUBLE:
            return ctype_double;
        case CTYPE_ARRAY:
        case CTYPE_PTR:
            return b->ptr;
        }
        error("internal error");
    case CTYPE_LONG:
        switch (b->type) {
        case CTYPE_LONG:
            return ctype_long;
        case CTYPE_FLOAT:
        case CTYPE_DOUBLE:
            return ctype_double;
        case CTYPE_ARRAY:
        case CTYPE_PTR:
            return b->ptr;
        }
        error("internal error");
    case CTYPE_FLOAT:
        if (b->type == CTYPE_FLOAT || b->type == CTYPE_DOUBLE)
            return ctype_double;
        goto err;
    case CTYPE_DOUBLE:
        if (b->type == CTYPE_DOUBLE)
            return ctype_double;
        goto err;
    case CTYPE_ARRAY:
        if (b->type != CTYPE_ARRAY) {
            goto err;
        }
        return result_type(jmpbuf, op, a->ptr, b->ptr);
    default:
        error("internal error: %s %s", getype(a), getype(b));
    }
err:
    longjmp(*jmpbuf, 1);
}


static Ast *ast_binop(int punct, Ast *left, Ast *right)
{
    Ast *ast = new_ast();
    jmp_buf jmpbuf;

    ast->type = TTYPE_PUNCT;
    ast->ival = punct;
    ast->left = left;
    ast->right = right;

    if(left == NULL) {
        ast->ctype = right->ctype;
        return ast;
    }
    if(right == NULL) {
        ast->ctype = left->ctype;
        return ast;
    }
    if (setjmp(jmpbuf) == 0) {
        ast->ctype = result_type(&jmpbuf, punct, left->ctype, right->ctype);
    } else {
        fprintf(stderr, "err: %d %d\n", left->ctype->type, right->ctype->type);
        error("%c %d", punct, left->type);
    }
    if(left->ctype->type < right->ctype->type)
        swap(left, right);
    if(punct != '=' && left->ctype->type == CTYPE_PTR && right->ctype->type == CTYPE_INT)
        ast->ctype = left->ctype->ptr;
    return ast;
}

static Ast *decl_or_func()
{
    token *tok = read_token();
    char *s = malloc(sizeof(char) * 100);

    if(!tok) 
        return NULL;
    Ctype *ctype = get_ctype(tok);
    token *name  = read_token();
    if(name->type != Id)
        error("next is not Id");
    tok = peek_token();
    if(is_punct(tok, '=') || is_punct(tok, ';')) {
        gctype->add(gctype, &(struct symbol){.name = strcpy(s, name->sval), .type = *ctype});
        Ast *var = ast_var(ctype, name->sval, AST_GVAR);
        return decl_init(var);
    }
    if(is_punct(tok, '(')) {
        gctype->add(gctype, &(struct symbol){.name = strcpy(s, name->sval), .type = *ctype});
        return func_def(ctype, name->sval); 
    }
    expect(';');
    error("null\n");
    return NULL;
}

static Ast *func_def(Ctype *ctype, char *name)
{
    List *args = make_list();
    lctype = sym_init();

    expect('(');

    token *tok = peek_token();
    while(!is_punct(tok, ')')) {
        list_push(args, decl());
        tok = peek_token();
        if(is_punct(tok, PUNCT_VLA)) {
            //不定引數 '...'
            list_push(args, ast_var(NULL, NULL, PUNCT_VLA));
            tok = read_token();
            tok = peek_token();
        }
    }

    expect(')');
    expect('{');

    Ast *func = new_ast();
    func->type = AST_FUNC;
    func->ctype = ctype;
    func->fname = name;
    func->args = args;
    func->body = compound_stmt();

    //sym_del(lctype);

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
static Ctype *read_union()
{
    return read_struct();
}
static int get_size(Ctype *ctype)
{
    if(ctype->type == CTYPE_ARRAY)
        return ctype->len * get_size(ctype->ptr);
    else if(ctype->type == CTYPE_STRUCT)
        return ctype->offset;
    return ctype->size;
}

static Ast *s_decl()
{
    token *tok = read_token();
    List *list = make_list();
    Ctype *ctype = is_struct(tok);
    ctype = ctype?ctype:get_ctype(tok);
    char *s = malloc(sizeof(char) * 100);

    if(ctype == NULL) 
        error("next token is type specifier\n");
    declarator(list);
    ctype = create_ctype(list, ctype);
 
    Ast *var = ast_var(ctype, gname, AST_LVAR);
     
    return decl_init(var);
}

static Ctype *read_struct()
{
    token *tok = read_token();
    Ctype *ctype = malloc(sizeof(Ctype));
    ctype->type = CTYPE_STRUCT;
    ctype->dict = dict_init();
    ctype->offset = 0;

    if(tok->type == Id) {
        expect('{');
        struct sym_obj *r = lctype;

        lctype = sym_init();
        while(!is_punct(tok, '}')) {
            Ast *at = s_decl()->declvar;
            at->ctype->offset = ctype->offset;
            //fprintf(stderr, "p: %d\n", at->ctype->offset);
            dict_add(ctype->dict, at->varname, at->ctype, sizeof(*at->ctype));
            tok = peek_token();
            ctype->offset += get_size(at->ctype);
        }
        expect('}');
        lctype = r;
    } else if(is_punct(tok, '{')){
        expect('}');
    } else 
        error("struct error");

    return ctype;       
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
    if (!strcmp(tok->sval, "void"))
        return ctype_void;
    if (!strcmp(tok->sval, "float"))
        return ctype_float;
    if (!strcmp(tok->sval, "double"))
        return ctype_double;
    if (!strcmp(tok->sval, "struct"))
        return (void*)1;
    if (!strcmp(tok->sval, "union"))
        return (void*)1;
    return NULL;
}

static int is_type_keyword(token *tok)
{
    return !!get_ctype(tok);
}
#if 0
static Ast *decl()
{
    token *tok   = read_token();
    Ctype *ctype = get_ctype(tok);
    if(ctype == NULL) error("next token is type specifier\n");
    ctype = declarator(ctype);
    token *name  = read_token();
    if(name->type != Id) error("next token is not Id\n");
    Ast *var = ast_var(ctype, name->sval, AST_LVAR);

    return decl_init(var);
    
    /*
    token *tok   = read_token();
    Ctype *ctype = get_ctype(tok);
    token *name  = read_token();
    if(name->type != Id) error("next token is not Id\n");
    Ast *var = ast_var(ctype, name->sval, AST_LVAR);

    return decl_init(var);
    */
}
#endif
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
static Ast *initializer_list()
{
    Ast *init = new_ast();
    token *tok = read_token();
    init->type = AST_ARRAY_INIT;
    init->arrayinit = make_list();
  
    if(is_punct(tok, '{')) 
        list_push(init->arrayinit, initializer_list());
    else {
        unget_token(tok);
        list_push(init->arrayinit, assignment_expr());
    }
    tok = read_token();
    while(is_punct(tok, ',')) {
        if(is_punct(peek_token(), '{')) {
            expect('{');
            list_push(init->arrayinit, initializer_list());
        }
        else
            list_push(init->arrayinit, assignment_expr());
        tok = read_token();
    }

    unget_token(tok);
    expect('}');

    return init;
}
static Ast *decl_init(Ast *var)
{
    token *tok = read_token();

    if(is_punct(tok, ',') || is_punct(tok, ';')) {
        //unget_token(tok);
        return ast_decl(var, NULL);
    }
    if(is_punct(tok, ')')) {
        unget_token(tok);
        return ast_decl(var, NULL);
    }
    if(is_punct(tok, '=')) {
        Ast *init = NULL;
        if(is_punct(tok = read_token(), '{')) {
            init = initializer_list();
        }
        else {
            unget_token(tok);
            init = assignment_expr();
        }
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
