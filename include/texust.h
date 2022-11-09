/*
#
# File Name: texust.h
# Author: Seree Rakwong
# Date: 26-MAY-2020
#
*/

#ifndef _TEXUSTRUCT_H_
#define _TEXUSTRUCT_H_

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct texu_env;
    typedef struct texu_env texu_env;

    struct texu_wnd;
    typedef struct texu_wnd texu_wnd;

    struct texu_wnd_attrs
    {
        texu_i32 y;
        texu_i32 x;
        texu_i32 height;
        texu_i32 width;
        texu_ui32 style;
        texu_ui32 exstyle;
        texu_bool enable;
        texu_bool visible;
        const texu_char *text;
        texu_i32 normalcolor;
        texu_i32 disabledcolor;
        texu_i32 focuscolor;
        texu_ui32 id;
        const texu_char *clsname;
        void *userdata;
    };
    typedef struct texu_wnd_attrs texu_wnd_attrs;

    struct texu_rect
    {
        texu_i32 y;
        texu_i32 x;
        texu_i32 lines;
        texu_i32 cols;
    };
    typedef struct texu_rect texu_rect;

    struct texu_wnd_notify
    {
        texu_wnd *wnd;
        texu_ui32 id;
        texu_ui32 code;
    };
    typedef struct texu_wnd_notify texu_wnd_notify;

    struct texu_wnd_keycmd
    {
        texu_i32 key;
        texu_i32 alt;
        texu_ui32 cmd;
    };
    typedef struct texu_wnd_keycmd texu_wnd_keycmd;

    struct texu_lbwnd_notify
    {
        texu_wnd_notify hdr;
        texu_i32 index;
    };
    typedef struct texu_lbwnd_notify texu_lbwnd_notify;

    struct texu_cbwnd_notify
    {
        texu_wnd_notify hdr;
        texu_i32 index;
    };
    typedef struct texu_cbwnd_notify texu_cbwnd_notify;

    struct texu_wnd_subitem
    {
        texu_i32 idx; /* row index, zero based    */
        texu_i32 col; /* column index, zero based */
        texu_char *text;
        texu_i32 normcolor; /* text attributes          */
        texu_i32 discolor;  /* text attributes          */
        texu_i32 selcolor;  /* text attributes          */
        void *data;         /* user data                */
    };
    typedef struct texu_wnd_subitem texu_wnd_subitem;

    struct texu_header_notify
    {
        texu_wnd_notify hdr; /* notification header */
        texu_i32 row;
        texu_i32 col;
    };
    typedef struct texu_header_notify texu_header_notify;

    struct texu_wnd_header
    {
        texu_char *caption;
        texu_i32 cols;
        texu_i32 align;      /* column alignment         */
        texu_i32 normcolor;  /* text attributes          */
        texu_i32 discolor;   /* text attributes          */
        texu_i32 selcolor;   /* text attributes          */
        texu_ui32 editstyle; /* edit style, see TES_XXX  */
        texu_i32 decwidth;   /* TES_DECIMAL or TES_AUTODECIMALCOMMA, default 6 */
    };
    typedef struct texu_wnd_header texu_wnd_header;

    /* edit */
    struct texu_editminmax
    {
        texu_i32 min;
        texu_i32 max;
    };
    typedef struct texu_editminmax texu_editminmax;

    /* tree */

    struct texu_treewnd_item
    {
        texu_char itemtext[TEXU_MAX_WNDTEXT + 1];
        texu_i64 lparam;
        texu_bool expanded;
        texu_bool selected;
        texu_i32 children;
        texu_bool populated;
        texu_i32 normcolor;
        texu_i32 discolor;
        texu_i32 selcolor;
    };
    typedef struct texu_treewnd_item texu_treewnd_item;

    struct texu_treewnd_notify
    {
        texu_wnd_notify hdr; /* notification header */
        texu_tree_item *item;
    };
    typedef struct texu_treewnd_notify texu_treewnd_notify;

    struct texu_msgbox_notify
    {
        texu_wnd_notify hdr; /* notification header */
        texu_ui32 id;
    };
    typedef struct texu_msgbox_notify texu_msgbox_notify;

    /*MENU*/

    struct texu_menu_item
    {
        texu_ui32 id;
        texu_char text[TEXU_MAX_WNDTEXT + 1];
        texu_bool enable;
        texu_i32 normcolor;
        texu_i32 discolor;
        texu_i32 selcolor;
        texu_ui32 style; /*TEXU_MS_TEXT, TEXU_MS_BREAK*/
        texu_char info[TEXU_MAX_WNDTEXT + 1];
        void *userdata;
    };
    typedef struct texu_menu_item texu_menu_item;

    struct texu_menu;
    typedef struct texu_menu texu_menu;

    struct texu_menuitem_notify
    {
        texu_wnd_notify hdr; /* notification header */
        texu_ui32 id;
        texu_char info[TEXU_MAX_WNDTEXT + 1];
    };
    typedef struct texu_menuitem_notify texu_menuitem_notify;

    struct texu_wnd_template
    {
        const texu_char *text;
        const texu_char *clsname;
        texu_ui32 style;
        texu_ui32 exstyle;
        texu_i32 y;
        texu_i32 x;
        texu_i32 h;
        texu_i32 w;
        texu_i32 id;
    };
    typedef struct texu_wnd_template texu_wnd_template;

    struct texu_menuitem_template
    {
        const texu_char *text;
        texu_ui32 id;
        texu_i32 enabled;
        const texu_char *info;
    };
    typedef struct texu_menuitem_template texu_menuitem_template;

#define TEXU_MAX_MENUITEMS 32
    struct texu_menu_template
    {
        const texu_char *text;
        texu_i32 enabled;
        const texu_char *info;
        texu_i32 nitems;
        texu_menuitem_template items[TEXU_MAX_MENUITEMS];
    };
    typedef struct texu_menu_template texu_menu_template;

    /*REBAR*/
    struct texu_rbwnd_band
    {
        texu_char   caption[TEXU_MAX_WNDTEXT + 1];
        texu_i32    align;
        texu_i32    normcolor;  /* text attributes          */
        texu_i32    discolor;   /* text attributes          */
        texu_i32    selcolor;   /* text attributes          */
        texu_wnd*   childwnd;   /* child window for each cell*/
                                /* it could be editbox/listbox/combobox etc*/
        texu_i32    width;
        texu_i32    height;
    };
    typedef struct texu_rbwnd_band texu_rbwnd_band;

#ifdef __cplusplus
}
#endif

#endif /*_TEXUSTRUCT_H_*/
