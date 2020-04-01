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

static void dectype(Ctype *ctype)
{
    if(ctype->type == CTYPE_ARRAY)
        printf("([] ");
    else if(ctype->type == CTYPE_PTR)
        printf("(* ");
    else
        printf("(%s ", getype(ctype->type));
    if(ctype->ptr != NULL) {
        dectype(ctype->ptr);
    }
}

void p_ast(Ast *ast)
{
    if(!ast)
        return ;
    if(ast->type == AST_FUNC) {
        printf("(AST_FUNC (%s (%s)",ast->fname, getype(ast->ctype->type), ast->ctype->size);
        for (Iter i = list_iter(ast->args); !iter_end(i);) {
            Ast *v = iter_next(&i);
            p_ast(v->declvar);
        }
        printf(")");
        p_ast(ast->body);
        printf(")");
    }
    if(ast->type == AST_COMPOUND_STMT) {
//        printf("(AST_COMPOUND_STMT");
        p_list(ast->stmts);
//        printf(")");
    }
    if(ast->type == AST_STRING) {
        printf("\"%s\" ", ast->sval);
    }
    if(ast->type == AST_LITERAL){
        printf("%d ", ast->ival);
    }
    if(ast->type == TTYPE_PUNCT) {
        switch(ast->ival) {
            case PUNCT_CIR:
                printf("(>> ");
                break;
            case PUNCT_CIL:
                printf("(<< ");
                break;
            case PUNCT_INC:
                printf("(++ ");
                break;
            case PUNCT_DEC:
                printf("(-- ");
                break;
            case PUNCT_EQ:
                printf("(== ");
                break;
            default:
                printf("(%c ", ast->ival);
                break ;
        }
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
        int i = 0;
        Ctype *t = ast->ctype;
        if(t != NULL) {
            printf("(");
            for(i = 0; t != NULL ; i++, t = t->ptr) {
                if(t->type == CTYPE_ARRAY) {
                    printf("([%d]", t->len);
                }
                else if(t->type == CTYPE_PTR)
                    printf("(*");
                else
                    printf("(%s",getype(t->type));
            }
            while(i--)
                printf(")");
            printf("%s)", ast->varname);
        }
        else 
            printf(" %s ", ast->varname);
    }
    if(ast->type == AST_DEREF) {
        printf("(*");
        p_ast(ast->operand);
        printf(")");
    }
    if(ast->type == AST_ADDR) {
        printf("(&");
        p_ast(ast->operand);
        printf(")");
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
        printf("(AST_FUNCALL (%s ", ast->fname);
        for (Iter i = list_iter(ast->args); !iter_end(i);) {
            Ast *v = iter_next(&i);
            p_ast(v);
        }
        printf("))");
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
    if(ast->type == AST_ASSIGNMENT_EXPR) {
        for (Iter i = list_iter(ast->exprs); !iter_end(i);) {
            Ast *v = iter_next(&i);
            p_ast(v);
        }
        //fprintf(stderr, ".....\n");
    }
}

void p_list(List *list)
{
    for(Iter i = list_iter(list); !iter_end(i);) {
        Ast *v = iter_next(&i);
        p_ast(v);
    }
}
int main()
{
    p_list(statement());
}

