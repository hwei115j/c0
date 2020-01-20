#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "asm.h"
#include "c0.h"

#define CR(STR, NUM) do{int _n = printf("%s", STR); for(int i = _n; i < NUM; i++) printf(" ");}while(0);
#define error(STR) pferror(STR, __LINE__)
static void pferror(char *s, int line)
{
    fprintf(stderr, "nasm.c %d:%s error!\n",line, s);
    exit(1);
}

struct sym_obj *sym_global;
struct sym_obj *sym_local;
static int sp;
static int rc;

static char *push_const(int n, Ctype *type)
{
    struct symbol *r;
    char *str = malloc(sizeof(char)*100);

    sprintf(str, "%d", n);
    if((r = sym_global->read(sym_global, str)) != NULL) {
        return r->name;
    }
    else {
        struct symbol r;
        r.name = str;
        r.data = atoi(str);
        if(type == NULL)
            r.type = (Ctype){CTYPE_INT, 2, NULL};
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
        }
        else
            CR(sym, n);
        return ;
    }
    strcpy(state, sym);
}

static void emit(char *fmt, ...)
{
    char sym[100] = {}, com[100] = {}, par[100] = {}, si[100]={};
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

    if(sym[0] == '.' && sym[1] == 'R') {
        SCR(sym, 0);
        return ;
    }
    SCR(sym, 8);
    CR(com, 8);
    CR(par, 8);
    CR(si, 1);
    printf("\n");
}

static const char *getype(int type)
{
    switch(type) {
        case CTYPE_VOID:
            return "void";
        case CTYPE_CHAR:
            return "char";
        case CTYPE_INT:
            return "int";
        case CTYPE_LONG:
            return "long";
        case CTYPE_FLOAT:
            return "float";
        case CTYPE_DOUBLE:
            return "double";
        case CTYPE_ARRAY:
            return "array";
        case CTYPE_PTR:
            return "prt";
        case CTYPE_STRUCT:
            return "struct";
    }
}
static void print_func()
{
    emit("out,  DEC 0");
    emit("      BSA GETF");
    emit("      BUN out I");
    emit("      LDA a");
    emit("      OUT");
    emit("      BSA RET");

    emit("OSET,   DEC 0");      //AC <- mem[BP+AC]
    emit("        ADD BP");
    emit("        STA R0");
    emit("        LDA R0 I");
    emit("        BUN OSET I");

    emit("PUSH,   DEC 0");       //將AC PUSH到mem[SP--]
    emit("        STA SP I");
    emit("        LDA SP");
    emit("        ADD N1");
    emit("        STA SP");
    emit("        BUN PUSH I");

    emit("POP,    DEC 0");       //將mem[++SP]讀到AC
    emit("        LDA SP");
    emit("        INC");
    emit("        STA SP");
    emit("        LDA SP I");
    emit("        BUN POP I");
/*
    emit("SAVE,   DEC 0");
    emit("        BSA PUSH");
    emit("        LDA BP");
    emit("        BSA PUSH");
    emit("        LDA SP");
    emit("        STA BP");
    emit("        BUN SAVE I");
*/

    emit("RET,    DEC 0");       //將return值存到AC，並返回上一函式
    emit("        STA R1");
    emit("        LDA BP");
    emit("        STA SP");
    emit("        BSA POP");
    emit("        STA BP");
    emit("        BSA POP");
    emit("        STA R0");
    emit("        LDA R1");
    emit("        BUN R0 I");

    emit("CALL,   DEC 0");       //呼叫AC儲存的函式位置
    emit("        STA R0");
    emit("        LDA CALL");
    emit("        BSA PUSH");
    emit("        LDA BP");
    emit("        BSA PUSH");
    emit("        LDA SP");
    emit("        STA BP");
    emit("        BUN R0 I");

    emit("GETF,   DEC 0");       //取得函式的記憶體位置
    emit("        LDA GETF");
    emit("        INC");
    emit("        BUN GETF I");

    emit("NUM,   DEC 48");
}

static void print_global()
{
    emit("SP,   DEC 3999");   
    emit("BP,   DEC 3999");   
    emit("R0,   DEC 0");
    emit("R1,   DEC 0");
    emit("N1,   DEC -1");

    for (Iter i = list_iter(sym_global->symbol); !iter_end(i);) {
        struct symbol *reg = iter_next(&i);
        emit("%s,   DEC %d", reg->name, reg->data);
    }
}

static void emit_func_start(Ast *v)
{
    emit("%s, DEC 0 ", v->fname);
    emit("    BSA GETF");
    emit("    BUN %s I", v->fname);
    
}
static void emit_func_end(void)
{
    emit("   BSA RET");
}
static void lda(Ast *ast)
{
    if(ast->type == TTYPE_PUNCT) {
        emit_func_body(ast);
    }
    else if(ast->type == AST_LITERAL){
        emit("  LDA %s", push_const(ast->ival, ast->ctype));
        emit("  BSA PUSH");
        sp--;
    }
    else if(ast->type == AST_LVAR) {
        struct symbol *r;
        if((r = sym_local->read(sym_local, ast->varname)) != NULL) {
            emit("  LDA %s", push_const(r->offset, NULL));
            emit("  BSA OSET");
            /*
            emit("  LDA BP");
            emit("  ADD %s", push_const(r->offset, NULL));
            emit("  STA R0");
            emit("  LDA R0 I");
            */
            emit("  BSA PUSH");
            sp--;
        }
        else if((r = sym_global->read(sym_global, ast->varname)) != NULL) {
            emit("  LDA %s", r->name);
            emit("  BSA PUSH"); 
            sp--;
        }
        else {
            error("error");
        }
    }
    else {
        error("error");
    }
}

static void emit_func_body(Ast *ast)
{
    if(ast == NULL)
        return ;
    switch(ast->type) {
        case AST_COMPOUND_STMT: {
            struct sym_obj *reg = sym_local;
            sym_local = sym_init();
            int rsp;

            if(reg != NULL)
                list_copy(sym_local->symbol, reg->symbol);
            for (Iter i = list_iter(ast->stmts); !iter_end(i);) {
                Ast *reg = iter_next(&i);
                if(reg->type == TTYPE_PUNCT) {
                    emit_func_body(reg);
                    /*
                    if(sp != rsp) {
                        emit("  LDA SP");
                        emit("  ADD %s", push_const(rsp-sp, NULL));
                        emit("  STA SP");
                    }
                    sp = rsp;
                    */
                }
                else {
                    emit_func_body(reg);
                }
            }

            sym_del(sym_local);     
            sym_local = reg;
            break;
        }
        case TTYPE_PUNCT:
            switch(ast->ival) {
                case '=':
                    emit_func_body(ast->right);
                    if(ast->left->type == AST_LVAR) {
                        struct symbol *r;
                        if((r = sym_local->read(sym_local, ast->left->varname)) != NULL) {
                            emit("  LDA BP");
                            emit("  ADD %s", push_const(r->offset, NULL));
                            emit("  STA R0");
                            emit("  BSA POP");
                            emit("  STA R0 I");
                            sp++;
                        }
                        else if((r = sym_global->read(sym_global, ast->left->varname)) != NULL) {
                            emit("  BSA POP");
                            emit("  STA %s", r->name);
                            sp++;
                        }
                        else
                            error("error");
                    }
                    else {
                        error("error");
                    }
                    break;
                case '+':
                    if(ast->left != NULL) {
                        lda(ast->left);
                        lda(ast->right);
                        emit("  BSA POP");
                        emit("  STA R1");
                        emit("  BSA POP");
                        emit("  ADD R1");
                        emit("  BSA PUSH");
                        sp++;
                    }
                    else
                        lda(ast->right);
                    break;
                case '-':
                    if(ast->left != NULL) {
                        lda(ast->left);
                        lda(ast->right);
                        emit("  BSA POP");
                        emit("  CMA");
                        emit("  INC");
                        emit("  STA R1");
                        emit("  BSA POP");
                        emit("  ADD R1");
                        emit("  BSA PUSH");
                        sp++;
                    }
                    else {
                        lda(ast->right);
                        emit("  BSA POP");
                        emit("  CMP");
                        emit("  INC");
                        emit("  BSA PUSH");
                    }
                    break;
                case '*':
                    break;
                case '/':
                    break;
            }
            break;
        case AST_LITERAL: {
            emit("  LDA %s", push_const(ast->ival, ast->ctype));
            emit("  BSA PUSH");
            sp--;
            break;
        }
        case AST_STRING:
            break;
        case AST_LVAR: {
            struct symbol *s;
            if((s = sym_local->read(sym_local, ast->varname)) != NULL) {
                emit("  LDA %s", push_const(s->offset, NULL));
                emit("  BSA OSET");
                emit("  BSA PUSH"); 
                sp--;
            }
            else if((s = sym_global->read(sym_global, ast->varname)) != NULL) {
                emit("  LDA %s", s->name);
                emit("  BSA PUSH"); 
                sp--;
            }
            else {
                error("error");
            }
            break;
        }
        case AST_FUNCALL: {
            emit("  BSA %s", ast->fname);
            emit("  BSA CALL");
            break;
        }
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
            //sp -= r->type.size/2;
            
            if(r == &o)
                sym_local->add(sym_local, r);
            if(ast->declinit == NULL) {
                emit("  BSA PUSH");
            }
            else
                emit_func_body(ast->declinit);
            break;
        }
        case AST_ARRAY_INIT:
            break;
        case AST_ADDR:
            break;
        case AST_DEREF:
            break;
        case AST_IF: {
            int r = rc;
            rc+=2;
            emit_func_body(ast->cond);
            emit("  BSA POP");
            emit("  SZA");  
            emit("  BUN .R%d", r);
            sp++;
            emit_func_body(ast->els);
            emit("  BUN .R%d", r+1);
            emit(".R%d, NOP", r);
            emit_func_body(ast->then);
            emit(".R%d, NOP", r+1);
            break;
        }
        case AST_TERNARY:
            break;
        case AST_FOR: {
            int r = rc;
            rc+=2;
            emit_func_body(ast->forinit);
            emit("  BUN .R%d", r);
            emit(".R%d, NOP", r+1);
            emit_func_body(ast->forbody);
            emit_func_body(ast->forstep);
            emit(".R%d, NOP", r);
            emit_func_body(ast->forcond);
            emit("  BSA POP");
            emit("  SZA");
            emit("  BUN .R%d", r+1);
            break;
        }
        case AST_WHILE: {
            int r = rc;
            rc+=2;
            emit("  BUN .R%d", r);
            emit(".R%d, NOP", r+1);
            emit_func_body(ast->whilebody);
            emit(".R%d, NOP", r);
            emit_func_body(ast->whilecond);
            emit("  BSA POP");
            emit("  SZA");
            emit("  BUN .R%d", r+1);
            break;
        }
        case AST_RETURN: {
            emit_func_body(ast->retval);
            emit("  BSA POP");
            sp++;
            break;
         }
        case AST_STRUCT_REF:
            break;
        case PUNCT_EQ:
            break;
        case PUNCT_INC:
            break;
        case PUNCT_DEC:
            break;
        case PUNCT_LOGAND:
            break;
        case PUNCT_LOGOR:
            break;
    }
}
static void p_ast(Ast *v)
{
    if(v->type == AST_FUNC) {
        emit_func_start(v);
        emit_func_body(v->body);
        emit_func_end();
    }
    else if (v->type == AST_DECL) {
        struct symbol o;
        struct symbol *r = sym_global->read(sym_global, v->declvar->varname);

        if(r == NULL) 
            r = &o;
        r->name = v->declvar->varname;
        r->type = *v->declvar->ctype;
        
        if(r == &o) {
            if(v->declinit == NULL) {
                r->data = 0;
            }
            else if(v->declinit->type == AST_LITERAL) {
                r->data = v->declinit->ival;
            }
            else
                error("error");
            sym_global->add(sym_global, r);
        }
        else
            error("error");
    }
    else
        error("error");
}

static void p_list(List *list)
{
    sym_global = sym_init();

    emit("  BSA main");
    emit("  BSA CALL");
//    emit("  ADD NUM");
//    emit("  OUT");
    emit("  HLT");

    for (Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
    emit("  ORG 1000");
    print_func();
    print_global();
    emit("  END");

    sym_del(sym_global);
}
int main()
{
    p_list(statement());
}

