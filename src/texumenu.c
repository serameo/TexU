/*
#
# File Name: texumenu.c
# Author: Seree Rakwong
# Date: 05-JUN-2020
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
#include "texumenu.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
#-------------------------------------------------------------------------------
# TexU menu
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct _texu_popup_menu_stack
{
    texu_i32 x;
    texu_i32 y;
    texu_popup_menu         *popup;
    texu_popup_menu_item    *curitem;
};
typedef struct _texu_popup_menu_stack _texu_popup_menu_stack;

#define MAX_POPUP_LEVEL     10

struct texu_menu
{
    texu_wnd *wndbar;
    texu_i32 id;
    texu_tree *tree;
    texu_wnd *owner;
    texu_bool showpopup;
    texu_popup_menu *curbaritem;
    texu_popup_menu_item *curitem;
    texu_i32  barid;
    texu_ui32 normcolor;
    texu_ui32 discolor;
    texu_ui32 selcolor;
    texu_ui32 normbg;
    texu_ui32 disbg;
    texu_ui32 selbg;
    /*level2*/
    texu_popup_menu *popupitem;
    texu_popup_menu_item *curpopitem;
    texu_popup_menu_item *prevpopitem;
    _texu_popup_menu_stack  stack[MAX_POPUP_LEVEL];  /*while showing popup menu holding _texu_popup_menu_stack*/
    texu_i32                idx_stack;
};

texu_menu_item *_texu_menu_item_new(texu_env *env, const texu_char *text, texu_i32 id, texu_bool enable, const texu_char *info);
void _texu_menu_item_del(texu_menu_item *);
texu_tree_item *_texu_menu_find_item(texu_menu *, texu_tree_item *, texu_i32 id);

void _TexuMenuProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);

void _TexuMenuWndroc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_i32 id, const texu_char *info);
void _TexuMenuWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect);
void _TexuMenuWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void _TexuMenuWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuMenuWndProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuMenuWndProc_OnDestroy(texu_wnd *);
#if (defined WIN32 && defined _WINDOWS)
void _TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor,
    texu_i32 normbg,
    texu_i32 selbg,
    texu_i32 disbg);
#else
void _TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor);
#endif
texu_i32 _TexuMenuWndProc_DrawPopupMenu2(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *popupitem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor,
    texu_i32 level);
texu_i32 _TexuMenuProc_GetMaxLength(texu_tree_item *baritem);
texu_tree_item *_TexuMenuWndProc_GetNextMenuItem(texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetPrevMenuItem(texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetNextMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetPrevMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem);

texu_menu_item *
_texu_menu_item_new(
    texu_env *env,
    const texu_char *text,
    texu_i32 id,
    texu_bool enable,
    const texu_char *info)
{
    texu_menu_item *item = (texu_menu_item *)malloc(sizeof(texu_menu_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_menu_item));
        texu_strcpy(item->text, (text ? text : TEXUTEXT("")));
        item->id = id;
        item->enable = enable;
        if (text)
        {
            item->style = TEXU_MS_TEXT;
        }
        else
        {
            item->style = TEXU_MS_BREAK;
            item->enable = TEXU_FALSE;
        }
        item->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
        item->discolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        item->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_SELECTED);

        item->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM);
        item->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        item->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_SELECTED);

        texu_strcpy(item->info, (info ? info : TEXUTEXT("")));
        
        item->maxwidth = 0;
        item->height = 1;
    }
    return item;
}

texu_menu_item *
_texu_menu_item_new2(
    texu_env *env,
    texu_i32 style,
    const texu_char *text,
    texu_i32 id,
    texu_bool enable,
    const texu_char *info)
{
    texu_menu_item *item = (texu_menu_item *)malloc(sizeof(texu_menu_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_menu_item));
        texu_strcpy(item->text, (text ? text : TEXUTEXT("")));
        item->id = id;
        item->enable = enable;
        item->style = style;
        if (TEXU_MS_BREAK == style)
        {
            item->enable = TEXU_FALSE;
        }
        item->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
        item->discolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        item->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_SELECTED);

        item->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM);
        item->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        item->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_SELECTED);

        texu_strcpy(item->info, (info ? info : TEXUTEXT("")));
        
        item->maxwidth = 0;
        item->height = 1;
    }
    return item;
}

void
_texu_menu_item_del(texu_menu_item *item)
{
    if (item)
    {
        free(item);
    }
}

texu_i32
_texu_menu_find_proc(texu_longptr itemdata, texu_longptr id, void *userdata)
{
    texu_menu_item *menuitem = (texu_menu_item *)itemdata;
    return (menuitem->id == id);
}

texu_tree_item *
_texu_menu_find_item(texu_menu *menu, texu_tree_item *startitem, texu_i32 id)
{
    texu_tree_item *finditem = 0;
    texu_tree_item *root = texu_tree_get_root(menu->tree);
    texu_menu_item *item = 0;

    if (!(startitem))
    {
        finditem = root->firstchild;
    }
    else
    {
        finditem = startitem->firstchild;
    }

    while (finditem)
    {
        item = (texu_menu_item *)finditem->data;
        if (item->id == id)
        {
            return finditem;
        }
        finditem = finditem->next;
    }

    return 0;
}

texu_wnd *
_texu_menu_create_wndbar(texu_menu *menu, texu_wnd *owner, texu_i32 id)
{
    texu_wnd_attrs attrs;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(owner);
    texu_wnd *wnd = texu_wnd_new(env);
    if (!wnd)
    {
        return 0;
    }

    memset(&attrs, 0, sizeof(texu_wnd_attrs));
    attrs.y = 0;
    attrs.x = 0;
    attrs.height    = 1;
    attrs.width     = COLS;
    attrs.enable    = TEXU_FALSE;
    attrs.visible   = TEXU_TRUE;
    attrs.text      = TEXUTEXT("");
    attrs.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_MENU_SELECTED);

    attrs.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    attrs.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU_SELECTED);

    attrs.id = id;
    attrs.clsname = TEXU_MENU_CLASS;
    attrs.userdata = menu;
    attrs.style = 0;
    attrs.exstyle = 0;

    rc = texu_wnd_create(wnd, owner, &attrs);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(wnd);
        return 0;
    }

    texu_wnd_set_menu(owner, menu);
    return wnd;
}

texu_tree_item *
texu_menu_find_item(texu_menu *menu, texu_i32 id)
{
    return _texu_menu_find_item(menu, 0, id);
}

texu_menu *
texu_menu_new(texu_wnd *owner, texu_i32 id)
{
    texu_wnd *wnd = 0;
    texu_menu *menu = (texu_menu *)malloc(sizeof(texu_menu));
    texu_env *env = texu_wnd_get_env(owner);

    if (menu)
    {
        memset(menu, 0, sizeof(texu_menu));
        menu->owner = owner;

        wnd = _texu_menu_create_wndbar(menu, owner, id);
        if (!wnd)
        {
            free(menu);
            return 0;
        }
        menu->tree = texu_tree_new();
        menu->wndbar = wnd;
        menu->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
        menu->discolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        menu->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_SELECTED);

        menu->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM);
        menu->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        menu->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM_SELECTED);
        /*stack*/
        menu->idx_stack = -1;
    }
    return menu;
}

void
texu_menu_del(texu_menu *menu)
{
    if (menu)
    {
        texu_tree_del(menu->tree);
        free(menu);
    }
}

texu_wnd *
texu_menu_get_menubar(texu_menu *menu)
{
    return (menu ? menu->wndbar : 0);
}

texu_popup_menu *
texu_menu_add_menu_info(
    texu_menu *menu,
    const texu_char *text,
    texu_bool enable,
    const texu_char *info)
{
    texu_i32 barid = menu->barid;
    ++menu->barid;
    return (texu_popup_menu*)texu_menu_add_item_info(menu, 0, text, barid, enable, info);
}

texu_popup_menu*   texu_menu_add_submenu_info(
                            texu_menu *menu, /*main menu*/
                            texu_popup_menu *popupitem, /*parent popup*/
                            const texu_char* text,
                            texu_i32 id,
                            texu_bool enable,
                            const texu_char* info)
{
    texu_tree_item *treeitem = 0;
    texu_env *env = texu_wnd_get_env(menu->owner);
    texu_i32 popid = id;//popupitem->nchildren;
    texu_menu_item *item = _texu_menu_item_new2(env, TEXU_MS_POPUP, text, (1+popid), enable, info);

    treeitem = texu_tree_add_item(menu->tree, popupitem, (texu_lparam)item);
    if (!(treeitem))
    {
        _texu_menu_item_del(item);
        return 0;
    }
    if (popupitem)
    {
        texu_menu_item *item = (texu_menu_item*)popupitem->data;
        texu_i32 itemlen = (text ? (texu_i32)strlen(text) : 0);
        if (item)
        {
            if (item->maxwidth < itemlen)
            {
                item->maxwidth = itemlen;
            }
            ++item->height;
        }
    }
    return (texu_popup_menu*)treeitem;

}

texu_popup_menu_item *
texu_menu_add_item_info(
    texu_menu *menu,
    texu_popup_menu *baritem,
    const texu_char *text,
    texu_i32 id,
    texu_bool enable,
    const texu_char *info)
{
    texu_tree_item *treeitem = 0;
    texu_env *env = texu_wnd_get_env(menu->owner);
    texu_menu_item *item = _texu_menu_item_new(env, text, id, enable, info);

    treeitem = texu_tree_add_item(menu->tree, baritem, (texu_lparam)item);
    if (!(treeitem))
    {
        _texu_menu_item_del(item);
        return 0;
    }
    if (baritem)
    {
        texu_menu_item *item = (texu_menu_item*)baritem->data;
        texu_i32 itemlen = (text ? (texu_i32)strlen(text) : 0);
        if (item)
        {
            if (item->maxwidth < itemlen)
            {
                item->maxwidth = itemlen;
            }
            ++item->height;
        }
    }
    return (texu_popup_menu_item*)treeitem;
}


texu_popup_menu_item *
texu_menu_add_subitem_info(
    texu_menu       *menu,
    texu_popup_menu *submenu,
    texu_i32        style,
    const texu_char *text,
    texu_i32        id,
    texu_bool       enable,
    const texu_char *info)
{
    texu_tree_item *treeitem = 0;
    texu_env *env = texu_wnd_get_env(menu->owner);
    texu_menu_item *item = _texu_menu_item_new2(env, style, text, id, enable, info);

    treeitem = texu_tree_add_item(menu->tree, submenu, (texu_lparam)item);
    if (!(treeitem))
    {
        _texu_menu_item_del(item);
        return 0;
    }
    if (submenu)
    {
        texu_menu_item *item = (texu_menu_item*)submenu->data;
        texu_i32 itemlen = (text ? (texu_i32)strlen(text) : 0);
        if (item)
        {
            if (item->maxwidth < itemlen)
            {
                item->maxwidth = itemlen;
            }
            ++item->height;
        }
    }
    return (texu_popup_menu_item*)treeitem;
}


texu_popup_menu *
texu_menu_add_menu(texu_menu *menu, const texu_char *text, texu_bool enable)
{
    return texu_menu_add_menu_info(menu, text, enable, TEXUTEXT(""));
}

texu_popup_menu *
texu_menu_get_menu(texu_menu *menu, texu_i32 id)
{
    texu_tree_item *baritem = _texu_menu_find_item(menu, 0, id);
    return (texu_popup_menu*)baritem;
}

texu_popup_menu *
texu_menu_set_curmenu(texu_menu *menu, texu_popup_menu *baritem)
{
    texu_tree_item *olditem = menu->curbaritem;
    menu->curbaritem = baritem;
    if (baritem)
    {
        menu->idx_stack = 0;
        menu->stack[0].popup = baritem;
        menu->stack[0].curitem = baritem->firstchild;
    }
    else
    {
        menu->idx_stack = -1;
        menu->stack[0].popup = 0;
        menu->stack[0].curitem = 0;
    }
    return olditem;
}

texu_popup_menu_item *
texu_menu_add_item(
    texu_menu *menu,
    texu_popup_menu *baritem,
    const texu_char *text,
    texu_i32 id,
    texu_bool enable)
{
    return (texu_popup_menu_item*)texu_menu_add_item_info(menu, baritem, text, id, enable, TEXUTEXT(""));
}

texu_popup_menu_item *
texu_menu_get_menuitem(texu_menu *menu, texu_popup_menu *baritem, texu_i32 id)
{
    texu_tree_item *item = _texu_menu_find_item(menu, baritem, id);
    return (texu_popup_menu_item*)item;
}

texu_popup_menu_item *
texu_menu_set_curmenuitem(texu_menu *menu, texu_popup_menu_item *item)
{
    texu_menu_item *menuitem = 0;
    texu_popup_menu_item *olditem = (texu_popup_menu_item*)menu->curitem;
    menu->curitem = item;
    if (item)
    {
        menuitem = (texu_menu_item *)item->data;
        _TexuMenuWndroc_NotifyItem(menu->wndbar, TEXU_MNN_ITEMCHANGED, menuitem->id, menuitem->info);
        if (0 == menu->idx_stack)
        {
            menu->stack[0].curitem = item;
        }
    }
    else
    {
        menu->idx_stack = -1;
        menu->stack[0].popup = 0;
        menu->stack[0].curitem = 0;
    }
    return olditem;
}

texu_bool
texu_menu_enable_menu(texu_menu *menu, texu_i32 idx, texu_bool enable)
{
    texu_menu_item *menuitem = 0;
    texu_bool oldenable = TEXU_FALSE;
    texu_tree_item *item = texu_menu_get_menu(menu, idx);
    if (item)
    {
        menuitem = (texu_menu_item *)item->data;
        oldenable = menuitem->enable;
        menuitem->enable = enable;
    }
    return oldenable;
}

texu_bool
texu_menu_enable_menuitem(texu_menu *menu, texu_popup_menu *baritem, texu_i32 id, texu_bool enable)
{
    texu_menu_item *menuitem = 0;
    texu_bool oldenable = TEXU_FALSE;
    texu_tree_item *item = texu_menu_get_menuitem(menu, baritem, id);
    if (item)
    {
        menuitem = (texu_menu_item *)item->data;
        oldenable = menuitem->enable;
        menuitem->enable = enable;
    }
    return oldenable;
}

void
_TexuMenuProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    texu_tree_item *treeitem = 0;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_menu_item *menuitem = 0;
    texu_i32 len = 0;
    texu_ui32 normcolor = menu->normcolor;
    texu_ui32 discolor = menu->discolor;
    texu_ui32 selcolor = menu->selcolor;

    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    texu_ui32 normbg = menu->normbg;
    texu_ui32 disbg = menu->disbg;
    texu_ui32 selbg = menu->selbg;
    _texu_popup_menu_stack *popup1 = &menu->stack[0];

    /*draw menu bar*/
    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!(texu_wnd_is_visible(wnd)))
    {
        return;
    }
    treeitem = texu_tree_get_root(menu->tree);
    treeitem = (treeitem ? treeitem->firstchild : 0);
    if (!(treeitem))
    {
        return;
    }
    /*draw bar*/
    texu_memset(buf, 0, sizeof(buf));
    texu_memset(buf, TEXUTEXT(' '), texu_env_screen_width(env));
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_draw_text2(dc, y, x, buf, normcolor, normbg, TB_REVERSE,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
#else
    texu_cio_draw_text(dc, y, x, buf, normcolor, normbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif

    while (treeitem)
    {
        menuitem = (texu_menu_item *)treeitem->data;
        len = texu_strlen(menuitem->text);
        texu_printf_alignment(buf, menuitem->text, len, TEXU_ALIGN_CENTER);

        if (menuitem->enable)
        {
            color = normcolor;

            bgcolor = normbg;

            /*color = (treeitem == menu->curbaritem ? selcolor : color);*/
            if (treeitem == popup1->popup) //menu->curbaritem)
            {
                color = selcolor;
                bgcolor = selbg;

                texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));

            }
            else
            {
#ifdef TEXU_CIO_COLOR_MONO
                texu_cio_draw_text2(dc, y, x, buf, color, bgcolor, TB_REVERSE,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));
#else
                texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));

#endif
            }
        }
        else
        {
            color = discolor;
            bgcolor = disbg;
#ifdef TEXU_CIO_COLOR_MONO
                texu_cio_draw_text2(dc, y, x, buf, color, bgcolor, TB_REVERSE,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));
#else
            texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                  texu_wnd_get_clsname(wnd),
                                  texu_wnd_get_id(wnd));

#endif
        }
        treeitem = treeitem->next;

        x += len;
    }
}

texu_longptr
_TexuMenuProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
    case TEXU_WM_PAINT:
        _TexuMenuProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

void
_TexuMenuWndroc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_i32 id, const texu_char *info)
{
    texu_menuitem_notify notify;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_wnd *parent = menu->owner;

    if (!parent)
    {
        parent = texu_wnd_get_parent(wnd);
    }

    memset(&notify, 0, sizeof(notify));
    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.id = id;
    texu_strcpy(notify.info, (info ? info : TEXUTEXT("")));
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}

void
_TexuMenuWndProc_OnDestroy(texu_wnd *wnd)
{
    texu_wnd *topwnd = 0;
    texu_env *env = 0;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);

    env = texu_wnd_get_env(wnd);
    texu_env_pop_wnd(env);

    texu_menu_set_curmenu(menu, 0);
    texu_menu_set_curmenuitem(menu, 0);

    topwnd = texu_env_top_wnd(env);

    texu_wnd_send_msg(topwnd, TEXU_WM_LEAVEMENU, 0, 0);
    texu_wnd_send_msg(topwnd, TEXU_WM_SHOW, TEXU_TRUE, 0);
    texu_wnd_invalidate(topwnd);
}

texu_tree_item *
_TexuMenuWndProc_GetNextMenuItem(texu_tree_item *curitem)
{
    texu_tree_item *item = curitem;
    texu_menu_item *menuitem = 0;
    if (item)
    {
        item = item->next;
    }
    while (item)
    {
        menuitem = (texu_menu_item *)item->data;
        if (menuitem->enable)
        {
            return item;
        }
        item = item->next;
    }
    return curitem;
}

texu_tree_item *
_TexuMenuWndProc_GetPrevMenuItem(texu_tree_item *curitem)
{
    texu_tree_item *item = curitem;
    texu_menu_item *menuitem = 0;
    if (item)
    {
        item = item->prev;
    }
    while (item)
    {
        menuitem = (texu_menu_item *)item->data;
        if (menuitem->enable)
        {
            return item;
        }
        item = item->prev;
    }
    return curitem;
}

texu_tree_item *
_TexuMenuWndProc_GetPrevMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem)
{
    texu_tree_item *item = 0;
    texu_menu_item *menuitem = 0;
    if (!parentitem || !parentitem->lastchild)
    {
        return 0;
    }
    if (curitem)
    {
        item = _TexuMenuWndProc_GetPrevMenuItem(curitem);
        menuitem = (texu_menu_item *)item->data;
        if (!(menuitem->enable) || item == curitem)
        {
            menuitem = (texu_menu_item *)parentitem->lastchild->data;
            if (!(menuitem->enable))
            {
                item = _TexuMenuWndProc_GetPrevMenuItem(parentitem->lastchild);
            }
            else
            {
                item = parentitem->lastchild;
            }
        }
    }
    else
    {
        menuitem = (texu_menu_item *)parentitem->lastchild->data;
        if (!(menuitem->enable))
        {
            item = _TexuMenuWndProc_GetPrevMenuItem(parentitem->lastchild);
        }
        else
        {
            item = parentitem->lastchild;
        }
    }
    return item;
}

texu_tree_item *
_TexuMenuWndProc_GetNextMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem)
{
    texu_tree_item *item = 0;
    texu_menu_item *menuitem = 0;
    if (!parentitem || !parentitem->firstchild)
    {
        return 0;
    }
    if (curitem)
    {
        item = _TexuMenuWndProc_GetNextMenuItem(curitem);
        menuitem = (texu_menu_item *)item->data;
        if (!(menuitem->enable) || item == curitem)
        {
            menuitem = (texu_menu_item *)parentitem->firstchild->data;
            if (!(menuitem->enable))
            {
                item = _TexuMenuWndProc_GetNextMenuItem(parentitem->firstchild);
            }
            else
            {
                item = parentitem->firstchild;
            }
        }
    }
    else
    {
        menuitem = (texu_menu_item *)parentitem->firstchild->data;
        if (!(menuitem->enable))
        {
            item = _TexuMenuWndProc_GetNextMenuItem(parentitem->firstchild);
        }
        else
        {
            item = parentitem->firstchild;
        }
    }
    return item;
}

void
_TexuMenuWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_popup_menu *baritem = menu->curbaritem;
    texu_popup_menu_item *curitem = menu->curitem;
    texu_tree_item *root = texu_tree_get_root(menu->tree);
    texu_menu_item *menuitem = (curitem ? (texu_menu_item *)curitem->data : 0);
    texu_popup_menu *baritem2 = menu->curbaritem;
    texu_popup_menu_item *curitem2 = menu->curitem;
    _texu_popup_menu_stack *popup1 = 0;
    _texu_popup_menu_stack *popup2 = 0;
    texu_i32 idx_stack = menu->idx_stack;
    if (idx_stack < 1)
    {
        menu->idx_stack = 0;
        popup1 = &menu->stack[menu->idx_stack];
        popup1->popup = baritem;
        popup1->curitem = curitem;
        popup1->x = 0;
        popup1->y = 0;
    }
    else if (idx_stack >= MAX_POPUP_LEVEL)
    {
        menu->idx_stack = MAX_POPUP_LEVEL - 1;
        popup1 = &menu->stack[menu->idx_stack];
    }
    else
    {
        popup1 = &menu->stack[menu->idx_stack];
    }
    popup2 = popup1;

    if (TEXU_KEY_SELMENU == ch) /*pressed enter*/
    {
        /*texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);*/
        texu_wnd_send_msg(menu->owner, TEXU_WM_COMMAND,
                          (menuitem ? (texu_lparam)menuitem->id : -1), 0);
        texu_wnd_destroy(wnd);
        return;
    }
    else if (TEXU_KEY_ESCAPE == ch) /*pressed escape*/
    {
        /*texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);*/
        texu_wnd_destroy(wnd);
        return;
    }

    if (!(baritem))
    {
        return;
    }
    baritem2 = baritem;
    curitem2 = curitem;
    if (menu->idx_stack > -1 && menu->idx_stack < MAX_POPUP_LEVEL)
    {
        baritem = popup1->popup;
        curitem = popup1->curitem;
    }
    switch (ch)
    {
        case TEXU_KEY_UP:
        {
            curitem = _TexuMenuWndProc_GetPrevMenuItemEnabled(popup1->popup, popup1->curitem);
            if (menu->idx_stack > -1 && menu->idx_stack < (MAX_POPUP_LEVEL))
            {
                menuitem = (texu_menu_item *)curitem->data;//popup1->curitem->data;
                if (0)//(TEXU_MS_POPUP == menuitem->style)
                {
                    /*expand submenu if need*/
                    ++menu->idx_stack;
                    popup1 = &menu->stack[menu->idx_stack];
                    /*set the new popup*/
                    popup1->popup = curitem;
                    popup1->curitem = popup1->popup->firstchild;
                }
                else
                {
                    popup1->curitem = curitem;
                }
            }
            if (0 == idx_stack)
            {
                menu->curitem = curitem;
            }
            break;
        }
        case TEXU_KEY_DOWN:
        {
            curitem = _TexuMenuWndProc_GetNextMenuItemEnabled(baritem, curitem);
            if (menu->idx_stack > -1 && menu->idx_stack < (MAX_POPUP_LEVEL))
            {
                menuitem = (texu_menu_item *)curitem->data;//popup1->curitem->data;
                if (0)//(TEXU_MS_POPUP == menuitem->style)
                {
                    /*expand submenu if need*/
                    ++menu->idx_stack;
                    popup1 = &menu->stack[menu->idx_stack];
                    /*set the new popup*/
                    popup1->popup = curitem;
                    popup1->curitem = popup1->popup->firstchild;
                }
                else
                {
                    popup1->curitem = curitem;
                }
            }
            if (0 == idx_stack)
            {
                menu->curitem = curitem;
            }
            break;
        }
        case TEXU_KEY_LEFT: /*collapse submenu if need*/
        {
            if (menu->idx_stack > 0 && menu->idx_stack < (MAX_POPUP_LEVEL))
            {
                menuitem = (texu_menu_item *)popup1->curitem->data;
                if (1)//(TEXU_MS_POPUP == menuitem->style)
                {
                    /*collapse submenu if need*/
                    --menu->idx_stack;
                    popup1 = &menu->stack[menu->idx_stack];
                }
            }
            if (0 == idx_stack)
            {
                baritem = _TexuMenuWndProc_GetPrevMenuItemEnabled(root, popup1[0].popup);
                menu->curbaritem = baritem;
                menu->curitem = baritem->firstchild;
                popup1[0].popup = baritem;
                popup1[0].curitem = popup1->popup->firstchild;
            }
            break;
        }
        case TEXU_KEY_RIGHT:
        {
            if (menu->idx_stack > -1 && menu->idx_stack < (MAX_POPUP_LEVEL - 1))
            {
                menuitem = (texu_menu_item *)popup1->curitem->data;
                if (TEXU_MS_POPUP == menuitem->style)
                {
                    if (0)//(0 == idx_stack)
                    {
                        baritem = _TexuMenuWndProc_GetNextMenuItemEnabled(root, popup1[0].popup);
                        menu->curbaritem = baritem;
                        menu->curitem = baritem->firstchild;
                        popup1[0].popup = baritem;
                        popup1[0].curitem = popup1->popup->firstchild;
                    }
                    else
                    {
                        /*expand submenu if need*/
                        ++menu->idx_stack;
                        popup2 = &menu->stack[menu->idx_stack];
                        /*set the new popup*/
                        popup2->popup = popup1->curitem;
                        popup2->curitem = popup1->curitem->firstchild;
                    }
                }
                else
                {
                    if (0 == idx_stack)
                    {
                        baritem = _TexuMenuWndProc_GetNextMenuItemEnabled(root, popup1[0].popup);
                        menu->curbaritem = baritem;
                        menu->curitem = baritem->firstchild;
                        popup1[0].popup = baritem;
                        popup1[0].curitem = popup1->popup->firstchild;
                    }
                }
            }
            break;
        }
    }
    menuitem = (texu_menu_item *)popup2->popup;//menu->curitem->data;
    _TexuMenuWndroc_NotifyItem(wnd, TEXU_MNN_ITEMCHANGED,
                            menuitem->id,
                            menuitem->info);
#if 0
    if (menu->curpopitem)
    {
        menuitem = (texu_menu_item *)menu->curpopitem->data;
        if (TEXU_MS_POPUP == menuitem->style)
        {
            menu->popupitem = menu->curpopitem;
            menu->curpopitem = menu->curpopitem->firstchild;
        }
        /*else
        {
            menu->popupitem = 0;
            menu->curpopitem = 0;
        }*/
        _TexuMenuWndroc_NotifyItem(wnd, TEXU_MNN_ITEMCHANGED,
                                   menuitem->id,
                                   menuitem->info);
    }
    else if (menu->curitem)
    {
        menuitem = (texu_menu_item *)menu->curitem->data;
        if (TEXU_MS_POPUP == menuitem->style)
        {
            menu->popupitem = menu->curitem;
            menu->curpopitem = menu->curitem->firstchild;
        }
        else
        {
            menu->popupitem = 0;
            menu->curpopitem = 0;
        }
        _TexuMenuWndroc_NotifyItem(wnd, TEXU_MNN_ITEMCHANGED,
                                   menuitem->id,
                                   menuitem->info);
    }
#endif
    texu_wnd_invalidate(wnd);
}

texu_i32
_TexuMenuProc_GetMaxLength(texu_tree_item *baritem)
{
    texu_i32 maxlen = -1;
    texu_i32 len = 0;
    texu_tree_item *item = baritem->firstchild;
    texu_menu_item *menuitem = 0;

    while (item)
    {
        menuitem = (texu_menu_item *)item->data;

        len = texu_strlen(menuitem->text);
        if (len > maxlen)
        {
            maxlen = len;
        }
        item = item->next;
    }
    return maxlen;
}
#if (defined WIN32 && defined _WINDOWS)
void
_TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor,
    texu_i32 normbg,
    texu_i32 selbg,
    texu_i32 disbg)
#else
void
_TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor)
#endif
{
    texu_tree_item *item = baritem->firstchild;
    texu_menu_item *menuitem = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 color = 0;
    texu_i32 maxlen = _TexuMenuProc_GetMaxLength(baritem);

    texu_i32 bgcolor = 0;
    texu_i32 width = 0;
    texu_i32 height = 1;
    texu_char blank[TEXU_MAX_WNDTEXT + 1];
    _texu_popup_menu_stack *popup1 = 0;
    
    if (menu->idx_stack > -1)
    {
        popup1 = &menu->stack[0];
    }

    /*texu_env *env = texu_wnd_get_env(wnd);*/
    menuitem = (texu_menu_item *)baritem->data;
    if (menuitem)
    {
        width  = menuitem->maxwidth;
        height = menuitem->height;
        /*draw shadow*/
#if !(defined __ESCAPE_CODE__)
        texu_i32 i = 0;
        memset(blank, ' ', width+2);
        blank[width+2] = 0;
        for (i = 0; i < height; ++i)
        {
            texu_cio_draw_text(dc, y+i, x+1, blank, color, bgcolor,
                                    texu_wnd_get_clsname(wnd),
                                    texu_wnd_get_id(wnd));
        }
#else
        /*draw correctly, but erase incompletely*/
        texu_char esc[TEXU_2048_WNDTEXT + 1];
        texu_term_sprint_shadow(esc, TEXU_2048_WNDTEXT, y, x+1, height, width+3);
        texu_term_write_screen(esc);
    
        texu_term_sprint_box(esc, TEXU_2048_WNDTEXT, y, x, height, width+2);
        texu_term_write_screen(esc);
#endif
    }
    /*draw top line*/
    {
#if !(defined __ESCAPE_CODE__)
        memset(blank, '-', width+2);
        blank[width+2] = 0;
        blank[0] = '+';
        blank[width+1] = '+';

        color = menuitem->normcolor;
        bgcolor = menuitem->normbg;
        texu_cio_draw_text(dc, y, x, blank, color, bgcolor,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
        ++y;
#endif
    }
    while (item)
    {
        menuitem = (texu_menu_item *)item->data;

        /*draw left border*/
        {
#if !(defined __ESCAPE_CODE__)
            strcpy(buf, "|");
            color = menuitem->normcolor;
            bgcolor = menuitem->normbg;
            texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
#endif
        }

        texu_printf_alignment(buf, menuitem->text, maxlen, TEXU_ALIGN_LEFT);

        color = menuitem->normcolor;
        if (popup1 && item == popup1->curitem)//(item == menu->curitem)
        {
            color = menuitem->selcolor;
            bgcolor = menuitem->selbg;

            if (TEXU_MS_POPUP == menuitem->style)
            {
                /**/
                _TexuMenuWndProc_DrawPopupMenu2(wnd, dc, menu, item, y, x+width+2, 
                    menuitem->normcolor, menuitem->selcolor, menuitem->discolor, 0);
            }
            //else
            {
                texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                    texu_wnd_get_clsname(wnd),
                                    texu_wnd_get_id(wnd));
            }

        }
        else
        {
            if (!(menuitem->enable))
            {
                if (TEXU_MS_BREAK == menuitem->style)
                {
                    memset(buf, '-', width);
                    buf[width] = 0;
                    color = menuitem->normcolor;
                    bgcolor = menuitem->normbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
                }
                else
                {
#ifdef TEXU_CIO_COLOR_MONO
                    color = menuitem->normcolor;
                    texu_cio_draw_text2(dc, y, x+1, buf, color, bgcolor, TB_REVERSE,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#else
                    color = menuitem->discolor;
                    bgcolor = menuitem->disbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#endif
                }
            }
            else
            {
                if (TEXU_MS_BREAK == menuitem->style)
                {
                    memset(buf, '-', width);
                    buf[width] = 0;
                    color = menuitem->normcolor;
                    bgcolor = menuitem->normbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
                }
                else
                {
                    bgcolor = menuitem->normbg;
#ifdef TEXU_CIO_COLOR_MONO
                    texu_cio_draw_text2(dc, y, x+1, buf, color, bgcolor, TB_REVERSE,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#else
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#endif
                }
            }
        }
        /*draw right border*/
        {
#if !(defined __ESCAPE_CODE__)
            strcpy(buf, "|");
            color = menuitem->normcolor;
            bgcolor = menuitem->normbg;
            texu_cio_draw_text(dc, y, x+width+1, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
#endif
        }
        item = item->next;
        ++y;
    }
    /*draw bottom line*/
    {
#if !(defined __ESCAPE_CODE__)
        memset(blank, '-', width+2);
        blank[width+2] = 0;
        blank[0] = '+';
        blank[width+1] = '+';

        color = menuitem->normcolor;
        bgcolor = menuitem->normbg;
        texu_cio_draw_text(dc, y, x, blank, color, bgcolor,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
#endif
    }

}

texu_i32 _TexuMenuWndProc_DrawPopupMenu2(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_popup_menu *popupitem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor,
    texu_i32 level)
{
    texu_tree_item *item = popupitem->firstchild;
    texu_menu_item *menuitem = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 color = 0;
    texu_i32 maxlen = _TexuMenuProc_GetMaxLength(popupitem);
    texu_i32 y_nextpopup = y;

    texu_i32 bgcolor = 0;
    texu_i32 width = 0;
    texu_i32 height = 1;
    texu_char blank[TEXU_MAX_WNDTEXT + 1];
    //texu_popup_menu_item *curitem = menu->curpopitem;
    texu_popup_menu_item *prevpopitem = menu->prevpopitem;
    _texu_popup_menu_stack *popup1 = &menu->stack[level];

    /*texu_env *env = texu_wnd_get_env(wnd);*/
    menuitem = (texu_menu_item *)popup1->popup->data;
    texu_popup_menu_item *curitem = popup1->curitem;
    if (menuitem)
    {
        width  = menuitem->maxwidth;
        height = menuitem->height;
        /*draw shadow*/
#if (defined __USE_TERMBOX2__)
#if (defined __UTF8__)
#else
        /*texu_i32 i = 0;
        memset(blank, ' ', width+2);
        blank[width+2] = 0;
        for (i = -1; i < height-1; ++i)
        {
            texu_cio_draw_text(dc, y+i, x+1, blank, color, bgcolor,
                                    texu_wnd_get_clsname(wnd),
                                    texu_wnd_get_id(wnd));
        }*/
#endif
#elif 0
        /*draw correctly, but erase incompletely*/
        texu_char esc[TEXU_2048_WNDTEXT + 1];
        texu_term_sprint_shadow(esc, TEXU_2048_WNDTEXT, y, x+1, height, width+3);
        texu_term_write_screen(esc);
    
        texu_term_sprint_box(esc, TEXU_2048_WNDTEXT, y, x, height, width+2);
        texu_term_write_screen(esc);
#endif
    }
    /*draw top line*/
    {
#if !(defined __ESCAPE_CODE__)
        memset(blank, '-', width+2);
        blank[width+2] = 0;
        blank[0] = '+';
        blank[width+1] = '+';

        color = menuitem->normcolor;
        bgcolor = menuitem->normbg;
        texu_cio_draw_text(dc, y, x, blank, color, bgcolor,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
        ++y;
#endif
    }
    while (item)
    {
        menuitem = (texu_menu_item *)item->data;

        /*draw left border*/
        {
#if !(defined __ESCAPE_CODE__)
            strcpy(buf, "|");
            color = menuitem->normcolor;
            bgcolor = menuitem->normbg;
            texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
#endif
        }

        texu_printf_alignment(buf, menuitem->text, maxlen, TEXU_ALIGN_LEFT);

        color = menuitem->normcolor;
        if (item == curitem)
        {
            color = menuitem->selcolor;
            bgcolor = menuitem->selbg;

            texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
            if (TEXU_MS_POPUP == menuitem->style)
            {
                y_nextpopup = y;
                /*_TexuMenuWndProc_DrawPopupMenu2(wnd, dc, menu, item, y, x+width+0,//2, 
                    menuitem->normcolor, menuitem->selcolor, menuitem->discolor, level);*/
            }
        }
        else
        {
            if (!(menuitem->enable))
            {
                if (TEXU_MS_BREAK == menuitem->style)
                {
                    memset(buf, '-', width);
                    buf[width] = 0;
                    color = menuitem->normcolor;
                    bgcolor = menuitem->normbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
                }
                else
                {
#ifdef TEXU_CIO_COLOR_MONO
                    color = menuitem->normcolor;
                    texu_cio_draw_text2(dc, y, x+1, buf, color, bgcolor, TB_REVERSE,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#else
                    color = menuitem->discolor;
                    bgcolor = menuitem->disbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#endif
                }
            }
            else
            {
                if (TEXU_MS_BREAK == menuitem->style)
                {
                    memset(buf, '-', width);
                    buf[width] = 0;
                    color = menuitem->normcolor;
                    bgcolor = menuitem->normbg;
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
                }
                else
                {
                    bgcolor = menuitem->normbg;
#ifdef TEXU_CIO_COLOR_MONO
                    texu_cio_draw_text2(dc, y, x+1, buf, color, bgcolor, TB_REVERSE,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#else
                    texu_cio_draw_text(dc, y, x+1, buf, color, bgcolor,
                                        texu_wnd_get_clsname(wnd),
                                        texu_wnd_get_id(wnd));
#endif
                }
            }
        }
        /*draw right border*/
        {
#if !(defined __ESCAPE_CODE__)
            strcpy(buf, "|");
            color = menuitem->normcolor;
            bgcolor = menuitem->normbg;
            texu_cio_draw_text(dc, y, x+width+1, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
#endif
        }
        item = item->next;
        ++y;
    }
    /*draw bottom line*/
    {
#if !(defined __ESCAPE_CODE__)
        memset(blank, '-', width+2);
        blank[width+2] = 0;
        blank[0] = '+';
        blank[width+1] = '+';

        color = menuitem->normcolor;
        bgcolor = menuitem->normbg;
        texu_cio_draw_text(dc, y, x, blank, color, bgcolor,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
#endif
    }
    return y_nextpopup;
}

void
_TexuMenuWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
    texu_tree_item *treeitem = 0;
    texu_menu_item *baritem = 0;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 len = 0;
    texu_ui32 normcolor = menu->normcolor;
    texu_ui32 discolor = menu->discolor;
    texu_ui32 selcolor = menu->selcolor;
    texu_i32 y_nextpopup = 0;
    texu_ui32 normbg = menu->normbg;
#if !(defined TEXU_CIO_COLOR_MONO) || (defined WIN32 && defined _WINDOWS)
    texu_ui32 disbg = menu->disbg;
#endif
    texu_ui32 selbg = menu->selbg;
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM);
    _texu_popup_menu_stack *popup1 = &menu->stack[0];
    /*draw menu bar*/
    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!(texu_wnd_is_visible(wnd)))
    {
        return;
    }
    texu_wnd_invalidate(menu->owner);

    treeitem = texu_tree_get_root(menu->tree);
    treeitem = (treeitem ? treeitem->firstchild : 0);
    if (!(treeitem))
    {
        return;
    }
    /*draw bar*/
    memset(buf, 0, sizeof(buf));
    texu_memset(buf, TEXUTEXT(' '), texu_env_screen_width(env));
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_draw_text2(dc, y, x, buf, normcolor, normbg, TB_REVERSE,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#else
    texu_cio_draw_text(dc, y, x, buf, normcolor, normbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif

    while (treeitem)
    {
        baritem = (texu_menu_item *)treeitem->data;
        len = texu_strlen(baritem->text);
        texu_printf_alignment(buf, baritem->text, len, TEXU_ALIGN_CENTER);

        if (baritem->enable)
        {
            color = normcolor;
            bgcolor = normbg;
#ifdef TEXU_CIO_COLOR_MONO
            texu_cio_draw_text2(dc, y, x, buf, color, bgcolor, TB_REVERSE,
                                  texu_wnd_get_clsname(wnd),
                                  texu_wnd_get_id(wnd));
#else
            texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                  texu_wnd_get_clsname(wnd),
                                  texu_wnd_get_id(wnd));

#endif
            /* draw popup menu */
            if (treeitem == popup1->popup)//(treeitem == menu->curbaritem)
            {
                /*shall safe the first popup menu*/
                color = selcolor;
                bgcolor = selbg;
                texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));

#if (defined WIN32 && defined _WINDOWS)
                _TexuMenuWndProc_DrawPopupMenu(
                    wnd,
                    dc,
                    menu,
                    treeitem,
                    y + 1,
                    x,
                    normcolor,
                    selcolor,
                    discolor,
                    normbg,
                    selbg,
                    disbg);
#else
                y_nextpopup = _TexuMenuWndProc_DrawPopupMenu2(
                    wnd,
                    dc,
                    menu,
                    treeitem,
                    y + 1,
                    x,
                    normcolor,
                    selcolor,
                    discolor,
                    0);
                /*draw child popup*/
                if (menu->idx_stack > 0 && menu->idx_stack < MAX_POPUP_LEVEL)
                {
                    /*loop draw popup*/
                    texu_i32 idx = 0;
                    texu_i32 ypopup = y_nextpopup;
                    texu_i32 maxlen = _TexuMenuProc_GetMaxLength(popup1->popup);
                    texu_i32 xpopup = x + maxlen;
                    _texu_popup_menu_stack *popup2 = 0;
                    for (idx = 1; idx <= menu->idx_stack; ++idx)
                    {
                        popup2  = &menu->stack[idx];
                        y_nextpopup = _TexuMenuWndProc_DrawPopupMenu2(
                            wnd,
                            dc,
                            menu,
                            popup2->popup,
                            ypopup,
                            xpopup + (idx * 2),
                            normcolor,
                            selcolor,
                            discolor,
                            idx);
                        /*next popup if need*/
                        maxlen = _TexuMenuProc_GetMaxLength(popup2->popup);
                        xpopup += maxlen;
                        ypopup = y_nextpopup;
                    }
                }
#endif
            }
        }
        else
        {
#ifdef TEXU_CIO_COLOR_MONO
            color = normcolor;
            texu_cio_draw_text2(dc, y, x, buf, color, bgcolor, TB_REVERSE,
                                  texu_wnd_get_clsname(wnd),
                                  texu_wnd_get_id(wnd));
#else
            color = discolor;
            bgcolor = disbg;

            texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                                  texu_wnd_get_clsname(wnd),
                                  texu_wnd_get_id(wnd));

#endif
        }
        treeitem = treeitem->next;

        x += len;
    }
}

texu_status
_TexuMenuWndProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_wnd_set_userdata(wnd, attrs->userdata);
    return TEXU_OK;
}


texu_longptr
_TexuMenuWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_KEYDOWN:
        /*case TEXU_WM_CHAR:*/
            _TexuMenuWndProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

        case TEXU_WM_PAINT:
            _TexuMenuWndProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_CREATE:
            _TexuMenuWndProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuMenuWndProc_OnDestroy(wnd);
            break;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

struct texu_track_popupwnd
{
    texu_menu       *menu;
    //texu_popup_menu *popup;
    texu_i32         id;
    texu_wnd        *owner;
};
/*popup menu*/
texu_wnd *texu_menu_track_menu_popup(texu_wnd *owner, texu_menu* menu, texu_i32 id, texu_i32 y, texu_i32 x)
{
    texu_wnd *wnd = 0;
    texu_wnd_attrs attrs;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(owner);
    texu_wnd *desktop = texu_env_get_desktop(env);
    //texu_popup_menu* popup = texu_menu_get_menu(menu, id);
    struct texu_track_popupwnd popwnd = { menu, id, owner };
    texu_wnd *frame = texu_wnd_get_frame(owner);

    wnd = texu_wnd_new(env);
    if (!wnd)
    {
        return 0;
    }
    memset(&attrs, 0, sizeof(attrs));
    attrs.y = y;
    attrs.x = x;
    attrs.height = 1;
    attrs.width = texu_env_screen_width(env);
    attrs.enable = TEXU_TRUE;
    attrs.visible = TEXU_TRUE;
    attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);

    attrs.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    attrs.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    attrs.text = TEXUTEXT("");

    attrs.id = 0xffffffff-1;
    attrs.clsname = TEXU_POPUPMENUWND_CLASS;
    attrs.userdata = &popwnd;
    attrs.style = 0;
    attrs.exstyle = 0;

    rc = texu_wnd_create(wnd, desktop, &attrs);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(wnd);
        return 0;
    }

    /*texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER, (texu_lparam)owner, 0);*/

    texu_env_push_wnd(env, wnd);
    texu_wnd_send_msg(wnd, TEXU_WM_ACTIVATED, 0, 0);
    texu_wnd_set_activechild(desktop, wnd);
    /*texu_wnd_visible(wnd, TEXU_TRUE);
    texu_wnd_invalidate(wnd);*/

    texu_wnd_post_msg(desktop, TEXU_WM_INVALIDATE, 0, 0);
    //texu_wnd_invalidate(desktop);
    //texu_wnd_invalidate(frame);
    return wnd;
}

void
_TexuPopupMenuWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
    struct texu_track_popupwnd *popwnd = (struct texu_track_popupwnd *)texu_wnd_get_userdata(wnd);
    texu_menu* menu = popwnd->menu;
    texu_popup_menu *treeitem = texu_menu_get_menu(menu, popwnd->id);
    texu_menu_item *menuitem = (texu_menu_item *)treeitem->data;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 len = 0;
    texu_ui32 normcolor = menuitem->normcolor;
    texu_ui32 discolor = menuitem->discolor;
    texu_ui32 selcolor = menuitem->selcolor;

    texu_ui32 normbg = menuitem->normbg;
#if !(defined TEXU_CIO_COLOR_MONO) || (defined WIN32 && defined _WINDOWS)
    texu_ui32 disbg = menuitem->disbg;
#endif
    texu_ui32 selbg = menuitem->selbg;
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENUITEM);

    /*draw menu bar*/
    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!(texu_wnd_is_visible(wnd)))
    {
        return;
    }
    //texu_wnd_invalidate(popwnd->owner);

    //treeitem = menu;//->firstchild;
    if (menuitem)//while (treeitem)
    {
        //menuitem = (texu_menu_item *)treeitem->data;
        /*len = texu_strlen(menuitem->text);
        texu_printf_alignment(buf, menuitem->text, len, TEXU_ALIGN_CENTER);*/

        if (menuitem->enable)
        {
            /* draw popup menu */
            //if (treeitem == menu->curbaritem)
            {
                color = selcolor;
                bgcolor = selbg;

#if (defined WIN32 && defined _WINDOWS)
                _TexuMenuWndProc_DrawPopupMenu(
                    wnd,
                    dc,
                    menu,
                    treeitem,
                    y + 1,
                    x,
                    normcolor,
                    selcolor,
                    discolor,
                    normbg,
                    selbg,
                    disbg);
#else
                _TexuMenuWndProc_DrawPopupMenu(
                    wnd,
                    dc,
                    menu,
                    treeitem,
                    y + 1,
                    x,
                    normcolor,
                    selcolor,
                    discolor);
#endif
            }
        }

    }
}

texu_status
_TexuPopupMenuWndProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    struct texu_track_popupwnd *popwnd = (struct texu_track_popupwnd*)malloc(sizeof(struct texu_track_popupwnd));
    *popwnd = *(struct texu_track_popupwnd *)attrs->userdata;
    texu_wnd_set_userdata(wnd, popwnd);
    return TEXU_OK;
}

void
_TexuPopupMenuWndProc_OnDestroy(texu_wnd *wnd)
{
    texu_wnd *topwnd = 0;
    texu_env *env = 0;
    struct texu_track_popupwnd *popwnd = (struct texu_track_popupwnd *)texu_wnd_get_userdata(wnd);

    env = texu_wnd_get_env(wnd);
    texu_env_pop_wnd(env);

    /*texu_menu_set_curmenu(menu, 0);
    texu_menu_set_curmenuitem(menu, 0);*/

    topwnd = texu_env_top_wnd(env);

    texu_wnd_send_msg(topwnd, TEXU_WM_LEAVEMENU, 0, 0);
    texu_wnd_send_msg(topwnd, TEXU_WM_SHOW, TEXU_TRUE, 0);
    texu_wnd_invalidate(topwnd);

    free(popwnd);
}

texu_longptr _TexuPopupMenuWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_DESTROY:
            _TexuPopupMenuWndProc_OnDestroy(wnd);
            return 0;

        /*case TEXU_WM_KEYDOWN:
            _TexuPopupMenuWndProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;*/
        
        case TEXU_WM_CREATE:
            _TexuPopupMenuWndProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
            return 0;

        case TEXU_WM_PAINT:
            _TexuPopupMenuWndProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
    //return _TexuMenuWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
