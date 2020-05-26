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
extern "C" {
#endif
/*
# TexU string
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_char*
texu_alloc_string(texu_ui32 len)
{
  texu_char* str = (texu_char*)malloc(len + 1);
  if (str)
  {
    memset(str, 0, len+1);
  }
  return str;
}

texu_char*
texu_init_string(const texu_char* str, texu_ui32 len)
{
  texu_char* newstr = texu_alloc_string(len);
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

texu_char*
texu_concat_string(const texu_char* dest, const texu_char* src)
{
  texu_ui32 destlen = strlen(dest);
  texu_ui32 srclen = strlen(src);
  texu_ui32 newlen = destlen + srclen;

  texu_char* newstr = texu_init_string(dest, newlen);
  if (newstr)
  {
    memcpy(&newstr[destlen], src, srclen);
  }
  return newstr;
}

texu_char*
texu_copy_string(texu_char* dest, const texu_char* src)
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
  texu_char* out,
  const texu_char* in,
  texu_i32 limit,
  texu_ui32 align)
{
  texu_i32 len = 0;
  texu_char text[TEXU_MAX_WNDTEXT+1];
  texu_i32 firstlen = 0;
  texu_i32 lastlen = 0;
  
  len = strlen(in);
  if (len > limit)
  {
    len = limit;
  }
  memset(text, 0, limit+1);
  memset(out, 0, limit+1);
  
  if ((TEXU_ALIGN_CENTER == align) || (TEXU_WS_CENTER & align))
  {
    firstlen = (limit - len)/2;
    lastlen  = limit - (len + firstlen);
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

  /*free(text);*/
  return strlen(out);
}

/*
# TexU xcnf
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_xcnf
{
  texu_map*        map;
};

texu_status        _texu_xcnf_parse(texu_xcnf*, texu_char*, texu_char*, texu_char*);
void               _texu_xcn_free_map(texu_i64, texu_i64, void*);



texu_status
_texu_xcnf_parse(texu_xcnf* xcnf, texu_char* line, texu_char* key, texu_char* val)
{
  texu_status rc = TEXU_OK;
  texu_char* tok = 0;
  texu_char* q1 = 0;
  texu_char* q2 = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_char* brkb, *brkl;

  /* empty string or a comment line, then ignore it */
  if (0 == strlen(line) || ';' == line[0] || '#' == line[0] || '!' == line[0])
  {
    return TEXU_XCNF_SKIP;
  }
  /* white spaces line */
  strcpy(buf, line);
  tok = strtok_r(buf, " \t\n", &brkb);
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
  tok = strtok_r(line, " \t=\n", &brkl);
  if (tok)
  {
    strcpy(key, tok);
  }
  /* read a double-quote sign if a value is a string */
  strncpy(val, (char*)(q1+1), (size_t)(q2-q1-1)); 
  
  return rc;
}

void
_texu_xcn_free_map(texu_i64 key, texu_i64 val, void* user)
{
  if (key)
  {
    free((texu_char*)key);
  }
  if (val)
  {
    free((texu_char*)val);
  }
}

texu_xcnf*
texu_xcnf_new()
{
  texu_xcnf* xcnf = (texu_xcnf*)malloc(sizeof(texu_xcnf));
  if (xcnf)
  {
    memset(xcnf, 0, sizeof(texu_xcnf));
    xcnf->map = texu_map_new();
  }
  return xcnf;
}

void
texu_xcnf_del(texu_xcnf* xcnf)
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
texu_xcnf_load(texu_xcnf* xcnf, FILE* fp)
{
  texu_status rc = TEXU_OK;
  texu_char line[TEXU_MAX_WNDTEXT+1];
  texu_char key[TEXU_MAX_WNDTEXT+1];
  texu_char val[TEXU_MAX_WNDTEXT+1];
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
    ky = (texu_char*)malloc(sizeof(texu_char) * (strlen(key) + 1));
    vl = (texu_char*)malloc(sizeof(texu_char) * (strlen(val) + 1));
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

texu_char*
texu_xcnf_get_string(texu_xcnf* xcnf, texu_char* key, texu_char* def)
{
  texu_char* val = 0;
  texu_status rc = TEXU_OK;

  rc = texu_map_cmp_find(xcnf->map, (texu_i64)key, (texu_i64*)&val, texu_map_strcmp);
  if (TEXU_OK == rc)
  {
    return val;
  }
  return def;
}

texu_i64
texu_xcnf_get_int(texu_xcnf* xcnf, texu_char* key, texu_i64 def)
{
  texu_char* val = 0;

  val = texu_xcnf_get_string(xcnf, key, 0);
  if (val)
  {
    return atol(val);
  }
  return def;
}

texu_f64
texu_xcnf_get_float(texu_xcnf* xcnf, texu_char* key, texu_f64 def)
{
  texu_char* val = 0;

  val = texu_xcnf_get_string(xcnf, key, 0);
  if (val)
  {
    return atof(val);
  }
  return def;
}


/*
# TexU dblog
#
# This object is required sqlite
#
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

#define TEXU_DBLOG_CREATE_LOG \
"CREATE TABLE IF NOT EXISTS DBLOG.%q ("\
" LOGNO    INTEGER PRIMARY KEY ASC AUTOINCREMENT,"\
" LOGTIME  TEXT    DEFAULT CURRENT_TIMESTAMP,"\
" LOGLVL   INTEGER NOT NULL,"\
" LOGCAP   TEXT    NOT NULL,"\
" LOGINFO  TEXT    NOT NULL"\
");"

#define TEXU_DBLOG_DROP_LOG \
"DROP TABLE IF EXISTS DBLOG.%q;"

#define TEXU_DBLOG_INSERT_LOG \
"INSERT INTO DBLOG.%q (LOGLVL, LOGCAP, LOGINFO)"\
" VALUES(%d, '%q', '%q');"

#define TEXU_DBLOG_SELECT_LOG \
"SELECT * FROM DBLOG.%q WHERE LOGTIME BETWEEN '%q' AND '%q';"

#define TEXU_DBLOG_ATTACH_LOG \
"ATTACH DATABASE '%q' AS 'DBLOG';"

struct texu_dblog
{
  sqlite3*         db;
  texu_i32         nlevel;
};

texu_status        _texu_dblog_sql_exec(texu_dblog* log, texu_char* zsql);
texu_status        _texu_dblog_attach(texu_dblog*, texu_char*);

texu_status
_texu_dblog_sql_exec(texu_dblog* log, texu_char* zsql)
{
  texu_status rc = TEXU_OK;
  char* zmsg = 0;
  /*const char* zerr = 0;*/
  rc = sqlite3_exec(log->db, zsql, 0, 0, &zmsg);

  if (rc != SQLITE_OK)
  {
    sqlite3_free(zmsg);
    /*zerr = sqlite3_errstr(rc);*/
  }
  return rc;
}

texu_dblog*
texu_dblog_new(texu_i32 nlevel)
{
  texu_dblog* log = (texu_dblog*)malloc(sizeof(texu_dblog));
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
texu_dblog_del(texu_dblog* log)
{
  if (log)
  {
    texu_dblog_close(log);
    free(log);
    log = 0;
  }
}

texu_status
_texu_dblog_attach(texu_dblog* log, texu_char* dbfile)
{
  texu_status rc = TEXU_OK;
  char* zsql = 0;

  /* attach the db file as a DBLOG schema */
  zsql = sqlite3_mprintf(TEXU_DBLOG_ATTACH_LOG, dbfile);
  rc = _texu_dblog_sql_exec(log, zsql);
  sqlite3_free(zsql);
  return rc;
}

texu_status
texu_dblog_open(texu_dblog* log, texu_char* dbfile)
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
texu_dblog_close(texu_dblog* log)
{
  return sqlite3_close_v2(log->db);
}

texu_status
texu_dblog_log(
  texu_dblog* log,
  texu_char* name,
  texu_i32 nlevel,
  texu_char* caption,
  texu_char* info)
{
  texu_status rc = TEXU_OK;
  char* zsql = 0;

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
texu_dblog_create_logname(texu_dblog* log, texu_char* name)
{
  char* zsql = sqlite3_mprintf(TEXU_DBLOG_CREATE_LOG, name);
  texu_status rc = _texu_dblog_sql_exec(log, zsql);
  sqlite3_free(zsql);
  return rc;
}

texu_status
texu_dblog_drop_logname(texu_dblog* log, texu_char* name)
{
  char* zsql = sqlite3_mprintf(TEXU_DBLOG_DROP_LOG, name);
  texu_status rc = _texu_dblog_sql_exec(log, zsql);
  sqlite3_free(zsql);
  return rc;
}











#ifdef __cplusplus
}
#endif

