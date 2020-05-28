/*
#
# File Name: texuproc.c
# Author: Seree Rakwong
# Date: 15-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include "texulib.h"
#include "texutils.h"
#include "texui.h"
#include "texucio.h"
#include "texust.h"
#include "texuconst.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
#-------------------------------------------------------------------------------
# TexU desktop
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_status        _TexuDesktopProc_OnCreate(texu_wnd*, texu_wnd_attrs*);
texu_bool          _TexuDesktopWndProc_OnEnable(texu_wnd* wnd, texu_bool enable);

texu_status
_TexuDesktopProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_wnd_enable(wnd, TEXU_TRUE);
  texu_wnd_set_color(wnd,
    TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
  return TEXU_OK;
}

texu_bool
_TexuDesktopWndProc_OnEnable(texu_wnd* wnd, texu_bool enable)
{
  return TEXU_TRUE;
}

texu_i64
TexuDesktopProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_ENABLE:
      return _TexuDesktopWndProc_OnEnable(wnd, (texu_bool)param1);

    case TEXU_WM_CREATE:
      return _TexuDesktopProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU label
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

void               _TexuLabelProc_OnPaint(texu_wnd*, texu_cio*);
texu_status        _TexuLabelProc_OnCreate(texu_wnd*, texu_wnd_attrs*);

void
_TexuLabelProc_OnPaint(texu_wnd* wnd, texu_cio* cio)
{
  if (texu_wnd_is_visible(wnd))
  {
    texu_char buf[TEXU_MAX_WNDTEXT+1];
    texu_char text[TEXU_MAX_WNDTEXT+1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_i32 normcolor = TEXU_CIO_COLOR_CYAN_BLACK;
    texu_i32 discolor = TEXU_CIO_COLOR_WHITE_BLACK;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_bool enable = texu_wnd_is_enable(wnd);
    texu_i32 color = TEXU_CIO_COLOR_CYAN_BLACK;

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_printf_alignment(buf, text, width, style);

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = (enable ? normcolor : discolor);
    
    texu_cio_putstr_attr(cio, y, x, buf, COLOR_PAIR(color));
    return;
  }
}

texu_status
_TexuLabelProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_wnd_enable(wnd, TEXU_FALSE);
  texu_wnd_set_color(wnd,
    TEXU_CIO_COLOR_CYAN_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
  return TEXU_OK;
}

texu_i64
TexuLabelProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CREATE:
      return _TexuLabelProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_PAINT:
      _TexuLabelProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}


/*
#-------------------------------------------------------------------------------
# TexU edit
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/



struct texu_editwnd
{
  texu_i32           firstvisit;
  texu_i32           selected;
  texu_char          passchar;
  texu_bool          showpass;
  texu_i32           firstchar;
  texu_i32           limitchars;
  texu_i32           editing;
  texu_i32           decwidth;
  texu_i32           min; 
  texu_i32           max; 
  texu_i32           onminmax;
  void*              exparam;
  texu_char          validstr[TEXU_MAX_WNDTEXT+1];
  texu_char          editbuf[TEXU_MAX_WNDTEXT+1];
};
typedef struct texu_editwnd texu_editwnd;


#define TEXU_EDITWND_KILO      1000 
#define TEXU_EDITWND_MEGA      1000000
#define TEXU_EDITWND_GIGA      1000000000
#define TEXU_EDITWND_TERA      1000000000000
#define TEXU_EDITWND_PETA      1000000000000000
#define TEXU_EDITWND_EXA       1000000000000000000


texu_status        _TexuEditProc_OnCreate(texu_wnd*, texu_wnd_attrs*);
void               _TexuEditProc_OnChar(texu_wnd*, texu_i32 ch);
void               _TexuEditProc_OnDestroy(texu_wnd*);
void               _TexuEditProc_OnPaint(texu_wnd* wnd, texu_cio* dc);
texu_i32           _TexuEditProc_OnKillFocus(texu_wnd* wnd, texu_wnd* nextwnd);
void               _TexuEditProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd);
void               _TexuEditProc_OnLimitText(texu_wnd* wnd, texu_i32 limit);
void               _TexuEditProc_OnSetPasswdChar(texu_wnd* wnd, texu_char passchar);
void               _TexuEditProc_OnShowPasswdChar(texu_wnd* wnd, texu_i32 show);
void               _TexuEditProc_OnSetDecWidth(texu_wnd* wnd, texu_i32 width);
void               _TexuEditProc_OnSetValidMinMax(texu_wnd* wnd, texu_i32 on, texu_editminmax* vmm);

texu_status        _TexuEditProc_ValidateNumberStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch);
texu_status        _TexuEditProc_ValidateDecimalStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch);
texu_status        _TexuEditProc_ValidateA2ZStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch);
texu_i32           _TexuEditProc_RemoveDecimalFormat(texu_editwnd* edit);
texu_i32           _TexuEditProc_AddDecimalFormat(texu_editwnd* edit);

void               _TexuWndProc_Notify(texu_wnd*, texu_ui32);


void
_TexuWndProc_Notify(texu_wnd* wnd, texu_ui32 code)
{
  texu_wnd_notify notify;
  texu_wnd* parent = texu_wnd_get_parent(wnd);
  
  notify.wnd  = wnd;
  notify.id   = texu_wnd_get_id(wnd);
  notify.code = code;
  texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
}

void _TexuEditProc_OnLimitText(texu_wnd* wnd, texu_i32 limit)
{
  texu_editwnd* edit = 0;
  texu_char text[TEXU_MAX_WNDTEXT + 1];
  
  if (limit > 0 || limit <= TEXU_MAX_WNDTEXT)
  {
    edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
    edit->limitchars = limit;
    
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    if (strlen(text) > limit)
    {
      text[limit] = 0;
      texu_wnd_set_text(wnd, text);
      strcpy(edit->editbuf, text);
    }
  }
}

void _TexuEditProc_OnSetPasswdChar(texu_wnd* wnd, texu_char passchar)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  edit->passchar = passchar;
}

void _TexuEditProc_OnShowPasswdChar(texu_wnd* wnd, texu_i32 show)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  edit->showpass = (show == TEXU_TRUE ? TEXU_TRUE : TEXU_FALSE);
}

void _TexuEditProc_OnSetDecWidth(texu_wnd* wnd, texu_i32 width)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  edit->decwidth = width;
}

void _TexuEditProc_OnSetText(texu_wnd* wnd, const texu_char* text)
{
  texu_editwnd* edit = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  /*texu_char buf[TEXU_MAX_WNDTEXT+1];*/
  
  if (!text)
  {
    return;
  }
  TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)text, 0);
  edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  /*texu_wnd_get_text(wnd, edit->editbuf, TEXU_MAX_WNDTEXT);*/
  strcpy(edit->editbuf, text);
  edit->firstvisit = 1;
  
  if (TEXU_ES_AUTODECIMALCOMMA & style)
  {
    _TexuEditProc_RemoveDecimalFormat(edit);
    _TexuEditProc_AddDecimalFormat(edit);
  }
}

void _TexuEditProc_OnSetValidString(texu_wnd* wnd, const texu_char* validstr)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  if (validstr)
  {
    /* set the valid string */
    strcpy(edit->validstr, validstr);
  }
  else
  {
    /* remove valid string */
    memset(edit->validstr, 0, sizeof(edit->validstr));
  }
}

void _TexuEditProc_OnSetValidMinMax(texu_wnd* wnd, texu_i32 on, texu_editminmax* vmm)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  edit->min = vmm->min;
  edit->max = vmm->max;
  edit->onminmax = (on == TEXU_TRUE ? TEXU_TRUE : TEXU_FALSE);
}


texu_i32 _TexuEditProc_AddDecimalFormat(texu_editwnd* edit)
{
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_i32 cnt = 1;
  texu_char* pbuf;
  texu_i32 len = strlen(edit->editbuf) - 1;
  texu_char* psz = &edit->editbuf[len];
  
  memset(buf, 0, sizeof(buf));
  pbuf = buf;
  
  while (*psz != 0 && *psz != '.')
  {
    *pbuf = *psz;
    ++pbuf;
    --psz;
    --len;
  }
  if ('.' == *psz)
  {
    *pbuf = *psz;
    ++pbuf;
    --psz;
    --len;
  }
  
  while (len >= 0)
  {
    if (cnt % 4 == 0 && *psz != '-')
    {
      *pbuf = ',';
      ++pbuf;
      ++cnt;
    }
    *pbuf = *psz;
    ++pbuf;
    --psz;
    --len;
    ++cnt;
  }
  /* save */
  memset(edit->editbuf, 0, sizeof(edit->editbuf));
  /* reverse copy */
  len  = strlen(buf) - 1;
  pbuf = &buf[len];
  psz  = edit->editbuf;
  while (len >= 0)
  {
    *psz = *pbuf;
    ++psz;
    --pbuf;
    --len;
  }
  return TEXU_OK;
}

texu_i32 _TexuEditProc_RemoveDecimalFormat(texu_editwnd* edit)
{
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_char* psz = edit->editbuf;
  texu_char* pbuf = buf;
  
  if (strchr(edit->editbuf, ','))
  {
    memset(buf, 0, sizeof(buf));
    while (*psz != 0 && *psz != '.')
    {
      if (*psz != ',')
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
    /* save */
    strcpy(edit->editbuf, buf);
  }
  return TEXU_OK;
}


texu_i32
_TexuEditProc_OnKillFocus(texu_wnd* wnd, texu_wnd* nextwnd)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_f64  decimal = 0.0;
  texu_i32  number = 0;
  texu_i32  rcminmax = TEXU_OK;
  
    /* check if style is TEXU_ES_DECIMAL */
  if (TEXU_ES_DECIMAL & style || TEXU_ES_AUTODECIMALCOMMA & style)
  {
    decimal = atof(edit->editbuf);
    sprintf((texu_char*)buf, "%.*f", (texu_i32)edit->decwidth, decimal);
    strcpy(edit->editbuf, buf);
    if (TEXU_TRUE == edit->onminmax)
    {
      if (decimal < (texu_f64)edit->min &&
          decimal > (texu_f64)edit->max)
      {
        rcminmax = TEXU_ERROR;
      }
    }
  }
  else
  {
    strcpy(buf, edit->editbuf);
  }
  
  if ((TEXU_ES_NUMBER & style) && (TEXU_TRUE == edit->onminmax))
  {
    number = atol(edit->editbuf);
    if (number < edit->min &&
        number > edit->max)
    {
      rcminmax = TEXU_ERROR;
    }
  }
  /* validate min & max */
  if (rcminmax != TEXU_CONTINUE)
  {
    return rcminmax;
  }  
  if (TEXU_ES_AUTODECIMALCOMMA & style)
  {
    _TexuEditProc_AddDecimalFormat(edit);
  }
  /* update text */
  edit->firstvisit = 1;
  edit->firstchar  = 0;
  edit->editing    = 0;
  edit->selected   = 0;
  texu_wnd_set_text(wnd, edit->editbuf);
  texu_wnd_invalidate(wnd);

  _TexuWndProc_Notify(wnd, TEXU_EN_KILLFOCUS);
  return TEXU_OK;
}

void
_TexuEditProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  texu_editwnd* edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  texu_ui32 style = texu_wnd_get_style(wnd);
 
  texu_wnd_get_text(wnd, edit->editbuf, TEXU_MAX_WNDTEXT);

  if (TEXU_ES_AUTODECIMALCOMMA & style)
  {
    _TexuEditProc_RemoveDecimalFormat(edit);
  }
  
  edit->firstvisit = 1;
  edit->firstchar  = 0;
  edit->editing    = 0;
  edit->selected   = 1;
  
  _TexuWndProc_Notify(wnd, TEXU_EN_SETFOCUS);
  texu_wnd_invalidate(wnd);
}



void
_TexuEditProc_OnDestroy(texu_wnd* wnd)
{
  texu_editwnd* edit = texu_wnd_get_userdata(wnd);
  if (edit)
  {
    free(edit);
    texu_wnd_set_userdata(wnd, 0);
  }
}

texu_status _TexuEditProc_ValidateA2ZStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch)
{
  texu_status rc = TEXU_OK;
  if (ch >= 'a' && ch <= 'z')
  {
    rc = TEXU_OK;
  }
  else if (ch >= 'A' && ch <= 'Z')
  {
    rc = TEXU_OK;
  }
  return rc;
}

texu_status _TexuEditProc_ValidateNumberStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch)
{
  texu_status rc = TEXU_OK;
  if (ch < '0' || ch > '9')
  {
    rc = TEXU_ERROR;
  }
  return rc;
}


texu_status _TexuEditProc_ValidateDecimalStyle(texu_wnd* wnd, texu_editwnd* edit, texu_i32 ch)
{
  texu_status rc = TEXU_OK;
  texu_char* decimal = strchr(edit->editbuf, '.');
  texu_i64 len = strlen(edit->editbuf);

  /* not allowed '-' in the string */
  if ((len > 0 && (edit->firstvisit == 0)) && ch == '-')
  {
    rc = TEXU_ERROR;
  }
  if ((rc == TEXU_OK) && (len == 0 || (edit->firstvisit == 1)) && ch == '-')
  {
    /* ok */
  }
  else if (ch == '.')
  {
    if (decimal)
    {
      /* not allowed having '.' more than one */
      rc = TEXU_ERROR;
    }
  }
  else if (ch < '0' || ch > '9')
  {
    rc = TEXU_ERROR;
  }
  return rc;
}


void
_TexuEditProc_OnChar(texu_wnd* wnd, texu_i32 ch)
{
  texu_editwnd* edit = 0;
  texu_i32 changed = 0;
  texu_cio* dc = texu_wnd_get_cio(wnd);
  texu_i64 len = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_char text[TEXU_MAX_WNDTEXT+1];
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 normcolor = TEXU_CIO_COLOR_WHITE_BLUE|A_BOLD;
  texu_i32 discolor = TEXU_CIO_COLOR_WHITE_BLACK;
  texu_i64  ret = TEXU_OK;
  texu_char* psz = 0;
  texu_i32  y = texu_wnd_get_y(wnd);
  texu_i32  x = texu_wnd_get_x(wnd);
  texu_i32  width = texu_wnd_get_width(wnd);

  if (!(texu_wnd_is_enable(wnd)))
  {
    return;
  }

  edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
  texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
  texu_wnd_get_color(wnd, &normcolor, &discolor);

  if (TEXU_KEY_ESCAPE == ch)/* == KEY_ESCAPE)*/
  {
    if (edit->firstvisit == 0)
    {
      edit->firstvisit = 1;
      strcpy(edit->editbuf, text);
      edit->firstchar  = 0;
      edit->editing    = 0;
      edit->selected   = 1;
      texu_wnd_invalidate(wnd);
      return;
    }
  }
  if (ch >= 0x20 && ch < 0x7f)
  {
    /* add char */
    len = strlen(edit->editbuf);
    /* is the first typing? */
    if (edit->firstvisit == 1)
    {
      edit->firstvisit = 0;
      if (!(TEXU_ES_APPENDMODE & style) || (TEXU_ES_PASSWORD & style)) /* replace mode */
      {
        edit->editbuf[0] = 0;
        memset(buf, ' ', width);
        buf[width] = 0;
        texu_cio_putstr_attr(dc, y, x, buf, COLOR_PAIR(normcolor));
        texu_cio_gotoyx(dc, y, x);

        len = 0;
      }
      else if (TEXU_ES_AUTOHSCROLL & style)
      {
        /* show first char at */
        if (len > width)
        {
          edit->firstchar = len - width - 1;
        }
      }
    }

    if (len + 1 <= edit->limitchars)
    {
      if (TEXU_ES_NUMBER & style)
      {
        /* require only number input */
        ret = _TexuEditProc_ValidateNumberStyle(wnd, edit, ch);
        if (ret != TEXU_OK)
        {
          return;
        }
      }
      else if (TEXU_ES_DECIMAL & style || TEXU_ES_AUTODECIMALCOMMA & style)
      {
        /* require only decimal input */
        ret = _TexuEditProc_ValidateDecimalStyle(wnd, edit, ch);
        if (ret != TEXU_OK)
        {
          return;
        }
      }
      else if (TEXU_ES_UPPERCASE & style)
      {
        /* require changing from small to CAPITAL */
        if (ch >= 'a' && ch <= 'z')
        {
          ch = ch - 'a' + 'A';
        }
      }
      else if (TEXU_ES_LOWERCASE & style)
      {
        if (ch >= 'A' && ch <= 'Z')
        {
          ch = ch - 'A' + 'a';
        }
      }
      if (TEXU_ES_A2Z & style)
      {
        /* require only A-Z input */
        ret = _TexuEditProc_ValidateA2ZStyle(wnd, edit, ch);
        if (ret != TEXU_OK)
        {
          return;
        }
      }
      /* valid char if it is in valid string */
      if (edit->validstr[0] != 0)
      {
        psz = strchr(edit->validstr, ch);
        if (!psz)
        {
          return;
        }
      }

      /* append a new char */
      len = strlen(edit->editbuf);
      if (len < edit->limitchars)
      {
        edit->editbuf[len]   = ch;
        edit->editbuf[len+1] = 0;
        edit->selected   = 0;
        if (len < width)
        {
          if (TEXU_ES_PASSWORD & style)
          {
            if (edit->showpass == TEXU_TRUE)
            {
              texu_cio_putch_attr(dc, y, x + len,
                edit->passchar,
                COLOR_PAIR(normcolor)|A_REVERSE);
            }
          }
          else
          {
            texu_cio_putch_attr(dc, y, x + len,
              ch,
              COLOR_PAIR(normcolor)|A_REVERSE);
          }
          changed = 1;
          texu_cio_gotoyx(dc, y, x + len);
          texu_wnd_invalidate(wnd);
          return;
        }
        changed = 1;
      } /* len < limit */

      if (TEXU_ES_AUTOHSCROLL & style)
      {
        len = strlen(edit->editbuf);
        if (len <= edit->limitchars && len > width && changed)
        {
          if (len > width)
          {
            edit->firstchar = len - width;
          }
          /* editing */
          edit->editing = 1;
          texu_wnd_invalidate(wnd);
          return;
        }
      }

      /* editing */
      edit->editing    = 1;
    } /*TEXU_MAX_WNDTEXT*/
  }
  else
  if (0x7f == ch || KEY_BACKSPACE == ch) /* delete char */
  {
    edit->selected   = 0;
    len = strlen(edit->editbuf);
    if (edit->firstvisit == 1)
    {
      edit->firstvisit = 0;
      if (!(TEXU_ES_APPENDMODE & style) || TEXU_ES_PASSWORD & style) /* replace mode */
      {
        edit->editbuf[0] = 0;
        strcpy(buf, " ");
        texu_printf_alignment(text, buf, width, style);
        texu_cio_putstr_attr(dc, y, x, text, COLOR_PAIR(normcolor)|A_REVERSE);
        texu_cio_gotoyx(dc, y, x);
      }
      else if (TEXU_ES_AUTOHSCROLL & style)
      {
        /* show first char at */
        if (len > width)
        {
          edit->firstchar = len - width - 1;
        }
      }
    }

    if (len > 0)
    {
      edit->editbuf[len - 1] = 0;
      changed = TEXU_TRUE;

      if (TEXU_ES_AUTOHSCROLL & style)
      {
        len = strlen(edit->editbuf);
        if (len > width)
        {
          edit->firstchar = len - width - 1;
        }
        len = TEXU_MIN(len, width);
        texu_cio_gotoyx(dc, y, x + len);
        texu_wnd_invalidate(wnd);
        return;
      }
    }

    /* editing */
    edit->editing = 1;
  } /*end delete char*/
}

texu_status
_TexuEditProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_ui32 style = attrs->style;
  texu_editwnd* edit = 0;

  /* initial memory for edit control */
  edit = (texu_editwnd*)malloc(sizeof(texu_editwnd));
  if (!edit)
  {
    return TEXU_NOMEM;
  }

  memset(edit, 0, sizeof(texu_editwnd));
  texu_wnd_get_text(wnd, edit->editbuf, TEXU_MAX_WNDTEXT);

  edit->firstvisit = 1;
  edit->firstchar  = 0;
  edit->passchar   = '*';
  edit->showpass   = TEXU_TRUE;
  edit->editing    = 0;
  edit->decwidth   = 6;
  edit->limitchars = (TEXU_ES_AUTOHSCROLL & style ? TEXU_MAX_WNDTEXT : attrs->width);
  edit->selected   = 0;

  texu_wnd_set_color(wnd,
    TEXU_CIO_BRIGHT_WHITE_CYAN, TEXU_CIO_COLOR_WHITE_CYAN);
  texu_wnd_set_userdata(wnd, edit);
  texu_wnd_set_focuscolor(wnd, TEXU_CIO_CYAN_BRIGHT_WHITE);

  return TEXU_OK;
}

void _TexuEditProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_editwnd* edit = 0;
  texu_i64 len = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_char text[TEXU_MAX_WNDTEXT+1];
  texu_i32 normcolor = TEXU_CIO_COLOR_WHITE_BLUE|A_BOLD;
  texu_i32 discolor = TEXU_CIO_COLOR_WHITE_BLACK;
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 y = texu_wnd_get_y(wnd);
  texu_i32 x = texu_wnd_get_x(wnd);
  texu_i32 width = texu_wnd_get_width(wnd);
  texu_i32 color = TEXU_CIO_COLOR_WHITE_BLUE|A_BOLD;
 
  edit = (texu_editwnd*)texu_wnd_get_userdata(wnd);
 
  if (texu_wnd_is_visible(wnd))
  {
    texu_wnd_get_color(wnd, &normcolor, &discolor); 
    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);
   
    len = width;
    if (TEXU_ES_PASSWORD & style)
    {
      if (edit->showpass == TEXU_TRUE)
      {
        memset(buf, edit->passchar, len);
      }
      else
      {
        memset(buf, ' ', len);
      }
      buf[len] = 0;
      texu_cio_putstr_attr(dc, y, x, buf, COLOR_PAIR(color));
      return;
    }
   
    len = TEXU_MIN(strlen(edit->editbuf), width);
    memset(buf, 0, sizeof(buf));
    memset(text, 0, sizeof(buf));
    strncpy(buf, &edit->editbuf[edit->firstchar], width);

    if (edit->editing)
    {
      style = TEXU_ALIGN_LEFT;
    }
    texu_printf_alignment(text, buf, width, style);
    texu_cio_putstr_attr(dc, y, x, text, 
      (edit->selected ? COLOR_PAIR(color)|A_REVERSE : COLOR_PAIR(color)));
    texu_cio_gotoyx(dc, y, x+len);
  }
}

texu_i64
TexuEditProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  
  switch (msg)
  {
    case TEXU_WM_CREATE:
    {
      return _TexuEditProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
    }  

    case TEXU_WM_CHAR:
    {
      _TexuEditProc_OnChar(wnd, (texu_i32)param1);
      return 0;
    }

    case TEXU_WM_DESTROY:
    {
      _TexuEditProc_OnDestroy(wnd);
      break;
    }

    case TEXU_WM_PAINT:
    {
      _TexuEditProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
    }

    case TEXU_WM_SETFOCUS:
    {
      _TexuEditProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;
    }

    case TEXU_WM_KILLFOCUS:
    {
      return _TexuEditProc_OnKillFocus(wnd, (texu_wnd*)param1);
    }
      
    case TEXU_EM_LIMITTEXT:
    {
      _TexuEditProc_OnLimitText(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_EM_SETPASSWDCHAR:
    {
      _TexuEditProc_OnSetPasswdChar(wnd, (texu_char)param1);
      return 0;
    }
    case TEXU_EM_SETDECWIDTH:
    {
      _TexuEditProc_OnSetDecWidth(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_EM_SHOWPASSWDCHAR:
    {
      _TexuEditProc_OnShowPasswdChar(wnd, (texu_i32)param1);
      return 0;
    }
    
    case TEXU_EM_SETVALIDSTRING:
    {
      _TexuEditProc_OnSetValidString(wnd, (const texu_char*)param2);
      return 0;
    }
    
    case TEXU_EM_SETVALIDMINMAX:
    {
      _TexuEditProc_OnSetValidMinMax(wnd, (texu_i32)param1, (texu_editminmax*)param2);
      return 0;
    }

    
    case TEXU_WM_SETTEXT:
      _TexuEditProc_OnSetText(wnd, (const texu_char*)param1);
      return 0;

  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}


/*
#-------------------------------------------------------------------------------
# TexU listbox
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_lbwnd_item
{
  texu_char               itemtext[TEXU_MAX_WNDTEXT+1];
  texu_i64                data;
  texu_i32                checked;
  texu_i32                enable;
  struct texu_lbwnd_item *prev;
  struct texu_lbwnd_item *next;
};
typedef struct texu_lbwnd_item texu_lbwnd_item;

struct texu_lbwnd
{
  texu_i32            firstvisible;
  texu_i32            nitems;        /* item counts */
  texu_i32            cursel;       /* highlight item */

  texu_lbwnd_item*    firstitem;    /* always be item id = 0 if it is existing */
  texu_lbwnd_item*    lastitem;

  texu_lbwnd_item*    selitem;      /* must be the same cursel item */
  texu_lbwnd_item*    firstvisibleitem;

  texu_i32            checkeditems;    /* count item checked */
  texu_lbwnd_item*    lastitemchecked; /* to identify the last item checked */
  
  void*               exparam;
  
  texu_i32            selcolor;
};
typedef struct texu_lbwnd texu_lbwnd;


void               _TexuListBoxProc_OnPaint(texu_wnd*, texu_cio*);
texu_status        _TexuListBoxProc_OnCreate(texu_wnd*, texu_wnd_attrs*);
void               _TexuListBoxProc_OnDestroy(texu_wnd* wnd);
void               _TexuListBoxProc_OnDeleteItem(texu_wnd* wnd, texu_i32 idx);
texu_i32           _TexuListBoxProc_OnKillFocus(texu_wnd* wnd, texu_wnd* nextwnd);
void               _TexuListBoxProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd);
texu_i32           _TexuListBoxProc_OnAddItem(texu_wnd* wnd, const texu_char* text);
void               _TexuListBoxProc_OnSelChanged(texu_wnd* wnd);
texu_i32           _TexuListBoxProc_OnCountItemCheck(texu_wnd* wnd);
texu_i32           _TexuListBoxProc_OnGetItemChecked(texu_wnd* wnd, texu_i32 idx);
texu_i32           _TexuListBoxProc_OnSetItemChecked(texu_wnd* wnd, texu_i32 idx, texu_i32 check);
texu_i64           _TexuListBoxProc_OnGetItemData(texu_wnd* wnd, texu_i32 idx);
void               _TexuListBoxProc_OnSetItemData(texu_wnd* wnd, texu_i32 idx, texu_i64 data);
void               _TexuListBoxProc_OnSetCurSel(texu_wnd* wnd, texu_i32 idx);
texu_i32           _TexuListBoxProc_OnGetItemCount(texu_wnd* wnd);
void               _TexuListBoxProc_OnSetItemText(texu_wnd* wnd, texu_i32 idx, const texu_char* text);
texu_i32           _TexuListBoxProc_OnGetItemText(texu_wnd* wnd, texu_i32 idx, texu_char* text);
texu_i32           _TexuListBoxProc_OnGetCurSel(texu_wnd* wnd);
void               _TexuListBoxProc_OnDeleteAllItems(texu_wnd* wnd);
void               _TexuListBoxProc_OnChar(texu_wnd*, texu_i32 ch);
void               _TexuListBoxProc_OnEnableItem(texu_wnd* wnd, texu_i32 idx, texu_i32 check);


texu_i32           _TexuListBoxProc_FindIndexByItem(texu_lbwnd* lb, texu_lbwnd_item* item);
texu_lbwnd_item*   _TexuListBoxProc_FindItemByIndex(texu_lbwnd* lb, texu_i32 idx);
void               _TexuListProc_Notify(texu_wnd*, texu_ui32, texu_i32);
texu_lbwnd_item*   _TexuListBoxProc_GetFirstItemEnabled(texu_lbwnd* lb);
texu_lbwnd_item*   _TexuListBoxProc_GetNextItemEnabled(texu_lbwnd* lb, texu_lbwnd_item* item);
texu_lbwnd_item*   _TexuListBoxProc_GetLastItemEnabled(texu_lbwnd* lb);
texu_lbwnd_item*   _TexuListBoxProc_GetPrevItemEnabled(texu_lbwnd* lb, texu_lbwnd_item* item);


texu_i32
_TexuListBoxProc_FindIndexByItem(texu_lbwnd* lb, texu_lbwnd_item* item)
{
  texu_i32 idx = 0;
  texu_lbwnd_item* chkitem = lb->firstitem;
  while (chkitem)
  {
    if (chkitem == item)
    {
      return idx;
    }
    ++idx;
    chkitem = chkitem->next;
  }
  return -1;
}

texu_lbwnd_item*
_TexuListBoxProc_GetFirstItemEnabled(texu_lbwnd* lb)
{
  texu_lbwnd_item* item = lb->firstitem;
  if (item && item->enable)
  {
    return item;
  }
  return _TexuListBoxProc_GetNextItemEnabled(lb, item);
}

texu_lbwnd_item*
_TexuListBoxProc_GetNextItemEnabled(texu_lbwnd* lb, texu_lbwnd_item* item)
{
  if (item)
  {
    item = item->next;
    while (item)
    {
      if (item->enable)
      {
        return item;
      }
      item = item->next;
    }
  }
  return 0;
}

texu_lbwnd_item*
_TexuListBoxProc_GetLastItemEnabled(texu_lbwnd* lb)
{
  texu_lbwnd_item* item = lb->lastitem;
  if (item && item->enable)
  {
    return item;
  }
  return _TexuListBoxProc_GetPrevItemEnabled(lb, item);
}

texu_lbwnd_item*
_TexuListBoxProc_GetPrevItemEnabled(texu_lbwnd* lb, texu_lbwnd_item* item)
{
  if (item)
  {
    item = item->prev;
    while (item)
    {
      if (item->enable)
      {
        return item;
      }
      item = item->prev;
    }
  }
  return 0;
}

void
_TexuListBoxProc_OnChar(texu_wnd* wnd, texu_i32 ch)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_i32 repaint = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 lines = 0;
  texu_i32 height = texu_wnd_get_height(wnd);
  texu_lbwnd_item* item = 0;
  texu_i32 cursel = 0;
  texu_i32 prevsel = 0;
  
  if (lb->nitems <= 0)
  {
    return;
  }
  item = _TexuListBoxProc_GetFirstItemEnabled(lb);
  if (!item)
  {
    return;
  }
  
  switch (ch)
  {
    case 0x20:
    {
      if ((style & TEXU_LBS_CHECKBOX) ||
          (style & TEXU_LBS_RADIOBOX))
      {
        /* get current and check it */
        _TexuListBoxProc_OnSetItemChecked(wnd, lb->cursel, TEXU_LB_CHECKED);
      }
      ++repaint;
      break;
    }

    case KEY_DOWN:
    {
      ++lines;
      ++repaint;
      break;
    }

    case KEY_UP:
    {
      --lines;
      ++repaint;
      break;
    }

    case KEY_PPAGE:
    {
      lines -= height;
      ++repaint;
      break;
    }
    
    case KEY_NPAGE:
    {
      lines += height;
      ++repaint;
      break;
    }
    default:
    {
       break;
    }
  }
  if (repaint)
  {
    prevsel = lb->cursel;
    lb->cursel += lines;
    if (lb->cursel >= lb->nitems)
    {
      lb->cursel = lb->nitems - 1;
    }
    else if (lb->cursel < 0)
    {
      lb->cursel = 0;
    }
    cursel = lb->cursel;
    
    item = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
    if (item && !(item->enable))
    {
      if (lines < 0)
      {
        item = _TexuListBoxProc_GetPrevItemEnabled(lb, item);
        if (!item)
        {
          return;
        }
        else
        {
          lb->cursel = _TexuListBoxProc_FindIndexByItem(lb, item);
        }
      }
      else
      {
        item = _TexuListBoxProc_GetNextItemEnabled(lb, item);
        if (!item)
        {
          return;
        }
        else
        {
          lb->cursel = _TexuListBoxProc_FindIndexByItem(lb, item);
        }
      }
    }
    /*lines = (cursel - prevsel);*/
    
    /* find the new first visible */
    if (lb->cursel >= lb->firstvisible + height)
    {
      lb->firstvisible += lines + (cursel - prevsel);
      if (lb->firstvisible > lb->nitems - height)
      {
        lb->firstvisible = lb->nitems - height;
      }
    }
    if (lb->firstvisible > lb->cursel)
    {
      lb->firstvisible += lines + (cursel - prevsel);
      if (lb->firstvisible < 0)
      {
        lb->firstvisible = 0;
      }
    }
    
    /* send notification */
    _TexuListBoxProc_OnSelChanged(wnd);
    /* redraw after changing */
    texu_wnd_invalidate(wnd);
  }
  lb->selitem = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
  lb->firstvisibleitem = _TexuListBoxProc_FindItemByIndex(lb, lb->firstvisible);
}

void _TexuListBoxProc_OnDeleteAllItems(texu_wnd* wnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_i32 nitems = 0;
  texu_i32 i = 0;
  
  nitems = lb->nitems;
  for (i = 0; i < nitems; ++i)
  {
    _TexuListBoxProc_OnDeleteItem(wnd, 0);
  }
}

texu_i32 _TexuListBoxProc_OnGetCurSel(texu_wnd* wnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  return lb->cursel;
}

texu_i32 _TexuListBoxProc_OnGetItemText(texu_wnd* wnd, texu_i32 idx, texu_char* text)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);
  
  if (item)
  {
    strcpy(text, item->itemtext);
  }
  return strlen(text);
}

void _TexuListBoxProc_OnSetItemText(texu_wnd* wnd, texu_i32 idx, const texu_char* text)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);  
  if (item)
  {
    strcpy(item->itemtext, text);
  }
}

texu_i32 _TexuListBoxProc_OnGetItemCount(texu_wnd* wnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  return lb->nitems;
}

void _TexuListBoxProc_OnSetCurSel(texu_wnd* wnd, texu_i32 idx)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_i32 height = texu_wnd_get_height(wnd);
  
  if (idx < 0 || idx >= lb->nitems)
  {
    idx = -1;
  }
  lb->cursel = idx;
  
  if (lb->cursel >= 0)
  {
    lb->firstvisible = lb->cursel;
    if (lb->firstvisible + height > lb->nitems)
    {
      lb->firstvisible = lb->nitems - height;
    }
  }
  else
  {
    lb->firstvisible = 0;
  }
  
  /* send notification */
  _TexuListBoxProc_OnSelChanged(wnd);
  /* redraw after changing */
  texu_wnd_invalidate(wnd);

  lb->selitem = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
  lb->firstvisibleitem = _TexuListBoxProc_FindItemByIndex(lb, lb->firstvisible);
}

void _TexuListBoxProc_OnSetItemData(texu_wnd* wnd, texu_i32 idx, texu_i64 data)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);  
  if (item)
  {
    item->data = data;
  }
}

texu_i64 _TexuListBoxProc_OnGetItemData(texu_wnd* wnd, texu_i32 idx)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);
  if (item)
  {
    return item->data;
  }
  return 0;
}

void
_TexuListBoxProc_OnEnableItem(texu_wnd* wnd, texu_i32 idx, texu_i32 enable)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);
  {
    if (item->enable != enable)
    {
      texu_wnd_invalidate(wnd);
    }
    item->enable = enable;
  }
}

texu_i32 _TexuListBoxProc_OnSetItemChecked(texu_wnd* wnd, texu_i32 idx, texu_i32 check)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_ui32 style = texu_wnd_get_style(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);
  /* found and do the check item state */
  if (item)
  {
    if (check == TEXU_LB_UNCHECKED)
    {
      --lb->checkeditems;
      if (lb->checkeditems < 0)
      {
        lb->checkeditems    = 0;
        lb->lastitemchecked = 0;
      }
      item->checked = TEXU_LB_UNCHECKED;
    }
    else
    {
      /* unselected the last if it is radio style */
      if (style & TEXU_LBS_RADIOBOX)
      {
        if (lb->lastitemchecked)
        {
          lb->lastitemchecked->checked = TEXU_LB_UNCHECKED;
        }
      }
      else if (style & TEXU_LBS_CHECKBOX)
      {
        if (item->checked == TEXU_LB_CHECKED)
        {
          --lb->checkeditems;
          if (lb->checkeditems < 0)
          {
            lb->checkeditems    = 0;
            lb->lastitemchecked = 0;
          }
          item->checked = TEXU_LB_UNCHECKED;
          return TEXU_LB_OK;
        }
      }
      /* count checked item */
      ++lb->checkeditems;
      if (lb->checkeditems > lb->nitems)
      {
        lb->checkeditems = lb->nitems;
      }
      /* checked and save the last checked item */
      item->checked = TEXU_LB_CHECKED;
      lb->lastitemchecked = item;
    }
    texu_wnd_invalidate(wnd);
    return TEXU_LB_OK;
  }
  return TEXU_LB_ERROR;
}

texu_i32 _TexuListBoxProc_OnGetItemChecked(texu_wnd* wnd, texu_i32 idx)
{
  texu_lbwnd_item* item = 0;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);  
  if (item)
  {
    return item->checked;
  }
  return TEXU_LB_ERROR;
}

texu_i32 _TexuListBoxProc_OnCountItemCheck(texu_wnd* wnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  return lb->checkeditems;
}

void
_TexuListProc_Notify(texu_wnd* wnd, texu_ui32 code, texu_i32 index)
{
  texu_lbwnd_notify notify;
  texu_wnd* parent = texu_wnd_get_parent(wnd);
  
  notify.hdr.wnd  = wnd;
  notify.hdr.id   = texu_wnd_get_id(wnd);
  notify.hdr.code = code;
  notify.index    = index;
  texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
}



texu_lbwnd_item*
_TexuListBoxProc_FindItemByIndex(texu_lbwnd* lb, texu_i32 idx)
{
  texu_i32 i = 0;
  texu_lbwnd_item* item = 0;
  
  if (idx < 0 || idx >= lb->nitems)
  {
    return 0;
  }
  item = lb->firstitem;
  
  for (i = 0; i < lb->nitems && item; ++i, item = item->next)
  {
    if (i == idx)
    {
      return item;
    }
  }
  return 0;
}


void
_TexuListBoxProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_lbwnd_item* item = 0;
  if (lb->cursel >= lb->nitems)
  {
    lb->cursel = lb->nitems - 1;
    /*texu_wnd_invalidate(wnd);*/
  }
  else if (lb->cursel < 0)
  {
    lb->cursel = 0;
    /*texu_wnd_invalidate(wnd);*/
  }
  item = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
  if (item && !(item->enable))
  {
    item = _TexuListBoxProc_GetNextItemEnabled(lb, item);
    if (item)
    {
      lb->cursel = _TexuListBoxProc_FindIndexByItem(lb, item);
    }
  }
  texu_wnd_invalidate(wnd);
  
  /* send notification */
  _TexuListProc_Notify(wnd, TEXU_LBN_SETFOCUS, lb->cursel);
}

texu_i32
_TexuListBoxProc_OnKillFocus(texu_wnd* wnd, texu_wnd* nextwnd)
{
  texu_i32 rc = TEXU_OK;
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  
  /* send notification */
  _TexuListProc_Notify(wnd, TEXU_LBN_KILLFOCUS, lb->cursel);
  return rc;
}

void _TexuListBoxProc_OnSelChanged(texu_wnd* wnd)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  _TexuListProc_Notify(wnd, TEXU_LBN_SELCHANGED, lb->cursel);
}


void
_TexuListBoxProc_OnPaint(texu_wnd* wnd, texu_cio* cio)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  if (texu_wnd_is_visible(wnd))
  {
    texu_char buf[TEXU_MAX_WNDTEXT+1];
    texu_char text[TEXU_MAX_WNDTEXT+1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_i32 height = texu_wnd_get_height(wnd);
    texu_i32 normcolor = TEXU_CIO_COLOR_CYAN_BLACK;
    texu_i32 discolor = TEXU_CIO_COLOR_WHITE_BLACK;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_bool enable = texu_wnd_is_enable(wnd);
    texu_i32 color = TEXU_CIO_COLOR_CYAN_BLACK;
    texu_i32 i = 0;
    texu_i32 lines = 0;
    texu_lbwnd_item* item = 0;

    color = (enable ? normcolor : discolor);
     /* draw */
    if (lb->nitems > 0)
    {
      item = lb->firstitem;
      lines = height;

      for (i = 0; i < lb->nitems && item; ++i, item = item->next)
      {
        if (i < lb->firstvisible)
        {
          /* do nothing */
          continue;
        }
        else if (i - lb->firstvisible < lines)
        {
          memset(buf, 0, sizeof(buf));
          if (style & TEXU_LBS_CHECKBOX)
          {
            if (item->checked)
            {
              strcpy(buf, "[X] ");
            }
            else
            {
              strcpy(buf, "[ ] ");
            }
          }
          else if (style & TEXU_LBS_RADIOBOX)
          {
            if (item->checked)
            {
              strcpy(buf, "(*) ");
            }
            else
            {
              strcpy(buf, "( ) ");
            }
          }          
          
          /* copy from item text */
          strcat(buf, item->itemtext);
          
          memset(text, 0, sizeof(text));
          texu_printf_alignment(text, buf, width, style);
          
          if (i == lb->cursel && item->enable)
          {
            color = lb->selcolor;
          }
          else
          {
            color = (item->enable ? normcolor : discolor);
          }
          texu_cio_putstr_attr(cio, y+(i-lb->firstvisible), x, text, COLOR_PAIR(color));
        }/* not owner draw */
      } /* for each item */
    } /* items are valid */
  }
}

texu_status
_TexuListBoxProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_lbwnd* lb = 0;

  /* initial memory for edit control */
  lb = (texu_lbwnd*)malloc(sizeof(texu_lbwnd));
  if (!lb)
  {
    return TEXU_NOMEM;
  }

  memset(lb, 0, sizeof(texu_lbwnd));

  lb->firstvisible      = -1;
  lb->cursel            = -1;
  lb->nitems            = 0;
  lb->firstitem         = 0;
  lb->lastitem          = 0;
  lb->selitem           = 0;
  lb->firstvisibleitem  = 0;
  lb->checkeditems      = 0;
  lb->lastitemchecked   = 0;
  lb->selcolor          = TEXU_CIO_COLOR_WHITE_BLUE;
  
  texu_wnd_set_color(wnd,
    TEXU_CIO_COLOR_CYAN_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
  texu_wnd_set_userdata(wnd, lb);
  
  return TEXU_OK;
}


void
_TexuListBoxProc_OnDestroy(texu_wnd* wnd)
{
  texu_lbwnd* lb = (texu_lbwnd*)texu_wnd_get_userdata(wnd);
  
  _TexuListBoxProc_OnDeleteAllItems(wnd);
  if (lb)
  {
    texu_wnd_set_userdata(wnd, 0);
    free(lb);
  }
}


texu_i32 _TexuListBoxProc_OnAddItem(texu_wnd* wnd, const texu_char* text)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_i32 len = 0;
  texu_lbwnd_item* item = 0;
  
  item = (texu_lbwnd_item*)malloc(sizeof(texu_lbwnd_item));
  if (item)
  {
    memset(item, 0, sizeof(texu_lbwnd_item));
    item->enable = TEXU_TRUE;
    
    len = strlen(text);
    if (len > TEXU_MAX_WNDTEXT)
    {
      len = TEXU_MAX_WNDTEXT;
    }
    strncpy(item->itemtext, text, len);
  
    if (lb->firstitem)
    {
      item->prev = lb->lastitem;
      lb->lastitem->next = item;
      lb->lastitem = item;
    }
    else
    {
      lb->firstitem = lb->lastitem = item;
      lb->firstvisible = 0;
    }
    ++lb->nitems;
    if (!(texu_wnd_is_update_locked(wnd)))
    {
      texu_wnd_invalidate(wnd);
    }
    return lb->nitems;
  }
  return TEXU_NOMEM;
}

void _TexuListBoxProc_OnDeleteItem(texu_wnd* wnd, texu_i32 idx)
{
  texu_lbwnd* lb = texu_wnd_get_userdata(wnd);
  texu_lbwnd_item* item = 0;
  texu_lbwnd_item* nextitem = 0;
  texu_lbwnd_item* previtem = 0;
  texu_i32 height = texu_wnd_get_height(wnd);
  
  item = _TexuListBoxProc_FindItemByIndex(lb, idx);
  
  if (item)
  {
    /* notify to parent */
    _TexuListProc_Notify(wnd, TEXU_LBN_DELETEITEM, idx);
    
    /* remove the links */
    nextitem = item->next;
    previtem = item->prev;
    if (nextitem)
    {
      nextitem->prev = previtem;
    }
    if (previtem)
    {
      previtem->next = nextitem;
    }
    /* check if removed item is the first item or the last item */
    if (item == lb->firstitem)
    {
      lb->firstitem = nextitem;
    }
    else if (item == lb->lastitem)
    {
      lb->lastitem = previtem;
    }
    
    /* free the memory */
    item->next = item->prev = 0;
    
    free(item);
    
    /* decrement items */
    --lb->nitems;
    if (lb->cursel >= lb->nitems)
    {
      lb->cursel = lb->nitems - 1;
    }
    
    /* repaint if idx is visible */
    if (idx >= lb->firstvisible && idx <= lb->firstvisible+height)
    {
      if (!(texu_wnd_is_update_locked(wnd)))
      {
        texu_wnd_invalidate(wnd);
      }
    }
    
  }
  if (lb->nitems <= 0)
  {
    lb->nitems = 0;
    lb->firstitem = lb->lastitem = 0;
    lb->firstvisible = lb->cursel = -1;
  }
}

texu_i64
TexuListBoxProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CHAR:
      _TexuListBoxProc_OnChar(wnd, (texu_i32)param1);  
      return 0;
      
    case TEXU_WM_CREATE:
      return _TexuListBoxProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_PAINT:
      _TexuListBoxProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
      
    case TEXU_WM_DESTROY:
      _TexuListBoxProc_OnDestroy(wnd);
      break;
      
    case TEXU_WM_SETFOCUS:
      _TexuListBoxProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;

    case TEXU_WM_KILLFOCUS:
      return _TexuListBoxProc_OnKillFocus(wnd, (texu_wnd*)param1);
      
    case TEXU_LBM_DELETEITEM:
    {
      _TexuListBoxProc_OnDeleteItem(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_LBM_ADDITEM:
    {
      return _TexuListBoxProc_OnAddItem(wnd, (const texu_char*)param1);
    }
    case TEXU_LBM_GETCURSEL:
    {
      return _TexuListBoxProc_OnGetCurSel(wnd);
    }
    case TEXU_LBM_SETCURSEL:
    {
      _TexuListBoxProc_OnSetCurSel(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_LBM_DELETEALLITEMS:
    {
      _TexuListBoxProc_OnDeleteAllItems(wnd);
      return 0;
    }
    case TEXU_LBM_GETITEMCOUNT:
    {
      return _TexuListBoxProc_OnGetItemCount(wnd);
    }
    case TEXU_LBM_SETITEMDATA:
    {
      _TexuListBoxProc_OnSetItemData(wnd, (texu_i32)param1, (texu_i64)param2);
      return 0;
    }
    case TEXU_LBM_GETITEMDATA:
    {
      return (texu_i32)_TexuListBoxProc_OnGetItemData(wnd, (texu_i32)param1);
    }
    case TEXU_LBM_SETITEMTEXT:
    {
      _TexuListBoxProc_OnSetItemText(wnd, (texu_i32)param1, (const texu_char*)param2);
      return 0;
    }
    case TEXU_LBM_GETITEMTEXT:
    {
      return _TexuListBoxProc_OnGetItemText(wnd, (texu_i32)param1, (texu_char*)param2);
    }
    case TEXU_LBM_COUNTITEMCHECKED:
    {
      return _TexuListBoxProc_OnCountItemCheck(wnd);
    }
    case TEXU_LBM_GETITEMCHECKED:
    {
      return _TexuListBoxProc_OnGetItemChecked(wnd, (texu_i32)param1);
    }
    case TEXU_LBM_SETITEMCHECKED:
    {
      return _TexuListBoxProc_OnSetItemChecked(wnd, (texu_i32)param1, (texu_i32)param2);
    }
    case TEXU_LBM_ENABLEITEM:
    {
      _TexuListBoxProc_OnEnableItem(wnd, (texu_i32)param1, (texu_i32)param2);
      return 0;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



#ifdef __cplusplus
}
#endif


