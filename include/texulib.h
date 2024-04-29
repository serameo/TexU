/*
#
# File Name: texulib.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXULIB_H_
#define _TEXULIB_H_

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
# TexU list
#
            1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_list;
typedef struct texu_list texu_list;

struct texu_list_item
{
    /* CAN READ ONLY */
    struct texu_list_item *prev;
    struct texu_list_item *next;
    /* CAN READ AND WRITE */
    texu_longptr data;
};
typedef struct texu_list_item texu_list_item;

/* new a list */
texu_list *texu_list_new();
/* delete the list created by texu_list_new() */
void texu_list_del(texu_list *);
/* free the list */
void texu_list_free(texu_list *);
/* callback free the list */
void texu_list_cb_free(
    texu_list *,
    void (*)(texu_longptr, void *),
    void *);
/* add/insert a new item */
texu_status texu_list_add(texu_list *, texu_longptr);
texu_status texu_list_insert_first(texu_list *, texu_longptr);
texu_status texu_list_insert_last(texu_list *, texu_longptr);
texu_status texu_list_insert(texu_list *, texu_list_item *, texu_longptr);
/* remove an item on the list */
texu_status texu_list_remove(texu_list *, texu_list_item *);
texu_status texu_list_cb_remove(
    texu_list *,
    texu_list_item *,
    void (*)(texu_longptr, void *),
    void *);
/* find a data in the list */
texu_list_item *texu_list_find_first(texu_list *, texu_longptr);
texu_list_item *texu_list_find_next(texu_list *, texu_list_item *, texu_longptr);
/* reverse find a data in the list */
texu_list_item *texu_list_rfind_last(texu_list *, texu_longptr);
texu_list_item *texu_list_rfind_prev(texu_list *, texu_list_item *, texu_longptr);

/* properties */
texu_list_item *texu_list_first(texu_list *);
texu_list_item *texu_list_last(texu_list *);
texu_i32 texu_list_count(texu_list *);

/*
# TexU queue
#
            1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_queue;
typedef struct texu_queue texu_queue;

texu_queue *texu_queue_new();
void texu_queue_del(texu_queue *);

void texu_queue_free(texu_queue *);
/* callback free the queue */
void texu_queue_cb_free(
    texu_queue *,
    void (*)(texu_longptr, void *),
    void *);
/* add/insert a new item */
texu_status texu_queue_enqueue(texu_queue *, texu_longptr);
/* remove an item on the queue */
texu_status texu_queue_dequeue(texu_queue *);
texu_status texu_queue_cb_dequeue(
    texu_queue *,
    void (*)(texu_longptr, void *),
    void *);
texu_longptr texu_queue_first(texu_queue *);
texu_bool texu_queue_empty(texu_queue *);

/*
# TexU array
#
            1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_array;
typedef struct texu_array texu_array;

/* new an array*/
texu_array *texu_array_new(texu_i32);
/* delete the array */
void texu_array_del(texu_array *);
/* free the array */
void texu_array_free(texu_array *);
/* callback free the array */
void texu_array_cb_free(
    texu_array *,
    void (*)(texu_longptr, void *),
    void *);
/* get a data */
texu_longptr texu_array_get(texu_array *, texu_longptr);
/* set a data */
void texu_array_set(texu_array *, texu_longptr, texu_longptr);
/* re-allocate memory and copy the old data to the new */
texu_status texu_array_realloc(texu_array *, texu_longptr);

/* properties */
texu_i32 texu_array_count(texu_array *);

/*
# TexU stack
#
            1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_stack;
typedef struct texu_stack texu_stack;

texu_stack *texu_stack_new(texu_longptr);
void texu_stack_del(texu_stack *);
/* free the stack */
void texu_stack_free(texu_stack *);
/* callback free the stack */
void texu_stack_cb_free(
    texu_stack *,
    void (*)(texu_longptr, void *),
    void *);

texu_bool texu_stack_empty(texu_stack *);
texu_bool texu_stack_full(texu_stack *);

texu_status texu_stack_push(texu_stack *, texu_longptr);
texu_status texu_stack_pop(texu_stack *);
texu_status texu_stack_cb_pop(
    texu_stack *,
    void (*)(texu_longptr, void *),
    void *);
texu_longptr texu_stack_top(texu_stack *);
texu_longptr texu_stack_count(texu_stack *);

/*
# TexU tree
#
            1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_tree;
typedef struct texu_tree texu_tree;

struct texu_tree_item
{
    /* CAN READ ONLY */
    struct texu_tree_item *parent;
    struct texu_tree_item *firstchild;
    struct texu_tree_item *lastchild;
    struct texu_tree_item *prev;
    struct texu_tree_item *next;
    texu_i32 nchildren;
    texu_i32 nlevel;
    /* CAN READ AND WRITE */
    texu_longptr data;
};
typedef struct texu_tree_item texu_tree_item;

texu_tree *texu_tree_new();
void texu_tree_del(texu_tree *);

texu_tree_item *texu_tree_add_item(texu_tree *, texu_tree_item *, texu_longptr);
texu_status texu_tree_remove_item(texu_tree *, texu_tree_item *);
texu_status texu_tree_cb_remove_item(
    texu_tree *,
    texu_tree_item *,
    void (*)(texu_longptr, void *),
    void *);

texu_tree_item *texu_tree_find_item(
    texu_tree *,
    texu_longptr,
    texu_i32 (*)(texu_longptr, texu_longptr, void *),
    void *);
texu_tree_item *texu_tree_find_next_item(
    texu_tree *,
    texu_tree_item *,
    texu_longptr,
    texu_i32 (*)(texu_longptr, texu_longptr, void *),
    void *);
texu_tree_item *texu_tree_get_root(texu_tree *);
texu_i32 texu_tree_count(texu_tree *);

/*
# 3rd parameter
#   < 0 : pre-order traversal
#   = 0 : in-order traversal
#   > 0 : post-order traversal
*/
texu_status texu_tree_populate(
    texu_tree *,
    texu_tree_item *,
    texu_longptr,
    texu_longptr (*)(texu_tree_item *, void *),
    void *);

/*
# TexU map
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_map_keyval
{
    texu_longptr    key;
    texu_longptr    value;
    texu_bool       used;
};
typedef struct texu_map_keyval texu_map_keyval;

struct texu_map;
typedef struct texu_map texu_map;

texu_map *texu_map_new();
void texu_map_del(texu_map *);
void texu_map_free(texu_map *);
void texu_map_cb_free(
    texu_map *,
    void (*)(texu_longptr, texu_longptr, void *),
    void *);
/* insert */
/*
# texu_map_strcmp: to use, when a key is a string
#
#
*/
int texu_map_strcmp(const void *, const void *);
int texu_map_wstrcmp(const void *v1, const void *v2);

texu_status texu_map_insert(
    texu_map *,
    texu_longptr,
    texu_longptr);
texu_status texu_map_cmp_insert(
    texu_map *,
    texu_longptr,
    texu_longptr,
    int (*)(const void *, const void *));
/* remove */
texu_status texu_map_remove(
    texu_map *,
    texu_longptr);
texu_status texu_map_cmp_remove(
    texu_map *,
    texu_longptr,
    int (*)(const void *, const void *));
texu_status texu_map_cb_remove(
    texu_map *,
    texu_longptr,
    int (*)(const void *, const void *),
    void (*)(texu_longptr, texu_longptr, void *),
    void *);

/* find */
texu_status texu_map_find(
    texu_map *,
    texu_longptr,
    texu_longptr *);
texu_status texu_map_cmp_find(
    texu_map *,
    texu_longptr,
    texu_longptr *,
    int (*)(const void *, const void *));
texu_status texu_map_foreach(
    texu_map *,
    texu_i32 (*)(texu_longptr, texu_longptr, void *),
    int (*)(const void *, const void *),
    void *);

/*
# TexU bits
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_bits;
typedef struct texu_bits texu_bits;
#define TEXU_BITS_OFF       0
#define TEXU_BITS_ON        1

texu_bits* texu_bits_new(texu_i32 nbits);
void       texu_bits_del(texu_bits*);
texu_i32   texu_bits_get(texu_bits*, texu_i32 at);
texu_i32   texu_bits_set(texu_bits*, texu_i32 at, texu_i32 on);
void       texu_bits_set_all(texu_bits*, texu_i32 on);

#ifdef __cplusplus
}
#endif

#endif /* _TEXULIB_H_ */
