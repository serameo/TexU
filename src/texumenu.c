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

struct texu_menu
{
    texu_wnd *wndbar;
    texu_ui32 id;
    texu_tree *tree;
    texu_wnd *owner;
    texu_bool showpopup;
    texu_tree_item *curbaritem;
    texu_tree_item *curitem;
    texu_ui32 barid;
    texu_i32 normcolor;
    texu_i32 discolor;
    texu_i32 selcolor;
};

texu_menu_item *_texu_menu_item_new(texu_env *env, const texu_char *text, texu_ui32 id, texu_bool enable, const texu_char *info);
void _texu_menu_item_del(texu_menu_item *);
texu_tree_item *_texu_menu_find_item(texu_menu *, texu_tree_item *, texu_ui32 id);

void _TexuMenuProc_OnPaint(texu_wnd *, texu_cio *);

void _TexuMenuWndroc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_ui32 id, const texu_char *info);
void _TexuMenuWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
void _TexuMenuWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuMenuWndProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuMenuWndProc_OnDestroy(texu_wnd *);

void _TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_tree_item *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor);

texu_i32 _TexuMenuProc_GetMaxLength(texu_tree_item *baritem);
texu_tree_item *_TexuMenuWndProc_GetNextMenuItem(texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetPrevMenuItem(texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetNextMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem);
texu_tree_item *_TexuMenuWndProc_GetPrevMenuItemEnabled(texu_tree_item *parentitem, texu_tree_item *curitem);

texu_menu_item *
_texu_menu_item_new(
    texu_env *env,
    const texu_char *text,
    texu_ui32 id,
    texu_bool enable,
    const texu_char *info)
{
    texu_menu_item *item = (texu_menu_item *)malloc(sizeof(texu_menu_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_menu_item));
        strcpy(item->text, (text ? text : ""));
        item->id = id;
        item->enable = enable;
        item->style = (text ? TEXU_MS_TEXT : TEXU_MS_BREAK);
        item->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM);
        item->discolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_DISABLED);
        item->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENUITEM_SELECTED);
        strcpy(item->info, (info ? info : ""));
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
_texu_menu_find_proc(texu_i64 itemdata, texu_i64 id, void *userdata)
{
    texu_menu_item *menuitem = (texu_menu_item *)itemdata;
    return (menuitem->id == id);
}

texu_tree_item *
_texu_menu_find_item(texu_menu *menu, texu_tree_item *startitem, texu_ui32 id)
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
_texu_menu_create_wndbar(texu_menu *menu, texu_wnd *owner, texu_ui32 id)
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
    attrs.height = 1;
    attrs.width = COLS;
    attrs.enable = TEXU_FALSE;
    attrs.visible = TEXU_TRUE;
    attrs.text = "";
    attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focuscolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_SELECTED);
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
texu_menu_find_item(texu_menu *menu, texu_ui32 id)
{
    return _texu_menu_find_item(menu, 0, id);
}

texu_menu *
texu_menu_new(texu_wnd *owner, texu_ui32 id)
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
        menu->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
        menu->discolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
        menu->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_SELECTED);
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

texu_tree_item *
texu_menu_add_menu_info(
    texu_menu *menu,
    const texu_char *text,
    texu_bool enable,
    const texu_char *info)
{
    texu_ui32 barid = menu->barid;
    ++menu->barid;
    return texu_menu_add_item_info(menu, 0, text, barid, enable, info);
}
texu_tree_item *
texu_menu_add_item_info(
    texu_menu *menu,
    texu_tree_item *baritem,
    const texu_char *text,
    texu_ui32 barid,
    texu_bool enable,
    const texu_char *info)
{
    texu_tree_item *treeitem = 0;
    texu_env *env = texu_wnd_get_env(menu->owner);
    texu_menu_item *item = _texu_menu_item_new(env, text, barid, enable, info);

    treeitem = texu_tree_add_item(menu->tree, baritem, (texu_i64)item);
    if (!(treeitem))
    {
        _texu_menu_item_del(item);
        return 0;
    }
    return treeitem;
}

texu_tree_item *
texu_menu_add_menu(texu_menu *menu, const texu_char *text, texu_bool enable)
{
    return texu_menu_add_menu_info(menu, text, enable, "");
}

texu_tree_item *
texu_menu_get_menu(texu_menu *menu, texu_ui32 id)
{
    texu_tree_item *baritem = _texu_menu_find_item(menu, 0, id);
    return baritem;
}

texu_tree_item *
texu_menu_set_curmenu(texu_menu *menu, texu_tree_item *baritem)
{
    texu_tree_item *olditem = menu->curbaritem;
    menu->curbaritem = baritem;
    return olditem;
}

texu_tree_item *
texu_menu_add_item(
    texu_menu *menu,
    texu_tree_item *baritem,
    const texu_char *text,
    texu_ui32 id,
    texu_bool enable)
{
    return texu_menu_add_item_info(menu, baritem, text, id, enable, "");
}

texu_tree_item *
texu_menu_get_menuitem(texu_menu *menu, texu_tree_item *baritem, texu_ui32 id)
{
    texu_tree_item *item = _texu_menu_find_item(menu, baritem, id);
    return item;
}

texu_tree_item *
texu_menu_set_curmenuitem(texu_menu *menu, texu_tree_item *item)
{
    texu_menu_item *menuitem = 0;
    texu_tree_item *olditem = menu->curitem;
    menu->curitem = item;
    if (item)
    {
        menuitem = (texu_menu_item *)item->data;
        _TexuMenuWndroc_NotifyItem(menu->wndbar, TEXU_MNN_ITEMCHANGED, menuitem->id, menuitem->info);
    }
    return olditem;
}

texu_bool
texu_menu_enable_menu(texu_menu *menu, texu_ui32 idx, texu_bool enable)
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
texu_menu_enable_menuitem(texu_menu *menu, texu_tree_item *baritem, texu_ui32 id, texu_bool enable)
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
_TexuMenuProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    texu_tree_item *treeitem = 0;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_menu_item *menuitem = 0;
    texu_i32 len = 0;
    texu_i32 normcolor = menu->normcolor;
    texu_i32 discolor = menu->discolor;
    texu_i32 selcolor = menu->selcolor;

    /*draw menu bar*/
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
    memset(buf, 0, sizeof(buf));
    memset(buf, ' ', texu_env_screen_width(env));
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_reverse(dc, normcolor));
#else
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_color(dc, normcolor));
#endif

    while (treeitem)
    {
        menuitem = (texu_menu_item *)treeitem->data;
        len = strlen(menuitem->text);
        texu_printf_alignment(buf, menuitem->text, len, TEXU_ALIGN_CENTER);

        if (menuitem->enable)
        {
            color = normcolor;
            /*color = (treeitem == menu->curbaritem ? selcolor : color);*/
            if (treeitem == menu->curbaritem)
            {
                color = selcolor;
                texu_cio_putstr_attr(dc, y, x, buf,
                                     texu_cio_get_color(dc, color));
            }
            else
            {
#ifdef TEXU_CIO_COLOR_MONO
                texu_cio_putstr_attr(dc, y, x, buf,
                                     texu_cio_get_reverse(dc, color));
#else
                texu_cio_putstr_attr(dc, y, x, buf,
                                     texu_cio_get_color(dc, color));
#endif
            }
        }
        else
        {
            color = discolor;
#ifdef TEXU_CIO_COLOR_MONO
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_reverse(dc, color));
#else
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
#endif
        }
        treeitem = treeitem->next;

        x += len;
    }
}

texu_i64
_TexuMenuProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
    case TEXU_WM_PAINT:
        _TexuMenuProc_OnPaint(wnd, (texu_cio *)param1);
        return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

void
_TexuMenuWndroc_NotifyItem(texu_wnd *wnd, texu_ui32 code, texu_ui32 id, const texu_char *info)
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
    strcpy(notify.info, (info ? info : ""));
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
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
_TexuMenuWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_tree_item *baritem = menu->curbaritem;
    texu_tree_item *curitem = menu->curitem;
    texu_tree_item *root = texu_tree_get_root(menu->tree);
    texu_menu_item *menuitem = (curitem ? (texu_menu_item *)curitem->data : 0);

    if (TEXU_KEY_SELMENU == ch) /*pressed enter*/
    {
        texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);
        texu_wnd_send_msg(menu->owner, TEXU_WM_COMMAND,
                          (menuitem ? (texu_i64)menuitem->id : -1), 0);
        return;
    }
    else if (TEXU_KEY_ESCAPE == ch) /*pressed escape*/
    {
        texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);
        return;
    }

    if (!(baritem))
    {
        return;
    }
    switch (ch)
    {
    case KEY_UP:
    {
        curitem = _TexuMenuWndProc_GetPrevMenuItemEnabled(baritem, curitem);
        menu->curitem = curitem;
        break;
    }
    case KEY_DOWN:
    {
        curitem = _TexuMenuWndProc_GetNextMenuItemEnabled(baritem, curitem);
        menu->curitem = curitem;
        break;
    }
    case KEY_LEFT:
    {
        baritem = _TexuMenuWndProc_GetPrevMenuItemEnabled(root, baritem);
        menu->curbaritem = baritem;
        menu->curitem = baritem->firstchild;
        break;
    }
    case KEY_RIGHT:
    {
        baritem = _TexuMenuWndProc_GetNextMenuItemEnabled(root, baritem);
        menu->curbaritem = baritem;
        menu->curitem = baritem->firstchild;
        break;
    }
    }

    if (menu->curitem)
    {
        menuitem = (texu_menu_item *)menu->curitem->data;
        _TexuMenuWndroc_NotifyItem(wnd, TEXU_MNN_ITEMCHANGED,
                                   menuitem->id,
                                   menuitem->info);
    }
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

        len = strlen(menuitem->text);
        if (len > maxlen)
        {
            maxlen = len;
        }
        item = item->next;
    }
    return maxlen;
}

void
_TexuMenuWndProc_DrawPopupMenu(
    texu_wnd *wnd,
    texu_cio *dc,
    texu_menu *menu,
    texu_tree_item *baritem,
    texu_i32 y,
    texu_i32 x,
    texu_i32 normcolor,
    texu_i32 selcolor,
    texu_i32 discolor)
{
    texu_tree_item *item = baritem->firstchild;
    texu_menu_item *menuitem = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 color = 0;
    texu_i32 maxlen = _TexuMenuProc_GetMaxLength(baritem);

    while (item)
    {
        menuitem = (texu_menu_item *)item->data;

        texu_printf_alignment(buf, menuitem->text, maxlen, TEXU_ALIGN_LEFT);

        color = menuitem->normcolor;
        if (item == menu->curitem)
        {
            color = menuitem->selcolor;
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
        }
        else
        {
#ifdef TEXU_CIO_COLOR_MONO
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_reverse(dc, color));
#else
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
#endif
        }
        if (!(menuitem->enable))
        {
#ifdef TEXU_CIO_COLOR_MONO
            color = menuitem->normcolor;
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_reverse(dc, color));
#else
            color = menuitem->discolor;
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
#endif
        }
        item = item->next;
        ++y;
    }
}

void
_TexuMenuWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    texu_tree_item *treeitem = 0;
    texu_menu_item *baritem = 0;
    texu_menu *menu = (texu_menu *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 len = 0;
    texu_i32 normcolor = menu->normcolor;
    texu_i32 discolor = menu->discolor;
    texu_i32 selcolor = menu->selcolor;

    /*draw menu bar*/
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
    memset(buf, ' ', texu_env_screen_width(env));
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_reverse(dc, normcolor));
#else
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_color(dc, normcolor));
#endif

    while (treeitem)
    {
        baritem = (texu_menu_item *)treeitem->data;
        len = strlen(baritem->text);
        texu_printf_alignment(buf, baritem->text, len, TEXU_ALIGN_CENTER);

        if (baritem->enable)
        {
            color = normcolor;
#ifdef TEXU_CIO_COLOR_MONO
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_reverse(dc, color));
#else
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
#endif
            /* draw popup menu */
            if (treeitem == menu->curbaritem)
            {
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
                color = selcolor;
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
            }
        }
        else
        {
#ifdef TEXU_CIO_COLOR_MONO
            color = normcolor;
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_reverse(dc, color));
#else
            color = discolor;
            texu_cio_putstr_attr(dc, y, x, buf,
                                 texu_cio_get_color(dc, color));
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

texu_i64
_TexuMenuWndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
    case TEXU_WM_CHAR:
        _TexuMenuWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_WM_PAINT:
        _TexuMenuWndProc_OnPaint(wnd, (texu_cio *)param1);
        return 0;

    case TEXU_WM_CREATE:
        _TexuMenuWndProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuMenuWndProc_OnDestroy(wnd);
        return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
