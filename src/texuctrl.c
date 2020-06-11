/*
#
# File Name: texuctrl.c
# Author: Seree Rakwong
# Date: 27-MAY-2020
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


void                _TexuWndProc_Notify(texu_wnd*, texu_ui32);

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

void                  _TexuListCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt);
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
  texu_env* env = texu_wnd_get_env(wnd);
  
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
  attrs2.normalcolor    = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL);
  attrs2.disabledcolor  = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_DISABLED);
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
    texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL), texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_DISABLED));
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
  texu_env* env = texu_wnd_get_env(wnd);

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
      newcell->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM);
      newcell->discolor  = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM_DISABLED);
      newcell->selcolor  = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM_SELECTED);
      
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
  texu_i32 cols = rccell->cols;
  texu_i32 x = rccell->x;
  texu_i32 y = rccell->y;
  
  memset(buf, 0, sizeof(buf));
  memset(buf, ' ', cols);
  texu_cio_putstr_attr(dc,
      y,
      x,
      buf,
      COLOR_PAIR(color));
  
  cols = (isheader ? cols : cols - 2);
  len = texu_printf_alignment(buf, 
          caption,
          cols,
          align);
  /* is is a header */
  if (isheader)
  {
    buf[0] = '[';
    buf[len - 1] = ']';
  }
  else
  {
    x += 1;
  }

  texu_cio_putstr_attr(dc,
      y,
      x,
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
  texu_i32 normcolor = 0;
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

    normcolor =    header->normcolor;
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
  texu_wnd_send_msg(lctl->editbox, TEXU_WM_KILLFOCUS, 0, 0);
  
  texu_wnd_move(lctl->editbox, rccell.y, rccell.x, rccell.lines, rccell.cols, TEXU_TRUE);

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
  texu_wnd_send_msg(lctl->editbox, TEXU_WM_SETFOCUS, 0, 0);
  
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



void _TexuListCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt)
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
_TexuListCtrlProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CHAR:
      _TexuListCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);  
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


/*-------------------------------------------------------------------
 * TREECTRL functions
 *-----------------------------------------------------------------*/
struct texu_treewnd
{
  texu_tree*            tree; /* see also texulib.h */
  texu_tree_find_proc   findproc; /* to compare item */
  texu_tree_item*       firstvisibleitem; /* the first item is visible in the tree control */
  texu_tree_item*       lastvisibleitem; /* the first item is visible in the tree control */
  texu_tree_item*       curselitem; /* the current selected item */
  texu_list*            visibleitems;
  texu_i32              indent;
  texu_i32              shifted_right;
  texu_i32              shifted;
  texu_i32              normcolor;
  texu_i32              discolor;
  texu_i32              selcolor;
  void*                 userdata_findproc;
};
typedef struct texu_treewnd texu_treewnd;

void                _TexuTreeProc_Notify(texu_wnd* wnd, texu_ui32, texu_tree_item*);


void                _TexuTreeCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc);
texu_tree_item*     _TexuTreeCtrlProc_OnInsertItem(texu_wnd* wnd, texu_tree_item* parentitem, texu_treewnd_item* insertitem);
texu_i32            _TexuTreeCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs*);
texu_i32            _TexuTreeCtrlProc_OnDeleteItem(texu_wnd* wnd, texu_tree_item* item);
texu_i32            _TexuTreeCtrlProc_OnSetItem(texu_wnd* wnd, texu_tree_item* item, texu_treewnd_item* setitem);
texu_i32            _TexuTreeCtrlProc_OnGetItem(texu_wnd* wnd, texu_tree_item* item, texu_treewnd_item* getitem);
texu_tree_item*     _TexuTreeCtrlProc_OnFindItem(texu_wnd* wnd, texu_treewnd_item* finditem, void* userdata);
texu_tree_item*     _TexuTreeCtrlProc_OnFindNextItem(texu_wnd* wnd, texu_tree_item* previtem, texu_treewnd_item* finditem);
texu_i32            _TexuTreeCtrlProc_OnExpandItem(texu_wnd* wnd, texu_tree_item* item);
texu_i32            _TexuTreeCtrlProc_OnCollapseItem(texu_wnd* wnd, texu_tree_item* item);
void                _TexuTreeCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt);
void                _TexuTreeCtrlProc_OnDestroy(texu_wnd* wnd);
void                _TexuTreeCtrlProc_OnSetFocus(texu_wnd* wnd, texu_wnd* );
texu_i32            _TexuTreeCtrlProc_OnKillFocus(texu_wnd* wnd, texu_wnd* );
texu_tree_find_proc _TexuTreeCtrlProc_OnSetFindItemProc(texu_wnd* wnd, texu_tree_find_proc findproc);
texu_i32            _TexuTreeCtrlProc_OnSetSelItem(texu_wnd* wnd, texu_tree_item* setitem);
texu_tree_item*     _TexuTreeCtrlProc_OnGetSelItem(texu_wnd* wnd);
texu_i32            _TexuTreeCtrlProc_OnExportToFile(texu_wnd* wnd, FILE* fp, texu_tree_exp_proc prnproc);
texu_i32            _TexuTreeCtrlProc_OnImportFromFile(texu_wnd* wnd, FILE* fp, texu_tree_imp_proc impproc);
void                _TexuTreeCtrlProc_OnExpandAllItems(texu_wnd* wnd);
void                _TexuTreeCtrlProc_OnCollapseAllItems(texu_wnd* wnd);
texu_i32            _TexuTreeCtrlProc_OnGetIndentText(texu_wnd* wnd);
void                _TexuTreeCtrlProc_OnSetIndentText(texu_wnd* wnd, texu_i32 indent);
texu_i32            _TexuTreeCtrlProc_OnGetShiftedText(texu_wnd* wnd);
void                _TexuTreeCtrlProc_OnSetShiftedText(texu_wnd* wnd, texu_i32 shifted);

void                _TexuTreeCtrlProc_RefreshView(texu_wnd* wnd);
/*typedef texu_i32         (*fn_tree_compare_proc)(const void*, const void*);*/
/*
texu_i32            _TexuTreeCtrlProc_DefFindItemProc(const void* datap, const void* itemp);
*/
texu_i32            _TexuTreeCtrlProc_DefFindItemProc(texu_i64, texu_i64, void*);

void                _TexuTreeCtrlProc_GetDisplayedText(
                            texu_wnd* wnd,
                            texu_char* outtext, texu_i32* startx, texu_i32* endx,
                            texu_tree_item* item,
                            texu_i32 maxlen,
                            texu_bool file);

void                _TexuTreeCtrlProc_AdjustVisibleItems(texu_wnd* wnd);
texu_tree_item*     _TexuTreeCtrlProc_MoveNext(texu_wnd* wnd, texu_i32 times);
texu_tree_item*     _TexuTreeCtrlProc_MovePrev(texu_wnd* wnd, texu_i32 times);

struct texu_treeview_item
{
    texu_tree_item* item;
};
typedef struct texu_treeview_item texu_treeview_item;


texu_i64            _TexuTreeCtrlProc_PreorderTraverseProc(texu_tree_item*, void*);
texu_i64            _TexuTreeCtrlProc_ExpandAllItemsProc(texu_tree_item*, void*);
texu_i64            _TexuTreeCtrlProc_CollapseAllItemsProc(texu_tree_item*, void*);

/* helper functions */
texu_treeview_item* _texu_treeview_item_new(texu_tree_item* item);
void                _texu_treeview_item_del(texu_i64 data, void* userdata);
texu_treewnd_item*  _texu_treewnd_item_new(texu_treewnd_item* item);
void                _texu_treewnd_item_del(texu_i64 data, void* userdata);


void
_TexuTreeProc_Notify(texu_wnd* wnd, texu_ui32 code, texu_tree_item* item)
{
  texu_treewnd_notify notify;
  texu_wnd* parent = texu_wnd_get_parent(wnd);
  
  notify.hdr.wnd  = wnd;
  notify.hdr.id   = texu_wnd_get_id(wnd);
  notify.hdr.code = code;
  notify.item     = item;
  texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
}


texu_i32
_TexuTreeCtrlProc_DefFindItemProc(texu_i64 d1, texu_i64 d2, void* userdata)
/*texu_i32 _TexuTreeCtrlProc_DefFindItemProc(const void* datap, const void* itemp)*/
{
  /* this function compare the item text */
  texu_treewnd_item* dataitem = (texu_treewnd_item*) d1;
  texu_treewnd_item* treeitem = (texu_treewnd_item*) d2;
  return memcmp(dataitem->itemtext, treeitem->itemtext, TEXU_MAX_WNDTEXT);
}

texu_treeview_item*
_texu_treeview_item_new(texu_tree_item* item)
{
  texu_treeview_item* view = (texu_treeview_item*)malloc(sizeof(texu_treeview_item));
  if (view)
  {
    view->item = item;
  }
  return view;
}

void _texu_treeview_item_del(texu_i64 data, void* userdata)
{
  texu_treeview_item* view = (texu_treeview_item*)data;
  free(view);
}

texu_i32 _TexuTreeCtrlProc_OnImportFromFile(texu_wnd* wnd, FILE* fp, texu_tree_imp_proc proc)
{
  texu_char buf[BUFSIZ + 1];
  texu_treewnd* tc = 0;
  texu_treewnd_item* data = 0;
  texu_treeview_item *view = 0;
  texu_stack* stack = 0;
  texu_tree_item* parent = 0;
  texu_i32 tabs = 0;
  texu_char* psz = 0;
  texu_i32 items = 0;
  texu_tree_item* newitem = 0;
  texu_i32 len = 0;

  if (!fp)
  {
    /* nothing to do */
    return -1;
  }
  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  parent = texu_tree_get_root(tc->tree);

  /**/
  stack = texu_stack_new(512);
  view = _texu_treeview_item_new(parent);
  texu_stack_push(stack, (texu_i64)view);

  while (!feof(fp))
  {
    if (!fgets(buf, BUFSIZ, fp))
    {
      break;
    }
    /* comment */
    if (buf[0] == 0 || buf[0] == '#' || buf[0] == '\n')
    {
      continue;
    }
    /* look up the parent item */
    tabs = 0;
    psz = buf;
    while (psz)
    {
      if (*psz == '\t')
      {
        ++tabs;
        ++psz;
      }
      else
      {
        break;
      }
    }
    /* tabs are required */
    if (0 == tabs)
    {
        continue;
    }
    len = strlen(psz);
    if (len > 0 && psz[len - 1] == '\n')
    {
        psz[len - 1] = 0;
    }
    /* insert the new item */
    items = texu_stack_count(stack);
    if (tabs > items)
    {
      /* insert to the next parent */
      if (tabs > items + 1)
      {
          /* invalid item */
          continue;
      }
      
      view = (texu_treeview_item*)texu_stack_top(stack);
      parent = view->item;
    }
    else if (tabs < items)
    {
      /* pop the stack */
      while (items >= tabs)
      {
        texu_stack_cb_pop(
             stack,
             _texu_treeview_item_del,
             0);
        items = texu_stack_count(stack);
      }
      if (texu_stack_empty(stack))
      {
        parent = 0;
      }
      else
      {
        view = (texu_treeview_item*)texu_stack_top(stack);
        parent = view->item;
      }
    }
    else
    {
      /* same level */
      texu_stack_cb_pop(
               stack,
               _texu_treeview_item_del,
               0);
      if (texu_stack_empty(stack))
      {
        parent = 0;
      }
      else
      {
        view = (texu_treeview_item*)texu_stack_top(stack);
        parent = view->item;
      }
    }
    /* format text or data */
    memset(&data, 0, sizeof (data));
    if (proc)
    {
      if (proc(data, psz) != 0)
      {
        /* sometimes input may not required by user */
        continue;
      }
    }
    else
    {
      /* no procedure introduced */
      strcpy(data->itemtext, psz);
    }

    newitem = _TexuTreeCtrlProc_OnInsertItem(wnd, parent, data);

    view = _texu_treeview_item_new(newitem);
      texu_stack_push(stack, (texu_i64)view);
  }
  texu_stack_cb_free(
    stack,
    _texu_treeview_item_del,
    0);
  texu_stack_del(stack);
  return 0;
}

texu_i32 _TexuTreeCtrlProc_OnExportToFile(texu_wnd* wnd, FILE* fp, texu_tree_exp_proc prnproc)
{
    texu_queue* queue = 0;
    texu_char buf[BUFSIZ + 1];
    texu_treewnd* tc = 0;
    texu_treewnd_item* data = 0;
    texu_treeview_item* view = 0;
    texu_tree_item* root = 0;
    texu_i32 startx = 0, endx = 0;

    if (!fp)
    {
        /* nothing to do */
        return -1;
    }
    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

    /* visible window & there is a visible item */
    if (!tc->firstvisibleitem)
    {
        return 0;
    }
    /* populate all items */
    queue = texu_queue_new();
    root = texu_tree_get_root(tc->tree);
    
    texu_tree_populate(
                    tc->tree,
                    root->firstchild,
                    TEXU_TPO_PRE,
                    _TexuTreeCtrlProc_PreorderTraverseProc,
                    queue);
                    

    /* print all items to file */
    while (!texu_queue_empty(queue))
    {
        
        view = (texu_treeview_item*)texu_queue_first(queue);
        
        texu_queue_dequeue(queue);

        data = (texu_treewnd_item*)view->item->data;

        memset(buf, 0, sizeof (buf));
        _TexuTreeCtrlProc_GetDisplayedText(wnd, buf, &startx, &endx, view->item, BUFSIZ - 1, TEXU_TRUE);

        if (prnproc)
        {
            if (prnproc(fp, data) != 0)
            {
              free(view);
                return 1;
            }
        }
        else
        {
            fprintf(fp, "%s\n", buf);
        }
        free(view);
    }
    /* release memory */
    texu_queue_cb_free(queue, _texu_treeview_item_del, 0);
    
    texu_queue_del(queue);
    return 0;
}

void _TexuTreeCtrlProc_AdjustVisibleItems(texu_wnd* wnd)
{
    texu_treewnd* tc = 0;
    texu_treewnd_item* data = 0;
    texu_treeview_item *view = 0;
    texu_tree_item* root = 0;
    texu_tree_item* next_item = 0;
    texu_queue* queue;
    texu_queue* subqueue;
    texu_list* items = 0;

    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

    /* visible window & there is a visible item */
    if (!tc->firstvisibleitem)
    {
        return;
    }

    items = texu_list_new();

    queue = texu_queue_new();
    root = texu_tree_get_root(tc->tree);
    texu_tree_populate(
                    tc->tree,
                    root->firstchild,
                    TEXU_TPO_PRE,
                    _TexuTreeCtrlProc_PreorderTraverseProc,
                    queue);
                    

    while (!texu_queue_empty(queue))
    {
        
        view = (texu_treeview_item*)texu_queue_first(queue);
        
        texu_queue_dequeue(queue);

        
        texu_list_add(items, (texu_i64)view);

        /* selected item */
        data = (texu_treewnd_item*)view->item->data;

        /* has data but not expanded */
        if (data->children > 0 && !data->expanded)
        {
            next_item = view->item->next;

            /* skip its children */
            subqueue = texu_queue_new();
            
            texu_tree_populate(
                    tc->tree,
                    view->item,
                    TEXU_TPO_PRE,
                    _TexuTreeCtrlProc_PreorderTraverseProc,
                    subqueue);
            
            texu_queue_cb_dequeue(subqueue, _texu_treeview_item_del, 0);
            
            while (!texu_queue_empty(subqueue))
            {
                view = (texu_treeview_item*)texu_queue_first(subqueue);
                texu_queue_dequeue(subqueue);
                
                if (view->item == next_item)
                {
                    break;
                }
                texu_queue_cb_dequeue(queue, _texu_treeview_item_del, 0);
            }
            texu_queue_cb_free(subqueue, _texu_treeview_item_del, 0);
            
            texu_queue_del(subqueue);
        }
    }
    texu_queue_cb_free(queue, _texu_treeview_item_del, 0);
    texu_queue_del(queue);
    

    /* assign to the visible items */
    if (texu_list_count(items) > 0)
    {
        if (tc->visibleitems)
        {
            
            texu_list_cb_free(tc->visibleitems, _texu_treeview_item_del, 0);
            texu_list_del(tc->visibleitems);
        }
        tc->visibleitems = items;
    }
    else
    {
        
        texu_list_cb_free(items, _texu_treeview_item_del, 0);
        texu_list_del(items);
    }
}

void _TexuTreeCtrlProc_RefreshView(texu_wnd* wnd)
{
    /* refresh update if the new item or deleting item is in the visible bound */
    texu_wnd_invalidate(wnd);
}

texu_tree_item* _TexuTreeCtrlProc_MoveNext(texu_wnd* wnd, texu_i32 move_times)
{
    texu_treewnd* tc = 0;
    texu_treewnd_item* data = 0;
    texu_list_item* iter = 0;
    texu_treeview_item* view = 0;
    texu_i32 i = 0;
    texu_i32 items = 0;
    texu_i32 firstindex = 0;
    texu_i32 curindex = 0;
    texu_list_item* first_iter = 0;
    texu_list_item* cur_iter = 0;
    texu_rect rc;
    texu_i32 first_moves = 0;
    texu_i32 cur_moves = 0;
    texu_bool movefirst = TEXU_TRUE;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
    items = texu_list_count(tc->visibleitems);

    /* change selection item */
    iter = texu_list_first(tc->visibleitems);
    /* find the first index */
    while (iter)
    {
        view = (texu_treeview_item*)iter->data;
        data = (texu_treewnd_item*)view->item->data;
        
        if (view->item == tc->firstvisibleitem)
        {
            first_iter = iter;
            break;
        }
        iter = iter->next;
        ++firstindex;
    }
    /* find the current selection index */
    curindex = firstindex;
    while (iter)
    {
        view = (texu_treeview_item*)iter->data;
        data = (texu_treewnd_item*)view->item->data;
        if (view->item == tc->curselitem)
        {
            cur_iter = iter;
            break;
        }
        iter = iter->next;
        ++curindex;
    }

    if (curindex == items - 1)
    {
        /* no move required the last item */
        return tc->curselitem;
    }

    /* un-highlighting the old item */
    data = (texu_treewnd_item*)tc->curselitem->data;
    data->selected = 0;

    /* highlighting the new item */
    iter = cur_iter;
    cur_moves = TEXU_MAX(0, TEXU_MIN(move_times, items - curindex - 1));
    for (i = 0; i < cur_moves && iter; ++i)
    {
        iter = iter->next;
    }
    view = (texu_treeview_item*)iter->data;
    data = (texu_treewnd_item*)view->item->data;
    
    tc->curselitem = view->item;
    /*tc->tree->GetItemData(tc->curselitem, &data, sizeof (data));*/
    data->selected = 1;

    /* is the new index out-of-bound visible items? */
    if ((curindex + cur_moves) < (firstindex + rc.lines))
    {
        /* no move first */
        movefirst = TEXU_FALSE;
    }
    if ((curindex + cur_moves) >= items)
    {
        movefirst = TEXU_FALSE;
    }

    if (movefirst)
    {
        /* how actual many movements */
        first_moves = TEXU_MIN(cur_moves, items - move_times);
        iter = first_iter;
        for (i = 0; i < first_moves && iter; ++i)
        {
            iter = iter->next;
        }
        
        view = (texu_treeview_item*)iter->data;
        tc->firstvisibleitem = view->item;
    }
    return tc->curselitem;
}

texu_tree_item* _TexuTreeCtrlProc_MovePrev(texu_wnd* wnd, texu_i32 move_times)
{
    texu_treewnd* tc = 0;
    texu_treewnd_item* data = 0;
    texu_list_item* iter = 0;
    texu_treeview_item* view = 0;
    texu_i32 i = 0;
    texu_i32 firstvisibleindex = 0;
    texu_i32 curindex = 0;
    texu_list_item* first_iter = 0;
    texu_list_item* cur_iter = 0;
    texu_rect rc;
    texu_i32 first_moves = 0;
    texu_i32 cur_moves = 0;
    texu_bool movefirst = TEXU_FALSE;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

    /* change selection item */
    /* find the current selection index */
    firstvisibleindex = 0;
    iter = texu_list_first(tc->visibleitems);
    while (iter)
    {
        view = (texu_treeview_item*)iter->data;
        data = (texu_treewnd_item*)view->item->data;
        
        if (view->item == tc->firstvisibleitem)
        {
            first_iter = iter;
            break;
        }
        iter = iter->next;
        ++firstvisibleindex;
    }
    if (0 == first_iter)
    {
        return tc->curselitem;
    }

    /* find the current index */
    cur_iter = first_iter;
    curindex = firstvisibleindex;
    while (iter)
    {
        view = (texu_treeview_item*)iter->data;
        data = (texu_treewnd_item*)view->item->data;

        if (view->item == tc->curselitem)
        {
            cur_iter = iter;
            break;
        }
        iter = iter->next;
        ++curindex;
    }

    /* un-highlighting the old item */
    
    data = (texu_treewnd_item*)tc->curselitem->data;
    data->selected = TEXU_FALSE;
    

    /* highlighting the new item */
    iter = cur_iter;
    cur_moves = 0;
    for (i = 0; i < move_times && iter != 0; ++i, ++cur_moves)
    {
        iter = iter->prev;
    }

    if (firstvisibleindex > 0 && curindex - cur_moves < firstvisibleindex)
    {
        movefirst = TEXU_TRUE;
    }
    else if (0 == firstvisibleindex && cur_moves >= curindex)
    {
        iter = texu_list_first(tc->visibleitems);
    }

    view = (texu_treeview_item*)iter->data;
    data = (texu_treewnd_item*)view->item->data;
    
    
    tc->curselitem = view->item;
    /*tc->tree->GetItemData(tc->curselitem, &data, sizeof (data));*/
    data->selected = TEXU_TRUE;
    

    if (movefirst)
    {
        /* how actual many movements */
        first_moves = TEXU_MIN(cur_moves, move_times);
        iter = first_iter;
        for (i = 0; i < first_moves && iter; ++i)
        {
            iter = iter->prev;
        }
        
        view = (texu_treeview_item*)iter->data;
        tc->firstvisibleitem = view->item;
    }
    return tc->curselitem;
}

void _TexuTreeCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt)
{
    texu_treewnd* tc = 0;
    texu_rect rc;
    texu_treewnd_item* data = 0;
    texu_tree_item* selitem = 0;

    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
    if (0 == tc->visibleitems)
    {
        /* no item */
        return;
    }
    texu_wnd_get_rect(wnd, &rc);

    switch (ch)
    {
    case 0x20:/*SPACE*/
    {
        /* expand or collapse the current selection item */
        
        data = (texu_treewnd_item*)tc->curselitem->data;
        if (data->children > 0)
        {
            /* toggle item */
            if (data->expanded)
            {
                _TexuTreeCtrlProc_OnCollapseItem(wnd, tc->curselitem);
            }
            else
            {
                _TexuTreeCtrlProc_OnExpandItem(wnd, tc->curselitem);
            }
        }
        break;
    }

    case KEY_UP:
    {
        /* check if the last visible item */
        selitem = _TexuTreeCtrlProc_MovePrev(wnd, 1);
        /* notify select item */
        _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
        break;
    }

    case KEY_DOWN:
    {
        selitem = _TexuTreeCtrlProc_MoveNext(wnd, 1);
        _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
        break;
    }

    case KEY_NPAGE:
    {
        selitem = _TexuTreeCtrlProc_MoveNext(wnd, rc.lines);
        _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
        break;
    }

        /* move up 1 page */
    case KEY_PPAGE:
    {
        selitem = _TexuTreeCtrlProc_MovePrev(wnd, rc.lines);
        _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
        break;
    }

    case KEY_RIGHT:
    {
        tc->shifted_right = TEXU_MIN(rc.cols, tc->shifted_right + tc->shifted);
        /* update item on the screen */
        _TexuTreeCtrlProc_RefreshView(wnd);
        break;
    }

    case KEY_LEFT:
    {
        tc->shifted_right = TEXU_MAX(0, tc->shifted_right - tc->shifted);
        /* update item on the screen */
        _TexuTreeCtrlProc_RefreshView(wnd);
        break;
    }

    default:
    {
        break;
    }
    }
}

texu_tree_find_proc _TexuTreeCtrlProc_OnSetFindItemProc(texu_wnd* wnd, texu_tree_find_proc findproc)
{
    texu_tree_find_proc oldproc = 0;
    texu_treewnd* tc = 0;

    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
    oldproc = tc->findproc;
    tc->findproc = findproc;
    return oldproc;
}

void _TexuTreeCtrlProc_DestroyTreeProc(texu_i64 data, void* userdata)
{
  texu_treewnd_item* item = (texu_treewnd_item*)data;
  free(item);
}

void _TexuTreeCtrlProc_OnDestroy(texu_wnd* wnd)
{
    texu_treewnd* tc = 0;
    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
    texu_list_cb_free(tc->visibleitems, _texu_treeview_item_del, 0);
    texu_list_del(tc->visibleitems);
    
    
    texu_tree_cb_remove_item(tc->tree, 
      texu_tree_get_root(tc->tree), _TexuTreeCtrlProc_DestroyTreeProc, 0);
    texu_tree_del(tc->tree);
    free(tc);
}

void _TexuTreeCtrlProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
    _TexuWndProc_Notify(wnd, TEXU_TCN_SETFOCUS);
}

texu_i32 _TexuTreeCtrlProc_OnKillFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
    texu_i32 rc = TEXU_OK;
    
    _TexuWndProc_Notify(wnd, TEXU_TCN_KILLFOCUS);

    return rc;
}

texu_tree_item* _TexuTreeCtrlProc_OnInsertItem(
  texu_wnd* wnd,
  texu_tree_item* parentitem,
  texu_treewnd_item* insertitem)
{
    texu_treewnd* tc = 0;
    texu_i32 children = 0;
    texu_tree_item* newitem = 0;
    texu_treewnd_item* parentdata = 0;
    texu_treewnd_item* data = 0;

    if (0 == insertitem)
    {
        return 0;
    }

    tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
    /* get a number of children before inserting the new one */
    children = texu_tree_count(tc->tree);

    /* insert the new item */
    data = _texu_treewnd_item_new(insertitem);
    if (0 == children)
    {
        data->selected = 1;
    }
    
    newitem = texu_tree_add_item(tc->tree, parentitem, (texu_i64)data);

    if (newitem)
    {
        /* set the first visible item if need */
        if (0 == children)
        {
            tc->firstvisibleitem = tc->curselitem = (texu_tree_item*) newitem;
        }
        /* update parent */
        if (parentitem)
        {
          parentdata = (texu_treewnd_item*)parentitem->data;
            ++parentdata->children;
            
        }
        /* update item on the screen */
        /*_TexuTreeCtrlProc_RefreshView(wnd);*/
    }

    return (texu_tree_item*) newitem;
}

texu_treewnd_item*
_texu_treewnd_item_new(texu_treewnd_item* item)
{
  texu_treewnd_item* data = (texu_treewnd_item*)malloc(sizeof(texu_treewnd_item));
  if (data)
  {
    memset(data, 0, sizeof(texu_treewnd_item));
    memcpy(data, item, sizeof(texu_treewnd_item));
  }
  return data;
}

void
_texu_treewnd_item_del(texu_i64 data, void* userdata)
{
  texu_treewnd_item* item = (texu_treewnd_item*)data;
  free(item);
}

texu_i32 _TexuTreeCtrlProc_OnDeleteItem(texu_wnd* wnd, texu_tree_item* item)
{
  texu_treewnd* tc = 0;
  texu_i32 children = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

  texu_tree_cb_remove_item(
      tc->tree,
      item,
      _texu_treewnd_item_del,
      0);

  children = texu_tree_count(tc->tree);
  /* set non-visible if need */
  if (0 == children)
  {
      tc->firstvisibleitem = tc->lastvisibleitem = tc->curselitem = 0;
  }
  /* update item on the screen */
  _TexuTreeCtrlProc_RefreshView(wnd);

  return 0;
}

texu_i32 _TexuTreeCtrlProc_OnExpandItem(texu_wnd* wnd, texu_tree_item* item)
{
  texu_treewnd_item* data = 0;

  _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMEXPANDING, item);

  /* mark expanded */
  data = (texu_treewnd_item*)item->data;
  
  if (data->children > 0)
  {
      data->expanded = TEXU_TRUE;
  }
  

  _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMEXPANDED, item);

  /* update item on the screen */
  _TexuTreeCtrlProc_RefreshView(wnd);
  return 0;
}

texu_i32 _TexuTreeCtrlProc_OnGetIndentText(texu_wnd* wnd)
{
  texu_treewnd* tc = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  return tc->indent;
}

void _TexuTreeCtrlProc_OnSetIndentText(texu_wnd* wnd, texu_i32 indent)
{
  texu_treewnd* tc = 0;
  texu_rect rc;

  texu_wnd_get_rect(wnd, &rc);
  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  if (indent < 2)
  {
      indent = 2;
  }
  else if (indent >= rc.cols)
  {
      indent = rc.cols - 1;
  }
  tc->indent = indent;
}

texu_i32 _TexuTreeCtrlProc_OnGetShiftedText(texu_wnd* wnd)
{
  texu_treewnd* tc = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  return tc->shifted;
}

void _TexuTreeCtrlProc_OnSetShiftedText(texu_wnd* wnd, texu_i32 shifted)
{
  texu_treewnd* tc = 0;
  texu_rect rc;

  texu_wnd_get_rect(wnd, &rc);
  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  if (shifted < 1)
  {
      shifted = 1;
  }
  else if (shifted >= rc.cols)
  {
      shifted = rc.cols - 1;
  }
  tc->shifted = shifted;
}

void _TexuTreeCtrlProc_OnExpandAllItems(texu_wnd* wnd)
{
  texu_treewnd* tc = 0;
  texu_tree_item* root = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  root = texu_tree_get_root(tc->tree);
  
  texu_tree_populate(
                  tc->tree,
                  root->firstchild,
                  TEXU_TPO_PRE,
                  _TexuTreeCtrlProc_ExpandAllItemsProc,
                  wnd);


  _TexuTreeCtrlProc_OnExpandItem(wnd, root);
}

void _TexuTreeCtrlProc_OnCollapseAllItems(texu_wnd* wnd)
{
  texu_treewnd* tc = 0;
  texu_tree_item* root = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  root = texu_tree_get_root(tc->tree);
  
  texu_tree_populate(
                  tc->tree,
                  root->firstchild,
                  TEXU_TPO_PRE,
                  _TexuTreeCtrlProc_CollapseAllItemsProc,
                  wnd);


  _TexuTreeCtrlProc_OnCollapseItem(wnd, root);
}

texu_i32 _TexuTreeCtrlProc_OnCollapseItem(texu_wnd* wnd, texu_tree_item* item)
{
  texu_treewnd_item* data = 0;

  _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMCOLLAPSING, item);

  /* mark collapse */
  data = (texu_treewnd_item*)item->data;
  
  if (data->children > 0)
  {
      data->expanded = TEXU_FALSE;
  }


  _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMCOLLAPSED, item);

  /* update item on the screen */
  _TexuTreeCtrlProc_RefreshView(wnd);
  return 0;
}

texu_i32 _TexuTreeCtrlProc_OnSetSelItem(texu_wnd* wnd, texu_tree_item* setitem)
{
  texu_treewnd* tc = 0;
  texu_treewnd_item* data = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  /* remove the highlighting */
  if (tc->curselitem)
  {
    
      data = (texu_treewnd_item*)tc->curselitem;
      data->selected = TEXU_FALSE;
    
  }

  /* new selected item */
  tc->curselitem = setitem;

  /* set the highlighting */
  if (tc->curselitem)
  {
      
      data = (texu_treewnd_item*)tc->curselitem;
      data->selected = TEXU_TRUE;

  }
  /* update item on the screen */
  _TexuTreeCtrlProc_RefreshView(wnd);
  return 0;
}

texu_tree_item* _TexuTreeCtrlProc_OnGetSelItem(texu_wnd* wnd)
{
  texu_treewnd* tc = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  return tc->curselitem;
}

texu_i32 _TexuTreeCtrlProc_OnSetItem(texu_wnd* wnd, texu_tree_item* item, texu_treewnd_item* setitem)
{
  texu_treewnd_item* data = 0;

  if (!item)
  {
      return -1;
  }
  if (!setitem)
  {
      return -2;
  }

  if (setitem)
  {
    data = (texu_treewnd_item*)item->data;
    if (data)
    {
      _texu_treewnd_item_del((texu_i64)data, 0);
    }
    data = _texu_treewnd_item_new(setitem);
  }
  
  return 0;
}

texu_i32 _TexuTreeCtrlProc_OnGetItem(texu_wnd* wnd, texu_tree_item* item, texu_treewnd_item* getitem)
{
  if (!item)
  {
      return -1;
  }
  if (!getitem)
  {
      return -2;
  }

  getitem = (texu_treewnd_item*)item->data;
  return 0;
}

texu_tree_item*
_TexuTreeCtrlProc_OnFindItem(texu_wnd* wnd, texu_treewnd_item* finditem, void* userdata)
{
  texu_treewnd* tc = 0;
  texu_tree_item* founditem = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

  founditem = texu_tree_find_item(
                    tc->tree,
                    (texu_i64)finditem,
                    (tc->findproc ? tc->findproc : _TexuTreeCtrlProc_DefFindItemProc),
                    userdata);
                    
  tc->userdata_findproc = userdata;
  
  return (texu_tree_item*) founditem;
}

texu_tree_item*
_TexuTreeCtrlProc_OnFindNextItem(texu_wnd* wnd, texu_tree_item* previtem, texu_treewnd_item* finditem)
{
  texu_treewnd* tc = 0;
  texu_tree_item* founditem = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

  founditem = texu_tree_find_next_item(
                  tc->tree,
                  previtem,
                  (texu_i64)finditem,
                  (tc->findproc ? tc->findproc : _TexuTreeCtrlProc_DefFindItemProc),
                  tc->userdata_findproc);

  return (texu_tree_item*) founditem;
}

texu_i32 _TexuTreeCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_treewnd* tc = (texu_treewnd*) malloc(sizeof (texu_treewnd));
  texu_env* env = texu_wnd_get_env(wnd);
  if (!tc)
  {
      return TEXU_NOMEM;
  }
  memset(tc, 0, sizeof (texu_treewnd));
  tc->indent = 2;
  tc->shifted_right = 0;
  tc->shifted = 4;
  tc->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL);
  tc->discolor  = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_DISABLED);
  tc->selcolor  = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_SELECTED);

  tc->tree = texu_tree_new();
  if (!tc->tree)
  {
      free(tc);
      return TEXU_NOMEM;
  }
  /* set tree */
  texu_wnd_set_userdata(wnd, tc);
  texu_wnd_set_color(wnd, 
    texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL),
    texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_DISABLED));

  return TEXU_OK;
}

void _TexuTreeCtrlProc_GetDisplayedText(
  texu_wnd* wnd,
  texu_char* outtext, texu_i32* startx, texu_i32* endx,
  texu_tree_item* item,
  texu_i32 maxlen,
  texu_bool file)
{
  texu_i32 xpos = 0;
  texu_treewnd_item* data = 0;
  texu_treewnd* tc = 0;
  texu_ui32 children = 0;
  texu_char buf[BUFSIZ + 1];
  texu_i32 shifted_right = 0;
  texu_i32 textlen = 0;
  texu_ui32 style = texu_wnd_get_style(wnd);
  texu_i32 len = 0;

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);
  
  data = (texu_treewnd_item*)item->data;

  memset(buf, 0, sizeof (buf));
  
  xpos = item->nlevel;
  /* fill indents */
  if (xpos > 0)
  {
    if (file)
    {
        memset(buf, '\t', xpos);
    }
    else
    {
        xpos *= tc->indent;
        memset(buf, ' ', xpos);
    }
    *startx = xpos;
  }
  else
  {
      return;
  }
  /* extend 2 characters for node displayed */
  children = item->nchildren;

  /* print node */
  if ((TEXU_TCS_SHOWNODE & style) && children > 0 && !file)
  {
    if (data->expanded)
    {
        buf[xpos - tc->indent] = '-';
    }
    else
    {
        buf[xpos - tc->indent] = '+';
    }
  }
  /* print text */
  textlen = strlen(data->itemtext);
  if (textlen > 0)
  {
      memcpy(&buf[xpos], data->itemtext, textlen);
  }
  xpos += textlen;

  *endx = *startx + textlen;
  /* copy to output */
  if (!file)
  {
      shifted_right = tc->shifted_right;
  }
  if (shifted_right > 0)
  {
      *endx = TEXU_MAX(0, *endx - shifted_right);
      *startx = TEXU_MAX(0, *startx - shifted_right);
  }
  memcpy(outtext, &buf[shifted_right], xpos);

  len = strlen(outtext);
  if (len < maxlen)
  {
      memset(&outtext[len], ' ', maxlen - len);
      outtext[maxlen] = 0;
  }
}

texu_i64 _TexuTreeCtrlProc_ExpandAllItemsProc(texu_tree_item* item, void* args)
{
  texu_treewnd_item* data = 0;

  data = (texu_treewnd_item*)item->data;
  data->expanded = TEXU_TRUE;
  return 0;
}

texu_i64 _TexuTreeCtrlProc_CollapseAllItemsProc(texu_tree_item* item, void* args)
{
  texu_treewnd_item* data = 0;

  data = (texu_treewnd_item*)item->data;
  data->expanded = TEXU_FALSE;
  
  return 0;
}

texu_i64 _TexuTreeCtrlProc_PreorderTraverseProc(texu_tree_item* item, void* args)
{
  texu_queue* queue = (texu_queue*) args;
  texu_treeview_item* view = 0;

  view = _texu_treeview_item_new(item);
  texu_queue_enqueue(queue, (texu_i64)view);
  return 0;
}

void _TexuTreeCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_rect rc;
  texu_char buf[TEXU_MAX_WNDTEXT + 1];
  texu_i32 displayableitems = 0;
  texu_treewnd* tc = 0;
  texu_i32 y = 0;
  texu_treewnd_item* data = 0;
  texu_treeview_item* view = 0;
  texu_list_item* iter;
  texu_i32 count = 0, items = 0;
  texu_i32 ysel = 0;
  texu_i32 xsel = 0;
  texu_char bufsel[TEXU_MAX_WNDTEXT + 1];
  texu_ui32 style = 0;
  texu_char filler = 0;
  texu_bool fullrow = TEXU_FALSE;
  texu_i32 startx = 0, endx = 0;
  texu_env* env = texu_wnd_get_env(wnd);
  texu_i32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
  texu_i32 selcolor  = texu_env_get_syscolor(env, TEXU_COLOR_MENU_SELECTED);

  tc = (texu_treewnd*) texu_wnd_get_userdata(wnd);

  /* visible window & there is a visible item */
  if (texu_wnd_is_visible(wnd) && tc->firstvisibleitem)
  {
    texu_wnd_get_rect(wnd, &rc);
    y = rc.y;

    style = texu_wnd_get_style(wnd);
    if (TEXU_TCS_NOHIGHLIGHT & style)
    {
        selcolor = normcolor;
    }
    if (TEXU_TCS_FULLSECROW & style)
    {
        fullrow = TEXU_TRUE;
    }

    _TexuTreeCtrlProc_AdjustVisibleItems(wnd);
    iter = texu_list_first(tc->visibleitems);
    items = texu_list_count(tc->visibleitems);

    /* skip the previous item before the first visible item */
    while (iter)
    {
      view = (texu_treeview_item*)iter->data;
      if (tc->firstvisibleitem == view->item)
      {
          break;
      }
      iter = iter->next;
      ++count;
    }
    /* display items */
    while (iter && displayableitems < rc.lines)
    {
      view = (texu_treeview_item*)iter->data;
      data = (texu_treewnd_item*)view->item->data;
      normcolor = data->normcolor;
      selcolor = data->selcolor;

      memset(buf, filler, sizeof (buf));
      _TexuTreeCtrlProc_GetDisplayedText(wnd, buf, &startx, &endx, view->item, rc.cols, TEXU_FALSE);
      if (data->selected)
      {
        if (fullrow)
        {
            texu_cio_putstr_attr(dc, y, rc.x, buf, selcolor);
        }
        else
        {
            buf[endx] = 0;
            texu_cio_putstr_attr(dc, y, rc.x + startx, &buf[startx],
              texu_cio_get_color(dc, selcolor));
        }
        ysel = y;
        xsel = rc.x;
        strcpy(bufsel, buf);
      }
      else
      {
        buf[endx] = 0;
        texu_cio_putstr_attr(dc, y, rc.x, buf,
          texu_cio_get_color(dc, normcolor));
      }
      /* next line */
      ++y;
      /* limit item displayed */
      ++displayableitems;
      /* move to the next visible item */
      iter = iter->next;
      ++count;
    }
    /* keep the last visible item */
    if (displayableitems > items - count)
    {
        iter = (iter ? iter->prev : 0);
    }
    if (iter)
    {
      view = (texu_treeview_item*)iter->data; 
      tc->lastvisibleitem = view->item;
    }

    /* draw the selected item */
    texu_cio_putstr_attr(dc, ysel, xsel, bufsel,
      texu_cio_get_color(dc, selcolor));
    texu_cio_gotoyx(dc, ysel, xsel);
  }
}

texu_i64
_TexuTreeCtrlProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_PAINT:
    {
        _TexuTreeCtrlProc_OnPaint(wnd, (texu_cio*)param1);
        return 0;
    }
    case TEXU_WM_CREATE:
    {
        return _TexuTreeCtrlProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
    }
    case TEXU_WM_DESTROY:
    {
        /* release memory of static control */
        _TexuTreeCtrlProc_OnDestroy(wnd);
        return 0;
    }
    case TEXU_WM_SETFOCUS:
    {
        _TexuTreeCtrlProc_OnSetFocus(wnd, (texu_wnd*)param1);
        break;
    }
    case TEXU_WM_KILLFOCUS:
    {
        return _TexuTreeCtrlProc_OnKillFocus(wnd, (texu_wnd*)param1);
    }
    case TEXU_WM_CHAR:
    {
        _TexuTreeCtrlProc_OnChar(wnd, (texu_i32) param1, (texu_i32)param2);
        break;
    }
    case TEXU_TCM_INSERTITEM:
    {
        return (texu_i32) _TexuTreeCtrlProc_OnInsertItem(wnd, (texu_tree_item*) param1, (texu_treewnd_item*) param2);
    }
    case TEXU_TCM_DELETEITEM:
    {
        return _TexuTreeCtrlProc_OnDeleteItem(wnd, (texu_tree_item*) param1);
    }
    case TEXU_TCM_SETITEM:
    {
        return _TexuTreeCtrlProc_OnSetItem(wnd, (texu_tree_item*) param1, (texu_treewnd_item*) param2);
    }
    case TEXU_TCM_GETITEM:
    {
        return _TexuTreeCtrlProc_OnGetItem(wnd, (texu_tree_item*) param1, (texu_treewnd_item*) param2);
    }
    case TEXU_TCM_FINDITEM:
    {
        return (texu_i32) _TexuTreeCtrlProc_OnFindItem(wnd, (texu_treewnd_item*) param1, (void*)param2);
    }
    case TEXU_TCM_FINDNEXTITEM:
    {
        return (texu_i32) _TexuTreeCtrlProc_OnFindNextItem(wnd, (texu_tree_item*) param1, (texu_treewnd_item*) param2);
    }
    case TEXU_TCM_EXPANDITEM:
    {
        return _TexuTreeCtrlProc_OnExpandItem(wnd, (texu_tree_item*) param1);
    }
    case TEXU_TCM_COLLAPSEITEM:
    {
        return _TexuTreeCtrlProc_OnCollapseItem(wnd, (texu_tree_item*) param1);
    }
    case TEXU_TCM_SETFINDITEMPROC:
    {
        return (texu_i32) _TexuTreeCtrlProc_OnSetFindItemProc(wnd, (texu_tree_find_proc) param1);
    }
    case TEXU_TCM_SETSELITEM:
    {
        return _TexuTreeCtrlProc_OnSetSelItem(wnd, (texu_tree_item*) param1);
    }
    case TEXU_TCM_GETSELITEM:
    {
        return (texu_i32) _TexuTreeCtrlProc_OnGetSelItem(wnd);
    }
    case TEXU_TCM_EXPORTTOFILE:
    {
        return _TexuTreeCtrlProc_OnExportToFile(wnd, (FILE*) param1, (texu_tree_exp_proc) param2);
    }
    case TEXU_TCM_IMPORTFROMFILE:
    {
        return _TexuTreeCtrlProc_OnImportFromFile(wnd, (FILE*) param1, (texu_tree_imp_proc) param2);
    }
    case TEXU_TCM_COLLAPSEALLITEMS:
    {
        _TexuTreeCtrlProc_OnCollapseAllItems(wnd);
        break;
    }
    case TEXU_TCM_EXPANDALLITEMS:
    {
        _TexuTreeCtrlProc_OnExpandAllItems(wnd);
        break;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}


/*
#-------------------------------------------------------------------------------
# TexU up/down ctrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_udwnd
{
  texu_wnd*       editwnd;
  texu_i32        step;
  texu_i32        page;
  texu_i32        min;
  texu_i32        max;
};
typedef struct texu_udwnd texu_udwnd;


void                  _TexuUpDownCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt);
texu_status           _TexuUpDownCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs);
void                  _TexuUpDownCtrlProc_OnDestroy(texu_wnd* wnd);
void                  _TexuUpDownCtrlProc_OnSetFocus(texu_wnd*, texu_wnd*);
texu_i32              _TexuUpDownCtrlProc_OnKillFocus(texu_wnd*, texu_wnd*);
texu_i32              _TexuUpDownCtrlProc_OnGetText(texu_wnd* wnd, texu_char* text, texu_i32 textlen);
void                  _TexuUpDownCtrlProc_OnSetText(texu_wnd* wnd, const texu_char* text);
void                  _TexuUpDownCtrlProc_OnSetMinMax(texu_wnd* wnd, texu_i32, texu_i32);
texu_status           _TexuUpDownCtrlProc_OnGetMinMax(texu_wnd* wnd, texu_i32*, texu_i32*);
void                  _TexuUpDownCtrlProc_OnSetStep(texu_wnd* wnd, texu_i32);
texu_i32              _TexuUpDownCtrlProc_OnGetStep(texu_wnd* wnd);
void                  _TexuUpDownCtrlProc_OnStep(texu_wnd* wnd, texu_i32);
void                  _TexuUpDownCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc);
void                  _TexuUpDownCtrlProc_OnSetPage(texu_wnd* wnd, texu_i32);
texu_i32              _TexuUpDownCtrlProc_OnGetPage(texu_wnd* wnd);
void                  _TexuUpDownCtrlProc_OnPage(texu_wnd* wnd, texu_i32);


void
_TexuUpDownCtrlProc_OnSetMinMax(texu_wnd* wnd, texu_i32 min, texu_i32 max)
{
  texu_udwnd* udctl = 0;
  texu_editminmax minmax;
  
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  udctl->min = min;
  udctl->max = max;
  
  minmax.min = min;
  minmax.max = max;
  texu_wnd_send_msg(udctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
}

texu_status
_TexuUpDownCtrlProc_OnGetMinMax(texu_wnd* wnd, texu_i32* min, texu_i32* max)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  *min = udctl->min;
  *max = udctl->max;
  return TEXU_OK;
}

void
_TexuUpDownCtrlProc_OnSetStep(texu_wnd* wnd, texu_i32 step)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  udctl->step = step;
}

texu_i32
_TexuUpDownCtrlProc_OnGetStep(texu_wnd* wnd)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  return udctl->step;
}

void
_TexuUpDownCtrlProc_OnStep(texu_wnd* wnd, texu_i32 updown)
{
  if (updown > 0)
  {
    _TexuUpDownCtrlProc_OnChar(wnd, KEY_UP, 0);
  }
  else
  {
    _TexuUpDownCtrlProc_OnChar(wnd, KEY_DOWN, 0);
  }
}

void
_TexuUpDownCtrlProc_OnSetPage(texu_wnd* wnd, texu_i32 page)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  udctl->page = page;
}

texu_i32
_TexuUpDownCtrlProc_OnGetPage(texu_wnd* wnd)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  return udctl->page;
}

void
_TexuUpDownCtrlProc_OnPage(texu_wnd* wnd, texu_i32 updown)
{
  if (updown > 0)
  {
    _TexuUpDownCtrlProc_OnChar(wnd, KEY_NPAGE, 0);
  }
  else
  {
    _TexuUpDownCtrlProc_OnChar(wnd, KEY_PPAGE, 0);
  }
}

texu_status
_TexuUpDownCtrlProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_udwnd* udctl = 0;
  texu_wnd_attrs attrs2;
  texu_wnd* editwnd = 0;
  texu_status rc = TEXU_OK;
  texu_editminmax minmax;
  texu_env* env = texu_wnd_get_env(wnd);
  
  editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
  if (!editwnd)
  {
    return TEXU_NOMEM;
  }
  
  memset(&attrs2, 0, sizeof(attrs2));
  attrs2.y          = attrs->y;
  attrs2.x          = attrs->x;
  attrs2.height     = attrs->height;
  attrs2.width      = attrs->width-1;
  attrs2.enable     = TEXU_TRUE;
  attrs2.visible    = TEXU_TRUE;
  attrs2.text       = "0";
  attrs2.normalcolor    = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL);
  attrs2.disabledcolor  = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED);
  attrs2.id         = 1;
  attrs2.clsname    = TEXU_EDIT_CLASS;
  attrs2.userdata   = 0;
  attrs2.style      = TEXU_ES_AUTOHSCROLL|TEXU_ES_NUMBER|TEXU_ES_CENTER;
  attrs2.exstyle    = 0;
  
  rc = texu_wnd_create(editwnd, wnd, &attrs2);

  if (rc != TEXU_OK)
  {
    texu_wnd_del(editwnd);
    return TEXU_ERROR;
  }
 
  udctl = (texu_udwnd*)malloc(sizeof(texu_lcwnd));
  if (!udctl)
  {
    texu_wnd_del(editwnd);
    return TEXU_NOMEM;
  }
  
  memset(udctl, 0, sizeof(texu_udwnd));
  udctl->editwnd = editwnd;   /* no parameter */
  udctl->step = 1;
  udctl->page = 10;
  udctl->min = 0;
  udctl->max = 255;
  
  minmax.min = udctl->min;
  minmax.max = udctl->max;
  texu_wnd_send_msg(udctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);  
  
  /* save memory */
  texu_wnd_set_color(wnd,
    texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL),
    texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED));
  texu_wnd_set_userdata(wnd, udctl);
  return TEXU_OK;
}



void
_TexuUpDownCtrlProc_OnSetFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  texu_udwnd* udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  texu_wnd_send_msg(udctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
  _TexuWndProc_Notify(wnd, TEXU_UDCN_SETFOCUS);
}

texu_i32
_TexuUpDownCtrlProc_OnKillFocus(texu_wnd* wnd, texu_wnd* prevwnd)
{
  /*update value to window text */
  texu_udwnd* udctl = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  
 
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  
  texu_wnd_send_msg(udctl->editwnd, TEXU_WM_KILLFOCUS, 0, 0);
  
  texu_wnd_get_text(udctl->editwnd, buf, TEXU_MAX_WNDTEXT);
  texu_wnd_set_text(wnd, buf);
 
  _TexuWndProc_Notify(wnd, TEXU_UDCN_KILLFOCUS);

  return TEXU_CONTINUE;
}

void
_TexuUpDownCtrlProc_OnDestroy(texu_wnd* wnd)
{
  texu_udwnd* udctl = 0;
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  free(udctl);
}

texu_i32
_TexuUpDownCtrlProc_OnGetInt(texu_wnd* wnd)
{
  texu_udwnd* udctl = 0;
  texu_i32 val = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];

  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  texu_wnd_get_text(udctl->editwnd, buf, TEXU_MAX_WNDTEXT);
  val = atol(buf);

  return val;
}

void
_TexuUpDownCtrlProc_OnChar(texu_wnd* wnd, texu_i32 ch, texu_i32 alt)
{
  texu_udwnd* udctl = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_i32 val = 0;
  texu_bool updown = TEXU_FALSE;
  texu_i32 added = 0;
  
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  switch (ch)
  {
    case KEY_UP:
    {
      added += udctl->step;
      updown = TEXU_TRUE;
      break;
    }
    case KEY_DOWN:
    {
      added -= udctl->step;
      updown = TEXU_TRUE;
      break;
    }
    case KEY_NPAGE:
    {
      added -= udctl->page;
      updown = TEXU_TRUE;
      break;
    }
    case KEY_PPAGE:
    {
      added += udctl->page;
      updown = TEXU_TRUE;
      break;
    }
  }
  
  if (updown)
  {
    texu_wnd_send_msg(udctl->editwnd, TEXU_WM_KILLFOCUS, 0, 0);
    
    texu_wnd_get_text(udctl->editwnd, buf, TEXU_MAX_WNDTEXT);
    val = atol(buf);
    val += added;
    if (val < udctl->min)
    {
      val = udctl->min;
    }
    if (val > udctl->max)
    {
      val = udctl->max;
    }
    sprintf(buf, "%d", val);
    texu_wnd_set_text(wnd, buf);
    
    _TexuWndProc_Notify(wnd, TEXU_UDCN_STEP);
    texu_wnd_send_msg(udctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
  }
  else
  {
    texu_wnd_send_msg(udctl->editwnd, TEXU_WM_CHAR, (texu_i64)ch, 0);
  }
}

void
_TexuUpDownCtrlProc_OnSetText(texu_wnd* wnd, const texu_char* text)
{
  texu_udwnd* udctl = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_i32 val = 0;
  
  udctl = (texu_udwnd*)texu_wnd_get_userdata(wnd);
  val = atol(text);
  
  sprintf(buf, "%d", val);
  texu_wnd_set_text(udctl->editwnd, buf);
  TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)buf, 0);
}

void 
_TexuUpDownCtrlProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_i32 y = texu_wnd_get_y(wnd);
  texu_i32 x = texu_wnd_get_x(wnd);
  texu_i32 width = texu_wnd_get_width(wnd);
  texu_env* env = texu_wnd_get_env(wnd);
  texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL);
  
  texu_cio_putch_attr(dc, y, x+width-1, ACS_PLMINUS,
    texu_cio_get_color(dc, color));
}


texu_i64
_TexuUpDownCtrlProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_CHAR:
      _TexuUpDownCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);  
      return 0;
      
    case TEXU_WM_CREATE:
      return _TexuUpDownCtrlProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_PAINT:
      _TexuUpDownCtrlProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
      
    case TEXU_WM_DESTROY:
      _TexuUpDownCtrlProc_OnDestroy(wnd);
      break;
      
    case TEXU_WM_SETFOCUS:
      _TexuUpDownCtrlProc_OnSetFocus(wnd, (texu_wnd*)param1);
      break;

    case TEXU_WM_KILLFOCUS:
      return _TexuUpDownCtrlProc_OnKillFocus(wnd, (texu_wnd*)param1);

    case TEXU_WM_SETTEXT:
      _TexuUpDownCtrlProc_OnSetText(wnd, (const texu_char*)param1);
      return 0;
      
    case TEXU_UDCM_SETMINMAX:
    {
      _TexuUpDownCtrlProc_OnSetMinMax(wnd, (texu_i32)param1, (texu_i32)param2);
      return 0;
    }
    case TEXU_UDCM_GETMINMAX:
    {
      return _TexuUpDownCtrlProc_OnGetMinMax(wnd, (texu_i32*)param1, (texu_i32*)param2);
    }
    case TEXU_UDCM_SETSTEP:
    {
      _TexuUpDownCtrlProc_OnSetStep(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_UDCM_GETSTEP:
    {
      return _TexuUpDownCtrlProc_OnGetStep(wnd);
    }
    case TEXU_UDCM_STEP:
    {
      _TexuUpDownCtrlProc_OnStep(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_UDCM_SETPAGE:
    {
      _TexuUpDownCtrlProc_OnSetPage(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_UDCM_GETPAGE:
    {
      return _TexuUpDownCtrlProc_OnGetPage(wnd);
    }
    case TEXU_UDCM_PAGE:
    {
      _TexuUpDownCtrlProc_OnPage(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_UDCM_GETINT:
    {
      return _TexuUpDownCtrlProc_OnGetInt(wnd);
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}
  


/*
#-------------------------------------------------------------------------------
# TexU up/down ctrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_pgbwnd
{
  texu_i32        step;
  texu_i32        pos;
  texu_i32        max;
};
typedef struct texu_pgbwnd texu_pgbwnd;


texu_status           _TexuProgressBarProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs);
void                  _TexuProgressBarProc_OnDestroy(texu_wnd* wnd);
void                  _TexuProgressBarProc_OnSetMax(texu_wnd* wnd, texu_i32);
void                  _TexuProgressBarProc_OnSetPos(texu_wnd* wnd, texu_i32);
texu_i32              _TexuProgressBarProc_OnGetPos(texu_wnd* wnd);
void                  _TexuProgressBarProc_OnSetStep(texu_wnd* wnd, texu_i32);
texu_i32              _TexuProgressBarProc_OnGetStep(texu_wnd* wnd);
void                  _TexuProgressBarProc_OnStep(texu_wnd* wnd, texu_i32);
void                  _TexuProgressBarProc_OnPaint(texu_wnd* wnd, texu_cio* dc);
texu_i32              _TexuProgressBarProc_OnGetText(texu_wnd* wnd, texu_char* text, texu_i32 textlen);

texu_f32             _TexuProgressBarProc_GetPercent(texu_wnd* wnd);

void
_TexuProgressBarProc_OnSetMax(texu_wnd* wnd, texu_i32 max)
{
  texu_pgbwnd* pgb = 0;
  
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  pgb->max = max;
}

texu_i32
_TexuProgressBarProc_OnGetMax(texu_wnd* wnd)
{
  texu_pgbwnd* pgb = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  return pgb->max;
}

void
_TexuProgressBarProc_OnSetPos(texu_wnd* wnd, texu_i32 pos)
{
  texu_pgbwnd* pgb = 0;
  
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  if (pos < 0)
  {
    pos = 0;
  }
  if (pos > pgb->max)
  {
    pos = pgb->max;
  }
  pgb->pos = pos;
  texu_wnd_invalidate(wnd);
}

texu_i32
_TexuProgressBarProc_OnGetPos(texu_wnd* wnd)
{
  texu_pgbwnd* pgb = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  return pgb->pos;
}

void
_TexuProgressBarProc_OnSetStep(texu_wnd* wnd, texu_i32 step)
{
  texu_pgbwnd* pgb = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  pgb->step = step;
}

texu_i32
_TexuProgressBarProc_OnGetStep(texu_wnd* wnd)
{
  texu_pgbwnd* pgb = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  return pgb->step;
}

void
_TexuProgressBarProc_OnStep(texu_wnd* wnd, texu_i32 updown)
{
  texu_pgbwnd* pgb = 0;
  texu_i32 pos = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  pos = pgb->pos;
  
  if (updown > 0)
  {
    pos += pgb->step;
  }
  else
  {
    pos -= pgb->step;
  }
  
  if (pos < 0)
  {
    pos = 0;
  }
  if (pos > pgb->max)
  {
    pos = pgb->max;
  }
  pgb->pos = pos;
  texu_wnd_invalidate(wnd);
}

texu_status
_TexuProgressBarProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
  texu_pgbwnd* pgb = 0;
  texu_env* env = texu_wnd_get_env(wnd);
 
  pgb = (texu_pgbwnd*)malloc(sizeof(texu_lcwnd));
  if (!pgb)
  {
    return TEXU_NOMEM;
  }
  
  memset(pgb, 0, sizeof(texu_pgbwnd));
  pgb->step = 1;
  pgb->max = 100;
  pgb->pos = 0;
  
  texu_wnd_set_color(wnd,
    texu_env_get_syscolor(env, TEXU_COLOR_PROGRESSBAR),
    texu_env_get_syscolor(env, TEXU_COLOR_PROGRESSBAR_DISABLED));
  texu_wnd_set_userdata(wnd, pgb);
  texu_wnd_enable(wnd, TEXU_FALSE);
  return TEXU_OK;
}

void
_TexuProgressBarProc_OnDestroy(texu_wnd* wnd)
{
  texu_pgbwnd* pgb = 0;
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  free(pgb);
}

texu_f32
_TexuProgressBarProc_GetPercent(texu_wnd* wnd)
{
  texu_f32 pct = 0.0;
  texu_pgbwnd* pgb = 0;
  
  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  
  pct = (0 == pgb->max ? 0.0 : (texu_f32)(pgb->pos * 100.0 / pgb->max));
  return pct;
}

void 
_TexuProgressBarProc_OnPaint(texu_wnd* wnd, texu_cio* dc)
{
  texu_i32 y = texu_wnd_get_y(wnd);
  texu_i32 x = texu_wnd_get_x(wnd);
  texu_i32 width = texu_wnd_get_width(wnd);
  texu_env* env = texu_wnd_get_env(wnd);
  texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_PROGRESSBAR);

  texu_char buf[TEXU_MAX_WNDTEXT+1];
  texu_char text[TEXU_MAX_WNDTEXT+1];
  texu_f32 pct = 0.0;
  texu_pgbwnd* pgb = 0;
  texu_i32 pgwidth = 0;

  pgb = (texu_pgbwnd*)texu_wnd_get_userdata(wnd);
  
  memset(buf, 0, width+1);
  memset(buf, '.', width);
  texu_cio_putstr_attr(dc, y, x, buf,
    texu_cio_get_color(dc, color));
  
  pct = (0 == pgb->max ? 0.0 : (texu_f32)(pgb->pos * 100.0 / pgb->max));
  sprintf(text, "%.02f%%", pct);
  
  pgwidth = (0 == pgb->max ? 0 : (100 == (texu_i32)pct ? width : (pgb->pos * width / pgb->max)));
  texu_printf_alignment(buf, text, pgwidth, TEXU_ALIGN_RIGHT);
  
  strcat(buf, "%%");
  texu_cio_putstr_attr(dc, y, x, buf,
    texu_cio_get_reverse(dc, color));
}

texu_i32
_TexuProgressBarProcc_OnGetText(texu_wnd* wnd, texu_char* text, texu_i32 textlen)
{
  texu_f32 pct = _TexuProgressBarProc_GetPercent(wnd);
  texu_ui32 len = 0;
  texu_char buf[TEXU_MAX_WNDTEXT+1];

  sprintf(buf, "%.02f%%", pct);
  len = strlen(buf);
  if (textlen < 0 || 0 == text)
  {
    return len;
  }

  memset(text, 0, textlen);
  textlen = TEXU_MIN(len, textlen);
  memcpy(text, buf, textlen);

  return textlen;
}

texu_i64
_TexuProgressBarProc(texu_wnd* wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
  switch (msg)
  {
    case TEXU_WM_GETTEXT:
      return _TexuProgressBarProcc_OnGetText(wnd, (texu_char*)param1, (texu_i32)param2);
      
    case TEXU_WM_CREATE:
      return _TexuProgressBarProc_OnCreate(wnd, (texu_wnd_attrs*)param1);

    case TEXU_WM_PAINT:
      _TexuProgressBarProc_OnPaint(wnd, (texu_cio*)param1);
      return 0;
      
    case TEXU_WM_DESTROY:
      _TexuProgressBarProc_OnDestroy(wnd);
      break;

    case TEXU_WM_ENABLE:
      return TexuDefWndProc(wnd, msg, 0, 0);

    case TEXU_PGBM_SETMAX:
    {
      _TexuProgressBarProc_OnSetMax(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_PGBM_GETMAX:
    {
      return _TexuProgressBarProc_OnGetMax(wnd);
    }

    case TEXU_PGBM_SETPOS:
    {
      _TexuProgressBarProc_OnSetPos(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_PGBM_GETPOS:
    {
      return _TexuProgressBarProc_OnGetPos(wnd);
    }
    case TEXU_PGBM_SETSTEP:
    {
      _TexuProgressBarProc_OnSetStep(wnd, (texu_i32)param1);
      return 0;
    }
    case TEXU_PGBM_GETSTEP:
    {
      return _TexuProgressBarProc_OnGetStep(wnd);
    }
    case TEXU_PGBM_STEP:
    {
      _TexuProgressBarProc_OnStep(wnd, (texu_i32)param1);
      return 0;
    }
  }
  return TexuDefWndProc(wnd, msg, param1, param2);
}


#ifdef __cplusplus
}
#endif

