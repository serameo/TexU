#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texu.h"
#include "texulib.h"
#include "texutils.h"

char *s[] =
    {
        "Hello world",
        "Aloha",
        "Sawasdee",
        "Magenta",
        "Navy blue",
        "Channel Five",
        "Banana",
        "MacOS"};
/*
char* s1 = "Hello world";
char* s2 = "Aloha";
char* s3 = "Sawasdee";
char* s4 = "Magenta";
char* s5 = "Navy blue";
char* s6 = "Channel Five";
char* s7 = "Banana";
char* s8 = "MacOS";
*/

void test_list()
{
    texu_list *list = texu_list_new();
    texu_list_item *item = 0;
    texu_i32 i = 0;

    for (i = 0; i < 8; ++i)
    {
        texu_list_add(list, (texu_lparam)s[i]);
    }

    printf("Printing by the list...\n");
    item = texu_list_first(list);
    while (item)
    {
        printf("%s\n", (char *)item->data);
        item = item->next;
    }

    texu_list_del(list);
}

void test_array()
{
    texu_ui64 i = 0;
    texu_ui64 nitems = 0;
    texu_array *arr = texu_array_new(8);

    for (i = 0; i < 8; ++i)
    {
        texu_array_set(arr, i, (texu_lparam)s[i]);
    }

    printf("Printing by the array...\n");
    nitems = texu_array_count(arr);
    for (i = 0; i < nitems; ++i)
    {
        printf("%s\n", (char *)texu_array_get(arr, i));
    }
    texu_array_del(arr);
}

void test_stack()
{
    texu_stack *stack = texu_stack_new(4);
    texu_i32 i = 0;

    for (i = 0; i < 8; ++i)
    {
        texu_stack_push(stack, (texu_lparam)s[i]);
    }

    printf("Printing by the stack...\n");
    while (!texu_stack_empty(stack))
    {
        printf("%s\n", (char *)texu_stack_top(stack));
        texu_stack_pop(stack);
    }

    texu_stack_del(stack);
}

texu_i64 test_tree_populate(texu_tree_item *item, void *user)
{
    static texu_char buf[BUFSIZ];

    if (item->data)
    {
        memset(buf, 0, sizeof(buf));
        memset(buf, ' ', item->nlevel);

        printf("%s%s\n", buf, (char *)item->data);
    }
    return 0;
}

void test_tree()
{
    texu_tree *tree = texu_tree_new();
    texu_tree_item *item1 = texu_tree_add_item(tree, 0, (texu_lparam)s[0]);
    texu_tree_item *item2 = texu_tree_add_item(tree, item1, (texu_lparam)s[1]);
    texu_tree_item *item3 = texu_tree_add_item(tree, item2, (texu_lparam)s[2]);
    texu_tree_item *item4 = texu_tree_add_item(tree, 0, (texu_lparam)s[3]);
    texu_tree_item *item5 = texu_tree_add_item(tree, item4, (texu_lparam)s[4]);
    texu_tree_item *item6 = texu_tree_add_item(tree, item5, (texu_lparam)s[5]);
    texu_tree_item *item7 = texu_tree_add_item(tree, item6, (texu_lparam)s[6]);
    texu_tree_item *item8 = texu_tree_add_item(tree, item6, (texu_lparam)s[7]);

    printf("Printing tree by pre-order traversal...\n");
    texu_tree_populate(tree, 0, -1, test_tree_populate, tree);

    printf("Printing tree by in-order traversal...\n");
    texu_tree_populate(tree, 0, 0, test_tree_populate, tree);

    printf("Printing tree by post-order traversal...\n");
    texu_tree_populate(tree, 0, 1, test_tree_populate, tree);

    texu_tree_del(tree);
}

void test_map()
{
    texu_map *map = texu_map_new();
    texu_i32 i = 0;
    texu_status rc = TEXU_OK;
    char *val = 0;

    for (i = 0; i < 8; ++i)
    {
        texu_map_insert(map, 8 - i, (texu_lparam)s[i]);
    }

    for (i = 0; i < 10; ++i)
    {
        rc = texu_map_find(map, i, (texu_i64 *)&val);
        if (TEXU_OK == rc)
        {
            printf("FOUND key: %d, val: %s\n", i, val);
        }
        else
        {
            printf("NOT FOUND key: %d\n", i);
        }
    }

    texu_map_del(map);
}

void test_map2()
{
    texu_map *map = texu_map_new();
    texu_status rc = TEXU_OK;
    char *val = 0;
    char *key = 0;

    texu_map_insert(map, (texu_lparam) "green", (texu_lparam) "water melon");
    texu_map_insert(map, (texu_lparam) "yellow", (texu_lparam) "banana");
    texu_map_insert(map, (texu_lparam) "red", (texu_lparam) "apple");
    texu_map_insert(map, (texu_lparam) "orange", (texu_lparam) "orange");

    key = "red";
    rc = texu_map_find(map, (texu_lparam)key, (texu_i64 *)&val);
    if (TEXU_OK == rc)
    {
        printf("FOUND key=%s, val=%s\n", key, val);
    }
    else
    {
        printf("NOT FOUND key=%s\n", key);
    }

    key = "blue";
    rc = texu_map_cmp_find(map, (texu_lparam)key, (texu_i64 *)&val, texu_map_strcmp);
    if (TEXU_OK == rc)
    {
        printf("FOUND key=%s, val=%s\n", key, val);
    }
    else
    {
        printf("NOT FOUND key=%s\n", key);
    }
    texu_map_del(map);
}

void test_xcnf()
{
    texu_status rc = TEXU_OK;
    texu_xcnf *xcnf = 0;
    texu_char *filename = "./abc.cnf";
    /*texu_char* filename = "./def.cnf";*/
    FILE *fp = fopen(filename, "r");
    texu_char *key = 0;
    texu_char *val = 0;
    texu_i64 i64 = 0;
    texu_f64 f64 = 0.0;

    if (!fp)
    {
        printf("cannot open %s\n", filename);
        return;
    }

    xcnf = texu_xcnf_new();
    rc = texu_xcnf_load(xcnf, fp);
    fclose(fp);

    if (rc != TEXU_OK)
    {
        printf("cannot load abc.cnf\n");
        texu_xcnf_del(xcnf);
        return;
    }

    key = "TEXU.DESKTOP.BACKCOLOR";
    val = texu_xcnf_get_string(xcnf, key, "not found backcolor");
    printf("%s=%s\n", key, val);

    key = "TEXU.SCREEN.WIDTH";
    i64 = texu_xcnf_get_int(xcnf, key, -1);
    printf("%s=%d\n", key, i64);

    key = "TEXU.TEXT.SCALE";
    f64 = texu_xcnf_get_float(xcnf, key, 2.25);
    printf("%s=%f\n", key, f64);

    texu_xcnf_del(xcnf);
}

void test_dblog()
{
    texu_char *dbfile = "./test_texu.db";
    texu_char *logname = "mylog";
    texu_status rc = TEXU_OK;

    texu_dblog *log = texu_dblog_new(TEXU_DBLOG_LEVEL_INFORMATION);
    do
    {
        rc = texu_dblog_open(log, dbfile);
        if (rc != TEXU_OK)
        {
            break;
        }
        rc = texu_dblog_create_logname(log, logname);
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_OFF, "OFF", "This log should never be logged");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_FATAL, "FATAL", "A fatal log");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_SYSTEM, "SYSTEM", "A system log");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_WARNING, "WARNING", "A warning log");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_INFORMATION, "INFORMATION", "An information log");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_TRACE, "TRACE", "A traced log");
        rc = texu_dblog_log(log, logname, TEXU_DBLOG_LEVEL_DEBUG, "DEBUG", "A debug log");
    } while (0);

    /*
    #define TEXU_DBLOG_LEVEL_OFF           (0)
    #define TEXU_DBLOG_LEVEL_FATAL         (1)
    #define TEXU_DBLOG_LEVEL_SYSTEM        (2)
    #define TEXU_DBLOG_LEVEL_WARNING       (3)
    #define TEXU_DBLOG_LEVEL_INFORMATION   (4)
    #define TEXU_DBLOG_LEVEL_TRACE         (5)
    #define TEXU_DBLOG_LEVEL_DEBUG         (6)
    */

    rc = texu_dblog_close(log);
    texu_dblog_del(log);
}

int main(int argc, char *argv[])
{
    printf("\nTest list...\n");
    test_list();
    printf("\nTest array...\n");
    test_array();
    printf("\nTest stack...\n");
    test_stack();
    printf("\nTest map...\n");
    test_map();
    printf("\nTest map2...\n");
    test_map2();
    printf("\nTest xcnf...\n");
    test_xcnf();
    printf("\nTest dblog...\n");
    test_dblog();
    printf("\nTest tree...\n");
    test_tree();

    return 0;
}
