/*
#
# File Name: texulib.c
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "texutypes.h"
#include "texulib.h"

#ifdef TEXU_THREAD_SAFE
#include <pthread.h>
#endif

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

texu_list_item *_texu_list_item_new(texu_longptr data);
void _texu_list_item_del(texu_list_item *);

texu_list_item *
_texu_list_item_new(texu_longptr data)
{
    texu_list_item *item = (texu_list_item *)malloc(sizeof(texu_list_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_list_item));
        item->data = data;
    }
    return item;
}

void
_texu_list_item_del(texu_list_item *item)
{
    if (item)
    {
        free(item);
        item = 0;
    }
}

struct texu_list
{
    texu_ui64 nitems;
    texu_list_item *first;
    texu_list_item *last;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_t mutex;
#endif
};

texu_list *
texu_list_new()
{
    texu_list *list = (texu_list *)malloc(sizeof(texu_list));
    if (TEXU_IS_NULL(list))
    {
        return 0;
    }
    memset(list, 0, sizeof(texu_list));
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_init(&list->mutex, NULL);
#endif
    return list;
}

void
texu_list_del(texu_list *list)
{
    if (list)
    {
        texu_list_free(list);
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_destroy(&list->mutex);
#endif
        free(list);
        list = 0;
    }
}

void
texu_list_free(texu_list *list)
{
    texu_list_cb_free(list, 0, 0);
}

void
texu_list_cb_free(
    texu_list *list,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_list_item *delitem = 0;
    texu_list_item *item = 0;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    item = list->first;

    while (item)
    {
        if (cb)
        {
            cb(item->data, user);
        }
        delitem = item;
        item = item->next;

        _texu_list_item_del(delitem);
    }
    /* reset all pointers */
    list->first = list->last = 0;
    list->nitems = 0;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
}

texu_status
texu_list_insert_first(texu_list *list, texu_longptr data)
{
    texu_list_item *newitem = _texu_list_item_new(data);
    if (!newitem)
    {
        return TEXU_NOMEM;
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    if (list->first)
    {
        newitem->next = list->first->next;
        list->first->prev = newitem;
        list->first = newitem;
    }
    else
    {
        list->first = list->last = newitem;
    }

    ++list->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return TEXU_OK;
}

texu_status
texu_list_insert_last(texu_list *list, texu_longptr data)
{
    return texu_list_add(list, data);
}

texu_status
texu_list_insert(
    texu_list *list,
    texu_list_item *after,
    texu_longptr data)
{
    texu_list_item *prev = 0;
    texu_list_item *newitem = _texu_list_item_new(data);
    if (!newitem)
    {
        return TEXU_NOMEM;
    }
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
    #endif

    if (after == list->first)
    {
        newitem->next = list->first;
        list->first->prev = newitem;
        list->first = newitem;
    }
    else
    {
        prev = after->prev;

        newitem->next = after;
        newitem->prev = prev;
        if (prev)
        {
            prev->next = newitem;
        }
        after->prev = newitem;
    }
    ++list->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return TEXU_OK;
}

texu_status
texu_list_add(texu_list *list, texu_longptr data)
{
    texu_list_item *newitem = _texu_list_item_new(data);
    if (!newitem)
    {
        return TEXU_NOMEM;
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    if (list->first)
    {
        newitem->prev = list->last;
        list->last->next = newitem;
        list->last = newitem;
    }
    else
    {
        list->first = list->last = newitem;
    }

    ++list->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return TEXU_OK;
}

texu_status
texu_list_remove(texu_list *list, texu_list_item *item)
{
    return texu_list_cb_remove(list, item, 0, 0);
}

texu_status
texu_list_cb_remove(
    texu_list *list,
    texu_list_item *item,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_list_item *prev = 0;
    texu_list_item *next = 0;

    if (!item)
    {
        return TEXU_OK;
    }
    /* make a new link */
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
   if (item == list->first)
    {
        list->first = list->first->next;
        if (list->first)
        {
            list->first->prev = 0;
        }
    }
    else if (item == list->last)
    {
        list->last = list->last->prev;
        if (list->last)
        {
            list->last->next = 0;
        }
    }
    else
    {
        prev = item->prev;
        next = item->next;

        if (prev)
        {
            prev->next = next;
        }
        if (next)
        {
            next->prev = prev;
        }
    }
    /* can delete now */
    if (cb)
    {
        cb(item->data, user);
    }
    _texu_list_item_del(item);
    --list->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return TEXU_OK;
}

texu_list_item *
texu_list_first(texu_list *list)
{
    texu_list_item *item = 0;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    item = list->first;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return item;
}

texu_list_item *
texu_list_last(texu_list *list)
{
    texu_list_item *item = 0;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    item = list->last;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return item;
}

texu_ui64
texu_list_count(texu_list *list)
{
    texu_ui64 nitems = 0;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&list->mutex);
#endif
    nitems = list->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&list->mutex);
#endif
    return nitems;
}

texu_list_item *
texu_list_find_first(
    texu_list *list,
    texu_longptr data)
{
    return texu_list_find_next(list, 0, data);
}

texu_list_item *
texu_list_find_next(
    texu_list *list,
    texu_list_item *curitem,
    texu_longptr data)
{
    texu_list_item *next = 0;
    if (!curitem)
    {
        curitem = list->first;
        if (curitem && curitem->data == data)
        {
            return curitem;
        }
    }
    next = curitem->next;
    while (next)
    {
        if (next->data == data)
        {
            return next;
        }
        next = next->next;
    }
    return 0;
}

texu_list_item *
texu_list_rfind_last(
    texu_list *list,
    texu_longptr data)
{
    return texu_list_rfind_prev(list, 0, data);
}

texu_list_item *
texu_list_rfind_prev(
    texu_list *list,
    texu_list_item *curitem,
    texu_longptr data)
{
    texu_list_item *prev = 0;
    if (!curitem)
    {
        curitem = list->last;
        if (curitem && curitem->data == data)
        {
            return curitem;
        }
    }
    prev = curitem->prev;
    while (prev)
    {
        if (prev->data == data)
        {
            return prev;
        }
        prev = prev->prev;
    }
    return 0;
}

/*
# TexU queue
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_queue
{
    texu_list *list;
};

texu_queue *
texu_queue_new()
{
    texu_queue *queue = (texu_queue *)malloc(sizeof(texu_queue));
    if (queue)
    {
        queue->list = texu_list_new();
    }
    return queue;
}

void
texu_queue_del(texu_queue *queue)
{
    if (queue)
    {
        texu_list_del(queue->list);
        free(queue);
    }
}

void
texu_queue_free(texu_queue *queue)
{
    texu_queue_cb_free(queue, 0, 0);
}

/* callback free the queue */
void
texu_queue_cb_free(
    texu_queue *queue,
    void (*cb)(texu_longptr, void *),
    void *userdata)
{
    texu_list_cb_free(queue->list, cb, userdata);
}

/* add/insert a new item */
texu_status
texu_queue_enqueue(texu_queue *queue, texu_longptr data)
{
    return texu_list_add(queue->list, data);
}

/* remove an item on the queue */
texu_status texu_queue_dequeue(texu_queue *queue)
{
    return texu_queue_cb_dequeue(queue, 0, 0);
}

texu_status
texu_queue_cb_dequeue(
    texu_queue *queue,
    void (*cb)(texu_longptr, void *),
    void *userdata)
{
    texu_list_item *item = texu_list_first(queue->list);
    return texu_list_cb_remove(queue->list, item, cb, userdata);
}

texu_longptr
texu_queue_first(texu_queue *queue)
{
    texu_list_item *item = texu_list_first(queue->list);
    return (item ? item->data : 0);
}

texu_bool
texu_queue_empty(texu_queue *queue)
{
    return (texu_list_count(queue->list) == 0);
}

/*
# TexU array
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_array
{
    texu_longptr nitems;
    texu_longptr *items;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_t mutex;
#endif
};

texu_array *
texu_array_new(texu_ui64 nitems)
{
    texu_array *array = (texu_array *)malloc(sizeof(texu_array));
    texu_longptr *items = 0;
    size_t size = 0;
    if (array)
    {
        memset(array, 0, sizeof(texu_array));
        if (0 == nitems)
        {
            nitems = 16;
        }
        array->nitems = nitems;

        size = sizeof(texu_lparam) * nitems;
        items = (texu_longptr *)malloc(size);
        if (!items) /*cannot allocate a new memory*/
        {
            free(array);
            return 0;
        }
        memset(items, 0, size);
        array->items = items;
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_init(&array->mutex, NULL);
#endif
    }
    return array;
}

void
texu_array_del(texu_array *array)
{
    if (array)
    {
        texu_array_free(array);
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_destroy(&array->mutex);
#endif
        free(array);
        array = 0;
    }
}

void
texu_array_free(texu_array *array)
{
    texu_array_cb_free(array, 0, 0);
}

void
texu_array_cb_free(
    texu_array *array,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_longptr i = 0;
    if (cb)
    {
        for (; i < array->nitems; ++i)
        {
            cb(array->items[i], user);
        }
    }
    /* reset all items */
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&array->mutex);
#endif
    memset(array->items, 0, sizeof(texu_lparam)* array->nitems);
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&array->mutex);
#endif
}

texu_longptr
texu_array_get(texu_array *array, texu_longptr idx)
{
    if (idx < 0 || idx >= array->nitems)
    {
        return 0;
    }
    return array->items[idx];
}

void
texu_array_set(texu_array *array, texu_longptr idx, texu_longptr data)
{
    if (idx < 0 || idx >= array->nitems)
    {
        return;
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&array->mutex);
#endif
    array->items[idx] = data;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&array->mutex);
#endif
}

texu_status
texu_array_realloc(texu_array *array, texu_longptr nitems)
{
    texu_longptr *items = 0;
    size_t size = 0;
    if (0 >= nitems)
    {
        return TEXU_OK;
    }
    /* new memory */
    size = sizeof(texu_lparam) * nitems;
    items = (texu_longptr *)malloc(size);
    if (!items)
    {
        return TEXU_NOMEM;
    }
    memset(items, 0, size);

    /* copy to new memory */
    nitems = TEXU_MIN(nitems, array->nitems);
    size = sizeof(texu_lparam) * nitems;
    memcpy(items, array->items, size);

    /* delete the old memory */
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&array->mutex);
#endif
    free(array->items);
    array->items = items;
    array->nitems = nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&array->mutex);
#endif

    return TEXU_OK;
}

texu_ui64
texu_array_count(texu_array *array)
{
    return array->nitems;
}

/*
# TexU stack
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_stack
{
    texu_longptr index;
    texu_longptr nitems;
    texu_longptr *items;
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_t mutex;
    #endif
};

texu_stack *
texu_stack_new(texu_longptr nitems)
{
    texu_stack *stack = (texu_stack *)malloc(sizeof(texu_stack));
    texu_longptr *items = 0;
    size_t size = 0;
    if (stack)
    {
        memset(stack, 0, sizeof(texu_stack));
        if (nitems <= 0)
        {
            nitems = 256;
        }
        else if (nitems > 32768) /* 2^15 */
        {
            nitems = 32768;
        }
        stack->nitems = nitems;
        stack->index = -1;

        size = sizeof(texu_lparam) * nitems;
        items = (texu_longptr *)malloc(size);
        if (!items) /*cannot allocate a new memory*/
        {
            free(stack);
            return 0;
        }
        memset(items, 0, size);
        stack->items = items;
        #ifdef TEXU_THREAD_SAFE
        pthread_mutex_init(&stack->mutex, NULL);
        #endif
    }
    return stack;
}

void
texu_stack_del(texu_stack *stack)
{
    if (stack)
    {
        texu_stack_free(stack);
        #ifdef TEXU_THREAD_SAFE
        pthread_mutex_destroy(&stack->mutex);
        #endif
        free(stack);
        stack = 0;
    }
}

void
texu_stack_free(texu_stack *stack)
{
    texu_stack_cb_free(stack, 0, 0);
}

void
texu_stack_cb_free(
    texu_stack *stack,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_longptr i = 0;
    if (cb)
    {
        for (; i <= stack->index; ++i)
        {
            cb(stack->items[i], user);
        }
    }
    /* reset all items */
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&stack->mutex);
    #endif
    memset(stack->items, 0, sizeof(texu_lparam) * stack->nitems);
    stack->index = -1;
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&stack->mutex);
    #endif
}

texu_bool
texu_stack_empty(texu_stack *stack)
{
    return (stack->index < 0 ? TEXU_TRUE : TEXU_FALSE);
}

texu_bool
texu_stack_full(texu_stack *stack)
{
    return (stack->index >= (stack->nitems - 1) ? TEXU_TRUE : TEXU_FALSE);
}

texu_longptr
texu_stack_top(texu_stack *stack)
{
    if (texu_stack_empty(stack))
    {
        return 0;
    }
    return stack->items[stack->index];
}

texu_status
texu_stack_push(texu_stack *stack, texu_longptr data)
{
    if (texu_stack_full(stack))
    {
        return TEXU_NOMEM;
    }
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&stack->mutex);
    #endif
    ++stack->index;
    stack->items[stack->index] = data;
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&stack->mutex);
    #endif
    return TEXU_OK;
}

texu_status
texu_stack_pop(texu_stack *stack)
{
    return texu_stack_cb_pop(stack, 0, 0);
}

texu_status
texu_stack_cb_pop(
    texu_stack *stack,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    if (texu_stack_empty(stack))
    {
        return TEXU_OK;
    }
    if (cb)
    {
        cb(stack->items[stack->index], user);
    }
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&stack->mutex);
    #endif
    stack->items[stack->index] = 0;
    --stack->index;
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&stack->mutex);
    #endif

    return TEXU_OK;
}

texu_longptr
texu_stack_count(texu_stack *stack)
{
    return stack->nitems;
}
/*
# TexU tree
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_tree
{
    texu_tree_item *root;
    texu_ui64 nitems;
    #ifdef TEXU_THREAD_SAFE
    pthread_mutex_t mutex;
    #endif
};

texu_tree_item *_texu_tree_item_new(texu_longptr);
void _texu_tree_item_del(texu_tree_item *);
texu_status _texu_tree_cb_remove_item(
    texu_tree *tree,
    texu_tree_item *delitem,
    void (*cb)(texu_longptr, void *),
    void *user);
texu_status _texu_tree_populate_pre(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user);
texu_status _texu_tree_populate_in(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user);
texu_status _texu_tree_populate_post(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user);

texu_tree_item *
_texu_tree_item_new(texu_longptr data)
{
    texu_tree_item *item = (texu_tree_item *)malloc(sizeof(texu_tree_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_tree_item));
        item->data = data;
    }
    return item;
}

void
_texu_tree_item_del(texu_tree_item *item)
{
    if (item)
    {
        free(item);
        item = 0;
    }
}

texu_status
_texu_tree_cb_remove_item(
    texu_tree *tree,
    texu_tree_item *delitem,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_tree_item *prev = 0;
    if (!delitem)
    {
        return TEXU_OK;
    }
    if (delitem->lastchild)
    {
        prev = delitem->lastchild->prev;
    }
    _texu_tree_cb_remove_item(tree, delitem->lastchild, cb, user);
    _texu_tree_cb_remove_item(tree, prev, cb, user);
    if (!delitem->firstchild)
    {
        if (cb)
        {
            cb(delitem->data, user);
        }
    }
    return TEXU_OK;
}

texu_status
_texu_tree_populate_pre(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user)
{
    texu_longptr rc = TEXU_OK;
    if (item && proc)
    {
        rc = proc(item, user);
        if (TEXU_OK == rc)
        {
            rc = _texu_tree_populate_pre(
                tree, item->firstchild, proc, user);
            if (TEXU_OK == rc)
            {
                rc = _texu_tree_populate_pre(
                    tree, item->next, proc, user);
            }
        }
    }
    return rc;
}

texu_status
_texu_tree_populate_in(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user)
{
    texu_longptr rc = TEXU_OK;
    if (item && proc)
    {
        rc = _texu_tree_populate_in(
            tree, item->firstchild, proc, user);
        if (TEXU_OK == rc)
        {
            rc = proc(item, user);
            if (TEXU_OK == rc)
            {
                rc = _texu_tree_populate_in(
                    tree, item->next, proc, user);
            }
        }
    }
    return rc;
}

texu_status
_texu_tree_populate_post(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user)
{
    texu_longptr rc = TEXU_OK;
    if (item && proc)
    {
        rc = _texu_tree_populate_post(
            tree, item->firstchild, proc, user);
        if (TEXU_OK == rc)
        {
            rc = _texu_tree_populate_post(
                tree, item->next, proc, user);
            if (TEXU_OK == rc)
            {
                rc = proc(item, user);
            }
        }
    }
    return rc;
}

texu_tree *
texu_tree_new()
{
    texu_tree *tree = (texu_tree *)malloc(sizeof(texu_tree));
    if (tree)
    {
        memset(tree, 0, sizeof(texu_tree));
        tree->root = _texu_tree_item_new(0);
        if (!tree->root)
        {
            free(tree);
            return 0;
        }
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_init(&tree->mutex, NULL);
#endif
    }
    return tree;
}

void
texu_tree_del(texu_tree *tree)
{
    if (tree)
    {
        texu_tree_remove_item(tree, tree->root);
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_destroy(&tree->mutex);
#endif
        free(tree);
        tree = 0;
    }
}

texu_tree_item *
texu_tree_add_item(texu_tree *tree, texu_tree_item *parent, texu_longptr data)
{
    texu_tree_item *newitem = _texu_tree_item_new(data);
    if (!newitem)
    {
        return 0;
    }
    if (!parent)
    {
        parent = tree->root;
    }

    newitem->parent = parent;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&tree->mutex);
#endif
    if (!parent->firstchild)
    {
        parent->firstchild = newitem;
        parent->lastchild = newitem;
    }
    else
    {
        newitem->prev = parent->lastchild;
        parent->lastchild->next = newitem;
        parent->lastchild = newitem;
    }
    /* level and children counted */
    newitem->nlevel = (1 + parent->nlevel);
    ++parent->nchildren;

    /* items counted */
    ++tree->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&tree->mutex);
#endif
    return newitem;
}

texu_status
texu_tree_remove_item(texu_tree *tree, texu_tree_item *delitem)
{
    return texu_tree_cb_remove_item(tree, delitem, 0, 0);
}

texu_status
texu_tree_cb_remove_item(
    texu_tree *tree,
    texu_tree_item *delitem,
    void (*cb)(texu_longptr, void *),
    void *user)
{
    texu_tree_item *parent = 0;
    texu_tree_item *prev = 0;
    texu_tree_item *next = 0;
    if (!delitem)
    {
        return TEXU_OK;
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&tree->mutex);
#endif
    /* save infos */
    parent = delitem->parent;
    next = delitem->next;
    prev = delitem->prev;

    if (parent && parent->firstchild == delitem)
    {
        /* delete at the first child */
        parent->firstchild = parent->firstchild->next;
        if (parent->firstchild)
        {
            parent->firstchild->prev = 0;
        }
    }
    else if (parent && parent->lastchild == delitem)
    {
        /* delete at the last child */
        parent->lastchild = parent->lastchild->prev;
        if (parent->lastchild)
        {
            parent->lastchild->next = 0;
        }
    }
    else
    {
        /* another child to be deleted */
        if (next)
        {
            next->prev = prev;
        }
        if (prev)
        {
            prev->next = next;
        }
    }
    /* prompt to delete the item */
    _texu_tree_cb_remove_item(tree, delitem, cb, user);
    /* decrease children count */
    if (parent)
    {
        --parent->nchildren;
    }
    --tree->nitems;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&tree->mutex);
#endif

    return TEXU_OK;
}

texu_tree_item *texu_tree_find_item(
    texu_tree *tree,
    texu_longptr data,
    texu_i32 (*cmp)(texu_longptr, texu_longptr, void *),
    void *user)
{
    if (!cmp)
    {
        return 0;
    }
    return texu_tree_find_next_item(tree, tree->root->firstchild, data, cmp, user);
}

texu_tree_item *texu_tree_find_next_item(
    texu_tree *tree,
    texu_tree_item *curitem,
    texu_longptr data,
    texu_i32 (*cmp)(texu_longptr, texu_longptr, void *),
    void *user)
{
    texu_tree_item *founditem = 0;
    texu_longptr rc = -1;

    if (curitem)
    {
        if (cmp)
        {
            rc = cmp(curitem->data, data, user);
        }
        else
        {
            rc = (curitem->data == data);
        }
        if (0 == rc)
        {
            return curitem;
        }
        founditem = texu_tree_find_next_item(
            tree,
            curitem->firstchild,
            data,
            cmp,
            user);
        if (!founditem)
        {
            founditem = texu_tree_find_next_item(
                tree,
                curitem->next,
                data,
                cmp,
                user);
        }
    }
    return founditem;
}

texu_status
texu_tree_populate(
    texu_tree *tree,
    texu_tree_item *item,
    texu_longptr order,
    texu_longptr (*proc)(texu_tree_item *, void *),
    void *user)
{
    texu_longptr rc = TEXU_OK;

    if (!item)
    {
        item = tree->root;
    }
    if (order < 0)
    {
        rc = _texu_tree_populate_pre(
            tree, item, proc, user);
    }
    else if (order > 0)
    {
        rc = _texu_tree_populate_post(
            tree, item, proc, user);
    }
    else
    {
        rc = _texu_tree_populate_in(
            tree, item, proc, user);
    }
    return rc;
}

texu_tree_item *
texu_tree_get_root(texu_tree *tree)
{
    return tree->root;
}

texu_ui64
texu_tree_count(texu_tree *tree)
{
    return tree->nitems;
}

/*
# TexU map
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_map
{
    texu_map_keyval *keyvals;
    texu_longptr nitems;
    texu_longptr nkeys;
    texu_longptr ndels;
    texu_bool sorted;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_t mutex;
#endif
};
#define TEXU_MAP_INCREMENTAL_ITEMS 256

int _texu_map_cmp(const void *, const void *);
texu_map_keyval *_texu_map_find(texu_map *, texu_longptr);
texu_map_keyval *_texu_map_cmp_find(texu_map *map, texu_longptr key, int (*cmp)(const void *, const void *));
texu_map_keyval *_texu_map_keyval_init(texu_ui64, texu_map_keyval *);
void _texu_map_sort(texu_map *map, int (*cmp)(const void *, const void *));

texu_map_keyval *
_texu_map_keyval_init(texu_ui64 nitems, texu_map_keyval *src)
{
    texu_map_keyval *keyvals = 0;
    texu_ui64 size = sizeof(texu_map_keyval) * nitems;
    keyvals = (texu_map_keyval *)malloc(size);
    if (keyvals)
    {
        memset(keyvals, 0, size);
        if (src)
        {
            memcpy(keyvals, src, size);
        }
    }
    return keyvals;
}

void
_texu_map_sort(texu_map *map, int (*cmp)(const void *, const void *))
{
    if (0 == map->nkeys)
    {
        /* not items to be sorted */
        return;
    }
    qsort(map->keyvals, map->nkeys, sizeof(texu_map_keyval),
          (cmp ? cmp : _texu_map_cmp));
    map->sorted = TEXU_TRUE;
}

texu_map *
texu_map_new()
{
    texu_ui64 size = 0;
    texu_map_keyval *keyvals = 0;
    texu_map *map = (texu_map *)malloc(sizeof(texu_map));
    if (map)
    {
        size = sizeof(texu_map_keyval) * TEXU_MAP_INCREMENTAL_ITEMS;
        keyvals = _texu_map_keyval_init(size, 0);
        if (!keyvals)
        {
            free(map);
            return 0;
        }

        memset(map, 0, sizeof(texu_map));
        map->keyvals = keyvals;
        map->nitems = TEXU_MAP_INCREMENTAL_ITEMS;
        map->nkeys = 0;
        map->sorted = TEXU_TRUE;
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_init(&map->mutex, NULL);
#endif
    }
    return map;
}

void
texu_map_del(texu_map *map)
{
    if (map)
    {
        texu_map_free(map);
        free(map->keyvals);
#ifdef TEXU_THREAD_SAFE
        pthread_mutex_destroy(&map->mutex);
#endif
        free(map);
        map = 0;
    }
}

void
texu_map_free(texu_map *map)
{
    texu_map_cb_free(map, 0, 0);
}

void
texu_map_cb_free(
    texu_map *map,
    void (*cb)(texu_longptr, texu_longptr, void *),
    void *user)
{
    texu_ui64 size = 0;
    texu_longptr i = 0;
    if (cb)
    {
        for (i = 0; i < map->nkeys; ++i)
        {
            if (map->keyvals[i].used)
            {
                cb(map->keyvals[i].key, map->keyvals[i].value, user);
            }
        }
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&map->mutex);
#endif
    size = sizeof(texu_map_keyval) * map->nitems;
    memset(map->keyvals, 0, size);
    map->nkeys = 0;
    map->ndels = 0;
    map->sorted = TEXU_TRUE;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&map->mutex);
#endif
}

int
texu_map_strcmp(const void *v1, const void *v2)
{
    texu_map_keyval *kv1 = (texu_map_keyval *)v1;
    texu_map_keyval *kv2 = (texu_map_keyval *)v2;
    
    if (kv1 && kv2)
    {
        if (kv1->used && kv2->used)
        {
            return strcmp((const char *)kv1->key, (const char *)kv2->key);
        }
    }
    return TEXU_CMP_GT;
}

int
texu_map_wstrcmp(const void *v1, const void *v2)
{
    texu_map_keyval *kv1 = (texu_map_keyval *)v1;
    texu_map_keyval *kv2 = (texu_map_keyval *)v2;
    if (kv1 && kv2)
    {
        if (kv1->used && kv2->used)
        {
            return wcscmp((const wchar_t *)kv1->key, (const wchar_t *)kv2->key);
        }
    }
    return TEXU_CMP_GT;
}


int
_texu_map_cmp(const void *v1, const void *v2)
{
    texu_map_keyval *kv1 = (texu_map_keyval *)v1;
    texu_map_keyval *kv2 = (texu_map_keyval *)v2;
    if (kv1 && kv2)
    {
        if (kv1->used && kv2->used)
        {
            return (kv1->key - kv2->key);
        }
    }
    return TEXU_CMP_GT;
}

texu_map_keyval *
_texu_map_find(texu_map *map, texu_longptr key)
{
    return _texu_map_cmp_find(map, key, _texu_map_cmp);
}

texu_map_keyval *
_texu_map_cmp_find(texu_map *map, texu_longptr key, int (*cmp)(const void *, const void *))
{
    texu_map_keyval *keyval = 0;
    texu_map_keyval kv = {key, 0, TEXU_TRUE};
    if (map->nkeys > 0)
    {
        keyval = (texu_map_keyval *)bsearch(
            &kv,
            map->keyvals,
            map->nkeys,
            sizeof(texu_map_keyval),
            (cmp ? cmp : _texu_map_cmp));
    }
    return keyval;
}

texu_status
texu_map_insert(
    texu_map *map,
    texu_longptr key,
    texu_longptr value)
{
    return texu_map_cmp_insert(map, key, value, 0);
}

texu_status
texu_map_cmp_insert(
    texu_map *map,
    texu_longptr key,
    texu_longptr value,
    int (*cmp)(const void *, const void *))
{
    texu_status rc = TEXU_OK;
    texu_map_keyval *kv = 0;
    texu_ui64 nitems = 0;

    /* always sort before inserting the new item */
    _texu_map_sort(map, cmp);

    /* find the key if there is existing one */
    kv = _texu_map_cmp_find(map, key, cmp);
    if (kv)
    {
        /* there is existing one */
        return TEXU_ERROR;
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&map->mutex);
#endif

    if (map->nkeys >= map->nitems)
    {
        /* require the new memory */
        nitems = map->nitems + TEXU_MAP_INCREMENTAL_ITEMS;
        kv = _texu_map_keyval_init(nitems, map->keyvals);
        map->nitems = nitems;
        map->keyvals = kv;
    }

    map->keyvals[map->nkeys].key = key;
    map->keyvals[map->nkeys].value = value;
    map->keyvals[map->nkeys].used = TEXU_TRUE;
    ++map->nkeys;
    map->sorted = TEXU_FALSE;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&map->mutex);
#endif

    return rc;
}

texu_status
texu_map_remove(
    texu_map *map,
    texu_longptr key)
{
    return texu_map_cmp_remove(map, key, 0);
}

texu_status
texu_map_cmp_remove(
    texu_map *map,
    texu_longptr key,
    int (*cmp)(const void *, const void *))
{
    return texu_map_cb_remove(map, key, cmp, 0, 0);
}

texu_status
texu_map_cb_remove(
    texu_map *map,
    texu_longptr key,
    int (*cmp)(const void *, const void *),
    void (*cb)(texu_longptr, texu_longptr, void *),
    void *user)
{
    texu_status rc = TEXU_OK;
    texu_map_keyval *kv = _texu_map_cmp_find(map, key, cmp);

    if (!kv)
    {
        /* not found the key */
        return TEXU_NOTFOUND;
    }

    if (cb)
    {
        cb(kv->key, kv->value, user);
    }
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_lock(&map->mutex);
#endif

    memset(kv, 0, sizeof(texu_map_keyval));
    ++map->ndels;
    map->sorted = TEXU_FALSE;
#ifdef TEXU_THREAD_SAFE
    pthread_mutex_unlock(&map->mutex);
#endif

    if (map->ndels == map->nkeys)
    {
        /* no more items */
        texu_map_cb_free(map, 0, 0);
    }

    return rc;
}

texu_status
texu_map_find(
    texu_map *map,
    texu_longptr key,
    texu_longptr *value)
{
    return texu_map_cmp_find(map, key, value, 0);
}

texu_status
texu_map_cmp_find(
    texu_map *map,
    texu_longptr key,
    texu_longptr *value,
    int (*cmp)(const void *, const void *))
{
    texu_status rc = TEXU_OK;
    texu_map_keyval *kv = 0;

    if (!map)
    {
        return TEXU_NOTFOUND;
    }

    if (!map->sorted)
    {
        _texu_map_sort(map, cmp);
    }

    kv = _texu_map_cmp_find(map, key, cmp);
    if (!kv)
    {
        /* not found the key */
        return TEXU_NOTFOUND;
    }

    if (kv->used)
    {
        *value = kv->value;
    }
    return rc;
}

texu_status texu_map_foreach(
    texu_map *map,
    texu_i32 (*cb)(texu_longptr, texu_longptr, void *),
    int (*cmp)(const void *, const void *),
    void *user)
{
    texu_status rc = TEXU_OK;
    texu_map_keyval *kv = 0;
    texu_longptr i = 0;

    if (!map->sorted)
    {
        _texu_map_sort(map, cmp);
    }

    if (cb)
    {
        for (i = 0; i < map->nkeys; ++i)
        {
            kv = &map->keyvals[i];
            if (kv->used)
            {
                rc = cb(kv->key, kv->value, user);
                if (rc != TEXU_OK)
                {
                    break;
                }
            }
        }
    }
    return rc;
}
/*
# TexU bits
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_bits
{
    texu_ui32           nbits;
    texu_byte*          bytes;
};


texu_bits* texu_bits_new(texu_ui32 nbits)
{
    texu_i32 nbytes = 1 + (nbits / 8);
    texu_bits* bits = (texu_bits*)malloc(sizeof(texu_bits) + nbytes);
    if (bits)
    {
        memset(bits, 0, sizeof(texu_bits));
        bits->nbits  = nbits;
        bits->bytes  = (texu_byte*)(bits + sizeof(texu_bits));
    }
    return bits;
}
void       texu_bits_del(texu_bits* bits)
{
    free(bits);
}
void       texu_bits_set_all(texu_bits* bits, texu_i32 on)
{
    texu_i32 nbytes = 1 + (bits->nbits / 8);
    on = (TEXU_BITS_OFF == on ? 0 : 0xff);
    memset(bits->bytes, on, nbytes);
}

texu_i32   texu_bits_set(texu_bits* bits, texu_i32 at, texu_i32 on)
{
    texu_i32  block = 0;
    texu_i32  pos   = 0;
    texu_byte mask  = 0;
    texu_byte byte  = 0;

    if (at < 0 || at >= bits->nbits)
    {
        return -1; /*bit position is out-of-bound*/
    }

    block = (at / 8); /* 1 byte = 8 bits */
    pos   = at - (block * 8);
    mask  = 0x01 << pos;
    byte  = bits->bytes[block];

    if (on)
    {
        byte |= mask;
    }
    else
    {
        byte &= ~mask;
    }
    
    bits->bytes[block] = byte;
    return 0;
}
texu_i32   texu_bits_get(texu_bits* bits, texu_i32 at)
{
    texu_i32  block = 0;
    texu_i32  pos   = 0;
    texu_byte mask  = 0;

    if (at < 0 || at >= bits->nbits)
    {
        return -1; /*bit position is out-of-bound*/
    }
    block = (at / 8); /* 1 byte = 8 bits */
    pos   = at - (block * 8);
    mask  = 0x01 << pos;
    return (bits->bytes[block] & mask);
}


#ifdef __cplusplus
}
#endif
