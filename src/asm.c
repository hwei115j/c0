#include <stdio.h>
#include "c0.h"
#include "asm.h"
#define CR(STR, NUM) do{int _n = printf("%s", STR); for(int i = _n; i < NUM; i++) printf(" ");}while(0);

int bp = 0;
struct sym_obj *sym_global;
struct sym_obj *sym_local;
void tab(char *symbol, char *com, char *par, char *i)
{
    //8 4 8 1
    char str[100] = {};

    if(*symbol)
        sprintf(str, "%s,", symbol);
    CR(str, 8);
    CR(com, 4);
    CR(par, 8);
    CR(i, 1);
    printf("\n");
}
void p_list(List *list);

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

void p_ast(Ast *ast)
{
    if(!ast)
        return ;
    if(ast->type == AST_FUNC) {
        struct symbol r;
        r.name = ast->fname;
        r.type = *ast->ctype;
        sym_global->add(sym_global, r);
        char str[100];
        sprintf(str, "%s", ast->fname);
        tab(str, "DEC", "0", "");
        tab("", "BSA", "GETF", "");
        tab("", "BUN", str, "I");
        //printf("(AST_FUNC ((%s %d) %s)",getype(ast->ctype->type), ast->ctype->size, ast->fname);
        p_ast(ast->body);
        //printf(")");
    }
    if(ast->type == AST_COMPOUND_STMT) {
        sym_local = sym_init();
        p_list(ast->stmts);
        sym_del(sym_local);
    }
    if(ast->type == AST_LITERAL){
        char str[100];
        sprintf(str, "%d", ast->ival);
        tab("", "LDA", str, "");
        tab("", "BSA", "PUSH", "");
        //printf("%d ", ast->ival);
    }
    if(ast->type == TTYPE_PUNCT) {
        p_ast(ast->left);
        p_ast(ast->right);
        switch(ast->ival) {
            case '=':

                break;
            case '+':
                if(ast->left->type == TTYPE_PUNCT)
                    tab("", "BSA", "POP", "");
//                else
//                    tab("", "LDA", ***, "");
                tab("", "STA", "R1", "");
                if(ast->right->type == TTYPE_PUNCT)
                    tab("", "BSA", "POP", "");
                //else
                  //  tab("", "LDA", ***, "");
                tab("", "ADD", "R1", "");
                break;
            case '-':
                tab("", "BSA", "POP", "");
                tab("", "CMA", "", "");
                tab("", "INC", "", "");
                tab("", "STA", "R1", "");
                tab("", "BSA", "POP", "");
                tab("", "ADD", "R1", "");
            case '*':
                tab("", "BSA", "MUL", "");
                break;
            case '/':
                tab("", "BSA", "DIV", "");
                break;
        }
        tab("", "BSA", "PUSH", "");
        /*
        printf("(%c ", ast->ival);
        p_ast(ast->left);
        p_ast(ast->right);
        printf(")");
        */
    }
    if(ast->type == AST_DECL) {
        //printf("(AST_DECL (= ");
        struct symbol r; 
        r.name = ast->declvar->varname;
        r.type = *ast->declvar->ctype;
        if(ast->declvar->type == AST_LVAR) {
            r.base = "bp";
            r.offset = bp++;
            sym_local->add(sym_local, r);   
        }
        if(ast->declvar->type == AST_GVAR) {
            sym_global->add(sym_global, r);
        }
        p_ast(ast->declinit);
        //printf("))");
    }
    if(ast->type == AST_LVAR) {
        printf("(%s %s) ", getype(ast->ctype->type), ast->varname);
    }
    if(ast->type == AST_GVAR) {
    }
    if(ast->type == AST_IF) {
        printf("(AST_IF ");
        printf("(COND ");
        p_ast(ast->cond);
        printf(")(THEN ");
        p_ast(ast->then);
        printf(")(ELSE ");
        p_ast(ast->els);
        printf("))");
    }
    if(ast->type == AST_RETURN) {
        //printf("(AST_RETURN ");
        p_ast(ast->retval);
        tab("", "BSA", "POP", "");
        tab("", "BSA", "RET", "");
        //printf(")");
    }
    if(ast->type == AST_FUNCALL) {
        printf("(AST_FUNCALL %s)", ast->fname);
    }
    if(ast->type == AST_FOR) {
        printf("(AST_FOR ");
        printf("(INIT ");
        p_ast(ast->forinit);
        printf(")(COND ");
        p_ast(ast->forcond);
        printf(")(STEP ");
        p_ast(ast->forstep);
        printf(")(BODY ");
        p_ast(ast->forbody);
        printf("))");
    }
    if(ast->type == AST_WHILE) {
        printf("(AST_WHILE ");
        printf("(COND ");
        p_ast(ast->whilecond);
        printf(")(BODY ");
        p_ast(ast->whilebody);
        printf("))");
    }
}

void p_list(List *list)
{
    sym_global = sym_init();
    for (Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
}
int main()
{
    p_list(statement());
}

