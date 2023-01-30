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
extern "C"
{
#endif

void _TexuWndProc_Notify(texu_wnd *, texu_ui32);

/*
#-------------------------------------------------------------------------------
# TexU listctrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_lcwnd_cell
{
    texu_i32 y;
    texu_i32 x;
    texu_i32 lines;
    texu_i32 cols;
    texu_char caption[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 normcolor; /* text attributes          */
    texu_ui32 discolor;  /* text attributes          */
    texu_ui32 selcolor;  /* text attributes          */
#if (defined WIN32 && defined _WINDOWS)
    texu_ui32 normbg; /* text attributes          */
    texu_ui32 disbg;  /* text attributes          */
    texu_ui32 selbg;  /* text attributes          */
#endif
    void *data;
    texu_ui32 editstyle;
    texu_bool enable;

    struct texu_lcwnd_cell *prev;
    struct texu_lcwnd_cell *next;
};
typedef struct texu_lcwnd_cell texu_lcwnd_cell;

struct texu_lcwnd_header
{
    texu_i32 id;
    texu_char caption[TEXU_MAX_WNDTEXT + 1];
    texu_i32 cols;      /* width */
    texu_i32 align;     /* left is a default */
    texu_i32 normcolor; /* text attributes          */
    texu_i32 discolor;  /* text attributes          */
    texu_i32 selcolor;  /* text attributes          */
    texu_ui32 editstyle;
    texu_i32 decwidth;
    texu_bool enable;
    void *userdata;
#if (defined WIN32 && defined _WINDOWS)
    texu_i32 normbg; /* text attributes          */
    texu_i32 disbg;  /* text attributes          */
    texu_i32 selbg;  /* text attributes          */
#endif
    texu_lcwnd_cell *firstcell;
    texu_lcwnd_cell *lastcell;

    struct texu_lcwnd_header *prev;
    struct texu_lcwnd_header *next;
};
typedef struct texu_lcwnd_header texu_lcwnd_header;

struct texu_lcwnd
{
    texu_lcwnd_header *firsthdr;
    texu_lcwnd_header *lasthdr;
    texu_lcwnd_header *firstvisiblehdr;
    texu_lcwnd_header *lastvisiblehdr;

    texu_i32 nheaders;
    texu_i32 nitems;
    texu_i32 hdrids; /* header id */
    texu_i32 hdrallwidths;
    /* item control */
    texu_i32 curselrow;
    texu_i32 curselcol;
    texu_i32 firstvisiblerow;

    texu_wnd *editbox;
    texu_i32 editingstate;
    texu_i32 movingstate;
    texu_i32 insertingstate;
    texu_lcwnd_cell *editingcell;

    void *exparam;

    texu_i32 editkey;
    texu_i32 insertkey;
    texu_i32 delkey;
    texu_i32 canceleditkey;

    texu_i32 *editcols;
    texu_i32 nedtcols;
    texu_i32 curedtcol;

    texu_lcwnd_cell *firsteditcell;
    texu_lcwnd_cell *lasteditcell;
};
typedef struct texu_lcwnd texu_lcwnd;

texu_lcwnd_header *_TexuListCtrlProc_FindHeaderByIndex(texu_lcwnd *lctl, texu_i32 col);
texu_lcwnd_cell *_TexuListCtrlProc_FindCellByIndex(texu_lcwnd *lctl, texu_i32 col, texu_i32 idx);
texu_lcwnd_cell *_TexuListCtrlProc_FindCellByHeader(texu_lcwnd *lctl, texu_lcwnd_header *header, texu_i32 idx);
texu_i32 _TexuListCtrlProc_GetCellRect(texu_lcwnd_cell *cell, texu_rect *rect);
texu_i32 _TexuListCtrlProc_FindHeaderIndex(texu_lcwnd *lctl, texu_lcwnd_header *header);
texu_i32 _TexuListCtrlProc_GetFirstEditableCol(texu_lcwnd *lctl);
texu_i32 _TexuListCtrlProc_GetNextEditableCol(texu_lcwnd *lctl, texu_i32 prevcol);
texu_i32 _TexuListCtrlProc_GetPrevEditableCol(texu_lcwnd *lctl, texu_i32 prevcol);
texu_i32 _TexuListCtrlProc_GetLastEditableCol(texu_lcwnd *lctl);
texu_i32 _TexuListCtrlProc_InsertEmptyItem(texu_wnd *wnd);
texu_i32 _TexuListCtrlProc_GetColIdx(texu_lcwnd *lctl, texu_lcwnd_header *header);

texu_i32 _TexuListCtrlProc_GetVisibleWidth(texu_lcwnd *lctl);
void _TexuListCtrlProc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_i32 row, texu_i32 col);
#if (defined WIN32 && defined _WINDOWS)
void _TexuListCtrlProc_FillRow(texu_wnd *wnd, texu_lcwnd *lc,
                               texu_cio *dc, texu_i32 row,
                               texu_ui32 color, texu_ui32 bgcolor);
void _TexuListCtrlProc_FillCell(texu_wnd *wnd, texu_lcwnd *lc,
                                texu_cio *dc, texu_i32 row, texu_i32 col,
                                texu_ui32 color, texu_ui32 bgcolor);
void _TexuListCtrlProc_DrawItem(texu_wnd *wnd,
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_ui32 bgcolor, 
    texu_i32 align, texu_i32 isheader);
void _TexuListCtrlProc_DrawItem2(texu_wnd *wnd,
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_ui32 bgcolor,
    texu_i32 align, texu_i32 isheader, texu_i32 issel);
#else
void _TexuListCtrlProc_FillRow(texu_wnd *wnd, texu_lcwnd *lc,
                               texu_cio *dc, texu_i32 row,
                               texu_ui32 color);
void _TexuListCtrlProc_FillCell(texu_wnd *wnd, texu_lcwnd *lc,
                                texu_cio *dc, texu_i32 row, texu_i32 col,
                                texu_ui32 color);
void _TexuListCtrlProc_DrawItem(
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_i32 align, texu_i32 isheader);
void _TexuListCtrlProc_DrawItem2(
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_i32 align, texu_i32 isheader, texu_i32 issel);
#endif
void _TexuListCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void _TexuListCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_i32 _TexuListCtrlProc_OnAddItems(texu_wnd *wnd, texu_char *text, texu_i32 nitems);
texu_i32 _TexuListCtrlProc_OnAddItem(texu_wnd *wnd, const texu_char *);
void _TexuListCtrlProc_OnDeleteAllItems(texu_wnd *wnd);
void _TexuListCtrlProc_OnDeleteColumn(texu_wnd *wnd, texu_i32 col);
texu_i32 _TexuListCtrlProc_OnAddColumn(texu_wnd *wnd, texu_wnd_header *hdritem);
void _TexuListCtrlProc_OnDestroy(texu_wnd *wnd);
texu_status _TexuListCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
texu_i32 _TexuListCtrlProc_OnGetItem(texu_wnd *wnd, texu_ui32 flags, texu_wnd_subitem *subitem);
texu_i32 _TexuListCtrlProc_OnSetItem(texu_wnd *wnd, texu_ui32 flags, texu_wnd_subitem *subitem);
void _TexuListCtrlProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuListCtrlProc_OnKillFocus(texu_wnd *, texu_wnd *);
void _TexuListCtrlProc_OnInvalidateItem(texu_wnd *wnd, texu_i32 row, texu_i32 col);
void _TexuListCtrlProc_OnSelChanged(texu_wnd *wnd);
void _TexuListCtrlProc_OnBeginMoving(texu_wnd *wnd);
void _TexuListCtrlProc_OnMovingCursor(texu_wnd *wnd, texu_i32 ch);
void _TexuListCtrlProc_OnEndMoving(texu_wnd *wnd);
texu_ui32 _TexuListCtrlProc_OnGetEditStyle(texu_wnd *wnd, texu_i32 col);
void _TexuListCtrlProc_OnSetEditStyle(texu_wnd *wnd, texu_i32 col, texu_ui32 editstyle);
texu_i32 _TexuListCtrlProc_OnGetCurRow(texu_wnd *wnd);
void _TexuListCtrlProc_OnSetCurRow(texu_wnd *wnd, texu_i32 idx);
void _TexuListCtrlProc_OnSetColWidth(texu_wnd *wnd, texu_i32 col, texu_i32 width);
texu_wnd *_TexuListCtrlProc_OnGetEditBox(texu_wnd *wnd);
void _TexuListCtrlProc_OnSetCurPage(texu_wnd *wnd, texu_i32 npage);
texu_i32 _TexuListCtrlProc_OnGetCurPage(texu_wnd *wnd);
texu_i32 _TexuListCtrlProc_OnGetItemsPerPage(texu_wnd *wnd);
void _TexuListCtrlProc_OnSetEditableCols(texu_wnd *wnd, texu_i32 ncols, const texu_i32 *edtcols);
void _TexuListCtrlProc_OnEndEditRow(texu_wnd *wnd, texu_i32 row, texu_i32 ok);
void _TexuListCtrlProc_OnBeginEditRow(texu_wnd *wnd, texu_i32 row);
void _TexuListCtrlProc_OnBeginInsertRow(texu_wnd *wnd, texu_i32 row);
void _TexuListCtrlProc_OnBeginInsertCell(texu_wnd *wnd, texu_i32 row);
void _TexuListCtrlProc_OnBeginEdit(texu_wnd *wnd);
void _TexuListCtrlProc_OnEndEdit(texu_wnd *wnd, texu_i32 ok);
void _TexuListCtrlProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx);
void _TexuListCtrlProc_OnDeleteAllColumns(texu_wnd *wnd);
texu_i32    _TexuListCtrlProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuListCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text);

texu_i32
_TexuListCtrlProc_FindHeaderIndex(texu_lcwnd *lctl, texu_lcwnd_header *header)
{
    texu_i32 col = -1;
    texu_lcwnd_header *ctlhdr = lctl->firsthdr;

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

texu_i32
_TexuListCtrlProc_GetColIdx(texu_lcwnd *lctl, texu_lcwnd_header *header)
{
    texu_lcwnd_header *h1 = lctl->firsthdr;
    texu_i32 col = 0;
    while (h1 && h1 != header)
    {
        h1 = h1->next;
    }
    return col;
}

texu_lcwnd_header *
_TexuListCtrlProc_FindHeaderByIndex(texu_lcwnd *lctl, texu_i32 col)
{
    texu_lcwnd_header *header = 0;
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

texu_lcwnd_cell *
_TexuListCtrlProc_FindCellByIndex(texu_lcwnd *lctl, texu_i32 col, texu_i32 idx)
{
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_header *header = 0;
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

texu_lcwnd_cell *
_TexuListCtrlProc_FindCellByHeader(texu_lcwnd *lctl, texu_lcwnd_header *header, texu_i32 idx)
{
    texu_lcwnd_cell *cell = 0;
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
_TexuListCtrlProc_GetCellRect(texu_lcwnd_cell *cell, texu_rect *rect)
{
    rect->y = cell->y;
    rect->x = cell->x;
    rect->lines = cell->lines;
    rect->cols = cell->cols;
    return TEXU_OK;
}

texu_status
_TexuListCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_lcwnd *lctl = 0;
    texu_i32 editable = 0;
    texu_wnd_attrs attrs2;
    texu_wnd *editwnd = 0;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(wnd);

    editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y        = 0;
    attrs2.x        = 0;
    attrs2.height   = 1;
    attrs2.width    = 1;
    attrs2.enable   = TEXU_FALSE;
    attrs2.visible  = TEXU_FALSE;
    attrs2.text     = TEXU_EMPTY_STR;
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL);
    attrs2.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_SELECTED);
    attrs2.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL);
#if (defined WIN32 && defined _WINDOWS)
    attrs2.normalbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL);
    attrs2.disabledbg   = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_DISABLED);
    attrs2.selectedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_SELECTED);
    attrs2.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL);
#endif
    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }

    lctl = (texu_lcwnd *)malloc(sizeof(texu_lcwnd));
    if (!lctl)
    {
        texu_wnd_del(editwnd);
        return TEXU_NOMEM;
    }

    memset(lctl, 0, sizeof(texu_lcwnd));
    lctl->hdrids = 1;

    lctl->editbox = editwnd; /* no parameter */
    lctl->curselrow = -1;
    lctl->curselcol = -1;
    lctl->firstvisiblerow = -1;

    lctl->editkey = TEXU_KEY_ENTER;/* TEXU_KEY_BACKSPACE;*/
    lctl->insertkey = TEXU_KEY_IC;
    lctl->delkey = TEXU_KEY_DC;
    lctl->canceleditkey = TEXU_KEY_ESCAPE;

    lctl->curedtcol = -1;
    lctl->nedtcols = 256;
    lctl->editcols = (texu_i32 *)malloc(lctl->nedtcols * sizeof(texu_i32));
    if (TEXU_LCS_EDITABLE & attrs->style)
    {
        editable = 1;
    }
    memset(lctl->editcols, editable, lctl->nedtcols * sizeof(texu_i32));
    lctl->firsteditcell = 0;
    lctl->lasteditcell = 0;
    /* increment child ids */
    ++lctl->hdrids;

    /* save memory */
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL), 
                       texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_FOCUSED));
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_FOCUSED));
#endif
    texu_wnd_set_userdata(wnd, lctl);
    return TEXU_OK;
}

void
_TexuListCtrlProc_OnSelChanged(texu_wnd *wnd)
{
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    _TexuWndProc_Notify(wnd, TEXU_LCN_SELCHANGED);
}

void
_TexuListCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    _TexuWndProc_Notify(wnd, TEXU_LCN_SETFOCUS);
}

texu_i32
_TexuListCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_lcwnd *lctl = 0;
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
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
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_CONTINUE;
}

void
_TexuListCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

    _TexuListCtrlProc_OnDeleteAllItems(wnd);

    free(lctl->editcols);
    free(lctl);
}

void
_TexuListCtrlProc_OnSetEditableCols(texu_wnd *wnd, texu_i32 ncols, const texu_i32 *edtcols)
{
    texu_lcwnd *lctl = 0;
    texu_i32 i = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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
_TexuListCtrlProc_OnAddColumn(texu_wnd *wnd, texu_wnd_header *hdritem)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *header = 0;
    texu_rect rc;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

    if (lctl->nitems > 0)
    {
        /* not allowed to add header after there are any items */
        return TEXU_ERROR;
    }

    header = (texu_lcwnd_header *)malloc(sizeof(texu_lcwnd_header));
    if (!header)
    {
        return TEXU_NOMEM;
    }
    memset(header, 0, sizeof(texu_lcwnd_header));

    texu_wnd_get_rect(texu_wnd_get_parent(wnd), &rc);
    texu_strncpy(header->caption,
            hdritem->caption,
            TEXU_MIN(TEXU_MAX_WNDTEXT, texu_strlen(hdritem->caption)));
    header->cols = hdritem->cols;
    header->align = hdritem->align;

    header->normcolor = hdritem->normcolor; /* text attributes          */
    header->discolor = hdritem->discolor;   /* text attributes          */
    header->selcolor = hdritem->selcolor;   /* text attributes          */
#if (defined WIN32 && defined _WINDOWS)
    header->normbg = hdritem->normbg; /* text attributes          */
    header->disbg = hdritem->disbg;   /* text attributes          */
    header->selbg = hdritem->selbg;   /* text attributes          */
#endif
    header->editstyle = hdritem->editstyle;
    header->decwidth = hdritem->decwidth;
    header->id = lctl->hdrids;

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
_TexuListCtrlProc_OnDeleteAllColumns(texu_wnd *wnd)
{
    texu_i32 nheaders = 0;
    texu_i32 i = 0;
    texu_lcwnd *lctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    nheaders = lctl->nheaders;
    for (i = 0; i < nheaders; ++i)
    {
        _TexuListCtrlProc_OnDeleteColumn(wnd, 0);
    }
}

void
_TexuListCtrlProc_OnDeleteColumn(texu_wnd *wnd, texu_i32 col)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *next = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_cell *nextcell = 0;
    texu_lcwnd_header *header = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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
        cell->next = cell->prev = 0;
        free(cell);
        cell = nextcell;
    }

    free(header);

    /* done */
    --lctl->nheaders;
}

void
_TexuListCtrlProc_OnDeleteAllItems(texu_wnd *wnd)
{
    texu_i32 nitems = 0;
    texu_i32 i = 0;
    texu_lcwnd *lctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    nitems = lctl->nitems;
    for (i = 0; i < nitems; ++i)
    {
        _TexuListCtrlProc_OnDeleteItem(wnd, 0);
    }
}

void
_TexuListCtrlProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_cell *nextcell = 0;
    texu_lcwnd_header *header = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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
_TexuListCtrlProc_OnAddItem(texu_wnd *wnd, const texu_char *text)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
#if (defined WIN32 && defined UNICODE)
    swprintf(buf, sizeof(buf), L"\t%s", text);
#else
    sprintf(buf, "\t%s", text);
#endif
    return _TexuListCtrlProc_OnAddItems(wnd, (texu_char *)text, 1);
}

texu_i32
_TexuListCtrlProc_OnAddItems(texu_wnd *wnd, texu_char *text, texu_i32 nitems)
{
    texu_lcwnd *lctl = 0;
    texu_i32 i = 0;
    texu_char *tok;
    texu_lcwnd_header *header = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_lcwnd_cell *newcell = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_char *pszContext;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (lctl->nheaders > 0)
    {
        /* insert into all listboxes */
        header = lctl->firsthdr;

        texu_strcpy(buf, text);
#if (defined WIN32 && defined UNICODE)
        tok = wcstok_s(buf, TEXUTEXT("\t"), &pszContext);
#else
        tok = strtok(buf, "\t");
#endif
        while (/*tok &&*/ i < nitems && i < lctl->nheaders)
        {
            newcell = (texu_lcwnd_cell *)malloc(sizeof(texu_lcwnd_cell));
            if (!newcell)
            {
                break;
            }
            memset(newcell, 0, sizeof(texu_lcwnd_cell));
            if (tok)
            {
                texu_strncpy(newcell->caption, tok, TEXU_MIN(TEXU_MAX_WNDTEXT, texu_strlen(tok)));
            }
            else
            {
                texu_strcpy(newcell->caption, TEXU_EMPTY_STR);
            }
            newcell->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM);
            newcell->discolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM_DISABLED);
            newcell->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTCTRL_ITEM_SELECTED);
#if (defined WIN32 && defined _WINDOWS)
            newcell->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_ITEM);
            newcell->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_ITEM_DISABLED);
            newcell->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTCTRL_ITEM_SELECTED);
#endif
            /* add the new item */
            if (header->firstcell)
            {
                newcell->prev = header->lastcell;
                header->lastcell->next = newcell;
                header->lastcell = newcell;
            }
            else
            {
                header->firstcell = header->lastcell = newcell;

                lctl->firstvisiblerow = 0;
            }
            /* insert next */
            header = header->next;
#if (defined WIN32 && defined UNICODE)
            tok = wcstok_s(0, TEXUTEXT("\t"), &pszContext);
#else
            tok = strtok(0, "\t");
#endif
            ++i;
        }
        /* all items count */
        ++lctl->nitems;

    } /* have header */
    return lctl->nitems;
}

texu_i32
_TexuListCtrlProc_GetVisibleWidth(texu_lcwnd *lctl)
{
    texu_i32 width = 0;
    texu_lcwnd_header *header = lctl->firstvisiblehdr;

    /*find the width of visible columns*/
    while (header && header != lctl->lastvisiblehdr)
    {
        width += header->cols;
        header = header->next;
    }
    width += header->cols;
    return width;
}

#if (defined WIN32 && defined _WINDOWS)
void
_TexuListCtrlProc_FillCell(texu_wnd *wnd, texu_lcwnd *lctl,
    texu_cio *dc, texu_i32 row, texu_i32 col,
    texu_ui32 color, texu_ui32 bgcolor)
#else
void
_TexuListCtrlProc_FillCell(texu_wnd *wnd, texu_lcwnd *lctl,
                           texu_cio *dc, texu_i32 row, texu_i32 col,
                           texu_ui32 color)
#endif
{
    texu_lcwnd_cell *cell;
    texu_rect rccell;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_env *env = texu_wnd_get_env(wnd);

    cell = _TexuListCtrlProc_FindCellByIndex(lctl, col, row);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);

    memset(buf, 0, sizeof(buf));
    texu_memset(buf, TEXUTEXT(' '), rccell.cols);

#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(
        dc,
        rccell.y,
        rccell.x,
        buf,
        texu_cio_get_reverse(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, rccell.y, rccell.x, buf, color, bgcolor);
#else
    texu_cio_putstr_attr(
        dc,
        rccell.y,
        rccell.x,
        buf,
        texu_cio_get_color(dc, color));
#endif
#endif
}
#if (defined WIN32 && defined _WINDOWS)
void
_TexuListCtrlProc_FillRow(texu_wnd *wnd, texu_lcwnd *lctl,
                          texu_cio *dc, texu_i32 row,
                          texu_ui32 color, texu_ui32 bgcolor)
#else
void
_TexuListCtrlProc_FillRow(texu_wnd *wnd, texu_lcwnd *lctl,
                          texu_cio *dc, texu_i32 row,
                          texu_ui32 color)
#endif
{
    texu_lcwnd_header *header = lctl->firstvisiblehdr;
    texu_lcwnd_cell *cell;
    texu_rect rccell;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 width = _TexuListCtrlProc_GetVisibleWidth(lctl);
    texu_env *env = texu_wnd_get_env(wnd);

    memset(buf, 0, sizeof(buf));
    texu_memset(buf, TEXUTEXT(' '), width);

    cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, row);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);

#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(
        dc,
        rccell.y,
        rccell.x,
        buf,
        texu_cio_get_reverse(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, rccell.y, rccell.x, buf, color, bgcolor);
#else
    texu_cio_putstr_attr(
        dc,
        rccell.y,
        rccell.x,
        buf,
        texu_cio_get_color(dc, color));
#endif
#endif
}

#if (defined WIN32 && defined _WINDOWS)
void
_TexuListCtrlProc_DrawItem(texu_wnd *wnd,
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_ui32 bgcolor,
    texu_i32 align, texu_i32 isheader)
{
    _TexuListCtrlProc_DrawItem2(wnd,
        dc, rccell, caption,
        color, bgcolor,
        align, isheader, 0);
}
#else
void
_TexuListCtrlProc_DrawItem(
texu_cio *dc, texu_rect *rccell, const texu_char *caption,
texu_ui32 color, texu_i32 align, texu_i32 isheader)
{
    _TexuListCtrlProc_DrawItem2(
        dc, rccell, caption,
        color, align, isheader, 0);
}
#endif

#if (defined WIN32 && defined _WINDOWS)
void
_TexuListCtrlProc_DrawItem2(texu_wnd *wnd,
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_ui32 bgcolor,
    texu_i32 align, texu_i32 isheader, texu_i32 issel)
#else
void
_TexuListCtrlProc_DrawItem2(
    texu_cio *dc, texu_rect *rccell, const texu_char *caption,
    texu_ui32 color, texu_i32 align, texu_i32 isheader, texu_i32 issel)
#endif
{
    texu_i32 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 cols = rccell->cols;
    texu_i32 x = rccell->x;
    texu_i32 y = rccell->y;
    texu_char cap2[TEXU_MAX_WNDTEXT + 1];
    texu_env *env = texu_wnd_get_env(wnd);

    memset(buf, 0, sizeof(buf));
    cols = (isheader ? cols : cols - 0);
    len = texu_printf_alignment2(
            buf,
            caption,
            cols,
            align,
            TEXU_TRUE);

    /* is is a header */
    if (1 == isheader)
    {
        buf[0] = TEXT('[');
        if (len > 0)
        {
            buf[len - 1] = ']';
        }
    }
    else if (2 == isheader)
    {
        texu_strcpy(cap2, TEXT(" "));
        texu_strcat(cap2, caption);
        len = texu_printf_alignment2(
                buf,
                cap2,
                cols,
                align,
                TEXU_TRUE);
        buf[0] = '[';
        if (len > 0)
        {
            buf[len - 1] = ']';
        }
    }
    if (issel)
    {
#ifdef TEXU_CIO_COLOR_MONO
        texu_cio_putstr_attr(
            dc,
            y,
            x,
            buf,
            texu_cio_get_reverse(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
        texu_env_draw_text(env, y, x, buf, color, bgcolor);
#else
        texu_cio_putstr_attr(
            dc,
            y,
            x,
            buf,
            texu_cio_get_color(dc, color));
#endif
#endif
    }
    else
    {
#if (defined WIN32 && defined _WINDOWS)
        texu_env_draw_text(env, y, x, buf, color, bgcolor);
#else
        texu_cio_putstr_attr(
            dc,
            y,
            x,
            buf,
            texu_cio_get_color(dc, color));
#endif
    }
}

void
_TexuListCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *header = 0;
    texu_lcwnd_header *prevheader = 0;
    texu_i32 width = 0;
    texu_rect rcitem, rcwnd, rccell;
    texu_ui32 normcolor = 0;
    texu_ui32 selcolor = 0;
    texu_lcwnd_cell *visiblecell = 0;
    texu_i32 i = 0;
    texu_i32 forcewidth = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 border = 0;
    texu_i32 col = 0;
    texu_bool fDrewLastCol = TEXU_FALSE;
    texu_i32 itemwidth = 0;
    texu_i32 x, y;
    texu_env *env = texu_wnd_get_env(wnd);
#if (defined WIN32 && defined _WINDOWS)
    texu_ui32 normbg = 0;
    texu_ui32 selbg = 0;
#endif

    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

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
    if (!header)
    {
        return;
    }
    width = header->cols;
    col = _TexuListCtrlProc_GetColIdx(lctl, header);

    /* first column width must not be over window columns */
    if (width > rcwnd.cols)
    {
        width = rcwnd.cols - 1;
        forcewidth = 1;
    }

    while (header)
    {
        if (!fDrewLastCol && width > rcwnd.cols)
        {
            /*draw the last column*/
            itemwidth = rcwnd.cols - (width-header->cols);
            fDrewLastCol = TEXU_TRUE;
        }
        else if (width == rcwnd.cols)
        {
            /*ok to draw the last column*/
            /*draw the last column*/
            itemwidth = header->cols;
            fDrewLastCol = TEXU_TRUE;
        }
        else if (fDrewLastCol)
        {
            break; /*already drew the last column*/
        }
        else
        {
            itemwidth = header->cols;
        }
        rcitem.x = rcwnd.x + width - header->cols;
        rcitem.cols = itemwidth;
        if (rcitem.x < 0)
        {
            rcitem.x = rcwnd.x;
            if (1 == forcewidth)
            {
                rcitem.cols = width;
            }
            else
            {
                --rcitem.cols;
            }
        }

        normcolor = header->normcolor;
        selcolor = header->selcolor;
#if (defined WIN32 && defined _WINDOWS)
        normbg = header->normbg;
        selbg = header->selbg;
#endif
        /* draw header */
        if (!(style & TEXU_LCS_NOHEADER))
        {
            border = 1;
            if (style & TEXU_LCS_NOBORDER)
            {
                border = 0;
            }
            if (itemwidth == header->cols)
            {
#if (defined WIN32 && defined _WINDOWS)
                _TexuListCtrlProc_DrawItem(wnd, dc, &rcitem,
                                           header->caption,
                                           normcolor, normbg,
                                           TEXU_ALIGN_CENTER,
                                           border); /* 0=no border, 1=border */
#else
                _TexuListCtrlProc_DrawItem(wnd, dc, &rcitem,
                                           header->caption,
                                           normcolor,
                                           TEXU_ALIGN_CENTER,
                                           border); /* 0=no border, 1=border */
#endif
            }
            else
            {
                border = 2; /*column with extra 1 space*/
#if (defined WIN32 && defined _WINDOWS)
                _TexuListCtrlProc_DrawItem(wnd, dc, &rcitem,
                                           header->caption,
                                           normcolor, normbg,
                                           TEXU_ALIGN_LEFT,
                                           border); /* 0=no border, 1=border */
#else
                _TexuListCtrlProc_DrawItem(wnd, dc, &rcitem,
                                           header->caption,
                                           normcolor,
                                           TEXU_ALIGN_LEFT,
                                           border); /* 0=no border, 1=border */
#endif
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
                selcolor = visiblecell->selcolor;
#if (defined WIN32 && defined _WINDOWS)
                normbg = visiblecell->normbg;
                selbg = visiblecell->selbg;
#endif
                if (!(TEXU_LCS_NOSELECTION & style) && i == lctl->curselrow)
                {
                    normcolor = selcolor;
                    /* draw th item that it can be seen */
#if (defined WIN32 && defined _WINDOWS)
                    normbg = selbg;
                    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                                visiblecell->caption,
                                                normcolor, normbg,
                                                (itemwidth == header->cols ? header->align : TEXU_ALIGN_LEFT),
                                                0, 1);
#else
                    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                                visiblecell->caption,
                                                normcolor,
                                                (itemwidth == header->cols ? header->align : TEXU_ALIGN_LEFT),
                                                0, 1);
#endif
                }
                else
                {
                    /* draw th item that it can be seen */
#if (defined WIN32 && defined _WINDOWS)
                    _TexuListCtrlProc_DrawItem(wnd, dc, &rccell,
                                               visiblecell->caption,
                                               normcolor, normbg,
                                               (itemwidth == header->cols ? header->align : TEXU_ALIGN_LEFT),
                                               0);
#else
                    _TexuListCtrlProc_DrawItem(wnd, dc, &rccell,
                                               visiblecell->caption,
                                               normcolor,
                                               (itemwidth == header->cols ? header->align : TEXU_ALIGN_LEFT),
                                               0);
#endif
                }

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
        prevheader = header;
        header = header->next;
        if (header)
        {
            width += header->cols;
        }

        ++col;
    } /* while header */

    /* print arrow controls */
    if (visiblecell)
    {
        normcolor = visiblecell->normcolor;
#if (defined WIN32 && defined _WINDOWS)
        normbg = visiblecell->normbg;
#endif
    }
    else
    {
        normcolor = 0;
#if (defined WIN32 && defined _WINDOWS)
        normbg = RGB(192, 192, 192);
#endif
    }
    if (lctl->firsthdr != lctl->firstvisiblehdr)
    {
        if (!(style & TEXU_LCS_NOHEADER))
        {
#if (defined WIN32 && defined _WINDOWS)
            texu_env_draw_char(env, rcwnd.y, rcwnd.x + 1, TEXUTEXT('<'), normcolor, normbg);
#else
            texu_cio_putch_attr(dc, rcwnd.y, rcwnd.x + 1, '<', normcolor);
#endif
        }
    }
    /* save the last visible */
    if (header)
    {
        /*if (header == lctl->lasthdr)
        {
            lctl->lastvisiblehdr = header;
        }
        else
        {
            lctl->lastvisiblehdr = header->prev;
        }*/
        lctl->lastvisiblehdr = header->prev;
        if (!(style & TEXU_LCS_NOHEADER))
        {
            y = rcwnd.y;
            x = rcwnd.x + width - header->cols + itemwidth - 2;
            if (x >= rcwnd.cols)
            {
                x = rcwnd.cols - 1;
            }
#if (defined WIN32 && defined _WINDOWS)
            texu_env_draw_char(env, y, x, TEXUTEXT('>'), normcolor, normbg);
#else
            texu_cio_putch_attr(dc, y, x, '>', normcolor);
#endif
        }
    }
    else if (prevheader /*&& prevheader->next*/)
    {
        lctl->lastvisiblehdr = prevheader;/* ->prev;*/
        if (!(style & TEXU_LCS_NOHEADER))
        {
            y = rcwnd.y;
            x = rcwnd.x + width - prevheader->cols + itemwidth - 2;
            if (x >= rcwnd.cols)
            {
                x = rcwnd.cols - 1;
#if (defined WIN32 && defined _WINDOWS)
                texu_env_draw_char(env, y, x, TEXUTEXT('>'), normcolor, normbg);
#else
                texu_cio_putch_attr(dc, y, x, '>', normcolor);
#endif
            }
        }
    }

    texu_cio_gotoyx(dc, rcitem.y,
                    rcitem.x);
}

void _TexuListCtrlProc_OnBeginMoving(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;
    texu_rect rccell;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_header *header = 0;
    texu_i32 curselcol = -1;
    texu_cio *dc = texu_wnd_get_cio(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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
    cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);
#if (defined WIN32 && defined _WINDOWS)
    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                cell->caption,
                                cell->normcolor, cell->normbg,
                                header->align, 0, 1);
#else
    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                cell->caption,
                                cell->normcolor,
                                header->align, 0, 1);
#endif

    /* move cursor */
    texu_cio_gotoyx(dc, rccell.y, rccell.x);

    /* save the editing cell */
    lctl->editingcell = cell;
    /* send notification */
    _TexuWndProc_Notify(wnd, TEXU_LCN_BEGINMOVING);
}

void _TexuListCtrlProc_OnMovingCursor(texu_wnd *wnd, texu_i32 ch)
{
    texu_lcwnd *lctl = 0;
    texu_i32 firstcol = -1, lastcol = -1;
    texu_i32 newcol = -1;
    texu_i32 newrow = -1;
    texu_rect rccell;
    texu_rect rcwnd;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_header *header = 0;
    texu_i32 maxrows = 25;
    texu_ui32 style = texu_wnd_get_style(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (lctl->movingstate != TEXU_LCT_MOVINGCURSOR)
    {
        return;
    }

    texu_wnd_get_rect(wnd, &rcwnd);
    firstcol = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr);
    lastcol = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->lastvisiblehdr);
    newcol = lctl->curselcol;
    newrow = lctl->curselrow;

    maxrows = lctl->firstvisiblerow + rcwnd.lines - 2;

    switch (ch)
    {
        case TEXU_KEY_LEFT:
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

        case TEXU_KEY_RIGHT:
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

        case TEXU_KEY_DOWN:
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

        case TEXU_KEY_UP:
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
        cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
        _TexuListCtrlProc_GetCellRect(cell, &rccell);
#if (defined WIN32 && defined _WINDOWS)
        _TexuListCtrlProc_DrawItem(
            wnd,
            texu_wnd_get_cio(wnd),
            &rccell,
            cell->caption,
            cell->normcolor, cell->normbg,
            header->align,
            0);
#else
        _TexuListCtrlProc_DrawItem(
            wnd,
            texu_wnd_get_cio(wnd),
            &rccell,
            cell->caption,
            cell->normcolor,
            header->align,
            0);
#endif

        /* draw moving cursor */
        header = _TexuListCtrlProc_FindHeaderByIndex(lctl, newcol);
        cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, newrow);
        _TexuListCtrlProc_GetCellRect(cell, &rccell);
#if (defined WIN32 && defined _WINDOWS)
        _TexuListCtrlProc_DrawItem(wnd, texu_wnd_get_cio(wnd),
                                   &rccell,
                                   cell->caption,
                                   cell->normcolor, cell->normbg,
                                   header->align,
                                   0);
#else
        _TexuListCtrlProc_DrawItem(wnd, texu_wnd_get_cio(wnd),
                                   &rccell,
                                   cell->caption,
                                   cell->normcolor,
                                   header->align,
                                   0);
#endif

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

void _TexuListCtrlProc_OnEndMoving(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_header *header = 0;
    texu_rect rccell;
    texu_cio *dc = texu_wnd_get_cio(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (lctl->movingstate != TEXU_LCT_MOVINGCURSOR)
    {
        return;
    }
    lctl->movingstate = TEXU_LCT_ENDMOVING;

    /* redraw the previous moving cursor */
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
    cell = _TexuListCtrlProc_FindCellByHeader(lctl, header, lctl->curselrow);
    _TexuListCtrlProc_GetCellRect(cell, &rccell);

#if (defined WIN32 && defined _WINDOWS)
    _TexuListCtrlProc_DrawItem2(
        wnd,
        dc,
        &rccell,
        cell->caption,
        cell->normcolor, cell->normbg,
        header->align,
        0,
        1);
#else
    _TexuListCtrlProc_DrawItem2(
        wnd,
        dc,
        &rccell,
        cell->caption,
        cell->normcolor,
        header->align,
        0,
        1);
#endif

    /* update state */
    lctl->movingstate = TEXU_LCT_VIEW;

    texu_wnd_invalidate(wnd);
    /* send notification */

    _TexuWndProc_Notify(wnd, TEXU_LCN_ENDMOVING);
}

void _TexuListCtrlProc_OnBeginInsertRow(texu_wnd *wnd, texu_i32 row)
{
    texu_lcwnd *lctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (row < 0)
    {
        row = 0;
    }
    /* save all cells on the row */
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    lctl->insertingstate = TEXU_LCT_INSERTING;

    /* send notification */

    _TexuListCtrlProc_NotifyItem(wnd, TEXU_LCN_BEGININSERTROW, lctl->curselrow, lctl->curselcol);
}

void _TexuListCtrlProc_OnEndInsertRow(texu_wnd *wnd, texu_i32 row, texu_i32 ok)
{
    texu_lcwnd *lctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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

void _TexuListCtrlProc_OnBeginEditRow(texu_wnd *wnd, texu_i32 row)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_cell *newcell = 0;
    texu_i32 i = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (row < 0)
    {
        row = 0;
    }
    /* save all cells on the row */
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

    lctl->firsteditcell = lctl->lasteditcell = 0;

    for (i = 0; i < lctl->nheaders; ++i)
    {
        cell = _TexuListCtrlProc_FindCellByIndex(lctl, i, row);
        newcell = (texu_lcwnd_cell *)malloc(sizeof(texu_lcwnd_cell));
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

void _TexuListCtrlProc_OnEndEditRow(texu_wnd *wnd, texu_i32 row, texu_i32 ok)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_cell *nextcell = 0;
    texu_i32 i = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (TEXU_LC_ENDEDITCANCEL == ok)
    {
        /* rollback */
        nextcell = lctl->firsteditcell;
        for (i = 0; i < lctl->nheaders && nextcell != 0; ++i)
        {
            cell = _TexuListCtrlProc_FindCellByIndex(lctl, i, row);
            texu_strcpy(cell->caption, nextcell->caption);
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
_TexuListCtrlProc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_i32 row, texu_i32 col)
{
    texu_header_notify nmhdri;
    /* send notification */
    nmhdri.hdr.id = texu_wnd_get_id(wnd);
    nmhdri.hdr.wnd = wnd;
    nmhdri.hdr.code = code;
    nmhdri.row = row;
    nmhdri.col = col;

    texu_wnd_send_msg(texu_wnd_get_parent(wnd), TEXU_WM_NOTIFY, (texu_i64)&nmhdri, 0);
}

void _TexuListCtrlProc_OnBeginEdit(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;
    texu_rect rccell;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 editstyle = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (!(texu_wnd_get_style(wnd) & TEXU_LCS_EDITABLE))
    {
        /* if it has no EDIT style */
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

    lctl->editingstate = TEXU_LCT_BEGINEDITING;
    /* show edit box at the current row and column */
    _TexuListCtrlProc_GetCellRect(lctl->editingcell, &rccell);
    texu_wnd_send_msg(lctl->editbox, TEXU_WM_KILLFOCUS, 0, 0);

    texu_wnd_move(lctl->editbox, rccell.y, rccell.x, rccell.lines, rccell.cols, TEXU_TRUE);

    editstyle = _TexuListCtrlProc_OnGetEditStyle(wnd, lctl->curselcol);

    texu_wnd_set_style(lctl->editbox, editstyle);
    if (editstyle & TEXU_ES_DECIMAL)
    {
        texu_lcwnd_header *header = _TexuListCtrlProc_FindHeaderByIndex(lctl, lctl->curselcol);
        texu_wnd_send_msg(lctl->editbox, TEXU_EM_SETDECWIDTH, header->decwidth, 0);
    }

    /* update text before showing */
    texu_wnd_visible(lctl->editbox, TEXU_TRUE);
    texu_wnd_enable(lctl->editbox, TEXU_TRUE);
    texu_strcpy(buf, lctl->editingcell->caption);
    texu_wnd_set_text(lctl->editbox, buf);

    /*TuiSetFocus(lctl->editbox);*/
    texu_wnd_send_msg(lctl->editbox, TEXU_WM_SETFOCUS, 0, 0);

    /* update state */
    lctl->editingstate = TEXU_LCT_EDITING;

    _TexuListCtrlProc_NotifyItem(wnd, TEXU_LCN_BEGINEDIT, lctl->curselrow, lctl->curselcol);
    texu_wnd_invalidate(wnd);
    texu_wnd_invalidate(lctl->editbox);
}

void _TexuListCtrlProc_OnEndEdit(texu_wnd *wnd, texu_i32 ok)
{
    texu_lcwnd *lctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 rc = TEXU_CONTINUE;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (!(texu_wnd_get_style(wnd) & TEXU_LCS_EDITABLE))
    {
        /* if it has no EDIT style */
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (lctl->editingstate != TEXU_LCT_EDITING)
    {
        return;
    }
    lctl->editingstate = TEXU_LCT_ENDEDITING;

    /* hide edit box */
    rc = texu_wnd_send_msg(lctl->editbox, TEXU_WM_KILLFOCUS, 0, 0);
    texu_wnd_visible(lctl->editbox, TEXU_FALSE);
    if (TEXU_LC_ENDEDITOK == ok)
    {
        if (TEXU_CONTINUE == rc)
        {
            /* update */
            texu_wnd_get_text(lctl->editbox, buf, TEXU_MAX_WNDTEXT);
            texu_strcpy(lctl->editingcell->caption, buf);
        }
    }

    /* send notification */

    _TexuListCtrlProc_NotifyItem(wnd,
                                 (TEXU_LC_ENDEDITOK == ok ? TEXU_LCN_ENDEDITOK : TEXU_LCN_ENDEDITCANCEL),
                                 lctl->curselrow, lctl->curselcol);

    texu_wnd_invalidate(wnd);
}

void _TexuListCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_lcwnd *lctl = 0;
    texu_i32 repaint = 0;
    texu_rect rc;
    texu_i32 lines = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 row = 0;
    texu_i32 col = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_rect(wnd, &rc);

    if (lctl->nheaders <= 0)
    {
        return;
    }
    /*ensure the row is available*/
    row = lctl->curselrow;
    if (row < 0)
    {
        row = 0;
    }
    
    /*find the first column*/
    col = lctl->curselcol;

#if (defined WIN32 && defined _WINDOWS)
    if (lctl->nitems > 0 && (VK_RETURN == ch || VK_F2 == ch || ch == lctl->editkey) && (style & TEXU_LCS_EDITABLE))
#else
    if (lctl->nitems > 0 && ch == lctl->editkey && (style & TEXU_LCS_EDITABLE))
#endif
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
            lctl->curedtcol = _TexuListCtrlProc_GetFirstEditableCol(lctl); /*_TexuListCtrlProc_GetNextEditableCol(lctl, col); */
            lctl->curselcol = lctl->curedtcol;
            _TexuListCtrlProc_OnBeginMoving(wnd);
            _TexuListCtrlProc_OnBeginEditRow(wnd, row);
            _TexuListCtrlProc_OnBeginEdit(wnd); /* begin editing */
            return;
        }
    }
    else if (lctl->nitems > 0 && ch == lctl->canceleditkey && (style & TEXU_LCS_EDITABLE))
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
            _TexuListCtrlProc_OnSetCurRow(wnd, row - 1);
        }
        return;
    }
    else if (lctl->nitems > 0 && ch == lctl->insertkey && (style & TEXU_LCS_EDITABLE))
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
            _TexuListCtrlProc_OnSetCurRow(wnd, row - 1);
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
    else if (lctl->nitems > 0 && ch == lctl->delkey)
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
        case TEXU_KEY_RIGHT:
        {
            if (lctl->editingstate == TEXU_LCT_EDITING)
            {
                _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
                _TexuListCtrlProc_OnMovingCursor(wnd, ch);
                _TexuListCtrlProc_OnBeginEdit(wnd);
            }
            else if (lctl->lastvisiblehdr != lctl->lasthdr)
            {
                if (lctl->firstvisiblehdr && lctl->firstvisiblehdr->next)
                {
                    lctl->firstvisiblehdr = lctl->firstvisiblehdr->next;
                    ++repaint;
                }
            }
            break;
        }

        case TEXU_KEY_LEFT:
        {
            if (lctl->editingstate == TEXU_LCT_EDITING)
            {
                _TexuListCtrlProc_OnEndEdit(wnd, TEXU_LC_ENDEDITOK);
                _TexuListCtrlProc_OnMovingCursor(wnd, ch);
                _TexuListCtrlProc_OnBeginEdit(wnd);
            }
            else if (lctl->firstvisiblehdr != lctl->firsthdr)
            {
                if (lctl->firstvisiblehdr && lctl->firstvisiblehdr->prev)
                {
                    lctl->firstvisiblehdr = lctl->firstvisiblehdr->prev;
                    ++repaint;
                }
            }
            break;
        }

        case TEXU_KEY_DOWN:
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

        case TEXU_KEY_UP:
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

        case TEXU_KEY_PPAGE:
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

        case TEXU_KEY_NPAGE:
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
            /*put any char to editbox if it is available*/
            if (TEXU_LCT_EDITING == lctl->editingstate)
            {
                texu_wnd_send_msg(lctl->editbox, TEXU_WM_CHAR, ch, alt);
            }
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

texu_i32 _TexuListCtrlProc_GetFirstEditableCol(texu_lcwnd *lctl)
{
    texu_i32 col = _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr);
    return _TexuListCtrlProc_GetNextEditableCol(lctl, col - 1);
}

texu_i32 _TexuListCtrlProc_GetNextEditableCol(texu_lcwnd *lctl, texu_i32 prevcol)
{
    texu_i32 col = -1;
    texu_i32 i = 0;
    for (i = prevcol + 1; i < lctl->nheaders; ++i)
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

texu_i32 _TexuListCtrlProc_InsertEmptyItem(texu_wnd *wnd)
{
    texu_char sz[TEXU_MAX_WNDTEXT + 1];
    texu_lcwnd *lctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    texu_memset(sz, TEXUTEXT('\t'), lctl->nheaders);
    sz[lctl->nheaders] = 0;
    return _TexuListCtrlProc_OnAddItems(wnd, sz, lctl->nheaders);
}

texu_i32 _TexuListCtrlProc_GetLastEditableCol(texu_lcwnd *lctl)
{
    return _TexuListCtrlProc_GetPrevEditableCol(lctl, lctl->nheaders);
}

texu_i32 _TexuListCtrlProc_GetPrevEditableCol(texu_lcwnd *lctl, texu_i32 prevcol)
{
    texu_i32 col = -1;
    texu_i32 i = 0;
    for (i = prevcol - 1; i >= 0; --i)
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

void _TexuListCtrlProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch)
{
    texu_lcwnd *lctl = 0;
    texu_i32 repaint = 0;
    texu_rect rc;
    texu_i32 lines = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 row = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
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
            _TexuListCtrlProc_OnSetCurRow(wnd, row - 1);
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
            _TexuListCtrlProc_OnSetCurRow(wnd, row - 1);
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
    case TEXU_KEY_RIGHT:
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

    case TEXU_KEY_LEFT:
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

    case TEXU_KEY_DOWN:
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

    case TEXU_KEY_UP:
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

    case TEXU_KEY_PPAGE:
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

    case TEXU_KEY_NPAGE:
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

texu_i32 _TexuListCtrlProc_OnGetItemCount(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    return lctl->nitems;
}

texu_i32 _TexuListCtrlProc_OnSetItem(texu_wnd *wnd, texu_ui32 flags, texu_wnd_subitem *subitem)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_i32 rc = TEXU_ERROR;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    cell = _TexuListCtrlProc_FindCellByIndex(lctl, subitem->col, subitem->idx);
    if (cell)
    {
        if (flags & TEXU_LCFM_TEXT)
        {
            texu_strcpy(cell->caption, subitem->text);
        }
        if (flags & TEXU_LCFM_DATA)
        {
            cell->data = subitem->data;
        }
        if (flags & TEXU_LCFM_COLOR)
        {
            cell->normcolor = subitem->normcolor;
            cell->discolor  = subitem->discolor;
            cell->selcolor  = subitem->selcolor;
#if (defined WIN32 && defined _WINDOWS)
            cell->normbg = subitem->normbg;
            cell->disbg  = subitem->disbg;
            cell->selbg  = subitem->selbg;
#endif
        }
        rc = TEXU_OK;
        texu_wnd_invalidate(wnd);
    }
    return rc;
}

texu_i32 _TexuListCtrlProc_OnGetItem(texu_wnd *wnd, texu_ui32 flags, texu_wnd_subitem *subitem)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_i32 rc = TEXU_ERROR;

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    cell = _TexuListCtrlProc_FindCellByIndex(lctl, subitem->col, subitem->idx);
    if (cell)
    {
        if (flags & TEXU_LCFM_TEXT)
        {
            subitem->text = cell->caption;
        }
        if (flags & TEXU_LCFM_DATA)
        {
            subitem->data = cell->data;
        }
        if (flags & TEXU_LCFM_COLOR)
        {
            subitem->normcolor = cell->normcolor;
            subitem->discolor = cell->discolor;
            subitem->selcolor = cell->selcolor;
        }

        rc = TEXU_OK;
    }
    return rc;
}

void _TexuListCtrlProc_OnInvalidateItem(texu_wnd *wnd, texu_i32 row, texu_i32 col)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_cell *cell = 0;
    texu_lcwnd_header *header = 0;
    texu_rect rcwnd;
    texu_rect rccell;
    texu_i32 color = 0;
    texu_cio *dc = texu_wnd_get_cio(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
#if (defined WIN32 && defined _WINDOWS)
    texu_i32 bgcolor = 0;
#endif

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    cell = _TexuListCtrlProc_FindCellByIndex(lctl, col, row);
    if (cell)
    {
        /* check if the cell is available on screen */
        texu_wnd_get_rect(wnd, &rcwnd);
        if (row >= lctl->firstvisiblerow &&
            row < lctl->firstvisiblerow + rcwnd.lines)
        {
            if (col >= _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->firstvisiblehdr) &&
                col < _TexuListCtrlProc_FindHeaderIndex(lctl, lctl->lastvisiblehdr))
            {
                header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
                _TexuListCtrlProc_GetCellRect(cell, &rccell);

                color = cell->normcolor;
#if (defined WIN32 && defined _WINDOWS)
                bgcolor = cell->normbg;
#endif
                if (row == lctl->curselrow)
                {
                    color = cell->selcolor;
#if (defined WIN32 && defined _WINDOWS)
                    bgcolor = cell->selbg;
#endif
                    /* draw th item that it can be seen */
#if (defined WIN32 && defined _WINDOWS)
                    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                                cell->caption, color, bgcolor, header->align, 0, 1);
#else
                    _TexuListCtrlProc_DrawItem2(wnd, dc, &rccell,
                                                cell->caption, color, header->align, 0, 1);
#endif
                }
                else
                {
                    /* draw th item that it can be seen */
#if (defined WIN32 && defined _WINDOWS)
                    _TexuListCtrlProc_DrawItem(wnd, texu_wnd_get_cio(wnd), &rccell,
                                               cell->caption, color, bgcolor, header->align, 0);
#else
                    _TexuListCtrlProc_DrawItem(wnd, texu_wnd_get_cio(wnd), &rccell,
                                               cell->caption, color, header->align, 0);
#endif
                }
            }
        }
    }
}

void _TexuListCtrlProc_OnSetEditStyle(texu_wnd *wnd, texu_i32 col, texu_ui32 editstyle)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *header = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
    if (header)
    {
        header->editstyle = editstyle;
    }
}

texu_ui32 _TexuListCtrlProc_OnGetEditStyle(texu_wnd *wnd, texu_i32 col)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *header = 0;
    texu_ui32 editstyle = 0;

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
    if (header)
    {
        editstyle = header->editstyle;
    }
    return editstyle;
}

void _TexuListCtrlProc_OnSetCurRow(texu_wnd *wnd, texu_i32 idx)
{
    texu_lcwnd *lctl = 0;
    texu_rect rcwnd;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    if (idx < 0 || idx >= lctl->nitems)
    {
        if (lctl->nitems <= 0)
        {
            lctl->curselrow = -1;
        }
        else
        {
            lctl->curselrow = lctl->nitems - 1;
        }
        texu_wnd_invalidate(wnd);
        return;
    }

    /* find the new appropriate first visible item */
    texu_wnd_get_rect(wnd, &rcwnd);
    if (idx >= lctl->firstvisiblerow &&
        idx < lctl->firstvisiblerow + rcwnd.lines)
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

texu_i32 _TexuListCtrlProc_OnGetCurRow(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);

    return lctl->curselrow;
}

void _TexuListCtrlProc_OnSetColWidth(texu_wnd *wnd, texu_i32 col, texu_i32 width)
{
    texu_lcwnd *lctl = 0;
    texu_lcwnd_header *header = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    header = _TexuListCtrlProc_FindHeaderByIndex(lctl, col);
    if (header)
    {
        header->cols = width;
    }
}

texu_wnd *_TexuListCtrlProc_OnGetEditBox(texu_wnd *wnd)
{
    texu_lcwnd *lctl = 0;

    lctl = (texu_lcwnd *)texu_wnd_get_userdata(wnd);
    return lctl->editbox;
}

void _TexuListCtrlProc_OnSetCurPage(texu_wnd *wnd, texu_i32 npage)
{
    texu_i32 idx = 0;
    texu_i32 nitemspp = _TexuListCtrlProc_OnGetItemsPerPage(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    idx = (npage * nitemspp);
    texu_wnd_send_msg(wnd, TEXU_LCM_SETCURROW, (texu_i64)idx, 0);
}

texu_i32 _TexuListCtrlProc_OnGetCurPage(texu_wnd *wnd)
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

texu_i32 _TexuListCtrlProc_OnGetItemsPerPage(texu_wnd *wnd)
{
    texu_rect rcwnd;
    texu_wnd_get_rect(wnd, &rcwnd);
    return (rcwnd.lines > 1 ? rcwnd.lines - 1 : 0);
}

texu_i32
_TexuListCtrlProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_i32 curselrow = _TexuListCtrlProc_OnGetCurRow(wnd);
    texu_i32 nitems = _TexuListCtrlProc_OnGetItemCount(wnd);
    texu_wnd_subitem subitem;
    texu_i32 len = 0;
    
    if (curselrow > -1)
    {
        subitem.idx  = curselrow;
        subitem.col  = 0;
        subitem.text = text;
        _TexuListCtrlProc_OnGetItem(wnd, TEXU_LCFM_TEXT, &subitem);
    }
    else if (nitems <= 0)
    {
        texu_strcpy(text, TEXT(""));
    }
    
    len = texu_strlen(text);
    if (len > 0 && textlen < len)
    {
        text[textlen-1] = 0;
    }
    return texu_strlen(text);
}

void
_TexuListCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_i32 curselrow = _TexuListCtrlProc_OnGetCurRow(wnd);
    texu_wnd_subitem subitem;
    
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (curselrow > -1)
    {
        subitem.idx  = curselrow;
        subitem.col  = 0;
        subitem.text = (texu_char*)text;
        _TexuListCtrlProc_OnSetItem(wnd, TEXU_LCFM_TEXT, &subitem);
    }
}

texu_i64
_TexuListCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_CHAR:
            _TexuListCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuListCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuListCtrlProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuListCtrlProc_OnDestroy(wnd);
            break;

        case TEXU_WM_SETFOCUS:
            _TexuListCtrlProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuListCtrlProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_GETTEXT:
            return _TexuListCtrlProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

        case TEXU_WM_SETTEXT:
            _TexuListCtrlProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_LCM_ADDCOLUMN:
        {
            return _TexuListCtrlProc_OnAddColumn(wnd, (texu_wnd_header *)param1);
        }
        case TEXU_LCM_DELETECOLUMN:
        {
            _TexuListCtrlProc_OnDeleteColumn(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_LCM_DELETEALLCOLUMNS:
        {
            _TexuListCtrlProc_OnDeleteAllColumns(wnd);
            return 0;
        }
        case TEXU_LCM_ADDITEM:
        {
            return _TexuListCtrlProc_OnAddItem(wnd, (const texu_char *)param1);
        }
        case TEXU_LCM_ADDITEMS:
        {
            return _TexuListCtrlProc_OnAddItems(wnd, (texu_char *)param1, (texu_i32)param2);
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
            return _TexuListCtrlProc_OnSetItem(wnd, (texu_ui32)param1, (texu_wnd_subitem *)param2);
        }
        case TEXU_LCM_GETITEM:
        {
            return _TexuListCtrlProc_OnGetItem(wnd, (texu_ui32)param1, (texu_wnd_subitem *)param2);
        }
        case TEXU_LCM_GETITEMCOUNT:
        {
            return _TexuListCtrlProc_OnGetItemCount(wnd);
        }
        case TEXU_LCM_INVALIDATEITEM:
        {
            _TexuListCtrlProc_OnInvalidateItem(wnd, (texu_i32)param1, (texu_i32)param2);
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
            _TexuListCtrlProc_OnSetEditableCols(wnd, (texu_i32)param1, (const texu_i32 *)param2);
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
    texu_tree *tree;                  /* see also texulib.h */
    texu_tree_find_proc findproc;     /* to compare item */
    texu_tree_item *firstvisibleitem; /* the first item is visible in the tree control */
    texu_tree_item *lastvisibleitem;  /* the first item is visible in the tree control */
    texu_tree_item *curselitem;       /* the current selected item */
    texu_list *visibleitems;
    texu_i32 indent;
    texu_i32 shifted_right;
    texu_i32 shifted;
    texu_ui32 normcolor;
    texu_ui32 discolor;
    texu_ui32 selcolor;
    texu_ui32 focuscolor;
#if (defined WIN32 && defined _WINDOWS)
    texu_ui32 normbg;
    texu_ui32 disbg;
    texu_ui32 selbg;
    texu_ui32 focusbg;
#endif
    void *userdata_findproc;
    void *exparam;
    texu_wnd *editbox;
};
typedef struct texu_treewnd texu_treewnd;

void _TexuTreeProc_Notify(texu_wnd *wnd, texu_ui32, texu_tree_item *);

void _TexuTreeCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_tree_item *_TexuTreeCtrlProc_OnInsertItem(texu_wnd *wnd, texu_tree_item *parentitem, const texu_treewnd_item *insertitem);
texu_i32 _TexuTreeCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *);
texu_i32 _TexuTreeCtrlProc_OnDeleteItem(texu_wnd *wnd, texu_tree_item *item);
texu_i32 _TexuTreeCtrlProc_OnSetItem(texu_wnd *wnd, texu_tree_item *item, const texu_treewnd_item *setitem);
texu_i32 _TexuTreeCtrlProc_OnGetItem(texu_wnd *wnd, texu_tree_item *item, texu_treewnd_item *getitem);
texu_tree_item *_TexuTreeCtrlProc_OnFindItem(texu_wnd *wnd, texu_treewnd_item *finditem, void *userdata);
texu_tree_item *_TexuTreeCtrlProc_OnFindNextItem(texu_wnd *wnd, texu_tree_item *previtem, texu_treewnd_item *finditem);
texu_i32 _TexuTreeCtrlProc_OnExpandItem(texu_wnd *wnd, texu_tree_item *item);
texu_i32 _TexuTreeCtrlProc_OnCollapseItem(texu_wnd *wnd, texu_tree_item *item);
void _TexuTreeCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void _TexuTreeCtrlProc_OnDestroy(texu_wnd *wnd);
void _TexuTreeCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *);
texu_i32 _TexuTreeCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *);
texu_tree_find_proc _TexuTreeCtrlProc_OnSetFindItemProc(texu_wnd *wnd, texu_tree_find_proc findproc);
texu_i32 _TexuTreeCtrlProc_OnSetSelItem(texu_wnd *wnd, texu_tree_item *setitem);
texu_tree_item *_TexuTreeCtrlProc_OnGetSelItem(texu_wnd *wnd);
texu_i32 _TexuTreeCtrlProc_OnExportToFile(texu_wnd *wnd, FILE *fp, texu_tree_exp_proc prnproc);
texu_i32 _TexuTreeCtrlProc_OnImportFromFile(texu_wnd *wnd, FILE *fp, texu_tree_imp_proc impproc);
void _TexuTreeCtrlProc_OnExpandAllItems(texu_wnd *wnd);
void _TexuTreeCtrlProc_OnCollapseAllItems(texu_wnd *wnd);
texu_i32 _TexuTreeCtrlProc_OnGetIndentText(texu_wnd *wnd);
void _TexuTreeCtrlProc_OnSetIndentText(texu_wnd *wnd, texu_i32 indent);
texu_i32 _TexuTreeCtrlProc_OnGetShiftedText(texu_wnd *wnd);
void _TexuTreeCtrlProc_OnSetShiftedText(texu_wnd *wnd, texu_i32 shifted);
texu_i32    _TexuTreeCtrlProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuTreeCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text);

void _TexuTreeCtrlProc_RefreshView(texu_wnd *wnd);
texu_i32 _TexuTreeCtrlProc_DefFindItemProc(texu_i64, texu_i64, void *);

void _TexuTreeCtrlProc_GetDisplayedText(
    texu_wnd *wnd,
    texu_char *outtext, texu_i32 *startx, texu_i32 *endx,
    texu_tree_item *item,
    texu_i32 maxlen,
    texu_bool file);

void _TexuTreeCtrlProc_AdjustVisibleItems(texu_wnd *wnd);
texu_tree_item *_TexuTreeCtrlProc_MoveNext(texu_wnd *wnd, texu_i32 times);
texu_tree_item *_TexuTreeCtrlProc_MovePrev(texu_wnd *wnd, texu_i32 times);

struct texu_treeview_item
{
    texu_tree_item *item;
};
typedef struct texu_treeview_item texu_treeview_item;

texu_i64 _TexuTreeCtrlProc_PreorderTraverseProc(texu_tree_item *, void *);
texu_i64 _TexuTreeCtrlProc_ExpandAllItemsProc(texu_tree_item *, void *);
texu_i64 _TexuTreeCtrlProc_CollapseAllItemsProc(texu_tree_item *, void *);

/* helper functions */
texu_treeview_item *_texu_treeview_item_new(const texu_tree_item *item);
void _texu_treeview_item_del(texu_i64 data, void *userdata);
texu_treewnd_item *_texu_treewnd_item_new(const texu_treewnd_item *item);
void _texu_treewnd_item_del(texu_i64 data, void *userdata);

void
_TexuTreeProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_tree_item *item)
{
    texu_treewnd_notify notify;
    texu_wnd *parent = texu_wnd_get_parent(wnd);

    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.item = item;
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
}

texu_i32
_TexuTreeCtrlProc_DefFindItemProc(texu_i64 d1, texu_i64 d2, void *userdata)
{
    /* this function compare the item text */
    texu_treewnd_item *dataitem = (texu_treewnd_item *)d1;
    texu_treewnd_item *treeitem = (texu_treewnd_item *)d2;
    return memcmp(dataitem->itemtext, treeitem->itemtext, TEXU_MAX_WNDTEXT);
}

texu_treeview_item *
_texu_treeview_item_new(const texu_tree_item *item)
{
    texu_treeview_item *view = (texu_treeview_item *)malloc(sizeof(texu_treeview_item));
    if (view)
    {
        view->item = (texu_tree_item *)item;
    }
    return view;
}

void _texu_treeview_item_del(texu_i64 data, void *userdata)
{
    texu_treeview_item *view = (texu_treeview_item *)data;
    free(view);
}

texu_i32 _TexuTreeCtrlProc_OnImportFromFile(texu_wnd *wnd, FILE *fp, texu_tree_imp_proc proc)
{
    texu_char buf[BUFSIZ + 1];
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;
    texu_treeview_item *view = 0;
    texu_stack *stack = 0;
    texu_tree_item *parent = 0;
    texu_i32 tabs = 0;
    texu_char *psz = 0;
    texu_i32 items = 0;
    texu_tree_item *newitem = 0;
    texu_i32 len = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    if (!fp)
    {
        /* nothing to do */
        return -1;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    parent = texu_tree_get_root(tc->tree);

    /**/
    stack = texu_stack_new(512);
    view = _texu_treeview_item_new(parent);
    texu_stack_push(stack, (texu_i64)view);

    while (!feof(fp))
    {
#if (defined WIN32 && defined UNICODE)
        if (!fgetws(buf, BUFSIZ, fp))
#else
        if (!fgets(buf, BUFSIZ, fp))
#endif
        {
            break;
        }
        /* comment */
        if (buf[0] == 0 || buf[0] == TEXT('#') || buf[0] == TEXT('\n'))
        {
            continue;
        }
        /* look up the parent item */
        tabs = 0;
        psz = buf;
        while (psz)
        {
            if (*psz == TEXT('\t'))
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
        len = texu_strlen(psz);
        if (len > 0 && psz[len - 1] == TEXT('\n'))
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

            view = (texu_treeview_item *)texu_stack_top(stack);
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
                view = (texu_treeview_item *)texu_stack_top(stack);
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
                view = (texu_treeview_item *)texu_stack_top(stack);
                parent = view->item;
            }
        }
        /* format text or data */
        memset(&data, 0, sizeof(data));
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
            texu_strcpy(data->itemtext, psz);
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

texu_i32 _TexuTreeCtrlProc_OnExportToFile(texu_wnd *wnd, FILE *fp, texu_tree_exp_proc prnproc)
{
    texu_queue *queue = 0;
    texu_char buf[BUFSIZ + 1];
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;
    texu_treeview_item *view = 0;
    texu_tree_item *root = 0;
    texu_i32 startx = 0, endx = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    if (!fp)
    {
        /* nothing to do */
        return -1;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

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

        view = (texu_treeview_item *)texu_queue_first(queue);

        texu_queue_dequeue(queue);

        data = (texu_treewnd_item *)view->item->data;

        memset(buf, 0, sizeof(buf));
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

void _TexuTreeCtrlProc_AdjustVisibleItems(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;
    texu_treeview_item *view = 0;
    texu_tree_item *root = 0;
    texu_tree_item *next_item = 0;
    texu_queue *queue;
    texu_queue *subqueue;
    texu_list *items = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

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
        view = (texu_treeview_item *)texu_queue_first(queue);

        texu_queue_dequeue(queue);

        texu_list_add(items, (texu_i64)view);

        /* selected item */
        data = (texu_treewnd_item *)view->item->data;

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
                view = (texu_treeview_item *)texu_queue_first(subqueue);
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

void _TexuTreeCtrlProc_RefreshView(texu_wnd *wnd)
{
    /* refresh update if the new item or deleting item is in the visible bound */
    texu_wnd_invalidate(wnd);
}

texu_tree_item *_TexuTreeCtrlProc_MoveNext(texu_wnd *wnd, texu_i32 move_times)
{
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;
    texu_list_item *iter = 0;
    texu_treeview_item *view = 0;
    texu_i32 i = 0;
    texu_i32 items = 0;
    texu_i32 firstindex = 0;
    texu_i32 curindex = 0;
    texu_list_item *first_iter = 0;
    texu_list_item *cur_iter = 0;
    texu_rect rc;
    texu_i32 first_moves = 0;
    texu_i32 cur_moves = 0;
    texu_bool movefirst = TEXU_TRUE;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    items = texu_list_count(tc->visibleitems);

    /* change selection item */
    iter = texu_list_first(tc->visibleitems);
    /* find the first index */
    while (iter)
    {
        view = (texu_treeview_item *)iter->data;
        data = (texu_treewnd_item *)view->item->data;

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
        view = (texu_treeview_item *)iter->data;
        data = (texu_treewnd_item *)view->item->data;
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
    data = (texu_treewnd_item *)tc->curselitem->data;
    data->selected = 0;

    /* highlighting the new item */
    iter = cur_iter;
    cur_moves = TEXU_MAX(0, TEXU_MIN(move_times, items - curindex - 1));
    for (i = 0; i < cur_moves && iter; ++i)
    {
        iter = iter->next;
    }
    view = (texu_treeview_item *)iter->data;
    data = (texu_treewnd_item *)view->item->data;

    tc->curselitem = view->item;
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

        view = (texu_treeview_item *)iter->data;
        tc->firstvisibleitem = view->item;
    }
    return tc->curselitem;
}

texu_tree_item *_TexuTreeCtrlProc_MovePrev(texu_wnd *wnd, texu_i32 move_times)
{
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;
    texu_list_item *iter = 0;
    texu_treeview_item *view = 0;
    texu_i32 i = 0;
    texu_i32 firstvisibleindex = 0;
    texu_i32 curindex = 0;
    texu_list_item *first_iter = 0;
    texu_list_item *cur_iter = 0;
    texu_rect rc;
    texu_i32 first_moves = 0;
    texu_i32 cur_moves = 0;
    texu_bool movefirst = TEXU_FALSE;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

    /* change selection item */
    /* find the current selection index */
    firstvisibleindex = 0;
    iter = texu_list_first(tc->visibleitems);
    while (iter)
    {
        view = (texu_treeview_item *)iter->data;
        data = (texu_treewnd_item *)view->item->data;

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
        view = (texu_treeview_item *)iter->data;
        data = (texu_treewnd_item *)view->item->data;

        if (view->item == tc->curselitem)
        {
            cur_iter = iter;
            break;
        }
        iter = iter->next;
        ++curindex;
    }

    /* un-highlighting the old item */

    data = (texu_treewnd_item *)tc->curselitem->data;
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

    view = (texu_treeview_item *)iter->data;
    data = (texu_treewnd_item *)view->item->data;

    tc->curselitem = view->item;
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

        view = (texu_treeview_item *)iter->data;
        tc->firstvisibleitem = view->item;
    }
    return tc->curselitem;
}

void _TexuTreeCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_treewnd *tc = 0;
    texu_rect rc;
    texu_treewnd_item *data = 0;
    texu_tree_item *selitem = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    if (0 == tc->visibleitems)
    {
        /* no item */
        return;
    }
    texu_wnd_get_rect(wnd, &rc);

    switch (ch)
    {
        case 0x20: /*SPACE*/
        {
            /* expand or collapse the current selection item */
            data = (texu_treewnd_item *)tc->curselitem->data;
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

        case TEXU_KEY_UP:
        {
            /* check if the last visible item */
            selitem = _TexuTreeCtrlProc_MovePrev(wnd, 1);
            /* notify select item */
            _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
            break;
        }

        case TEXU_KEY_DOWN:
        {
            selitem = _TexuTreeCtrlProc_MoveNext(wnd, 1);
            _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
            break;
        }

        case TEXU_KEY_NPAGE:
        {
            selitem = _TexuTreeCtrlProc_MoveNext(wnd, rc.lines);
            _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
            break;
        }

            /* move up 1 page */
        case TEXU_KEY_PPAGE:
        {
            selitem = _TexuTreeCtrlProc_MovePrev(wnd, rc.lines);
            _TexuTreeCtrlProc_OnSetSelItem(wnd, selitem);
            break;
        }

        case TEXU_KEY_RIGHT:
        {
            tc->shifted_right = TEXU_MIN(rc.cols, tc->shifted_right + tc->shifted);
            /* update item on the screen */
            _TexuTreeCtrlProc_RefreshView(wnd);
            break;
        }

        case TEXU_KEY_LEFT:
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

texu_tree_find_proc _TexuTreeCtrlProc_OnSetFindItemProc(texu_wnd *wnd, texu_tree_find_proc findproc)
{
    texu_tree_find_proc oldproc = 0;
    texu_treewnd *tc = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    oldproc = tc->findproc;
    tc->findproc = findproc;
    return oldproc;
}

void _TexuTreeCtrlProc_DestroyTreeProc(texu_i64 data, void *userdata)
{
    texu_treewnd_item *item = (texu_treewnd_item *)data;
    free(item);
}

void _TexuTreeCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    if (tc->visibleitems)
    {
        texu_list_cb_free(tc->visibleitems, _texu_treeview_item_del, 0);
    }
    texu_list_del(tc->visibleitems);

    texu_tree_cb_remove_item(tc->tree,
                             texu_tree_get_root(tc->tree), _TexuTreeCtrlProc_DestroyTreeProc, 0);
    texu_tree_del(tc->tree);
    free(tc);
}

void _TexuTreeCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    _TexuWndProc_Notify(wnd, TEXU_TCN_SETFOCUS);
}

texu_i32 _TexuTreeCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_i32 rc = TEXU_OK;

    _TexuWndProc_Notify(wnd, TEXU_TCN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return rc;
}

texu_tree_item *_TexuTreeCtrlProc_OnInsertItem(
    texu_wnd *wnd,
    texu_tree_item *parentitem,
    const texu_treewnd_item *insertitem)
{
    texu_treewnd *tc = 0;
    texu_i32 children = 0;
    texu_tree_item *newitem = 0;
    texu_treewnd_item *parentdata = 0;
    texu_treewnd_item *data = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    if (0 == insertitem)
    {
        return 0;
    }

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
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
            tc->firstvisibleitem = tc->curselitem = (texu_tree_item *)newitem;
        }
        /* update parent */
        if (parentitem)
        {
            parentdata = (texu_treewnd_item *)parentitem->data;
            ++parentdata->children;
        }
        /* update item on the screen */
        /*_TexuTreeCtrlProc_RefreshView(wnd);*/
    }

    return (texu_tree_item *)newitem;
}

texu_treewnd_item *
_texu_treewnd_item_new(const texu_treewnd_item *item)
{
    texu_treewnd_item *data = (texu_treewnd_item *)malloc(sizeof(texu_treewnd_item));
    if (data)
    {
        memset(data, 0, sizeof(texu_treewnd_item));
        memcpy(data, item, sizeof(texu_treewnd_item));
    }
    return data;
}

void
_texu_treewnd_item_del(texu_i64 data, void *userdata)
{
    texu_treewnd_item *item = (texu_treewnd_item *)data;
    free(item);
}

texu_i32 _TexuTreeCtrlProc_OnDeleteItem(texu_wnd *wnd, texu_tree_item *item)
{
    texu_treewnd *tc = 0;
    texu_i32 children = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

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

texu_i32 _TexuTreeCtrlProc_OnExpandItem(texu_wnd *wnd, texu_tree_item *item)
{
    texu_treewnd_item *data = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMEXPANDING, item);

    /* mark expanded */
    data = (texu_treewnd_item *)item->data;

    if (data->children > 0)
    {
        data->expanded = TEXU_TRUE;
    }
    _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMEXPANDED, item);

    /* update item on the screen */
    _TexuTreeCtrlProc_RefreshView(wnd);
    return 0;
}

texu_i32 _TexuTreeCtrlProc_OnGetIndentText(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    return tc->indent;
}

void _TexuTreeCtrlProc_OnSetIndentText(texu_wnd *wnd, texu_i32 indent)
{
    texu_treewnd *tc = 0;
    texu_rect rc;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
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

texu_i32 _TexuTreeCtrlProc_OnGetShiftedText(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    return tc->shifted;
}

void _TexuTreeCtrlProc_OnSetShiftedText(texu_wnd *wnd, texu_i32 shifted)
{
    texu_treewnd *tc = 0;
    texu_rect rc;

    texu_wnd_get_rect(wnd, &rc);
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
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

void _TexuTreeCtrlProc_OnExpandAllItems(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;
    texu_tree_item *root = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    root = texu_tree_get_root(tc->tree);

    texu_tree_populate(
        tc->tree,
        root->firstchild,
        TEXU_TPO_PRE,
        _TexuTreeCtrlProc_ExpandAllItemsProc,
        wnd);

    _TexuTreeCtrlProc_OnExpandItem(wnd, root);
}

void _TexuTreeCtrlProc_OnCollapseAllItems(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;
    texu_tree_item *root = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    root = texu_tree_get_root(tc->tree);

    texu_tree_populate(
        tc->tree,
        root->firstchild,
        TEXU_TPO_PRE,
        _TexuTreeCtrlProc_CollapseAllItemsProc,
        wnd);

    _TexuTreeCtrlProc_OnCollapseItem(wnd, root);
}

texu_i32 _TexuTreeCtrlProc_OnCollapseItem(texu_wnd *wnd, texu_tree_item *item)
{
    texu_treewnd_item *data = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMCOLLAPSING, item);

    /* mark collapse */
    data = (texu_treewnd_item *)item->data;

    if (data->children > 0)
    {
        data->expanded = TEXU_FALSE;
    }
    _TexuTreeProc_Notify(wnd, TEXU_TCN_ITEMCOLLAPSED, item);

    /* update item on the screen */
    _TexuTreeCtrlProc_RefreshView(wnd);
    return 0;
}

texu_i32 _TexuTreeCtrlProc_OnSetSelItem(texu_wnd *wnd, texu_tree_item *setitem)
{
    texu_treewnd *tc = 0;
    texu_treewnd_item *data = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    /* remove the highlighting */
    if (tc->curselitem)
    {
        data = (texu_treewnd_item *)tc->curselitem;
        data->selected = TEXU_FALSE;
    }

    /* new selected item */
    tc->curselitem = setitem;

    /* set the highlighting */
    if (tc->curselitem)
    {
        data = (texu_treewnd_item *)tc->curselitem;
        data->selected = TEXU_TRUE;
    }
    /* update item on the screen */
    _TexuTreeCtrlProc_RefreshView(wnd);
    return 0;
}

texu_tree_item *_TexuTreeCtrlProc_OnGetSelItem(texu_wnd *wnd)
{
    texu_treewnd *tc = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    return tc->curselitem;
}

texu_i32 _TexuTreeCtrlProc_OnSetItem(texu_wnd *wnd, texu_tree_item *item, const texu_treewnd_item *setitem)
{
    texu_treewnd_item *data = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
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
        data = (texu_treewnd_item *)item->data;
        if (data)
        {
            _texu_treewnd_item_del((texu_i64)data, 0);
        }
        data = _texu_treewnd_item_new(setitem);
        texu_wnd_invalidate(wnd);
    }

    return 0;
}

texu_i32 _TexuTreeCtrlProc_OnGetItem(texu_wnd *wnd, texu_tree_item *item, texu_treewnd_item *getitem)
{
    if (!item)
    {
        return -1;
    }
    if (!getitem)
    {
        return -2;
    }

    getitem = (texu_treewnd_item *)item->data;
    return 0;
}

texu_tree_item *
_TexuTreeCtrlProc_OnFindItem(texu_wnd *wnd, texu_treewnd_item *finditem, void *userdata)
{
    texu_treewnd *tc = 0;
    texu_tree_item *founditem = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

    founditem = texu_tree_find_item(
        tc->tree,
        (texu_i64)finditem,
        (tc->findproc ? tc->findproc : _TexuTreeCtrlProc_DefFindItemProc),
        userdata);

    tc->userdata_findproc = userdata;

    return (texu_tree_item *)founditem;
}

texu_tree_item *
_TexuTreeCtrlProc_OnFindNextItem(texu_wnd *wnd, texu_tree_item *previtem, texu_treewnd_item *finditem)
{
    texu_treewnd *tc = 0;
    texu_tree_item *founditem = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

    founditem = texu_tree_find_next_item(
        tc->tree,
        previtem,
        (texu_i64)finditem,
        (tc->findproc ? tc->findproc : _TexuTreeCtrlProc_DefFindItemProc),
        tc->userdata_findproc);

    return (texu_tree_item *)founditem;
}

texu_i32 _TexuTreeCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_treewnd *tc = (texu_treewnd *)malloc(sizeof(texu_treewnd));
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd *editwnd = texu_wnd_new(env);
    texu_wnd_attrs attrs2;
    texu_status rc = TEXU_OK;

    if (!editwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y = 0;
    attrs2.x = 0;
    attrs2.height = 1;
    attrs2.width = 1;
    attrs2.enable = TEXU_FALSE;
    attrs2.visible = TEXU_FALSE;
    attrs2.text = TEXU_EMPTY_STR;
    attrs2.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL);
    attrs2.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_SELECTED);
    attrs2.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL);
#if (defined WIN32 && defined _WINDOWS)
    attrs2.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL);
    attrs2.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_DISABLED);
    attrs2.selectedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_SELECTED);
    attrs2.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL);
#endif
    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }

    if (!tc)
    {
        return TEXU_NOMEM;
    }
    memset(tc, 0, sizeof(texu_treewnd));
    tc->indent = 2;
    tc->shifted_right = 0;
    tc->shifted = 4;
    tc->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL);
    tc->discolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_DISABLED);
    tc->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_SELECTED);
    tc->focuscolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_FOCUSED);
#if (defined WIN32 && defined _WINDOWS)
    tc->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL);
    tc->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_DISABLED);
    tc->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_SELECTED);
    tc->focusbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_FOCUSED);
#endif
    tc->editbox = editwnd;

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
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_FOCUSED));
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                               texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_FOCUSED));
#endif
    return TEXU_OK;
}

void _TexuTreeCtrlProc_GetDisplayedText(
    texu_wnd *wnd,
    texu_char *outtext, texu_i32 *startx, texu_i32 *endx,
    texu_tree_item *item,
    texu_i32 maxlen,
    texu_bool file)
{
    texu_i32 xpos = 0;
    texu_treewnd_item *data = 0;
    texu_treewnd *tc = 0;
    texu_ui32 children = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 shifted_right = 0;
    texu_i32 textlen = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 len = 0;

    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);
    if (!tc)
    {
        return;
    }
    data = (texu_treewnd_item *)item->data;

    memset(buf, 0, sizeof(buf));

    xpos = item->nlevel;
    /* fill indents */
    if (xpos > 0)
    {
        if (file)
        {
            texu_memset(buf, TEXUTEXT('\t'), xpos);
        }
        else
        {
            xpos *= tc->indent;
            texu_memset(buf, TEXUTEXT(' '), xpos);
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
            buf[xpos - tc->indent] = TEXUTEXT('-');
        }
        else
        {
            buf[xpos - tc->indent] = TEXUTEXT('+');
        }
    }
    /* print text */
    textlen = texu_strlen(data->itemtext);
    if (textlen > 0)
    {
        texu_memcpy(&buf[xpos], data->itemtext, textlen);
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
    texu_memcpy(outtext, &buf[shifted_right], xpos);

    len = texu_strlen(outtext);
    if (len < maxlen)
    {
        texu_memset(&outtext[len], TEXUTEXT(' '), maxlen - len);
        outtext[maxlen] = 0;
    }
}

texu_i64 _TexuTreeCtrlProc_ExpandAllItemsProc(texu_tree_item *item, void *args)
{
    texu_treewnd_item *data = 0;

    data = (texu_treewnd_item *)item->data;
    data->expanded = TEXU_TRUE;
    return 0;
}

texu_i64 _TexuTreeCtrlProc_CollapseAllItemsProc(texu_tree_item *item, void *args)
{
    texu_treewnd_item *data = 0;

    data = (texu_treewnd_item *)item->data;
    data->expanded = TEXU_FALSE;

    return 0;
}

texu_i64 _TexuTreeCtrlProc_PreorderTraverseProc(texu_tree_item *item, void *args)
{
    texu_queue *queue = (texu_queue *)args;
    texu_treeview_item *view = 0;

    view = _texu_treeview_item_new(item);
    texu_queue_enqueue(queue, (texu_i64)view);
    return 0;
}

void _TexuTreeCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_rect rc;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 displayableitems = 0;
    texu_treewnd *tc = 0;
    texu_i32 y = 0;
    texu_treewnd_item *data = 0;
    texu_treeview_item *view = 0;
    texu_list_item *iter;
    texu_i32 count = 0, items = 0;
    texu_i32 ysel = 0;
    texu_i32 xsel = 0;
    texu_char bufsel[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 style = 0;
    texu_char filler = 0;
    texu_bool fullrow = TEXU_FALSE;
    texu_i32 startx = 0, endx = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM_SELECTED);
#if (defined WIN32 && defined _WINDOWS)
    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_ITEM);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_ITEM_SELECTED);
#endif
    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
    tc = (texu_treewnd *)texu_wnd_get_userdata(wnd);

    /* visible window & there is a visible item */
    if (tc->firstvisibleitem)
    {
        texu_wnd_get_rect(wnd, &rc);
        y = rc.y;

        style = texu_wnd_get_style(wnd);
        if (TEXU_TCS_NOHIGHLIGHT & style)
        {
            selcolor = normcolor;
#if (defined WIN32 && defined _WINDOWS)
            selbg = normbg;
#endif
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
            view = (texu_treeview_item *)iter->data;
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
            view = (texu_treeview_item *)iter->data;
            data = (texu_treewnd_item *)view->item->data;
            normcolor = data->normcolor;
            selcolor = data->selcolor;
#if (defined WIN32 && defined _WINDOWS)
            normbg = data->normbg;
            selbg = data->selbg;
#endif

            memset(buf, filler, sizeof(buf));
            _TexuTreeCtrlProc_GetDisplayedText(wnd, buf, &startx, &endx, view->item, rc.cols, TEXU_FALSE);
            if (data->selected)
            {
                if (fullrow)
                {
#if (defined WIN32 && defined _WINDOWS)
                    texu_env_draw_text(env, y, rc.x, buf, selcolor, selbg);
#else
                    texu_cio_putstr_attr(dc, y, rc.x, buf, selcolor);
#endif
                }
                else
                {
                    buf[endx] = 0;
#if (defined WIN32 && defined _WINDOWS)
                    texu_env_draw_text(env, y, rc.x + startx, &buf[startx], selcolor, selbg);
#else
                    texu_cio_putstr_attr(dc, y, rc.x + startx, &buf[startx],
                                         texu_cio_get_color(dc, selcolor));
#endif
                }
                ysel = y;
                xsel = rc.x;
                texu_strcpy(bufsel, buf);
            }
            else
            {
                buf[endx] = 0;
#if (defined WIN32 && defined _WINDOWS)
                texu_env_draw_text(env, y, rc.x, buf, normcolor, normbg);
#else
                texu_cio_putstr_attr(dc, y, rc.x, buf,
                                     texu_cio_get_color(dc, normcolor));
#endif
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
            view = (texu_treeview_item *)iter->data;
            tc->lastvisibleitem = view->item;
        }

        /* draw the selected item */
#if (defined WIN32 && defined _WINDOWS)
        texu_env_draw_text(env, ysel, xsel, bufsel, selcolor, selbg);
        texu_env_gotoyx(env, ysel, xsel);
#else
        texu_cio_putstr_attr(dc, ysel, xsel, bufsel,
                             texu_cio_get_color(dc, selcolor));
        texu_cio_gotoyx(dc, ysel, xsel);
#endif
    }
}

texu_i32
_TexuTreeCtrlProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_tree_item *treeitem = _TexuTreeCtrlProc_OnGetSelItem(wnd);
    texu_treewnd_item item;
    texu_i32 len = 0;

    if (treeitem)
    {
        _TexuTreeCtrlProc_OnGetItem(wnd, treeitem, &item);
        texu_strcpy(text, item.itemtext);
    }
    else
    {
        texu_strcpy(text, TEXT(""));
    }
    len = texu_strlen(text);
    if (len > 0 && textlen < len)
    {
        text[textlen-1] = 0;
    }
    return texu_strlen(text);
}

void
_TexuTreeCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_tree_item *treeitem = _TexuTreeCtrlProc_OnGetSelItem(wnd);
    texu_treewnd_item item;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (treeitem)
    {
        _TexuTreeCtrlProc_OnGetItem(wnd, treeitem, &item);
        texu_strcpy(item.itemtext, text);
        _TexuTreeCtrlProc_OnSetItem(wnd, treeitem, &item);
    }
}

texu_i64
_TexuTreeCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_PAINT:
        {
            _TexuTreeCtrlProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;
        }
        case TEXU_WM_CREATE:
        {
            return _TexuTreeCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
        }
        case TEXU_WM_DESTROY:
        {
            /* release memory of static control */
            _TexuTreeCtrlProc_OnDestroy(wnd);
            return 0;
        }
        case TEXU_WM_SETFOCUS:
        {
            _TexuTreeCtrlProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;
        }
        case TEXU_WM_KILLFOCUS:
        {
            return _TexuTreeCtrlProc_OnKillFocus(wnd, (texu_wnd *)param1);
        }
        case TEXU_WM_CHAR:
        {
            _TexuTreeCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            break;
        }
        case TEXU_WM_GETTEXT:
            return _TexuTreeCtrlProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

        case TEXU_WM_SETTEXT:
            _TexuTreeCtrlProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;
        case TEXU_TCM_INSERTITEM:
        {
            return (texu_i64)_TexuTreeCtrlProc_OnInsertItem(wnd, (texu_tree_item *)param1, (const texu_treewnd_item *)param2);
        }
        case TEXU_TCM_DELETEITEM:
        {
            return _TexuTreeCtrlProc_OnDeleteItem(wnd, (texu_tree_item *)param1);
        }
        case TEXU_TCM_SETITEM:
        {
            return _TexuTreeCtrlProc_OnSetItem(wnd, (texu_tree_item *)param1, (const texu_treewnd_item *)param2);
        }
        case TEXU_TCM_GETITEM:
        {
            return _TexuTreeCtrlProc_OnGetItem(wnd, (texu_tree_item *)param1, (texu_treewnd_item *)param2);
        }
        case TEXU_TCM_FINDITEM:
        {
            return (texu_i64)_TexuTreeCtrlProc_OnFindItem(wnd, (texu_treewnd_item *)param1, (void *)param2);
        }
        case TEXU_TCM_FINDNEXTITEM:
        {
            return (texu_i64)_TexuTreeCtrlProc_OnFindNextItem(wnd, (texu_tree_item *)param1, (texu_treewnd_item *)param2);
        }
        case TEXU_TCM_EXPANDITEM:
        {
            return _TexuTreeCtrlProc_OnExpandItem(wnd, (texu_tree_item *)param1);
        }
        case TEXU_TCM_COLLAPSEITEM:
        {
            return _TexuTreeCtrlProc_OnCollapseItem(wnd, (texu_tree_item *)param1);
        }
        case TEXU_TCM_SETFINDITEMPROC:
        {
            return (texu_i64)_TexuTreeCtrlProc_OnSetFindItemProc(wnd, (texu_tree_find_proc)param1);
        }
        case TEXU_TCM_SETSELITEM:
        {
            return _TexuTreeCtrlProc_OnSetSelItem(wnd, (texu_tree_item *)param1);
        }
        case TEXU_TCM_GETSELITEM:
        {
            return (texu_i64)_TexuTreeCtrlProc_OnGetSelItem(wnd);
        }
        case TEXU_TCM_EXPORTTOFILE:
        {
            return _TexuTreeCtrlProc_OnExportToFile(wnd, (FILE *)param1, (texu_tree_exp_proc)param2);
        }
        case TEXU_TCM_IMPORTFROMFILE:
        {
            return _TexuTreeCtrlProc_OnImportFromFile(wnd, (FILE *)param1, (texu_tree_imp_proc)param2);
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
    texu_wnd *editwnd;
    texu_i32 step;
    texu_i32 page;
    texu_i32 min;
    texu_i32 max;
    void *exparam;
};
typedef struct texu_udwnd texu_udwnd;

void _TexuUpDownCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuUpDownCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuUpDownCtrlProc_OnDestroy(texu_wnd *wnd);
void _TexuUpDownCtrlProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuUpDownCtrlProc_OnKillFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuUpDownCtrlProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuUpDownCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void _TexuUpDownCtrlProc_OnSetMinMax(texu_wnd *wnd, texu_i32, texu_i32);
texu_status _TexuUpDownCtrlProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *, texu_i32 *);
void _TexuUpDownCtrlProc_OnSetStep(texu_wnd *wnd, texu_i32);
texu_i32 _TexuUpDownCtrlProc_OnGetStep(texu_wnd *wnd);
void _TexuUpDownCtrlProc_OnStep(texu_wnd *wnd, texu_i32);
void _TexuUpDownCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
void _TexuUpDownCtrlProc_OnSetPage(texu_wnd *wnd, texu_i32);
texu_i32 _TexuUpDownCtrlProc_OnGetPage(texu_wnd *wnd);
void _TexuUpDownCtrlProc_OnPage(texu_wnd *wnd, texu_i32);

void
_TexuUpDownCtrlProc_OnSetMinMax(texu_wnd *wnd, texu_i32 min, texu_i32 max)
{
    texu_udwnd *udctl = 0;
    texu_editminmax minmax;

    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    udctl->min = min;
    udctl->max = max;

    minmax.min = min;
    minmax.max = max;
    texu_wnd_send_msg(udctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
}

texu_status
_TexuUpDownCtrlProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *min, texu_i32 *max)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    *min = udctl->min;
    *max = udctl->max;
    return TEXU_OK;
}

void
_TexuUpDownCtrlProc_OnSetStep(texu_wnd *wnd, texu_i32 step)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    step = TEXU_MAX(udctl->min, step);
    step = TEXU_MIN(udctl->max, step);
    udctl->step = step;
}

texu_i32
_TexuUpDownCtrlProc_OnGetStep(texu_wnd *wnd)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    return udctl->step;
}

void
_TexuUpDownCtrlProc_OnStep(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuUpDownCtrlProc_OnChar(wnd, TEXU_KEY_UP, 0);
    }
    else
    {
        _TexuUpDownCtrlProc_OnChar(wnd, TEXU_KEY_DOWN, 0);
    }
}

void
_TexuUpDownCtrlProc_OnSetPage(texu_wnd *wnd, texu_i32 page)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    page = TEXU_MAX(udctl->min, page);
    page = TEXU_MIN(udctl->max, page);
    udctl->page = page;
}

texu_i32
_TexuUpDownCtrlProc_OnGetPage(texu_wnd *wnd)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    return udctl->page;
}

void
_TexuUpDownCtrlProc_OnPage(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuUpDownCtrlProc_OnChar(wnd, TEXU_KEY_NPAGE, 0);
    }
    else
    {
        _TexuUpDownCtrlProc_OnChar(wnd, TEXU_KEY_PPAGE, 0);
    }
}

texu_status
_TexuUpDownCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_udwnd *udctl = 0;
    texu_wnd_attrs attrs2;
    texu_wnd *editwnd = 0;
    texu_status rc = TEXU_OK;
    texu_editminmax minmax;
    texu_env *env = texu_wnd_get_env(wnd);

    editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y = attrs->y;
    attrs2.x = attrs->x+1;
    attrs2.height   = attrs->height;
    attrs2.width    = attrs->width - 2;
    attrs2.enable   = TEXU_TRUE;
    attrs2.visible  = TEXU_TRUE;
    attrs2.text     = TEXT("0");
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED);
#if (defined WIN32 && defined _WINDOWS)
    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED);
#endif
    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER | TEXU_ES_CENTER;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }

    udctl = (texu_udwnd *)malloc(sizeof(texu_udwnd));
    if (!udctl)
    {
        texu_wnd_del(editwnd);
        return TEXU_NOMEM;
    }

    memset(udctl, 0, sizeof(texu_udwnd));
    udctl->editwnd = editwnd; /* no parameter */
    udctl->step = 1;
    udctl->page = 10;
    udctl->min = 0;
    udctl->max = 255;

    minmax.min = udctl->min;
    minmax.max = udctl->max;
    texu_wnd_send_msg(udctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);

    /* save memory */
    texu_wnd_set_userdata(wnd, udctl);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED));
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                               texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED));
#endif
    return TEXU_OK;
}

void
_TexuUpDownCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_udwnd *udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    texu_wnd_send_msg(udctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_UDCN_SETFOCUS);
}

texu_i32
_TexuUpDownCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    /*update value to window text */
    texu_udwnd *udctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);

    texu_wnd_send_msg(udctl->editwnd, TEXU_WM_KILLFOCUS, 0, 0);

    texu_wnd_get_text(udctl->editwnd, buf, TEXU_MAX_WNDTEXT);
    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_UDCN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_CONTINUE;
}

void
_TexuUpDownCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_udwnd *udctl = 0;
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    free(udctl);
}

texu_i32
_TexuUpDownCtrlProc_OnGetInt(texu_wnd *wnd)
{
    texu_udwnd *udctl = 0;
    texu_i32 val = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(udctl->editwnd, buf, TEXU_MAX_WNDTEXT);
    val = texu_atol(buf);

    return val;
}

void
_TexuUpDownCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_udwnd *udctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 val = 0;
    texu_bool updown = TEXU_FALSE;
    texu_i32 added = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    switch (ch)
    {
        case TEXU_KEY_RIGHT:
        case TEXU_KEY_UP:
        {
            added += udctl->step;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_LEFT:
        case TEXU_KEY_DOWN:
        {
            added -= udctl->step;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_NPAGE:
        {
            added -= udctl->page;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_PPAGE:
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
        val = texu_atol(buf);
        val += added;
        if (val < udctl->min)
        {
            val = udctl->min;
        }
        if (val > udctl->max)
        {
            val = udctl->max;
        }
#if (defined WIN32 && defined UNICODE)
        swprintf(buf, sizeof(buf), TEXT("%d"), val);
#else
        sprintf(buf, "%d", val);
#endif
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
_TexuUpDownCtrlProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_udwnd *udctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 val = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    udctl = (texu_udwnd *)texu_wnd_get_userdata(wnd);
    val = texu_atol(text);
#if (defined WIN32 && defined UNICODE)
    swprintf(buf, sizeof(buf), TEXT("%d"), val);
#else
    sprintf(buf, "%d", val);
#endif
    texu_wnd_set_text(udctl->editwnd, buf);
    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)buf, 0);
}

void
_TexuUpDownCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL);
#if (defined WIN32 && defined _WINDOWS)
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL);
#endif
    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putch_attr(dc, y, x, TEXU_ACS_MINUS,
                        texu_cio_get_color(dc, color));
    texu_cio_putch_attr(dc, y, x + width - 1, TEXU_ACS_PLUS,
                        texu_cio_get_color(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_char(env, y, x, TEXU_ACS_MINUS, color, bgcolor);
    texu_env_draw_char(env, y, x + width - 1, TEXU_ACS_PLUS, color, bgcolor);
#else
    texu_cio_putch_attr(dc, y, x, TEXU_ACS_MINUS,
                        texu_cio_get_color(dc, color));
    texu_cio_putch_attr(dc, y, x + width - 1, TEXU_ACS_PLUS,
                        texu_cio_get_color(dc, color));
#endif
#endif
}

texu_i64
_TexuUpDownCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
    case TEXU_WM_CHAR:
        _TexuUpDownCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_WM_CREATE:
        return _TexuUpDownCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_WM_PAINT:
        _TexuUpDownCtrlProc_OnPaint(wnd, (texu_cio *)param1);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuUpDownCtrlProc_OnDestroy(wnd);
        break;

    case TEXU_WM_SETFOCUS:
        _TexuUpDownCtrlProc_OnSetFocus(wnd, (texu_wnd *)param1);
        break;

    case TEXU_WM_KILLFOCUS:
        return _TexuUpDownCtrlProc_OnKillFocus(wnd, (texu_wnd *)param1);

    case TEXU_WM_SETTEXT:
        _TexuUpDownCtrlProc_OnSetText(wnd, (const texu_char *)param1);
        return 0;

    case TEXU_UDCM_SETMINMAX:
    {
        _TexuUpDownCtrlProc_OnSetMinMax(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;
    }
    case TEXU_UDCM_GETMINMAX:
    {
        return _TexuUpDownCtrlProc_OnGetMinMax(wnd, (texu_i32 *)param1, (texu_i32 *)param2);
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
# TexU progress bar ctrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_pgbwnd
{
    texu_i32 step;
    texu_i32 pos;
    texu_i32 max;
    void *exparam;
};
typedef struct texu_pgbwnd texu_pgbwnd;

texu_status _TexuProgressBarProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuProgressBarProc_OnDestroy(texu_wnd *wnd);
void _TexuProgressBarProc_OnSetMax(texu_wnd *wnd, texu_i32);
void _TexuProgressBarProc_OnSetPos(texu_wnd *wnd, texu_i32);
texu_i32 _TexuProgressBarProc_OnGetPos(texu_wnd *wnd);
void _TexuProgressBarProc_OnSetStep(texu_wnd *wnd, texu_i32);
texu_i32 _TexuProgressBarProc_OnGetStep(texu_wnd *wnd);
void _TexuProgressBarProc_OnStep(texu_wnd *wnd, texu_i32);
void _TexuProgressBarProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_i32 _TexuProgressBarProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuProgressBarProc_OnGetPercent(texu_wnd *wnd, texu_f32 *pct);

texu_f32 _TexuProgressBarProc_GetPercent(texu_wnd *wnd);

void
_TexuProgressBarProc_OnSetMax(texu_wnd *wnd, texu_i32 max)
{
    texu_pgbwnd *pgb = 0;

    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    pgb->max = max;
}

texu_i32
_TexuProgressBarProc_OnGetMax(texu_wnd *wnd)
{
    texu_pgbwnd *pgb = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    return pgb->max;
}

void
_TexuProgressBarProc_OnSetPos(texu_wnd *wnd, texu_i32 pos)
{
    texu_pgbwnd *pgb = 0;
    /*
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }*/
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
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
_TexuProgressBarProc_OnGetPos(texu_wnd *wnd)
{
    texu_pgbwnd *pgb = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    return pgb->pos;
}

void
_TexuProgressBarProc_OnSetStep(texu_wnd *wnd, texu_i32 step)
{
    texu_pgbwnd *pgb = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    pgb->step = step;
}

texu_i32
_TexuProgressBarProc_OnGetStep(texu_wnd *wnd)
{
    texu_pgbwnd *pgb = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    return pgb->step;
}

void
_TexuProgressBarProc_OnStep(texu_wnd *wnd, texu_i32 updown)
{
    texu_pgbwnd *pgb = 0;
    texu_i32 pos = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    pos = pgb->pos;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
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
_TexuProgressBarProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_pgbwnd *pgb = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    pgb = (texu_pgbwnd *)malloc(sizeof(texu_pgbwnd));
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
_TexuProgressBarProc_OnDestroy(texu_wnd *wnd)
{
    texu_pgbwnd *pgb = 0;
    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);
    free(pgb);
}

void
_TexuProgressBarProc_OnGetPercent(texu_wnd *wnd, texu_f32 *pct)
{
    *pct = _TexuProgressBarProc_GetPercent(wnd);
}

texu_f32
_TexuProgressBarProc_GetPercent(texu_wnd *wnd)
{
    texu_f32 pct = 0.0f;
    texu_pgbwnd *pgb = 0;

    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);

    pct = (0 == pgb->max ? 0.0f : (texu_f32)(pgb->pos * 100.0f / pgb->max));
    return pct;
}

void
_TexuProgressBarProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_PROGRESSBAR);
#if (defined WIN32 && defined _WINDOWS)
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PROGRESSBAR);
#endif
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_f32 pct = 0.0;
    texu_pgbwnd *pgb = 0;
    texu_i32 pgwidth = 0;

    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }

    pgb = (texu_pgbwnd *)texu_wnd_get_userdata(wnd);

    memset(buf, 0, sizeof(buf));
    texu_memset(buf, TEXUTEXT('.'), width);
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, y, x, buf, color, bgcolor);
#else
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_color(dc, color));
#endif
    /*get percent*/
    pct = (0 == pgb->max ? 0.0f : (texu_f32)(pgb->pos * 100.0f / pgb->max));
    pct = (pct > 100.0f ? 100.0f : pct);
#if (defined WIN32 && defined UNICODE)
    swprintf(text, sizeof(text), TEXUTEXT("%.02f"), pct);
#else
    sprintf(text, TEXUTEXT(""%.02f"), pct);
#endif
    texu_strcat(text, TEXUTEXT("%"));
    /*get drawable width*/
    pgwidth = (0 == pgb->max ? 0 : ((texu_i32)pct >= 100 ? width : (pgb->pos * width / pgb->max)));
    if ((texu_ui32)pgwidth < texu_strlen(text))
    {
        pgwidth = texu_strlen(text);
    }
    texu_printf_alignment2(buf, text, pgwidth, TEXU_ALIGN_RIGHT, TEXU_TRUE);

#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, y, x, buf, bgcolor, color);
#else
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_reverse(dc, color));
#endif
}

texu_i32
_TexuProgressBarProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_f32 pct = _TexuProgressBarProc_GetPercent(wnd);
    texu_i32 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

#if (defined WIN32 && defined UNICODE)
    swprintf(buf, sizeof(buf), TEXT("%.02f%%"), pct);
#else
    sprintf(buf, "%.02f%%", pct);
#endif
    len = texu_strlen(buf);
    if (textlen < 0 || 0 == text)
    {
        return len;
    }

    memset(text, 0, textlen);
    textlen = TEXU_MIN(len, textlen);
    texu_memcpy(text, buf, textlen);

    return textlen;
}

texu_i64
_TexuProgressBarProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
    case TEXU_WM_GETTEXT:
        return _TexuProgressBarProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

    case TEXU_WM_CREATE:
        return _TexuProgressBarProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_WM_PAINT:
        _TexuProgressBarProc_OnPaint(wnd, (texu_cio *)param1);
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
    case TEXU_PGBM_GETPERCENT:
    {
        _TexuProgressBarProc_OnGetPercent(wnd, (texu_f32 *)param1);
        return 0;
    }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU page ctrl
#
     1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_MAX_PAGE 128

struct texu_pagewnd_page
{
    texu_wnd *page;
    texu_wnd *firstchild;
    texu_wnd *lastchild;
    texu_wnd *activechild;
    void *userdata;
};
typedef struct texu_pagewnd_page texu_pagewnd_page;

struct texu_pagewnd
{
    texu_wnd *curpage;
    texu_wnd *firstactive;
    texu_wnd *lastactive;
    texu_wnd *activewnd;
    texu_array *pages;
    texu_i32 curidx;
    void *exparam;
    texu_bool ctl_k_pressed;
};
typedef struct texu_pagewnd texu_pagewnd;

texu_status _TexuPageCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuPageCtrlProc_OnDestroy(texu_wnd *wnd);
void _TexuPageCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
void _TexuPageCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);

texu_status _TexuPageCtrlProc_OnAddPage(texu_wnd *wnd, const texu_char *clsname, texu_ui32 id);
texu_wnd *_TexuPageCtrlProc_OnSetCurPage(texu_wnd *wnd, texu_i32 idx);
texu_wnd *_TexuPageCtrlProc_OnGetCurPage(texu_wnd *wnd);
texu_i32 _TexuPageCtrlProc_OnGetPageIndex(texu_wnd *wnd, texu_wnd *page);
texu_i32 _TexuPageCtrlProc_OnGetCountPage(texu_wnd *wnd);

texu_wnd *_TexuPageCtrl_CreatePage(
    texu_wnd *wnd,
    const texu_char *clsname,
    texu_ui32 id);
texu_wnd *_TexuPageCtrlProc_GetFirstPageEnabled(texu_wnd *);
texu_wnd *_TexuPageCtrlProc_GetLastPageEnabled(texu_wnd *);
texu_wnd *_TexuPageCtrlProc_GetPage(texu_wnd *wnd, texu_i32 idx);
texu_wnd *_TexuPageCtrlProc_GetNextPageEnabled(texu_wnd *wnd, texu_wnd *page);
texu_wnd *_TexuPageCtrlProc_GetPrevPageEnabled(texu_wnd *wnd, texu_wnd *page);
texu_wnd *_TexuPageCtrlProc_SetCurPage(texu_wnd *wnd, texu_wnd *newpage);
texu_i32 _TexuPageCtrlProc_GetPageIndex(texu_wnd *wnd, texu_wnd *whatpage);

void _texu_pagewnd_page_del(texu_i64 pg, void *userdata);
texu_pagewnd_page *_texu_pagewnd_page_new(texu_wnd *);

texu_i32
_TexuPageCtrlProc_OnGetPageIndex(texu_wnd *wnd, texu_wnd *page)
{
    return _TexuPageCtrlProc_GetPageIndex(wnd, page);
}

texu_i32
_TexuPageCtrlProc_OnGetCountPage(texu_wnd *wnd)
{
    texu_i32 children = texu_wnd_children(wnd);
    return children;
}

texu_i32
_TexuPageCtrlProc_GetPageIndex(texu_wnd *wnd, texu_wnd *whatpage)
{
    texu_wnd *page = texu_wnd_firstchild(wnd);
    texu_i32 i = 0;
    while (page)
    {
        if (page == whatpage)
        {
            return i;
        }
        page = texu_wnd_nextwnd(page);
        ++i; /* next index */
    }
    return -1;
}

texu_wnd *
_TexuPageCtrlProc_GetPage(texu_wnd *wnd, texu_i32 idx)
{
    texu_wnd *page = texu_wnd_firstchild(wnd);
    texu_i32 i = 0;
    for (i = 0; i <= idx; ++i)
    {
        page = texu_wnd_nextwnd(page);
    }
    return page;
}

texu_wnd *
_TexuPageCtrlProc_GetNextPageEnabled(texu_wnd *wnd, texu_wnd *page)
{
    texu_wnd *nextpage = texu_wnd_nextwnd(page);
    while (nextpage)
    {
        if (texu_wnd_is_enable(nextpage))
        {
            return nextpage;
        }
        nextpage = texu_wnd_nextwnd(nextpage);
    }
    return 0;
}

texu_wnd *
_TexuPageCtrlProc_GetPrevPageEnabled(texu_wnd *wnd, texu_wnd *page)
{
    texu_wnd *prevpage = texu_wnd_prevwnd(page);
    while (prevpage)
    {
        if (texu_wnd_is_enable(prevpage))
        {
            return prevpage;
        }
        prevpage = texu_wnd_prevwnd(prevpage);
    }
    return 0;
}

texu_wnd *
_TexuPageCtrlProc_GetFirstPageEnabled(texu_wnd *wnd)
{
    texu_wnd *page = texu_wnd_firstchild(wnd);
    while (page)
    {
        if (texu_wnd_is_enable(page))
        {
            return page;
        }
        page = texu_wnd_nextwnd(page);
    }
    return page;
}

texu_wnd *
_TexuPageCtrlProc_GetLastPageEnabled(texu_wnd *wnd)
{
    texu_wnd *page = texu_wnd_lastchild(wnd);
    while (page)
    {
        if (texu_wnd_is_enable(page))
        {
            return page;
        }
        page = texu_wnd_prevwnd(page);
    }
    return page;
}

texu_wnd *
_TexuPageCtrlProc_OnSetCurPage(texu_wnd *wnd, texu_i32 idx)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    texu_wnd *curpage = pgctl->curpage;
    texu_wnd *newpage = 0;
    texu_i32 children = texu_wnd_children(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    if (idx < 0 || idx >= children)
    {
        return curpage;
    }
    newpage = _TexuPageCtrlProc_GetPage(wnd, idx);
    return _TexuPageCtrlProc_SetCurPage(wnd, newpage);
}

texu_wnd *
_TexuPageCtrlProc_SetCurPage(texu_wnd *wnd, texu_wnd *newpage)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    texu_wnd *curpage = pgctl->curpage;

    if (newpage)
    {
        if (!(texu_wnd_is_enable(newpage)))
        {
            /* try to get the next enable page */
            newpage = _TexuPageCtrlProc_GetNextPageEnabled(wnd, newpage);
            if (!(texu_wnd_is_enable(newpage)))
            {
                /* no more page enabled */
                return curpage;
            }
        }
        if (curpage)
        {
            texu_wnd_send_msg(pgctl->activewnd, TEXU_WM_KILLFOCUS, (texu_i64)pgctl->activewnd, 0);
            texu_wnd_visible(curpage, TEXU_FALSE);
        }
        pgctl->curpage = newpage;
        pgctl->firstactive = texu_wnd_get_first_activechild(newpage);
        pgctl->lastactive = texu_wnd_get_last_activechild(newpage);
        pgctl->activewnd = pgctl->firstactive;
        texu_wnd_visible(newpage, TEXU_TRUE);
        texu_wnd_send_msg(pgctl->activewnd, TEXU_WM_SETFOCUS, (texu_i64)pgctl->activewnd, 0);
        texu_wnd_invalidate(wnd);
    }
    return curpage;
}

texu_wnd *
_TexuPageCtrlProc_OnGetCurPage(texu_wnd *wnd)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    return pgctl->curpage;
}

texu_wnd *
_TexuPageCtrl_CreatePage(
    texu_wnd *wnd,
    const texu_char *clsname,
    texu_ui32 id)
{
    texu_wnd_attrs attrs;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd *childwnd = 0;
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 h = texu_wnd_get_height(wnd);
    texu_i32 w = texu_wnd_get_width(wnd);

    childwnd = texu_wnd_new(env);
    if (!childwnd)
    {
        return 0;
    }
    memset(&attrs, 0, sizeof(attrs));
    attrs.y     = y;
    attrs.x     = x;
    attrs.height    = h;
    attrs.width     = w;
    attrs.enable    = TEXU_TRUE;
    attrs.visible   = TEXU_TRUE;
    attrs.text      = TEXT("");
    attrs.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
    attrs.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
    attrs.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
#if (defined WIN32 && defined _WINDOWS)
    attrs.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
    attrs.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
    attrs.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
    attrs.selectedbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
#endif
    attrs.id = id;
    attrs.clsname = clsname;
    attrs.userdata = 0;
    attrs.style = 0;
    attrs.exstyle = 0;

    rc = texu_wnd_create(childwnd, wnd, &attrs);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(childwnd);
        return 0;
    }

    return childwnd;
}

void
_texu_pagewnd_page_del(texu_i64 pg, void *userdata)
{
    texu_pagewnd_page *pgitem = (texu_pagewnd_page *)pg;
    free(pgitem);
}

texu_pagewnd_page *
_texu_pagewnd_page_new(texu_wnd *page)
{
    texu_pagewnd_page *pgitem = 0;
    pgitem = (texu_pagewnd_page *)malloc(sizeof(texu_pagewnd_page));
    if (pgitem)
    {
        pgitem->page = page;
        pgitem->firstchild = texu_wnd_get_first_activechild(page);
        pgitem->lastchild = texu_wnd_get_last_activechild(page);
        pgitem->activechild = pgitem->firstchild;
    }
    return pgitem;
}

texu_status
_TexuPageCtrlProc_OnAddPage(texu_wnd *wnd, const texu_char *clsname, texu_ui32 id)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    texu_wnd *page = 0;
    texu_pagewnd_page *pgitem = 0;
    texu_i32 nitems = texu_array_count(pgctl->pages);

    page = _TexuPageCtrl_CreatePage(wnd, clsname, id);
    if (!(page))
    {
        return TEXU_NOMEM;
    }
    if (!(pgctl->curpage))
    {
        _TexuPageCtrlProc_SetCurPage(wnd, page);
    }
    else
    {
        texu_wnd_visible(page, TEXU_FALSE);
    }
    pgitem = _texu_pagewnd_page_new(page);
    texu_array_set(pgctl->pages, (texu_i64)pgitem, nitems);

    return TEXU_OK;
}

texu_status
_TexuPageCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_pagewnd *pgctl = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    pgctl = (texu_pagewnd *)malloc(sizeof(texu_pagewnd));
    if (!pgctl)
    {
        return TEXU_NOMEM;
    }

    memset(pgctl, 0, sizeof(texu_pagewnd));
    pgctl->pages = texu_array_new(TEXU_MAX_PAGE);

    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_WINDOW),
                       texu_env_get_syscolor(env, TEXU_COLOR_WINDOW));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_WINDOW));
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW));
    texu_wnd_set_bgfocused_color(wnd,
                               texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW));
#endif
    texu_wnd_set_userdata(wnd, pgctl);

    return TEXU_OK;
}

void
_TexuPageCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    texu_array_cb_free(
        pgctl->pages,
        _texu_pagewnd_page_del,
        wnd);
    texu_array_del(pgctl->pages);
    free(pgctl);
}

void
_TexuPageCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
}

void
_TexuPageCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_pagewnd *pgctl = (texu_pagewnd *)texu_wnd_get_userdata(wnd);
    texu_wnd *curpage = pgctl->curpage;
    texu_wnd *newpage = 0;
    texu_wnd *activewnd = pgctl->activewnd;
    texu_wnd *nextwnd = 0;
    texu_i32 y = 0;
    texu_i32 x = 0;
    texu_i32 width = 0;
    texu_cio *cio = 0;
    texu_status rc = TEXU_OK;
    texu_wnd* qnextwnd = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if ((TEXU_KEYPRESSED_CTRL & alt) && ((TEXUTEXT('k') == ch) || (TEXUTEXT('K') == ch)))
    {
        pgctl->ctl_k_pressed = !pgctl->ctl_k_pressed;
        return;
    }
    switch (ch)
    {
        case TEXUTEXT('p'):
        case TEXUTEXT('P'):
        {
            if ((TEXU_KEYPRESSED_CTRL & alt) && (pgctl->ctl_k_pressed))
            {
                /* continue as KEY_PPAGE doing */
                newpage = _TexuPageCtrlProc_GetPrevPageEnabled(wnd, curpage);
                pgctl->ctl_k_pressed = TEXU_FALSE;
            }
            break;
        }
        case TEXU_KEY_PPAGE:
        {
            if (alt & TEXU_KEYPRESSED_ALT)
            {
                newpage = _TexuPageCtrlProc_GetPrevPageEnabled(wnd, curpage);
                pgctl->ctl_k_pressed = TEXU_FALSE;
            }
            break;
        }
        case TEXUTEXT('n'):
        case TEXUTEXT('N'):
        {
            if ((TEXU_KEYPRESSED_CTRL & alt) && (pgctl->ctl_k_pressed))
            {
                /* continue as KEY_NPAGE doing */
                newpage = _TexuPageCtrlProc_GetNextPageEnabled(wnd, curpage);
                pgctl->ctl_k_pressed = TEXU_FALSE;
            }
            break;
        }
        case TEXU_KEY_NPAGE:
        {
            if (alt & TEXU_KEYPRESSED_ALT)
            {
                newpage = _TexuPageCtrlProc_GetNextPageEnabled(wnd, curpage);
                pgctl->ctl_k_pressed = TEXU_FALSE;
            }
            break;
        }
    }
    if (newpage)
    {
        _TexuPageCtrlProc_SetCurPage(wnd, newpage);
    }
    else if (curpage)
    {
        if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
        {
            texu_menu *menu = texu_wnd_get_menu(curpage);
            if (menu)
            {
                rc = texu_wnd_send_msg(curpage, TEXU_WM_ENTERMENU, ch, alt);
                return;
            }
        }
        if (activewnd && activewnd != wnd)
        {
            if (ch == chNextKey)
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYNEXTWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_next_activechild(curpage, activewnd);
                }
            }
            else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYPREVWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_prev_activechild(curpage, activewnd);
                }
            }

            /* kill and set the new active window */
            if (activewnd)
            {
                if (nextwnd)
                {
                    /*active child may need to be done its action*/
                    texu_wnd *activechild = texu_wnd_get_activechild(activewnd);
                    if (activechild)
                    {
                        rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_i64)activewnd, 0);
                        if (rc != TEXU_OK)
                        {
                            texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_i64)activechild, 0);
                            return;
                        }
                    }
                    /*active window need to be done something by itself*/
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                        return;
                    }
                    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                    y = texu_wnd_get_y(nextwnd);
                    x = texu_wnd_get_x(nextwnd);
                    width = texu_wnd_get_width(nextwnd);

                    cio = texu_wnd_get_cio(curpage);
                    texu_cio_gotoyx(cio, y, x + width - 1);

                    /* the new active window */
                    pgctl->activewnd = nextwnd;
                    return;
                }
                else
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                    return;
                }
            }
            else
            {
                texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                return;
            }
        } /* child window is active */
        else
        {
            /* the current frame window is active */
            if (activewnd)
            {
                activewnd = texu_wnd_get_activechild(activewnd);
                texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                return;
            }
        }
    }
}

texu_i64
_TexuPageCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
    case TEXU_WM_CREATE:
        return _TexuPageCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_WM_PAINT:
        _TexuPageCtrlProc_OnPaint(wnd, (texu_cio *)param1);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuPageCtrlProc_OnDestroy(wnd);
        break;

    case TEXU_WM_CHAR:
        _TexuPageCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_PGM_ADDPAGE:
        return _TexuPageCtrlProc_OnAddPage(wnd, (const texu_char *)param1, (texu_ui32)param2);

    case TEXU_PGM_SETCURPAGE:
        return (texu_i64)_TexuPageCtrlProc_OnSetCurPage(wnd, (texu_i32)param1);

    case TEXU_PGM_GETCURPAGE:
        return (texu_i64)_TexuPageCtrlProc_OnGetCurPage(wnd);

    case TEXU_PGM_GETPAGEINDEX:
        return _TexuPageCtrlProc_OnGetPageIndex(wnd, (texu_wnd *)param1);

    case TEXU_PGM_COUNTPAGE:
        return _TexuPageCtrlProc_OnGetCountPage(wnd);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#if 0 /* OBSOLETED*/
/*
#-------------------------------------------------------------------------------
# TexU text ctrl
#
     1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_MAX_CHARPERLINE (COLS)

struct texu_textwnd_sentence
{
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 color;
    void *userdata;
};
typedef struct texu_textwnd_sentence texu_textwnd_sentence;

struct texu_textwnd_paragraph
{
    texu_list *sentences;
    void *userdata;
};
typedef struct texu_textwnd_paragraph texu_textwnd_paragraph;

struct texu_textwnd
{
    texu_list *paragraphs;
    texu_textwnd_paragraph *curpara;
    void *exparam;
};
typedef struct texu_textwnd texu_textwnd;

texu_textwnd_sentence *_texu_textwnd_sentence_new(const texu_char *, texu_i32, void *);
void _texu_textwnd_sentence_del(texu_textwnd_sentence *);
void _texu_textwnd_sentence_set_text(texu_textwnd_sentence *, const texu_char *);
texu_i32 _texu_textwnd_sentence_get_text(texu_textwnd_sentence *, texu_char *, texu_i32);

texu_textwnd_paragraph *_texu_textwnd_paragraph_new(void *);
void _texu_textwnd_paragraph_del(texu_textwnd_paragraph *);
void _texu_textwnd_paragraph_add(texu_textwnd_paragraph *, texu_textwnd_sentence *);
void _texu_textwnd_paragraph_remove(texu_textwnd_paragraph *, texu_textwnd_sentence *);
texu_textwnd_sentence *_texu_textwnd_paragraph_get(texu_textwnd_paragraph *, texu_i32);
texu_list_item *_texu_textwnd_paragraph_get_item(texu_textwnd_paragraph *, texu_i32);
void _texu_textwnd_paragraph_set(texu_textwnd_paragraph *, texu_i32, texu_textwnd_sentence *);
void _texu_textwnd_paragraph_insert(texu_textwnd_paragraph *, texu_i32, texu_textwnd_sentence *);

void _texu_textwnd_add(texu_textwnd *, texu_textwnd_paragraph *);
void _texu_textwnd_remove(texu_textwnd *, texu_textwnd_paragraph *);
texu_textwnd_paragraph *_texu_textwnd_get(texu_textwnd *, texu_i32);
texu_list_item *_texu_textwnd_get_item(texu_textwnd *, texu_i32);
void _texu_textwnd_set(texu_textwnd *, texu_i32, texu_textwnd_paragraph *);
void _texu_textwnd_insert(texu_textwnd *, texu_i32, texu_textwnd_paragraph *);

texu_status _TexuTextCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuTextCtrlProc_OnDestroy(texu_wnd *wnd);
void _TexuTextCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
void _TexuTextCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);

texu_textwnd_sentence *
_texu_textwnd_sentence_new(const texu_char *text, texu_i32 color, void *userdata)
{
    texu_textwnd_sentence *snt = (texu_textwnd_sentence *)malloc(sizeof(texu_textwnd_sentence));
    if (snt)
    {
        memset(snt, 0, sizeof(texu_textwnd_sentence));
        texu_strcpy(snt->text, (text ? text : TEXT("")));
        snt->color = color;
        snt->userdata = userdata;
    }
    return snt;
}

void
_texu_textwnd_sentence_del(texu_textwnd_sentence *snt)
{
    if (snt)
    {
        free(snt);
    }
}

void
_texu_textwnd_sentence_set_text(texu_textwnd_sentence *snt, const texu_char *text)
{
    texu_i32 len = (text ? texu_strlen(text) : 0);
    if (len >= TEXU_MAX_WNDTEXT)
    {
        texu_memset(snt->text, 0, sizeof(snt->text));
        memcpy(snt->text, text, TEXU_MAX_WNDTEXT);
    }
    else
    {
        texu_strcpy(snt->text, (text ? text : TEXT("")));
    }
}

texu_i32
_texu_textwnd_sentence_get_text(texu_textwnd_sentence *snt, texu_char *buf, texu_i32 buflen)
{
    texu_i32 len = texu_strlen(snt->text);
    if (!(buf) || buflen < 0)
    {
        return len;
    }
    memset(buf, 0, buflen);
    if (buflen > len)
    {
        buflen = len;
    }
    texu_strcpy(buf, snt->text);
    return buflen;
}

texu_textwnd_paragraph *
_texu_textwnd_paragraph_new(void *userdata)
{
    texu_textwnd_paragraph *para = (texu_textwnd_paragraph *)malloc(sizeof(texu_textwnd_paragraph));
    if (para)
    {
        memset(para, 0, sizeof(texu_textwnd_paragraph));
        para->sentences = texu_list_new();
        para->userdata = userdata;
    }
    return para;
}

void
_texu_textwnd_paragraph_del(texu_textwnd_paragraph *para)
{
    texu_textwnd_sentence *snt = 0;
    texu_list_item *item = texu_list_first(para->sentences);
    while (item)
    {
        snt = (texu_textwnd_sentence *)item->data;
        _texu_textwnd_sentence_del(snt);
        item = item->next;
    }
    texu_list_del(para->sentences);
    free(para);
}

void
_texu_textwnd_paragraph_add(texu_textwnd_paragraph *para, texu_textwnd_sentence *snt)
{
    texu_list_add(para->sentences, (texu_i64)snt);
}

void
_texu_textwnd_paragraph_remove(texu_textwnd_paragraph *para, texu_textwnd_sentence *snt)
{
    texu_list_item *item = texu_list_find_first(para->sentences, (texu_i64)snt);
    if (item)
    {
        texu_list_remove(para->sentences, item);
    }
}

texu_list_item *
_texu_textwnd_paragraph_get_item(texu_textwnd_paragraph *para, texu_i32 idx)
{
    texu_i32 i = 0;
    texu_list_item *item = texu_list_first(para->sentences);
    if (idx < 0)
    {
        return 0;
    }
    while ((i < idx) && item)
    {
        item = item->next;
    }
    return item;
}

texu_textwnd_sentence *
_texu_textwnd_paragraph_get(texu_textwnd_paragraph *para, texu_i32 idx)
{
    texu_list_item *item = _texu_textwnd_paragraph_get_item(para, idx);
    if (item)
    {
        return (texu_textwnd_sentence *)item->data;
    }
    return 0;
}

void
_texu_textwnd_paragraph_set(texu_textwnd_paragraph *para, texu_i32 idx, texu_textwnd_sentence *newsnt)
{
    texu_list_item *item = _texu_textwnd_paragraph_get_item(para, idx);
    texu_textwnd_sentence *oldsnt = 0;

    if (item)
    {
        oldsnt = (texu_textwnd_sentence *)item->data;
        /* replace the new sentence if it is possible */
        if (newsnt)
        {
            if (newsnt != oldsnt)
            {
                item->data = (texu_i64)newsnt;
            }
        }
        else
        {
            /* if the new sentence is null, remove the old from the paragraph */
            texu_list_remove(para->sentences, item);
        }
        /* delete the old sentence */
        _texu_textwnd_sentence_del(oldsnt);
    }
    else if (newsnt)
    {
        /*add the new paragraph*/
        _texu_textwnd_paragraph_add(para, newsnt);
    }
}

void
_texu_textwnd_paragraph_insert(texu_textwnd_paragraph *para, texu_i32 idx, texu_textwnd_sentence *newsnt)
{
    texu_list_item *item = _texu_textwnd_paragraph_get_item(para, idx);

    if (item && newsnt)
    {
        /*insert after*/
        texu_list_insert(para->sentences, item, (texu_i64)newsnt);
    }
    else if (newsnt)
    {
        /*add the new sentence*/
        _texu_textwnd_paragraph_add(para, newsnt);
    }
}

void
_texu_textwnd_add(texu_textwnd *txt, texu_textwnd_paragraph *para)
{
    texu_list_add(txt->paragraphs, (texu_i64)para);
}

void
_texu_textwnd_remove(texu_textwnd *txt, texu_textwnd_paragraph *para)
{
    texu_list_item *item = texu_list_find_first(txt->paragraphs, (texu_i64)para);
    if (item)
    {
        texu_list_remove(txt->paragraphs, item);
    }
}

texu_textwnd_paragraph *
_texu_textwnd_get(texu_textwnd *txt, texu_i32 idx)
{
    texu_list_item *item = _texu_textwnd_get_item(txt, idx);
    if (item)
    {
        return (texu_textwnd_paragraph *)item->data;
    }
    return 0;
}

texu_list_item *
_texu_textwnd_get_item(texu_textwnd *txt, texu_i32 idx)
{
    texu_i32 i = 0;
    texu_list_item *item = texu_list_first(txt->paragraphs);
    if (idx < 0)
    {
        return 0;
    }
    while ((i < idx) && item)
    {
        item = item->next;
    }
    return item;
}

void
_texu_textwnd_set(texu_textwnd *txt, texu_i32 idx, texu_textwnd_paragraph *newpara)
{
    texu_list_item *item = _texu_textwnd_get_item(txt, idx);
    texu_textwnd_paragraph *oldpara = 0;

    if (item)
    {
        oldpara = (texu_textwnd_paragraph *)item->data;
        /* replace the new sentence if it is possible */
        if (newpara)
        {
            if (newpara != oldpara)
            {
                item->data = (texu_i64)newpara;
            }
        }
        else
        {
            /* if the new sentence is null, remove the old from the paragraph */
            texu_list_remove(txt->paragraphs, item);
        }
        /* delete the old sentence */
        _texu_textwnd_paragraph_del(oldpara);
    }
    else if (newpara)
    {
        /*add the new paragraph*/
        _texu_textwnd_add(txt, newpara);
    }
}

void
_texu_textwnd_insert(texu_textwnd *txt, texu_i32 idx, texu_textwnd_paragraph *newpara)
{
    texu_list_item *item = _texu_textwnd_get_item(txt, idx);

    if (item && newpara)
    {
        /*insert after*/
        texu_list_insert(txt->paragraphs, item, (texu_i64)newpara);
    }
    else if (newpara)
    {
        /*add the new paragraph*/
        _texu_textwnd_add(txt, newpara);
    }
}

texu_status
_TexuTextCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_textwnd *txt = (texu_textwnd *)malloc(sizeof(texu_textwnd));
    if (!txt)
    {
        return TEXU_NOMEM;
    }
    memset(txt, 0, sizeof(texu_textwnd));
    txt->paragraphs = texu_list_new();

    texu_wnd_set_userdata(wnd, txt);
    return TEXU_OK;
}

void
_TexuTextCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = 0;
    texu_list_item *item = texu_list_first(txt->paragraphs);
    while (item)
    {
        para = (texu_textwnd_paragraph *)item->data;
        _texu_textwnd_paragraph_del(para);
        item = item->next;
    }
    texu_list_del(txt->paragraphs);
    free(txt);
}

void
_TexuTextCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    /*
    texu_textwnd* txt = (texu_textwnd*)texu_wnd_get_userdata(wnd);
    if (!(texu_wnd_is_visible(wnd)))
    {
      return;
    }
    */
}

void
_TexuTextCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    /*
    texu_textwnd* txt = (texu_textwnd*)texu_wnd_get_userdata(wnd);
    */
}

texu_status _TexuTextCtrlProc_OnInsertSentence(texu_wnd *wnd, texu_i32 sntidx, const texu_char *text);
texu_status _TexuTextCtrlProc_OnAddSentence(texu_wnd *wnd, const texu_char *text);
void _TexuTextCtrlProc_OnRemoveSentence(texu_wnd *wnd, texu_i32 sntidx);
texu_status _TexuTextCtrlProc_OnAddParagraph(texu_wnd *wnd);
void _TexuTextCtrlProc_OnRemoveParagraph(texu_wnd *wnd, texu_i32 paraidx);
void _TexuTextCtrlProc_OnSetCurParagraph(texu_wnd *wnd, texu_i32 paraidx);

texu_status
_TexuTextCtrlProc_OnInsertParagraph(texu_wnd *wnd, texu_i32 paraidx)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = 0;
    texu_i32 count = 0;
    para = _texu_textwnd_paragraph_new(0);
    if (!(para))
    {
        return TEXU_NOMEM;
    }
    count = texu_list_count(txt->paragraphs);
    _texu_textwnd_insert(txt, paraidx, para);
    /* set current paragraph */
    if (0 == count)
    {
        txt->curpara = para;
    }
    return TEXU_OK;
}

void
_TexuTextCtrlProc_OnSetCurParagraph(texu_wnd *wnd, texu_i32 paraidx)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = _texu_textwnd_get(txt, paraidx);
    if (para)
    {
        txt->curpara = para;
    }
}

void
_TexuTextCtrlProc_OnRemoveParagraph(texu_wnd *wnd, texu_i32 paraidx)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_list_item *paraitem = _texu_textwnd_get_item(txt, paraidx);
    texu_textwnd_paragraph *para = 0;
    if (paraitem)
    {
        para = (texu_textwnd_paragraph *)paraitem->data;
        /* move the current paragraph if it is being deleted */
        if (txt->curpara == para)
        {
            if (paraitem != texu_list_first(txt->paragraphs))
            {
                paraitem = paraitem->prev;
            }
            else
            {
                paraitem = paraitem->next;
            }
            txt->curpara = (paraitem ? (texu_textwnd_paragraph *)paraitem->data : 0);
        }
        _texu_textwnd_remove(txt, para);
    }
}

texu_status
_TexuTextCtrlProc_OnAddParagraph(texu_wnd *wnd)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = 0;
    texu_i32 count = 0;
    para = _texu_textwnd_paragraph_new(0);
    if (!(para))
    {
        return TEXU_NOMEM;
    }
    count = texu_list_count(txt->paragraphs);
    _texu_textwnd_add(txt, para);
    /* set current paragraph */
    if (0 == count)
    {
        txt->curpara = para;
    }
    return TEXU_OK;
}

void
_TexuTextCtrlProc_OnRemoveSentence(texu_wnd *wnd, texu_i32 sntidx)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = txt->curpara;
    texu_textwnd_sentence *snt = 0;

    if (!(para))
    {
        return;
    }
    snt = _texu_textwnd_paragraph_get(para, sntidx);
    if (!(snt))
    {
        return;
    }
    _texu_textwnd_paragraph_remove(para, snt);
}

texu_status
_TexuTextCtrlProc_OnInsertSentence(texu_wnd *wnd, texu_i32 sntidx, const texu_char *text)
{
    /* this fuction to insert a new sentence in the current paragraph only */
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = txt->curpara;
    texu_textwnd_sentence *snt = 0;
    texu_list_item *paraitem = 0;
    if (!(para))
    {
        paraitem = texu_list_last(txt->paragraphs);
        if (!(paraitem)) /* there is no one paragraph at the moment*/
        {
            /* new paragraph */
            if (TEXU_OK != _TexuTextCtrlProc_OnAddParagraph(wnd))
            {
                return TEXU_NOMEM;
            }
        }
        para = txt->curpara;
    }
    /* new a sentence */
    snt = _texu_textwnd_sentence_new(text, TEXU_CIO_COLOR_WHITE_BLACK, 0);
    if (!snt)
    {
        return TEXU_NOMEM;
    }
    /* add the new sentence the current paragraph */
    _texu_textwnd_paragraph_insert(para, sntidx, snt);
    return TEXU_OK;
}

texu_status
_TexuTextCtrlProc_OnAddSentence(texu_wnd *wnd, const texu_char *text)
{
    texu_textwnd *txt = (texu_textwnd *)texu_wnd_get_userdata(wnd);
    texu_textwnd_paragraph *para = txt->curpara;
    texu_textwnd_sentence *snt = 0;
    texu_list_item *paraitem = 0;
    if (!(para))
    {
        paraitem = texu_list_last(txt->paragraphs);
        if (!(paraitem)) /* there is no one paragraph at the moment*/
        {
            /* new paragraph */
            if (TEXU_OK != _TexuTextCtrlProc_OnAddParagraph(wnd))
            {
                return TEXU_NOMEM;
            }
        }
        para = txt->curpara;
    }
    /* new a sentence */
    snt = _texu_textwnd_sentence_new(text, TEXU_CIO_COLOR_WHITE_BLACK, 0);
    if (!snt)
    {
        return TEXU_NOMEM;
    }
    /* add the new sentence the current paragraph */
    _texu_textwnd_paragraph_add(para, snt);

    return TEXU_OK;
}

texu_i64
_TexuTextCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    return 0; /*not implement this class*/
    switch (msg)
    {
    case TEXU_WM_CREATE:
        return _TexuTextCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_WM_PAINT:
        _TexuTextCtrlProc_OnPaint(wnd, (texu_cio *)param1);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuTextCtrlProc_OnDestroy(wnd);
        break;

    case TEXU_WM_CHAR:
        _TexuTextCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_TXCM_ADDSENTENCE:
        return _TexuTextCtrlProc_OnAddSentence(wnd, (const texu_char *)param1);

    case TEXU_TXCM_REMOVESENTENCE:
        _TexuTextCtrlProc_OnRemoveSentence(wnd, (texu_i32)param1);
        return 0;

    case TEXU_TXCM_INSERTSENTENCE:
        return _TexuTextCtrlProc_OnInsertSentence(wnd, (texu_i32)param1, (const texu_char *)param2);

    case TEXU_TXCM_SETSENTENCE:
        return 0;
    case TEXU_TXCM_GETSENTENCE:
        return 0;
    case TEXU_TXCM_ADDPARAGRAPH:
        return _TexuTextCtrlProc_OnAddParagraph(wnd);

    case TEXU_TXCM_REMOVEPARAGRAPH:
        _TexuTextCtrlProc_OnRemoveParagraph(wnd, (texu_i32)param1);
        return 0;

    case TEXU_TXCM_INSERTPARAGRAPH:
        return _TexuTextCtrlProc_OnInsertParagraph(wnd, (texu_i32)param1);

    case TEXU_TXCM_SETPARAGRAPH:
        return 0;
    case TEXU_TXCM_GETPARAGRAPH:
        return 0;
    case TEXU_TXCM_SETCURPARAGRAPH:
        _TexuTextCtrlProc_OnSetCurParagraph(wnd, (texu_i32)param1);
        return 0;
    case TEXU_TXCM_GETCURPARAGRAPH:
        return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#endif /* OBSOLETED*/

/*
#-------------------------------------------------------------------------------
# TexU ReBar
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_rbwnd
{
    texu_list           *bands;
    texu_i32            nbands;
    texu_i32            capwidth;

    texu_rbwnd_band     *firstvisibleband;
    texu_rbwnd_band     *lastvisibleband;
    texu_rbwnd_band     *curband;

    void *exparam;
};
typedef struct texu_rbwnd texu_rbwnd;

void _TexuReBarProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void _TexuReBarProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_i32 _TexuReBarProc_OnAddBand(texu_wnd *wnd, const texu_rbwnd_band* band);
void _TexuReBarProc_OnDestroy(texu_wnd *wnd);
texu_status _TexuReBarProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
texu_i32 _TexuReBarProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd);
void     _TexuReBarProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
texu_wnd* _TexuReBarProc_OnQueryNextWnd(texu_wnd* wnd);
texu_wnd* _TexuReBarProc_OnQueryPrevWnd(texu_wnd* wnd);


void _TexuReBarProc_SetFirstVisibleBand(texu_rbwnd *rbwnd, texu_wnd *childwnd);
void _TexuReBarProc_SetCurBand(texu_rbwnd *rbwnd, texu_wnd *childwnd);
void _TexuReBarProc_SetCurBandByBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band);
texu_list_item *_TexuReBarProc_FindBand(texu_rbwnd *rbwnd, texu_wnd *childwnd);
texu_list_item *_TexuReBarProc_FindBandByBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band);
texu_rbwnd_band *_TexuReBarProc_GetNextActiveBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band);
texu_rbwnd_band *_TexuReBarProc_GetPrevActiveBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band);
texu_rbwnd_band *_TexuReBarProc_GetBestFirstBand(texu_wnd *wnd, texu_rbwnd_band *band);
texu_rbwnd_band *_TexuReBarProc_GetFirstActiveBand(texu_rbwnd *rbwnd);
texu_rbwnd_band *_TexuReBarProc_GetLastActiveBand(texu_rbwnd *rbwnd);

texu_rbwnd_band *_TexuReBarProc_GetBestFirstBand(texu_wnd *wnd, texu_rbwnd_band *curband)
{
    texu_rbwnd *rbwnd = (texu_rbwnd*)texu_wnd_get_userdata(wnd);
    texu_list_item *item = _TexuReBarProc_FindBandByBand(rbwnd, curband);
    texu_rbwnd_band *band = 0;
    texu_rbwnd_band *prevband = 0;
    texu_i32 height = texu_wnd_get_height(wnd);
    texu_i32 h = 0;
    texu_i32 hchilds = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        h = band->height;
        
        /* draw caption if need*/
        if (1 > h)
        {
            h = 1;
        }
        /* check if there are row left to draw a childwnd*/
        if ((hchilds + h) > height)
        {
            return prevband; /*the previous band is the best first visible*/
        }
        
        /*get prev band*/
        prevband = band;
        
        item = item->prev;

        hchilds += h;
    }
    return curband;
}

texu_rbwnd_band *_TexuReBarProc_GetFirstActiveBand(texu_rbwnd *rbwnd)
{
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        if (texu_wnd_is_visible(band->childwnd) && 
            texu_wnd_is_enable(band->childwnd))
        {
            return band;
        }
        item = item->next;
    }
    return 0;
}

texu_rbwnd_band *_TexuReBarProc_GetLastActiveBand(texu_rbwnd *rbwnd)
{
    texu_list_item *item = texu_list_last(rbwnd->bands);
    texu_rbwnd_band *band = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        if (texu_wnd_is_visible(band->childwnd) &&
            texu_wnd_is_enable(band->childwnd))
        {
            return band;
        }
        item = item->prev;
    }
    return 0;
}

texu_rbwnd_band *_TexuReBarProc_GetNextActiveBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band)
{
    texu_list_item* item = _TexuReBarProc_FindBandByBand(rbwnd, band);
    while (item)
    {
        item = item->next;
        if (!item)
        {
            break;
        }
        band = (texu_rbwnd_band*)item->data;
        if (band &&
            texu_wnd_is_visible(band->childwnd) &&
            texu_wnd_is_enable(band->childwnd))
        {
            return band;
        }
    }
    return 0;
}

texu_rbwnd_band *_TexuReBarProc_GetPrevActiveBand(texu_rbwnd *rbwnd, texu_rbwnd_band *band)
{
    texu_list_item* item = _TexuReBarProc_FindBandByBand(rbwnd, band);
    while (item)
    {
        item = item->prev;
        if (!item)
        {
            break;
        }
        band = (texu_rbwnd_band*)item->data;
        if (band &&
            texu_wnd_is_visible(band->childwnd) &&
            texu_wnd_is_enable(band->childwnd))
        {
            return band;
        }
    }
    return 0;
}

void _TexuReBarProc_SetCurBand(texu_rbwnd *rbwnd, texu_wnd *childwnd)
{
    texu_list_item *item = _TexuReBarProc_FindBand(rbwnd, childwnd);

    if (item)
    {
        rbwnd->curband = (texu_rbwnd_band*)item->data;
    }
}

void _TexuReBarProc_SetCurBandByBand(texu_rbwnd *rbwnd, texu_rbwnd_band* band)
{
    if (band != rbwnd->curband)
    {
        rbwnd->curband = band;
    }
}

void _TexuReBarProc_SetFirstVisibleBand(texu_rbwnd *rbwnd, texu_wnd *childwnd)
{
    texu_list_item *item = _TexuReBarProc_FindBand(rbwnd, childwnd);

    if (item)
    {
        rbwnd->firstvisibleband = (texu_rbwnd_band*)item->data;
    }
}

texu_list_item *_TexuReBarProc_FindBandByBand(texu_rbwnd *rbwnd, texu_rbwnd_band *findband)
{
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        if (band == findband)
        {
            return item;
        }
        item = item->next;
    }
    return 0;
}

texu_list_item *_TexuReBarProc_FindBand(texu_rbwnd *rbwnd, texu_wnd *childwnd)
{
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        if (band->childwnd == childwnd)
        {
            return item;
        }
        item = item->next;
    }
    return 0;
}

texu_wnd* _TexuReBarProc_OnQueryNextWnd(texu_wnd* wnd)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)texu_wnd_get_userdata(wnd);
    texu_list_item *item = _TexuReBarProc_FindBandByBand(rbwnd, rbwnd->curband);
    texu_rbwnd_band *band = 0;
    if (item && item->next != 0)
    {
        item = item->next;
        band = (texu_rbwnd_band*)item->data;
        return band->childwnd;
    }
    return 0;
}

texu_wnd* _TexuReBarProc_OnQueryPrevWnd(texu_wnd* wnd)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)texu_wnd_get_userdata(wnd);
    texu_list_item *item = _TexuReBarProc_FindBandByBand(rbwnd, rbwnd->curband);
    texu_rbwnd_band *band = 0;
    if (item && item->prev != 0)
    {
        item = item->prev;
        band = (texu_rbwnd_band*)item->data;
        return band->childwnd;
    }
    return 0;
}

void _TexuReBarProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)texu_wnd_get_userdata(wnd);
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (rbwnd->curband)
    {
        texu_wnd_send_msg(rbwnd->curband->childwnd, TEXU_WM_SETFOCUS, 0, 0);
    }
    else if (item)
    {
        rbwnd->firstvisibleband = rbwnd->curband = (texu_rbwnd_band*)item->data;
        /*find the first focus*/
        band = _TexuReBarProc_GetFirstActiveBand(rbwnd);
        if (band)
        {
            texu_wnd_send_msg(band->childwnd, TEXU_WM_SETFOCUS, 0, 0);
        }
    }
}

texu_i32 _TexuReBarProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)texu_wnd_get_userdata(wnd);
    texu_list_item *item = texu_list_first(rbwnd->bands);
    if (item)
    {
        rbwnd->firstvisibleband = rbwnd->curband = (texu_rbwnd_band*)item->data;
        texu_wnd_invalidate(wnd);
    }
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_OK;
}

void _TexuReBarProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 y = 0;
    texu_i32 x = 0;
    texu_i32 width = 0;
    texu_cio *cio = 0;
    texu_status rc = TEXU_OK;
    texu_wnd *activewnd = rbwnd->curband->childwnd;
    texu_wnd *nextwnd = 0;
    texu_rbwnd_band* nextband = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);
    texu_bool fInvalidate = TEXU_FALSE;
    texu_bool fPrevMove = TEXU_FALSE;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (activewnd && activewnd != wnd)
    {
        if (ch == chNextKey)
        {
            nextband = _TexuReBarProc_GetNextActiveBand(rbwnd, rbwnd->curband);
            if (nextband)
            {
                nextwnd = nextband->childwnd;
            }
        }
        else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
        {
            nextband = _TexuReBarProc_GetPrevActiveBand(rbwnd, rbwnd->curband);
            if (nextband)
            {
                nextwnd = nextband->childwnd;
                fPrevMove = TEXU_TRUE;
            }
        }

        /* kill and set the new active window */
        if (activewnd)
        {
            if (nextwnd)
            {
                /*active child may need to be done its action*/
                texu_wnd *activechild = texu_wnd_get_activechild(activewnd);
                if (activechild)
                {
                    rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_i64)activewnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_i64)activechild, 0);
                        return;
                    }
                }
                /*active window need to be done something by itself*/
                rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (rc != TEXU_OK)
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                    return;
                }
                rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                y = texu_wnd_get_y(nextwnd);
                x = texu_wnd_get_x(nextwnd);
                width = texu_wnd_get_width(nextwnd);

                cio = texu_wnd_get_cio(wnd);
                texu_cio_gotoyx(cio, y, x + width - 1);

                /* the new active window */
                if (nextband != rbwnd->curband)
                {
                    _TexuReBarProc_SetCurBandByBand(rbwnd, nextband);
                    fInvalidate = TEXU_TRUE;
                }

                /*if the last visible and the next band */
                if (TEXU_FALSE == fPrevMove && /*rbwnd->lastvisibleband &&*/
                    nextband == _TexuReBarProc_GetNextActiveBand(rbwnd, rbwnd->lastvisibleband))
                {
                    /* find the best first visible */
                    rbwnd->firstvisibleband = _TexuReBarProc_GetBestFirstBand(wnd, nextband);
                    texu_wnd_invalidate(wnd);
                }
                else if (TEXU_TRUE == fPrevMove &&
                         nextband == _TexuReBarProc_GetPrevActiveBand(rbwnd, rbwnd->firstvisibleband))
                {
                    rbwnd->firstvisibleband = _TexuReBarProc_GetBestFirstBand(wnd, nextband);
                    texu_wnd_invalidate(wnd);
                }

                return;
            }
            else
            {
                texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                return;
            }
        }
        else
        {
            texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
            return;
        }
    } /* child window is active */
    else
    {
        /* the current frame window is active */
        if (activewnd)
        {
            activewnd = texu_wnd_get_activechild(activewnd);
            texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
            return;
        }
    }

}

void _TexuReBarProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_rbwnd *rbwnd = (texu_rbwnd*)texu_wnd_get_userdata(wnd);
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_i32 height = texu_wnd_get_height(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_REBAR);
    texu_char caption[TEXU_MAX_WNDTEXT + 1];
    texu_i32 h = 0;
    texu_i32 w = 0;
    texu_i32 hchilds = 0;
    texu_bool skip = TEXU_TRUE;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 capwidth = rbwnd->capwidth;
#if (defined WIN32 && defined _WINDOWS)
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_REBAR);
#endif
    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
    while (item)
    {
        capwidth = rbwnd->capwidth;
        band = (texu_rbwnd_band*)item->data;
        if (skip && band != rbwnd->firstvisibleband)
        {
            /*hide child wnd if not found the first visible band*/
            texu_wnd_move(band->childwnd, 0, 0, 0, 0, TEXU_TRUE);
            item = item->next;
            continue;
        }
        else if (TEXU_FALSE == texu_wnd_is_visible(band->childwnd))
        {
            item = item->next;
            continue;
        }
        else if (band == rbwnd->firstvisibleband)
        {
            if (skip)
            {
                skip = TEXU_FALSE;
            }
        }
        h = band->height;
        w = band->width;
        
        /* draw caption if need*/
        if (1 > h)
        {
            h = 1;
        }
        /* check if there are row left to draw a childwnd*/
        if ((hchilds + h) > height)
        {
            /*try if the next band height = 1*/
            if (h != 1)
            {
                h = height - hchilds;
            }
            if ((hchilds + h) > height)
            {
                texu_wnd_move(band->childwnd, 0, 0, 0, 0, TEXU_TRUE);
            }
            else
            {
                /*draw caption*/
                if (TEXU_RBS_NOCAPTION & style)
                {
                    /*nothing*/
                    capwidth = 0;
                }
                else
                {
                    texu_printf_alignment2(
                        caption,
                        band->caption,
                        rbwnd->capwidth,
                        band->align,
                        TEXU_TRUE);

#ifdef TEXU_CIO_COLOR_MONO
                    texu_cio_putstr_attr(dc, y, x, caption,
                                         texu_cio_get_color(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
                    texu_env_draw_text(env, y, x, caption, color, bgcolor);
#else
                    texu_cio_putstr_attr(dc, y, x, caption,
                                         texu_cio_get_color(dc, color));
#endif
#endif
                }
                /*clip window*/
                if (width < x+/*rbwnd->*/capwidth+1+w)
                {
                    w = width - (x+/*rbwnd->*/capwidth+1);
                    if (w < 0)
                    {
                        w = 0;
                    }
                }
                texu_wnd_move(band->childwnd, y, x+/*rbwnd->*/capwidth+1, h, w, TEXU_TRUE);
            }
            item = item->next;
            continue; /*no more draw*/
        }
        /*draw caption*/
        if (TEXU_RBS_NOCAPTION & style)
        {
            /*nothing*/
            capwidth = 0;
        }
        else
        {
            texu_printf_alignment2(
                caption,
                band->caption,
                rbwnd->capwidth,
                band->align, TEXU_TRUE);

#ifdef TEXU_CIO_COLOR_MONO
            texu_cio_putstr_attr(dc, y, x, caption,
                                 texu_cio_get_color(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
            texu_env_draw_text(env, y, x, caption, color, bgcolor);
#else
            texu_cio_putstr_attr(dc, y, x, caption,
                                 texu_cio_get_color(dc, color));
#endif
#endif
        }
        
        /*draw window*/
        texu_wnd_move(band->childwnd, y, x+/*rbwnd->*/capwidth+1, h, w, TEXU_TRUE);

        /*save the last visible band*/
        rbwnd->lastvisibleband = band;
        
        /*get next band*/
        item = item->next;

        /*increment y*/
        y += h;
        hchilds += h;
    }
}

texu_i32 _TexuReBarProc_OnAddBand(texu_wnd *wnd, const texu_rbwnd_band* band)
{
    texu_rbwnd *rbwnd = texu_wnd_get_userdata(wnd);
    texu_rbwnd_band *newband = (texu_rbwnd_band*)malloc(sizeof(texu_rbwnd_band));
    if (!newband)
    {
        return -1;
    }
    memcpy(newband, band, sizeof(texu_rbwnd_band));
    
    texu_list_insert_last(rbwnd->bands, (texu_i64)newband);
    if (!rbwnd->firstvisibleband)
    {
        rbwnd->firstvisibleband = newband;
        rbwnd->curband = newband;
        rbwnd->lastvisibleband = newband;
    }

    ++rbwnd->nbands;
    return 0;
}

void _TexuReBarProc_OnDestroy(texu_wnd *wnd)
{
    texu_rbwnd *rbwnd = (texu_rbwnd*)texu_wnd_get_userdata(wnd);
    texu_list_item *item = texu_list_first(rbwnd->bands);
    texu_rbwnd_band *band = 0;

    while (item)
    {
        band = (texu_rbwnd_band*)item->data;
        texu_wnd_destroy(band->childwnd);
        item = item->next;
    }
    texu_list_del(rbwnd->bands);
    free(rbwnd);
}

texu_status _TexuReBarProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_rbwnd *rbwnd = (texu_rbwnd *)malloc(sizeof(texu_rbwnd));
    if (!rbwnd)
    {
        return TEXU_NOMEM;
    }
    memset(rbwnd, 0, sizeof(texu_rbwnd));
    rbwnd->bands = texu_list_new();
    rbwnd->capwidth = attrs->width/2;

    texu_wnd_set_userdata(wnd, rbwnd);
    return TEXU_OK;
}

void _TexuReBarProc_OnSetInfoWidth(texu_wnd *wnd, texu_i32 width)
{
    texu_rbwnd *rbwnd = (texu_rbwnd*)texu_wnd_get_userdata(wnd);
    rbwnd->capwidth = width;
}

texu_i64
_TexuReBarProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
            return _TexuReBarProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuReBarProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuReBarProc_OnDestroy(wnd);
            break;

        case TEXU_WM_CHAR:
            _TexuReBarProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_QUERYNEXTWND:
            return (texu_i64)_TexuReBarProc_OnQueryNextWnd(wnd);

        case TEXU_WM_QUERYPREVWND:
            return (texu_i64)_TexuReBarProc_OnQueryPrevWnd(wnd);

        case TEXU_WM_KILLFOCUS:
            return _TexuReBarProc_OnKillFocus(wnd, (texu_wnd*)param1);

        case TEXU_WM_SETFOCUS:
            _TexuReBarProc_OnSetFocus(wnd, (texu_wnd*)param1);
            return 0;

        case TEXU_RBM_ADDBAND:
            return _TexuReBarProc_OnAddBand(wnd, (const texu_rbwnd_band*)param1);

        case TEXU_RBM_SETINFOWIDTH:
            _TexuReBarProc_OnSetInfoWidth(wnd, param1);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
