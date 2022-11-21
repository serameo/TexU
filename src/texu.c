/*
#
# File Name: texu.c
# Author: Seree Rakwong
# Date: 15-MAY-2020
#
*/

#include <stdlib.h>
#include <string.h>
#include "texu.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void TexuPushWindow(
        texu_wnd *wnd);
    texu_wnd *TexuPopWindow();
    texu_wnd *TexuTopWindow();

    /* global variables */
    texu_env *genv = 0;

#ifdef USE_TCL_AUTOMATION
    texu_status
    TexuStartup(texu_i32 lines, texu_i32 cols, const char* pathname)
    {
        genv = texu_env_new(lines, cols, pathname);
        return (genv ? TEXU_OK : TEXU_ERROR);
    }
#else
    texu_status
    TexuStartup(texu_i32 lines, texu_i32 cols)
    {
        genv = texu_env_new(lines, cols);
        return (genv ? TEXU_OK : TEXU_ERROR);
    }
#endif /*USE_TCL_AUTOMATION*/

    texu_status
    TexuShutdown()
    {
        texu_env_del(genv);
        return TEXU_OK;
    }

    texu_status
    TexuRun()
    {
        return (genv ? texu_env_run(genv) : TEXU_ERROR);
    }

    texu_env *
    TexuGetEnv()
    {
        return genv;
    }

    texu_cio *
    TexuGetDC()
    {
        return (genv ? texu_env_get_cio(genv) : 0);
    }

    void
    TexuExit()
    {
        if (genv)
        {
            texu_env_exit(genv);
        }
    }

    void
    TexuSetThemeColor(texu_i32 (*setcolor)(texu_i32))
    {
        if (genv && setcolor)
        {
            texu_env_set_syscolors(genv, setcolor);
        }
    }

    texu_status
    TexuRegisterClass(
        texu_char *clsname,
        texu_wndproc wndproc)
    {
        return (genv ? texu_env_register_cls(genv, clsname, wndproc) : TEXU_ERROR);
    }

    texu_wnd *
    TexuMessageBox(
        texu_char *caption,
        texu_char *text,
        texu_wnd *owner,
        texu_ui32 id,
        texu_ui32 buttons,
        void *userdata)
    {
        texu_wnd *wnd = 0;
        texu_wnd_attrs attrs;
        texu_status rc = TEXU_OK;
        texu_wnd *desktop = texu_env_get_desktop(genv);
        texu_wnd *childwnd = 0;
        texu_env *env = genv;

        if (!genv)
        {
            return 0;
        }
        wnd = texu_wnd_new(genv);
        if (!wnd)
        {
            return 0;
        }
        memset(&attrs, 0, sizeof(attrs));
        attrs.y = 0;
        attrs.x = 0;
        attrs.height = 0;
        attrs.width = 0;
        attrs.enable = TEXU_TRUE;
        attrs.visible = TEXU_TRUE;
        attrs.text = text;
        attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        attrs.id = id;
        attrs.clsname = TEXU_MSGBOX_CLASS;
        attrs.userdata = userdata;
        attrs.style = buttons;
        attrs.exstyle = 0;

        rc = texu_wnd_create(wnd, desktop, &attrs);

        if (rc != TEXU_OK)
        {
            texu_wnd_del(wnd);
            return 0;
        }

        texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER, (texu_i64)owner, 0);
        texu_wnd_send_msg(wnd, TEXU_MBM_SETCAPTION, (texu_i64)caption, 0);

        TexuPushWindow(wnd);

        childwnd = texu_wnd_get_activechild(wnd);
        if (childwnd)
        {
            texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
        }

        TexuShowWindow(wnd, TEXU_WS_SHOW);
        TexuInvalidateWindow(wnd);

        return wnd;
    }

    texu_status
    TexuCreateControls(texu_wnd *wnd, texu_wnd_template *templ, texu_i32 nitems)
    {
        texu_status rc = TEXU_OK;
        texu_i32 i = 0;

        for (i = 0; i < nitems; ++i)
        {
            if (!(templ[i].clsname) || strlen(templ[i].clsname) == 0)
            {
                break;
            }
            TexuCreateWindow(
                templ[i].text,
                templ[i].clsname,
                templ[i].style,
                templ[i].exstyle,
                templ[i].y,
                templ[i].x,
                templ[i].h,
                templ[i].w,
                wnd,
                templ[i].id,
                0);
        }
        return rc;
    }

    texu_status
    TexuCreateControls2(texu_wnd *wnd, texu_wnd_template2 *templ, texu_i32 nitems)
    {
        texu_status rc = TEXU_OK;
        texu_i32 i = 0;

        for (i = 0; i < nitems; ++i)
        {
            if (!(templ[i].clsname) || strlen(templ[i].clsname) == 0)
            {
                break;
            }
            TexuCreateWindow2(
                templ[i].text,
                templ[i].clsname,
                templ[i].style,
                templ[i].exstyle,
                templ[i].y,
                templ[i].x,
                templ[i].h,
                templ[i].w,
                wnd,
                templ[i].id,
                0,
                templ[i].on_validate);
        }
        return rc;
    }


    texu_wnd *
    TexuCreateWindow(
        const texu_char *text,
        const texu_char *clsname,
        texu_ui32 style,
        texu_ui32 exstyle,
        texu_i32 y,
        texu_i32 x,
        texu_i32 h,
        texu_i32 w,
        texu_wnd *parent,
        texu_i32 id,
        void *userdata)
    {
        texu_wnd *wnd = 0;
        texu_wnd_attrs attrs;
        texu_status rc = TEXU_OK;
        texu_env *env = genv;
        texu_wnd *desktop = texu_env_get_desktop(env);
        texu_wnd *childwnd = 0;

        if (!genv)
        {
            return 0;
        }
        wnd = texu_wnd_new(genv);
        if (!wnd)
        {
            return 0;
        }
        memset(&attrs, 0, sizeof(attrs));
        attrs.y = y;
        attrs.x = x;
        attrs.height = h;
        attrs.width = w;
        attrs.enable = TEXU_TRUE;
        attrs.visible = TEXU_TRUE;
        attrs.text = text;
        attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
        attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
        attrs.id = id;
        attrs.clsname = clsname;
        attrs.userdata = userdata;
        attrs.style = style;
        attrs.exstyle = exstyle;

        if (!(parent))
        {
            parent = desktop;
        }
        rc = texu_wnd_create(wnd, parent, &attrs);

        if (rc != TEXU_OK)
        {
            texu_wnd_del(wnd);
            return 0;
        }

        if (parent == desktop)
        {
            TexuPushWindow(wnd);

            childwnd = texu_wnd_get_activechild(wnd);
            if (childwnd)
            {
                texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
            }
        }

        return wnd;
    }

    texu_wnd *
    TexuCreateWindow2(
        const texu_char *text,
        const texu_char *clsname,
        texu_ui32 style,
        texu_ui32 exstyle,
        texu_i32 y,
        texu_i32 x,
        texu_i32 h,
        texu_i32 w,
        texu_wnd *parent,
        texu_i32 id,
        void *userdata,
        texu_i32 (*on_validate)(texu_wnd*, texu_char*))
    {
        texu_wnd *wnd = 0;
        texu_wnd_attrs attrs;
        texu_status rc = TEXU_OK;
        texu_env *env = genv;
        texu_wnd *desktop = texu_env_get_desktop(env);
        texu_wnd *childwnd = 0;

        if (!genv)
        {
            return 0;
        }
        wnd = texu_wnd_new(genv);
        if (!wnd)
        {
            return 0;
        }
        memset(&attrs, 0, sizeof(attrs));
        attrs.y = y;
        attrs.x = x;
        attrs.height = h;
        attrs.width = w;
        attrs.enable = TEXU_TRUE;
        attrs.visible = TEXU_TRUE;
        attrs.text = text;
        attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
        attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
        attrs.id = id;
        attrs.clsname = clsname;
        attrs.userdata = userdata;
        attrs.style = style;
        attrs.exstyle = exstyle;
        attrs.on_validate = on_validate;

        if (!(parent))
        {
            parent = desktop;
        }
        rc = texu_wnd_create(wnd, parent, &attrs);

        if (rc != TEXU_OK)
        {
            texu_wnd_del(wnd);
            return 0;
        }

        if (parent == desktop)
        {
            TexuPushWindow(wnd);

            childwnd = texu_wnd_get_activechild(wnd);
            if (childwnd)
            {
                texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
            }
        }

        return wnd;
    }

    void
    TexuDestroyWindow(
        texu_wnd *wnd)
    {
        texu_wnd *topwnd = 0;
        TexuPopWindow();

        texu_wnd_destroy(wnd);
        texu_wnd_del(wnd);

        topwnd = TexuTopWindow();
        TexuShowWindow(topwnd, TEXU_WS_SHOW);
        TexuInvalidateWindow(topwnd);
    }

    void
    TexuPushWindow(
        texu_wnd *wnd)
    {
        /* can push a window if its parent is null */
        /* because it is a frame window */
        if (!(genv) || !(wnd))
        {
            return;
        }

        texu_env_push_wnd(genv, wnd);
    }

    texu_wnd *
    TexuTopWindow()
    {
        return (genv ? texu_env_top_wnd(genv) : 0);
    }

    texu_wnd *
    TexuPopWindow()
    {
        return (genv ? texu_env_pop_wnd(genv) : 0);
    }

    texu_i32
    TexuGetMaxY()
    {
        return LINES;
    }

    texu_i32
    TexuGetMaxX()
    {
        return COLS;
    }

    void
    TexuShowWindow(
        texu_wnd *wnd,
        texu_i32 show)
    {
        texu_wnd_visible(wnd, (TEXU_WS_SHOW == show));
    }

    void
    TexuInvalidateWindow(
        texu_wnd *wnd)
    {
        texu_wnd_invalidate(wnd);
    }

    texu_i64
    TexuSendMessage(
        texu_wnd *wnd,
        texu_ui32 msg,
        texu_i64 param1,
        texu_i64 param2)
    {
        return texu_wnd_send_msg(wnd, msg, param1, param2);
    }

    texu_wnd *
    TexuGetWindowItem(
        texu_wnd *wnd,
        texu_ui32 id)
    {
        return texu_wnd_find_child(wnd, id);
    }
    
    texu_wnd *
    TexuGetParent(
        texu_wnd *wnd)
    {
        return texu_wnd_get_parent(wnd);
    }

    void
    TexuSetWindowText(
        texu_wnd *wnd,
        const texu_char *text)
    {
        texu_wnd_set_text(wnd, (text ? text : ""));
    }
    texu_i32
    TexuGetWindowText(
      texu_wnd*         wnd,
      texu_char*        text,
      texu_i32          len
    )
    {
        TexuSendMessage(wnd, TEXU_WM_GETTEXT, (texu_i64)text, len);
        return strlen(text);
    }
    void
    TexuSetColor(
        texu_wnd *wnd,
        texu_i32 color,
        texu_i32 discolor)
    {
        texu_wnd_set_color(wnd, color, discolor);
    }

    void
    TexuAddHotKey(
        texu_wnd *wnd,
        texu_i32 key,
        texu_ui32 cmd,
        texu_i32 alt)
    {
        texu_wnd_add_keycmd(wnd, key, cmd, alt);
    }

    void
    TexuEnableWindow(texu_wnd *wnd, texu_bool enable)
    {
        texu_wnd_enable(wnd, enable);
    }

    void
    TexuSaveCursorPosition(texu_wnd *wnd)
    {
        texu_wnd_save_curpos(wnd);
    }

    void
    TexuRestoreCursorPosition(texu_wnd *wnd)
    {
        texu_wnd_restore_curpos(wnd);
    }

    /*menus*/
    texu_menu *
    TexuCreateMenu(texu_wnd *owner, texu_ui32 id)
    {
        return texu_menu_new(owner, id);
    }

    texu_popup_menu *
    TexuAddPopupMenu(
        texu_menu *menu,
        const texu_char *text,
        texu_bool enable,
        const texu_char *info)
    {
        return texu_menu_add_menu_info(
            menu,
            text,
            enable,
            info);
    }
    texu_popup_menu_item *
    TexuAddPopupMenuItem(
        texu_menu *menu,
        texu_tree_item *popup,
        const texu_char *text,
        texu_ui32 id,
        texu_bool enable,
        const texu_char *info)
    {
        return texu_menu_add_item_info(
            menu,
            popup,
            text,
            id,
            enable,
            info);
    }

    texu_popup_menu *
    TexuGetPopupMenu(texu_menu *menu, texu_ui32 idx)
    {
        return texu_menu_get_menu(menu, idx);
    }

    texu_popup_menu_item *
    TexuGetPopupMenuItem(
        texu_menu *menu,
        texu_popup_menu *popup,
        texu_ui32 id)
    {
        return texu_menu_get_menuitem(menu, popup, id);
    }

    texu_bool
    TexuEnablePopupMenu(texu_menu *menu, texu_ui32 idx, texu_bool enable)
    {
        return texu_menu_enable_menu(menu, idx, enable);
    }

    texu_bool
    TexuEnablePopupMenuItem(texu_menu *menu, texu_popup_menu *popup, texu_ui32 id, texu_bool enable)
    {
        return texu_menu_enable_menuitem(menu, popup, id, enable);
    }

    texu_menu *
    TexuCreateMenus(texu_wnd *wnd, texu_ui32 id, const texu_menu_template *templ, texu_i32 npopups)
    {
        texu_menu *menu = 0;
        texu_popup_menu *popup = 0;
        /*texu_popup_menu_item* item = 0;*/
        texu_i32 i = 0;
        texu_i32 j = 0;
        texu_i32 nitems = 0;

        menu = TexuCreateMenu(wnd, id);
        for (i = 0; i < npopups; ++i)
        {
            popup = TexuAddPopupMenu(
                menu,
                templ[i].text,
                templ[i].enabled,
                templ[i].info);

            nitems = templ[i].nitems;
            for (j = 0; j < nitems; ++j)
            {
                /*item =*/TexuAddPopupMenuItem(menu, popup,
                                               templ[i].items[j].text,
                                               templ[i].items[j].id,
                                               templ[i].items[j].enabled,
                                               templ[i].items[j].info);
            }
        }
        return menu;
    }

    /*device context*/
    texu_i32
    TexuGetColor(texu_cio *cio, texu_i32 clridx)
    {
        return texu_cio_get_color(cio, clridx);
    }

    texu_i32
    TexuDrawHRects(texu_cio *cio, texu_rect *rect, texu_i32 *widths, texu_i32 nwidth, texu_i32 attrs)
    {
        return texu_cio_draw_hrects(cio, rect, widths, nwidth, attrs);
    }

    texu_i32
    TexuDrawVRects(texu_cio *cio, texu_rect *rect, texu_i32 *heights, texu_i32 nheight, texu_i32 attrs)
    {
        return texu_cio_draw_vrects(cio, rect, heights, nheight, attrs);
    }

    texu_i32
    TexuDrawFrame(texu_cio *cio, const texu_char *text, texu_rect *rect, texu_i32 attrs)
    {
        return texu_cio_draw_frame(cio, text, rect, attrs);
    }

#ifdef __cplusplus
}
#endif
