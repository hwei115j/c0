#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "asm.h"
#include "c0.h"
#define CR(STR, NUM) do{int _n = printf("%s", STR); for(int i = _n; i < NUM; i++) printf(" ");}while(0);

struct sym_obj *sym_global;
struct sym_obj *sym_local;
static int sp;

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

    CR(sym, 8);
    CR(com, 4);
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

static void emit_func_start(Ast *v)
{
    emit("%s DEC 0 ", v->fname);
    emit("   BSA GETF");
    emit("   BUN %s I", v->fname);
    
}
static void emit_func_end(void)
{
    emit("   BSA RET");
}
static void emit_func_body(Ast *ast)
{
    switch(ast->type) {
        case AST_COMPOUND_STMT:
            sym_local = sym_init();
            sp = 0;
            for (Iter i = list_iter(ast->stmts); !iter_end(i);) {
                emit_func_body(iter_next(&i));
            }
            sym_del(sym_local);     
            break;
        case TTYPE_PUNCT:
            switch(ast->ival) {
                case '=':
                    emit_func_body(ast->left);
                    emit_func_body(ast->right);
                    break;
                case '+':
                    if(ast->left->type == TTYPE_PUNCT) {
                        emit_func_body(ast->left);
                        emit("  BSA POP");                
                    }
                    else if(ast->left->type == AST_LITERAL){
                        struct symbol *r;
                        char *str = malloc(sizeof(char)*100);
                        sprintf(str, "%d", ast->left->ival);
                        if((r = sym_global->read(sym_global, str)) != NULL) {
                            emit("  LDA %s", r->name);
                        }
                        else {
                            struct symbol r;
                            r.name = str;
                            r.type = *ast->left->ctype;
                            sym_global->add(sym_global, &r);
                            emit("  LDA %s", r.name);
                        }
                    }
                    else if(ast->left->type == AST_LVAR) {
                        struct symbol *r;
                        if((r = sym_local->read(sym_local, ast->left->varname)) != NULL) {
                            emit("  LDA BP");
                            emit("  ADD %d", sp);
                            emit("  STA R0");
                            emit("  LDA R0 I");
                        }
                        else {
                            ;//error
                        }
                    }
                    else {
                        ; //error
                    }
                    emit("  STA R1");
                    if(ast->right->type == TTYPE_PUNCT)
                        emit("  BSA POP");
                    else {
                    }
                    break;
                case '-':
                    break;
                case '*':
                    break;
                case '/':
                    break;
            }
            break;
        case AST_LITERAL: {
            struct symbol *r;
            char *str = malloc(sizeof(char));
            sprintf(str, "%d", ast->ival);
            r = sym_global->read(sym_global, str);
            if((r = sym_global->read(sym_global, str)) == NULL) {
                struct symbol reg;
                reg.name = str;
                reg.type = *(ast->ctype);
                sym_global->add(sym_global, &reg);
            }
            emit("  LDA %s", str);
            emit("  BUN PUSH");
            break;
        }
        case AST_STRING:
            break;
        case AST_LVAR:
            break;
        case AST_FUNCALL:
            break;
        case AST_FUNC:
            break;
        case AST_DECL:
            if(sym_local->read(sym_local, ast->declvar->varname) == NULL) {
                struct symbol r;
                r.name = ast->declvar->varname;
                r.type = *ast->declvar->ctype;
                r.offset = sp;
                sp -= r.type.size/2;
                sym_local->add(sym_local, &r);
            }
            else {
                //error
            }
            emit_func_body(ast->declinit);
            break;
        case AST_ARRAY_INIT:
            break;
        case AST_ADDR:
            break;
        case AST_DEREF:
            break;
        case AST_IF:
            break;
        case AST_TERNARY:
            break;
        case AST_FOR:
            break;
        case AST_WHILE:
            break;
        case AST_RETURN:
            break;
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
    else if(v->type == AST_GVAR) {
    }
    else
        ;
}

static void p_list(List *list)
{
    sym_global = sym_init();
    for (Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
    sym_del(sym_global);
}
int main()
{
    p_list(statement());
}

