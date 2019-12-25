#include <stdio.h>
#include "c0.h"

#define CR(STR, NUM) do{int _n = printf("%s", STR); for(int i = _n; i < NUM; i++) printf(" ");}while(0);

struct symbol *global, *local;
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
        char str[100];
        sprintf(str, "%s", ast->fname);
        tab(str, "DEC", "0", "");
        tab("", "LDA", str, "");
        tab("", "BSA", "SAVE", "");
        //printf("(AST_FUNC ((%s %d) %s)",getype(ast->ctype->type), ast->ctype->size, ast->fname);
        p_ast(ast->body);
        //printf(")");
    }
    if(ast->type == AST_COMPOUND_STMT) {
        struct symbol *reg = local;
        local = init_symbol();
        p_list(ast->stmts);
        free_symbol(&local);
        local = reg;
    }
    if(ast->type == AST_LITERAL){
        char str[100];
        sprintf(str, "%d", ast->ival);
        tab("", "LDA", str, "");
        tab("", "BSA", "PUSH", "");
        //printf("%d ", ast->ival);
    }
    if(ast->type == TTYPE_PUNCT) {
        switch(ast->ival) {
            case '+':
                p_ast(ast->left);
                p_ast(ast->right);
                tab("", "BSA", "POP", "");
                tab("", "STA", "R1", "");
                tab("", "BSA", "POP", "");
                tab("", "ADD", "R1", "");
                break;
            case '-':
                p_ast(ast->left);
                p_ast(ast->right);
                tab("", "BSA", "POP", "");
                tab("", "CMA", "", "");
                tab("", "INC", "", "");
                tab("", "STA", "R1", "");
                tab("", "BSA", "POP", "");
                tab("", "ADD", "R1", "");
            case '*':
                p_ast(ast->left);
                p_ast(ast->right);
                tab("", "BSA", "MUL", "");
                break;
            case '/':
                p_ast(ast->left);
                p_ast(ast->right);
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
        /*
        printf("(AST_DECL (= ");
        p_ast(ast->declvar);
        p_ast(ast->declinit);
        printf("))");
        */
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
    for (Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
}
int main()
{
    global = init_symbol();
    p_list(statement());
}

