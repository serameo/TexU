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
#include "cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif

void        _TexuPushWindow(texu_wnd *wnd);
texu_wnd    *_TexuPopWindow();
texu_wnd    *_TexuTopWindow();

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
#if (defined WIN32 && defined _WINDOWS)
texu_status
TexuStartup(HINSTANCE hinst, texu_i32 lines, texu_i32 cols)
{
    genv = texu_env_new(hinst, lines, cols);
    return (genv ? TEXU_OK : TEXU_ERROR);
}
#else
texu_status
TexuStartup(texu_i32 lines, texu_i32 cols)
{
    genv = texu_env_new(lines, cols);
    return (genv ? TEXU_OK : TEXU_ERROR);
}
#endif
#endif /*USE_TCL_AUTOMATION*/

texu_status
TexuShutdown()
{
    texu_env_del(genv);
    return TEXU_OK;
}

#if (defined WIN32 && defined _WINDOWS)
texu_status
TexuCreateMainEnvWnd(DWORD dwExStyle, DWORD dwStyle, 
    int x, int y, int cx, int cy,
    LPVOID lpData)
{
    texu_status rc = texu_env_create_mainwnd(genv,
                        dwExStyle,
                        dwStyle,
                        x, y, cx, cy,
                        lpData);
    if (TEXU_OK == rc)
    {
        HWND hwnd = texu_env_get_hwnd(genv);
        UpdateWindow(hwnd);
        ShowWindow(hwnd, SW_SHOW);
    }
    return rc;
}

texu_status
TexuCreateChildEnvWnd(DWORD dwExStyle, DWORD dwStyle,
                 int x, int y, int cx, int cy,
                 HWND hWndParent,
                 UINT nID,
                 LPVOID lpData)
{
    texu_status rc = texu_env_create_childwnd(genv,
                                        dwExStyle,
                                        dwStyle,
                                        x, y, cx, cy,
                                        hWndParent,
                                        nID,
                                        lpData);
    return rc;
}

texu_status
TexuRun()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
#else
texu_status
TexuRun()
{
    return (genv ? texu_env_run(genv) : TEXU_ERROR);
}
#endif

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
#if (defined WIN32 && defined _WINDOWS)
        PostQuitMessage(0);
#else
        texu_env_exit(genv);
#endif
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
    texu_wnd_attrs attrs;
    texu_wnd    *wnd    = 0;
    texu_status rc      = TEXU_OK;
    texu_wnd    *desktop    = texu_env_get_desktop(genv);
    texu_wnd    *childwnd   = 0;
    texu_env    *env        = genv;

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
    attrs.y             = 0;
    attrs.x             = 0;
    attrs.height        = 0;
    attrs.width         = 0;
    attrs.enable        = TEXU_TRUE;
    attrs.visible       = TEXU_TRUE;
    attrs.text          = text;
    attrs.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    attrs.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);

    attrs.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    attrs.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    attrs.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);

    attrs.id = id;
    attrs.clsname       = TEXU_MSGBOX_CLASS;
    attrs.userdata      = userdata;
    attrs.style         = buttons;
    attrs.exstyle       = 0;

    rc = texu_wnd_create(wnd, desktop, &attrs);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(wnd);
        return 0;
    }

    texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER,   (texu_i64)owner, 0);
    texu_wnd_send_msg(wnd, TEXU_MBM_SETCAPTION, (texu_i64)caption, 0);

    _TexuPushWindow(wnd);

    childwnd = texu_wnd_get_activechild(wnd);
    if (childwnd)
    {
        texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
    }

    TexuShowWindow(wnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(wnd);

    return wnd;
}

#if USE_TCL_AUTOMATION
texu_status
TexuCreateControls(texu_wnd *wnd, texu_wnd_template *templ, texu_i32 nitems, texu_char *templname)
#else
texu_status
TexuCreateControls(texu_wnd *wnd, texu_wnd_template *templ, texu_i32 nitems)
#endif
{
    texu_status rc = TEXU_OK;
    texu_i32    i  = 0;
#if USE_TCL_AUTOMATION
    texu_env    *env    = genv;
    cJSON   *root       = cJSON_CreateObject();
    cJSON   *wndname    = cJSON_CreateObject();
    FILE    *fstdout    = texu_env_get_stdout(env);
    cJSON   *child      = 0;
    texu_char name[64];
    char    *outjson;
#endif

    for (i = 0; i < nitems; ++i)
    {
        if (!(templ[i].clsname) || texu_strlen(templ[i].clsname) == 0)
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
#if USE_TCL_AUTOMATION
        /*print to file*/
        child = cJSON_CreateObject();
        
        cJSON_AddStringToObject(child, "text", templ[i].text);
        cJSON_AddStringToObject(child, "clsname", templ[i].clsname);

        cJSON_AddNumberToObject(child, "y", templ[i].y);
        cJSON_AddNumberToObject(child, "x", templ[i].x);
        cJSON_AddNumberToObject(child, "h", templ[i].h);
        cJSON_AddNumberToObject(child, "w", templ[i].w);
        cJSON_AddNumberToObject(child, "id", templ[i].id);
        
        sprintf(name, "%s_%d", templ[i].clsname, templ[i].id);
        
        cJSON_AddItemToObject(wndname, name, child);
#endif
    }
#if USE_TCL_AUTOMATION
    cJSON_AddItemToObject(root, templname, wndname);
    outjson = cJSON_Print(root);
    fprintf(fstdout, "%s\n", outjson);
    fflush(fstdout);
    
    free(outjson);
    cJSON_Delete(root);
#endif
    return rc;
}

#if USE_TCL_AUTOMATION
texu_status
TexuCreateControls2(texu_wnd *wnd, texu_wnd_template2 *templ, texu_i32 nitems, texu_char *templname)
#else
texu_status
TexuCreateControls2(texu_wnd *wnd, texu_wnd_template2 *templ, texu_i32 nitems)
#endif
{
    texu_status rc = TEXU_OK;
    texu_i32    i  = 0;
#if USE_TCL_AUTOMATION
    texu_env    *env    = genv;
    cJSON   *root       = cJSON_CreateObject();
    cJSON   *wndname    = cJSON_CreateObject();
    FILE    *fstdout    = texu_env_get_stdout(env);
    cJSON   *child      = 0;
    texu_char name[64];
    char    *outjson;
#endif

    for (i = 0; i < nitems; ++i)
    {
        if (!(templ[i].clsname) || texu_strlen(templ[i].clsname) == 0)
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
#if USE_TCL_AUTOMATION
        /*print to file*/
        child = cJSON_CreateObject();
        
        cJSON_AddStringToObject(child, "text", templ[i].text);
        cJSON_AddStringToObject(child, "clsname", templ[i].clsname);

        cJSON_AddNumberToObject(child, "y", templ[i].y);
        cJSON_AddNumberToObject(child, "x", templ[i].x);
        cJSON_AddNumberToObject(child, "h", templ[i].h);
        cJSON_AddNumberToObject(child, "w", templ[i].w);
        cJSON_AddNumberToObject(child, "id", templ[i].id);
        
        sprintf(name, "%s_%d", templ[i].clsname, templ[i].id);
        
        cJSON_AddItemToObject(wndname, name, child);
#endif
    }
#if USE_TCL_AUTOMATION
    cJSON_AddItemToObject(root, templname, wndname);
    outjson = cJSON_Print(root);
    fprintf(fstdout, "%s\n", outjson);
    fflush(fstdout);
    
    free(outjson);
    cJSON_Delete(root);
#endif
    return rc;
}


texu_wnd *
TexuCreateWindow(
    const texu_char     *text,
    const texu_char     *clsname,
    texu_ui32           style,
    texu_ui32           exstyle,
    texu_i32            y,
    texu_i32            x,
    texu_i32            h,
    texu_i32            w,
    texu_wnd            *parent,
    texu_i32            id,
    void                *userdata)
{
    texu_wnd *wnd = TexuCreateWindow2(
                        text,
                        clsname,
                        style,
                        exstyle,
                        y,
                        x,
                        h,
                        w,
                        parent,
                        id,
                        userdata,
                        0);

    return wnd;
}

texu_wnd *
TexuCreateWindow2(
    const texu_char     *text,
    const texu_char     *clsname,
    texu_ui32           style,
    texu_ui32           exstyle,
    texu_i32            y,
    texu_i32            x,
    texu_i32            h,
    texu_i32            w,
    texu_wnd            *parent,
    texu_i32            id,
    void                *userdata,
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
    attrs.y             = y;
    attrs.x             = x;
    attrs.height        = h;
    attrs.width         = w;
    attrs.enable        = (TEXU_WS_DISABLED & style ? TEXU_FALSE : TEXU_TRUE);
    attrs.visible       = (TEXU_WS_HIDE     & style ? TEXU_FALSE : TEXU_TRUE);
    attrs.text          = text;
    attrs.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);
    attrs.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_WINDOW);

    attrs.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
    attrs.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);
    attrs.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_WINDOW);

    attrs.id = id;
    attrs.clsname       = clsname;
    attrs.userdata      = userdata;
    attrs.style         = style;
    attrs.exstyle       = exstyle;
    attrs.on_validate   = on_validate;

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
        _TexuPushWindow(wnd);

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
    texu_env    *env    = genv;
    texu_wnd    *desktop = texu_env_get_desktop(env);
    texu_wnd    *parent = texu_wnd_get_parent(wnd);
    texu_wnd    *topwnd = 0;
    
    if (parent == desktop)
    {
        _TexuPopWindow();
        topwnd = _TexuTopWindow();
        if (topwnd)
        {
            TexuShowWindow(topwnd, TEXU_SW_SHOW);
            TexuInvalidateWindow(topwnd);
        }
        else
        {
            /*no more frame window*/
            TexuExit();
        }
    }
    texu_wnd_destroy(wnd);
    texu_wnd_del(wnd);
}

texu_i64
TexuCloseWindow(texu_wnd *wnd)
{
    texu_i64 rc = texu_wnd_close(wnd);
    if (TEXU_OK == rc)
    {
        TexuDestroyWindow(wnd);
    }
    return rc;
}

texu_bool
TexuIsExecutableCommand(
    texu_wnd*   wnd,
    texu_ui32   id)
{
    return texu_wnd_is_executable(wnd, id);
}

void
_TexuPushWindow(texu_wnd *wnd)
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
_TexuTopWindow()
{
    return (genv ? texu_env_top_wnd(genv) : 0);
}

texu_wnd *
_TexuPopWindow()
{
    return (genv ? texu_env_pop_wnd(genv) : 0);
}

texu_i32
TexuGetMaxY()
{
    return (genv ? texu_env_screen_height(genv) : LINES);
}

texu_i32
TexuGetMaxX()
{
    return (genv ? texu_env_screen_width(genv) : COLS);
}

void
TexuShowWindow(
    texu_wnd *wnd,
    texu_i32 show)
{
    texu_wnd_visible(wnd, show);
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

texu_i64
TexuPostMessage(
    texu_wnd *wnd,
    texu_ui32 msg,
    texu_i64 param1,
    texu_i64 param2)
{
    return texu_wnd_post_msg(wnd, msg, param1, param2);
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
    texu_wnd_set_text(wnd, (text ? text : TEXUTEXT("")));
}
texu_i32
TexuGetWindowText(
  texu_wnd*         wnd,
  texu_char*        text,
  texu_i32          len
)
{
    TexuSendMessage(wnd, TEXU_WM_GETTEXT, (texu_i64)text, len);
    return texu_strlen(text);
}

void
TexuSetColor(
    texu_wnd *wnd,
    texu_ui32 color,
    texu_ui32 discolor)
{
    texu_wnd_set_color(wnd, color, discolor);
}

void
TexuSetBgColor(
texu_wnd *wnd,
texu_ui32 color,
texu_ui32 discolor)
{
    texu_wnd_set_bgcolor(wnd, color, discolor);
}

texu_ui32
TexuGetSysColor(texu_i32 color)
{
    return texu_env_get_syscolor(genv, color);
}

texu_ui32
TexuGetSysBgColor(texu_i32 color)
{
    return texu_env_get_sysbgcolor(genv, color);
}



texu_i64
TexuSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_i64 rc = TexuSendMessage(prevwnd, TEXU_WM_KILLFOCUS, 0, 0);
    if (rc != TEXU_OK)            
    {
        return rc;
    }
    return TexuSendMessage(wnd, TEXU_WM_SETFOCUS, (texu_i64)prevwnd, 0);
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
            TexuAddPopupMenuItem(
                    menu, popup,
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
#if (defined WIN32 && defined _WINDOWS)
    return clridx;
#else
    return texu_cio_get_color(cio, clridx);
#endif
}

#if (defined WIN32 && defined _WINDOWS)
texu_i32
TexuDrawHRects(texu_cio* cio, texu_rect* rect, texu_i32* heights, texu_i32 nheight, texu_ui32 color, texu_ui32 bgcolor)
{
    return texu_env_draw_hframes(
                texu_cio_get_env(cio),
                rect,
                heights,
                nheight,
                color,
                bgcolor);
}

texu_i32
TexuDrawVRects(texu_cio* cio, texu_rect* rect, texu_i32* widths, texu_i32 nwidth, texu_ui32 color, texu_ui32 bgcolor)
{
    return texu_env_draw_vframes(
                texu_cio_get_env(cio),
                rect,
                widths,
                nwidth,
                color,
                bgcolor);
}

texu_i32
TexuDrawFrame(texu_cio* cio, const texu_char* text, texu_rect* rect, texu_ui32 color, texu_ui32 bgcolor)
{
    return texu_env_draw_rect(
                texu_cio_get_env(cio),
                rect,
                color,
                bgcolor);
}

texu_i32
TexuDrawRect(texu_cio* cio, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor)
{
    return texu_env_draw_rect(texu_cio_get_env(cio), rect, textcolor, bgcolor);
}

#else
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
#endif

#ifdef __cplusplus
}
#endif
