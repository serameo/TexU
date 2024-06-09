/*
#
# File Name: texutils.c
# Author: Seree Rakwong
# Date: 08-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <wchar.h>

#include "texutypes.h"
#include "texulib.h"
#include "texutils.h"
#include "texukeys.h"

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
    texu_char *str = (texu_char *)malloc(sizeof(texu_char) * (len + 1));
    if (str)
    {
        memset(str, 0, sizeof(texu_char) * (len + 1));
    }
    return str;
}

size_t
texu_strlen(const texu_char *str)
{
#if (defined WIN32 && defined UNICODE)
    return wcslen(str);
#else
    return strlen(str);
#endif
}

texu_i32
texu_atol(const texu_char *buf)
{
    texu_i32 val = 0;
#if (defined WIN32 && defined UNICODE)
    val = wcstol(buf, NULL, 10);
#else
    val = atol(buf);
#endif
    return val;
}

texu_char*
texu_strchr(const texu_char *str, texu_char ch)
{
#if (defined WIN32 && defined UNICODE)
    return wcschr(str, ch);
#else
    return strchr(str, ch);
#endif
}
#if defined WIN32
texu_char texu_win32_to_ascii(texu_i32 keycode)
{
    UINT scancode = MapVirtualKey(keycode, MAPVK_VK_TO_VSC);
    WORD chars[2];
    int rc = ToAscii(keycode, scancode << 16, 0, chars, 0);
    if (rc > 0)
    {
        return (texu_char)chars[0];
    }
    return 0;
}
texu_char texu_win32_oem_chars(texu_i32 ch)
{
    switch (ch)
    {
    case TVK_OEM_1:
        return '1';
    case TVK_OEM_PLUS:
        return '+';
    case TVK_OEM_COMMA:
        return ',';
    case TVK_OEM_MINUS:
        return '-';
    case TVK_OEM_PERIOD:
        return '.';
    case TVK_OEM_2:
        return '2';
    case TVK_OEM_3:
        return '3';
    }
    return -1;
}
#endif

texu_char*
texu_strrchr(const texu_char *str, texu_char ch)
{
#if (defined WIN32 && defined UNICODE)
    return wcsrchr(str, ch);
#else
    return strrchr(str, ch);
#endif
}

/*float string to long string*/
texu_i32 texu_fs2ls(const texu_char *_sPriceStr, texu_i32 _lSize, texu_i32 _lDec, texu_char *sLongStr_)
{
    texu_i32    lLen = 0;           /*get the real length*/
    texu_char*   psz = (texu_char*)_sPriceStr;  /*pointing to the input */
    texu_i32    fNeg = 0;       /*negative flag*/
    texu_i32    fDot = 0;       /*decimal flag*/
    texu_char    szLongStr[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT(""); /*conversion from price string to long string*/
    texu_char    szPriceStrNoComma[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT(""); /*price string without ','*/
    texu_char*   pszLongStr = szLongStr;

    memset(szPriceStrNoComma, 0, sizeof(szPriceStrNoComma));
    /*remove ','*/
    while (lLen < _lSize   &&
           *psz)
    {
        if (TEXUTEXT(',') != *psz)
        {
            szPriceStrNoComma[lLen] = *psz;
            ++lLen;
        }
        ++psz;
    }
    /*remove leading unexpected characters until found the first price string*/
    psz = szPriceStrNoComma;        /*re-point to the new price string*/
    while (*psz        &&          /*string is still valid*/
           *psz != TEXUTEXT('-') &&          /*is it a negative?*/
           *psz != TEXUTEXT('.') &&          /*is it a decimal?*/
           (*psz < TEXUTEXT('0') || *psz > TEXUTEXT('9'))  /*is it a number?*/
           )
    {
        ++psz;
    }
    /*limit decimal point*/
    if (_lDec < 0)
    {
        _lDec = 0;
    }
    else if (_lDec > 6)
    {
        _lDec = 6;
    }
    /*check if the character is not null and space*/
    /*also the length counting is not greater than the input length*/
    memset(szLongStr, 0, sizeof(szLongStr));
    while (*psz && TEXUTEXT(' ') != *psz)
    {
        /*is it a negative value?*/
        if (TEXUTEXT('-') == *psz)
        {
            if (0 == fNeg)
            {
                /*it is a negative value*/
                fNeg = 1;
                *pszLongStr = *psz;
                ++pszLongStr;
            }
            else
            {
                /*found another '-'*/
                break;
            }
        }
        /*is it a decimal value*/
        else if (TEXUTEXT('.') == *psz)
        {
            if (0 == fDot)
            {
                /*it is a decimal value*/
                fDot = 1;
            }
            else
            {
                /*found another '.'*/
                break;
            }
        }
        else if (*psz >= TEXUTEXT('0') && *psz <= TEXUTEXT('9'))
        {
            if (fDot)
            {
                if (_lDec <= 0)
                {
                    /*no more parsing the decimal*/
                    break;
                }
                --_lDec;
            }
            *pszLongStr = *psz; /*copy the digit character*/
            ++pszLongStr;
        }
        else
        {
            /*found others*/
            break;
        }
        ++psz;
    }
    /*ETSSUPX-486*/
    /*fill zero to all decimals requested*/
    /*e.g. _sPriceStr = '1.11' and _lDec = 4 */
    /*the output would be sLongStr_ = '11100'*/
    while (_lDec > 0)
    {
        texu_strcat(szLongStr, TEXUTEXT("0"));
        --_lDec;
    }
    /*copy to the output*/
    texu_strcpy(sLongStr_, szLongStr);
    return texu_strlen(sLongStr_);
}

texu_f64
texu_atof(const texu_char *buf)
{
    texu_f64 val = 0;
#if (defined WIN32 && defined UNICODE)
    val = wcstod(buf, NULL);
#else
    val = atof(buf);
#endif
    return val;
}

texu_char*
texu_strcat(texu_char* dest, const texu_char* src)
{
#if (defined WIN32 && defined UNICODE)
    wcscat_s(dest, TEXU_MAX_WNDTEXT, src);
    return dest;
#else
    return strcat(dest, src);
#endif
}

texu_char*
texu_rtrim(texu_char* in)
{
    texu_char sz[TEXU_MAX_BUFFER + 1];
    texu_char* psz = sz;
    texu_char* pin = in;

    while (*pin && ' ' == *pin) ++pin;
    while (*pin)
    {
        *psz = *pin;
        ++pin;
        ++psz;
    }
    *psz = 0;
    texu_strcpy(in, sz);
    return in;
}
texu_char*
texu_ltrim(texu_char* in)
{
    texu_char rsz[TEXU_MAX_BUFFER + 1];
    texu_char* psz;
    texu_strrcpy(rsz, in);  /*reverse*/
    psz = texu_rtrim(rsz);
    texu_strrcpy(in, psz);  /*reverse back*/
    return in;
}
texu_char*
texu_trim(texu_char* in)
{
    texu_ltrim(in);
    texu_rtrim(in);
    return in;
}

texu_char*
texu_strcpy(texu_char* dest, const texu_char *str)
{
#if (defined WIN32 && defined UNICODE)
    wcscpy_s(dest, TEXU_MAX_WNDTEXT, str);
    return dest;
#else
    return strcpy(dest, str);
#endif
}

texu_char*
texu_strtok(texu_char *str, const texu_char *delim)
{
    texu_char *brkb;
#if (defined WIN32 && defined UNICODE)
    return wcstok_s(str, delim, (texu_char**)&brkb);
#else
#if defined __LINUX__
    return strtok_r(str, delim, (char**)&brkb);
#else
    return strtok(str, delim);
#endif
#endif
}

texu_char*
texu_strrcpy(texu_char* dest, const texu_char *str)
{
    texu_char out[TEXU_MAX_WNDTEXT+1];
    texu_i32 i = 0;
    texu_i32 j = 0;
#if (defined WIN32 && defined UNICODE)
    wcscpy_s(out, TEXU_MAX_WNDTEXT, str);
#else
    strcpy(out, str);
#endif
    i = texu_strlen(out);
    for (; i > 0; --i, ++j)
    {
        dest[j] = out[i-1];
    }
    dest[j] = 0;

    return dest;
}

/*
int MultiByteToWideChar(
    [in]            UINT                              CodePage,
    [in]            DWORD                             dwFlags,
    [in]            _In_NLS_string_(cbMultiByte)LPCCH lpMultiByteStr,
    [in]            int                               cbMultiByte,
    [out, optional] LPWSTR                            lpWideCharStr,
    [in]            int                               cchWideChar
    );
*/
#if (defined WIN32)
#if (defined UNICODE)
texu_i32
texu_a2w(wchar_t *out, texu_i32 outlen, const char *ascii, texu_i32 asciilen)
{
    memset(out, 0, sizeof(wchar_t)*outlen);
    return MultiByteToWideChar(CP_ACP,
                               MB_PRECOMPOSED,
                               ascii, asciilen,
                               out, outlen);
}
/*
int WideCharToMultiByte(
[in]            UINT                               CodePage,
[in]            DWORD                              dwFlags,
[in]            _In_NLS_string_(cchWideChar)LPCWCH lpWideCharStr,
[in]            int                                cchWideChar,
[out, optional] LPSTR                              lpMultiByteStr,
[in]            int                                cbMultiByte,
[in, optional]  LPCCH                              lpDefaultChar,
[out, optional] LPBOOL                             lpUsedDefaultChar
);
*/
texu_i32
texu_w2a(char *out, texu_i32 outlen, const wchar_t *wide, texu_i32 widelen)
{
    memset(out, 0, sizeof(char)*outlen);
    return WideCharToMultiByte(CP_ACP,
                               WC_COMPOSITECHECK,
                               wide,
                               widelen,
                               out,
                               outlen,
                               NULL,
                               NULL);
}

#else
texu_i32
texu_a2w(char *out, texu_i32 outlen, const char *ascii, texu_i32 asciilen)
{
    memset(out, 0, outlen); 
    strncpy(out, ascii, outlen);
    return strlen(out);
}
texu_i32
texu_w2a(char *out, texu_i32 outlen, const char *wide, texu_i32 widelen)
{
    memset(out, 0, sizeof(char)*outlen);
    strncpy(out, wide, outlen);
    return strlen(out);
}

#endif
#endif



texu_char *
texu_init_string(const texu_char *str, texu_ui32 len)
{
    texu_char *newstr = texu_alloc_string(len);
    texu_ui32 srclen = texu_strlen(str);
    if (newstr)
    {
        if (srclen < len)
        {
            len = srclen;
        }
        memcpy(newstr, str, sizeof(texu_char)*len);
    }
    return newstr;
}

texu_char *
texu_concat_string(const texu_char *dest, const texu_char *src)
{
    texu_ui32 destlen = texu_strlen(dest);
    texu_ui32 srclen = texu_strlen(src);
    texu_ui32 newlen = destlen + srclen;

    texu_char *newstr = texu_init_string(dest, newlen);
    if (newstr)
    {
        memcpy(&newstr[destlen], src, sizeof(texu_char)*srclen);
    }
    return newstr;
}

texu_char *
texu_copy_string(texu_char *dest, const texu_char *src)
{
    texu_ui32 destlen = texu_strlen(dest);
    texu_ui32 srclen = texu_strlen(src);
    texu_ui32 len = TEXU_MIN(destlen, srclen);

    memset(dest, 0, sizeof(texu_char)*destlen);
    memcpy(dest, src, sizeof(texu_char)*len);
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
    return texu_printf_alignment3(out, in, limit, align, more, -1, -1);
}

texu_status
texu_printf_alignment3(
    texu_char   *out,
    const texu_char *in,
    texu_i32    limit,
    texu_ui32   align,
    texu_bool   more,
    texu_i32    x,
    texu_i32    cx
    )
{
    texu_i32    len     = 0;
    texu_char   text[TEXU_MAX_WNDTEXT + 1];
    texu_i32    mid = 0;

    if (limit < 1)
    {
        texu_strcpy(out, in);
        return TEXU_OK;
    }
    if (x >= 0 && cx > 0)
    {
        if (x + limit > cx)
        {
            limit = cx;
        }
    }
    memset(text, 0, sizeof(texu_char)*(limit + 1));
    memset(out, 0, sizeof(texu_char)*(limit + 1));

    if ((TEXU_ALIGN_CENTER == align) || (TEXU_WS_CENTER & align))
    {
        len = texu_strlen(in);
        if (len >= limit)
        {
            texu_strncpy(out, in, limit);
        }
        else
        {
            memset(out, ' ', limit);
            mid = (limit - len)/2;
            texu_strncpy2(&out[mid], in, len);
        }
    }
    else if ((TEXU_ALIGN_RIGHT == align) || (TEXU_WS_RIGHT & align))
    {
        len = texu_strlen(in);
        if (len >= limit)
        {
            mid = (len - limit);
            texu_strncpy(out, &in[mid], limit);
        }
        else
        {
            memset(out, ' ', limit);
            mid = (limit - len);
            texu_strncpy2(&out[mid], in, len);
            /*memcpy(&out[mid], in, len);*/
        }
    }
    else
    {
        len = texu_strlen(in);
        if (len >= limit)
        {
            texu_strncpy(out, in, limit);
        }
        else
        {
            memset(out, ' ', limit);
            texu_strncpy2(out, in, len);
        }
    }
    /*final trim string*/
#if 0
    if (more != TEXU_FALSE)
    {
        outlen = texu_strlen(out);
        len = texu_strlen(in);

        if (outlen > 3 && ((len > outlen) || (outlen > limit)))
        {
            if ((TEXU_ALIGN_RIGHT == align) || (TEXU_WS_RIGHT & align))
            {
                out[0] = TEXUTEXT('.');
                out[1] = TEXUTEXT('.');
                out[2] = TEXUTEXT('.');
            }
            else/*if ((TEXU_ALIGN_LEFT == align) || (TEXU_WS_LEFT & align))*/
            {
                out[outlen - 1] = TEXUTEXT('.');
                out[outlen - 2] = TEXUTEXT('.');
                out[outlen - 3] = TEXUTEXT('.');
            }
        }
    }
#endif

    return texu_strlen(out);
}

texu_i32
texu_sprintf(texu_char *buf, texu_i32 buflen, const texu_char* format, ...)
{
    texu_i32 rc = 0;
    va_list args;
    va_start(args, format);
#if (defined WIN32 && defined _UNICODE)
    vswprintf(buf, buflen, format, args);
#else
    vsnprintf(buf, buflen, format, args);
#endif
    va_end(args);

    return rc;
}



texu_i32 texu_add_commas(texu_char *commas, texu_i32 outlen, const texu_char *nocommas)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 cnt = 1;
    texu_char *pbuf;
    texu_i32 len = texu_strlen(nocommas) - 1;
    texu_char *psz = (texu_char*)&nocommas[len];
    texu_char* pos = texu_strchr(nocommas, '.');
    texu_char number[TEXU_MAX_WNDTEXT + 1] = "";
    texu_char dec[TEXU_MAX_WNDTEXT + 1] = "";
    texu_char* neg = texu_strchr(nocommas, '-');
    texu_char* plus = texu_strchr(nocommas, '+');
    texu_char rnumber[TEXU_MAX_WNDTEXT + 1] = "";

    if (neg && plus)
    {
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    pbuf = buf;

    memset(commas, 0, outlen);

    if (pos)
    {
        texu_strncpy(number, nocommas, (texu_longptr)(pos)-(texu_longptr)nocommas);
        texu_strrcpy(rnumber, number);
        texu_strcpy(dec, pos);
    }
    else
    {
        texu_strrcpy(rnumber, nocommas);
    }
    len = texu_strlen(rnumber);
    psz = rnumber;

    while (len >= 0)
    {
        if (len > 0 && cnt % 4 == 0 && *psz != TEXUTEXT('-'))
        {
            *pbuf = TEXUTEXT(',');
            ++pbuf;
            ++cnt;
        }
        *pbuf = *psz;
        ++pbuf;
        ++psz;
        --len;
        ++cnt;
    }
    /* save */
    if (commas)
    {
        memset(commas, 0, outlen);
        /* reverse copy */
        texu_strrcpy(commas, buf);
        if (texu_strlen(dec))
        {
            texu_strcat(commas, dec);
        }
    }
    return TEXU_OK;
}

texu_i32 texu_remove_commas(texu_char *nocommas, texu_i32 len, const texu_char *commas)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char *psz = (texu_char*)commas;
    texu_char *pbuf = buf;


    if (texu_strchr(psz, TEXUTEXT(',')))
    {
        memset(buf, 0, sizeof(buf));
        while (*psz != 0 && *psz != TEXUTEXT('.'))
        {
            if (*psz != TEXUTEXT(','))
            {
                *pbuf = *psz;
                ++pbuf;
            }
            ++psz;
        }
        while (*psz != 0)
        {
            *pbuf = *psz;
            ++pbuf;
            ++psz;
        }
    }
    /* save back to output*/
    if (nocommas)
    {
        texu_strncpy(nocommas, buf, len);
    }
    return TEXU_OK;
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
void _texu_xcnf_free_map(texu_longptr, texu_longptr, void *);

texu_status
_texu_xcnf_parse(texu_xcnf *xcnf, texu_char *line, texu_char *key, texu_char *val)
{
    texu_status rc = TEXU_OK;
    texu_char *tok;
    texu_char *q1 = 0;
    texu_char *q2 = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    texu_char* psz = (texu_char*)buf;

    /* empty string or a comment line, then ignore it */
    if (0 == texu_strlen(line) || 
        TEXUTEXT(';') == line[0] || 
        TEXUTEXT('#') == line[0] || 
        TEXUTEXT('!') == line[0])
    {
        return TEXU_XCNF_SKIP;
    }
    /* white spaces line */
    texu_strcpy(buf, line);
    tok = texu_strtok(buf, TEXUTEXT(" \t\r\n"));

    if (!tok)
    {
        return TEXU_XCNF_SKIP;
    }

    /* check a simple statement: key = "value" */
    tok = texu_strchr(line, TEXUTEXT('='));
    if (!tok)
    {
        return TEXU_XCNF_NOTFOUND_EQUAL_SIGN;
    }
    /*2 double-quotes are required*/
    q1 = texu_strchr(line, TEXUTEXT('"'));
    q2 = texu_strrchr(line, L'"');

    if (!q1 || !q2 || q1 == q2)
    {
        return TEXU_XCNF_NOTFOUND_VALUE;
    }

    /* read a key */
    psz = (texu_char*)line;
    texu_strtok(psz, TEXUTEXT(" =\t\r\n"));

    if (tok)
    {
        texu_strcpy(key, tok);
    }
    /* read a double-quote sign if a value is a string */
    texu_strncpy(val, (texu_char *)(q1 + 1), (size_t)(q2 - q1 - 1));

    return rc;
}

texu_char *texu_strncpy(texu_char *dest, const texu_char *src, size_t size)
{
    texu_char out[TEXU_MAX_WNDTEXT + 1];

    memset(out, 0, sizeof(out));
#if (defined WIN32 && defined UNICODE)
    wcsncpy_s(out, sizeof(texu_char)*(texu_strlen(src)+1), src, size);
#else
    strncpy(out, src, size);
#endif
    texu_strcpy(dest, out);
    return dest;
}

texu_char *texu_strncpy2(texu_char *dest, const texu_char *src, size_t size)
{
    texu_char out[TEXU_MAX_WNDTEXT + 1];

    memset(out, 0, sizeof(out));
#if (defined WIN32 && defined UNICODE)
    wcsncpy_s(out, sizeof(texu_char)*(texu_strlen(src)+1), src, size);
#else
    strncpy(out, src, size);
#endif
    texu_memcpy(dest, out, texu_strlen(out));
    return dest;
}

texu_char *texu_strnrcpy(texu_char *dest, const texu_char *src, size_t size)
{
    texu_char out[TEXU_MAX_WNDTEXT + 1];
    texu_char in[TEXU_MAX_WNDTEXT + 1];

    memset(out, 0, sizeof(out));
    memset(in, 0, sizeof(int));
    texu_strrcpy(in, src);
#if (defined WIN32 && defined UNICODE)
    wcsncpy_s(out, sizeof(texu_char)*(texu_strlen(in)+1), in, size);
#else
    strncpy(out, in, size);
#endif
    texu_strrcpy(dest, out);
    return dest;
}

texu_char *
texu_memset(texu_char *dest, texu_char ch, size_t len)
{
#if (defined WIN32 && defined UNICODE)
    return (texu_char*)wmemset(dest, ch, len);
#else
    return (texu_char*)memset(dest, ch, len);
#endif
}

texu_char *
texu_memcpy(texu_char *dest, const texu_char *src, size_t len)
{
#if (defined WIN32 && defined UNICODE)
    return (texu_char*)wmemcpy(dest, src, len);
#else
    return (texu_char*)memcpy(dest, src, len);
#endif
}


texu_i32
texu_memcmp(const texu_char *dest, const texu_char *src, size_t len)
{
#if (defined WIN32 && defined UNICODE)
    return wmemcmp(dest, src, len);
#else
    return memcmp(dest, src, len);
#endif
}


texu_i32
texu_strcmp(const texu_char *s1, const texu_char *s2)
{
#if (defined WIN32 && defined UNICODE)
    return wcscmp(s1, s2);
#else
    return strcmp(s1, s2);
#endif
}

texu_i32
texu_strncmp(const texu_char *s1, const texu_char *s2, texu_i32 len)
{
#if (defined WIN32 && defined UNICODE)
    return wcsncmp(s1, s2, len);
#else
    return strncmp(s1, s2, len);
#endif
}

/*
texu_xcnf
*/

void
_texu_xcnf_free_map(texu_longptr key, texu_longptr val, void *user)
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
            _texu_xcnf_free_map,
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
#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
        if (!fgetws(line, TEXU_MAX_WNDTEXT, fp))
#else
        if (!fgets(line, TEXU_MAX_WNDTEXT, fp))
#endif
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
        ky = (texu_char *)malloc(sizeof(texu_char) * (texu_strlen(key) + 1));
        vl = (texu_char *)malloc(sizeof(texu_char)* (texu_strlen(val) + 1));
        texu_strcpy(ky, key);
        texu_strcpy(vl, val);
        texu_map_cmp_insert(xcnf->map, (texu_lparam)ky, (texu_lparam)vl, texu_map_strcmp);
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

    rc = texu_map_cmp_find(xcnf->map, (texu_lparam)key, (texu_lparam *)&val, texu_map_strcmp);
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
#if (defined WIN32 && defined UNICODE)
        texu_char *endptr;
        return wcstol(val, &endptr, 10);
#else
        return atol(val);
#endif
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
#if (defined WIN32 && defined UNICODE)
        texu_char *endptr;
        return wcstod(val, &endptr);
#else
        return atof(val);
#endif
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


static int _texu_safedb_has_table(void* args, int argc, char** argv, char** colname)
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
    rc = sqlite3_exec(_db->db, zSql, _texu_safedb_has_table, &row, &zErrMsg);
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
