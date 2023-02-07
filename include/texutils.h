/*
#
# File Name: texutils.h
# Author: Seree Rakwong
# Date: 08-MAY-2020
#
*/

#ifndef _TEXUTILS_H_
#define _TEXUTILS_H_

#include <stdio.h>

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
# TexU string
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_char*          texu_alloc_string(texu_ui32);
texu_char*          texu_init_string(const texu_char*, texu_ui32);
texu_char*          texu_copy_string(texu_char*, const texu_char*);
texu_char*          texu_concat_string(const texu_char*, const texu_char*);
size_t              texu_strlen(const texu_char *str);
texu_char          *texu_strcpy(texu_char* dest, const texu_char *str);
texu_char          *texu_strrcpy(texu_char* dest, const texu_char *str); /*reverse copy*/
texu_char          *texu_strncpy(texu_char *dest, const texu_char *src, size_t size);
texu_char          *texu_strnrcpy(texu_char *dest, const texu_char *src, size_t size); /*reverse copy*/
texu_i32            texu_strcmp(const texu_char *s1, const texu_char *s2);
texu_i32            texu_strncmp(const texu_char *s1, const texu_char *s2, texu_i32 len);
texu_char          *texu_strcat(texu_char* dest, const texu_char* src);
texu_char          *texu_strchr(const texu_char *str, texu_char ch);
texu_char *         texu_memset(texu_char *dest, texu_char ch, size_t len);
texu_char *         texu_memcpy(texu_char *dest, const texu_char *src, size_t len);
texu_i32            texu_memcmp(const texu_char *dest, const texu_char *src, size_t len);
/*texu_fs2ls to convert floating-point string to long string*/
texu_i32            texu_fs2ls(const texu_char *_sPriceStr, texu_i32 _lSize, texu_i32 _lDec, texu_char *sLongStr_);
texu_i32            texu_sprintf(texu_char *buf, texu_i32 buflen, const texu_char* format, ...);
texu_i32            texu_add_commas(texu_char *commas, texu_i32 outlen, const texu_char *nocommas);
texu_i32            texu_remove_commas(texu_char *nocommas, texu_i32 len, const texu_char *commas);

#if (defined WIN32)
/*texu_a2w to convert multi-byte string (ascii string) to wide string*/
texu_i32            texu_a2w(wchar_t *out, texu_i32 outlen, const char *ascii, texu_i32 asciilen);
/*texu_w2a to convert wide string to multi-byte string (ascii string)*/
texu_i32            texu_w2a(char *out, texu_i32 outlen, const wchar_t *wide, texu_i32 widelen);
#endif

texu_i32            texu_atol(const texu_char *buf);
texu_f64            texu_atof(const texu_char *buf);

texu_status         texu_printf_alignment(texu_char*, const texu_char*, texu_i32, texu_ui32);
texu_status         texu_printf_alignment2(texu_char*, const texu_char*, texu_i32, texu_ui32, texu_bool);
texu_status         texu_printf_alignment3(texu_char*, const texu_char*, texu_i32, texu_ui32, texu_bool, texu_i32 x, texu_i32 cx);
/*
# TexU xcnf
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

#define TEXU_XCNF_SKIP                 (-1)
#define TEXU_XCNF_FILE_EMPTY           (1)
#define TEXU_XCNF_NOTFOUND_KEY         (2)
#define TEXU_XCNF_NOTFOUND_EQUAL_SIGN  (3)
#define TEXU_XCNF_NOTFOUND_VALUE       (4)

struct texu_xcnf;
typedef struct texu_xcnf texu_xcnf;

texu_xcnf*         texu_xcnf_new();
void               texu_xcnf_del(texu_xcnf*);

texu_status        texu_xcnf_load(texu_xcnf*, FILE*);
texu_char*         texu_xcnf_get_string(texu_xcnf*, texu_char*, texu_char*);
texu_i64           texu_xcnf_get_int(texu_xcnf*, texu_char*, texu_i64);
texu_f64           texu_xcnf_get_float(texu_xcnf*, texu_char*, texu_f64);

#ifdef DECLARE_SQLITE3
/*
# TexU dblog
#
# This object is required sqlite
#
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#include <sqlite3.h>

#define TEXU_DBLOG_OPENERROR           (1)

#define TEXU_DBLOG_LEVEL_OFF           (0)
#define TEXU_DBLOG_LEVEL_FATAL         (1)
#define TEXU_DBLOG_LEVEL_SYSTEM        (2)
#define TEXU_DBLOG_LEVEL_WARNING       (3)
#define TEXU_DBLOG_LEVEL_INFORMATION   (4)
#define TEXU_DBLOG_LEVEL_TRACE         (5)
#define TEXU_DBLOG_LEVEL_DEBUG         (6)


struct texu_dblog;
typedef struct texu_dblog texu_dblog;

texu_dblog*        texu_dblog_new(texu_i32);
void               texu_dblog_del(texu_dblog*);

texu_status        texu_dblog_open(texu_dblog*, texu_char*);
texu_status        texu_dblog_close(texu_dblog*);
texu_status        texu_dblog_create_logname(texu_dblog*, texu_char*);
texu_status        texu_dblog_drop_logname(texu_dblog*, texu_char*);
texu_status        texu_dblog_log(
                        texu_dblog*,
                        texu_char*,
                        texu_i32,
                        texu_char*,
                        texu_char*);


/*
# TexU db
#
# This object is required sqlite. It wraps around sqlite3 API
#
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_db;
typedef struct texu_db texu_db;


/*
# TexU safedb
#
# This object is required sqlite. It wraps around sqlite3 API
#
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/


#define TEXU_SAFEDB_BREAK        ( 2)
#define TEXU_SAFEDB_NOROW        ( 1)
#define TEXU_SAFEDB_OK           ( 0)
#define TEXU_SAFEDB_ERROR        (-1)
#define TEXU_SAFEDB_EMEM         (-2)
#define TEXU_SAFEDB_ESQL         (-3)
#define TEXU_SAFEDB_ECONSTRUCTOR (-4)
#define TEXU_SAFEDB_EOUTBOUND    (-5)

#define TEXU_SAFEDB_FAILED(e)    ((e) <  0)
#define TEXU_SAFEDB_SUCCEEDED(e) ((e) >= 0)

#define TEXU_SAFEDB_TRUE         (1)
#define TEXU_SAFEDB_FALSE        (0)

#define TEXU_SAFEDB_NULL         ((void*)0)
#define TEXU_SAFEDB_ISNULL(p)    (!(p))

struct _texu_safedb
{
    sqlite3 *db;
};
typedef struct _texu_safedb texu_safedb_t;
typedef struct _texu_safedb *texu_safedb_ptr; /*db*/
typedef void*  texu_safedb_rowptr;       /*row ptr*/
typedef void*  texu_safedb_userptr;      /*user ptr*/

typedef char                    texu_safedb_i8;
typedef short                   texu_safedb_i16;
typedef int                     texu_safedb_i32;
typedef long long               texu_safedb_i64;
typedef unsigned char           texu_safedb_ui8;
typedef unsigned short          texu_safedb_ui16;
typedef unsigned int            texu_safedb_ui32;
typedef unsigned long long      texu_safedb_ui64;
typedef float                   texu_safedb_f32;
typedef double                  texu_safedb_f64;

typedef unsigned char           texu_safedb_byte;
typedef char                    texu_safedb_char;
typedef texu_safedb_i64         texu_safedb_int;
typedef texu_safedb_f64         texu_safedb_float;
typedef texu_safedb_i32         texu_safedb_status;
typedef const char*             texu_safedb_cstr;
typedef char*                   texu_safedb_str;
typedef texu_safedb_i32         texu_safedb_bool;


texu_safedb_ptr     texu_safedb_new();
void                texu_safedb_del(texu_safedb_ptr db);

texu_safedb_status      texu_safedb_open(texu_safedb_ptr db, texu_safedb_cstr dbfile);
texu_safedb_status      texu_safedb_open2(texu_safedb_ptr db, texu_safedb_cstr dbfile, texu_safedb_ui32 flags);
texu_safedb_status      texu_safedb_open_read(texu_safedb_ptr db, texu_safedb_cstr dbfile);
void                    texu_safedb_close(texu_safedb_ptr db);

/*****************************************************************************
CREATE TABLE student(
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    name    TEXT,
    age     REAL,
    height  REAL,
    weight  REAL
    );

INSERT INTO student(name,age,height,weight) VALUES('smith',12.5,157.2,58.2);
INSERT INTO student(name,age,height,weight) VALUES('neo',12.3,158.2,52.3);
INSERT INTO student(name,age,height,weight) VALUES('trinity',12.6,162.2,55.8);
INSERT INTO student(name,age,height,weight) VALUES('morphoues',13.5,163.9,65.3);
INSERT INTO student(name,age,height,weight) VALUES('oracle',33.8,160.9,60.3);
INSERT INTO student(name,age,height,weight) VALUES('tank',10.5,152.9,45.3);

UPDATE student SET name='smith' WHERE id=1;

DELETE student WHERE id=6;
*****************************************************************************/
/* See sqlite3 document*/
typedef int (*texu_safedb_exec_callback)(void* userptr, int argc, char **argv, char **name);
int         texu_safedb_exec(texu_safedb_ptr db, texu_safedb_cstr sqlcmd, texu_safedb_exec_callback callback, texu_safedb_userptr userptr);

/*****************************************************************************
Example to call copy from callback function
struct _student
{
    long    id;
    char    name[32];
    double  age;
    double  height;
    double  weight;
};
typedef struct _student student_t;

texu_safedb_status copy_student_callback(texu_safedb_userptr userptr, texu_safedb_i32 col, const texu_safedb_byte* value)
{
    student_t *student = (student_t*)userptr;
    char* val = (char*)value;
    switch(col)
    {
        case 1: strcpy(student->name, (val ? val   : ""));      break;
        case 0: student->id     = (val ? atol(val) : 0);        break;
        case 2: student->age    = (val ? atof(val) : 0);        break;
        case 3: student->height = (val ? atof(val) : 0);        break;
        case 4: student->weight = (val ? atof(val) : 0);        break;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    const char *sqlcmd = "SELECT * FROM student WHERE AGE > 7;";
    texu_safedb_ptr  db = texu_safedb_new();
    texu_safedb_status rc = texu_safedb_open(db, "./student.db");
    if (TEXU_SAFEDB_OK != rc)
    {
        return 1;
    }

    texu_safedb_sel_ptr selptr = texu_safedb_select(db, sqlcmd, copy_student_callback, sizeof(student_t));
    if (selptr)
    {
        if (selptr->nitems > 0 && selptr->items)
        {
            texu_safedb_ui32 rows = selptr->nitems;
            student_t *student = 0;
            texu_safedb_i32 i = 0;
            for (i = 0; i < rows; ++i)
            {
                student = (student_t *)texu_safedb_sel_get(selptr, i);
                printf("id:%ld, name:'%s', height: %f, weight: %f\n",
                    student->id, student->name, student->height, student->weight);
            }
        }
        else
        {
            printf("data not found\n");
        }
    }
    else
    {
        printf("statement is incorrect or table not found\n");
    }

    texu_safedb_close(db);
    texu_safedb_del(db);
    return 0;
}
*****************************************************************************/
/*
texu_safedb_status texu_safedb_copy_callback(P1, P2, P3)
PURPOSE:
    To copy a row data from DB by calling texu_safedb_select()

RETURN:
    TEXU_SAFEDB_OK to continue copy data from DB. Otherwise break the copy from DB.

PARAMETERS:
    P1 - to copy for each row from DB
    P2 - the DB column index by the zero based
    P3 - the DB value for each column returned by selecting
*/
typedef texu_safedb_status (*texu_safedb_copy_callback)(texu_safedb_userptr userptr, texu_safedb_i32 col, const texu_safedb_byte* value);

struct _texu_safedb_select
{
    texu_safedb_ui32     size;   /*size of an item per record*/
    texu_safedb_ui32     nrows;  /*a number of rows*/
    texu_safedb_rowptr   items;  /*an array of items*/
    texu_safedb_ui32     nitems; /*an actual items, it may not be equal nrows*/
                            /*it depends on the implementation of callback*/
                            /*it always not be greater than nrows*/
};
typedef struct _texu_safedb_select texu_safedb_sel_t;
typedef struct _texu_safedb_select *texu_safedb_sel_ptr;
/*****************************************************************************
texu_safedb_select() would return an array of records
SQL Example:
    const char *sqlcmd = "SELECT * FROM STUDENT_TABLE WHERE AGE > 7;";

Return:
    texu_safedb_sel_ptr if sqlcmd was successfully called. Please double-check nrows and items if there
    is no row returned.
    Otherwise, NULL if sqlcmd is incorrect statement. Such as no table in database and so on.

Example:
    texu_safedb_sel_ptr selptr = texu_safedb_select(db, sqlcmd, copy_student_callback, sizeof(student_t));
    if (selptr)
    {
        if (selptr->nitems > 0 && selptr->items)
        {
            printf("data found\n");
        }
        else
        {
            printf("data not found\n");
        }
    }
    else
    {
        printf("statement is incorrect or table not found\n");
    }
*****************************************************************************/
texu_safedb_sel_ptr  texu_safedb_select(texu_safedb_ptr db, texu_safedb_cstr sqlcmd, texu_safedb_copy_callback callback, texu_safedb_ui32 size);
/*
texu_safedb_sel_get() to read a specific record with a zero base
Example:
    student_t *student = texu_safedb_sel_get(selptr, 0);
*/
texu_safedb_rowptr   texu_safedb_sel_get(texu_safedb_sel_ptr selptr, texu_safedb_i32 row);
/*
texu_safedb_sel_free() to release texu_safedb_sel_ptr object
*/
void            texu_safedb_sel_free(texu_safedb_sel_ptr selptr);
/*****************************************************************************
texu_safedb_aggregate() would only one aggregate SQL function
SQL Example:
    const char *sqlcmd1 = "SELECT COUNT(*) FROM student;";
    const char *sqlcmd2 = "SELECT AVG(age) FROM student;";
    const char *sqlcmd3 = "SELECT MAX(age) FROM student;";
    const char *sqlcmd4 = "SELECT SUM(age) FROM student;";

Example:
    long    count_students  = (long)texu_safedb_aggregate(db, sqlcmd1);
    double  avg_age         = texu_safedb_aggregate(db, sqlcmd2);
    double  max_age         = texu_safedb_aggregate(db, sqlcmd3);
    double  sum_age         = texu_safedb_aggregate(db, sqlcmd4);
*****************************************************************************/
texu_safedb_float    texu_safedb_aggregate(texu_safedb_ptr db, texu_safedb_cstr sqlcmd);

texu_safedb_status   texu_safedb_last_errno();
texu_safedb_bool     texu_safedb_is_existing(texu_safedb_ptr db, texu_safedb_cstr tabname);

#endif /*DECLARE_SQLITE3*/

#ifdef __cplusplus
}
#endif


#endif /* _TEXUTILS_H_ */

