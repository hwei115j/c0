#ifndef __SYMBOL_H_
#define __SYMBOL_H_
#include "list.h"
#include "dict.h"
#include "c0.h"
#include <stdlib.h>
#include <string.h>

struct symbol {
    char *name;
    union {
        int data;
        int offset;
        char *str;
        void *ptr;
    };
//    struct symbol *struct_symbol;
    Ctype type;                
};

struct sym_obj {
    //List *symbol;
    struct dict *symbol;
    struct symbol *(*read)(struct sym_obj *, char *);
    void (*add)(struct sym_obj *, struct symbol *);
    struct sym_obj *(*clone)(struct sym_obj *);
};
#if 1
static struct sym_obj *sym_clone(struct sym_obj *obj);
static struct symbol *sym_read(struct sym_obj *obj, char *str)
{
    return dict_read(obj->symbol, str);
}
static void sym_add(struct sym_obj *obj, struct symbol *r)
{
    dict_add(obj->symbol, r->name, r, sizeof(struct symbol));
}
static struct sym_obj *sym_init()
{
    struct sym_obj *r = malloc(sizeof(struct sym_obj));
    r->symbol = dict_init();
    r->read = sym_read;
    r->add = sym_add;
    r->clone = sym_clone;
    return r;
}
static void sym_del(struct sym_obj *r)
{
    if(r == NULL)
        return ;
    dict_del(r->symbol);
    free(r);
}
static struct sym_obj *sym_clone(struct sym_obj *obj)
{
    struct sym_obj *r = sym_init();
    r->symbol = dict_clone(obj->symbol);
    return r;
}
#endif
#if 0
static struct sym_obj *sym_clone(struct sym_obj *obj);
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

static struct sym_obj *sym_init()
{
    struct sym_obj *r = malloc(sizeof(struct sym_obj));
    r->symbol = make_list();
    r->read = sym_read;
    r->add = sym_add;
    r->clone = sym_clone;
    return r;
}

static void sym_del(struct sym_obj *r)
{
    if(r == NULL)
        return ;
    list_node_free(r->symbol);
    free(r);
}

static struct sym_obj *sym_clone(struct sym_obj *obj)
{
    struct sym_obj *r = sym_init();
    list_copy(r->symbol, obj->symbol);
    return r;
}
#endif
#endif
