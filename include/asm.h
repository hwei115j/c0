#ifndef __ASM_H_
#define __ASM_H_
#include "list.h"
#include "c0.h"
#include <stdlib.h>
#include <string.h>

struct symbol {
    char *name;
    union {
        int data;
        int offset;
    };
    Ctype type;                
};

struct sym_obj {
    List *symbol;
    struct symbol *(*read)(struct sym_obj *, char *);
    void (*add)(struct sym_obj *, struct symbol *);
};

static struct symbol *sym_read(struct sym_obj *obj, char *str)
{
    if(str == NULL)
        return NULL;
    for (Iter i = list_iter(obj->symbol); !iter_end(i);) {
        struct symbol *r = iter_next(&i);
        if(!strcmp(r->name, str))
            return r;
    }
    return NULL;
}

static void sym_add(struct sym_obj *obj, struct symbol *r)
{
    if(r == NULL)
        return ;
    struct symbol *m = malloc(sizeof(struct symbol));
    memcpy(m, r, sizeof(struct symbol));
    list_push(obj->symbol, m);
}
struct sym_obj *sym_init()
{
    struct sym_obj *r = malloc(sizeof(struct sym_obj));
    r->symbol = make_list();
    r->read = sym_read;
    r->add = sym_add;

    return r;
}
void sym_del(struct sym_obj *r)
{
    free(r);
}
#endif
