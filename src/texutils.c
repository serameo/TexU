/*
#
# File Name: texutils.c
# Author: Seree Rakwong
# Date: 08-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include "texulib.h"
#include "texutils.h"

#ifdef __cplusplus
extern "C"
{
#endif
/*
# TexU string
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_char *
texu_alloc_string(texu_ui32 len)
{
    texu_char *str = (texu_char *)malloc(len + 1);
    if (str)
    {
        memset(str, 0, len + 1);
    }
    return str;
}

texu_char *
texu_init_string(const texu_char *str, texu_ui32 len)
{
    texu_char *newstr = texu_alloc_string(len);
    texu_ui32 srclen = strlen(str);
    if (newstr)
    {
        if (srclen < len)
        {
            len = srclen;
        }
        memcpy(newstr, str, len);
    }
    return newstr;
}

texu_char *
texu_concat_string(const texu_char *dest, const texu_char *src)
{
    texu_ui32 destlen = strlen(dest);
    texu_ui32 srclen = strlen(src);
    texu_ui32 newlen = destlen + srclen;

    texu_char *newstr = texu_init_string(dest, newlen);
    if (newstr)
    {
        memcpy(&newstr[destlen], src, srclen);
    }
    return newstr;
}

texu_char *
texu_copy_string(texu_char *dest, const texu_char *src)
{
    texu_ui32 destlen = strlen(dest);
    texu_ui32 srclen = strlen(src);
    texu_ui32 len = TEXU_MIN(destlen, srclen);

    memset(dest, 0, destlen);
    memcpy(dest, src, len);
    return dest;
}

texu_status
texu_printf_alignment(
    texu_char *out,
    const texu_char *in,
    texu_i32 limit,
    texu_ui32 align)
{
    return texu_printf_alignment2(out, in, limit, align, TEXU_FALSE);
}

texu_status
texu_printf_alignment2(
    texu_char   *out,
    const texu_char *in,
    texu_i32    limit,
    texu_ui32   align,
    texu_bool   more
    )
{
    texu_i32    len     = 0;
    texu_char   text[TEXU_MAX_WNDTEXT + 1];
    texu_i32    firstlen = 0;
    texu_i32    lastlen = 0;
    texu_i32    outlen  = 0;

    if (limit < 1)
    {
        strcpy(out, in);
        return TEXU_OK;
    }
    memset(text, 0, limit + 1);
    memset(out, 0, limit + 1);

    if ((TEXU_ALIGN_CENTER == align) || (TEXU_WS_CENTER & align))
    {
        len = strlen(in);
        if (len > limit)
        {
            len = limit;
        }
        firstlen = (limit - len) / 2;
        lastlen = limit - (len + firstlen);
        if (firstlen > 0 && lastlen > 0)
        {
            sprintf(text, "%*s%s%*s",
                    firstlen, " ",
                    in,
                    lastlen, " ");
        }
        else if (lastlen > 0)
        {
            sprintf(text, "%s%*s",
                    in,
                    lastlen, " ");
        }
        else
        {
            sprintf(text, "%s", in);
        }
        strcpy(out, text);

    }
    else if ((TEXU_ALIGN_RIGHT == align) || (TEXU_WS_RIGHT & align))
    {
        sprintf(text, "%*s",
                (texu_i32)(limit),
                in);
        strcpy(out, text);
    }
    else
    {
        sprintf(text, "%-*s",
                (texu_i32)(limit),
                in);
        strncpy(out, text, limit);
    }
    /*final trim string*/
    if (more != TEXU_FALSE)
    {
        outlen = strlen(out);
        len = strlen(in);

        if (outlen > 3 && ((len > outlen) || (outlen > limit)))
        {
            out[outlen-1] = '.';
            out[outlen-2] = '.';
            out[outlen-3] = '.';
        }
    }

    /*free(text);*/
    return strlen(out);
}

/*
# TexU xcnf
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
/*
; comment 1
! comment 2
# comment 3
key = "value"
*/
struct texu_xcnf
{
    texu_map *map;
};

texu_status _texu_xcnf_parse(texu_xcnf *, texu_char *, texu_char *, texu_char *);
void _texu_xcn_free_map(texu_i64, texu_i64, void *);

texu_status
_texu_xcnf_parse(texu_xcnf *xcnf, texu_char *line, texu_char *key, texu_char *val)
{
    texu_status rc = TEXU_OK;
    char *tok;
    texu_char *q1 = 0;
    texu_char *q2 = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    char *brkb, *brkl;
    char* psz = (char*)buf;

    /* empty string or a comment line, then ignore it */
    if (0 == strlen(line) || ';' == line[0] || '#' == line[0] || '!' == line[0])
    {
        return TEXU_XCNF_SKIP;
    }
    /* white spaces line */
    strcpy(buf, line);
    tok = strtok_r(buf, " \t\n", (char**)&brkb);
    if (!tok)
    {
        return TEXU_XCNF_SKIP;
    }

    /* check a simple statement: key = "value" */
    tok = strchr(line, '=');
    if (!tok)
    {
        return TEXU_XCNF_NOTFOUND_EQUAL_SIGN;
    }
    q1 = strchr(line, '"');
    q2 = strrchr(line, '"');
    if (!q1 || !q2 || q1 == q2)
    {
        return TEXU_XCNF_NOTFOUND_VALUE;
    }

    /* read a key */
    psz = (char*)line;
    tok = strtok_r(psz, " \t=\n", (char**)&brkl);
    if (tok)
    {
        strcpy(key, tok);
    }
    /* read a double-quote sign if a value is a string */
    strncpy(val, (char *)(q1 + 1), (size_t)(q2 - q1 - 1));

    return rc;
}

void
_texu_xcn_free_map(texu_i64 key, texu_i64 val, void *user)
{
    if (key)
    {
        free((texu_char *)key);
    }
    if (val)
    {
        free((texu_char *)val);
    }
}

texu_xcnf *
texu_xcnf_new()
{
    texu_xcnf *xcnf = (texu_xcnf *)malloc(sizeof(texu_xcnf));
    if (xcnf)
    {
        memset(xcnf, 0, sizeof(texu_xcnf));
        xcnf->map = texu_map_new();
    }
    return xcnf;
}

void
texu_xcnf_del(texu_xcnf *xcnf)
{
    if (xcnf)
    {
        texu_map_cb_free(
            xcnf->map,
            _texu_xcn_free_map,
            0);
        texu_map_del(xcnf->map);
        free(xcnf);
        xcnf = 0;
    }
}

texu_status
texu_xcnf_load(texu_xcnf *xcnf, FILE *fp)
{
    texu_status rc = TEXU_OK;
    texu_char line[TEXU_MAX_WNDTEXT + 1];
    texu_char key[TEXU_MAX_WNDTEXT + 1];
    texu_char val[TEXU_MAX_WNDTEXT + 1];
    texu_char *ky = 0;
    texu_char *vl = 0;

    if (!fp)
    {
        return TEXU_XCNF_FILE_EMPTY;
    }
    while (!feof(fp))
    {
        if (!fgets(line, TEXU_MAX_WNDTEXT, fp))
        {
            break;
        }
        memset(key, 0, sizeof(key));
        memset(val, 0, sizeof(val));
        rc = _texu_xcnf_parse(xcnf, line, key, val);
        if (rc != TEXU_OK)
        {
            if (TEXU_XCNF_SKIP == rc)
            {
                continue;
            }
            break;
        }
        /* insert a new key and value */
        ky = (texu_char *)malloc(sizeof(texu_char) * (strlen(key) + 1));
        vl = (texu_char *)malloc(sizeof(texu_char) * (strlen(val) + 1));
        strcpy(ky, key);
        strcpy(vl, val);
        texu_map_cmp_insert(xcnf->map, (texu_i64)ky, (texu_i64)vl, texu_map_strcmp);
    }

    if (TEXU_XCNF_SKIP == rc)
    {
        rc = TEXU_OK;
    }
    return rc;
}

texu_char *
texu_xcnf_get_string(texu_xcnf *xcnf, texu_char *key, texu_char *def)
{
    texu_char *val = 0;
    texu_status rc = TEXU_OK;

    rc = texu_map_cmp_find(xcnf->map, (texu_i64)key, (texu_i64 *)&val, texu_map_strcmp);
    if (TEXU_OK == rc)
    {
        return val;
    }
    return def;
}

texu_i64
texu_xcnf_get_int(texu_xcnf *xcnf, texu_char *key, texu_i64 def)
{
    texu_char *val = 0;

    val = texu_xcnf_get_string(xcnf, key, 0);
    if (val)
    {
        return atol(val);
    }
    return def;
}

texu_f64
texu_xcnf_get_float(texu_xcnf *xcnf, texu_char *key, texu_f64 def)
{
    texu_char *val = 0;

    val = texu_xcnf_get_string(xcnf, key, 0);
    if (val)
    {
        return atof(val);
    }
    return def;
}
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

#define TEXU_DBLOG_CREATE_LOG                          \
"CREATE TABLE IF NOT EXISTS DBLOG.%q ("            \
" LOGNO    INTEGER PRIMARY KEY ASC AUTOINCREMENT," \
" LOGTIME  TEXT    DEFAULT CURRENT_TIMESTAMP,"     \
" LOGLVL   INTEGER NOT NULL,"                      \
" LOGCAP   TEXT    NOT NULL,"                      \
" LOGINFO  TEXT    NOT NULL"                       \
");"

#define TEXU_DBLOG_DROP_LOG \
"DROP TABLE IF EXISTS DBLOG.%q;"

#define TEXU_DBLOG_INSERT_LOG                        \
"INSERT INTO DBLOG.%q (LOGLVL, LOGCAP, LOGINFO)" \
" VALUES(%d, '%q', '%q');"

#define TEXU_DBLOG_SELECT_LOG \
"SELECT * FROM DBLOG.%q WHERE LOGTIME BETWEEN '%q' AND '%q';"

#define TEXU_DBLOG_ATTACH_LOG \
"ATTACH DATABASE '%q' AS 'DBLOG';"

struct texu_dblog
{
    sqlite3 *db;
    texu_i32 nlevel;
};

texu_status _texu_dblog_sql_exec(texu_dblog *log, texu_char *zsql);
texu_status _texu_dblog_attach(texu_dblog *, texu_char *);

texu_status
_texu_dblog_sql_exec(texu_dblog *log, texu_char *zsql)
{
    texu_status rc = TEXU_OK;
    char *zmsg = 0;
    /*const char* zerr = 0;*/
    rc = sqlite3_exec(log->db, zsql, 0, 0, &zmsg);

    if (rc != SQLITE_OK)
    {
        sqlite3_free(zmsg);
        /*zerr = sqlite3_errstr(rc);*/
    }
    return rc;
}

texu_dblog *
texu_dblog_new(texu_i32 nlevel)
{
    texu_dblog *log = (texu_dblog *)malloc(sizeof(texu_dblog));
    if (log)
    {
        memset(log, 0, sizeof(texu_dblog));
        if (nlevel < TEXU_DBLOG_LEVEL_OFF)
        {
            nlevel = TEXU_DBLOG_LEVEL_OFF;
        }
        else if (nlevel > TEXU_DBLOG_LEVEL_TRACE)
        {
            nlevel = TEXU_DBLOG_LEVEL_TRACE;
        }
        log->nlevel = nlevel;
    }
    return log;
}

void
texu_dblog_del(texu_dblog *log)
{
    if (log)
    {
        texu_dblog_close(log);
        free(log);
        log = 0;
    }
}

texu_status
_texu_dblog_attach(texu_dblog *log, texu_char *dbfile)
{
    texu_status rc = TEXU_OK;
    char *zsql = 0;

    /* attach the db file as a DBLOG schema */
    zsql = sqlite3_mprintf(TEXU_DBLOG_ATTACH_LOG, dbfile);
    rc = _texu_dblog_sql_exec(log, zsql);
    sqlite3_free(zsql);
    return rc;
}

texu_status
texu_dblog_open(texu_dblog *log, texu_char *dbfile)
{
    texu_status rc = TEXU_OK;

    rc = sqlite3_open_v2(dbfile, &log->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
    if (rc != SQLITE_OK)
    {
        sqlite3_close_v2(log->db);
        return TEXU_DBLOG_OPENERROR;
    }
    /* attach the db file as a DBLOG schema */
    rc = _texu_dblog_attach(log, dbfile);
    return rc;
}

texu_status
texu_dblog_close(texu_dblog *log)
{
    return sqlite3_close_v2(log->db);
}

texu_status
texu_dblog_log(
    texu_dblog *log,
    texu_char *name,
    texu_i32 nlevel,
    texu_char *caption,
    texu_char *info)
{
    texu_status rc = TEXU_OK;
    char *zsql = 0;

    if (log->nlevel > TEXU_DBLOG_LEVEL_OFF &&
        nlevel <= log->nlevel &&
        nlevel > TEXU_DBLOG_LEVEL_OFF)
    {
        zsql = sqlite3_mprintf(TEXU_DBLOG_INSERT_LOG, name, nlevel, caption, info);
        rc = _texu_dblog_sql_exec(log, zsql);
        sqlite3_free(zsql);
        return rc;
    }
    return log->nlevel;
}

texu_status
texu_dblog_create_logname(texu_dblog *log, texu_char *name)
{
    char *zsql = sqlite3_mprintf(TEXU_DBLOG_CREATE_LOG, name);
    texu_status rc = _texu_dblog_sql_exec(log, zsql);
    sqlite3_free(zsql);
    return rc;
}

texu_status
texu_dblog_drop_logname(texu_dblog *log, texu_char *name)
{
    char *zsql = sqlite3_mprintf(TEXU_DBLOG_DROP_LOG, name);
    texu_status rc = _texu_dblog_sql_exec(log, zsql);
    sqlite3_free(zsql);
    return rc;
}


/*
# TexU texu_safedb
#
# This object is required sqlite. It wraps around sqlite3 API
#
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789s012345678901234567890
*/

static texu_safedb_status   _texu_safedb_open(texu_safedb_ptr _db, texu_safedb_cstr _dbfile, texu_safedb_ui32 _flags);
static texu_safedb_status texu_safedb_errno = TEXU_SAFEDB_OK;

texu_safedb_status texu_safedb_last_errno()
{
    return texu_safedb_errno;
}

texu_safedb_ptr texu_safedb_new()
{
    texu_safedb_ptr db = (texu_safedb_ptr)malloc(sizeof(texu_safedb_t));
    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (db)
    {
        db->db = 0;
    }
    else
    {
        texu_safedb_errno = TEXU_SAFEDB_EMEM;
    }
    return db;
}

void texu_safedb_del(texu_safedb_ptr _db)
{
    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (_db)
    {
        texu_safedb_close(_db);
        free(_db);
    }
}

texu_safedb_status texu_safedb_open(texu_safedb_ptr _db, texu_safedb_cstr _dbfile)
{
    return _texu_safedb_open(_db, _dbfile, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
}

texu_safedb_status 
texu_safedb_open_read(texu_safedb_ptr _db, texu_safedb_cstr _dbfile)
{
    return _texu_safedb_open(_db, _dbfile, SQLITE_OPEN_READONLY);
}
texu_safedb_status
texu_safedb_open2(
    texu_safedb_ptr _db, 
    texu_safedb_cstr _dbfile, 
    texu_safedb_ui32 _flags)
{
    return _texu_safedb_open(_db, _dbfile, _flags);
}
static texu_safedb_status
_texu_safedb_open(
    texu_safedb_ptr _db, 
    texu_safedb_cstr _dbfile, 
    texu_safedb_ui32 _flags)
{
    texu_safedb_status rc = 0;
    sqlite3 *db;

    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (_db->db)
    {
        return rc;
    }
    rc = sqlite3_open_v2(_dbfile, &db, _flags, 0);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        texu_safedb_errno = TEXU_SAFEDB_ESQL;
        return rc;
    }
    _db->db = db;
    return rc;
}

void texu_safedb_close(texu_safedb_ptr _db)
{
    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (_db->db)
    {
        sqlite3_close(_db->db);
        _db->db = 0;
    }
}

int texu_safedb_exec(
    texu_safedb_ptr _db, 
    texu_safedb_cstr _sqlcmd, 
    texu_safedb_exec_callback _callback, 
    texu_safedb_userptr _userptr)
{
    int rc = 0;
    char *errmsg = 0;
    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (TEXU_SAFEDB_ISNULL(_db->db))
    {
        texu_safedb_errno = TEXU_SAFEDB_ECONSTRUCTOR;
        return SQLITE_ERROR;
    }
    rc = sqlite3_exec(_db->db, _sqlcmd, _callback, _userptr, &errmsg);
    if (rc != SQLITE_OK)
    {
        texu_safedb_errno = TEXU_SAFEDB_ESQL;
        sqlite3_free(errmsg);
    }
    return rc;
}

texu_safedb_sel_ptr
texu_safedb_select(
    texu_safedb_ptr _db, 
    texu_safedb_cstr _sqlcmd, 
    texu_safedb_copy_callback _callback, 
    texu_safedb_ui32 _size)
{
    /*See sqlite doc*/
    sqlite3_stmt *stmt;
    int     rc      = 0; 
    int     ncols   = 0;
    long    col     = 0;
    const unsigned char*   value   = 0;

    texu_safedb_sel_ptr  sel     = 0;
    texu_safedb_status   rc2     = 0;
    texu_safedb_ui32     nrows   = 0;
    texu_safedb_ui32     row     = 0;
    texu_safedb_rowptr   item    = 0;
    texu_safedb_rowptr   items   = 0;
    texu_safedb_bool     done    = TEXU_SAFEDB_FALSE;
    texu_safedb_ui32     size    = _size;
    texu_safedb_ui32     nitems  = 0;

    texu_safedb_errno = TEXU_SAFEDB_OK;
/*
L000_INIT:
*/
    if (TEXU_SAFEDB_ISNULL(_callback) || _size < 1)
    {
        /*required callback and size correctly*/
        texu_safedb_errno = TEXU_SAFEDB_ERROR;
        return 0;   /*NULL*/
    }
/*
L100_PREPARE_SQL:
*/
    rc = sqlite3_prepare_v2(_db->db, _sqlcmd, -1, &stmt, 0);
    /*rc = sqlite3_prepare_v3(_db->db, _sqlcmd, -1, 0, &stmt, 0);*/
    if (rc != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        texu_safedb_errno = TEXU_SAFEDB_ESQL;
        return 0;   /*NULL*/
    }
/*
L200_COUNT_ROWS_RETURNED:
*/
    /*count rows*/
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            break;
        }
        ++nrows;
    } while (1);
    sqlite3_reset(stmt);
/*
L300_INIT_AN_ARRAY:
*/
    /*create a pointer to an array of records*/
    if (nrows > 0)
    {
        size = _size * nrows;
        items = (texu_safedb_rowptr)malloc(size);
        if (items)
        {
            memset(items, 0, size);
        }
    }
    else
    {
        items   = 0;
        texu_safedb_errno = TEXU_SAFEDB_NOROW;
    }
/*
L400_ALLOC_NEW_OBJECT_RETURNED:
*/
    sel = (texu_safedb_sel_ptr)malloc(sizeof(texu_safedb_sel_t));
    if (TEXU_SAFEDB_ISNULL(sel))
    {
        free(items);
        texu_safedb_errno = TEXU_SAFEDB_EMEM;
        return 0;   /*NULL*/
    }
    sel->nrows  = nrows;
    sel->items  = items;
    sel->size   = _size;
    sel->nitems = 0;
    if (0 == nrows)
    {
        sqlite3_finalize(stmt);
        return sel;
    }
/*
L500_COPY_DATA_BY_USER_CALLBACK:
*/
    ncols   = sqlite3_column_count(stmt);
    row     = 0;
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            done = TEXU_SAFEDB_TRUE;
            break;
        }
        item = (char*)(items) + (row * _size);
        for (col = 0; col < ncols; ++col)
        {
            value = sqlite3_column_text(stmt, col);
            rc2 = _callback(item, col, value);
            if (rc2 != 0)
            {
                done = TEXU_SAFEDB_TRUE;
                break;
            }
        }
        ++row;
        ++nitems;
    } while ((row < nrows) && (TEXU_SAFEDB_FALSE == done));
    sel->nitems = nitems;
/*
L600_RELEASE_SQL_STATEMENT:
*/
    sqlite3_finalize(stmt);
/*
L1000_EXIT_FUNCTION:
*/
    return sel;
}

texu_safedb_rowptr
texu_safedb_sel_get(
    texu_safedb_sel_ptr _selptr, 
    texu_safedb_i32 _row)
{
    texu_safedb_rowptr ptr = 0;
    texu_safedb_errno = TEXU_SAFEDB_EOUTBOUND;
    if (_row >= 0 && _selptr && _row < _selptr->nitems)
    {
        ptr = (texu_safedb_rowptr)((char*)(_selptr->items) + (_row * _selptr->size));
        texu_safedb_errno = TEXU_SAFEDB_OK;
    }
    return ptr;
}

void texu_safedb_sel_free(texu_safedb_sel_ptr _selptr)
{
    texu_safedb_errno = TEXU_SAFEDB_OK;
    if (_selptr)
    {
        free(_selptr->items);
        free(_selptr);
    }
}

texu_safedb_float
texu_safedb_aggregate(
    texu_safedb_ptr _db, 
    texu_safedb_cstr _sqlcmd)
{
    sqlite3_stmt *stmt;
    texu_safedb_float agg = 0.0;
    int rc = SQLITE_OK;

    texu_safedb_errno = TEXU_SAFEDB_OK;
    rc = sqlite3_prepare_v2(_db->db, _sqlcmd, -1, &stmt, 0);
    /*rc = sqlite3_prepare_v3(_db->db, _sqlcmd, -1, 0, &stmt, 0);*/
    if (rc != SQLITE_OK)
    {
        texu_safedb_errno = TEXU_SAFEDB_ESQL;
        return 0;
    }
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            texu_safedb_errno = TEXU_SAFEDB_NOROW;
            break;
        }
        agg = sqlite3_column_double(stmt, 0);
    } while (0);
    
    return agg;
}


static int _db_HasTable_callback(void* args, int argc, char** argv, char** colname) /*ETSDEV-794*/
{
    int* row = (int*)args;
    *row = atoi(argv[0]);
    return SQLITE_OK;
}

texu_safedb_bool
texu_safedb_is_existing(texu_safedb_ptr _db, texu_safedb_cstr _tabname)
{
    /*see sqlite3 doc*/
    char  zSql[BUFSIZ];
    const char* sql = "SELECT COUNT(type) FROM sqlite_master WHERE type = 'table' AND name = '%s'";
    char *zErrMsg = 0;
    int rc = SQLITE_ERROR;
    int row = -1;

    texu_safedb_bool fExisting = TEXU_SAFEDB_FALSE;

    sqlite3_snprintf(BUFSIZ, zSql, sql, _tabname);
    rc = sqlite3_exec(_db->db, zSql, _db_HasTable_callback, &row, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        sqlite3_free(zErrMsg);
    }
    fExisting = (row > 0);
    return fExisting;
}
#endif /*DECLARE_SQLITE3*/

#ifdef __cplusplus
}
#endif
