#ifndef __DICT_H_
#define __DICT_H_
#include "list.h"
#include <string.h>
#include <stdlib.h>

struct __dict {
    char *name;
    void *data;
};

struct dict {
    List *dict;
};

static struct dict *dict_clone(struct dict *obj);

static void *dict_read(struct dict *obj, char *str)
{
    if(str == NULL)
        return NULL;
    for (Iter i = list_iter(obj->dict); !iter_end(i);) {
        struct __dict *r = iter_next(&i);
        if(!strcmp(r->name, str))
            return r->data;
    }
    return NULL;
}

static void dict_add(struct dict *obj, char *name, void *data, size_t data_size)
{
    if(obj == NULL)
        return ;
    struct __dict *r = malloc(sizeof(struct __dict));
    r->name = strcpy(malloc(sizeof(char)*strlen(name)), name);
    r->data = memcpy(malloc(data_size), data, data_size);
    list_push(obj->dict, r);
}

static struct dict *dict_init() 
{
    struct dict *obj = malloc(sizeof(struct dict));
    obj->dict = make_list();

    return obj;
}
static void dict_del(struct dict *r)
{
    if(r == NULL)
        return ;
    list_node_free(r->dict);
    free(r);
}

static struct dict *dict_clone(struct dict *obj)
{
    struct dict *r = dict_init();
    list_copy(r->dict, obj->dict);
    return r;
}
static void dict_for()
{
}

#endif
