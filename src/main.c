#include <stdio.h>
#include "c0.h"

void p_list(List *list);

void p_ast(Ast *ast)
{
    printf("ast = %x ", ast);
    if(!ast)
        return ;
    if(ast->type == AST_FUNC) {
        printf("AST_FUNC name = %s size = %d\n", ast->fname, ast->ctype->size);
        p_ast(ast->body);
    }
    if(ast->type == AST_COMPOUND_STMT) {
        printf("AST_COMPOUND node = %x\n", ast);
        p_list(ast->stmts);
    }
    if(ast->type == AST_LITERAL)
        printf("AST_LITERAL node = %x size = %d ival = %d sval = %s\n",ast, ast->ctype->size, ast->ival, ast->varname);
    if(ast->type == TTYPE_PUNCT) {
        printf("AST_PUNCT node = %x punct = %c left = %x right = %x\n", ast, ast->ival, ast->left, ast->right);
        p_ast(ast->left);
        p_ast(ast->right);
    }
    if(ast->type == AST_DECL) {
        printf("AST_DECL node = %x declvar = %x declinit = %x\n", ast, ast->declvar, ast->declinit);
        p_ast(ast->declvar);
        p_ast(ast->declinit);
    }
    if(ast->type == AST_GVAR || ast->type == AST_LVAR) {
        printf("AST_LVAR node = %x size = %d varname = %s\n", ast, ast->ctype->size, ast->varname); 
    }
    if(ast->type == AST_IF) {
        printf("IF node = %x cond = %x then = %x else = %x\n", ast, ast->cond, ast->then, ast->els);
        p_ast(ast->cond);
        p_ast(ast->then);
        p_ast(ast->els);
    }
    if(ast->type == AST_RETURN) {
        printf("AST_RETURN node = %x retval = %x\n", ast, ast->retval);
        p_ast(ast->retval);
    }
    if(ast->type == AST_FUNCALL) {
        printf("AST_FUNCALL node = %x fname = %s\n", ast, ast->fname);
    }
    if(ast->type == AST_FOR) {
        printf("AST_FOR node = %x init = %x cond = %x step = %x body = %x\n",\
                ast, ast->forinit, ast->forcond, ast->forstep, ast->forbody);
        p_ast(ast->forinit);
        p_ast(ast->forcond);
        p_ast(ast->forstep);
        p_ast(ast->forbody);
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
//    statement();
}

