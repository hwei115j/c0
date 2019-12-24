#include <stdio.h>
#include "c0.h"

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
        printf("(AST_FUNC ((%s %d) %s)",getype(ast->ctype->type), ast->ctype->size, ast->fname);
        p_ast(ast->body);
        printf(")");
    }
    if(ast->type == AST_COMPOUND_STMT) {
        printf("(AST_COMPOUND_STMT");
        p_list(ast->stmts);
        printf(")");
    }
    if(ast->type == AST_LITERAL){
        printf("%d ", ast->ival);
    }
    if(ast->type == TTYPE_PUNCT) {
        printf("(%c ", ast->ival);
        p_ast(ast->left);
        p_ast(ast->right);
        printf(")");
    }
    if(ast->type == AST_DECL) {
        printf("(AST_DECL (= ");
        p_ast(ast->declvar);
        p_ast(ast->declinit);
        printf("))");
    }
    if(ast->type == AST_GVAR || ast->type == AST_LVAR) {
        printf("(%s %s) ", getype(ast->ctype->type), ast->varname);
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
        printf("(AST_RETURN ");
        p_ast(ast->retval);
        printf(")");
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
    p_list(statement());
}

