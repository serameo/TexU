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
      return _TexuEditProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_CHAR:
      _TexuEditProc_OnChar(wnd, (texu_i32)param1);
      return 0;

    case TEXU_WM_DESTROY:
      _TexuEditProc_OnDestroy(wnd);
      break;

    case TEXU_WM_PAINT:
      _TexuEditProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;

    case TEXU_WM_SETFOCUS:
      _TexuEditProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;

    case TEXU_WM_KILLFOCUS:
      return _TexuEditProc_OnKillFocus(wnd, (texu_wnd*)param1);
      
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



/*
#-------------------------------------------------------------------------------
# TexU listctrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_lcwnd_cell
{
  texu_i32        y;
  texu_i32        x;
  texu_i32        lines;
  texu_i32        cols;
  texu_char       caption[TEXU_MAX_WNDTEXT+1];
  texu_i32        normcolor;    /* text attributes          */
  texu_i32        discolor;    /* text attributes          */
  texu_i32        selcolor;    /* text attributes          */
  void*           data;
  texu_ui32       editstyle;
  texu_bool       enable;
  
  struct texu_lcwnd_cell *prev;
  struct texu_lcwnd_cell *next;
};
typedef struct texu_lcwnd_cell texu_lcwnd_cell;

struct texu_lcwnd_header
{
  texu_i32           id;
  texu_char          caption[TEXU_MAX_WNDTEXT+1];
  texu_i32           cols;      /* width */  
  texu_i32           align;     /* left is a default */
  texu_i32           normcolor;    /* text attributes          */
  texu_i32           discolor;    /* text attributes          */
  texu_i32           selcolor;    /* text attributes          */
  texu_ui32          editstyle;
  texu_i32           decwidth;
  texu_bool          enable;
  
  texu_lcwnd_cell*   firstcell;
  texu_lcwnd_cell*   lastcell;

  struct texu_lcwnd_header *prev;
  struct texu_lcwnd_header *next;
};
typedef struct texu_lcwnd_header texu_lcwnd_header;


struct texu_lcwnd
{
  texu_lcwnd_header*      firsthdr;
  texu_lcwnd_header*      lasthdr;
  texu_lcwnd_header*      firstvisiblehdr;
  texu_lcwnd_header*      lastvisiblehdr;

  texu_i32            nheaders;
  texu_i32            nitems;
  texu_i32            hdrids;      /* header id */
  texu_i32            hdrallwidths;
  /* item control */
  texu_i32            curselrow;
  texu_i32            curselcol;
  texu_i32            firstvisiblerow;

  texu_wnd*           editbox;
  texu_i32            editingstate;
  texu_i32            movingstate;
  texu_i32            insertingstate;
  texu_lcwnd_cell*    editingcell;

  void*               exparam;
  
  texu_i32            editkey;
  texu_i32            insertkey;
  texu_i32            delkey;
  texu_i32            canceleditkey;
  
  texu_i32*           editcols;
  texu_i32            nedtcols;
  texu_i32            curedtcol;
  
  texu_lcwnd_cell*    firsteditcell;
  texu_lcwnd_cell*    lasteditcell;
};
typedef struct texu_lcwnd texu_lcwnd;


texu_lcwnd_header*    _TexuListCtrlProc_FindHeaderByIndex(texu_lcwnd* lctl, texu_i32 col);
texu_lcwnd_cell*      _TexuListCtrlProc_FindCellByIndex(texu_lcwnd* lctl, texu_i32 col, texu_i32 idx);
texu_lcwnd_cell*      _TexuListCtrlProc_FindCellByHeader(texu_lcwnd* lctl, texu_lcwnd_header* header, texu_i32 idx);
texu_i32              _TexuListCtrlProc_GetCellRect(texu_lcwnd_cell* cell, texu_rect* rect);
texu_i32              _TexuListCtrlProc_FindHeaderIndex(texu_lcwnd* lctl, texu_lcwnd_header* header);
texu_i32              _TexuListCtrlProc_GetFirstEditableCol(texu_lcwnd* lctl);
texu_i32              _TexuListCtrlProc_GetNextEditableCol(texu_lcwnd* lctl, texu_i32 prevcol);
texu_i32              _TexuListCtrlProc_GetPrevEditableCol(texu_lcwnd* lctl, texu_i32 prevcol);
texu_i32              _TexuListCtrlProc_GetLastEditableCol(texu_lcwnd* lctl);
texu_i32              _TexuListCtrlProc_InsertEmptyItem(texu_wnd* wnd);

void                  _TexuListCtrlProc_NotifyItem(texu_wnd* wnd, texu_ui32 code, texu_i32 row, texu_i32 col);
void                  _TexuListCtrlProc_DrawItem(
                        texu_cio* dc, texu_rect* rccell, const texu_char* caption,
                        texu_ui32 attrs, texu_i32 align, texu_i32 isheader);

void                  _TexuListCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch);
void                  _TexuListCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc);
texu_i32              _TexuListCtrlProc_OnAddItem(texu_wnd* wnd, texu_char* text, texu_i32 nitems);
void                  _TexuListCtrlProc_OnDeleteAllItems(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnDeleteColumn(texu_wnd* wnd, texu_i32 col);
texu_i32              _TexuListCtrlProc_OnAddColumn(texu_wnd* wnd, texu_wnd_header* hdritem);
void                  _TexuListCtrlProc_OnDestroy(texu_wnd* wnd);
texu_status           _TexuListCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs);
texu_i32              _TexuListCtrlProc_OnGetItem(texu_wnd* wnd, texu_ui32 flags, texu_wnd_subitem* subitem);
texu_i32              _TexuListCtrlProc_OnSetItem(texu_wnd* wnd, texu_ui32 flags, texu_wnd_subitem* subitem);
void                  _TexuListCtrlProc_OnSetFocus(texu_wnd*, texu_wnd*);
texu_i32              _TexuListCtrlProc_OnKillFocus(texu_wnd*, texu_wnd*);
void                  _TexuListCtrlProc_OnInvalidateItem(texu_wnd* wnd, texu_ui32 col, texu_ui32 idx);
void                  _TexuListCtrlProc_OnSelChanged(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnBeginMoving(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnMovingCursor(texu_wnd* wnd, texu_i32 ch);
void                  _TexuListCtrlProc_OnEndMoving(texu_wnd* wnd);
texu_ui32             _TexuListCtrlProc_OnGetEditStyle(texu_wnd* wnd, texu_i32 col);
void                  _TexuListCtrlProc_OnSetEditStyle(texu_wnd* wnd, texu_i32 col, texu_ui32 editstyle);
texu_i32              _TexuListCtrlProc_OnGetCurRow(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnSetCurRow(texu_wnd* wnd, texu_i32 idx);
void                  _TexuListCtrlProc_OnSetColWidth(texu_wnd* wnd, texu_i32 col, texu_i32 width);
texu_wnd*             _TexuListCtrlProc_OnGetEditBox(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnSetCurPage(texu_wnd* wnd, texu_i32 npage);
texu_i32              _TexuListCtrlProc_OnGetCurPage(texu_wnd* wnd);
texu_i32              _TexuListCtrlProc_OnGetItemsPerPage(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnSetEditableCols(texu_wnd* wnd, texu_i32 ncols, texu_i32* edtcols);
void                  _TexuListCtrlProc_OnEndEditRow(texu_wnd* wnd, texu_i32 row, texu_i32 ok);
void                  _TexuListCtrlProc_OnBeginEditRow(texu_wnd* wnd, texu_i32 row);
void                  _TexuListCtrlProc_OnBeginInsertRow(texu_wnd* wnd, texu_i32 row);
void                  _TexuListCtrlProc_OnBeginInsertCell(texu_wnd* wnd, texu_i32 row);
void                  _TexuListCtrlProc_OnBeginEdit(texu_wnd* wnd);
void                  _TexuListCtrlProc_OnEndEdit(texu_wnd* wnd, texu_i32 ok);
void                  _TexuListCtrlProc_OnDeleteItem(texu_wnd* wnd, texu_i32 idx);





texu_i32
_TexuListCtrlProc_FindHeaderIndex(texu_lcwnd* lctl, texu_lcwnd_header* header)
{
  texu_i32 col = -1;
  texu_lcwnd_header* ctlhdr = lctl->firsthdr;
  
  if (header && ctlhdr)
  {
    while (ctlhdr)
    {
      ++col;
      if (header == ctlhdr)
      {
        break;
      }
      ctlhdr = ctlhdr->next;
    }
  }
  return col;
}

texu_lcwnd_header*
_TexuListCtrlProc_FindHeaderByIndex(texu_lcwnd* lctl, texu_i32 col)
{
  texu_lcwnd_header* header = 0;
  texu_i32 i = 0;
  
  if (col < 0 || col >= lctl->nheaders)
  {
    return 0; /* no header deleted */
  }
  
  header = lctl->firsthdr;
  while (header)
  {
    if (i == col)
    {
      break;
    }
    header = header->next;
    ++i;
  }
  return header;
}

texu_lcwnd_cell*
_TexuListCtrlProc_FindCellByIndex(texu_lcwnd* lctl, texu_i32 col, texu_i32 idx)
{
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_header* header = 0;
  texu_i32 i = 0;
  
  if (idx < 0 || idx >= lctl->nitems)
  {
    return 0;
  }
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
  if (header)
  {
    cell = header->firstcell;
    while (cell)
    {
      if (i == idx)
      {
        break;
      }
      cell = cell->next;
      ++i;
    }
  }
  return cell;
}

texu_lcwnd_cell*
_TexuListCtrlProc_FindCellByHeader(texu_lcwnd* lctl, texu_lcwnd_header* header, texu_i32 idx)
{
  texu_lcwnd_cell* cell = 0;
  texu_i32 i = 0;
  
  if (idx < 0 || idx >= lctl->nitems)
  {
    return 0;
  }

  if (header)
  {
    cell = header->firstcell;
    while (cell)
    {
      if (i == idx)
      {
        break;
      }
      cell = cell->next;
      ++i;
    }
  }
  return cell;
}

texu_i32
_TexuListCtrlProc_GetCellRect(texu_lcwnd_cell* cell, texu_rect* rect)
{
  rect->y = cell->y;
  rect->x = cell->x;
  rect->lines = cell->lines;
  rect->cols  = cell->cols;
  return TEXU_OK;
}

texu_status
_TexuListCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_lcwnd* lctl = 0;
  texu_i32 editable = 0;
  texu_wnd_attrs attrs2;
  texu_wnd* editwnd = 0;
  texu_status rc = TEXU_OK;
  
  editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
  if (!editwnd)
  {
    return TEXU_NOMEM;
  }
  
  memset(&attrs2, 0, sizeof(attrs2));
  attrs2.y          = 0;
  attrs2.x          = 0;
  attrs2.height     = 1;
  attrs2.width      = 1;
  attrs2.enable     = TEXU_FALSE;
  attrs2.visible    = TEXU_FALSE;
  attrs2.text       = "";
  attrs2.normalcolor    = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs2.disabledcolor  = TEXU_CIO_COLOR_WHITE_BLACK;
  attrs2.id         = 1;
  attrs2.clsname    = TEXU_EDIT_CLASS;
  attrs2.userdata   = 0;
  attrs2.style      = TEXU_ES_AUTOHSCROLL;
  attrs2.exstyle    = 0;
  
  rc = texu_wnd_create(editwnd, wnd, &attrs2);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(editwnd);
    return TEXU_ERROR;
  }
 
  lctl = (texu_lcwnd*)malloc(sizeof(texu_lcwnd));
  if (!lctl)
  {
    texu_wnd_del(editwnd);
    return TEXU_NOMEM;
  }
  
  memset(lctl, 0, sizeof(texu_lcwnd));
  lctl->hdrids    = 1;

  lctl->editbox = editwnd;   /* no parameter */
  lctl->curselrow = -1;
  lctl->curselcol = -1;
  lctl->firstvisiblerow = -1;
  
  lctl->editkey         = KEY_BACKSPACE;
  lctl->insertkey       = KEY_IC;
  lctl->delkey          = KEY_DC;
  lctl->canceleditkey   = 0x1B;
  
  lctl->curedtcol = -1;
  lctl->nedtcols  = 256;
  lctl->editcols  = (texu_i32*)malloc(lctl->nedtcols * sizeof(texu_i32));
  if (TEXU_LCS_EDITABLE & attrs->style)
  {
    editable = 1;
  }
  memset(lctl->editcols, editable, lctl->nedtcols * sizeof(texu_i32));
  lctl->firsteditcell = 0;
  lctl->lasteditcell  = 0;
  /* increment child ids */
  ++lctl->hdrids;
  
  /* save memory */
  texu_wnd_set_color(wnd,
    TEXU_CIO_COLOR_CYAN_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
  texu_wnd_set_userdata(wnd, lctl);
  return TEXU_OK;
}

void
_TexuListCtrlProc_OnSelChanged(texu_wnd* wnd)
{
  _TexuWndProc_Notify(wnd, TEXU_LCN_SELCHANGED);
}

void
_TexuListCtrlProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  _TexuWndProc_Notify(wnd, TEXU_LCN_SETFOCUS);
}

texu_i32
_TexuListCtrlProc_OnKillFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  texu_lcwnd* lctl = 0;
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->movingstate == TEXU_LCT_MOVINGCURSOR)
  {
    _TexuListCtrlProc_OnEndMoving(wnd);
    lctl->movingstate = TEXU_LCT_VIEW;
  }
  else if (lctl->editingstate == TEXU_LCT_EDITING)
  {
    _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL);
    lctl->editingstate = TEXU_LCT_VIEW;
  }
  /* send notification */

  _TexuWndProc_Notify(wnd, TEXU_LCN_KILLFOCUS);

  return TEXU_CONTINUE;
}

void
_TexuListCtrlProc_OnDestroy(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  
  _TexuListCtrlProc_OnDeleteAllItems(wnd);
  free(lctl);
}

void
_TexuListCtrlProc_OnSetEditableCols(texu_wnd* wnd, texu_i32 ncols, texu_i32* edtcols)
{
  texu_lcwnd* lctl = 0;
  texu_i32 i = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (0 == edtcols)
  {
    /* this means re*/
    return;
  }
  if (ncols > lctl->nheaders)
  {
    ncols = lctl->nheaders;
  }
  for (i = 0; i < ncols; ++i)
  {
    lctl->editcols[i] = edtcols[i];
  }
}

texu_i32
_TexuListCtrlProc_OnAddColumn(texu_wnd* wnd, texu_wnd_header* hdritem)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* header = 0;
  texu_rect rc;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  
  if (lctl->nitems > 0)
  {
    /* not allowed to add header after there are any items */
    return TEXU_ERROR;
  }

  header = (texu_lcwnd_header*)malloc(sizeof(texu_lcwnd_header));
  if (!header)
  {
    return TEXU_NOMEM;
  }
  memset(header, 0, sizeof(texu_lcwnd_header));
  
  texu_wnd_get_rect(texu_wnd_get_parent(wnd), &rc);
  strncpy(header->caption,
    hdritem->caption,
    TEXU_MIN(TEXU_MAX_WNDTEXT, strlen(hdritem->caption)));
  header->cols      = hdritem->cols;
  header->align     = hdritem->align;
  
  header->normcolor = hdritem->normcolor;    /* text attributes          */
  header->discolor  = hdritem->discolor ;    /* text attributes          */
  header->selcolor  = hdritem->selcolor ;    /* text attributes          */

  header->editstyle = hdritem->editstyle;
  header->decwidth  = hdritem->decwidth;
  header->id        = lctl->hdrids;
 
  /* make link */
  if (lctl->firsthdr)
  {
    header->prev = lctl->lasthdr;
    lctl->lasthdr->next = header;
    lctl->lasthdr = header;
  }
  else
  {
    lctl->firsthdr = lctl->lasthdr = header;
    lctl->firstvisiblehdr = header;
  }
  /* increment child ids */
  ++lctl->hdrids;
  ++lctl->nheaders;
  lctl->hdrallwidths += header->cols;
  
  return 0;
}

void
_TexuListCtrlProc_OnDeleteColumn(texu_wnd* wnd, texu_i32 col)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* next = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_cell* nextcell = 0;
  texu_lcwnd_header* header = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
  if (!header)
  {
    return;
  }

  /* re-link */
  next = header->next;
  if (next)
  {
    next->prev = header->prev;
  }
  if (header->prev)
  {
    header->prev->next = next;
  }
  
  if (header == lctl->firsthdr)
  {
    lctl->firsthdr = next;
  }
  else if (header == lctl->lasthdr)
  {
    lctl->lasthdr = header->prev;
  }
  /* delete */
  header->next = header->prev = 0;

  cell = header->firstcell;
  while (cell)
  {
    nextcell = cell->next;
    /*free(cell->caption);*/
    cell->next = cell->prev = 0;
    free(cell);
    cell = nextcell;
  }

  free(header);

  /* done */
  --lctl->nheaders;
}

void
_TexuListCtrlProc_OnDeleteAllItems(texu_wnd* wnd)
{
  texu_i32 nitems = 0;
  texu_i32 i = 0;
  texu_lcwnd* lctl = 0;

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  nitems = lctl->nheaders;
  for (i = 0; i < nitems; ++i)
  {
    texu_wnd_send_msg(wnd, TEXU_LCM_DELETEITEM, 0, 0);
  }
}

void
_TexuListCtrlProc_OnDeleteItem(texu_wnd* wnd, texu_i32 idx)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_cell* nextcell = 0;
  texu_lcwnd_header* header = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->nitems <= 0 || idx < 0 || idx >= lctl->nitems)
  {
    return;
  }
  
  header = lctl->firsthdr;
  while (header)
  {
    cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, idx);
    nextcell = cell->next;
    if (nextcell)
    {
      nextcell->prev = cell->prev;
    }
    if (cell->prev)
    {
      cell->prev->next = nextcell;
    }
    if (header->firstcell == cell)
    {
      header->firstcell = cell->next;
    }
    else if (header->lastcell == cell)
    {
      header->lastcell = cell->prev;
    }
    
    cell->next = cell->prev = 0;
    free(cell);
    /* next header */
    header = header->next;
  }
  --lctl->nitems;
}

texu_i32
_TexuListCtrlProc_OnAddItem(texu_wnd* wnd, texu_char* text, texu_i32 nitems)
{
  texu_lcwnd* lctl = 0;
  texu_i32 i = 0;
  texu_char* tok;
  texu_lcwnd_header* header = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_lcwnd_cell* newcell = 0;

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->nheaders > 0)
  {
    /* insert into all listboxes */
    header = lctl->firsthdr;
    
    strcpy(buf, text);
    tok = strtok(buf, "\t");
    while (/*tok &&*/ i < nitems && i < lctl->nheaders)
    {
      newcell = (texu_lcwnd_cell*)malloc(sizeof(texu_lcwnd_cell));
      if (!newcell)
      {
        break;
      }
      memset(newcell, 0, sizeof(texu_lcwnd_cell));
      if (tok)
      {
        strncpy(newcell->caption, tok, TEXU_MIN(TEXU_MAX_WNDTEXT, strlen(tok)));
      }
      else
      {
        strcpy(newcell->caption, "");
      }
      newcell->normcolor = TEXU_CIO_COLOR_WHITE_BLACK;
      newcell->discolor  = TEXU_CIO_COLOR_WHITE_BLACK;
      newcell->selcolor  = TEXU_CIO_COLOR_BLACK_WHITE;
      
      /* add the new item */
      if (header->firstcell)
      {
        newcell->prev  = header->lastcell;
        header->lastcell->next = newcell;
        header->lastcell = newcell;
      }
      else
      {
        header->firstcell = header->lastcell = newcell;

        lctl->firstvisiblerow   = 0;
      }
      /* insert next */
      header = header->next;
      tok = strtok(0, "\t");
      ++i;
    }
    /* all items count */
    ++lctl->nitems;

  } /* have header */
  return lctl->nitems;
}

void
_TexuListCtrlProc_DrawItem(
  texu_cio* dc, texu_rect* rccell, const texu_char* caption, 
  texu_ui32 color, texu_i32 align, texu_i32 isheader)
{
  texu_i32 len = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  len = texu_printf_alignment(buf, 
          caption,
          rccell->cols,
          align);
  /* is is a header */
  if (isheader)
  {
    buf[0] = '[';
    buf[len - 1] = ']';
  }

  texu_cio_putstr_attr(dc,
      rccell->y,
      rccell->x,
      buf,
      COLOR_PAIR(color));
}

void
_TexuListCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* header = 0;
  texu_i32 width = 0;
  texu_rect rcitem, rcwnd, rccell;
  /*texu_ui32 attrs = 0;*/
  texu_i32 normcolor = 0;
  /*texu_i32 discolor = 0;*/
  texu_i32 selcolor = 0;
  texu_lcwnd_cell* visiblecell = 0;
  texu_i32 i = 0;
  texu_i32 forcewidth = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  
  /* if no headers or control is hiding */
  if (lctl->nheaders <= 0 || !texu_wnd_is_visible(wnd))
  {
    return;
  }

  /* draw headers */
  texu_wnd_get_rect(wnd, &rcwnd);
  rcitem = rcwnd; /* copy some values */
  rcitem.lines = (style & TEXU_LCS_NOHEADER ? 0 : 1);
  
  header = lctl->firstvisiblehdr;
  width  = header->cols;
  
  /* first column width must not be over window columns */
  if (width > rcwnd.cols)
  {
    width = rcwnd.cols - 1;
    forcewidth = 1;
  }
  
  while (header && width < rcwnd.cols)
  {    
    rcitem.x    = rcwnd.x + width - header->cols;
    rcitem.cols = header->cols;
    if (rcitem.x < 0)
    {
      rcitem.x = rcwnd.x;
      if (forcewidth == 1)
      {
        rcitem.cols = width;
      }
      else
      {
        --rcitem.cols;
      }
    }

    header->normcolor =    header->normcolor;
    header->discolor  =    header->discolor;
    header->selcolor  =    header->selcolor;
  
    normcolor =    header->normcolor;
    /*discolor  =    header->discolor;*/
    selcolor  =    header->selcolor;

    if (!(style & TEXU_LCS_NOHEADER))
    {
        if (style & TEXU_LCS_NOBORDER)
        {
            _TexuListCtrlProc_DrawItem(dc, &rcitem,
                header->caption, normcolor, header->align, 0); /* 0=no border */
        }
        else
        {
            _TexuListCtrlProc_DrawItem(dc, &rcitem,
                header->caption, normcolor, TEXU_ALIGN_CENTER, 1); /* 1=header */
        }
    }

    /* draw cells */
    rccell = rcitem;
    rccell.y += (style & TEXU_LCS_NOHEADER ? 0 : 1);
    rccell.lines = 1;
    visiblecell = header->firstcell;
    for (i = 0; i < lctl->nitems && visiblecell;
        ++i, visiblecell = visiblecell->next)
    {
      if (i < lctl->firstvisiblerow)
      {
        /* do nothing */
        continue;
      }
      else if (i - lctl->firstvisiblerow <= rcwnd.lines - 2)
      {

        normcolor = visiblecell->normcolor;
        /*discolor  =    header->discolor;*/
        selcolor  = visiblecell->selcolor;
        if (!(TEXU_LCS_NOSELECTION & style) && i == lctl->curselrow)
        {
          normcolor = selcolor;
        }
        /* draw th item that it can be seen */
        _TexuListCtrlProc_DrawItem(dc, &rccell, 
          visiblecell->caption, normcolor, header->align, 0);

        /* update cell rect */
        visiblecell->y = rccell.y;
        visiblecell->x = rccell.x;
        visiblecell->lines = rccell.lines;
        visiblecell->cols  = rccell.cols;
        /* next cell line */
        ++rccell.y;
      }
      else
      {
        /* no need to draw more items than client lines */
        break;
      }
    } /* for each drawing cell */
    
    /* draw next header */
    header = header->next;
    if (header)
    {
      width += header->cols;
      if (width > rcwnd.cols)
      {
        break;
      }
    }
  } /* while header */
  
  /* print arrow controls */
  if (visiblecell)
  {
    normcolor = visiblecell->normcolor;
  }
  else
  {
    normcolor = 0;
  }
  if (lctl->firsthdr != lctl->firstvisiblehdr)
  {
    if (!(style & TEXU_LCS_NOHEADER))
    {
      texu_cio_putch_attr(dc, rcwnd.y, rcwnd.x + 1, '<', normcolor);
    }
  }
  /* save the last visible */
  if (header)
  {
    lctl->lastvisiblehdr = header->prev;
    if (!(style & TEXU_LCS_NOHEADER))
    {
      texu_cio_putch_attr(dc, rcwnd.y, 
        rcwnd.x + width - header->cols - 2, '>', normcolor);
    }
  }
  else
  {
    lctl->lastvisiblehdr = lctl->lasthdr;
  }

  texu_cio_gotoyx(dc, rcitem.y,
      rcitem.x);
}

void _TexuListCtrlProc_OnBeginMoving(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  texu_rect rccell;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_header* header = 0;
  texu_i32 curselcol = -1;
    
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->movingstate == TEXU_LCT_MOVINGCURSOR)
  {
    _TexuListCtrlProc_OnEndMoving(wnd);
    lctl->movingstate = TEXU_LCT_VIEW;
    return;
  }
  
  lctl->movingstate = TEXU_LCT_MOVINGCURSOR;
  /* move cursor to the current row */
  curselcol = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr);
  if (curselcol < 0)
  {
    lctl->curselrow = 0;
    texu_wnd_invalidate(wnd);
  }

  /* draw moving cursor */
  if (0 > lctl->curselcol)
  {
    lctl->curselcol = curselcol;
  }
  if (0 > lctl->curselrow)
  {
    lctl->curselrow = 0;
  }
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
  cell   = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
  _TexuListCtrlProc_GetCellRect(cell, &rccell);
  
  _TexuListCtrlProc_DrawItem(texu_wnd_get_cio(wnd), &rccell, 
          cell->caption,
          cell->normcolor,
          header->align, 0);

  /* move cursor */
  texu_cio_gotoyx(texu_wnd_get_cio(wnd), rccell.y, rccell.x);
  
  /* save the editing cell */
  lctl->editingcell = cell;
  /* send notification */
  _TexuWndProc_Notify(wnd, TEXU_LCN_BEGINMOVING);
}

void _TexuListCtrlProc_OnMovingCursor(texu_wnd* wnd, texu_i32 ch)
{
  texu_lcwnd* lctl = 0;
  texu_i32 firstcol = -1, lastcol = -1;
  texu_i32 newcol = -1;
  texu_i32 newrow = -1;
  texu_rect rccell;
  texu_rect rcwnd;
  texu_lcwnd_cell* cell = 0;  
  texu_lcwnd_header* header = 0;
  texu_i32 maxrows = 25;
  texu_ui32 style = texu_wnd_get_style(wnd);

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->movingstate != TEXU_LCT_MOVINGCURSOR)
  {
    return;
  }
  
  texu_wnd_get_rect(wnd, &rcwnd);
  firstcol = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr);
  lastcol  = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->lastvisiblehdr);
  newcol   = lctl->curselcol;
  newrow   = lctl->curselrow;
  
  maxrows  = lctl->firstvisiblerow + rcwnd.lines - 2;
  
  switch (ch)
  {
    case KEY_LEFT:
    {
      newcol = _TexuListCtrlProc_GetPrevEditableCol(lctl, lctl->curedtcol);
      if (newcol < 0)
      {
        newcol = _TexuListCtrlProc_GetFirstEditableCol(lctl);
      }
        /*--newcol;*/
      if (newcol < firstcol)
      {
        newcol = firstcol;
      }
      break;
    }
    
    case KEY_RIGHT:
    {
      newcol = _TexuListCtrlProc_GetNextEditableCol(lctl, lctl->curedtcol);
      if (newcol < 0)
      {
        newcol = _TexuListCtrlProc_GetLastEditableCol(lctl);
      }
      if (newcol < 0)
      {
        
      }
      /*++newcol;*/
      if (newcol > lastcol)
      {
        newcol = lastcol;
      }
      break;
    }
    
    case KEY_DOWN:
    {
      if (!(TEXU_LCS_LINEEDIT & style))
      {
        /* not fixed line edit style */
        ++newrow;
        if (newrow > maxrows)
        {
          newrow = maxrows;
        }
      }
      break;
    }
    
    case KEY_UP:
    {
      if (!(TEXU_LCS_LINEEDIT & style))
      {
        /* not fixed line edit style */
        --newrow;
        if (newrow < lctl->firstvisiblerow)
        {
          newrow = lctl->firstvisiblerow;
        }
      }
      break;
    }
  }
  
  if (newcol != lctl->curselcol || newrow != lctl->curselrow)
  {
    /* redraw the previous moving cursor */
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
    cell   = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);
    _TexuListCtrlProc_DrawItem(
      texu_wnd_get_cio(wnd),
      &rccell,
      cell->caption,
      cell->normcolor,
      header->align,
      0);

    /* draw moving cursor */
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, newcol);
    cell   = _TexuListCtrlProc_FindCellByHeader(lctl, header, newrow);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);
    _TexuListCtrlProc_DrawItem(texu_wnd_get_cio(wnd),
      &rccell,
      cell->caption,
      cell->normcolor,
      header->align,
      0);
    
    texu_cio_gotoyx(texu_wnd_get_cio(wnd), rccell.y, rccell.x);

    /* save the last current selected column */
    lctl->curselcol = newcol;
    lctl->curselrow = newrow;
    /* save the editing cell */
    lctl->curedtcol = newcol;
    lctl->editingcell = cell;
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
    }
  }
}

void _TexuListCtrlProc_OnEndMoving(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_header* header = 0;
  texu_rect rccell;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->movingstate != TEXU_LCT_MOVINGCURSOR)
  {
    return;
  }
  lctl->movingstate = TEXU_LCT_ENDMOVING;
  
  /* redraw the previous moving cursor */
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
  cell   = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
  _TexuListCtrlProc_GetCellRect(cell, &rccell);

  _TexuListCtrlProc_DrawItem(
    texu_wnd_get_cio(wnd),
    &rccell, 
    cell->caption,
    cell->normcolor,
    header->align,
    0);

  /* update state */
  lctl->movingstate   = TEXU_LCT_VIEW;
  
  texu_wnd_invalidate(wnd);
  /* send notification */

  _TexuWndProc_Notify(wnd, TEXU_LCN_ENDMOVING);
}


void  _TexuListCtrlProc_OnBeginInsertRow(texu_wnd* wnd, texu_i32 row)
{
  texu_lcwnd* lctl = 0;


  if (row < 0)
  {
    row = 0;
  }
  /* save all cells on the row */
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  lctl->insertingstate = TEXU_LCT_INSERTING;
  
  /* send notification */

  _TexuListCtrlProc_NotifyItem(wnd, TEXU_LCN_BEGININSERTROW, lctl->curselrow, lctl->curselcol);
}

void  _TexuListCtrlProc_OnEndInsertRow(texu_wnd* wnd, texu_i32 row, texu_i32 ok)
{

  texu_lcwnd* lctl = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  lctl->insertingstate = TEXU_LCT_ENDINSERTING;
  
  if (TEXU_LC_ENDEDITCANCEL == ok)
  {
    /* rollback */
    _TexuListCtrlProc_OnDeleteItem(wnd, row);
  }
   
  /* send notification */

  _TexuListCtrlProc_NotifyItem(wnd,
    (TEXU_LC_ENDEDITOK == ok ? TEXU_LCN_ENDINSERTROWOK : TEXU_LCN_ENDINSERTROWCANCEL),
    lctl->curselrow, lctl->curselcol);
}

void  _TexuListCtrlProc_OnBeginEditRow(texu_wnd* wnd, texu_i32 row)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_cell* newcell = 0;
  texu_i32 i = 0;


  if (row < 0)
  {
    row = 0;
  }
  /* save all cells on the row */
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  
  lctl->firsteditcell = lctl->lasteditcell = 0;
  
  for (i=0; i<lctl->nheaders; ++i)
  {
    cell = _TexuListCtrlProc_FindCellByIndex(lctl, i, row);
    newcell = (texu_lcwnd_cell*)malloc(sizeof(texu_lcwnd_cell));
    memcpy(newcell, cell, sizeof(texu_lcwnd_cell));
    newcell->next = newcell->prev = 0;
    
    if (0 == lctl->lasteditcell)
    {
      lctl->firsteditcell = lctl->lasteditcell = newcell;
    }
    else
    {
      newcell->prev = lctl->lasteditcell;
      lctl->lasteditcell->next = newcell;
      lctl->lasteditcell = newcell;
    }
  }
  /* send notification */

  _TexuListCtrlProc_NotifyItem(wnd, TEXU_LCN_BEGINEDITROW, lctl->curselrow, lctl->curselcol);
}

void  _TexuListCtrlProc_OnEndEditRow(texu_wnd* wnd, texu_i32 row, texu_i32 ok)
{

  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_cell* nextcell = 0;
  texu_i32 i = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (TEXU_LC_ENDEDITCANCEL == ok)
  {
    /* rollback */
    nextcell = lctl->firsteditcell;
    for (i=0; i<lctl->nheaders && nextcell != 0; ++i)
    {
      cell = _TexuListCtrlProc_FindCellByIndex(lctl, i, row);
      strcpy(cell->caption, nextcell->caption);
      nextcell = nextcell->next;
    }
  }
  
  /* release backed-up cells */
  nextcell = lctl->firsteditcell;
  while (nextcell)
  {
    cell = nextcell;
    nextcell = nextcell->next;
    
    free(cell);
  }
  lctl->firsteditcell = lctl->lasteditcell = 0;
  
  /* send notification */

  _TexuListCtrlProc_NotifyItem(wnd,
    (TEXU_LC_ENDEDITOK == ok ? TEXU_LCN_ENDEDITROWOK : TEXU_LCN_ENDEDITROWCANCEL),
    lctl->curselrow, lctl->curselcol);
}

void
_TexuListCtrlProc_NotifyItem(texu_wnd* wnd, texu_ui32 code, texu_i32 row, texu_i32 col)
{
  texu_header_notify nmhdri;
  /* send notification */
  nmhdri.hdr.id   = texu_wnd_get_id(wnd);
  nmhdri.hdr.wnd  = wnd;
  nmhdri.hdr.code = code;
  nmhdri.row      = row;
  nmhdri.col      = col;

  texu_wnd_send_msg(texu_wnd_get_parent(wnd), TEXU_WM_NOTIFY, (texu_i64)&nmhdri, 0);
}

void _TexuListCtrlProc_OnBeginEdit(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  texu_rect rccell;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_ui32 editstyle = 0;
  
  if (!(texu_wnd_get_style(wnd) & TEXU_LCS_EDITABLE))
  {
    /* if it has no EDIT style */
    return;
  }
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);

  lctl->editingstate = TEXU_LCT_BEGINEDITING;
  /* show edit box at the current row and column */
  _TexuListCtrlProc_GetCellRect(lctl->editingcell, &rccell);
  texu_wnd_move(lctl->editbox, rccell.y, rccell.x, rccell.lines, rccell.cols);

  editstyle = _TexuListCtrlProc_OnGetEditStyle(wnd, lctl->curselcol);

  texu_wnd_set_style(lctl->editbox, editstyle);
  if (editstyle & TEXU_ES_DECIMAL)
  {
    texu_lcwnd_header* header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
    texu_wnd_send_msg(lctl->editbox, TEXU_EM_SETDECWIDTH, header->decwidth, 0);
  }
  
  /* update text before showing */
  strcpy(buf, lctl->editingcell->caption);
  texu_wnd_set_text(lctl->editbox, buf);
  texu_wnd_visible(lctl->editbox, TEXU_TRUE);
  
  /*TuiSetFocus(lctl->editbox);*/
  
  /* update state */
  lctl->editingstate = TEXU_LCT_EDITING;
  
  _TexuListCtrlProc_NotifyItem(wnd, TEXU_LCN_BEGINEDIT, lctl->curselrow, lctl->curselcol);
  texu_wnd_invalidate(wnd);
}

void _TexuListCtrlProc_OnEndEdit(texu_wnd* wnd, texu_i32 ok)
{
  texu_lcwnd* lctl = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_i32 rc = TEXU_CONTINUE;
  
  if (!(texu_wnd_get_style(wnd) & TEXU_LCS_EDITABLE))
  {
    /* if it has no EDIT style */
    return;
  }
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (lctl->editingstate != TEXU_LCT_EDITING)
  {
    return;
  }
  lctl->editingstate = TEXU_LCT_ENDEDITING;

  /* hide edit box */
  rc = texu_wnd_send_msg(lctl->editbox, TEXU_WM_KILLFOCUS, 0, 0);
  texu_wnd_visible(lctl->editbox, TEXU_FALSE);
  if (ok)
  {
    if (TEXU_CONTINUE == rc)
    {
      /* update */
      texu_wnd_get_text(lctl->editbox, buf, TEXU_MAX_WNDTEXT);
      strcpy(lctl->editingcell->caption, buf);
    }
  }
  
  /* send notification */

  _TexuListCtrlProc_NotifyItem(wnd, 
    (TEXU_LC_ENDEDITOK == ok ? TEXU_LCN_ENDEDITOK : TEXU_LCN_ENDEDITCANCEL),
    lctl->curselrow, lctl->curselcol); 
  
  texu_wnd_invalidate(wnd);
}



void _TexuListCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch)
{
  texu_lcwnd* lctl = 0;
  texu_i32 repaint = 0;
  texu_rect rc;
  texu_i32 lines = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 row = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  texu_wnd_get_rect(wnd, &rc);
  
  if (lctl->nheaders <= 0)
  {
    return;
  }
  
  row = lctl->curselrow;
  if (row < 0)
  {
    row = 0;
  }
  if (ch == lctl->editkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK); /* edit ok */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITOK);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITOK);
      }
      return;
    }
    else
    {
      if (lctl->nitems <= 0)
      {
        return;
      }
      lctl->curedtcol = _TexuListCtrlProc_GetFirstEditableCol(lctl);
      lctl->curselcol = lctl->curedtcol;
      _TexuListCtrlProc_OnBeginMoving(wnd);
      _TexuListCtrlProc_OnBeginEditRow(wnd, row);
      _TexuListCtrlProc_OnBeginEdit(wnd); /* begin editing */      
    }
  }
  else if (ch == lctl->canceleditkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      _TexuListCtrlProc_OnSetCurRow(wnd, row-1);
    }
    return;
  }
  else if (ch == lctl->insertkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      _TexuListCtrlProc_OnSetCurRow(wnd, row-1);
    }
    else
    {
      row = _TexuListCtrlProc_InsertEmptyItem(wnd);
      if (row <= 0)
      {
        return;
      }
      --row; /* move to the last inserted row */
      /* find the properly first visible row */
      if (row >= lctl->firstvisiblerow + rc.lines - 1)
      {
        lctl->firstvisiblerow += 1;
        if (lctl->firstvisiblerow - 1 > lctl->nitems - rc.lines)
        {
          lctl->firstvisiblerow = lctl->nitems - rc.lines;
        }
      }
      else if (lctl->firstvisiblerow > row)
      {
        lctl->firstvisiblerow += 1;
        if (lctl->firstvisiblerow < 0)
        {
          lctl->firstvisiblerow = 0;
        }
      }      
      _TexuListCtrlProc_OnSetCurRow(wnd, row);
      
      lctl->curedtcol = _TexuListCtrlProc_GetFirstEditableCol(lctl);
      lctl->curselcol = lctl->curedtcol;
      _TexuListCtrlProc_OnBeginMoving(wnd);
      _TexuListCtrlProc_OnBeginInsertRow(wnd, row);
      _TexuListCtrlProc_OnBeginEdit(wnd); /* begin editing */      
    }
    return;
  }
  else if (ch == lctl->delkey)
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      _TexuListCtrlProc_OnEndMoving(wnd);
    }
    else if (TEXU_LCT_INSERTING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      _TexuListCtrlProc_OnEndMoving(wnd);
    }
    _TexuListCtrlProc_OnDeleteItem(wnd, row);
    if (row >= lctl->nitems)
    {
      --row;
    }
    if (row < 0)
    {
      texu_wnd_invalidate(wnd);
    }
    else
    {
      _TexuListCtrlProc_OnSetCurRow(wnd, row);
    }
    return;
  }

  switch (ch)
  {
    case KEY_RIGHT:
    {
      if (lctl->editingstate == TEXU_LCT_EDITING)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
        _TexuListCtrlProc_OnMovingCursor(wnd, ch);
        _TexuListCtrlProc_OnBeginEdit(wnd);
      }
      else if (lctl->lastvisiblehdr != lctl->lasthdr)
      {
        lctl->firstvisiblehdr = lctl->firstvisiblehdr->next;
        ++repaint;
      }
      break;
    }

    case KEY_LEFT:
    {
      if (lctl->editingstate == TEXU_LCT_EDITING)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
        _TexuListCtrlProc_OnMovingCursor(wnd, ch);
        _TexuListCtrlProc_OnBeginEdit(wnd);
      }
      else if (lctl->firstvisiblehdr != lctl->firsthdr)
      {
        lctl->firstvisiblehdr = lctl->firstvisiblehdr->prev;
        ++repaint;
      }
      break;
    }

    case KEY_DOWN:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        ++lines;
        ++repaint;
      }
      break;
    }
  

    case KEY_UP:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        --lines;
        ++repaint;
      }
      break;
    }
      
    case KEY_PPAGE:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        lines -= rc.lines - 1;
        ++repaint;
      }
      break;
    }
    
    case KEY_NPAGE:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        lines += rc.lines - 1;
        ++repaint;
      }
      break;
    }
    
    default:
    {
       break;
    }
  }
  if (repaint)
  {
    lctl->curselrow += lines;
    if (lctl->curselrow >= lctl->nitems)
    {
      lctl->curselrow = lctl->nitems - 1;
    }
    else if (lctl->curselrow < 0)
    {
      lctl->curselrow = 0;
    }
    
    if (lctl->curselrow >= lctl->firstvisiblerow + rc.lines - 1)
    {
      lctl->firstvisiblerow += lines;
      if (lctl->firstvisiblerow - 1 > lctl->nitems - rc.lines)
      {
        lctl->firstvisiblerow = lctl->nitems - rc.lines;
      }
    }
    else if (lctl->firstvisiblerow > lctl->curselrow)
    {
      lctl->firstvisiblerow += lines;
      if (lctl->firstvisiblerow < 0)
      {
        lctl->firstvisiblerow = 0;
      }
    }
    /* send notification */
    _TexuListCtrlProc_OnSelChanged(wnd);
    
    texu_wnd_invalidate(wnd);
  }
}


texu_i32 _TexuListCtrlProc_GetFirstEditableCol(texu_lcwnd* lctl)
{
  return _TexuListCtrlProc_GetNextEditableCol(lctl, -1);
}

texu_i32 _TexuListCtrlProc_GetNextEditableCol(texu_lcwnd* lctl, texu_i32 prevcol)
{
  texu_i32 col = -1;
  texu_i32 i = 0;
  for (i = prevcol+1; i < lctl->nheaders; ++i)
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      if (lctl->editcols[i] != 0)
      {
        col = i;
        break;
      }
    }
    else
    {
      col = i;
      break;
    }
  }
  return col;
}

texu_i32 _TexuListCtrlProc_InsertEmptyItem(texu_wnd* wnd)
{
  texu_char sz[TEXU_MAX_WNDTEXT+1];
  texu_lcwnd* lctl = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  memset(sz, '\t', lctl->nheaders);
  sz[lctl->nheaders] = 0;
  return _TexuListCtrlProc_OnAddItem(wnd, sz, lctl->nheaders);
}

texu_i32 _TexuListCtrlProc_GetLastEditableCol(texu_lcwnd* lctl)
{
  return _TexuListCtrlProc_GetPrevEditableCol(lctl, lctl->nheaders);
}

texu_i32 _TexuListCtrlProc_GetPrevEditableCol(texu_lcwnd* lctl, texu_i32 prevcol)
{
  texu_i32 col = -1;
  texu_i32 i = 0;
  for (i = prevcol-1; i>=0; --i)
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      if (lctl->editcols[i] != 0)
      {
        col = i;
        break;
      }
    }
    else
    {
      col = i;
      break;
    }
  }
  return col;
}

void _TexuListCtrlProc_OnKeyDown(texu_wnd* wnd, texu_i32 ch)
{
  texu_lcwnd* lctl = 0;
  texu_i32 repaint = 0;
  texu_rect rc;
  texu_i32 lines = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 row = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  texu_wnd_get_rect(wnd, &rc);
  
  if (lctl->nheaders <= 0)
  {
    return;
  }
  
  row = lctl->curselrow;
  if (row < 0)
  {
    row = 0;
  }
  if (ch == lctl->editkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK); /* edit ok */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITOK);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITOK);
      }
      return;
    }
    else
    {
      if (lctl->nitems <= 0)
      {
        return;
      }
      lctl->curedtcol = _TexuListCtrlProc_GetFirstEditableCol(lctl);
      lctl->curselcol = lctl->curedtcol;
      _TexuListCtrlProc_OnBeginMoving(wnd);
      _TexuListCtrlProc_OnBeginEditRow(wnd, row);
      _TexuListCtrlProc_OnBeginEdit(wnd); /* begin editing */      
    }
  }
  else if (ch == lctl->canceleditkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      _TexuListCtrlProc_OnSetCurRow(wnd, row-1);
    }
    return;
  }
  else if (ch == lctl->insertkey && (style & TEXU_LCS_EDITABLE))
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndMoving(wnd);
      if (TEXU_LCT_INSERTING == lctl->insertingstate)
      {
        _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      else
      {
        _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      }
      _TexuListCtrlProc_OnSetCurRow(wnd, row-1);
    }
    else
    {
      row = _TexuListCtrlProc_InsertEmptyItem(wnd);
      if (row <= 0)
      {
        return;
      }
      --row; /* move to the last inserted row */
      /* find the properly first visible row */
      if (row >= lctl->firstvisiblerow + rc.lines - 1)
      {
        lctl->firstvisiblerow += 1;
        if (lctl->firstvisiblerow - 1 > lctl->nitems - rc.lines)
        {
          lctl->firstvisiblerow = lctl->nitems - rc.lines;
        }
      }
      else if (lctl->firstvisiblerow > row)
      {
        lctl->firstvisiblerow += 1;
        if (lctl->firstvisiblerow < 0)
        {
          lctl->firstvisiblerow = 0;
        }
      }      
      _TexuListCtrlProc_OnSetCurRow(wnd, row);
      
      lctl->curedtcol = _TexuListCtrlProc_GetFirstEditableCol(lctl);
      lctl->curselcol = lctl->curedtcol;
      _TexuListCtrlProc_OnBeginMoving(wnd);
      _TexuListCtrlProc_OnBeginInsertRow(wnd, row);
      _TexuListCtrlProc_OnBeginEdit(wnd); /* begin editing */      
    }
    return;
  }
  else if (ch == lctl->delkey)
  {
    if (TEXU_LCT_EDITING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      _TexuListCtrlProc_OnEndMoving(wnd);
    }
    else if (TEXU_LCT_INSERTING == lctl->editingstate)
    {
      _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
      _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
      _TexuListCtrlProc_OnEndMoving(wnd);
    }
    _TexuListCtrlProc_OnDeleteItem(wnd, row);
    if (row >= lctl->nitems)
    {
      --row;
    }
    if (row < 0)
    {
      texu_wnd_invalidate(wnd);
    }
    else
    {
      _TexuListCtrlProc_OnSetCurRow(wnd, row);
    }
    return;
  }

  switch (ch)
  {
    case KEY_RIGHT:
    {
      if (lctl->editingstate == TEXU_LCT_EDITING)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
        _TexuListCtrlProc_OnMovingCursor(wnd, ch);
        _TexuListCtrlProc_OnBeginEdit(wnd);
      }
      else if (lctl->lastvisiblehdr != lctl->lasthdr)
      {
        lctl->firstvisiblehdr = lctl->firstvisiblehdr->next;
        ++repaint;
      }
      break;
    }

    case KEY_LEFT:
    {
      if (lctl->editingstate == TEXU_LCT_EDITING)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
        _TexuListCtrlProc_OnMovingCursor(wnd, ch);
        _TexuListCtrlProc_OnBeginEdit(wnd);
      }
      else if (lctl->firstvisiblehdr != lctl->firsthdr)
      {
        lctl->firstvisiblehdr = lctl->firstvisiblehdr->prev;
        ++repaint;
      }
      break;
    }

    case KEY_DOWN:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        ++lines;
        ++repaint;
      }
      break;
    }
  

    case KEY_UP:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        --lines;
        ++repaint;
      }
      break;
    }
      
    case KEY_PPAGE:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        /*_TexuListCtrlProc_OnMovingCursor(wnd, ch);*/
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        lines -= rc.lines - 1;
        ++repaint;
      }
      break;
    }
    
    case KEY_NPAGE:
    {
      if (TEXU_LCT_EDITING == lctl->editingstate)
      {
        _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITCANCEL); /* edit cancel */
        _TexuListCtrlProc_OnEndMoving(wnd);
        if (TEXU_LCT_INSERTING == lctl->insertingstate)
        {
          _TexuListCtrlProc_OnEndInsertRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
        else
        {
          _TexuListCtrlProc_OnEndEditRow(wnd, row, TEXU_LC_ENDEDITCANCEL);
        }
      }
      else
      {
        lines += rc.lines - 1;
        ++repaint;
      }
      break;
    }
    
    default:
    {
       break;
    }
  }
  if (repaint)
  {
    lctl->curselrow += lines;
    if (lctl->curselrow >= lctl->nitems)
    {
      lctl->curselrow = lctl->nitems - 1;
    }
    else if (lctl->curselrow < 0)
    {
      lctl->curselrow = 0;
    }
    
    if (lctl->curselrow >= lctl->firstvisiblerow + rc.lines - 1)
    {
      lctl->firstvisiblerow += lines;
      if (lctl->firstvisiblerow - 1 > lctl->nitems - rc.lines)
      {
        lctl->firstvisiblerow = lctl->nitems - rc.lines;
      }
    }
    else if (lctl->firstvisiblerow > lctl->curselrow)
    {
      lctl->firstvisiblerow += lines;
      if (lctl->firstvisiblerow < 0)
      {
        lctl->firstvisiblerow = 0;
      }
    }
    /* send notification */
    _TexuListCtrlProc_OnSelChanged(wnd);
    
    texu_wnd_invalidate(wnd);
  }
}

texu_i32 _TexuListCtrlProc_OnGetItemCount(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  return lctl->nitems;
}

texu_i32 _TexuListCtrlProc_OnSetItem(texu_wnd* wnd, texu_ui32 flags, texu_wnd_subitem* subitem)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_i32 rc = TEXU_ERROR;

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  cell = _TexuListCtrlProc_FindCellByIndex(lctl, subitem->col, subitem->idx);
  if (cell)
  {
    if (flags & TEXU_LCFM_TEXT)  { strcpy(cell->caption, subitem->text);  }
    if (flags & TEXU_LCFM_DATA)  { cell->data  = subitem->data;  }
    if (flags & TEXU_LCFM_COLOR)
    {
      cell->normcolor = subitem->normcolor;
      cell->discolor = subitem->discolor;
      cell->selcolor = subitem->selcolor;
    }
    rc = TEXU_OK;
  }
  return rc;
}

texu_i32 _TexuListCtrlProc_OnGetItem(texu_wnd* wnd, texu_ui32 flags, texu_wnd_subitem* subitem)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_i32 rc = TEXU_ERROR;

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  cell = _TexuListCtrlProc_FindCellByIndex(lctl, subitem->col, subitem->idx);
  if (cell)
  {
    if (flags & TEXU_LCFM_TEXT)  { subitem->text  = cell->caption;  }
    if (flags & TEXU_LCFM_DATA)  { subitem->data  = cell->data;  }
    if (flags & TEXU_LCFM_COLOR)
    {
      subitem->normcolor = cell->normcolor;
      subitem->discolor  = cell->discolor;
      subitem->selcolor  = cell->selcolor;
    }

    rc = TEXU_OK;
  }
  return rc;
}

void _TexuListCtrlProc_OnInvalidateItem(texu_wnd* wnd, texu_ui32 col, texu_ui32 idx)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_cell* cell = 0;
  texu_lcwnd_header* header = 0;
  texu_rect rcwnd;
  texu_rect rccell;
  texu_i32 color = 0;

  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  cell = _TexuListCtrlProc_FindCellByIndex(lctl, col, idx);
  if (cell)
  {
    /* check if the cell is available on screen */
    texu_wnd_get_rect(wnd, &rcwnd);
    if (idx >= lctl->firstvisiblerow && 
        idx <  lctl->firstvisiblerow + rcwnd.lines)
    {
      if (col >= _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr) &&
          col <  _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->lastvisiblehdr))
      {
        header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
        _TexuListCtrlProc_GetCellRect(cell, &rccell);

        
        color = cell->normcolor;
        if (idx == lctl->curselrow)
        {
          color = cell->selcolor;
        }
        /* draw th item that it can be seen */
        _TexuListCtrlProc_DrawItem(texu_wnd_get_cio(wnd), &rccell, 
          cell->caption, color, header->align, 0);
      }
    }
  }
}

void _TexuListCtrlProc_OnSetEditStyle(texu_wnd* wnd, texu_i32 col, texu_ui32 editstyle)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* header = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
  if (header)
  {
    header->editstyle = editstyle;
  }
}

texu_ui32 _TexuListCtrlProc_OnGetEditStyle(texu_wnd* wnd, texu_i32 col)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* header = 0;
  texu_ui32 editstyle = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
  if (header)
  {
    editstyle = header->editstyle;
  }
  return editstyle;
}

void _TexuListCtrlProc_OnSetCurRow(texu_wnd* wnd, texu_i32 idx)
{
  texu_lcwnd* lctl = 0;
  texu_rect rcwnd;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  if (idx < 0 || idx >= lctl->nitems)
  {
    if (lctl->nitems <= 0)
    {
      lctl->curselrow = -1;
    }
    else
    {
      lctl->curselrow = lctl->nitems-1;
    }
    texu_wnd_invalidate(wnd);
    return;
  }
  
  /* find the new appropriate first visible item */
  texu_wnd_get_rect(wnd, &rcwnd);
  if (idx >= lctl->firstvisiblerow &&
      idx <  lctl->firstvisiblerow + rcwnd.lines)
  {
    /* no moved first visible item */
  }
  else
  {
    if (idx > lctl->nitems - rcwnd.lines)
    {
      lctl->firstvisiblerow = lctl->nitems - rcwnd.lines;
    }
    else
    {      
      lctl->firstvisiblerow = idx;
    }
  }
  /* set current selected row */
  lctl->curselrow = idx;
  
  /* re-fresh window */
  texu_wnd_invalidate(wnd);
}

texu_i32 _TexuListCtrlProc_OnGetCurRow(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  
  return lctl->curselrow;
}

void _TexuListCtrlProc_OnSetColWidth(texu_wnd* wnd, texu_i32 col, texu_i32 width)
{
  texu_lcwnd* lctl = 0;
  texu_lcwnd_header* header = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
  if (header)
  {
    header->cols = width;
  }
}

texu_wnd* _TexuListCtrlProc_OnGetEditBox(texu_wnd* wnd)
{
  texu_lcwnd* lctl = 0;
  
  lctl = (texu_lcwnd*)texu_wnd_get_userdata(wnd);
  return lctl->editbox;
}

void _TexuListCtrlProc_OnSetCurPage(texu_wnd* wnd, texu_i32 npage)
{
  texu_i32 idx = 0;
  texu_i32 nitemspp = _TexuListCtrlProc_OnGetItemsPerPage(wnd);
  
  idx = (npage * nitemspp);
  texu_wnd_send_msg(wnd, TEXU_LCM_SETCURROW, (texu_i64)idx, 0);
}

texu_i32 _TexuListCtrlProc_OnGetCurPage(texu_wnd* wnd)
{
  texu_i32 curselrow = _TexuListCtrlProc_OnGetCurRow(wnd);
  texu_i32 nitems = _TexuListCtrlProc_OnGetItemCount(wnd);
  texu_i32 nitemspp = _TexuListCtrlProc_OnGetItemsPerPage(wnd);
  texu_i32 npage = -1;
  
  /* items is greater than zero */
  if (nitems > 0 && nitemspp > 0)
  {
    /* has selected row yet? */
    if (curselrow < 0)
    {
      curselrow = 0;
    }
    /* find the current page */
    npage = (curselrow / nitemspp);
  }
  return npage;
}

texu_i32 _TexuListCtrlProc_OnGetItemsPerPage(texu_wnd* wnd)
{
  texu_rect rcwnd;
  texu_wnd_get_rect(wnd, &rcwnd);
  return (rcwnd.lines > 1 ? rcwnd.lines - 1 : 0);
}

texu_i64
TexuListCtrlProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CHAR:
      _TexuListCtrlProc_OnChar(wnd, (texu_i32)param1);  
      return 0;
      
    case TEXU_WM_CREATE:
      return _TexuListCtrlProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_PAINT:
      _TexuListCtrlProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
      
    case TEXU_WM_DESTROY:
      _TexuListCtrlProc_OnDestroy(wnd);
      break;
      
    case TEXU_WM_SETFOCUS:
      _TexuListCtrlProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;

    case TEXU_WM_KILLFOCUS:
      return _TexuListCtrlProc_OnKillFocus(wnd, (texu_wnd*)param1);

    
    case TEXU_LCM_ADDCOLUMN:
    {
      return _TexuListCtrlProc_OnAddColumn(wnd, (texu_wnd_header*)param1);
    }
    case TEXU_LCM_DELETECOLUMN:
    {
      _TexuListCtrlProc_OnDeleteColumn(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_LCM_DELETEALLCOLUMNS:
    {
      _TexuListCtrlProc_OnDeleteAllItems(wnd);
      return 0;
    }
    case TEXU_LCM_ADDITEM:
    {
      return _TexuListCtrlProc_OnAddItem(wnd, (texu_char*)param1, (texu_i32)param2);
    }
    case TEXU_LCM_DELETEITEM:
    {
      _TexuListCtrlProc_OnDeleteItem(wnd, (texu_i32)param2);
      return 0;
    }
    case TEXU_LCM_DELETEALLITEMS:
    {
      _TexuListCtrlProc_OnDeleteAllItems(wnd);
      return 0;
    }
    case TEXU_LCM_SETITEM:
    {
      return _TexuListCtrlProc_OnSetItem(wnd, (texu_ui32)param1, (texu_wnd_subitem*)param2);
    }
    case TEXU_LCM_GETITEM:
    {
      return _TexuListCtrlProc_OnGetItem(wnd, (texu_ui32)param1, (texu_wnd_subitem*)param2);
    }
    case TEXU_LCM_GETITEMCOUNT:
    {
      return _TexuListCtrlProc_OnGetItemCount(wnd);
    }
    case TEXU_LCM_INVALIDATEITEM:
    {
      _TexuListCtrlProc_OnInvalidateItem(wnd, (texu_ui32)param1, (texu_ui32)param2);
      return 0;
    }
    case TEXU_LCM_SETEDITSTYLE:
    {
      _TexuListCtrlProc_OnSetEditStyle(wnd, (texu_i32)param1, (texu_ui32)param2);
      return 0;
    }
    case TEXU_LCM_GETEDITSTYLE:
    {
      return _TexuListCtrlProc_OnGetEditStyle(wnd, (texu_i32)param1);
    }
    case TEXU_LCM_GETCURROW:
    {
      return _TexuListCtrlProc_OnGetCurRow(wnd);
    }
    case TEXU_LCM_SETCURROW:
    {
      _TexuListCtrlProc_OnSetCurRow(wnd, (texu_i32)param2);
      return 0;
    }
    case TEXU_LCM_SETCOLWIDTH:
    {
      _TexuListCtrlProc_OnSetColWidth(wnd, (texu_i32)param1, (texu_i32)param2);
      return 0;
    }
    case TEXU_LCM_GETEDITBOX:
    {
      return (texu_i32)_TexuListCtrlProc_OnGetEditBox(wnd);
    }
    case TEXU_LCM_SETCURPAGE:
    {
      _TexuListCtrlProc_OnSetCurPage(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_LCM_GETCURPAGE:
    {
      return _TexuListCtrlProc_OnGetCurPage(wnd);
    }
    case TEXU_LCM_GETITEMSPERPAGE:
    {
      return _TexuListCtrlProc_OnGetItemsPerPage(wnd);
    }
    case TEXU_LCM_SETEDITABLECOLS:
    {
      _TexuListCtrlProc_OnSetEditableCols(wnd, (texu_i32)param1, (texu_i32*)param2);
      break;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}



#ifdef __cplusplus
}
#endif


