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
#include "texukeys.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
#-------------------------------------------------------------------------------
# TexU message box
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_msgbox
{
    texu_wnd *owner;
    texu_ui32 id;
    texu_ui32 titlecolor;
    texu_ui32 labelcolor;
    texu_ui32 okcolor;
    texu_ui32 cancelcolor;
    texu_ui32 yescolor;
    texu_ui32 nocolor;

    texu_ui32 titlebg;
    texu_ui32 labelbg;
    texu_ui32 okbg;
    texu_ui32 cancelbg;
    texu_ui32 yesbg;
    texu_ui32 nobg;

    void *userdata;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_char caption[TEXU_MAX_WNDTEXT + 1];
};
typedef struct texu_msgbox texu_msgbox;

void _TexuMsgBoxProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuMsgBoxProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuMsgBoxProc_OnDestroy(texu_wnd *);
void _TexuMsgBoxProc_OnSetCaption(texu_wnd *, const texu_char *);
void _TexuMsgBoxProc_OnSetOwner(texu_wnd *, texu_wnd *);

texu_status _TexuMsgBoxProc_CreateChildren(texu_wnd *wnd, texu_wnd_attrs *attrs, texu_i32 lines);
texu_status _TexuMsgBoxProc_CreateButtons(
    texu_wnd *wnd,
    texu_i32 y,
    texu_i32 x,
    texu_char *caption,
    texu_ui32 id,
    texu_i32 cmd,
    texu_ui32 color,
    texu_ui32 bgcolor);

void _TexuMsgBoxProc_Notify(texu_wnd *wnd, texu_ui32, texu_ui32);

void
_TexuMsgBoxProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_ui32 id)
{
    texu_msgbox_notify notify;
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    texu_wnd *parent = msgbox->owner;

    if (!parent)
    {
        parent = texu_wnd_get_parent(wnd);
    }

    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.id = id;
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}
void _TexuWndProc_Notify(texu_wnd *, texu_ui32);

void
_TexuWndProc_Notify(texu_wnd *wnd, texu_ui32 code)
{
    texu_wnd_notify notify;
    texu_wnd *parent = texu_wnd_get_parent(wnd);

    notify.wnd = wnd;
    notify.id = texu_wnd_get_id(wnd);
    notify.code = code;
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}


void
_TexuMsgBoxProc_OnDestroy(texu_wnd *wnd)
{
    texu_wnd *topwnd = 0;
    texu_env *env = 0;
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    free(msgbox);

    env = texu_wnd_get_env(wnd);
    texu_env_pop_wnd(env);

    topwnd = texu_env_top_wnd(env);
    texu_wnd_send_msg(topwnd, TEXU_WM_SHOW, TEXU_TRUE, 0);
    texu_wnd_invalidate(topwnd);

    /*remove msgbox from topwnd*/
    texu_wnd_remove_child(topwnd, wnd);
    /*texu_wnd_del(wnd);*/
}

void
_TexuMsgBoxProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    /*draw caption*/
    texu_printf_alignment3(buf, msgbox->caption, width, TEXU_ALIGN_CENTER, TEXU_TRUE, x, cx);
    texu_cio_draw_text(dc, y, x, buf, msgbox->titlecolor, msgbox->titlebg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

texu_i32
_TexuDefWndProc_CountLines(const texu_char *text, texu_i32 *maxlen)
{
    texu_i32 lines = 0;
    texu_char *tok;
    texu_i32 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
#if (defined WIN32 && defined UNICODE)
    texu_char *pszContext;
#endif
    *maxlen = 0;
    if (text && texu_strlen(text) > 0)
    {
        ++lines;
    }
    else
    {
        return lines;
    }
    texu_strcpy(buf, text);

#if (defined WIN32 && defined UNICODE)
    tok = wcstok_s(buf, TEXUTEXT("\n"), &pszContext);
#else
    tok = strtok(buf, "\n");
#endif
    while (tok)
    {
        ++lines;
        len = texu_strlen(tok);
        if (len > *maxlen)
        {
            *maxlen = len;
        }
#if (defined WIN32 && defined UNICODE)
        tok = wcstok_s(NULL, TEXUTEXT("\n"), &pszContext);
#else
        tok = strtok(0, "\n");
#endif
    }
    return lines;
}

texu_status
_TexuMsgBoxProc_CreateButtons(
    texu_wnd *wnd,
    texu_i32 y,
    texu_i32 x,
    texu_char *caption,
    texu_ui32 id,
    texu_i32 cmd,
    texu_ui32 color,
    texu_ui32 bgcolor)
{
    texu_wnd *child = 0;
    texu_wnd_attrs childattrs;
    texu_status rc = TEXU_OK;

    child = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!child)
    {
        return TEXU_NOMEM;
    }

    memset(&childattrs, 0, sizeof(texu_wnd_attrs));
    childattrs.y = y;
    childattrs.x = x;
    childattrs.height = 1;
    childattrs.width = texu_strlen(caption);
    childattrs.enable = TEXU_TRUE;
    childattrs.visible = TEXU_TRUE;
    childattrs.text = caption;
    childattrs.normalcolor = color;
    childattrs.disabledcolor = color;
    childattrs.focusedcolor = color;

    childattrs.normalbg = bgcolor;
    childattrs.disabledbg = bgcolor;
    childattrs.focusedbg = bgcolor;

    childattrs.id = id;
    childattrs.clsname = TEXU_LABEL_CLASS;
    childattrs.userdata = 0;
    childattrs.style = TEXU_WS_CENTER;
    childattrs.exstyle = 0;

    rc = texu_wnd_create(child, wnd, &childattrs);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(child);
        return TEXU_ERROR;
    }

    texu_wnd_set_color(child, color, color);
    texu_wnd_set_bgcolor(child, bgcolor, bgcolor);

    texu_wnd_add_keycmd(wnd, cmd, id, 0);
    return rc;
}

enum
{
    F1_OK,
    F2_YES,
    F3_NO,
    F4_CANCEL
};
#if (defined WIN32 && defined UNICODE)
static texu_char *buttons[] = { TEXUTEXT("   OK   "), TEXUTEXT("   Yes  "), TEXUTEXT("   No   "), TEXUTEXT(" Cancel ") };
static texu_i32 btnwidths[] = {8, 8, 8, 8};
#else
static texu_char *buttons[] = { TEXUTEXT(" F1 - OK "), TEXUTEXT(" F2 - Yes "), TEXUTEXT(" F3 - No "), TEXUTEXT(" F4 - Cancel ") };
static texu_i32 btnwidths[] = { 9, 10, 9, 13 };
#endif

texu_status
_TexuMsgBoxProc_CreateChildren(texu_wnd *wnd, texu_wnd_attrs *attrs, texu_i32 lines)
{
    texu_wnd *child = 0;
    texu_char *tok;
    texu_wnd_attrs childattrs;
    texu_i32 id = 1000;
    texu_i32 y = 2;
    texu_i32 x = 0;
    texu_bool idok = 0;
    texu_bool idcancel = 0;
    texu_bool idyes = 0;
    texu_bool idno = 0;
    texu_char *caption = 0;
    texu_status rc = TEXU_OK;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 width = 0;
    texu_i32 wndwidth = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
#if (defined WIN32 && defined UNICODE)
    texu_char *pszContext;
#endif
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);

    if (lines < 1)
    {
        return TEXU_ERROR;
    }

    /* create labels*/
    texu_strcpy(buf, attrs->text);
#if (defined WIN32 && defined UNICODE)
    tok = wcstok_s(buf, TEXUTEXT("\n"), &pszContext);
#else
    tok = strtok(buf, "\n");
#endif
    while (tok)
    {
        child = texu_wnd_new(texu_wnd_get_env(wnd));
        if (!child)
        {
            return TEXU_NOMEM;
        }

        memset(&childattrs, 0, sizeof(texu_wnd_attrs));
        childattrs.y = y;
        childattrs.x = 0;
        childattrs.height = 1;
        childattrs.width = attrs->width;
        childattrs.enable = TEXU_FALSE;
        childattrs.visible = TEXU_TRUE;
        childattrs.text = tok;
        childattrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        childattrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        childattrs.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);

        childattrs.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
        childattrs.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
        childattrs.focusedbg  = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);

        childattrs.id = id;
        childattrs.clsname = TEXU_LABEL_CLASS;
        childattrs.userdata = 0;
        childattrs.style = TEXU_WS_CENTER;
        childattrs.exstyle = 0;

        rc = texu_wnd_create(child, wnd, &childattrs);

        if (rc != TEXU_OK)
        {
            texu_wnd_del(child);
            return TEXU_ERROR;
        }
        texu_wnd_set_color(child,
                           texu_env_get_syscolor(env, TEXU_COLOR_DIALOG),
                           texu_env_get_syscolor(env, TEXU_COLOR_DIALOG));

#if (defined WIN32)
        texu_wnd_set_bgcolor(child,
                           texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG),
                           texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG));
        //tok = wcstok_s(NULL, TEXUTEXT("\n"), &pszContext);
#endif
        tok = strtok(0, "\n");

        ++y;
        ++id;
    }
    /* create buttons */
    if (attrs->style & TEXU_MBS_OK)
    {
        ++idok;
        width += btnwidths[F1_OK];
    }
    if (attrs->style & TEXU_MBS_YES)
    {
        ++idyes;
        width += btnwidths[F2_YES] + 1;
    }
    if (attrs->style & TEXU_MBS_NO)
    {
        ++idno;
        width += btnwidths[F3_NO] + 1;
    }
    if (attrs->style & TEXU_MBS_CANCEL)
    {
        ++idcancel;
        width += btnwidths[F4_CANCEL] + 1;
    }

    y += 2;// attrs->y + 2;
    x = /*attrs->x +*/ (wndwidth - width) / 2;
    if (idok)
    {
        caption = buttons[F1_OK];
        rc = _TexuMsgBoxProc_CreateButtons(
            wnd,
            y,
            x,
            caption,
            TEXU_IDOK,
            TVK_F1,
            msgbox->okcolor,
#if (defined WIN32)
            msgbox->okbg);
#else
            msgbox->okcolor);
#endif
        if (rc != TEXU_OK)
        {
            return rc;
        }
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, TEXU_IDOK, 0);
        x += texu_strlen(caption) + 1;
    }
    if (idyes)
    {
        caption = buttons[F2_YES];
        /* re-color if available*/
        if (!(idok))
        {
            msgbox->yescolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_OK);

            msgbox->yesbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_OK);

        }
        rc = _TexuMsgBoxProc_CreateButtons(
            wnd,
            y,
            x,
            caption,
            TEXU_IDYES,
            TVK_F2,
            msgbox->yescolor,
#if (defined WIN32)
            msgbox->yesbg);
#else
            msgbox->yescolor);
#endif
        if (rc != TEXU_OK)
        {
            return rc;
        }
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, TEXU_IDYES, 0);
        x += texu_strlen(caption) + 1;
    }
    if (idno)
    {
        caption = buttons[F3_NO];
        rc = _TexuMsgBoxProc_CreateButtons(
            wnd,
            y,
            x,
            caption,
            TEXU_IDNO,
            TVK_F3,
            msgbox->nocolor,
#if (defined WIN32)
        msgbox->nobg);
#else
        msgbox->nocolor);
#endif
        if (rc != TEXU_OK)
        {
            return rc;
        }
        x += texu_strlen(caption) + 1;
    }
    if (idcancel)
    {
        caption = buttons[F4_CANCEL];
        /*if there is no cancel button, then assign escape to no button*/
        if (!(idno))
        {
            msgbox->cancelcolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_NO);
#if (defined WIN32)
            msgbox->cancelbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_NO);
#endif
        }
        rc = _TexuMsgBoxProc_CreateButtons(
            wnd,
            y,
            x,
            caption,
            TEXU_IDCANCEL,
            TVK_F4,
            msgbox->cancelcolor,
#if (defined WIN32)
        msgbox->cancelbg);
#else
        msgbox->cancelcolor);
#endif
        if (rc != TEXU_OK)
        {
            return rc;
        }
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ESCAPE, TEXU_IDCANCEL, 0);
        x += texu_strlen(caption) + 1;
    }

    return TEXU_OK;
}

texu_status
_TexuMsgBoxProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_i32 lines = 0;
    texu_i32 reserved = 5;
    texu_i32 maxlen = 0;
    texu_i32 maxbtns = 60; /*"[ F1 - OK ] [ F2 - CANCEL ] [ F3 - YES ] [ F4 - NO ]"*/
    texu_status rc = TEXU_OK;
    texu_rect rect;
    texu_msgbox *msgbox = 0;
    texu_i32 width = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    msgbox = (texu_msgbox *)malloc(sizeof(texu_msgbox));
    if (!msgbox)
    {
        return TEXU_NOMEM;
    }

    memset(msgbox, 0, sizeof(texu_msgbox));
    msgbox->titlecolor = texu_env_get_syscolor(env, TEXU_COLOR_TITLE_WINDOW);
    msgbox->labelcolor = texu_env_get_syscolor(env, TEXU_COLOR_LABEL);
    msgbox->okcolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_OK);
    msgbox->cancelcolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_CANCEL);
    msgbox->yescolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_YES);
    msgbox->nocolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_NO);
#if (defined WIN32)
    msgbox->titlebg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TITLE_WINDOW);
    msgbox->labelbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL);
    msgbox->okbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_OK);
    msgbox->cancelbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_CANCEL);
    msgbox->yesbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_YES);
    msgbox->nobg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_NO);
#endif
    texu_wnd_set_userdata(wnd, msgbox);

    /* adjust window rect */
    lines = _TexuDefWndProc_CountLines(attrs->text, &maxlen);
    maxlen += 4;

    if (attrs->style & TEXU_MBS_OK)
    {
        width += btnwidths[F1_OK] + 2;
    }
    if (attrs->style & TEXU_MBS_YES)
    {
        width += btnwidths[F2_YES] + 2;
    }
    if (attrs->style & TEXU_MBS_NO)
    {
        width += btnwidths[F3_NO] + 2;
    }
    if (attrs->style & TEXU_MBS_CANCEL)
    {
        width += btnwidths[F4_CANCEL] + 2;
    }

    maxbtns = TEXU_MIN(maxbtns, width);
    rect.cols = TEXU_MAX(maxbtns, maxlen) + 2;
    rect.lines = reserved + lines;
    rect.y = (LINES - rect.lines) / 2;
    rect.x = (COLS - rect.cols) / 2;

    texu_wnd_move(wnd, rect.y, rect.x, rect.lines, rect.cols, TEXU_FALSE);

    attrs->x = rect.x;
    attrs->y = rect.y;
    attrs->width = rect.cols;
    attrs->height = rect.lines;

    /*create labels and buttons*/
    rc = _TexuMsgBoxProc_CreateChildren(wnd, attrs, lines);
    if (rc != TEXU_OK)
    {
        free(msgbox);
        return rc;
    }

    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_DIALOG),
                       texu_env_get_syscolor(env, TEXU_COLOR_DIALOG));
#if (defined WIN32 && defined UNICODE)
    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG));
#endif
    return TEXU_OK;
}

void _TexuMsgBoxProc_OnSetCaption(texu_wnd *wnd, const texu_char *caption)
{
    texu_msgbox *msgbox = 0;

    msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    if (caption)
    {
        texu_strcpy(msgbox->caption, caption);
    }
}

void _TexuMsgBoxProc_OnSetOwner(texu_wnd *wnd, texu_wnd *owner)
{
    texu_msgbox *msgbox = 0;

    msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    msgbox->owner = owner;
}

texu_longptr
_TexuMsgBoxProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
    case TEXU_WM_COMMAND:
    {
        /*
        switch (param1)
        {
          case TEXU_IDOK:
          case TEXU_IDCANCEL:
          case TEXU_IDYES:
          case TEXU_IDNO:
            break;
        }*/
        _TexuMsgBoxProc_Notify(wnd, TEXU_MBN_ENDDIALOG, param1);
        texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);
        return 0;
    }

    case TEXU_WM_PAINT:
        _TexuMsgBoxProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuMsgBoxProc_OnDestroy(wnd);
        return 0;

    case TEXU_WM_CREATE:
        return _TexuMsgBoxProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_MBM_SETOWNER:
        _TexuMsgBoxProc_OnSetOwner(wnd, (texu_wnd *)param1);
        return 0;

    case TEXU_MBM_SETCAPTION:
        _TexuMsgBoxProc_OnSetCaption(wnd, (const texu_char *)param1);
        return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU desktop
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_status _TexuDesktopProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
texu_bool _TexuDesktopWndProc_OnEnable(texu_wnd *wnd, texu_bool enable);

texu_status
_TexuDesktopProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
    texu_wnd_set_color(wnd, color, color);
    texu_wnd_enable(wnd, TEXU_TRUE);
    return TEXU_OK;
}

texu_bool
_TexuDesktopWndProc_OnEnable(texu_wnd *wnd, texu_bool enable)
{
    return TEXU_TRUE;
}

texu_longptr
_TexuDesktopProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
    case TEXU_WM_ENABLE:
        return _TexuDesktopWndProc_OnEnable(wnd, (texu_bool)param1);

    case TEXU_WM_CREATE:
        return _TexuDesktopProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
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

void _TexuLabelProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
void _TexuLabelProc_OnEraseBg(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuLabelProc_OnCreate(texu_wnd *, texu_wnd_attrs *);

void
_TexuLabelProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    TexuDefWndProc(wnd, TEXU_WM_ERASEBG, (texu_lparam)cio, (texu_lparam)rect);
}

void
_TexuLabelProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_LABEL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_LABEL_DISABLED);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL_DISABLED);
    texu_ui32 colorbg = normbg;
    texu_i32 cx = texu_env_screen_width(env);
/*    texu_wnd* parent = texu_wnd_get_parent(wnd);*/

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }
    if (rect)
    {
        y = rect->y;
        x = rect->x;
        width = rect->cols;
    }

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_printf_alignment3(buf, text, width, style, TEXU_FALSE, x, cx);

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = normcolor;
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    colorbg = normbg;
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
        colorbg = disbg;
    }
    texu_cio_draw_text(cio, y, x, buf, color, colorbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

texu_status
_TexuLabelProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL_DISABLED));
    texu_wnd_enable(wnd, TEXU_FALSE);

    return TEXU_OK;
}


texu_longptr
_TexuLabelProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
            return _TexuLabelProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuLabelProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_ERASEBG:
            _TexuLabelProc_OnEraseBg(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_ENABLE:
            return TexuDefWndProc(wnd, msg, 0, 0);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU panel
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_status _TexuPanelProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
texu_i32    _TexuPanelProc_OnKeyDown(texu_wnd *, texu_i32 ch, texu_i32 alt);
texu_i32    _TexuPanelProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_wnd*   _TexuPanelProc_OnQueryNextWnd(texu_wnd* wnd);
texu_wnd*   _TexuPanelProc_OnQueryPrevWnd(texu_wnd* wnd);
void        _TexuPanelProc_OnSetFocus(texu_wnd *, texu_wnd *prevwnd);
void        _TexuPanelProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void        _TexuPanelProc_OnSetTitleColor(texu_wnd* wnd, texu_i32 color, texu_i32 bgcolor);
void        _TexuPanelProc_OnSetTitleAlign(texu_wnd *wnd, texu_i32 alignment);
void        _TexuPanelProc_OnSize(texu_wnd *wnd, texu_rect *newsz, texu_rect *oldsz);
texu_wnd*   _TexuPanelProc_OnGetStatusBar(texu_wnd* wnd);
texu_wnd*   _TexuPanelProc_OnGetTitle(texu_wnd* wnd);
void        _TexuPanelProc_OnPaint(texu_wnd* wnd, texu_cio* cio, texu_rect* rect);

texu_wnd*   _TexuPanelProc_OnGetStatusBar(texu_wnd* wnd)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_wnd* status = 0;
    if (style & TEXU_PNS_STATUS)
    {
        status = texu_wnd_find_child(wnd, 0);
    }
    return status;
}

texu_wnd*   _TexuPanelProc_OnGetTitle(texu_wnd* wnd)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_wnd* title = 0;
    if (style & TEXU_PNS_TITLE)
    {
        title = texu_wnd_find_child(wnd, -1);
    }
    return title;
}

void
_TexuPanelProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_i32 style = texu_wnd_get_style(wnd);

    if (style & TEXU_PNS_TITLE)
    {
/*        texu_env* env = texu_wnd_get_env(wnd);
        texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_PANEL_TITLE);
        texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PANEL_TITLE);
*/
        texu_wnd* title = texu_wnd_find_child(wnd, -1); /*the top of label*/
/*        texu_wnd_set_color(title, color, bgcolor);*/
        texu_wnd_set_text(title, text);
    }
}

void
_TexuPanelProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    /*to make sure that the first active child is set focus*/
    texu_wnd* activechild = texu_wnd_get_activechild(wnd);
    /*texu_wnd_invalidate(activechild);*/
    /*simulate key down to the first active child*/
    texu_wnd_send_msg(activechild, TEXU_WM_KEYDOWN, (texu_lparam)TEXU_KEY_LEFT, 0);
}

texu_status
_TexuPanelProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_i32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_i32 disbgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PANEL_TITLE);
    /*container MUST BE clip all children*/
    texu_wnd_set_style(wnd, attrs->style | TEXU_WS_CLIPWINDOW);
    texu_wnd_set_color(wnd,
                       color,
                       discolor);

    texu_wnd_set_bgcolor(wnd,
                         bgcolor,
                         disbgcolor);
    /*texu_wnd_enable(wnd, TEXU_FALSE);*/
    /*to paint the background*/
    texu__wnd_create_blank_lines(env, wnd);
    /*after creating the blank lines*/
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_wnd* title = texu_wnd_find_child(wnd, -1); /*the top of label*/
    if ((attrs->style & TEXU_PNS_TITLE) && (attrs->style & TEXU_PNS_BORDER))
    {
        /*there are title and border*/
#if defined TEXU_CIO_COLOR_MONO
        /*hide the first line and resize all lines*/
        texu_i32 id = 0;
        texu_i32 i = 0;
        texu_wnd* label = 0;
        texu_rect rect;
        for (i = 1; i < attrs->height; ++i)
        {
            id = (-1)*(i + 1);
            label = texu_wnd_find_child(wnd, id);
            texu_wnd_get_rect(label, &rect);
            texu_wnd_move(label, rect.y, rect.x + 1, rect.lines, rect.cols - 2, TEXU_FALSE);
        }
        texu_wnd_visible(label, TEXU_FALSE); /*prepare to draw at _TexuPanelProc_OnPaint()*/
        texu_wnd_visible(title, TEXU_FALSE); /*prepare to draw at _TexuPanelProc_OnPaint()*/
#else
        texu_wnd_set_color(title, color, bgcolor);
#endif
        texu_wnd_set_text(title, text);
    }
    else if (attrs->style & TEXU_PNS_TITLE)
    {
        /*there is a title*/
#if defined TEXU_CIO_COLOR_MONO
        texu_wnd_visible(title, TEXU_FALSE); /*prepare to draw at _TexuPanelProc_OnPaint()*/
#else
        texu_wnd_set_color(title, color, bgcolor);
#endif
        texu_wnd_set_text(title, text);
    }
    else if (attrs->style & TEXU_PNS_BORDER)
    {
        /*there is a border*/
#if defined TEXU_CIO_COLOR_MONO
        /*hide the first line and resize all lines*/
        texu_i32 id = 0;
        texu_i32 i = 0;
        texu_wnd* label = 0;
        texu_rect rect;
        for (i = 1; i < attrs->height; ++i)
        {
            id = (-1)*(i + 1);
            label = texu_wnd_find_child(wnd, id);
            texu_wnd_get_rect(label, &rect);
            texu_wnd_move(label, rect.y, rect.x + 1, rect.lines, rect.cols - 2, TEXU_FALSE);
        }
        texu_wnd_visible(label, TEXU_FALSE); /*prepare to draw at _TexuPanelProc_OnPaint()*/
        texu_wnd_visible(title, TEXU_FALSE);
#endif
    }
    /*statusbar*/
    if (attrs->style & TEXU_PNS_STATUS)
    {
        texu_wnd_attrs attrs2;/* = *attrs;*/
        texu_wnd* status = texu_wnd_new(env);
        if (!status)
        {
            return -1;
        }
        memset(&attrs2, 0, sizeof(attrs2));
        attrs2.y = attrs->y + attrs->height - 1;
        attrs2.x = attrs->x;
        attrs2.height = 1;
        attrs2.width = attrs->width;
#if defined TEXU_CIO_COLOR_MONO
        if (attrs->style & TEXU_PNS_TITLE)
        {
        }
        else
        {
        }
        if (attrs->style & TEXU_PNS_BORDER)
        {
            ++attrs2.x;
            attrs2.width -= 2;
        }
        else
        {
        }
#else
#endif
        attrs2.enable = TEXU_TRUE;
#if defined TEXU_CIO_COLOR_MONO
        attrs2.visible = TEXU_FALSE;
#else
        attrs2.visible = TEXU_TRUE;
#endif
        attrs2.text = TEXUTEXT("");

        texu_wnd_get_color(wnd, &attrs2.normalcolor, &attrs2.disabledcolor);
        attrs2.focusedcolor = texu_wnd_get_focused_color(wnd);
        texu_wnd_get_bgcolor(wnd, &attrs2.normalbg, &attrs2.disabledbg);
        attrs2.focusedbg = texu_wnd_get_bgfocused_color(wnd);

        attrs2.id = 0;
        attrs2.clsname = TEXU_STATUSBAR_CLASS;
        attrs2.userdata = 0;
        attrs2.style = TEXU_WS_LEFT;
        attrs2.exstyle = 0;
        attrs2.on_validate = 0;

        texu_i32 rc = texu_wnd_create(status, wnd, &attrs2);
        if (rc != TEXU_OK)
        {
            texu_wnd_del(status);
            return TEXU_ERROR;
        }
    }
    return TEXU_OK;
}


texu_wnd* _TexuPanelProc_OnQueryNextWnd(texu_wnd* wnd)
{
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    return texu_wnd_get_next_activechild(wnd, activewnd);
}

texu_wnd* _TexuPanelProc_OnQueryPrevWnd(texu_wnd* wnd)
{
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    return texu_wnd_get_prev_activechild(wnd, activewnd);
}

texu_i32
_TexuPanelProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    /* the current frame window is active */
    if (activewnd && activewnd != wnd)
    {
        texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_lparam)ch, alt);
    }
    else if (activewnd && activewnd == wnd)
    {
        activewnd = texu_wnd_get_activechild(activewnd);
        if (activewnd)
        {
            texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_lparam)ch, alt);
        }
    }
    return 0;
}

texu_i32    _TexuPanelProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_i32 rc = 0;
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = 0;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd *desktop = (env ? texu_env_get_desktop(env) : 0);
    texu_wnd_keycmd *keycmd = 0;
    texu_menu *menu = 0;
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);
    texu_wnd* qnextwnd = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
    if (keycmd && parent == desktop)
    {
        /* if there are any hotkey registered */
        return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_lparam)keycmd->cmd, alt);
    }
    if (parent)
    {
        if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
        {
            menu = texu_wnd_get_menu(wnd);
            if (menu)
            {
                rc = texu_wnd_send_msg(wnd, TEXU_WM_ENTERMENU, ch, alt);
                return 0;
            }
        }
        if (activewnd && activewnd != wnd)
        {
            if (ch == chNextKey)
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYNEXTWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
                }
            }
            else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYPREVWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
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
                        rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_lparam)activewnd, 0);
                        if (rc != TEXU_OK)
                        {
                            texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_lparam)activechild, 0);
                            return 0;
                        }
                    }
                    /*active window need to be done something by itself*/
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);
                        return 0;
                    }
                    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);
                    texu_i32 y = texu_wnd_get_y(nextwnd);
                    texu_i32 x = texu_wnd_get_x(nextwnd);
                    texu_i32 width = texu_wnd_get_width(nextwnd);

                    texu_cio* cio = texu_wnd_get_cio(wnd);
                    texu_cio_gotoyx(cio, y, x + width - 1);

                    /* the new active window */
                    texu_wnd_set_activechild(wnd, nextwnd);
                    /*texu_wnd_send_msg(nextwnd, TEXU_WM_KEYDOWN, (texu_lparam)ch, alt);*/
                    return 0;
                }
                else
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_lparam)ch, alt);
                    return 0;
                }
            }
            else
            {
                texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_lparam)ch, alt);
                return 0;
            }
        }
    }
    else
    {
        if (activewnd)
        {
            return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_lparam)ch, alt);
        }
    }
    return 0;
}

void
_TexuPanelProc_OnPaint(texu_wnd* wnd, texu_cio* cio, texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }

#if defined TEXU_CIO_COLOR_MONO
    texu_wnd* title = texu_wnd_find_child(wnd, -1);
    texu_rect rcwnd;
    texu_i32 discolor;
    texu_i32 disbgcolor;
    if (rect)
    {
        rcwnd = *rect;
    }
    else
    {
        texu_wnd_get_rect(wnd, &rcwnd);
    }
    texu_wnd_get_color(wnd, &color, &discolor);
    texu_wnd_get_bgcolor(wnd, &bgcolor, &disbgcolor);

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    if (style & TEXU_PNS_TITLE)
    {
        /*there are title and border*/
        texu_cio_draw_frame(cio, text, &rcwnd, color);
    }
    else if (style & TEXU_PNS_BORDER)
    {
        /*there are title and border*/
        texu_cio_draw_rect(cio, &rcwnd, color);
    }
    if (style & TEXU_PNS_STATUS)
    {
        texu_wnd* status = texu_wnd_find_child(wnd, 0);
        texu_i32 nparts = texu_wnd_send_msg(status, TEXU_SBM_GETPARTCOUNT, 0, 0);
        texu_char text[TEXU_MAX_WNDTEXT + 1] = "";
        texu_char buf[TEXU_MAX_WNDTEXT + 1] = "";
        texu_i32 i = 0;
        for (i = 0; i < nparts; ++i)
        {
            sprintf(buf, "[ %s ]", texu_wnd_send_msg(status, TEXU_SBM_GETTEXT, (texu_lparam)i, 0));
            texu_strcat(text, buf);
        }
        texu_rect rc;
        texu_wnd_get_rect(status, &rc);
        texu_cio_draw_text(cio, rc.y, rc.x, text, color, bgcolor,
                           texu_wnd_get_clsname(wnd),
                           texu_wnd_get_id(wnd));
    }
#endif
}
void _TexuPanelProc_OnSetTitleColor(texu_wnd* wnd, texu_i32 color, texu_i32 bgcolor)
{
    texu_i32 style = texu_wnd_get_style(wnd);
    if (style & TEXU_PNS_TITLE)
    {
        texu__wnd_set_blank_line_color(wnd, 
            -1, /*the top of the blank line*/
            color, bgcolor);
    }
}

void _TexuPanelProc_OnSetTitleAlign(texu_wnd *wnd, texu_i32 alignment)
{
    texu_i32 style = texu_wnd_get_style(wnd);
    if (style & TEXU_PNS_TITLE)
    {
        texu_wnd* title = texu_wnd_find_child(wnd, -1); /*the top of label*/
        style = texu_wnd_get_style(title);
        style &= ~(TEXU_ALIGN_CENTER | TEXU_ALIGN_LEFT | TEXU_ALIGN_RIGHT);
        if (alignment & TEXU_ALIGN_CENTER)
        {
            alignment = TEXU_ALIGN_CENTER;
        }
        else if (alignment & TEXU_ALIGN_LEFT)
        {
            alignment = TEXU_ALIGN_LEFT;
        }
        else
        {
            alignment = TEXU_ALIGN_RIGHT;
        }
        style |= alignment;
        texu_wnd_set_style(title, style);
    }
}

void _TexuPanelProc_OnSize(texu_wnd *wnd, texu_rect *newsz, texu_rect *oldsz)
{
    texu_i32 id;
    texu_rect rcchild;
/*    texu_i32 lines = newsz->lines;*/
    texu_wnd* child = texu_wnd_firstchild(wnd);
    texu_i32 dy = (oldsz ? newsz->y - oldsz->y : 0);
    texu_i32 dx = (oldsz ? newsz->x - oldsz->x : 0);

#if defined TEXU_CIO_COLOR_MONO
    texu_ui32 style = texu_wnd_get_style(wnd);
#endif

    while (child)
    {
        texu_wnd_get_rect(child, &rcchild);
        rcchild.y += dy;
        rcchild.x += dx;

        id = texu_wnd_get_id(child);
        if (id < 0)
        {
            /*special characteristic of the blank lines*/
            /*its width must always be equal to the parent*/
            rcchild.cols = newsz->cols;
#if defined TEXU_CIO_COLOR_MONO
            if (style & TEXU_PNS_BORDER)
            {
                rcchild.cols = newsz->cols - 2;
            }
#endif
            texu_wnd_send_msg(child, TEXU_WM_SIZE, (texu_lparam)&rcchild, 0);
        }
        else
        {
            texu_wnd_send_msg(child, TEXU_WM_MOVE, (texu_lparam)&rcchild, TEXU_TRUE);
        }
        child = texu_wnd_nextwnd(child);
    }
}


texu_longptr
_TexuPanelProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_PNM_GETTITLE:
            return (texu_longptr)_TexuPanelProc_OnGetTitle(wnd);

        case TEXU_PNM_GETSTATUSBAR:
            return (texu_longptr)_TexuPanelProc_OnGetStatusBar(wnd);

        case TEXU_PNM_SETTITLEALIGNMENT:
            _TexuPanelProc_OnSetTitleAlign(wnd, (texu_i32)param1);
            break;

        case TEXU_PNM_SETTITLECOLOR:
            _TexuPanelProc_OnSetTitleColor(wnd, (texu_i32)param1, (texu_i32)param2);
            break;

        case TEXU_WM_SETTEXT:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuPanelProc_OnSetText(wnd, (const texu_char *)param1);
            break;

        case TEXU_WM_SIZE:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuPanelProc_OnSize(wnd, (texu_rect *)param1, (texu_rect *)param2);
            return 0;

        case TEXU_WM_COMMAND:
        case TEXU_WM_NOTIFY:
            texu_wnd_send_msg(texu_wnd_get_parent(wnd), msg, param1, param2);
            return 0;

        case TEXU_WM_SETFOCUS:
            TexuDefWndProc(wnd, msg, param1, param2);   /*call default proc first*/
            _TexuPanelProc_OnSetFocus(wnd, (texu_wnd *)param1); /*set the first focus if it has not*/
            break;

        case TEXU_WM_CREATE:
            return _TexuPanelProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_KEYDOWN:/*virtual key*/
            return _TexuPanelProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);

        case TEXU_WM_CHAR:/*printable chars*/
            return _TexuPanelProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);


        case TEXU_WM_QUERYNEXTWND:
            return (texu_lparam)_TexuPanelProc_OnQueryNextWnd(wnd);

        case TEXU_WM_QUERYPREVWND:
            return (texu_lparam)_TexuPanelProc_OnQueryPrevWnd(wnd);

        case TEXU_WM_PAINT:
            _TexuPanelProc_OnPaint(wnd, (texu_cio*)param1, (texu_rect*)param2);
            return 0;
/*
        case TEXU_WM_ENABLE:
            return TexuDefWndProc(wnd, msg, 0, 0);*/
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU button
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_btnwnd
{
    texu_i32    state;  /*focus = 1, normal = 0*/
};
typedef struct texu_btnwnd texu_btnwnd;

void            _TexuButtonProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status     _TexuButtonProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void            _TexuButtonProc_OnPush(texu_wnd *wnd);
void            _TexuButtonProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);

void
_TexuButtonProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_char   buf[TEXU_MAX_WNDTEXT + 1];
    texu_char   text[TEXU_MAX_WNDTEXT + 1];
    texu_i32    y = texu_wnd_get_y(wnd);
    texu_i32    x = texu_wnd_get_x(wnd);
    texu_i32    width = texu_wnd_get_width(wnd);
    texu_env    *env = texu_wnd_get_env(wnd);
    texu_ui32    normcolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON);
    texu_ui32    discolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_DISABLED);
    texu_ui32    color = TEXU_CIO_COLOR_CYAN_BLACK;
    size_t      len = 0;
    texu_btnwnd *btnwnd = (texu_btnwnd*)texu_wnd_get_userdata(wnd);

    texu_ui32    normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON);
    texu_ui32    disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_DISABLED);
    texu_ui32    bgcolor = TEXU_CIO_COLOR_CYAN_BLACK;
    texu_i32    cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_printf_alignment3(buf, text, width, TEXU_ALIGN_CENTER, TEXU_TRUE, x, cx);

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = normcolor;
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
    }
    if (1 == btnwnd->state)
    {
        len = texu_strlen(buf);
        if (len > 1)
        {
            buf[0] = TEXUTEXT('[');
            buf[len - 1] = TEXUTEXT(']');
        }
    }
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    bgcolor = normbg;
    if (!(texu_wnd_is_enable(wnd)))
    {
        bgcolor = disbg;
    }

    texu_cio_draw_text(cio, y, x, buf, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

texu_status
_TexuButtonProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_btnwnd *btnwnd = (texu_btnwnd*)malloc(sizeof(texu_btnwnd));
    if (!btnwnd)
    {
        return TEXU_ERROR;
    }
    btnwnd->state = 0;  /*normal*/
    texu_wnd_set_userdata(wnd, btnwnd);
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_BUTTON),
                       texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_DISABLED));

    return TEXU_OK;
}

void
_TexuButtonProc_OnDestroy(texu_wnd *wnd)
{
    texu_btnwnd *btnwnd = (texu_btnwnd*)texu_wnd_get_userdata(wnd);
    free(btnwnd);
    texu_wnd_set_userdata(wnd, 0);
}

void _TexuButtonProc_OnPush(texu_wnd *wnd)
{
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    texu_wnd_send_msg(parent, TEXU_WM_COMMAND, texu_wnd_get_id(wnd), 0);
}

void
_TexuButtonProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (TEXUTEXT(' ') == ch) /*space bar*/
    {
        _TexuButtonProc_OnPush(wnd);
        return;
    }
}
void
_TexuButtonProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_btnwnd *btnwnd = (texu_btnwnd*)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    btnwnd->state = 1; /*focus*/
    _TexuWndProc_Notify(wnd, TEXU_BN_SETFOCUS);
    texu_wnd_invalidate(wnd);
}

texu_i32
_TexuButtonProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_btnwnd *btnwnd = (texu_btnwnd*)texu_wnd_get_userdata(wnd);
    btnwnd->state = 0; /*normal*/

    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    texu_wnd_invalidate(wnd);
    _TexuWndProc_Notify(wnd, TEXU_BN_KILLFOCUS);
    return 0;
}

texu_longptr
_TexuButtonProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
            return _TexuButtonProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_DESTROY:
            _TexuButtonProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuButtonProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
            
        case TEXU_WM_SETFOCUS:
            _TexuButtonProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            _TexuButtonProc_OnKillFocus(wnd, (texu_wnd *)param1);
            break;
         
        case TEXU_WM_KEYDOWN:
        {
            _TexuButtonProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            break;
        }
        
        case TEXU_BM_PUSH:
        {
            _TexuButtonProc_OnPush(wnd);
            break;
        }
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
    texu_i32 firstvisit;
    texu_i32 selected;
    texu_char passchar;
    texu_bool showpass;
    texu_i32 firstchar;
    texu_i32 limitchars;
    texu_i32 editing;
    texu_i32 decwidth;
    texu_i32 min;
    texu_i32 max;
    texu_i32 onminmax;
    texu_bool invalid;
    void *exparam;
    texu_char validstr[TEXU_MAX_WNDTEXT + 1];
    texu_char editbuf[TEXU_MAX_WNDTEXT + 1];
    texu_i32  (*on_validate)(texu_wnd*, texu_char*);
};
typedef struct texu_editwnd texu_editwnd;

#define TEXU_EDITWND_KILO   (1000)
#define TEXU_EDITWND_MEGA   (1000000)
#define TEXU_EDITWND_GIGA   (1000000000)
#define TEXU_EDITWND_TERA   (1000000000000)
#define TEXU_EDITWND_PETA   (1000000000000000)
#define TEXU_EDITWND_EXA    (1000000000000000000)

texu_i32    _TexuEditProc_OnKeyDown(texu_wnd *, texu_i32 ch, texu_i32 alt);
texu_status _TexuEditProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuEditProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);
void _TexuEditProc_OnDestroy(texu_wnd *);
void _TexuEditProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect);
void _TexuEditProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio, texu_rect* rect);
texu_i32 _TexuEditProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd);
void _TexuEditProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
void _TexuEditProc_OnLimitText(texu_wnd *wnd, texu_i32 limit);
void _TexuEditProc_OnSetPasswdChar(texu_wnd *wnd, texu_char passchar);
void _TexuEditProc_OnShowPasswdChar(texu_wnd *wnd, texu_i32 show);
void _TexuEditProc_OnSetDecWidth(texu_wnd *wnd, texu_i32 width);
void _TexuEditProc_OnSetValidMinMax(texu_wnd *wnd, texu_i32 on, texu_editminmax *vmm);
void _TexuEditProc_OnSetText(texu_wnd *wnd, const texu_char *text);
texu_i32    _TexuEditProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);


texu_status _TexuEditProc_ValidateNumberStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch);
texu_status _TexuEditProc_ValidateDecimalStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch);
texu_status _TexuEditProc_ValidateA2ZStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch);
texu_i32 _TexuEditProc_RemoveDecimalFormat(texu_editwnd *edit);
texu_i32 _TexuEditProc_AddDecimalFormat(texu_editwnd *edit);
texu_status _TexuEditProc_ValidateMinMax(texu_editwnd *edit, texu_i32 val);
texu_i32 _TexuEditProc_SelMinMax(texu_i32 min, texu_i32 max, texu_i32 val);

texu_status
_TexuEditProc_ValidateMinMax(texu_editwnd *edit, texu_i32 val)
{
    if (edit->onminmax && val >= edit->min && val <= edit->max)
    {
        return TEXU_OK;
    }
    return TEXU_ERROR;
}

void _TexuEditProc_OnLimitText(texu_wnd *wnd, texu_i32 limit)
{
    texu_editwnd *edit = 0;
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    if (limit > 0 || limit <= TEXU_MAX_WNDTEXT)
    {
        edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
        edit->limitchars = limit;

        texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
        if (texu_strlen(text) > (texu_ui32)limit)
        {
            text[limit] = 0;
            texu_wnd_set_text(wnd, text);
            texu_strcpy(edit->editbuf, text);
        }
    }
}

void _TexuEditProc_OnSetPasswdChar(texu_wnd *wnd, texu_char passchar)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    edit->passchar = passchar;
}

void _TexuEditProc_OnShowPasswdChar(texu_wnd *wnd, texu_i32 show)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    edit->showpass = (show == TEXU_TRUE ? TEXU_TRUE : TEXU_FALSE);
}

void _TexuEditProc_OnSetDecWidth(texu_wnd *wnd, texu_i32 width)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    if (width < 0)
    {
        width = 0;
    }
    else if (width > 6)
    {
        width = 6;
    }
    edit->decwidth = width;
}
/*
texu_i32
_TexuEditProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = (wnd ? texu_wnd_get_style(wnd) : 0);
    texu_ui32 len = texu_strlen(edit ? edit->editbuf : "");
    
    if (!wnd)
    {
        return -1;
    }
    if (textlen < 0 || 0 == text)
    {
        return len;
    }

    memset(text, 0, textlen);
    if (style & TEXU_ES_PASSWORD)
    {
        
        texu_memset(text, (edit ? edit->passchar : TEXUTEXT('*')), 8);
    }
    else
    {
       TexuDefWndProc(wnd, TEXU_WM_GETTEXT, (texu_lparam)text, textlen);
    }

    return textlen;
}
*/

void _TexuEditProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_editwnd *edit = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (!text)
    {
        return;
    }
    /*copy to buffer to prevent Mac crashed*/
    texu_strcpy(buf, text);

    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)text, 0);
    edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_strcpy(edit->editbuf, buf);
    edit->firstvisit = 1;

    if (TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
        _TexuEditProc_AddDecimalFormat(edit);
    }
}

void _TexuEditProc_OnSetValidString(texu_wnd *wnd, const texu_char *validstr)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    if (validstr)
    {
        /* set the valid string */
        texu_strcpy(edit->validstr, validstr);
    }
    else
    {
        /* remove valid string */
        memset(edit->validstr, 0, sizeof(edit->validstr));
    }
}

texu_i32
_TexuEditProc_SelMinMax(texu_i32 min, texu_i32 max, texu_i32 val)
{
    if (val < min)
    {
        val = min;
    }
    if (val > max)
    {
        val = max;
    }
    return val;
}

void _TexuEditProc_OnInvalidate(texu_wnd *wnd)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    edit->invalid = TEXU_TRUE;
}

void _TexuEditProc_OnSetValidMinMax(texu_wnd *wnd, texu_i32 on, texu_editminmax *vmm)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 val = 0;

    edit->min = vmm->min;
    edit->max = vmm->max;
    edit->onminmax = (on == TEXU_TRUE ? TEXU_TRUE : TEXU_FALSE);

    if (on)
    {
        texu_strcpy(buf, edit->editbuf);
        val = texu_atol(buf);
        val = _TexuEditProc_SelMinMax(vmm->min, vmm->max, val);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
        texu_wnd_set_text(wnd, buf);
    }
}

texu_i32 _TexuEditProc_AddDecimalFormat(texu_editwnd *edit)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 cnt = 1;
    texu_char *pbuf;
    texu_i32 len = texu_strlen(edit->editbuf) - 1;
    texu_char *psz = &edit->editbuf[len];

    memset(buf, 0, sizeof(buf));
    pbuf = buf;

    while (*psz != 0 && *psz != TEXUTEXT('.'))
    {
        *pbuf = *psz;
        ++pbuf;
        --psz;
        --len;
    }
    if (TEXUTEXT('.') == *psz)
    {
        *pbuf = *psz;
        ++pbuf;
        --psz;
        --len;
    }

    while (len >= 0)
    {
        if (cnt % 4 == 0 && *psz != TEXUTEXT('-'))
        {
            *pbuf = TEXUTEXT(',');
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
    len = texu_strlen(buf) - 1;
    pbuf = &buf[len];
    psz = edit->editbuf;
    while (len >= 0)
    {
        *psz = *pbuf;
        ++psz;
        --pbuf;
        --len;
    }
    return TEXU_OK;
}

texu_i32 _TexuEditProc_RemoveDecimalFormat(texu_editwnd *edit)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char *psz = edit->editbuf;
    texu_char *pbuf = buf;

    if (texu_strchr(edit->editbuf, TEXUTEXT(',')))
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
        /* save */
        texu_strcpy(edit->editbuf, buf);
    }
    return TEXU_OK;
}

texu_i32
_TexuEditProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_f64 decimal = 0.0;
    texu_i32 number = 0;
    texu_i32 rcminmax = TEXU_OK;
    texu_i32 rc = TEXU_OK;

    /* check if style is TEXU_ES_DECIMAL */
    if (TEXU_ES_DECIMAL & style || TEXU_ES_AUTODECIMALCOMMA & style)
    {
        decimal = texu_atof(edit->editbuf);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%.*f"), (texu_i32)edit->decwidth, decimal);
        texu_strcpy(edit->editbuf, buf);
        if (TEXU_TRUE == edit->onminmax)
        {
            if (decimal < (texu_f64)edit->min &&
                decimal > (texu_f64)edit->max)
            {
                _TexuWndProc_Notify(wnd, TEXU_EN_INVALIDMINMAX);
                texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
                rcminmax = TEXU_ERROR;
            }
        }
    }
    else
    {
        texu_strcpy(buf, edit->editbuf);
    }

    if ((TEXU_ES_NUMBER & style) && (TEXU_TRUE == edit->onminmax))
    {
        number = texu_atol(edit->editbuf);
        if (number < edit->min &&
            number > edit->max)
        {
            rcminmax = TEXU_ERROR;
        }
    }
    /* validate min & max */
    if (rcminmax != TEXU_CONTINUE)
    {
        _TexuWndProc_Notify(wnd, TEXU_EN_INVALIDMINMAX);
        texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
        return rcminmax;
    }

    if (edit->on_validate)
    {
        rc = edit->on_validate(wnd, edit->editbuf);
        if (rc != TEXU_OK)
        {
            _TexuWndProc_Notify(wnd, TEXU_EN_INVALIDATE);
            texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
            return rc;
        }
    }

    if (TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_AddDecimalFormat(edit);
    }
    /* update text */
    edit->firstvisit = 1;
    edit->firstchar = 0;
    edit->editing = 0;
    edit->selected = 0;
    texu_wnd_set_text(wnd, edit->editbuf);
    
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    texu_wnd_invalidate(wnd);

    _TexuWndProc_Notify(wnd, TEXU_EN_KILLFOCUS);
    return TEXU_OK;
}

void
_TexuEditProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = texu_wnd_get_style(wnd);
    /*texu_env *env = texu_wnd_get_env(wnd);*/

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    texu_wnd_get_text(wnd, edit->editbuf, TEXU_MAX_WNDTEXT);

    if (TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
    }

    edit->firstvisit = 1;
    edit->firstchar = 0;
    edit->editing = 0;
    edit->selected = 1;

    _TexuWndProc_Notify(wnd, TEXU_EN_SETFOCUS);

    texu_wnd_invalidate(wnd);
}

void
_TexuEditProc_OnDestroy(texu_wnd *wnd)
{
    texu_editwnd *edit = texu_wnd_get_userdata(wnd);
    if (edit)
    {
        free(edit);
        texu_wnd_set_userdata(wnd, 0);
    }
}

texu_status _TexuEditProc_ValidateA2ZStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch)
{
    texu_status rc = TEXU_OK;
    if (ch >= TEXUTEXT('a') && ch <= TEXUTEXT('z'))
    {
        rc = TEXU_OK;
    }
    else if (ch >= TEXUTEXT('A') && ch <= TEXUTEXT('Z'))
    {
        rc = TEXU_OK;
    }
    return rc;
}

texu_status _TexuEditProc_ValidateNumberStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch)
{
    texu_status rc = TEXU_OK;
    if (ch < TEXUTEXT('0') || ch > TEXUTEXT('9'))
    {
        return TEXU_ERROR;
    }
    if (edit->onminmax)
    {
        texu_i32 len = texu_strlen(edit->editbuf);
        texu_i32 val = 0;
        texu_char newbuf[TEXU_MAX_WNDTEXT + 1];
        memset(newbuf, 0, sizeof(newbuf));

        if (edit->firstvisit)
        {
            newbuf[0] = ch;
        }
        else
        {
            texu_strcpy(newbuf, edit->editbuf);
            newbuf[len] = ch;
        }
        val = texu_atol(newbuf);
        rc = _TexuEditProc_ValidateMinMax(edit, val);
    }
    return rc;
}

texu_status _TexuEditProc_ValidateDecimalStyle(texu_wnd *wnd, texu_editwnd *edit, texu_i32 ch)
{
    texu_status rc = TEXU_OK;
    texu_char *decimal = texu_strchr(edit->editbuf, TEXUTEXT('.'));
    texu_i32 len = texu_strlen(edit->editbuf);
    texu_i32 pos = -1;
    texu_i32 declen = 0;

    /* not allowed '-' in the string */
    if ((len > 0 && (edit->firstvisit == 0)) && ch == TEXUTEXT('-'))
    {
        rc = TEXU_ERROR;
    }
    if ((rc == TEXU_OK) && (len == 0 || (edit->firstvisit == 1)) && ch == TEXUTEXT('-'))
    {
        /* ok */
    }
    else if (ch == TEXUTEXT('.'))
    {
        if (decimal)
        {
            /* not allowed having '.' more than one */
            rc = TEXU_ERROR;
        }
    }
    else if (ch < TEXUTEXT('0') || ch > TEXUTEXT('9'))
    {
        rc = TEXU_ERROR;
    }
    if (TEXU_OK == rc && decimal)
    {
        pos = (texu_i32)(decimal - edit->editbuf);
        declen = len - pos - 1;
        if (declen >= edit->decwidth && (edit->firstvisit == 0))
        {
            rc = TEXU_ERROR;
        }
        else if (declen <= edit->decwidth && (edit->selected == 1))
        {
            rc = TEXU_OK;
        }
    }
    if (TEXU_OK == rc && edit->onminmax)
    {
        texu_i32 len = texu_strlen(edit->editbuf);
        texu_f64 val = 0;
        texu_char newbuf[TEXU_MAX_WNDTEXT + 1];
        memset(newbuf, 0, sizeof(newbuf));

        if (edit->firstvisit)
        {
            newbuf[0] = ch;
        }
        else
        {
            texu_strcpy(newbuf, edit->editbuf);
            newbuf[len] = ch;
        }
        val = texu_atof(newbuf);
        rc = (val >= (texu_f64)edit->min && val <= (texu_f64)edit->max) ? TEXU_OK : TEXU_ERROR;
    }
    return rc;
}

texu_i32
_TexuEditProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    switch (ch)
    {
        case TEXU_KEY_LEFT:
        case TEXU_KEY_RIGHT:
        case TEXU_KEY_UP:
        case TEXU_KEY_DOWN:
            texu_wnd_invalidate(wnd);
            break;
    }
    return 0;
}

/* Select all       : CTRL + A*/
/* Cancel editting  : ESCAPE*/
void
_TexuEditProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_editwnd *edit = 0;
    texu_i32 changed = 0;
    texu_cio *dc = texu_wnd_get_cio(wnd);
    texu_i32 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED);
    texu_i32 ret = TEXU_OK;
    texu_char *psz = 0;
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    texu_i32 cx = texu_env_screen_width(env);
#if (defined WIN32)// && defined _WINDOWS)
    static texu_char printable_chars[] = TEXUTEXT(" ~!@#$%^&*_-+=,?/|.\\\'\"()[]{}<>abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
#endif
    texu_char textcommas[TEXU_MAX_WNDTEXT + 1];

    if (!(texu_wnd_is_enable(wnd)))
    {
        return;
    }

    edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(wnd, textcommas, TEXU_MAX_WNDTEXT);
    texu_wnd_get_color(wnd, &normcolor, &discolor);

    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    if (TEXU_ES_AUTOCOMMAS & style)
    {
        if ((TEXU_ES_NUMBER & style) ||
            (TEXU_ES_DECIMAL & style))
        {
            /*remove commas*/
            texu_remove_commas(text, TEXU_MAX_WNDTEXT, textcommas);
        }
    }


    /*select all*/
    if ((alt & TEXU_KEYPRESSED_CTRL) && ((TEXUTEXT('a') == ch) || (TEXUTEXT('A') == ch)))
    {
        edit->firstvisit = 1;
        edit->selected = 1;
        texu_wnd_invalidate(wnd);
        return;
    }

    /*simulate escape pressed*/
    if ((alt & TEXU_KEYPRESSED_CTRL) && ((TEXUTEXT('e') == ch) || (TEXUTEXT('E') == ch)))
    {
        ch = TEXU_KEY_ESCAPE;
    }
    if (TEXU_KEY_ESCAPE == ch) /* == KEY_ESCAPE)*/
    {
        if (edit->firstvisit == 0)
        {
            edit->firstvisit = 1;
            texu_strcpy(edit->editbuf, text);
            edit->firstchar = 0;
            edit->editing = 0;
            edit->selected = 1;

            len = texu_strlen(edit->editbuf);
            len = TEXU_MIN(len, width);
            texu_env_set_cursor(env, y, x + len);
            /*texu_env_show_cursor(env, TEXU_TRUE);*/
            texu_wnd_invalidate(wnd);
            return;
        }
    }
#if (defined WIN32)// && defined _WINDOWS)
    texu_char ch2 = texu_win32_oem_chars(ch);
    if (ch2 > 0)
    {
        ch = ch2;
    }
    if (texu_strchr(printable_chars, ch))
    {
#else
    if (ch >= 0x20 && ch < 0x7f)
    {
#endif
        /* add char */
        len = texu_strlen(edit->editbuf);
        /*test block with its styles*/
        do
        {
            ret = TEXU_OK;
            if (TEXU_ES_NUMBER & style)
            {
                /* require only number input */
                ret = _TexuEditProc_ValidateNumberStyle(wnd, edit, ch);
                if (ret != TEXU_OK)
                {
                    break;
                }
            }
            else if (TEXU_ES_DECIMAL & style || TEXU_ES_AUTODECIMALCOMMA & style)
            {
                /* require only decimal input */
                ret = _TexuEditProc_ValidateDecimalStyle(wnd, edit, ch);
                if (ret != TEXU_OK)
                {
                    break;
                }
            }
            else if (TEXU_ES_UPPERCASE & style)
            {
                /* require changing from small to CAPITAL */
                if (ch >= TEXUTEXT('a') && ch <= TEXUTEXT('z'))
                {
                    ch = ch - TEXUTEXT('a') + TEXUTEXT('A');
                }
            }
            else if (TEXU_ES_LOWERCASE & style)
            {
                if (ch >= TEXUTEXT('A') && ch <= TEXUTEXT('Z'))
                {
                    ch = ch - TEXUTEXT('A') + TEXUTEXT('a');
                }
            }
            if (TEXU_ES_A2Z & style)
            {
                /* require only A-Z input */
                ret = _TexuEditProc_ValidateA2ZStyle(wnd, edit, ch);
                if (ret != TEXU_OK)
                {
                    break;
                }
            }
            /* valid char if it is in valid string */
            if (edit->validstr[0] != 0)
            {
                psz = texu_strchr(edit->validstr, ch);
                if (!psz)
                {
                    ret = TEXU_EVALID;
                    break;
                }
            }
        } while (0);
        /*validate failed*/
        if (ret != TEXU_OK)
        {
            return;
        }

        /* is the first typing? */
        if (edit->firstvisit == 1)
        {
            edit->firstvisit = 0;
            if (!(TEXU_ES_APPENDMODE & style)/* || (TEXU_ES_PASSWORD & style)*/) /* replace mode */
            {
                texu_env_show_cursor(env, TEXU_FALSE);
                edit->editbuf[0] = 0;
                texu_memset(buf, TEXUTEXT(' '), width);
                buf[width] = 0;
#ifdef TEXU_CIO_COLOR_MONO
                texu_cio_putstr_attr(dc, y, x, buf,
                                     texu_cio_get_reverse(dc, normcolor));
#else

                texu_cio_draw_text(dc, y, x, buf, normcolor, normbg,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));

#endif
                texu_cio_gotoyx(dc, y, x);
                texu_env_set_cursor(env, y, x);
                texu_env_show_cursor(env, TEXU_TRUE);
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
            /* append a new char */
            len = texu_strlen(edit->editbuf);
            if (len < edit->limitchars)
            {
                texu_env_show_cursor(env, TEXU_FALSE);

                /*no upper or lower style*/
                if (0 == ((TEXU_ES_UPPERCASE | TEXU_ES_LOWERCASE) & style))
                {
#if (defined WIN32 && defined _WINDOWS) /*because windows always sends a capital letter to us*/
                    if (alt & TEXU_KEYPRESSED_SHIFT)
                    {
                        edit->editbuf[len] = ch;
                    }
                    else
                    {
                        /*not pressed shift key*/
                        if (ch >= TEXUTEXT('A') && ch <= TEXUTEXT('Z'))
                        {
                            edit->editbuf[len] = ch - TEXUTEXT('A') + TEXUTEXT('a');
                        }
                        else
                        {
                            edit->editbuf[len] = ch;
                        }
                    }
#else
                    edit->editbuf[len] = ch;
#endif
                }
                else
                {
                    edit->editbuf[len] = ch;
                }
                edit->editbuf[len + 1] = 0;
                edit->selected = 0;

                if ((TEXU_ES_NUMBER & style) && edit->onminmax)
                {
                    texu_i32 val = 0;
                    val = texu_atol(edit->editbuf);
                    if (_TexuEditProc_ValidateMinMax(edit, val) != TEXU_OK)
                    {
                        val = _TexuEditProc_SelMinMax(edit->min, edit->max, val);

                        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
                        texu_strcpy(edit->editbuf, buf);
                        len = texu_strlen(edit->editbuf);
                    }
                }

                if (len < width)
                {
                    /*texu_env_show_cursor(env, TEXU_FALSE);*/
                    if (TEXU_ES_PASSWORD & style)
                    {
                        if (edit->showpass == TEXU_TRUE)
                        {
#if (defined WIN32 && defined _WINDOWS)
                            texu_env_draw_char_ex(env, y, x + len, edit->passchar, selcolor, selbg,
                                                  texu_wnd_get_clsname(wnd),
                                                  texu_wnd_get_id(wnd));

#else
                            texu_cio_putch_attr(dc, y, x + len,
                                                edit->passchar,
                                                texu_cio_get_color(dc, selcolor));
#endif
                        }
                    }
                    else
                    {
#if (defined WIN32 && defined _WINDOWS)
                        texu_env_draw_char_ex(env, y, x + len, ch, selcolor, selbg,
                                              texu_wnd_get_clsname(wnd),
                                              texu_wnd_get_id(wnd));

#else
                        texu_cio_putch_attr(dc, y, x + len,
                                            ch,
                                            texu_cio_get_color(dc, selcolor));
#endif
                    }
                    changed = 1;
                    texu_cio_gotoyx(dc, y, x + len);
                    /* editing */
                    edit->editing = 1;
                    texu_wnd_invalidate(wnd);

                    len = TEXU_MIN(len + 1, width);
                    texu_env_set_cursor(env, y, x + len);
                    texu_env_show_cursor(env, TEXU_TRUE);
                    return;
                }
                changed = 1;
            } /* len < limit */

            if (TEXU_ES_AUTOHSCROLL & style)
            {
                len = texu_strlen(edit->editbuf);
                if (len <= edit->limitchars && len > width && changed)
                {
                    if (len > width)
                    {
                        edit->firstchar = len - width;
                    }
                    /* editing */
                    edit->editing = 1;
                    /*texu_env_show_cursor(env, TEXU_FALSE);*/
                    texu_wnd_invalidate(wnd);

                    len = TEXU_MIN(len, width);
                    texu_env_set_cursor(env, y, x + len);
                    texu_env_show_cursor(env, TEXU_TRUE);
                    return;
                }
            }

            /* editing */
            edit->editing = 1;
        } /*TEXU_MAX_WNDTEXT*/
    }
    else if (0x7f == ch || TEXU_KEY_DC == ch || TEXU_KEY_BACKSPACE == ch) /* delete char */
    {
        edit->selected = 0;
        len = texu_strlen(edit->editbuf);
        if (edit->firstvisit == 1)
        {
            edit->firstvisit = 0;
            if (!(TEXU_ES_APPENDMODE & style)/* || TEXU_ES_PASSWORD & style*/) /* replace mode */
            {
                texu_env_show_cursor(env, TEXU_FALSE);
                edit->editbuf[0] = 0;
                texu_strcpy(buf, TEXUTEXT(" "));
                texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);

                texu_cio_draw_text(dc, y, x, text, selcolor, selbg,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));

                texu_cio_gotoyx(dc, y, x);
                texu_env_set_cursor(env, y, x);
                texu_env_show_cursor(env, TEXU_TRUE);
            }
            else if (TEXU_ES_AUTOHSCROLL & style)
            {
                /* show first char at */
                /*
                if (len > width)
                {
                    edit->firstchar = len - width - 1;
                    if (edit->firstchar < 0)
                    {
                        edit->firstchar = 0;
                    }
                }*/
            }
        }

        if (len > 0)
        {
            texu_env_show_cursor(env, TEXU_FALSE);
            edit->editbuf[len - 1] = 0;
            changed = TEXU_TRUE;

            len = texu_strlen(edit->editbuf); 
            if (TEXU_ES_AUTOHSCROLL & style)
            {
                /*len = texu_strlen(edit->editbuf);*/
                if (len >= width)
                {
                    edit->firstchar = len - width;/* -1;*/
                    if (edit->firstchar < 0)
                    {
                        edit->firstchar = 0;
                    }
                }
                len = TEXU_MIN(len, width);
                texu_cio_gotoyx(dc, y, x + len);

            }
            /* editing */
            edit->editing = 1;
            texu_wnd_invalidate(wnd);
            if (len >= 1)
            {
                x += len;
            }
            texu_env_set_cursor(env, y, x);
            texu_env_show_cursor(env, TEXU_TRUE);
            return;
        }

        /* editing */
        edit->editing = 1;
    } /*end delete char*/
}

texu_status
_TexuEditProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_ui32 style = attrs->style;
    texu_editwnd *edit = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    /* initial memory for edit control */
    edit = (texu_editwnd *)malloc(sizeof(texu_editwnd));
    if (!edit)
    {
        return TEXU_NOMEM;
    }

    memset(edit, 0, sizeof(texu_editwnd));
    texu_wnd_get_text(wnd, edit->editbuf, TEXU_MAX_WNDTEXT);

    edit->firstvisit = 1;
    edit->firstchar = 0;
    edit->passchar = TEXUTEXT('*');
    edit->showpass = TEXU_TRUE;
    edit->editing = 0;
    edit->decwidth = 6;
    edit->limitchars = (TEXU_ES_AUTOHSCROLL & style ? TEXU_MAX_WNDTEXT : attrs->width);
    edit->selected = 0;

    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED));
    texu_wnd_set_focused_color(wnd, texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED));
    texu_wnd_set_bgfocused_color(wnd, texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED));

    texu_wnd_set_userdata(wnd, edit);

    edit->on_validate = attrs->on_validate;

    return TEXU_OK;
}

void _TexuEditProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    TexuDefWndProc(wnd, TEXU_WM_ERASEBG, (texu_lparam)cio, (texu_lparam)rect);
}

void _TexuEditProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_editwnd *edit = 0;
    texu_i32 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED);
    texu_ui32 invcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_INVALID);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;
    texu_char textcommas[TEXU_MAX_WNDTEXT + 1];
    

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    texu_ui32 invbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_INVALID);
    texu_ui32 bgcolor = normbg;

    texu_i32 cx = texu_env_screen_width(env);
    /*texu_i32 cy = texu_env_screen_height(env);*/

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }
    edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);


    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);

    len = width;
    if (TEXU_ES_PASSWORD & style)
    {
        memset(buf, 0, sizeof(buf));
        if (edit->showpass == TEXU_TRUE)
        {
            len = texu_strlen(edit->editbuf);
            if (0 == len)
            {
                len = 8;
            }
            texu_memset(buf, edit->passchar, len);
        }
        else
        {
            texu_memset(buf, TEXUTEXT(' '), len);
        }
        buf[len] = 0;

        texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);
        texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
        bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);
        if (edit->selected)
        {
            color = selcolor;
            bgcolor = selbg;
        }
#if defined TEXU_CIO_COLOR_MONO
        texu_cio_putstr_attr(cio, y, x, text,
                             texu_cio_get_reverse(cio, color));
#else
        texu_cio_draw_text(cio, y, x, text, color, bgcolor,
                           texu_wnd_get_clsname(wnd),
                           texu_wnd_get_id(wnd));
#endif

        return;
    }

    /*len = TEXU_MIN(texu_strlen(edit->editbuf), width);*/
    memset(buf, 0, sizeof(buf));
    memset(text, 0, sizeof(text));
    texu_strncpy(buf, &edit->editbuf[edit->firstchar], width);
    if (0 == texu_strlen(buf))
    {
/*            memset(buf, TEXUTEXT(' '), sizeof(texu_char)*width);*/
        texu_strcpy(buf, TEXUTEXT(""));
    }

    if (edit->editing)
    {
        style = TEXU_ALIGN_LEFT;
    }

    if ((TEXU_ES_AUTOCOMMAS & style) &&
        ((TEXU_ES_NUMBER & style) || (TEXU_ES_DECIMAL & style)))
    {
        texu_add_commas(textcommas, TEXU_MAX_WNDTEXT, buf);
    }
    else
    {
        texu_strcpy(textcommas, buf);
    }
    texu_printf_alignment3(text, textcommas, width, style, TEXU_FALSE, x, cx);

    if (edit->selected)
    {
        color = selcolor;
        bgcolor = selbg;
        if (edit->invalid)
        {
            color = invcolor;
            bgcolor = invbg;
            edit->invalid = TEXU_FALSE;
        }
    }
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(cio, y, x, text,
                            texu_cio_get_reverse(cio, color));
#else
    texu_cio_draw_text(cio, y, x, text, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif /* TEXU_CIO_COLOR_MONO*/
    len = (texu_i32)TEXU_MIN(texu_strlen(buf), (texu_ui32)width);
    texu_cio_gotoyx(cio, y, x + len);
}

texu_status _TexuEditProc_OnGetInt(texu_wnd *wnd, texu_i32 *out);
texu_status _TexuEditProc_OnGetFloat(texu_wnd *wnd, texu_f64 *out);


texu_status _TexuEditProc_OnGetInt(texu_wnd *wnd, texu_i32 *out)
{
    texu_status rc = TEXU_OK;
    /*texu_editwnd *edit = 0;*/
    texu_i32 value = 0;
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    if (!(texu_wnd_is_enable(wnd)) || !out)
    {
        return TEXU_ERROR;
    }

    /*edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);*/
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);

    value = texu_atol(text);
    *out = value;

    return rc;
}

texu_status _TexuEditProc_OnGetFloat(texu_wnd *wnd, texu_f64 *out)
{
    texu_status rc = TEXU_OK;
    /*texu_editwnd *edit = 0;*/
    texu_f64 value = 0;
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    if (!(texu_wnd_is_enable(wnd)) || !out)
    {
        return TEXU_ERROR;
    }

    /*edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);*/
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);

    value = texu_atof(text);
    *out = value;

    return rc;
}


texu_longptr
_TexuEditProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            return _TexuEditProc_OnCreate(wnd, (texu_wnd_attrs *)param1);
        }

        case TEXU_WM_CHAR:
        {
            _TexuEditProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;
        }
#if 0
        case TEXU_WM_KEYDOWN:/*virtual key*/
        {
            texu_i32 rc = TexuDefWndProc(wnd, msg, param1, param2);
            if (0 == rc)
            {
                rc = _TexuEditProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            }
            return rc;
        }
#endif
        case TEXU_WM_DESTROY:
        {
            _TexuEditProc_OnDestroy(wnd);
            break;
        }

        case TEXU_WM_PAINT:
        {
            _TexuEditProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
        }

        case TEXU_WM_ERASEBG:
        {
            _TexuEditProc_OnEraseBg(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
        }

        case TEXU_WM_SETFOCUS:
        {
            _TexuEditProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;
        }

        case TEXU_WM_KILLFOCUS:
        {
            return _TexuEditProc_OnKillFocus(wnd, (texu_wnd *)param1);
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
            _TexuEditProc_OnSetValidString(wnd, (const texu_char *)param2);
            return 0;
        }

        case TEXU_EM_SETVALIDMINMAX:
        {
            _TexuEditProc_OnSetValidMinMax(wnd, (texu_i32)param1, (texu_editminmax *)param2);
            return 0;
        }

        case TEXU_WM_SETTEXT:
            _TexuEditProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EM_INVALIDATE:
            _TexuEditProc_OnInvalidate(wnd);
            return 0;

        case TEXU_EM_GETINT:
            return _TexuEditProc_OnGetInt(wnd, (texu_i32*)param1);

        case TEXU_EM_GETFLOAT:
            return _TexuEditProc_OnGetFloat(wnd, (texu_f64*)param1);
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
    texu_char itemtext[TEXU_MAX_WNDTEXT + 1];
    texu_longptr data;
    texu_i32 checked;
    texu_i32 enable;
    texu_i32 visible;
    texu_ui32 normcolor;
    texu_ui32 discolor;
    texu_ui32 selcolor;

    texu_ui32 normbg;
    texu_ui32 disbg;
    texu_ui32 selbg;

    void *userdata;
    struct texu_lbwnd_item *prev;
    struct texu_lbwnd_item *next;
};
typedef struct texu_lbwnd_item texu_lbwnd_item;

struct texu_lbwnd
{
    texu_i32 firstvisible;
    texu_i32 nitems; /* item counts */
    texu_i32 cursel; /* highlight item */

    texu_lbwnd_item *firstitem; /* always be item id = 0 if it is existing */
    texu_lbwnd_item *lastitem;

    texu_lbwnd_item *selitem; /* must be the same cursel item */
    texu_lbwnd_item *firstvisibleitem;

    texu_i32 checkeditems;            /* count item checked */
    texu_lbwnd_item *lastitemchecked; /* to identify the last item checked */

    void *exparam;

    /*texu_ui32 selcolor;
    texu_ui32 selbg;*/

    texu_bool       focused;
    texu_i32        xoffset;
    texu_i32        length_max;
};
typedef struct texu_lbwnd texu_lbwnd;

void _TexuListBoxProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuListBoxProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuListBoxProc_OnDestroy(texu_wnd *wnd);
void _TexuListBoxProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuListBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd);
void _TexuListBoxProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
texu_i32 _TexuListBoxProc_OnAddItem(texu_wnd *wnd, const texu_char *text);
void _TexuListBoxProc_OnSelChanged(texu_wnd *wnd);
texu_i32 _TexuListBoxProc_OnCountItemCheck(texu_wnd *wnd);
texu_i32 _TexuListBoxProc_OnGetItemChecked(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuListBoxProc_OnSetItemChecked(texu_wnd *wnd, texu_i32 idx, texu_i32 check);
texu_longptr _TexuListBoxProc_OnGetItemData(texu_wnd *wnd, texu_i32 idx);
void _TexuListBoxProc_OnSetItemData(texu_wnd *wnd, texu_i32 idx, texu_longptr data);
void _TexuListBoxProc_OnSetCurSel(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuListBoxProc_OnGetItemCount(texu_wnd *wnd);
void _TexuListBoxProc_OnSetItemText(texu_wnd *wnd, texu_i32 idx, const texu_char *text);
texu_i32 _TexuListBoxProc_OnGetItemText(texu_wnd *wnd, texu_i32 idx, texu_char *text);
texu_i32 _TexuListBoxProc_OnGetCurSel(texu_wnd *wnd);
void _TexuListBoxProc_OnDeleteAllItems(texu_wnd *wnd);
void _TexuListBoxProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);
void _TexuListBoxProc_OnKeyDown(texu_wnd *, texu_i32 ch, texu_i32 alt);
void _TexuListBoxProc_OnEnableItem(texu_wnd *wnd, texu_i32 idx, texu_i32 enable);
#if (defined WIN32 && defined _WINDOWS)
texu_i32 _TexuListBoxProc_OnSetSelColor(texu_wnd *wnd, texu_ui32 color, texu_ui32 bgcolor);
#else
texu_i32 _TexuListBoxProc_OnSetSelColor(texu_wnd *wnd, texu_ui32 color);
#endif
texu_i32    _TexuListBoxProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuListBoxProc_OnSetText(texu_wnd *wnd, const texu_char *text);

texu_i32 _TexuListBoxProc_FindIndexByItem(texu_lbwnd *lb, texu_lbwnd_item *item);
texu_lbwnd_item *_TexuListBoxProc_FindItemByIndex(texu_lbwnd *lb, texu_i32 idx);
void _TexuListBoxProc_Notify(texu_wnd *, texu_ui32, texu_i32);
texu_lbwnd_item *_TexuListBoxProc_GetFirstItemEnabled(texu_lbwnd *lb);
texu_lbwnd_item *_TexuListBoxProc_GetNextItemEnabled(texu_lbwnd *lb, texu_lbwnd_item *item);
texu_lbwnd_item *_TexuListBoxProc_GetLastItemEnabled(texu_lbwnd *lb);
texu_lbwnd_item *_TexuListBoxProc_GetPrevItemEnabled(texu_lbwnd *lb, texu_lbwnd_item *item);

texu_i32
_TexuListBoxProc_FindIndexByItem(texu_lbwnd *lb, texu_lbwnd_item *item)
{
    texu_i32 idx = 0;
    texu_lbwnd_item *chkitem = lb->firstitem;
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
#if (defined WIN32 && defined _WINDOWS)
texu_i32 _TexuListBoxProc_OnSetSelColor(texu_wnd *wnd, texu_ui32 color, texu_ui32 bgcolor)
{
    /*texu_lbwnd *lb = texu_wnd_get_userdata(wnd);*/
    texu_ui32 oldcolor = texu_wnd_get_selected_color(wnd);/*lb->selcolor;*/
    /*lb->selcolor = color;
    lb->selbg = bgcolor;*/
    texu_wnd_set_selected_color(wnd, color);
    texu_wnd_set_bgselected_color(wnd, bgcolor);
    return oldcolor;
}
#else
texu_i32 _TexuListBoxProc_OnSetSelColor(texu_wnd *wnd, texu_ui32 color)
{
    /*texu_lbwnd *lb = texu_wnd_get_userdata(wnd);*/
    texu_ui32 oldcolor = texu_wnd_get_selected_color(wnd);/*lb->selcolor;*/
    /*lb->selcolor = color;*/
    return oldcolor;
}
#endif


texu_lbwnd_item *
_TexuListBoxProc_GetFirstItemEnabled(texu_lbwnd *lb)
{
    texu_lbwnd_item *item = lb->firstitem;
    if (item && item->enable)
    {
        return item;
    }
    return _TexuListBoxProc_GetNextItemEnabled(lb, item);
}

texu_lbwnd_item *
_TexuListBoxProc_GetNextItemEnabled(texu_lbwnd *lb, texu_lbwnd_item *item)
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

texu_lbwnd_item *
_TexuListBoxProc_GetLastItemEnabled(texu_lbwnd *lb)
{
    texu_lbwnd_item *item = lb->lastitem;
    if (item && item->enable)
    {
        return item;
    }
    return _TexuListBoxProc_GetPrevItemEnabled(lb, item);
}

texu_lbwnd_item *
_TexuListBoxProc_GetPrevItemEnabled(texu_lbwnd *lb, texu_lbwnd_item *item)
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
_TexuListBoxProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_i32 repaint = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 lines = 0;
    texu_i32 height = texu_wnd_get_height(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_lbwnd_item *item = 0;
    texu_i32 cursel = lb->cursel;
    texu_i32 margin = 1;
    texu_i32 nitems = lb->nitems;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (nitems <= 0)
    {
        return;
    }
    item = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
    
    if ((style & TEXU_LBS_CHECKBOX) || (style & TEXU_LBS_RADIOBOX))
    {
        margin = 5;
    }

    switch (ch)
    {
        case 0x20: /*space bar*/
        {
            if (!item->enable)
            {
                return;
            }
            if ((style & TEXU_LBS_CHECKBOX) ||
                 (style & TEXU_LBS_RADIOBOX))
            {
                /* get current and check it */
                _TexuListBoxProc_OnSetItemChecked(wnd, lb->cursel, TEXU_LB_CHECKED);
                ++repaint;
            }
            break;
        }

        case TEXU_KEY_DOWN:
        {
            if (cursel < nitems - 1)
            {
                ++lines;
                ++repaint;
            }
            break;
        }

        case TEXU_KEY_UP:
        {
            if (cursel > 0)
            {
                --lines;
                ++repaint;
            }
            break;
        }
        
        case TEXU_KEY_LEFT:
            if (lb->xoffset > 0)
            {
                --lb->xoffset;
                ++repaint;
            }
            break;
        case TEXU_KEY_RIGHT:
            if (width + lb->xoffset <= lb->length_max + margin)
            {
                ++lb->xoffset;
                ++repaint;
            }
            break;

        case TEXU_KEY_PPAGE:
        {
            if (cursel > 0)
            {
                lines -= height;
                ++repaint;
            }
            break;
        }

        case TEXU_KEY_NPAGE:
        {
            if (cursel < nitems - 2)
            {
                lines += height;
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

        /* find the new first visible */
        if (height > 1)
        {
            lb->firstvisible = cursel - height + 1;
            if (lb->firstvisible < 0)
            {
                lb->firstvisible = 0;
            }
        }
        else
        {
            lb->firstvisible = lb->cursel;
        }

        /* send notification */
        _TexuListBoxProc_OnSelChanged(wnd);
        /* redraw after changing */
        texu_wnd_invalidate(wnd);
    }
    lb->selitem = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
    lb->firstvisibleitem = _TexuListBoxProc_FindItemByIndex(lb, lb->firstvisible);
}

void _TexuListBoxProc_OnDeleteAllItems(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_i32 nitems = 0;
    texu_i32 i = 0;

    if (!lb)
    {
        return;
    }
    nitems = lb->nitems;
    for (i = 0; i < nitems; ++i)
    {
        _TexuListBoxProc_OnDeleteItem(wnd, 0);
    }
}

texu_i32 _TexuListBoxProc_OnGetCurSel(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    return lb->cursel;
}

texu_i32 _TexuListBoxProc_OnGetItemText(texu_wnd *wnd, texu_i32 idx, texu_char *text)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);

    if (item)
    {
        texu_strcpy(text, item->itemtext);
    }
    return texu_strlen(text);
}

void _TexuListBoxProc_OnSetItemText(texu_wnd *wnd, texu_i32 idx, const texu_char *text)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);
    if (item)
    {
        texu_strcpy(item->itemtext, text);
        texu_wnd_invalidate(wnd);
    }
}

texu_i32 _TexuListBoxProc_OnGetItemCount(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    return lb->nitems;
}

void _TexuListBoxProc_OnSetCurSel(texu_wnd *wnd, texu_i32 idx)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
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

void _TexuListBoxProc_OnSetItemData(texu_wnd *wnd, texu_i32 idx, texu_longptr data)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);
    if (item)
    {
        item->data = data;
    }
}

texu_longptr _TexuListBoxProc_OnGetItemData(texu_wnd *wnd, texu_i32 idx)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);
    if (item)
    {
        return item->data;
    }
    return 0;
}

void
_TexuListBoxProc_OnEnableItem(texu_wnd *wnd, texu_i32 idx, texu_i32 enable)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    item = _TexuListBoxProc_FindItemByIndex(lb, idx);
    {
        if (item->enable != enable)
        {
            texu_wnd_invalidate(wnd);
        }
        item->enable = enable;
    }
}

texu_i32 _TexuListBoxProc_OnSetItemChecked(texu_wnd *wnd, texu_i32 idx, texu_i32 check)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
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
                lb->checkeditems = 0;
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
                        lb->checkeditems = 0;
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

texu_i32 _TexuListBoxProc_OnGetItemChecked(texu_wnd *wnd, texu_i32 idx)
{
    texu_lbwnd_item *item = 0;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);
    if (item)
    {
        return item->checked;
    }
    return TEXU_LB_ERROR;
}

texu_i32 _TexuListBoxProc_OnCountItemCheck(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    return lb->checkeditems;
}

void
_TexuListBoxProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_i32 index)
{
    texu_lbwnd_notify notify;
    texu_wnd *parent = texu_wnd_get_parent(wnd);

    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.index = index;
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}

texu_lbwnd_item *
_TexuListBoxProc_FindItemByIndex(texu_lbwnd *lb, texu_i32 idx)
{
    texu_i32 i = 0;
    texu_lbwnd_item *item = 0;

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
_TexuListBoxProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    /*texu_lbwnd_item *item = 0;*/
    /*texu_env *env = texu_wnd_get_env(wnd);*/
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (lb->cursel >= lb->nitems)
    {
        lb->cursel = lb->nitems - 1;
    }
    else if (lb->cursel < 0)
    {
        lb->cursel = 0;
    }
    /*
    item = _TexuListBoxProc_FindItemByIndex(lb, lb->cursel);
    if (item && !(item->enable))
    {
        item = _TexuListBoxProc_GetNextItemEnabled(lb, item);
        if (item)
        {
            lb->cursel = _TexuListBoxProc_FindIndexByItem(lb, item);
        }
    }*/
    lb->focused = TEXU_TRUE;

    texu_wnd_invalidate(wnd);
    /* send notification */
    _TexuListBoxProc_Notify(wnd, TEXU_LBN_SETFOCUS, lb->cursel);
}

texu_i32
_TexuListBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
{
    texu_i32 rc = TEXU_OK;
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    /*texu_env *env = texu_wnd_get_env(wnd);*/

    /* send notification */
    _TexuListBoxProc_Notify(wnd, TEXU_LBN_KILLFOCUS, lb->cursel);

    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    lb->focused = TEXU_FALSE;
    texu_wnd_invalidate(wnd);
    return rc;
}

void _TexuListBoxProc_OnSelChanged(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    _TexuListBoxProc_Notify(wnd, TEXU_LBN_SELCHANGED, lb->cursel);
}

void
_TexuListBoxProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_i32 height = texu_wnd_get_height(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_DISABLED);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_SELECTED);
    texu_ui32 focuscolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_FOCUSED);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_bool enable = texu_wnd_is_enable(wnd);
    texu_ui32 color = normcolor;
    texu_i32 i = 0;
    texu_i32 lines = 0;
    texu_lbwnd_item *item = 0;
    texu_i32  movey = y;
    texu_i32  movex = x;
    /*texu_wnd *parent = texu_wnd_get_parent(wnd);*/
    /*texu_bool fFocused = lb->focused;*/

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_DISABLED);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_SELECTED);
    texu_ui32 focusbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_FOCUSED);
    texu_ui32 bgcolor = normbg;

    texu_i32 cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!(texu_wnd_is_visible(wnd)))
    {
        return;
    }

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    focuscolor = texu_wnd_get_focused_color(wnd);
    focusbg = texu_wnd_get_bgfocused_color(wnd);
    selcolor = texu_wnd_get_selected_color(wnd);
    selbg = texu_wnd_get_bgselected_color(wnd);

    color = (enable ? normcolor : discolor);
    bgcolor = (enable ? normbg : disbg);

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
                if (!item->enable)
                {
                    texu_strcat(buf, TEXUTEXT("*"));
                }
                else
                {
                    texu_strcpy(buf, TEXUTEXT(" "));
                }
                if (style & TEXU_LBS_CHECKBOX)
                {
                    if (item->checked && item->enable)
                    {
                        texu_strcat(buf, TEXUTEXT("[X] "));
                    }
                    else
                    {
                        texu_strcat(buf, TEXUTEXT("[ ] "));
                    }
                }
                else if (style & TEXU_LBS_RADIOBOX)
                {
                    if (item->checked && item->enable)
                    {
                        texu_strcat(buf, TEXUTEXT("(O) "));
                    }
                    else
                    {
                        texu_strcat(buf, TEXUTEXT("( ) "));
                    }
                }

                /* copy from item text */
                if (1)
                {
                    texu_i32 xoffset = lb->xoffset;
                    texu_strcat(buf, &item->itemtext[xoffset]);
                }
                else
                {
                    texu_strcat(buf, item->itemtext);
                }

                memset(text, 0, sizeof(text));
                texu_printf_alignment3(text, buf, width - 1, style, TEXU_FALSE, x, cx);
                texu_strcat(text, TEXUTEXT(" "));

                if (enable && i == lb->cursel)/* && item->enable)*/
                {
                    color = (enable && item->enable ? selbg : disbg);
                    bgcolor = (enable && item->enable ? selcolor : discolor);

                    color = (lb->focused ? color : focuscolor);
                    bgcolor = (lb->focused ? bgcolor : focusbg);

                    if (style & TEXU_LBS_OWNERCOLOR)
                    {
                        color = (item->enable ? item->selcolor : item->discolor);
                        bgcolor = (item->enable ? item->selbg : item->disbg);
                    }

#ifdef TEXU_CIO_COLOR_MONO
                texu_cio_putstr_attr(cio, y + (i - lb->firstvisible), x, text,
                                     texu_cio_get_reverse(cio, color));
#else

                    texu_cio_draw_text(cio, y + (i - lb->firstvisible), x, text, color, bgcolor,
                                          texu_wnd_get_clsname(wnd),
                                          texu_wnd_get_id(wnd));

#endif /* TEXU_CIO_COLOR_MONO*/
                    movey = y + (i - lb->firstvisible);
                    movex = x + width;
                }
                else/*not selected*/
                {
#ifdef TEXU_CIO_COLOR_MONO
                    color = normcolor;
#else
                    color = (enable && item->enable ? normcolor : discolor);
                    bgcolor = (enable && item->enable ? normbg : disbg);
                    if (style & TEXU_LBS_OWNERCOLOR)
                    {
                        color = (item->enable ? item->normcolor : item->discolor);
                        bgcolor = (item->enable ? item->normbg : item->disbg);
                    }
#endif /* TEXU_CIO_COLOR_MONO*/
                    texu_cio_draw_text(cio, y + (i - lb->firstvisible), x, text, color, bgcolor,
                                          texu_wnd_get_clsname(wnd),
                                          texu_wnd_get_id(wnd));

                }

            } /* not owner draw */
        }     /* for each item */
    }         /* items are valid */
    texu_cio_gotoyx(cio, movey, movex);
}

texu_status
_TexuListBoxProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_lbwnd *lb = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    /* initial memory for edit control */
    lb = (texu_lbwnd *)malloc(sizeof(texu_lbwnd));
    if (!lb)
    {
        return TEXU_NOMEM;
    }

    memset(lb, 0, sizeof(texu_lbwnd));

    lb->firstvisible = -1;
    lb->cursel = -1;
    lb->nitems = 0;
    lb->firstitem = 0;
    lb->lastitem = 0;
    lb->selitem = 0;
    lb->firstvisibleitem = 0;
    lb->checkeditems = 0;
    lb->lastitemchecked = 0;
    lb->xoffset = 0;
    lb->length_max = -1;
    /*lb->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_SELECTED);*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX),
                       texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_FOCUSED));
    texu_wnd_set_selected_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_SELECTED));

    /*lb->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_SELECTED);*/
    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                               texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_FOCUSED));

    texu_wnd_set_bgselected_color(wnd,
                                texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_SELECTED));

    texu_wnd_set_userdata(wnd, lb);

    return TEXU_OK;
}

void
_TexuListBoxProc_OnDestroy(texu_wnd *wnd)
{
    texu_lbwnd *lb = (texu_lbwnd *)texu_wnd_get_userdata(wnd);

    _TexuListBoxProc_OnDeleteAllItems(wnd);
    if (lb)
    {
        texu_wnd_set_userdata(wnd, 0);
        free(lb);
    }
}

texu_i32 _TexuListBoxProc_OnAddItem(texu_wnd *wnd, const texu_char *text)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_i32 len = 0;
    texu_lbwnd_item *item = 0;

    item = (texu_lbwnd_item *)malloc(sizeof(texu_lbwnd_item));
    if (item)
    {
        memset(item, 0, sizeof(texu_lbwnd_item));
        item->enable = TEXU_TRUE;
        item->visible = TEXU_TRUE;
        item->normcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX);
        item->discolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_DISABLED);
        item->selcolor = texu_env_get_syscolor(env, TEXU_COLOR_LISTBOX_SELECTED);

        item->normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX);
        item->disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_DISABLED);
        item->selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_SELECTED);

        len = texu_strlen(text);
        if (len > TEXU_MAX_WNDTEXT)
        {
            len = TEXU_MAX_WNDTEXT;
        }
        texu_strncpy(item->itemtext, text, len);

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
        /*find the length max*/
        if (len > lb->length_max)
        {
            lb->length_max = len;
        }
        return lb->nitems;
    }
    return TEXU_NOMEM;
}

void _TexuListBoxProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_lbwnd_item *item = 0;
    texu_lbwnd_item *nextitem = 0;
    texu_lbwnd_item *previtem = 0;
    texu_i32 height = texu_wnd_get_height(wnd);

    item = _TexuListBoxProc_FindItemByIndex(lb, idx);

    if (item)
    {
        /* notify to parent */
        _TexuListBoxProc_Notify(wnd, TEXU_LBN_DELETEITEM, idx);

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
        if (idx >= lb->firstvisible && idx <= lb->firstvisible + height)
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

texu_i32
_TexuListBoxProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);

    texu_i32 sel = TEXU_MAX(lb->cursel, 0);
    texu_i32 len = 0;
    
    if (sel > -1)
    {
        _TexuListBoxProc_OnGetItemText(wnd, sel, (texu_char *)text);
    }
    else if (lb->nitems <= 0)
    {
        texu_strcpy(text, TEXUTEXT(""));
    }
    len = texu_strlen(text);
    if (len > 0 && textlen < len)
    {
        text[textlen-1] = 0;
    }
    return texu_strlen(text);
}

void
_TexuListBoxProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    texu_i32 sel = TEXU_MAX(lb->cursel, 0);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    _TexuListBoxProc_OnSetItemText(wnd, sel, text);
}

texu_longptr
_TexuListBoxProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_KEYDOWN:
            _TexuListBoxProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuListBoxProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuListBoxProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuListBoxProc_OnDestroy(wnd);
            break;

        case TEXU_WM_SETFOCUS:
            _TexuListBoxProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuListBoxProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_GETTEXT:
            return _TexuListBoxProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

        case TEXU_WM_SETTEXT:
            _TexuListBoxProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_LBM_DELETEITEM:
        {
            _TexuListBoxProc_OnDeleteItem(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_LBM_ADDITEM:
        {
            return _TexuListBoxProc_OnAddItem(wnd, (const texu_char *)param1);
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
            _TexuListBoxProc_OnSetItemData(wnd, (texu_i32)param1, (texu_lparam)param2);
            return 0;
        }
        case TEXU_LBM_GETITEMDATA:
        {
            return (texu_lparam)_TexuListBoxProc_OnGetItemData(wnd, (texu_i32)param1);
        }
        case TEXU_LBM_SETITEMTEXT:
        {
            _TexuListBoxProc_OnSetItemText(wnd, (texu_i32)param1, (const texu_char *)param2);
            return 0;
        }
        case TEXU_LBM_GETITEMTEXT:
        {
            return _TexuListBoxProc_OnGetItemText(wnd, (texu_i32)param1, (texu_char *)param2);
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
        case TEXU_LBM_SETSELCOLOR:
        {
#if (defined WIN32 && defined _WINDOWS)
            return _TexuListBoxProc_OnSetSelColor(wnd, (texu_ui32)param1, (texu_ui32)param2);
#else
            return _TexuListBoxProc_OnSetSelColor(wnd, (texu_ui32)param1);
#endif
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU combobox
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_cbwnd
{
    texu_wnd    *lb;
    texu_bool   focused;
    void *exparam;
};
typedef struct texu_cbwnd texu_cbwnd;

void _TexuComboBoxProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuComboBoxProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuComboBoxProc_OnDestroy(texu_wnd *wnd);
texu_i32 _TexuComboBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd);
void _TexuComboBoxProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
texu_i32 _TexuComboBoxProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);
texu_i32 _TexuComboBoxProc_OnKeyDown(texu_wnd *, texu_i32 ch, texu_i32 alt);
void _TexuComboBoxProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuComboBoxProc_OnAddItem(texu_wnd *wnd, const texu_char *text);
texu_i32 _TexuComboBoxProc_OnCountItemCheck(texu_wnd *wnd);
texu_i32 _TexuComboBoxProc_OnGetItemChecked(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuComboBoxProc_OnSetItemChecked(texu_wnd *wnd, texu_i32 idx, texu_i32 check);
texu_longptr _TexuComboBoxProc_OnGetItemData(texu_wnd *wnd, texu_i32 idx);
void _TexuComboBoxProc_OnSetItemData(texu_wnd *wnd, texu_i32 idx, texu_longptr data);
texu_i32 _TexuComboBoxProc_OnGetItemCount(texu_wnd *wnd);
void _TexuComboBoxProc_OnSetItemText(texu_wnd *wnd, texu_i32 idx, const texu_char *text);
texu_i32 _TexuComboBoxProc_OnGetItemText(texu_wnd *wnd, texu_i32 idx, texu_char *text);
texu_i32 _TexuComboBoxProc_OnGetCurSel(texu_wnd *wnd);
void _TexuComboBoxProc_OnSetCurSel(texu_wnd *wnd, texu_i32 idx);
void _TexuComboBoxProc_OnDeleteAllItems(texu_wnd *wnd);
void _TexuComboBoxProc_OnEnableItem(texu_wnd *wnd, texu_i32 idx, texu_i32 enable);
texu_wnd *_TexuComboBoxProc_OnGetListBox(texu_wnd *wnd);
texu_i32    _TexuComboBoxProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuComboBoxProc_OnSetText(texu_wnd *wnd, const texu_char *text);

void _TexuComboBoxProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_i32 index);

texu_wnd *
_TexuComboBoxProc_OnGetListBox(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return cb->lb;
}

void
_TexuComboBoxProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_i32 index)
{
    texu_cbwnd_notify notify;
    texu_wnd *parent = texu_wnd_get_parent(wnd);

    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.index = index;
    texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}

texu_i32
_TexuComboBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 height = 1;
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMTEXT, cursel, (texu_lparam)text);
    texu_wnd_set_text(wnd, text);

    texu_wnd_move(cb->lb, y, x, height, width, TEXU_TRUE);
    texu_wnd_visible(cb->lb, TEXU_FALSE);
    _TexuComboBoxProc_Notify(wnd, TEXU_CBN_KILLFOCUS, cursel);
    /*show '+'*/
    cb->focused = TEXU_FALSE;

    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    texu_wnd_invalidate(texu_wnd_get_parent(wnd));
    return TEXU_OK;
}

void
_TexuComboBoxProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    texu_i32 nitems = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMCOUNT, 0, 0);
    texu_i32 height = TEXU_MIN(5, nitems);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }

    texu_wnd_move(cb->lb, y + 1, x, height, width, TEXU_TRUE);
    texu_wnd_send_msg(cb->lb, TEXU_WM_SETFOCUS, 0, 0);
/*    texu_wnd_visible(cb->lb, TEXU_TRUE);*/
    _TexuComboBoxProc_Notify(wnd, TEXU_CBN_SETFOCUS, cursel);
    /*show '-'*/
    cb->focused = TEXU_TRUE;

    texu_wnd_invalidate(wnd);
}

texu_i32
_TexuComboBoxProc_OnCountItemCheck(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_COUNTITEMCHECKED, 0, 0);
}

texu_i32
_TexuComboBoxProc_OnGetItemChecked(texu_wnd *wnd, texu_i32 idx)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMCHECKED, idx, 0);
}

texu_i32
_TexuComboBoxProc_OnSetItemChecked(texu_wnd *wnd, texu_i32 idx, texu_i32 check)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_SETITEMCHECKED, idx, check);
}

void
_TexuComboBoxProc_OnSetCurSel(texu_wnd *wnd, texu_i32 idx)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_SETCURSEL, idx, 0);
}

texu_i32
_TexuComboBoxProc_OnGetCurSel(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
}

texu_i32
_TexuComboBoxProc_OnGetItemCount(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMCOUNT, 0, 0);
}

texu_i32
_TexuComboBoxProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_WM_GETTEXT, (texu_lparam)text, (texu_lparam)textlen);
}

void
_TexuComboBoxProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    texu_wnd_send_msg(cb->lb, TEXU_WM_SETTEXT, (texu_lparam)text, (texu_lparam)-1);
}

void
_TexuComboBoxProc_OnSetItemText(texu_wnd *wnd, texu_i32 idx, const texu_char *text)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_SETITEMTEXT, idx, (texu_lparam)text);
}

texu_i32
_TexuComboBoxProc_OnGetItemText(texu_wnd *wnd, texu_i32 idx, texu_char *text)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMTEXT, idx, (texu_lparam)text);
}

void
_TexuComboBoxProc_OnDeleteAllItems(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_DELETEALLITEMS, 0, 0);
}

void
_TexuComboBoxProc_OnEnableItem(texu_wnd *wnd, texu_i32 idx, texu_i32 enable)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_ENABLEITEM, idx, enable);
}

texu_longptr
_TexuComboBoxProc_OnGetItemData(texu_wnd *wnd, texu_i32 idx)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMDATA, idx, 0);
}

void
_TexuComboBoxProc_OnSetItemData(texu_wnd *wnd, texu_i32 idx, texu_longptr data)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_SETITEMDATA, idx, data);
}

void
_TexuComboBoxProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_send_msg(cb->lb, TEXU_LBM_DELETEITEM, idx, 0);
}

texu_i32
_TexuComboBoxProc_OnAddItem(texu_wnd *wnd, const texu_char *text)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    return texu_wnd_send_msg(cb->lb, TEXU_LBM_ADDITEM, (texu_lparam)text, 0);
}

texu_i32
_TexuComboBoxProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    switch (ch)
    {
        case TEXU_KEY_UP:
        case TEXU_KEY_DOWN:
        case TEXU_KEY_PPAGE:
        case TEXU_KEY_NPAGE:
        {
            texu_i32 idx = -1;
            texu_char text[TEXU_MAX_WNDTEXT + 1];
            texu_wnd_send_msg(cb->lb, TEXU_WM_KEYDOWN, ch, alt);
            /*texu_wnd_invalidate(cb->lb);*/
            /*update combobox text*/
            idx = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
            texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMTEXT, (texu_lparam)idx, (texu_lparam)text);
            texu_wnd_set_text(wnd, text);
            texu_wnd_invalidate(wnd);
        }
        break;
#if 0
        case ' ':
        {
            if (texu_wnd_is_visible(cb->lb))
            {
                texu_wnd_visible(cb->lb, TEXU_FALSE);
            }
            else
            {
                texu_wnd_visible(cb->lb, TEXU_TRUE);
                texu_wnd_invalidate(texu_wnd_get_parent(cb));
            }
            texu_wnd_set_focus(wnd);
            /*texu_wnd_invalidate(wnd);*/
        }
#endif
        break;
    }
    return 0;

}
texu_i32
_TexuComboBoxProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    switch (ch)
    {
        case ' ':
        {
            if (texu_wnd_is_visible(cb->lb))
            {
                texu_wnd_visible(cb->lb, TEXU_FALSE);
                texu_wnd_set_focus(wnd);
            }
            else
            {
                texu_wnd_visible(cb->lb, TEXU_TRUE);
                texu_wnd_set_focus(wnd);
            }
            texu_wnd_invalidate(cb->lb);
        }
        break;
    }
    return 0;
}

void
_TexuComboBoxProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX);
    texu_ui32 sel = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX_SELECTED);

    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX_SELECTED);

    texu_char expanding = (texu_wnd_is_visible(cb->lb) ? TEXUTEXT('-') : TEXUTEXT('+'));

    texu_i32 cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!(texu_wnd_is_visible(wnd)))
    {
        return;
    }
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_printf_alignment3(buf, text, width, TEXU_ALIGN_LEFT, TEXU_TRUE, x, cx);
    if (texu_wnd_get_activechild(parent) == wnd)
    {
        color = sel;
        bgcolor = selbg;
    }

    texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
    texu_cio_draw_char(dc, y, x + width - 1, expanding, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

texu_status
_TexuComboBoxProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_cbwnd *cb = 0;
    texu_wnd *lb = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd_attrs attrs2;
    texu_status rc = TEXU_OK;
    texu_wnd* parent = texu_wnd_get_parent(wnd);
    texu_i32 yparent = texu_wnd_get_y(parent);
    texu_i32 xparent = texu_wnd_get_x(parent);
    texu_i32 hparent = texu_wnd_get_height(parent);
    texu_i32 wparent = texu_wnd_get_width(parent);

    lb = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!lb)
    {
        return TEXU_NOMEM;
    }
    attrs->height = 5; /* set to 5 */

    memset(&attrs2, 0, sizeof(attrs2));
    /*check y-position*/
    if (attrs->y + 1 > yparent + hparent)
    {
        attrs2.y = attrs->y - attrs->height - 1; /*default display 5 items*/
    }
    else
    {
        attrs2.y = attrs->y + 1;
    }
    attrs2.x                = attrs->x;
    attrs2.height           = attrs->height;
    attrs2.width            = attrs->width;
    attrs2.enable           = TEXU_TRUE;
    attrs2.visible          = TEXU_FALSE;
    attrs2.text             = attrs->text;
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_COMBOBOX_FOCUSED);

    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX_DISABLED);
    attrs2.selectedbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_COMBOBOX_FOCUSED);

    attrs2.id = texu_env_get_wnd_id(env);
    attrs2.clsname = TEXU_LISTBOX_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_WS_POPUP;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(lb, parent, &attrs2);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(lb);
        return TEXU_ERROR;
    }

    texu_wnd_set_color(lb, attrs2.normalcolor, attrs2.disabledcolor);
    texu_wnd_set_bgcolor(lb, attrs2.normalbg, attrs2.disabledbg);
    texu_wnd_set_focused_color(lb, attrs2.focusedcolor);
    texu_wnd_set_bgfocused_color(lb, attrs2.focusedbg);
    texu_wnd_set_selected_color(lb, attrs2.selectedcolor);
    texu_wnd_set_bgselected_color(lb, attrs2.selectedbg);

    cb = (texu_cbwnd *)malloc(sizeof(texu_cbwnd));
    if (!cb)
    {
        texu_wnd_del(lb);
        return TEXU_NOMEM;
    }

    memset(cb, 0, sizeof(texu_cbwnd));
    cb->lb = lb;

    texu_wnd_set_userdata(wnd, cb);

    return TEXU_OK;
}

void
_TexuComboBoxProc_OnDestroy(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_del(cb->lb);
    free(cb);
}

texu_longptr
_TexuComboBoxProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_KEYDOWN:
            return _TexuComboBoxProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);

        case TEXU_WM_CHAR:
            _TexuComboBoxProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuComboBoxProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_DESTROY:
            _TexuComboBoxProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuComboBoxProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            break;

        case TEXU_WM_SETFOCUS:
            _TexuComboBoxProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuComboBoxProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_GETTEXT:
            return _TexuComboBoxProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

        case TEXU_WM_SETTEXT:
            _TexuComboBoxProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_CBM_DELETEITEM:
        {
            _TexuComboBoxProc_OnDeleteItem(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_CBM_ADDITEM:
        {
            return _TexuComboBoxProc_OnAddItem(wnd, (const texu_char *)param1);
        }
        case TEXU_CBM_GETCURSEL:
        {
            return _TexuComboBoxProc_OnGetCurSel(wnd);
        }
        case TEXU_CBM_SETCURSEL:
        {
            _TexuComboBoxProc_OnSetCurSel(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_CBM_DELETEALLITEMS:
        {
            _TexuComboBoxProc_OnDeleteAllItems(wnd);
            return 0;
        }
        case TEXU_CBM_GETITEMCOUNT:
        {
            return _TexuComboBoxProc_OnGetItemCount(wnd);
        }
        case TEXU_CBM_SETITEMDATA:
        {
            _TexuComboBoxProc_OnSetItemData(wnd, (texu_i32)param1, (texu_lparam)param2);
            return 0;
        }
        case TEXU_CBM_GETITEMDATA:
        {
            return (texu_i32)_TexuComboBoxProc_OnGetItemData(wnd, (texu_i32)param1);
        }
        case TEXU_CBM_SETITEMTEXT:
        {
            _TexuComboBoxProc_OnSetItemText(wnd, (texu_i32)param1, (const texu_char *)param2);
            return 0;
        }
        case TEXU_CBM_GETITEMTEXT:
        {
            return _TexuComboBoxProc_OnGetItemText(wnd, (texu_i32)param1, (texu_char *)param2);
        }
        case TEXU_CBM_COUNTITEMCHECKED:
        {
            return _TexuComboBoxProc_OnCountItemCheck(wnd);
        }
        case TEXU_CBM_GETITEMCHECKED:
        {
            return _TexuComboBoxProc_OnGetItemChecked(wnd, (texu_i32)param1);
        }
        case TEXU_CBM_SETITEMCHECKED:
        {
            return _TexuComboBoxProc_OnSetItemChecked(wnd, (texu_i32)param1, (texu_i32)param2);
        }
        case TEXU_CBM_ENABLEITEM:
        {
            _TexuComboBoxProc_OnEnableItem(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;
        }
        case TEXU_CBM_GETLISTBOX:
        {
            return (texu_lparam)_TexuComboBoxProc_OnGetListBox(wnd);
        }
    }

    return TexuDefWndProc(wnd, msg, param1, param2);
}

/*
#-------------------------------------------------------------------------------
# TexU status bar
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_sbwnd_part
{
    texu_i32 id;
    texu_i32 width;
    texu_ui32 color;

    texu_ui32 bgcolor;

    texu_i32 align;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    void *userdata;
};
typedef struct texu_sbwnd_part texu_sbwnd_part;

struct texu_sbwnd
{
    texu_list *parts;
    texu_sbwnd_part *firstpart;
    void *exparam;
};
typedef struct texu_sbwnd texu_sbwnd;

void _TexuStatusBarProc_OnEraseBg(texu_wnd *, texu_cio *, texu_rect* rect);
void _TexuStatusBarProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuStatusBarProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuStatusBarProc_OnDestroy(texu_wnd *);
texu_i32 _TexuStatusBarProc_OnGetPartCount(texu_wnd *wnd);
texu_i32 _TexuStatusBarProc_OnAddPart(texu_wnd *, const texu_char *, texu_i32);
void _TexuStatusBarProc_OnSetText(texu_wnd *, texu_i32, const texu_char *);
const texu_char *_TexuStatusBarProc_OnGetText(texu_wnd *wnd, texu_i32 id);
void _TexuStatusBarProc_OnSetWidth(texu_wnd *, texu_i32, texu_i32);
texu_i32 _TexuStatusBarProc_OnGetWidth(texu_wnd *wnd, texu_i32 id);
void _TexuStatusBarProc_OnSetColor(texu_wnd *, texu_i32, texu_ui32);
void _TexuStatusBarProc_OnSetBgColor(texu_wnd *, texu_i32, texu_ui32);
texu_ui32 _TexuStatusBarProc_OnGetColor(texu_wnd *wnd, texu_i32 id);
texu_ui32 _TexuStatusBarProc_OnGetBgColor(texu_wnd *wnd, texu_i32 id);
void _TexuStatusBarProc_OnSetAlign(texu_wnd *, texu_i32, texu_i32);
texu_i32 _TexuStatusBarProc_OnGetAlign(texu_wnd *wnd, texu_i32 id);

texu_sbwnd_part *_texu_sbwnd_part_new(const texu_char *, texu_i32, texu_ui32, texu_ui32, texu_ui32);
void _texu_sbwnd_part_del(texu_sbwnd_part *);
texu_sbwnd_part *_texu_sbwnd_part_find(texu_list *, texu_i32);

texu_sbwnd_part *
_texu_sbwnd_part_find(texu_list *list, texu_i32 id)

{
    texu_sbwnd_part *part = 0;
    texu_list_item *item = texu_list_first(list);
    while (item)
    {
        part = (texu_sbwnd_part *)item->data;
        if (part->id == id)
        {
            return part;
        }
        item = item->next;
    }
    return 0;
}

void
_texu_sbwnd_part_del(texu_sbwnd_part *part)
{
    if (part)
    {
        free(part);
    }
}

texu_sbwnd_part *
_texu_sbwnd_part_new(const texu_char *text, texu_i32 width, texu_ui32 color, texu_ui32 bgcolor, texu_ui32 align)
{
    texu_sbwnd_part *part = (texu_sbwnd_part *)malloc(sizeof(texu_sbwnd_part));
    if (part)
    {
        memset(part, 0, sizeof(texu_sbwnd_part));
        if (text)
        {
            texu_strcpy(part->text, text);
        }
        part->width = width;
        part->color = color;

        part->bgcolor = bgcolor;

        part->align = align;
    }
    return part;
}

void
_TexuStatusBarProc_OnSetText(texu_wnd *wnd, texu_i32 id, const texu_char *text)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    /*
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    */
    if (part && text)
    {
        texu_strcpy(part->text, text);
        texu_wnd_invalidate(wnd);
    }
}

const texu_char *
_TexuStatusBarProc_OnGetText(texu_wnd *wnd, texu_i32 id)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {
        return part->text;
    }
    return 0;
}

void
_TexuStatusBarProc_OnSetWidth(texu_wnd *wnd, texu_i32 id, texu_i32 width)
{
    texu_list_item *item = 0;
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    texu_i32 oldwidth = part->width;

    if (part && width >= 0)
    {
        part->width = width;

        item = texu_list_first(sb->parts);
        part = (texu_sbwnd_part *)item->data;
        part += (oldwidth - width);
        texu_wnd_invalidate(wnd);
    }
}

texu_i32
_TexuStatusBarProc_OnGetWidth(texu_wnd *wnd, texu_i32 id)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {
        return part->width;
    }
    return 0;
}

void
_TexuStatusBarProc_OnSetColor(texu_wnd *wnd, texu_i32 id, texu_ui32 color)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part && color > 0)
    {
        part->color = color;
        texu_wnd_invalidate(wnd);
    }
}

texu_ui32
_TexuStatusBarProc_OnGetColor(texu_wnd *wnd, texu_i32 id)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {
        return part->color;
    }
    return 0;
}


void
_TexuStatusBarProc_OnSetBgColor(texu_wnd *wnd, texu_i32 id, texu_ui32 color)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part && color > 0)
    {

        part->bgcolor = color;

        texu_wnd_invalidate(wnd);
    }
}

texu_ui32
_TexuStatusBarProc_OnGetBgColor(texu_wnd *wnd, texu_i32 id)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {

        return part->bgcolor;

    }
    return 0;
}
void
_TexuStatusBarProc_OnSetAlign(texu_wnd *wnd, texu_i32 id, texu_i32 align)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {
        switch (align)
        {
        case TEXU_ALIGN_LEFT:
        case TEXU_ALIGN_CENTER:
        case TEXU_ALIGN_RIGHT:
        {
            part->align = align;
            texu_wnd_invalidate(wnd);
            break;
        }
        }
    }
}

texu_i32
_TexuStatusBarProc_OnGetAlign(texu_wnd *wnd, texu_i32 id)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part)
    {
        return part->align;
    }
    return 0;
}

texu_i32 _TexuStatusBarProc_OnGetPartCount(texu_wnd *wnd)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 nparts = texu_list_count(sb->parts);
    return nparts;
}
texu_i32
_TexuStatusBarProc_OnAddPart(texu_wnd *wnd, const texu_char *text, texu_i32 width)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_list_item *item = 0;
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 nparts = texu_list_count(sb->parts);
    texu_sbwnd_part *part = _texu_sbwnd_part_new(text, width,
                                                 texu_env_get_syscolor(env, TEXU_COLOR_STATUSBAR),
                                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_STATUSBAR),
                                                 TEXU_ALIGN_LEFT);

    part->id = nparts;
    texu_list_add(sb->parts, (texu_lparam)part);

    item = texu_list_first(sb->parts);
    part = (texu_sbwnd_part *)item->data;
    part->width -= width;
    if (part->width < 0)
    {
        part->width = 0;
    }

    return nparts;
}

void
_TexuStatusBarProc_OnEraseBg(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    TexuDefWndProc(wnd, TEXU_WM_ERASEBG, (texu_lparam)dc, 0);
}

void
_TexuStatusBarProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_sbwnd *sb = 0;
    texu_list_item *item = 0;
    texu_sbwnd_part *part = 0;
    texu_char filler[TEXU_MAX_WNDTEXT + 1];
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }

    sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);

    item = texu_list_first(sb->parts);
    while (item)
    {
        part = (texu_sbwnd_part *)item->data;
        memset(filler, 0, sizeof(filler));
        texu_printf_alignment3(buf, part->text, part->width, TEXU_ALIGN_RIGHT, TEXU_TRUE, x, cx);
        memset(buf, 0, sizeof(buf));
        texu_printf_alignment3(buf, part->text, part->width, part->align, TEXU_TRUE, x, cx);
#ifdef TEXU_CIO_COLOR_MONO
        texu_cio_putstr_attr(dc, y, x, filler, texu_cio_get_reverse(dc, part->color));
        texu_cio_putstr_attr(dc, y, x, buf,
                             texu_cio_get_reverse(dc, part->color));
#else
        texu_cio_draw_text(dc, y, x, filler, part->color, part->bgcolor,
                              texu_wnd_get_clsname(wnd),
                              texu_wnd_get_id(wnd));
        texu_cio_draw_text(dc, y, x, buf, part->color, part->bgcolor,
                              texu_wnd_get_clsname(wnd),
                              texu_wnd_get_id(wnd));
#endif

        x += part->width;
        item = item->next;
    }
}

texu_status
_TexuStatusBarProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_list *parts = 0;
    texu_sbwnd *sb = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_STATUSBAR);
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_STATUSBAR);

    parts = texu_list_new();
    if (!parts)
    {
        return TEXU_NOMEM;
    }
    sb = (texu_sbwnd *)malloc(sizeof(texu_sbwnd));
    if (!sb)
    {
        return TEXU_NOMEM;
    }
    memset(sb, 0, sizeof(texu_sbwnd));
    sb->parts = parts;
    sb->firstpart = _texu_sbwnd_part_new(
        attrs->text,
        attrs->width,
        color,
        bgcolor,
        TEXU_ALIGN_LEFT);

    texu_list_add(sb->parts, (texu_lparam)sb->firstpart);

    texu_wnd_set_color(wnd, color, color);
    texu_wnd_set_userdata(wnd, sb);

    texu_wnd_move(wnd, attrs->y, attrs->x, 1, attrs->width, TEXU_FALSE);
    texu_wnd_enable(wnd, TEXU_FALSE);

    return TEXU_OK;
}

texu_longptr
_TexuStatusBarProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
    case TEXU_WM_SETTEXT:
        TexuDefWndProc(wnd, TEXU_WM_SETTEXT, param1, param2);
        _TexuStatusBarProc_OnSetText(wnd, 0, (const texu_char *)param1);
        break;

    case TEXU_WM_CREATE:
        return _TexuStatusBarProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_WM_ERASEBG:
        _TexuStatusBarProc_OnEraseBg(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;

    case TEXU_WM_PAINT:
        _TexuStatusBarProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;

    case TEXU_WM_ENABLE:
        return TexuDefWndProc(wnd, msg, 0, 0);

    case TEXU_SBM_GETPARTCOUNT:
        return _TexuStatusBarProc_OnGetPartCount(wnd);

    case TEXU_SBM_ADDPART:
        return _TexuStatusBarProc_OnAddPart(wnd, (const texu_char *)param1, (texu_i32)param2);

    case TEXU_SBM_SETTEXT:
        _TexuStatusBarProc_OnSetText(wnd, (texu_i32)param1, (const texu_char *)param2);
        return 0;

    case TEXU_SBM_GETTEXT:
        return (texu_lparam)_TexuStatusBarProc_OnGetText(wnd, (texu_i32)param1);

    case TEXU_SBM_SETWIDTH:
        _TexuStatusBarProc_OnSetWidth(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_SBM_GETWIDTH:
        return (texu_lparam)_TexuStatusBarProc_OnGetWidth(wnd, (texu_i32)param1);

    case TEXU_SBM_SETALIGN:
        _TexuStatusBarProc_OnSetAlign(wnd, (texu_i32)param1, (texu_i32)param2);
        return 0;

    case TEXU_SBM_GETALIGN:
        return (texu_lparam)_TexuStatusBarProc_OnGetAlign(wnd, (texu_i32)param1);

    case TEXU_SBM_SETCOLOR:
        _TexuStatusBarProc_OnSetColor(wnd, (texu_i32)param1, (texu_ui32)param2);
        return 0;

    case TEXU_SBM_GETCOLOR:
        return (texu_lparam)_TexuStatusBarProc_OnGetColor(wnd, (texu_i32)param1);

    case TEXU_SBM_SETBGCOLOR:
        _TexuStatusBarProc_OnSetBgColor(wnd, (texu_i32)param1, (texu_ui32)param2);
        return 0;

    case TEXU_SBM_GETBGCOLOR:
        return (texu_lparam)_TexuStatusBarProc_OnGetBgColor(wnd, (texu_i32)param1);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
