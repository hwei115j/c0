#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "symbol.h"
#include "c0.h"

#define CR(STR, NUM) do{int _n = printf("%s", STR); for(int i = _n; i < NUM; i++) printf(" ");}while(0);
struct sym_obj *sym_global;
struct sym_obj *sym_local;
static int sp;
static int rc;

static struct symbol *struct_par(Ast *ast);
static int csize(Ctype *ctype)
{
    if(ctype == NULL)
        return 1;
    if(ctype->ptr != NULL) {
        ctype = ctype->ptr;
    }
    if(ctype->type == CTYPE_PTR) {
        return ctype->size;
    }
    else if(ctype->type == CTYPE_ARRAY) {
        if(ctype->ptr == NULL)
            return ctype->len * ctype->ptr->size;
        return ctype->len * csize(ctype);
    }
    else
        return 1;
}

static int get_size(Ctype *ctype)
{
    if(ctype->type == CTYPE_ARRAY)
        return ctype->len * get_size(ctype->ptr);
    else if(ctype->type == CTYPE_STRUCT)
        return ctype->offset;
    return ctype->size;
}

static char *mkstr(char *fmt, ...)
{
    va_list args;
    char *buf = malloc(sizeof(char)*100);

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    
    return buf;
}

static char *push_const(int n, Ctype *type)
{
    struct symbol *r;
    char *str = malloc(sizeof(char)*100);

    sprintf(str, "%d", n);
    if((r = sym_global->read(sym_global, str)) != NULL) {
        return r->name;
    } else {
        struct symbol r;
        r.name = str;
        r.data = atoi(str);
        if(type == NULL)
             r.type = (Ctype) {
            CTYPE_INT, 2, NULL
        };
        else
            r.type = *type;
        sym_global->add(sym_global, &r);
        return r.name;
    }
}

static void emit_func_body(Ast *ast);

static void SCR(char *sym, int n)
{
    static char state[100];
    if(n) {
        if(state[0]) {
            CR(state, n);
            state[0] = 0;
        } else {
            CR(sym, n);
        }
         return ;
    }
     strcpy(state, sym);
} 

static void emit(char *fmt, ...)
{
    char sym[100] = {}, com[100] = {}, par[100] = {}, si[100]= {};
    char buf[200];
    char *str = buf;
    int i;
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    for(i = 0; *str && !isspace(*str) && (sym[i] = *str++); i++);
    while(*str && isspace(*str)) str++;
    for(i = 0; *str && !isspace(*str) && (com[i] = *str++); i++);
    while(*str && isspace(*str)) str++;
    for(i = 0; *str && !isspace(*str) && (par[i] = *str++); i++);
    while(*str && isspace(*str)) str++;
    for(i = 0; *str && !isspace(*str) && (si[i] = *str++); i++);
    for(i = 0; sym[i]; i++);

    if(sym[0] == '.' && sym[1] == 'L' && sym[2] >= '0' && sym[2] <= '9') {
        SCR(sym, 0);
        return ;
    }
    SCR(sym, 16);
    CR(com, 8);
    CR(par, 16);
    CR(si, 1);
    printf("\n");
    
    if(strstr(par, ".PUSH") != NULL)
        sp--;
    if(strstr(par, ".POP") != NULL)
        sp++;
}

static void print_func()
{
    emit("MCIR, DEC 0");        //將.R2右移.R3個
    emit("      BUN ._L0");
    emit("._L1, LDA .R2");
    emit("      CIR");
    emit("      STA .R2");
    emit("._L0, ISZ .R3");
    emit("      BUN ._L1");
    emit("      BUN MCIR I");

    emit("MCIL, DEC 0");        //將.R2左移.R3個
    emit("      BUN ._L2");
    emit("._L3, LDA .R2");
    emit("      CIL");
    emit("      STA .R2");
    emit("._L2, ISZ .R3");
    emit("      BUN ._L3");
    emit("      BUN MCIL I");
    
    emit("out,  DEC 0");        //func out()
    emit("      BSA .GETF");
    emit("      BUN out I");
    emit("      LDA %s", push_const(3, NULL));
    emit("      BSA .OSET");
    emit("      OUT");
    emit("      BSA .RET");

    emit("in,   DEC 0");        //func in()
    emit("      BSA .GETF");
    emit("      BUN in I");
    emit("      INP");
    emit("      BSA .RET");

    emit("va_start, DEC 0");    //func va_start()
    emit("      BSA .GETF");
    emit("      BUN va_start I");
    emit("      LDA .BP");
    emit("      ADD 3");
    emit("      STA .R0");
    emit("      LDA .R0 I");
    emit("      ADD 4");
    emit("      BSA .RET");

    emit(".OSET,  DEC 0");      //AC <- mem[.BP+AC]
    emit("        ADD .BP");
    emit("        STA .R0");
    emit("        LDA .R0 I");
    emit("        BUN .OSET I");

    emit(".PUSH,  DEC 0");       //將AC .PUSH到mem[.SP--]
    emit("        STA .SP I");
    emit("        LDA .SP");
    emit("        ADD .N1");
    emit("        STA .SP");
    emit("        BUN .PUSH I");

    emit(".POP,   DEC 0");       //將mem[++.SP]讀到AC
    emit("        LDA .SP");
    emit("        INC");
    emit("        STA .SP");
    emit("        LDA .SP I");
    emit("        BUN .POP I");

    emit(".RET,   DEC 0");       //將return值存到AC，並返回上一函式
    emit("        STA .R1");
    emit("        LDA .BP");
    emit("        STA .SP");
    emit("        BSA .POP");
    emit("        STA .BP");
    emit("        BSA .POP");
    emit("        STA .R0");
    emit("        LDA .R1");
    emit("        BUN .R0 I");

    emit(".CALL,  DEC 0");       //呼叫AC儲存的函式位置
    emit("        STA .R0");
    emit("        LDA .CALL");
    emit("        BSA .PUSH");
    emit("        LDA .BP");
    emit("        BSA .PUSH");
    emit("        LDA .SP");
    emit("        STA .BP");
    emit("        BUN .R0 I");

    emit(".GETF,  DEC 0");       //取得函式的記憶體位置
    emit("        LDA .GETF");
    emit("        INC");
    emit("        BUN .GETF I");

    emit(".MUL,   DEC 0");      //乘法
    emit(".LOP,   CLE");
    emit("        LDA .R3");
    emit("        CIR");
    emit("        STA .R3");
    emit("        SZE");
    emit("        BUN .ONE");
    emit("        BUN .ZRO");
    emit(".ONE,   LDA .R2");
    emit("        ADD .R1");
    emit("        STA .R1");
    emit("        CLE");
    emit(".ZRO,   LDA .R2");
    emit("        CIL");
    emit("        STA .R2");
    emit("        ISZ .R4");
    emit("        BUN .LOP");
    emit("        BUN .MUL I");

    emit(".NUM,   DEC 48");
}

static void emit_data(struct symbol *reg)
{
    if(reg->type.type == CTYPE_ARRAY) {
        emit("%s,   DEC 0", reg->name);
        emit("      BSA .GETF");
        emit("      BUN %s I", reg->name);
        emit("      DEC %d", reg->str[0]);
        for(int i = 1; reg->str[i]; i++)
            emit("   DEC %d", reg->str[i]);
        emit("  DEC 0"); 
    }
    else
        emit("%s,   DEC %d", reg->name, reg->data);
}
static void print_global()
{
    emit(".SP,   DEC 4095");
    emit(".BP,   DEC 4095");
    emit(".R0,   DEC 0");
    emit(".R1,   DEC 0");
    emit(".R2,   DEC 0");
    emit(".R3,   DEC 0");
    emit(".R4,   DEC 0");
    emit(".N1,   DEC -1");
    emit(".N2,   DEC -2");

    //!!hack
    for (Iter i = list_iter(sym_global->symbol->dict); !iter_end(i);) {
        struct symbol *reg = ((struct __dict *)(iter_next(&i)))->data;
        emit_data(reg);
    }
}

static void emit_func_start(Ast *v)
{
    sym_local = sym_init();
    emit("%s, DEC 0 ", v->fname);
    emit("    BSA .GETF");
    emit("    BUN %s I", v->fname);
    int count = 3;
    int lsp = 0;
    for (Iter i = list_iter(v->args); !iter_end(i);) {
        Ast *a = iter_next(&i);
        emit_func_body(a);
        if(a->type != PUNCT_VLA) {
            emit("  LDA .BP");
            emit("  ADD %s", push_const(lsp--, NULL));
            emit("  STA .R1");
            emit("  LDA %s", push_const(count++, NULL));
            emit("  BSA .OSET");
            emit("  STA .R1 I");
        }
    }
}

static void emit_func_end(void)
{
    emit("  CMA");
    emit("  BSA .RET");
    sym_del(sym_local);
    sym_local = NULL;
    sp = 0;
}

static void lda_lvalue(Ast *ast)
{
    switch(ast->type) {
    case AST_LVAR: {
        struct symbol *r = sym_local->read(sym_local, ast->varname);
        if(strchr(ast->varname, '.')) {
            r = struct_par(ast);
        }
        emit("  LDA .BP");
        emit("  ADD %s", push_const(r->offset, NULL));
        emit("  BSA .PUSH");
        break;
    }
    case AST_DEREF: {
        lda_lvalue(ast->operand);
        if(ast->ctype->type == CTYPE_PTR) {
            emit("  BSA .POP");
            emit("  STA .R0");
            emit("  LDA .R0 I");
            emit("  BSA .PUSH");
        }
        break;
    }
    case TTYPE_PUNCT:
    case AST_LITERAL:
        emit_func_body(ast);
        break;
    default:
        error("lvalue!\n");
        break;
    }
}

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

static char *struct_next(char *name,int *len, int *flag)
{
    int i;

    if(!*name)
        return NULL;

    if(name[0] == '.') {
        *flag = '.';
        *name++;
    }
    else if(name[0] == '-' && name[1] == '>') {
        *flag = PUNCT_ARROW;
        *name++;
    }

    for(*len = 0; name[*len] != '.' && name[*len] != '-' && name[*len] != '\0'; (*len)++);

    return &name[*len];
}

static struct symbol *struct_par(Ast *ast)
{
    int flag, len = 0, i;
    char str[100];
    char *s = ast->varname;
    s = struct_next(s, &len ,&flag);

    for(i = 0; i < len; i++)
        str[i] = s[i-len];
    str[i] = '\0';

    struct symbol *r = sym_local->read(sym_local, str);
    struct sym_obj *l = to_symbol(r->type.dict);


    while(s = struct_next(s, &len, &flag)) {
        for(i = 0; i < len; i++)
            str[i] = s[i-len];
        str[i] = '\0';
        if(flag == '.') {
            if(r->type.type == CTYPE_STRUCT)
                l = to_symbol(r->type.dict);
            r = l->read(l, str);
            r->offset = r->type.offset;
        } else if(flag == PUNCT_ARROW) {
        }
        else {
            error("");
        }
    }

    //fprintf(stderr, "n: %s, t: %d, o: %d\n", r->name, r->type.type, r->type.offset);
    return r;
}
static void lda_rvalue(Ast *ast, int size)
{
    switch(ast->type) {
    case AST_LVAR: {
        struct symbol *r = NULL;

        if((r = sym_local->read(sym_local, ast->varname)) != NULL) {
            if(r->type.type == CTYPE_ARRAY) {
                emit("  LDA .BP");
                emit("  ADD %s", push_const(r->offset, NULL));
                emit("  BSA .PUSH");
            } else if(r->type.type == CTYPE_PTR) {
                emit("  LDA .BP");
                emit("  ADD %s", push_const(r->offset, NULL));
                emit("  STA .R0");
                emit("  LDA .R0 I");
                emit("  BSA .PUSH");
            } else {
                emit("  LDA %s", push_const(r->offset, NULL));
                emit("  BSA .OSET");
                if(size > 1) {
                    emit("        STA .R2");
                    emit("        LDA %s", push_const(size, NULL));
                    emit("        STA .R3");
                    emit("        CLA");
                    emit("        STA .R1");
                    emit("        LDA %s", push_const(-8, NULL));
                    emit("        STA .R4");
                    emit("        BSA .MUL");
                    emit("        LDA .R1");
                }
                emit("  BSA .PUSH");
            }
        } else if((r = sym_global->read(sym_global, ast->varname)) != NULL) {
            emit("  LDA %s", r->name);
            emit("  BSA .PUSH");
        } else
            error("error %s", ast->varname);
        break;
    }
    case AST_LITERAL: {
        emit("  LDA %s", push_const(ast->ival*size, ast->ctype));
        emit("  BSA .PUSH");
        break;
    }
    case AST_STRING: {
        static int count = 0;
    
        struct symbol r;
        r.name = mkstr(".s%d", count++);
        r.str = ast->sval;
        r.type = *ast->ctype; 
        sym_global->add(sym_global, &r);
        emit("  BSA %s", r.name);
        emit("  BSA .PUSH");
        break;
    }
    case AST_FUNCALL: {
        List *r = list_reverse(ast->args);
        int count = 0;
        for (Iter i = list_iter(r); !iter_end(i);) {
            Ast *v = iter_next(&i);
            emit_func_body(v);
            if(v) count++;
        }
        emit("  BSA %s", ast->fname);
        emit("  BSA .CALL");
        emit("  STA .R1");
        emit("  LDA .SP");
        emit("  ADD %s", push_const(count, NULL)); 
        emit("  STA .SP");
        emit("  LDA .R1");
        emit("  BSA .PUSH");
        sp++;
       // if(count) sp = sp+count-1;
        list_free(r);
        break;
    }
    case AST_DEREF: {
        lda_rvalue(ast->operand, csize(NULL));
        if(ast->ctype->type != CTYPE_ARRAY) {
            emit("  BSA .POP");
            emit("  STA .R0");
            emit("  LDA .R0 I");
            emit("  BSA .PUSH");
        }
        break;
    }
    case AST_ADDR: {
        if(ast->operand->type == AST_DEREF) {
            lda_rvalue(ast->operand->operand, csize(NULL));
        }
        else {
            struct symbol *r = sym_local->read(sym_local, ast->operand->varname);
            emit("  LDA .BP");
            emit("  ADD %s", push_const(r->offset, NULL));
            emit("  BSA .PUSH");
        }
        break;
    }
    case TTYPE_PUNCT:
    case AST_ASSIGNMENT_EXPR: {
        emit_func_body(ast);
        break;
    }
    default:
        error("rvalue_lda %d %c", ast->type, ast->ival);
        break;
    }
}


static void struct_add_symbol(struct symbol *sym)
{
    struct symbol o;
    struct symbol *r = &o;
    int lsp = sp;

    for (Iter i = list_iter(sym->type.dict->dict); !iter_end(i);) {
        struct __dict *dict = iter_next(&i);
        fprintf(stderr, "%s\n", dict->name);
        r->name = dict->name;
        r->type = *(Ctype *)dict->data;
        r->offset = lsp;
        sym_local->add(sym_local, r);
        if(r->type.type == CTYPE_STRUCT)
            struct_add_symbol(r);
        lsp += get_size(&r->type);
    }
}


static List *make_array_list(List *in, int max)
{
    List *out = make_list();

    for (Iter i = list_iter(in);!iter_end(i);) {
        Ast *v = iter_next(&i);
        if(v->type == AST_ARRAY_INIT) {
            List *r = make_array_list(v->arrayinit, max);

            for(Iter i = list_iter(r); !iter_end(i);)
                list_push(out, iter_next(&i));
        }
        else 
            list_push(out, v);
    }
    int n = list_len(out);

    if(n < max+1) {
        while(n++ < max+1)
            list_push(out ,NULL);
    }
    else {
        while(n-- > max+1)
            list_pop(out);
    }
    return out;
}
static void emit_func_body(Ast *ast)
{
    if(ast == NULL)
        return ;
    switch(ast->type) {
    case AST_COMPOUND_STMT: {
        struct sym_obj *reg = sym_local;
        sym_local = sym_local->clone(sym_local);

        for (Iter i = list_iter(ast->stmts); !iter_end(i);) {
            Ast *reg = iter_next(&i);
            if(reg->type == TTYPE_PUNCT)
                emit_func_body(reg);
            else
                emit_func_body(reg);
        }

        sym_del(sym_local);
        sym_local = reg;
        break;
    }
    case AST_ASSIGNMENT_EXPR: {
        for (Iter i = list_iter(ast->exprs); !iter_end(i);) {
            Ast *v = iter_next(&i);
            emit_func_body(v);
        }
        break;
    }
    case TTYPE_PUNCT: {
        switch(ast->ival) {
        case '=': {
            lda_rvalue(ast->right, csize(NULL));
            lda_lvalue(ast->left);
            emit("  BSA .POP");
            emit("  STA .R0");
            emit("  BSA .POP");
            emit("  STA .R0 I");
            break;
        }
        case '+': {
            if (ast->left > ast->right || ast->left->type == AST_STRING)
                swap(ast->left, ast->right);
            lda_rvalue(ast->left, csize(NULL));
            lda_rvalue(ast->right, csize(ast->left->ctype));
            emit("  BSA .POP");
            emit("  STA .R1");
            emit("  BSA .POP");
            emit("  ADD .R1");
            emit("  BSA .PUSH");
            break;
        }
        case '-': {
            if (ast->left > ast->right || ast->left->type == AST_STRING)
                swap(ast->left, ast->right);
            lda_rvalue(ast->left, csize(NULL));
            lda_rvalue(ast->right, csize(ast->left->ctype));
            emit("  BSA .POP");
            emit("  CMA");
            emit("  INC");
            emit("  STA .R1");
            emit("  BSA .POP");
            emit("  ADD .R1");
            emit("  BSA .PUSH");
            break;
        }
        case '*': {
            lda_rvalue(ast->left, csize(NULL));
            lda_rvalue(ast->right, csize(ast->left->ctype));
            emit("        BSA .POP");
            emit("        STA .R2");
            emit("        BSA .POP");
            emit("        STA .R3");
            emit("        CLA");
            emit("        STA .R1");
            emit("        LDA %s", push_const(-8, NULL));
            emit("        STA .R4");
            emit("        BSA .MUL");
            emit("        LDA .R1");
            emit("        BSA .PUSH");
            break;
        }
        case '/':
            error("no /");
            break;
        case PUNCT_CIR:
        case PUNCT_CIL:
        case PUNCT_DEC: {
            struct symbol *r;
            if(ast->right != NULL) {
                //++i, --i
                if(ast->right->type != AST_LVAR && ast->right->type != AST_GVAR)
                    error("not lvalue");
                if((r = sym_local->read(sym_local, ast->right->varname)) != NULL) {
                    emit("  LDA .BP");
                    emit("  ADD %s", push_const(r->offset, NULL));
                    emit("  STA .R0");
                    emit("  LDA .R0 I");
                    emit("  ADD .N1");
                    emit("  STA .R0 I");
                    emit("  LDA .R0 I");
                    emit("  BSA .PUSH");

                } else if((r = sym_global->read(sym_global, ast->right->varname)) != NULL) {
                    emit("  LDA %s", r->name);
                    emit("  ADD .N1");
                    emit("  STA %s", r->name);
                    emit("  LDA %s", r->name);
                    emit("  BSA .PUSH");
                } else
                    error("error");
            } else if(ast->left != NULL) {
                //i++, i--
                if(ast->left->type != AST_LVAR && ast->left->type != AST_GVAR)
                    error("lvar");
                if((r = sym_local->read(sym_local, ast->left->varname)) != NULL) {
                    emit("  LDA .BP");
                    emit("  ADD %s", push_const(r->offset, NULL));
                    emit("  STA .R0");
                    emit("  LDA .R0 I");
                    emit("  BSA .PUSH");
                    emit("  LDA .R0 I");
                    emit("  ADD .N1");
                    emit("  STA .R0 I");

                } else if((r = sym_global->read(sym_global, ast->left->varname)) != NULL) {
                    emit("  LDA %s", r->name);
                    emit("  BSA .PUSH");
                    emit("  LDA %s", r->name);
                    emit("  ADD .N1");
                    emit("  STA %s", r->name);
                } else
                    error("DEC error");
            } else
                error("DEC err");
            break;
        }
        case PUNCT_INC: {
            struct symbol *r;
            if(ast->right != NULL) {
                //++i, --i
                if(ast->right->type != AST_LVAR && ast->right->type != AST_GVAR)
                    error("not lvalue");
                if((r = sym_local->read(sym_local, ast->right->varname)) != NULL) {
                    emit("  LDA .BP");
                    emit("  ADD %s", push_const(r->offset, NULL));
                    emit("  STA .R0");
                    emit("  LDA .R0 I");
                    emit("  INC");
                    emit("  STA .R0 I");
                    emit("  LDA .R0 I");
                    emit("  BSA .PUSH");

                } else if((r = sym_global->read(sym_global, ast->right->varname)) != NULL) {
                    emit("  LDA %s", r->name);
                    emit("  INC");
                    emit("  STA %s", r->name);
                    emit("  LDA %s", r->name);
                    emit("  BSA .PUSH");
                } else
                    error("error");
            } else if(ast->left != NULL) {
                //i++, i--
                if(ast->left->type != AST_LVAR && ast->left->type != AST_GVAR)
                    error("lvar");
                if((r = sym_local->read(sym_local, ast->left->varname)) != NULL) {
                    emit("  LDA .BP");
                    emit("  ADD %s", push_const(r->offset, NULL));
                    emit("  STA .R0");
                    emit("  LDA .R0 I");
                    emit("  BSA .PUSH");
                    emit("  LDA .R0 I");
                    emit("  INC");
                    emit("  STA .R0 I");

                } else if((r = sym_global->read(sym_global, ast->left->varname)) != NULL) {
                    emit("  LDA %s", r->name);
                    emit("  BSA .PUSH");
                    emit("  LDA %s", r->name);
                    emit("  INC");
                    emit("  STA %s", r->name);
                } else
                    error("INC error");
            } else
                error("INC err");
            break;
        }
        case PUNCT_EQ: {
            if(ast->left != NULL && ast->right != NULL) {
                lda_rvalue(ast->left, csize(NULL));
                lda_rvalue(ast->right, csize(NULL));
                emit("  BSA .POP");
                emit("  STA .R1");
                emit("  BSA .POP");
                emit("  CMA");
                emit("  INC");
                emit("  ADD .R1");
                emit("  CLE");
                emit("  SZA");
                emit("  CME");
                emit("  CME");
                emit("  CLA");
                emit("  CIL");
                emit("  BSA .PUSH");
            } else
                error("err EQ");
            break;
        }
        case PUNCT_NE: {
            if(ast->left != NULL && ast->right != NULL) {
                lda_rvalue(ast->left, csize(NULL));
                lda_rvalue(ast->right, csize(NULL));
                emit("  BSA .POP");
                emit("  STA .R1");
                emit("  BSA .POP"); 
                emit("  CMA");
                emit("  INC");
                emit("  ADD .R1");
                emit("  CLE");
                emit("  SZA");
                emit("  CME");
                emit("  CME");
                emit("  CLA");
                emit("  CIL");
                emit("  SPA");
                emit("  CME");
                emit("  CME");
                emit("  CME");
                emit("  CLA");
                emit("  CIL");
                emit("  BSA .PUSH");
            } else
                error("err NE");
            break;
        }
        case '!': {
            lda_rvalue(ast->left, csize(NULL));
            emit("  BSA .POP");
            emit("  CLE");
            emit("  SZA");
            emit("  CME");
            emit("  CME");
            emit("  CLA");
            emit("  CIR");
            emit("  BSA .PUSH");
            break;
        }
        case '<':
        case '>': {
            if(ast->left != NULL && ast->right != NULL) {
                lda_rvalue(ast->left, csize(NULL));
                lda_rvalue(ast->right, csize(NULL));
                emit("  BSA .POP");
                emit("  CMA");
                emit("  INC");
                emit("  STA .R1");
                emit("  BSA .POP");
                emit("  ADD .R1");
                emit("  CLE");
                if(ast->ival == '<')
                    emit("  SNA");
                else
                    emit("  SPA");
                emit("  CME");
                emit("  CME");
                emit("  CLA");
                emit("  CIL");
                emit("  BSA .PUSH");
            } else 
                 error("err >");
            break;
        }
        case PUNCT_LOGAND: {
            int r = rc;
            rc += 2;

            lda_rvalue(ast->left, csize(NULL));
            emit("  BSA .POP");
            emit("  SPA");
            emit("  BUN .L%d", r);
            lda_rvalue(ast->right, csize(NULL));
            emit("  BUN .L%d", r+1);
            emit(".L%d,  NOP", r);
            emit("  LDA  %s", push_const(0, NULL));
            emit("  BSA .PUSH");
            emit(".L%d,  NOP", r+1);
            break;
        }
        case PUNCT_LOGOR: {
            int r = rc;
            rc += 2;

            lda_rvalue(ast->left, csize(NULL));
            emit("  BSA .POP");
            emit("  SZA");
            emit("  BUN .L%d", r);
            lda_rvalue(ast->right, csize(NULL));
            emit("  BUN .L%d", r+1);
            emit(".L%d,  NOP", r);
            emit("  LDA  %s", push_const(1, NULL));
            emit("  BSA .PUSH");
            emit(".L%d,  NOP", r+1);
            break;
        }
        default: 
            error("%c", ast->ival);
        }
    break;
                      }
    case AST_LITERAL:
        lda_rvalue(ast, csize(NULL));
        break;
    case AST_STRING:
        lda_rvalue(ast, csize(NULL));
        break;
    case AST_LVAR:
        lda_rvalue(ast, csize(NULL));
        break;
    case AST_FUNCALL:
        lda_rvalue(ast, csize(NULL));
        break;
    case AST_FUNC:
        break;
    case AST_DECL: {
        struct symbol o;
        struct symbol *r = sym_local->read(sym_local, ast->declvar->varname);

        if(r == NULL)
            r = &o;
        r->name = ast->declvar->varname;
        r->type = *ast->declvar->ctype;
        r->offset = sp;

        if(ast->declvar->ctype->type == CTYPE_ARRAY || ast->declvar->ctype->type == CTYPE_STRUCT) {
            sp -= atoi(push_const(get_size(ast->declvar->ctype), NULL));
            r->offset = sp + 1;
            if(ast->declinit == NULL) {
                emit("  LDA %s", push_const(get_size(ast->declvar->ctype), NULL));
                emit("  CMA");
                emit("  INC");
                emit("  ADD .SP");
                emit("  STA .SP");
            } else {
                if(ast->declvar->ctype->type == CTYPE_ARRAY) {
                    List *r_list = make_array_list(ast->declinit->arrayinit, -r->offset);        
                    int count = 0;

                    for (Iter i = list_iter(list_reverse(r_list));!iter_end(i); count++) {
                        Ast *v = iter_next(&i);
                        if(v) 
                            emit_func_body(v);
                        else {
                            emit("  CLA");
                            emit("  BSA .PUSH");
                        }
                    }
                    /*
                    for(int i = count; i < -r->offset; i++) {
                        emit("  CLA");
                        emit("  BSA .PUSH");
                    }
                    */
                }
                sp = r->offset-1;
            }
        } else if(ast->declinit == NULL) {
            emit("  CLA");
            emit("  BSA .PUSH");
        } else {
            if(ast->declinit->type == AST_FUNCALL) {
                //emit("  BSA .PUSH");
                emit_func_body(ast->declinit);
                //emit(" BSA .PUSH");
                /*
                emit("  LDA .BP");
                emit("  ADD %s", push_const(r->offset, NULL));
                emit("  STA .R0"); 
                emit("  BSA .POP");
                emit("  STA .R0 I");
                */
            } else if(ast->declinit->type == AST_DEREF) {
                emit_func_body(ast->declinit);
                //lda_rvalue(ast->declinit, 1);
                //emit("  BSA .PUSH");
            } else {
                emit_func_body(ast->declinit);
            }
        }
        if(r == &o)
            sym_local->add(sym_local, r);
        break;
    }
    case AST_ARRAY_INIT: {
        break;
    }
    case AST_ADDR: {
        lda_rvalue(ast, csize(NULL));
        /*
        emit("  LDA .BP");
        emit("  ADD %s", push_const(sym_local->read(sym_local, ast->operand->varname)->offset, NULL));
        emit("  BSA .PUSH");
        */
        break;
    }
    case AST_DEREF: {
        lda_rvalue(ast, csize(NULL));
        break;
    }
    case AST_IF: {
        int r = rc;
        rc+=2;
        emit_func_body(ast->cond);
        emit("  BSA .POP");
        emit("  SZA");
        emit("  BUN .L%d", r);
        emit_func_body(ast->els);
        emit("  BUN .L%d", r+1);
        emit(".L%d, NOP", r);
        emit_func_body(ast->then);
        emit(".L%d, NOP", r+1);
        break;
    }
    case AST_TERNARY:
        break;
    case AST_FOR: {
        int r = rc;
        rc+=2;
        emit_func_body(ast->forinit);
        emit("  BUN .L%d", r);
        emit(".L%d, NOP", r+1);
        emit_func_body(ast->forbody);
        emit_func_body(ast->forstep);
        emit(".L%d, NOP", r);
        emit_func_body(ast->forcond);
        emit("  BSA .POP");
        emit("  SZA");
        emit("  BUN .L%d", r+1);
        break;
    }
    case AST_WHILE: {
        int r = rc;
        rc+=2;
        emit("  BUN .L%d", r);
        emit(".L%d, NOP", r+1);
        emit_func_body(ast->whilebody);
        emit(".L%d, NOP", r);
        emit_func_body(ast->whilecond);
        emit("  BSA .POP");
        emit("  SZA");
        emit("  BUN .L%d", r+1);
        break;
    }
    case AST_RETURN: {
        emit_func_body(ast->retval);
        emit("  BSA .POP");
        emit("  BSA .RET");
        break;
    }
    case AST_STRUCT_REF:
        break;
    case PUNCT_VLA: {
        break;
    }
    }
}
static void p_ast(Ast *v)
{
    if(v->type == AST_FUNC) {
        emit_func_start(v);
        emit_func_body(v->body);
        emit_func_end();
    } else if (v->type == AST_DECL) {
        struct symbol o;
        struct symbol *r = sym_global->read(sym_global, v->declvar->varname);

        if(r == NULL)
            r = &o;
        r->name = v->declvar->varname;
        r->type = *v->declvar->ctype;

        if(r == &o) {
            if(v->declinit == NULL) {
                r->data = 0;
            } else if(v->declinit->type == AST_LITERAL) {
                r->data = v->declinit->ival;
            } else
                error("error");
            sym_global->add(sym_global, r);
        } else
            error("error");
    } else
        error("error");
}

static void p_list(List *list)
{
    sym_global = sym_init();

    emit("  BSA main");
    emit("  BSA .CALL");
    emit("  HLT");

    for (Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
    emit("  ORG 2000");
    print_func();
    print_global();
    emit("  END");

    sym_del(sym_global);
}

int main()
{
    p_list(statement());
}

