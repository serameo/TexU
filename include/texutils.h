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

texu_char*         texu_alloc_string(texu_ui32);
texu_char*         texu_init_string(const texu_char*, texu_ui32);
texu_char*         texu_copy_string(texu_char*, const texu_char*);
texu_char*         texu_concat_string(const texu_char*, const texu_char*);

texu_status        texu_printf_alignment(texu_char*, const texu_char*, texu_i32, texu_ui32);
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
typedef struct texu_db texu_db;;



#ifdef __cplusplus
}
#endif

#endif /* _TEXUTILS_H_ */

