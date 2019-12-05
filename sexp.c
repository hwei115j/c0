#include <stdio.h>
#include "c0.h"

void p_list(List *list);

void p_ast(Ast *ast)
{
    if(!ast)
        return ;
    if(ast->type == AST_FUNC) {
        //printf("AST_FUNC name = %s size = %d\n", ast->fname, ast->ctype->size);
        printf("(AST_FUNC ((size %d) %s)", ast->ctype->size, ast->fname);
        p_ast(ast->body);
        printf(")");
    }
    if(ast->type == AST_COMPOUND_STMT) {
        //printf("AST_COMPOUND node = %x\n", ast);
        printf("(AST_COMPOUND");
        p_list(ast->stmts);
        printf(")");
    }
    if(ast->type == AST_LITERAL){
     //   printf("AST_LITERAL node = %x size = %d ival = %d sval = %s\n",ast, ast->ctype->size, ast->ival, ast->varname);
        printf("%d ", ast->ival);
    }
    if(ast->type == TTYPE_PUNCT) {
        //printf("AST_PUNCT node = %x punct = %c left = %x right = %x\n", ast, ast->ival, ast->left, ast->right);
        printf("(%c ", ast->ival);
        p_ast(ast->left);
        p_ast(ast->right);
        printf(")");
    }
    if(ast->type == AST_DECL) {
        //printf("AST_DECL node = %x declvar = %x declinit = %x\n", ast, ast->declvar, ast->declinit);
        printf("(AST_DECL (= ");
        p_ast(ast->declvar);
        p_ast(ast->declinit);
        printf("))");
    }
    if(ast->type == AST_GVAR || ast->type == AST_LVAR) {
        //printf("VAR node = %x size = %d varname = %s\n", ast, ast->ctype->size, ast->varname); 
        printf("((size %d) %s) ", ast->ctype->size, ast->varname);
    }
    if(ast->type == AST_IF) {
//        printf("IF node = %x cond = %x then = %x else = %x\n", ast, ast->cond, ast->then, ast->els);
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
        //printf("AST_RETURN node = %x retval = %x\n", ast, ast->retval);
        printf("(AST_RETURN ");
        p_ast(ast->retval);
        printf(")");
    }
    if(ast->type == AST_FUNCALL) {
        //printf("AST_FUNCALL node = %x fname = %s\n", ast, ast->fname);
        printf("(AST_FUNCALL %s)", ast->fname);
    }
    if(ast->type == AST_FOR) {
//        printf("AST_FOR node = %x init = %x cond = %x step = %x body = %x\n",\
//                ast, ast->forinit, ast->forcond, ast->forstep, ast->forbody);
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

