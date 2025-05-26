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
    texu_ui32 color; /*background*/
    texu_ui32 defcolor;
    texu_ui32 titlebg;
    texu_ui32 labelbg;
    texu_ui32 okbg;
    texu_ui32 cancelbg;
    texu_ui32 yesbg;
    texu_ui32 nobg;
    texu_ui32 bgcolor; /*background*/
    texu_ui32 defbg;
    texu_i32  defbutton;
    void *userdata;
    texu_char text[TEXU_1024_WNDTEXT + 1];
    texu_char caption[TEXU_MAX_WNDTEXT + 1];
    texu_i32  max_buttons;
    texu_i32  curbtn;
    texu_wnd *buttons[4];
};
typedef struct texu_msgbox texu_msgbox;

texu_wnd *
texu_wnd_msgbox3(
    const texu_char *caption,
    const texu_char *text,
    texu_wnd *owner,
    texu_ui32 id,
    texu_ui32 buttons,
    texu_ui32 defbtn,
    void *userdata,
    texu_msgbox_attrs *mbxattrs)
{
    texu_wnd_attrs attrs;
    texu_wnd    *wnd    = 0;
    texu_status rc      = TEXU_OK;
    texu_env    *env        = texu_wnd_get_env(owner);
    texu_wnd    *desktop    = texu_env_get_desktop(env);
    texu_wnd    *childwnd   = 0;
    texu_ui32  color = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    texu_ui32  bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    texu_msgbox_attrs mbxattrs2;

    if (!mbxattrs)
    {
        memset(&mbxattrs2, 0, sizeof(mbxattrs2));
        mbxattrs = &mbxattrs2;
    }
    mbxattrs->caption = caption;
    mbxattrs->text = text;
    mbxattrs->defbtn = defbtn;
    
    wnd = texu_wnd_new(env);
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
    attrs.text          = caption;
    attrs.normalcolor   = color;
    attrs.disabledcolor = color;
    attrs.focusedcolor  = color;

    attrs.normalbg      = bgcolor;
    attrs.disabledbg    = bgcolor;
    attrs.focusedbg     = bgcolor;

    attrs.id = id;
    attrs.clsname       = TEXU_MSGBOX_CLASS;
    attrs.userdata      = userdata;
    attrs.style         = buttons;
    attrs.exstyle       = 0;

    if (0 == mbxattrs->color      )     mbxattrs->color        = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    if (0 == mbxattrs->titlecolor )     mbxattrs->titlecolor   = texu_env_get_syscolor(env, TEXU_COLOR_TITLE_WINDOW);
    if (0 == mbxattrs->labelcolor )     mbxattrs->labelcolor   = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    if (0 == mbxattrs->okcolor    )     mbxattrs->okcolor      = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_OK);
    if (0 == mbxattrs->cancelcolor)     mbxattrs->cancelcolor  = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_CANCEL);
    if (0 == mbxattrs->yescolor   )     mbxattrs->yescolor     = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_YES);
    if (0 == mbxattrs->nocolor    )     mbxattrs->nocolor      = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_NO);
    if (0 == mbxattrs->defcolor   )     mbxattrs->defcolor     = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_DEFAULT);
    if (0 == mbxattrs->bgcolor    )     mbxattrs->bgcolor      = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    if (0 == mbxattrs->titlebg    )     mbxattrs->titlebg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_TITLE_WINDOW);
    if (0 == mbxattrs->labelbg    )     mbxattrs->labelbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    if (0 == mbxattrs->okbg       )     mbxattrs->okbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_OK);
    if (0 == mbxattrs->cancelbg   )     mbxattrs->cancelbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_CANCEL);
    if (0 == mbxattrs->yesbg      )     mbxattrs->yesbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_YES);
    if (0 == mbxattrs->nobg       )     mbxattrs->nobg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_NO);
    if (0 == mbxattrs->defbg      )     mbxattrs->defbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_DEFAULT);

    attrs.userdata2     = mbxattrs;

    rc = texu_wnd_create(wnd, desktop, &attrs);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(wnd);
        return 0;
    }

    texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER,   (texu_lparam)owner, 0);
    texu_wnd_send_msg(wnd, TEXU_MBM_SETCAPTION, (texu_lparam)mbxattrs->caption, 0);

    texu_env_push_wnd(env, wnd);

    childwnd = texu_wnd_get_activechild(wnd);
    if (childwnd)
    {
        texu_wnd_send_msg(childwnd, TEXU_WM_SETFOCUS, 0, 0);
    }

    texu_wnd_visible(wnd, TEXU_TRUE);
    texu_wnd_invalidate(wnd);

    return wnd;
}

texu_wnd *
texu_wnd_msgbox2(
    const texu_char *caption,
    const texu_char *text,
    texu_wnd *owner,
    texu_ui32 id,
    texu_ui32 buttons,
    void *userdata,
    texu_msgbox_attrs *mbxattrs)
{
    return texu_wnd_msgbox3(caption, text, owner, id, buttons, 0, userdata, 0);
}

texu_wnd *
texu_wnd_msgbox(
    const texu_char *caption,
    const texu_char *text,
    texu_wnd *owner,
    texu_ui32 id,
    texu_ui32 buttons,
    void *userdata)
{
    return texu_wnd_msgbox2(caption, text, owner, id, buttons, userdata, 0);
}


void _TexuMsgBoxProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuMsgBoxProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuMsgBoxProc_OnDestroy(texu_wnd *);
void _TexuMsgBoxProc_OnSetCaption(texu_wnd *, const texu_char *);
void _TexuMsgBoxProc_OnSetOwner(texu_wnd *, texu_wnd *);
void _TexuMsgBoxProc_OnSetTitleColor(texu_wnd *wnd, texu_ui32 color, texu_ui32 bgcolor);


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
texu_status _TexuMsgBoxProc_CreateButtons2(
    texu_wnd *wnd,
    texu_i32 y,
    texu_i32 x,
    texu_char *caption,
    texu_ui32 id,
    texu_i32 cmd,
    texu_ui32 color,
    texu_ui32 bgcolor,
    texu_ui32 defcolor,
    texu_ui32 defbgcolor);

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
    texu_wnd *frame = texu_wnd_get_frame(wnd);

    notify.wnd = wnd;
    notify.id = texu_wnd_get_id(wnd);
    notify.code = code;
    texu_wnd_send_msg(frame, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}

void
_TexuEditProc_Notify(texu_wnd *wnd, texu_ui32 code, texu_char* text, texu_i32 excode)
{
    texu_edit_notify notify;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_wnd *frame = texu_wnd_get_frame(wnd);

    notify.hdr.wnd = wnd;
    notify.hdr.id = texu_wnd_get_id(wnd);
    notify.hdr.code = code;
    notify.text = text;
    notify.excode = excode;
    texu_wnd_send_msg(frame, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
}

void
_TexuMsgBoxProc_OnDestroy(texu_wnd *wnd)
{
    texu_wnd *topwnd = 0;
    texu_env *env = 0;
    texu_wnd *desktop = 0;
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    free(msgbox);

    env = texu_wnd_get_env(wnd);
    desktop = texu_env_get_desktop(env);
    texu_env_pop_wnd(env);

    topwnd = texu_env_top_wnd(env);
    texu_wnd_send_msg(topwnd, TEXU_WM_SHOW, TEXU_TRUE, 0);
    texu_wnd_invalidate(topwnd);

    /*remove msgbox from topwnd*/
    texu_wnd_remove_child(desktop, wnd);
    /*texu_wnd_del(wnd);*/
    /*texu_cio_interval(texu_wnd_get_cio(wnd), 500);*/
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
    ++x;
    width -= 2;
    texu_printf_alignment3(buf, msgbox->caption, width, TEXU_ALIGN_CENTER, TEXU_TRUE, x, cx);
#if defined TEXU_CIO_COLOR_MONO
    {
        texu_rect rcwnd;
        texu_wnd_get_rect(wnd, &rcwnd);
        texu_cio_draw_rect(dc, &rcwnd, TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK, 0);
    }
    texu_cio_draw_text2(dc, y, x, buf, TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK, TB_REVERSE,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#else
    texu_cio_draw_text(dc, y, x, buf, msgbox->titlecolor, msgbox->titlebg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#endif
}

texu_i32
_TexuDefWndProc_CountLines(const texu_char *text, texu_i32 *maxlen)
{
    texu_i32 lines = 0;
    texu_char *tok;
    texu_i32 len = 0;
    texu_char buf[TEXU_1024_WNDTEXT + 1];
    texu_i32 max_len  = 0;
#if (defined WIN32 && defined UNICODE)
    texu_char *pszContext;
#endif
    /* *maxlen = 0;*/
    len = texu_strlen(text);
    if (text &&  len > 0)
    {
        ++lines;
    }
    else
    {
        *maxlen = 0;
        return lines;
    }
    memset(buf, 0, sizeof(buf));
    len = (len > TEXU_1024_WNDTEXT ? TEXU_1024_WNDTEXT : len);
    texu_memcpy(buf, text, len);

#if (defined WIN32 && defined UNICODE)
    tok = wcstok_s(buf, TEXUTEXT("\n"), &pszContext);
#else
    tok = strtok(buf, "\n");
#endif
    while (tok)
    {
        ++lines;
        len = texu_strlen(tok);
        if (len > max_len)
        {
            max_len = len;
        }
#if (defined WIN32 && defined UNICODE)
        tok = wcstok_s(NULL, TEXUTEXT("\n"), &pszContext);
#else
        tok = strtok(0, "\n");
#endif
    }
    *maxlen = max_len;
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
    childattrs.clsname = TEXU_BUTTON_CLASS;//TEXU_LABEL_CLASS;
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
#if defined TEXU_CIO_COLOR_MONO
    texu_wnd_send_msg(child, TEXU_LM_HIGHLIGHT, TEXU_TRUE, 0);
#endif
    texu_wnd_add_keycmd(wnd, cmd, id, 0);
    return rc;
}

texu_status
_TexuMsgBoxProc_CreateButtons2(
    texu_wnd *wnd,
    texu_i32 y,
    texu_i32 x,
    texu_char *caption,
    texu_ui32 id,
    texu_i32 cmd,
    texu_ui32 color,
    texu_ui32 bgcolor,
    texu_ui32 defcolor,
    texu_ui32 defbgcolor)
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
    childattrs.clsname = TEXU_BUTTON_CLASS;//TEXU_LABEL_CLASS;
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

    texu_wnd_set_focused_color(child, defcolor);
    texu_wnd_set_bgfocused_color(child, defbgcolor);
#if defined TEXU_CIO_COLOR_MONO
    texu_wnd_send_msg(child, TEXU_LM_HIGHLIGHT, TEXU_TRUE, 0);
#endif
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
static texu_char *buttons[] = { TEXUTEXT("   [O]K   "), TEXUTEXT("   [Y]es  "), TEXUTEXT("   [N]o   "), TEXUTEXT(" [C]ancel ") };
static texu_i32 btnwidths[] = {10, 10, 10, 10};
#else
static texu_char *buttons[] = { TEXUTEXT("   [O]K   "), TEXUTEXT("   [Y]es  "), TEXUTEXT("   [N]o   "), TEXUTEXT(" [C]ancel ") };
static texu_i32 btnwidths[] = {10, 10, 10, 10};
//static texu_char *buttons[] = { TEXUTEXT(" F1 - OK "), TEXUTEXT(" F2 - Yes "), TEXUTEXT(" F3 - No "), TEXUTEXT(" F4 - Cancel ") };
//static texu_i32 btnwidths[] = { 9, 10, 9, 13 };
#endif

texu_status
_TexuMsgBoxProc_CreateChildren(texu_wnd *wnd, texu_wnd_attrs *attrs, texu_i32 lines)
{
    texu_wnd *child = 0;
    texu_char *tok;
    texu_wnd_attrs childattrs;
    texu_i32 id = 10000;
    texu_i32 y = 2;
    texu_i32 x = 0;
    texu_bool idok = 0;
    texu_bool idcancel = 0;
    texu_bool idyes = 0;
    texu_bool idno = 0;
    texu_char *caption = 0;
    texu_status rc = TEXU_OK;
    texu_char buf[TEXU_1024_WNDTEXT + 1];
    texu_i32 width = 0;
    texu_i32 wndwidth = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
    texu_msgbox_attrs *mbxattrs = (texu_msgbox_attrs*)attrs->userdata2;
#if (defined WIN32 && defined UNICODE)
    texu_char *pszContext;
#endif
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);

    if (lines < 1)
    {
        return TEXU_ERROR;
    }

    /* create labels*/
    memset(buf, 0, sizeof(buf));
    texu_strcpy(buf, mbxattrs->text);
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
        childattrs.height = 1;
#if defined TEXU_CIO_COLOR_MONO
        childattrs.x = 1;
        childattrs.width = attrs->width - 2;
#else
        childattrs.x = 0;
        childattrs.width = attrs->width;
#endif
        childattrs.enable = TEXU_FALSE;
        childattrs.visible = TEXU_TRUE;
        childattrs.text = tok;
        childattrs.normalcolor      = color;
        childattrs.disabledcolor    = color;
        childattrs.focusedcolor     = color;

        childattrs.normalbg     = bgcolor;
        childattrs.disabledbg   = bgcolor;
        childattrs.focusedbg    = bgcolor;

        childattrs.id       = id;
        childattrs.clsname  = TEXU_LABEL_CLASS;
        childattrs.userdata = 0;
        childattrs.style    = TEXU_WS_CENTER;
        childattrs.exstyle  = 0;

        rc = texu_wnd_create(child, wnd, &childattrs);

        if (rc != TEXU_OK)
        {
            texu_wnd_del(child);
            return TEXU_ERROR;
        }
        texu_wnd_set_color(child, msgbox->labelcolor, msgbox->labelcolor);
                           //texu_env_get_syscolor(env, TEXU_COLOR_DIALOG),
                           //texu_env_get_syscolor(env, TEXU_COLOR_DIALOG));

        texu_wnd_set_bgcolor(child, msgbox->labelbg, msgbox->labelbg);
                           //texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG),
                           //texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG));
        //tok = wcstok_s(NULL, TEXUTEXT("\n"), &pszContext);

        tok = strtok(0, "\n");

        ++y;
        ++id;
    }
    /* create buttons */
    msgbox->max_buttons = 0;
    msgbox->curbtn = 0;
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
        rc = _TexuMsgBoxProc_CreateButtons2(
            wnd,
            y,
            x,
            caption,
            TEXU_IDOK,
            TVK_F1,
            msgbox->okcolor,
            msgbox->okcolor,
            msgbox->defcolor,
            msgbox->defcolor);
        if (rc != TEXU_OK)
        {
            return rc;
        }
        //texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, TEXU_IDOK, 0);
        texu_wnd_add_keycmd(wnd, 'O', TEXU_IDOK, 0);
        texu_wnd_add_keycmd(wnd, 'o', TEXU_IDOK, 0);
        /*texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, TEXU_IDOK, 0);*/
        x += texu_strlen(caption) + 1;
        msgbox->buttons[msgbox->max_buttons] = texu_wnd_find_child(wnd, TEXU_IDOK);
        ++msgbox->max_buttons;
        texu_wnd_set_focus(msgbox->buttons[msgbox->max_buttons]);
    }
    if (idyes && !idok)
    {
        caption = buttons[F2_YES];
        rc = _TexuMsgBoxProc_CreateButtons2(
            wnd,
            y,
            x,
            caption,
            TEXU_IDYES,
            TVK_F2,
            msgbox->yescolor,
            msgbox->yescolor,
            msgbox->defcolor,
            msgbox->defcolor);
        if (rc != TEXU_OK)
        {
            return rc;
        }
        //texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, TEXU_IDYES, 0);
        texu_wnd_add_keycmd(wnd, 'Y', TEXU_IDYES, 0);
        texu_wnd_add_keycmd(wnd, 'y', TEXU_IDYES, 0);
        x += texu_strlen(caption) + 1;
        msgbox->buttons[msgbox->max_buttons] = texu_wnd_find_child(wnd, TEXU_IDYES);
        ++msgbox->max_buttons;
        texu_wnd_set_focus(msgbox->buttons[msgbox->max_buttons]);
    }
    if (idno)
    {
        caption = buttons[F3_NO];
        rc = _TexuMsgBoxProc_CreateButtons2(
            wnd,
            y,
            x,
            caption,
            TEXU_IDNO,
            TVK_F3,
            msgbox->nocolor,
            msgbox->nocolor,
            msgbox->defcolor,
            msgbox->defcolor);
        if (rc != TEXU_OK)
        {
            return rc;
        }
        //++msgbox->curbtn;
        texu_wnd_add_keycmd(wnd, 'N', TEXU_IDNO, 0);
        texu_wnd_add_keycmd(wnd, 'n', TEXU_IDNO, 0);
        x += texu_strlen(caption) + 1;
        msgbox->buttons[msgbox->max_buttons] = texu_wnd_find_child(wnd, TEXU_IDNO);
        ++msgbox->max_buttons;
        texu_wnd_set_focus(msgbox->buttons[msgbox->max_buttons]);
    }
    if (idcancel)
    {
        caption = buttons[F4_CANCEL];
        rc = _TexuMsgBoxProc_CreateButtons2(
            wnd,
            y,
            x,
            caption,
            TEXU_IDCANCEL,
            TVK_F4,
            msgbox->cancelcolor,
            msgbox->cancelcolor,
            msgbox->defcolor,
            msgbox->defcolor);
        if (rc != TEXU_OK)
        {
            return rc;
        }
        texu_wnd_add_keycmd(wnd, 'C', TEXU_IDCANCEL, 0);
        texu_wnd_add_keycmd(wnd, 'c', TEXU_IDCANCEL, 0);
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ESCAPE, TEXU_IDCANCEL, 0);
        x += texu_strlen(caption) + 1;
        msgbox->buttons[msgbox->max_buttons] = texu_wnd_find_child(wnd, TEXU_IDCANCEL);
        ++msgbox->max_buttons;
        texu_wnd_set_focus(msgbox->buttons[msgbox->max_buttons]);
    }
#if 0
    if (1 == msgbox->max_buttons) /*only 1 button */
    {
        id = texu_wnd_get_id(msgbox->buttons[0]);
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ESCAPE, id, 0);
        texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER, id, 0);
        texu_wnd_set_focus(msgbox->buttons[0]);
    }
    else
#endif
    {
        if (msgbox->defbutton)
        {
            //texu_wnd_add_keycmd(wnd, TEXU_KEY_ENTER,  msgbox->defbutton, 0);
            texu_i32 i = 0;
            for (i = 0; i < msgbox->max_buttons; ++i)
            {
                id = texu_wnd_get_id(msgbox->buttons[i]);
                if (id == msgbox->defbutton)
                {
                    texu_wnd_set_focus(msgbox->buttons[i]);
                    msgbox->curbtn = i;
                    break;
                }
            }
        }
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
    texu_msgbox_attrs *mbxattrs = (texu_msgbox_attrs*)attrs->userdata2;

    msgbox = (texu_msgbox *)malloc(sizeof(texu_msgbox));
    if (!msgbox)
    {
        return TEXU_NOMEM;
    }

    memset(msgbox, 0, sizeof(texu_msgbox));
    if (mbxattrs)
    {
        msgbox->color       = mbxattrs->color;
        msgbox->titlecolor  = mbxattrs->titlecolor;
        msgbox->labelcolor  = mbxattrs->labelcolor;
        msgbox->okcolor     = mbxattrs->okcolor;
        msgbox->cancelcolor = mbxattrs->cancelcolor;
        msgbox->yescolor    = mbxattrs->yescolor;
        msgbox->nocolor     = mbxattrs->nocolor;
        msgbox->bgcolor     = mbxattrs->bgcolor;
        msgbox->titlebg     = mbxattrs->titlebg;
        msgbox->labelbg     = mbxattrs->labelbg;
        msgbox->okbg        = mbxattrs->okbg;
        msgbox->cancelbg    = mbxattrs->cancelbg;
        msgbox->yesbg       = mbxattrs->yesbg;
        msgbox->nobg        = mbxattrs->nobg;
        msgbox->defbutton   = mbxattrs->defbtn;
        msgbox->defcolor    = mbxattrs->defcolor;
        msgbox->defbg       = mbxattrs->defbg;
    }
    else
    {
        msgbox->color       = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        msgbox->titlecolor  = texu_env_get_syscolor(env, TEXU_COLOR_TITLE_WINDOW);
        msgbox->labelcolor  = texu_env_get_syscolor(env, TEXU_COLOR_DIALOG);
        msgbox->okcolor     = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_OK);
        msgbox->cancelcolor = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_CANCEL);
        msgbox->yescolor    = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_YES);
        msgbox->nocolor     = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_NO);
        msgbox->bgcolor     = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
        msgbox->titlebg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_TITLE_WINDOW);
        msgbox->labelbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_DIALOG);
        msgbox->okbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_OK);
        msgbox->cancelbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_CANCEL);
        msgbox->yesbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_YES);
        msgbox->nobg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_NO);
        msgbox->defbutton   = 0;
        msgbox->defcolor    = texu_env_get_syscolor(env, TEXU_COLOR_BUTTON_DEFAULT);
        msgbox->defbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_BUTTON_DEFAULT);
    }

    texu_wnd_set_userdata(wnd, msgbox); /*save info*/

    /* adjust window rect */
    lines = _TexuDefWndProc_CountLines(mbxattrs->text, &maxlen);
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
    rect.y = (texu_env_screen_height(env) - rect.lines) / 2;
    rect.x = (texu_env_screen_width(env) - rect.cols) / 2;

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
    texu_wnd_set_color(wnd, msgbox->color, msgbox->color);
    texu_wnd_set_bgcolor(wnd, msgbox->bgcolor, msgbox->bgcolor);

    return TEXU_OK;
}

void _TexuMsgBoxProc_OnSetTitleColor(texu_wnd *wnd, texu_ui32 color, texu_ui32 bgcolor)
{
    texu_msgbox *msgbox = 0;

    msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    msgbox->titlecolor = color;
    msgbox->titlebg = bgcolor;

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

texu_bool
_TexuMsgBoxProc_OnShow(texu_wnd *wnd, texu_bool visible)
{
    texu_msgbox *msgbox = 0;
    texu_bool oldvisible = TEXU_FALSE;

    msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);

    if (visible)
    {
        texu_wnd_invalidate(msgbox->owner);
        texu_wnd_lock_update(msgbox->owner, TEXU_TRUE);
    }
    return oldvisible;
}

texu_i32 _TexuMsgBoxProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 mod)
{
    texu_i32 i = 0;
    texu_msgbox *msgbox = 0;
    msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    switch (ch)
    {
        //case TEXU_KEY_UP:
        case TEXU_KEY_LEFT:
        {
            if (msgbox->curbtn > 0)
            {
                texu_wnd_kill_focus(msgbox->buttons[msgbox->curbtn]);
                --msgbox->curbtn;
                texu_wnd_set_focus(msgbox->buttons[msgbox->curbtn]);
            }
            break;
        }
        case TEXU_KEY_RIGHT:
        {
            if (msgbox->curbtn < msgbox->max_buttons - 1)
            {
                texu_wnd_kill_focus(msgbox->buttons[msgbox->curbtn]);
                ++msgbox->curbtn;
                texu_wnd_set_focus(msgbox->buttons[msgbox->curbtn]);
            }
            break;
        }
        case TEXU_KEY_ENTER:
        {
            texu_i32 id = texu_wnd_get_id(msgbox->buttons[msgbox->curbtn]);
            texu_wnd_post_msg(wnd, TEXU_WM_COMMAND, id, 0);
            break;
        }
    }
    return TEXU_OK;
}


texu_i32 _TexuMsgBoxProc_OnQueryKeyUp(texu_wnd *wnd)
{
    texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
    if (msgbox->curbtn > 0)
    {
        return 0;
    }
    return 1;
}

texu_longptr
_TexuMsgBoxProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
    case TEXU_WM_COMMAND:
    {
        texu_msgbox *msgbox = (texu_msgbox *)texu_wnd_get_userdata(wnd);
        _TexuMsgBoxProc_Notify(wnd, TEXU_MBN_ENDDIALOG, param1);
        texu_wnd_lock_update(msgbox->owner, TEXU_FALSE);
        texu_wnd_destroy(wnd);
        return 0;
    }
    case TEXU_WM_SHOW:
        return _TexuMsgBoxProc_OnShow(wnd, (texu_bool)param1);

    case TEXU_WM_PAINT:
        _TexuMsgBoxProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;

    case TEXU_WM_DESTROY:
        _TexuMsgBoxProc_OnDestroy(wnd);
        break;

    case TEXU_WM_CREATE:
        return _TexuMsgBoxProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

    case TEXU_MBM_SETOWNER:
        _TexuMsgBoxProc_OnSetOwner(wnd, (texu_wnd *)param1);
        return 0;

    case TEXU_MBM_SETCAPTION:
        _TexuMsgBoxProc_OnSetCaption(wnd, (const texu_char *)param1);
        return 0;

    case TEXU_MBM_SETTITLECOLOR:
        _TexuMsgBoxProc_OnSetTitleColor(wnd, (texu_ui32)param1, (texu_ui32)param2);
        return 0;
    
    case TEXU_WM_KEYDOWN:
        if (TEXU_KEY_UP == param1)
        {
            return 0;
        }
        _TexuMsgBoxProc_OnKeyDown(wnd, param1, param2);
        break;

    case TEXU_WM_QUERYKEYUP:
        return _TexuMsgBoxProc_OnQueryKeyUp(wnd);
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
struct texu_label
{
    texu_bool highlight;
    texu_i32  framecolor;
    texu_i32  framebg;
};
typedef struct texu_label texu_label;
void _TexuLabelProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
void _TexuLabelProc_OnEraseBg(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuLabelProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuLabelProc_OnDestroy(texu_wnd *wnd);
/*
void
_TexuLabelProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    TexuDefWndProc(wnd, TEXU_WM_ERASEBG, (texu_lparam)cio, (texu_lparam)rect);
}
*/
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
    texu_label *label = (texu_label*)texu_wnd_get_userdata(wnd);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
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
    /*if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
        colorbg = disbg;
    }*/
    if (style & TEXU_LS_FRAME)
    {
        color = label->framecolor;
        colorbg = label->framebg;
        texu_cio_draw_frame(cio, buf, rect, color, colorbg, 0);
    }
    else
    {
#if defined __USE_TERMBOX2__
        texu_cio_draw_text2(cio, y, x, buf, color, colorbg,
            (label->highlight ? TB_REVERSE : 0),
            texu_wnd_get_clsname(wnd),
            texu_wnd_get_id(wnd));
#else
        texu_cio_draw_text2(cio, y, x, buf, color, colorbg,
            0,
            texu_wnd_get_clsname(wnd),
            texu_wnd_get_id(wnd));
#endif
    }
}

texu_status
_TexuLabelProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_label *label = (texu_label*)malloc(sizeof(struct texu_label));
    if (!label)
    {
        return TEXU_ENOMEM;
    }
    label->highlight = TEXU_FALSE;
    texu_wnd_set_userdata(wnd, label);

    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL_DISABLED));
    texu_wnd_enable(wnd, TEXU_FALSE);

    return TEXU_OK;
}

void
_TexuLabelProc_OnDestroy(texu_wnd *wnd)
{
    texu_label *label = (texu_label*)texu_wnd_get_userdata(wnd);
    free(label);
    texu_wnd_set_userdata(wnd, 0);
}

void
_TexuLabelProc_OnHighlight(texu_wnd *wnd, texu_bool highlight)
{
    texu_label *label = (texu_label*)texu_wnd_get_userdata(wnd);
    label->highlight = highlight;
    texu_wnd_invalidate(wnd);
}

void 
_TexuLabelProc_OnFrameColor(texu_wnd *wnd, texu_i32 color, texu_i32 bgcolor)
{
    texu_label *label = (texu_label*)texu_wnd_get_userdata(wnd);
    label->framebg = bgcolor;
    label->framecolor = color;
}

texu_longptr
_TexuLabelProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_LM_HIGHLIGHT:
            _TexuLabelProc_OnHighlight(wnd, (texu_bool)param1);
            return 0;

        case TEXU_LM_FRAMECOLOR:
            _TexuLabelProc_OnFrameColor(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuLabelProc_OnDestroy(wnd);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuLabelProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuLabelProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
/*
        case TEXU_WM_ERASEBG:
            _TexuLabelProc_OnEraseBg(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
*/
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
#define PANEL_TITLE_ID      1
#define PANEL_STATUS_ID     2

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
void        _TexuPanelProc_OnNotify(texu_wnd* wnd, texu_wnd_notify* notify);
void        _TexuPanelProc_OnGetClientRect(texu_wnd *wnd, texu_urect *rect);

void        _TexuPanelProc_OnGetClientRect(texu_wnd *wnd, texu_urect *rect)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_urect rcwnd;

    texu_wnd_send_msg(wnd, TEXU_WM_GETWINDOWRECT, (texu_lparam)&rcwnd, 0);
    *rect = rcwnd;
#if defined TEXU_CIO_COLOR_MONO
    if ((style & TEXU_PNS_TITLE) || (style & TEXU_PNS_BORDER))
    {
        rect->r2.y += 1;
        rect->r2.x += 1;
        rect->r2.height -= 2;
        rect->r2.width  -= 2;
    }
    else if (style & TEXU_PNS_STATUS)
    {
    }
#else
    if ((style & TEXU_PNS_TITLE) || (style & TEXU_PNS_BORDER))
    {
        rect->r2.y += 1;
        rect->r2.x += 1;
        rect->r2.height -= 2;
        rect->r2.width  -= 2;
    }
    else if (style & TEXU_PNS_STATUS)
    {
    }
#endif
}

texu_wnd*   _TexuPanelProc_OnGetStatusBar(texu_wnd* wnd)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_wnd* status = 0;
    if (style & TEXU_PNS_STATUS)
    {
        status = texu_wnd_find_child(wnd, PANEL_STATUS_ID);
    }
    return status;
}

texu_wnd*   _TexuPanelProc_OnGetTitle(texu_wnd* wnd)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_wnd* title = 0;
    if (style & TEXU_PNS_TITLE)
    {
        title = texu_wnd_find_child(wnd, PANEL_TITLE_ID);
    }
    return title;
}

void
_TexuPanelProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_wnd* title = _TexuPanelProc_OnGetTitle(wnd);
    if (title)
    {
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
    texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, 0, 0);
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
    texu_wnd_set_color(wnd, color, discolor);
    texu_wnd_set_bgcolor(wnd, bgcolor, disbgcolor);
    /*texu_wnd_enable(wnd, TEXU_FALSE);*/
    /*to paint the background*/
    //texu__wnd_create_blank_lines(env, wnd);
    /*after creating the blank lines*/
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    texu_wnd* title = 0;
    if (attrs->style & TEXU_PNS_TITLE)
    {
        texu_wnd_attrs attrs2;/* = *attrs;*/
        title = texu_wnd_new(env);
        if (!title)
        {
            return -1;
        }
        memset(&attrs2, 0, sizeof(attrs2));
        attrs2.y = 0;//attrs->y;
        attrs2.x = 0;//attrs->x;
        attrs2.height = 1;
        attrs2.width = attrs->width;
#if defined TEXU_CIO_COLOR_MONO
        if (attrs->style & TEXU_PNS_BORDER)
        {
            ++attrs2.x;
            attrs2.width -= 2;
        }
#else
#endif
        attrs2.visible = TEXU_TRUE;
        attrs2.text = text;

        texu_wnd_get_color(wnd, &attrs2.normalcolor, &attrs2.disabledcolor);
        attrs2.focusedcolor = texu_wnd_get_focused_color(wnd);
        texu_wnd_get_bgcolor(wnd, &attrs2.normalbg, &attrs2.disabledbg);
        attrs2.focusedbg = texu_wnd_get_bgfocused_color(wnd);

        attrs2.id = PANEL_TITLE_ID;
#if 0//defined TEXU_CIO_COLOR_MONO
        attrs2.enable = TEXU_FALSE;
        attrs2.clsname = TEXU_EDIT_CLASS;
#else
        attrs2.enable = TEXU_TRUE;
        attrs2.clsname = TEXU_LABEL_CLASS;
#endif
        attrs2.userdata = 0;
        attrs2.style = TEXU_WS_CENTER;
        attrs2.exstyle = 0;
        attrs2.on_validate = 0;
        attrs2.validate_data = 0;

        texu_i32 rc = texu_wnd_create(title, wnd, &attrs2);
        if (rc != TEXU_OK)
        {
            texu_wnd_del(title);
            return TEXU_ERROR;
        }
#if defined TEXU_CIO_COLOR_MONO
        texu_wnd_send_msg(title, TEXU_LM_HIGHLIGHT, TEXU_TRUE, 0);
#endif
    }
    if ((attrs->style & TEXU_PNS_TITLE) && (attrs->style & TEXU_PNS_BORDER))
    {
        texu_wnd_set_color(title, color, bgcolor);
        texu_wnd_set_bgcolor(title, bgcolor, disbgcolor);
        texu_wnd_set_text(title, text);
    }
    else if (attrs->style & TEXU_PNS_TITLE)
    {
        /*there is a title*/
        texu_wnd_set_color(title, color, bgcolor);
        texu_wnd_set_bgcolor(title, bgcolor, disbgcolor);
        texu_wnd_set_text(title, text);
    }
    else if (attrs->style & TEXU_PNS_BORDER)
    {
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
        attrs2.y = attrs->height - 1;
        attrs2.x = 0;//attrs->x;
        attrs2.height = 1;
        attrs2.width = attrs->width;
#if defined TEXU_CIO_COLOR_MONO
        if (attrs->style & TEXU_PNS_BORDER)
        {
            ++attrs2.x;
            attrs2.width -= 2;
        }
#else
#endif
        attrs2.enable = TEXU_TRUE;
        attrs2.visible = TEXU_TRUE;
        attrs2.text = TEXUTEXT("");

        texu_wnd_get_color(wnd, &attrs2.normalcolor, &attrs2.disabledcolor);
        attrs2.focusedcolor = texu_wnd_get_focused_color(wnd);
        texu_wnd_get_bgcolor(wnd, &attrs2.normalbg, &attrs2.disabledbg);
        attrs2.focusedbg = texu_wnd_get_bgfocused_color(wnd);

        attrs2.id = PANEL_STATUS_ID;
        attrs2.clsname = TEXU_STATUSBAR_CLASS;
        attrs2.userdata = 0;
        attrs2.style = TEXU_WS_LEFT;
        attrs2.exstyle = 0;
        attrs2.on_validate = 0;
        attrs2.validate_data = 0;

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
    texu_wnd *nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
    if (activewnd && nextwnd)
    {
        return nextwnd;
    }
    else if (activewnd)
    {
        nextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYNEXTWND, 0, 0);
        return nextwnd;
    }
    return 0;
}

texu_wnd* _TexuPanelProc_OnQueryPrevWnd(texu_wnd* wnd)
{
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
    if (activewnd && nextwnd)
    {
        return nextwnd;
    }
    else if (activewnd)
    {
        nextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYPREVWND, 0, 0);
        return nextwnd;
    }
    return 0;
}

texu_i32 _TexuPanelProc_OnQueryKeyPgUp(texu_wnd *wnd)
{
    texu_wnd *activechild = texu_wnd_send_msg(wnd, TEXU_WM_GETACTIVECHILD, 0, 0);
    texu_i32 keyup = texu_wnd_send_msg(activechild, TEXU_WM_QUERYKEYPGUP, 0, 0);
    return keyup; /*1 = still process key up*/
}

texu_i32 _TexuPanelProc_OnQueryKeyPgDown(texu_wnd *wnd)
{
    texu_wnd *activechild = texu_wnd_send_msg(wnd, TEXU_WM_GETACTIVECHILD, 0, 0);
    texu_i32 keyup = texu_wnd_send_msg(activechild, TEXU_WM_QUERYKEYPGDOWN, 0, 0);
    return keyup; /*1 = still process key up*/
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
        if ((ch >= '1' && ch <= '9') && (alt & TEXU_KEYPRESSED_ALT))
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
            if (ch == chNextKey || ch == TEXU_KEY_DOWN)
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYNEXTWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
                    if (0 == nextwnd)
                    {
                        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)activewnd, ch);
                    }
                }
            }
            else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYPREVWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
                    if (0 == nextwnd)
                    {
                        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)activewnd, ch);
                    }
                }
            }
            else if (ch == TEXU_KEY_UP)
            {
                qnextwnd = (texu_wnd*)texu_wnd_send_msg(activewnd, TEXU_WM_QUERYPREVWND, 0, 0);
                if (0 == qnextwnd)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
                    if (0 == nextwnd)
                    {
                        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)activewnd, ch);
                    }
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
                        rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_lparam)activewnd, ch);
                        if (rc < 0)
                        {
                            /*texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_lparam)activechild, 0);*/
                            return -1;
                        }
                    }
                    /*active window need to be done something by itself*/
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                    if (rc < 0)
                    {
                        /*texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);*/
                        return -1;
                    }
                    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)activewnd, 0);
                    /*texu_i32 y = texu_wnd_get_y(nextwnd);
                    texu_i32 x = texu_wnd_get_x(nextwnd);
                    texu_i32 width = texu_wnd_get_width(nextwnd);

                    texu_cio* cio = texu_wnd_get_cio(wnd);
                    texu_cio_gotoyx(cio, y, x + width - 1);*/

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
        else
        {
            /*sometimes, panel there is no child, then notify back to its parent*/
            texu_panel_notify notify;
            notify.hdr.wnd = wnd;
            notify.hdr.id = texu_wnd_get_id(wnd);
            notify.hdr.code = TEXU_PNN_KEYDOWN;
            notify.ch = ch;
            notify.alt = alt;
            texu_wnd_send_msg(parent, TEXU_WM_NOTIFY, (texu_lparam)&notify, 0);
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
#if defined TEXU_CIO_COLOR_MONO
    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_PANEL_TITLE);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_rect rcwnd;
    texu_ui32 discolor;
    texu_ui32 disbgcolor;
#endif
    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }

#if defined TEXU_CIO_COLOR_MONO
    /*texu_wnd* title = texu_wnd_find_child(wnd, -1);*/
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
#if (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
        texu_cio_draw_frame(cio, text, &rcwnd, color, bgcolor, 0);
#else
        texu_cio_draw_frame(cio, text, &rcwnd, 0);
#endif
    }
    else if (style & TEXU_PNS_BORDER)
    {
        /*there are title and border*/
#if (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
        texu_cio_draw_rect(cio, &rcwnd, color, bgcolor, 0);
#else
        texu_cio_draw_rect(cio, &rcwnd, color);
#endif
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
            sprintf(buf, "[ %s ]", (char*)texu_wnd_send_msg(status, TEXU_SBM_GETTEXT, (texu_lparam)i, 0));
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
    texu_ui32 style = texu_wnd_get_style(wnd);
    if (style & TEXU_PNS_TITLE)
    {
        texu_wnd* title = texu_wnd_find_child(wnd, PANEL_TITLE_ID); /*the top of label*/
        texu_wnd_set_color(title, color, bgcolor);
        texu_wnd_set_bgcolor(title, color, bgcolor);
    }
}

void _TexuPanelProc_OnSetTitleBgColor(texu_wnd* wnd, texu_i32 color, texu_i32 bgcolor)
{
    texu_ui32 style = texu_wnd_get_style(wnd);
    if (style & TEXU_PNS_TITLE)
    {
        texu_wnd* title = texu_wnd_find_child(wnd, PANEL_TITLE_ID); /*the top of label*/
        texu_wnd_set_bgcolor(title, color, bgcolor);
    }
}

void _TexuPanelProc_OnSetTitleAlign(texu_wnd *wnd, texu_i32 alignment)
{
    texu_i32 style = texu_wnd_get_style(wnd);
    if (style & TEXU_PNS_TITLE)
    {
        texu_wnd* title = texu_wnd_find_child(wnd, PANEL_TITLE_ID); /*the top of label*/
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
        case TEXU_WM_QUERYKEYPGUP:
            return _TexuPanelProc_OnQueryKeyPgUp(wnd);

        case TEXU_WM_QUERYKEYPGDOWN:
            return _TexuPanelProc_OnQueryKeyPgDown(wnd);

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

        case TEXU_PNM_SETTITLEBGCOLOR:
            _TexuPanelProc_OnSetTitleBgColor(wnd, (texu_i32)param1, (texu_i32)param2);
            break;

        case TEXU_WM_SETTEXT:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuPanelProc_OnSetText(wnd, (const texu_char *)param1);
            break;

        /*case TEXU_WM_SIZE:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuPanelProc_OnSize(wnd, (texu_rect *)param1, (texu_rect *)param2);
            return 0;*/

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

        case TEXU_WM_GETCLIENTRECT:
            _TexuPanelProc_OnGetClientRect(wnd, (texu_urect*)param1);
            return 0;
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
#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32    bgcolor = TEXU_CIO_COLOR_CYAN_BLACK;
#endif
    texu_i32    cx = texu_env_screen_width(env);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
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
    texu_printf_alignment3(buf, text, width, TEXU_ALIGN_CENTER, TEXU_TRUE, x, cx);

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    color = normcolor;
    bgcolor = normbg;
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
        bgcolor = disbg;
    }
    if (1 == btnwnd->state)
    {
        len = texu_strlen(buf);
        if (len > 1)
        {
            buf[0] = TEXUTEXT('[');
            buf[len - 1] = TEXUTEXT(']');
        }
        color = texu_wnd_get_focused_color(wnd);
        bgcolor = texu_wnd_get_bgfocused_color(wnd);
    }
#if defined TEXU_CIO_COLOR_MONO
    texu_cio_draw_text2(cio, y, x, buf, color, bgcolor, TB_REVERSE,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#else
    if (!(texu_wnd_is_enable(wnd)))
    {
        bgcolor = disbg;
    }
    texu_cio_draw_text(cio, y, x, buf, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#endif


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
_TexuButtonProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
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
            _TexuButtonProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);
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
    texu_i32  (*on_validate)(texu_wnd*, texu_char*, void* validate_data);
    void* on_validate_data;
    texu_i32 xpos; /*to scroll the x-position*/
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
texu_i32 _TexuEditProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state);
void _TexuEditProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
void _TexuEditProc_OnLimitText(texu_wnd *wnd, texu_i32 limit);
void _TexuEditProc_OnSetPasswdChar(texu_wnd *wnd, texu_char passchar);
void _TexuEditProc_OnShowPasswdChar(texu_wnd *wnd, texu_i32 show);
void _TexuEditProc_OnSetDecWidth(texu_wnd *wnd, texu_i32 width);
void _TexuEditProc_OnSetValidMinMax(texu_wnd *wnd, texu_i32 on, texu_editminmax *vmm);
void _TexuEditProc_OnSetText(texu_wnd *wnd, const texu_char *text);
texu_i32    _TexuEditProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
texu_status _TexuEditProc_OnGetInt(texu_wnd *wnd, texu_i32 *out);
texu_status _TexuEditProc_OnGetFloat(texu_wnd *wnd, texu_f64 *out);

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
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1] = "";

    if (0 == text || 0 == texu_strlen(text))
    {
        /*allow an empty text, also no need to validate it*/
        texu_strcpy(edit->editbuf, buf);
        edit->firstvisit = 1;
        TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)"", 0);
        texu_wnd_invalidate(wnd);
        return;
    }

    texu_strcpy(edit->editbuf, text);
    edit->firstvisit = 1;
    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)text, 0);

    if (TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
        _TexuEditProc_AddDecimalFormat(edit);
    }
    texu_wnd_invalidate(wnd);
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

void _TexuEditProc_OnSetValidation(texu_wnd* wnd, texu_validate_proc validproc, void* userdata)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    edit->on_validate = validproc;
    edit->on_validate_data = userdata;
}

void _TexuEditProc_OnInvalidate(texu_wnd *wnd)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    edit->selected = TEXU_TRUE;
    edit->invalid = TEXU_TRUE;
    texu_wnd_invalidate(wnd);
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

texu_i32 _TexuEditProc_AddCommas(texu_editwnd *edit)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 cnt = 1;
    texu_char *pbuf;
    texu_i32 len = texu_strlen(edit->editbuf) - 1;
    texu_char *psz = &edit->editbuf[len];

    memset(buf, 0, sizeof(buf));
    pbuf = buf;

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
_TexuEditProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state)
{
    texu_editwnd *edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1] = "";
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_f64 decimal = 0.0;
    texu_i32 number = 0;
    texu_i32 rcminmax = TEXU_CONTINUE;
    texu_i32 rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(wnd);
    //texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    if (chPrevKey == state) /*restore without doing anything*/
    {
        /* update text */
        edit->firstvisit = 1;
        edit->firstchar = 0;
        edit->editing = 0;
        edit->selected = 0;
        edit->xpos = 0;

        texu_wnd_get_text(wnd, buf, TEXU_MAX_WNDTEXT);
        texu_strcpy(edit->editbuf, buf);
        _TexuEditProc_Notify(wnd, TEXU_EN_KILLFOCUS, buf, state);

        texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
        texu_wnd_invalidate(wnd);
        return TEXU_OK;
    }

    if (0 == texu_strlen(edit->editbuf))
    {
        /*an empty string do not want to validate*/
        /* update text */
        edit->firstvisit = 1;
        edit->firstchar = 0;
        edit->editing = 0;
        edit->selected = 0;
        edit->xpos = 0;

        texu_wnd_set_text(wnd, buf);
        _TexuEditProc_Notify(wnd, TEXU_EN_KILLFOCUS, buf, state);
        texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
        texu_wnd_invalidate(wnd);
        return TEXU_OK;
    }

    /* check if style is TEXU_ES_DECIMAL */
    if (TEXU_ES_DECIMAL & style || TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
        decimal = texu_atof(edit->editbuf);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%.*f"), (texu_i32)edit->decwidth, decimal);
        texu_strcpy(edit->editbuf, buf);
        if (TEXU_TRUE == edit->onminmax)
        {
            if (decimal < (texu_f64)edit->min &&
                decimal > (texu_f64)edit->max)
            {
                //_TexuWndProc_Notify(wnd, TEXU_EN_INVALIDMINMAX);
                _TexuEditProc_Notify(wnd, TEXU_EN_INVALIDMINMAX, edit->editbuf, 0);

                texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
                rcminmax = TEXU_ERROR;
            }
        }
    }
    else
    {
        texu_strcpy(buf, edit->editbuf);
    }
    if (TEXU_ES_NUMBER & style)
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
        number = texu_atol(edit->editbuf);
        sprintf(edit->editbuf, "%d", (int)number);
    }
    if ((TEXU_ES_NUMBER & style) && (TEXU_TRUE == edit->onminmax))
    {
        _TexuEditProc_RemoveDecimalFormat(edit);
        number = texu_atol(edit->editbuf);
        if (number < edit->min &&
            number > edit->max)
        {
            rcminmax = TEXU_ERROR;
        }
    }
    /* validate min & max */
    if (rcminmax < 0)
    {
        //_TexuWndProc_Notify(wnd, TEXU_EN_INVALIDMINMAX);
        _TexuEditProc_Notify(wnd, TEXU_EN_INVALIDMINMAX, edit->editbuf, 0);
        texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
        return rcminmax;
    }

    if (edit->on_validate)
    {
        rc = edit->on_validate(wnd, edit->editbuf, edit->on_validate_data);
        if (rc < 0)
        {
            //_TexuWndProc_Notify(wnd, TEXU_EN_INVALIDATE);
            _TexuEditProc_Notify(wnd, TEXU_EN_INVALIDATE, edit->editbuf, rc);
            texu_wnd_send_msg(wnd, TEXU_EM_INVALIDATE, 0, 0);
            return rc;
        }
    }

    if (TEXU_ES_AUTODECIMALCOMMA & style)
    {
        _TexuEditProc_AddDecimalFormat(edit);
    }
    else if (TEXU_ES_AUTOCOMMAS & style)
    {
        _TexuEditProc_AddCommas(edit);
    }
    /* update text */
    /*edit->firstvisit = 1;*/
    edit->firstchar = 0;
    edit->editing = 0;
    edit->selected = 0;
    edit->xpos = 0;
    texu_wnd_set_text(wnd, edit->editbuf);
    
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    texu_wnd_invalidate(wnd);

    _TexuEditProc_Notify(wnd, TEXU_EN_KILLFOCUS, edit->editbuf, 0); /*can move to the next windows*/
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
    edit->xpos = 0;

    //_TexuWndProc_Notify(wnd, TEXU_EN_SETFOCUS);
    _TexuEditProc_Notify(wnd, TEXU_EN_SETFOCUS, edit->editbuf, 0);

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
    if (0)/*(edit->onminmax)*//*validate min/max should be at OnKillFocus()*/
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
    if (0)/*TEXU_OK == rc && edit->onminmax)*//*validate min/max should be at OnKillFocus()*/
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
        case TEXU_KEY_ESCAPE:
        case 0x7f:
        case TEXU_KEY_DELETE:
        case TEXU_KEY_BACKSPACE:
        case TEXU_KEY_UP:
        case TEXU_KEY_DOWN:
        case TEXU_KEY_LEFT:
        case TEXU_KEY_RIGHT:
        case TEXU_KEY_ENTER:
            texu_wnd_send_msg(wnd, TEXU_WM_CHAR, ch, alt);
            break;

#if 0
        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);
                if (prevwnd)
                {
                    texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                }
            }
            break;
        case TEXU_KEY_DOWN:
            /*texu_wnd_invalidate(wnd);*/
            /*move next*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *nextwnd = texu_wnd_get_next_activechild(parent, wnd);
                if (nextwnd)
                {
                    texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                }
            }
            break;
#endif
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
#if 1 //(defined WIN32 && defined _WINDOWS) || !(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
#endif
    texu_i32 cx = texu_env_screen_width(env);
#if (defined WIN32)// && defined _WINDOWS)
    static texu_char printable_chars[] = TEXUTEXT(" ~!@#$%^&*_-+=,?/|.\\\'\"()[]{}<>abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
#endif
    texu_char textcommas[TEXU_MAX_WNDTEXT + 1];
    texu_urect rcscr;
    texu_urect rcwnd;

    if (!(texu_wnd_is_enable(wnd)))
    {
        return;
    }
    texu_wnd_get_screen_rect(wnd, &rcscr);
    texu_wnd_get_window_rect(wnd, &rcwnd);
    y = rcscr.r1.y;
    x = rcscr.r1.x;
    if (rcwnd.r2.width < 1)
    {
        return;
    }

    edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(wnd, textcommas, TEXU_MAX_WNDTEXT);
    texu_strcpy(text, textcommas);
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
#if 0 //defined TEXU_CIO_COLOR_MONO
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

                if (0)/*((TEXU_ES_NUMBER & style) && edit->onminmax)*/
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
                            /*texu_cio_putch_attr(dc, y, x + len,
                                                edit->passchar,
                                                texu_cio_get_color(dc, selcolor));*/
                            texu_cio_draw_char2(dc, y, x + len, edit->passchar,
                                selcolor, selbg, 0,
                                texu_wnd_get_clsname(wnd),
                                texu_wnd_get_id(wnd));
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
                        /*texu_cio_putch_attr(dc, y, x + len,
                                            ch,
                                            texu_cio_get_color(dc, selcolor));*/
                        texu_cio_draw_char2(dc, y, x + len, ch,
                            selcolor, selbg, 0,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
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
            if (1)//!(TEXU_ES_APPENDMODE & style)/* || TEXU_ES_PASSWORD & style*/) /* replace mode */
            {
                texu_env_show_cursor(env, TEXU_FALSE);
                edit->editbuf[0] = 0;
                texu_strcpy(buf, TEXUTEXT(" "));
                texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);

#if 0 //defined TEXU_CIO_COLOR_MONO
                texu_cio_putstr_attr(dc, y, x, text,
                                    texu_cio_get_reverse(dc, selcolor));
#else
                texu_cio_draw_text(dc, y, x, text, selcolor, selbg,
                                      texu_wnd_get_clsname(wnd),
                                      texu_wnd_get_id(wnd));
#endif

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
    else if (TEXU_KEY_UP == ch /*|| TEXU_KEY_DOWN == ch*/)
    {
        switch (ch)
        {
        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);
                if (prevwnd)
                {
                    /*move previous means do nothing if there are any changes*/
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                    }
                }
            }
            break;
        }
    }
    else if (TEXU_KEY_DOWN == ch)
    {
        /*un-selected*/
        edit->selected = 0;
        edit->editing = 1;
        edit->firstvisit = 0;
        texu_wnd_invalidate(wnd);
    }
    else if (TEXU_KEY_LEFT == ch)
    {
        /**/
        --edit->xpos;
        if (edit->xpos < 0)
        {
            edit->xpos = 0;
        }
        edit->editing = 0;
        texu_wnd_invalidate(wnd);
    }
    else if (TEXU_KEY_RIGHT == ch)
    {
        /**/
        ++edit->xpos;
        if (edit->xpos >= texu_strlen(edit->editbuf))
        {
            edit->xpos = texu_strlen(edit->editbuf) - 1;
        }
        edit->editing = 0;
        texu_wnd_invalidate(wnd);
    }
    else if (TEXU_KEY_ENTER == ch)
    {
        /*notify its parent in case of it is the last control*/
        {
            texu_wnd *parent = texu_wnd_get_parent(wnd);
            texu_wnd *nextwnd = texu_wnd_get_next_activechild(parent, wnd);
            if (nextwnd)
            {
                /*move previous means do nothing if there are any changes*/
                texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                if (rc >= TEXU_OK)
                {
                    /*texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);*/
                    _TexuEditProc_Notify(wnd, TEXU_EN_PRESSEDENTER, edit->editbuf, rc);
                }
            }
        }
    }
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
    if (edit->limitchars < 1)
    {
        edit->limitchars = 1;
    }
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
    edit->on_validate_data = attrs->validate_data;

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
#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 bgcolor = normbg;
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    texu_ui32 invbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_INVALID);
#endif
    texu_i32 cx = texu_env_screen_width(env);
    /*texu_i32 cy = texu_env_screen_height(env);*/
    texu_i32 xpos = 0;

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }
    if (rect)
    {
        y = rect->y;
        x = rect->x;
        width = rect->cols;
    }
    edit = (texu_editwnd *)texu_wnd_get_userdata(wnd);
    xpos = (edit->xpos);
    
    texu_wnd_get_color(wnd, &normcolor, &discolor);
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);
    bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);

    len = width;
    if ((TEXU_ES_PASSWORD & style) && width > 0)
    {
        len = texu_strlen(edit->editbuf);
        memset(buf, 0, sizeof(buf));
        if (TEXU_TRUE == edit->editing)
        {
            if (len > 0)
            {
                texu_memset(buf, edit->passchar, len);
            }
        }
        else
        {
            len = 8;
            if (len > width)
            {
                len = width;
            }
            if (edit->showpass == TEXU_TRUE)
            {
                texu_memset(buf, edit->passchar, len);
            }
            else
            {
                len = 0;
            }
        }
/*
        if (edit->showpass == TEXU_TRUE)
        {
            if (0 == len)
            {
                len = 8;
            }
            texu_memset(buf, edit->passchar, len);
        }
        else
        {
            texu_memset(buf, TEXUTEXT(' '), len);
        }*/
        buf[len] = 0;

        texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);
        texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
#if 0 //defined TEXU_CIO_COLOR_MONO
        if (edit->selected)
        {
            color = selcolor;
        }
        texu_cio_putstr_attr(cio, y, x, text,
                             texu_cio_get_reverse(cio, color));
#else
        bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);
        if (edit->selected)
        {
            color = selcolor;
            bgcolor = selbg;
        }
        texu_cio_draw_text2(cio, y, x, text, 
            color,
            bgcolor,
#if defined TEXU_CIO_COLOR_MONO
            (edit->selected ? TB_REVERSE : 0),
#else
            0,
#endif
            texu_wnd_get_clsname(wnd),
            texu_wnd_get_id(wnd));
#endif

        if (edit->firstvisit)
        {
            len = 0;
        }
        else
        {
            len = (texu_i32)TEXU_MIN(texu_strlen(buf), (texu_ui32)width);
        }
        texu_cio_gotoyx(cio, y, x + len);
        return;
    }/*password*/

    /*len = TEXU_MIN(texu_strlen(edit->editbuf), width);*/
    memset(buf, 0, sizeof(buf));
    memset(text, 0, sizeof(text));
    if (edit->editing)
    {
        texu_strncpy(buf, &edit->editbuf[edit->firstchar], width);
    }
    else
    {
        texu_strncpy(buf, &edit->editbuf[xpos], width);
    }
    if (0 == texu_strlen(buf))
    {
        texu_strcpy(buf, TEXUTEXT(""));
    }

    if (edit->editing)
    {
        style = TEXU_ALIGN_LEFT;
    }
#if 0
    if ((TEXU_ES_AUTOCOMMAS & style) &&
        ((TEXU_ES_NUMBER & style) || (TEXU_ES_DECIMAL & style)))
    {
        texu_add_commas(textcommas, TEXU_MAX_WNDTEXT, buf);
    }
    else
    {
        texu_strcpy(textcommas, buf);
    }
    /*draw text if the user scroll left/right*/
    /*if (xpos > texu_strlen(textcommas))
    {
        xpos = texu_strlen(textcommas) - 1;
        if (xpos < 0)
        {
            xpos = 0;
        }
        edit->xpos = xpos;
    }*/
    texu_printf_alignment3(text, &textcommas[xpos], width, style, TEXU_FALSE, x, cx);
#else
    texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);
#endif
#if 0 //defined TEXU_CIO_COLOR_MONO
    if (edit->selected)
    {
        color = selcolor;
        if (edit->invalid)
        {
            color = invcolor;
            edit->invalid = TEXU_FALSE;
        }
    }
    texu_cio_putstr_attr(cio, y, x, text,
                            texu_cio_get_reverse(cio, color));
#else
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
    texu_cio_draw_text2(cio, y, x, text, 
        color,
        bgcolor,
#if defined TEXU_CIO_COLOR_MONO
        (edit->selected ? TB_REVERSE : 0),
#else
        0,
#endif
        texu_wnd_get_clsname(wnd),
        texu_wnd_get_id(wnd));

#endif /* TEXU_CIO_COLOR_MONO*/
    len = (texu_i32)TEXU_MIN(texu_strlen(buf), (texu_ui32)width);
    texu_cio_gotoyx(cio, y, x + len);
}


texu_status _TexuEditProc_OnGetInt(texu_wnd *wnd, texu_i32 *out)
{
    texu_status rc = TEXU_OK;
    /*texu_editwnd *edit = 0;*/
    texu_i32 value = 0;
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    if (/*!(texu_wnd_is_enable(wnd)) || */!out)
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

    if (/*!(texu_wnd_is_enable(wnd)) || */!out)
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

#if 1
        case TEXU_WM_KEYDOWN:/*virtual key*/
        {
            /*texu_i32 rc = TexuDefWndProc(wnd, msg, param1, param2);
            if (0 == rc)
            {
                rc = _TexuEditProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            }*/
            return _TexuEditProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
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
            return _TexuEditProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);
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

        case TEXU_EM_SETVALIDATION:
            _TexuEditProc_OnSetValidation(wnd, (texu_validate_proc)param1, (void*)param2);
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
    texu_i32        page_height;
};
typedef struct texu_lbwnd texu_lbwnd;

void _TexuListBoxProc_OnPaint(texu_wnd *, texu_cio *, texu_rect* rect);
texu_status _TexuListBoxProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void _TexuListBoxProc_OnDestroy(texu_wnd *wnd);
void _TexuListBoxProc_OnDeleteItem(texu_wnd *wnd, texu_i32 idx);
texu_i32 _TexuListBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state);
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
texu_i32 _TexuListBoxProc_OnQueryKeyUp(texu_wnd *wnd);

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

void _TexuListBoxProc_OnSetPageHeight(texu_wnd *wnd, texu_i32 page_height)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    if (page_height > 1 && page_height != lb->page_height)
    {
        lb->page_height = page_height;
    }
}

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

texu_i32 _TexuListBoxProc_OnQueryKeyUp(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    if (lb->cursel <= 0)
    {
        return 0;
    }
    return 1; /*still process key up*/
}

texu_i32 _TexuListBoxProc_OnQueryKeyPgUp(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    if (lb->cursel <= 0)
    {
        return 0;
    }
    return 1; /*still process key up*/
}

texu_i32 _TexuListBoxProc_OnQueryKeyPgDown(texu_wnd *wnd)
{
    texu_lbwnd *lb = texu_wnd_get_userdata(wnd);
    if (lb->cursel < 0 || lb->cursel >= (lb->nitems - 1))
    {
        return 0;
    }
    return 1; /*still process key up*/
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
    texu_i32 page_height = lb->page_height;

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
                if (height > 1)
                {
                    lines -= height;
                }
                else
                {
                    lines -= page_height;
                }
                ++repaint;
            }
            break;
        }

        case TEXU_KEY_NPAGE:
        {
            if (cursel < nitems)
            {
                if (height > 1)
                {
                    lines += height;
                }
                else
                {
                    lines += page_height;
                }
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
_TexuListBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state)
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
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }
    if (rect)
    {
        y = rect->y;
        x = rect->x;
        width = rect->cols;
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

#if defined TEXU_CIO_COLOR_MONO
                    texu_cio_draw_text2(cio, y + (i - lb->firstvisible), x, text, color, bgcolor, TB_REVERSE,
                                          texu_wnd_get_clsname(wnd),
                                          texu_wnd_get_id(wnd));
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
#if 0 //defined TEXU_CIO_COLOR_MONO
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
    lb->page_height = 8;
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
#if 0
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
#endif
    _TexuListBoxProc_OnSetItemText(wnd, sel, text);
}

texu_longptr
_TexuListBoxProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_QUERYKEYUP:
            return _TexuListBoxProc_OnQueryKeyUp(wnd);

        case TEXU_WM_QUERYKEYPGUP:
            return _TexuListBoxProc_OnQueryKeyPgUp(wnd);

        case TEXU_WM_QUERYKEYPGDOWN:
            return _TexuListBoxProc_OnQueryKeyPgDown(wnd);

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
            return _TexuListBoxProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

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
        case TEXU_LBM_SETPAGEHEIGHT:
            _TexuListBoxProc_OnSetPageHeight(wnd, (texu_i32)param1);
            return 0;
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
texu_i32 _TexuComboBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state);
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
texu_i32 _TexuComboBoxProc_OnQueryKeyUp(texu_wnd *wnd);

texu_i32 _TexuComboBoxProc_OnQueryKeyUp(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    if (cursel <= 0)
    {
        return 0;
    }
    return 1; /*still process key up*/
}

texu_i32 _TexuComboBoxProc_OnQueryKeyPgUp(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    if (cursel <= 0)
    {
        return 0;
    }
    return 1; /*still process key up*/
}

texu_i32 _TexuComboBoxProc_OnQueryKeyPgDown(texu_wnd *wnd)
{
    texu_cbwnd *cb = (texu_cbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    texu_i32 nitems = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMCOUNT, 0, 0);
    if (cursel < 0 || cursel >= (nitems - 1))
    {
        return 0;
    }
    return 1; /*still process key up*/
}

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
_TexuComboBoxProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd, texu_i32 state)
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
    /*texu_urect rect;*/

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }

    /*texu_wnd_send_msg(wnd, TEXU_WM_GETCLIENTRECT, (texu_lparam)&rect, 0);
    y = rect.r2.y;
    x = rect.r2.x;
    width = rect.r2.width;*/
    /*height = rect.r2.height;*/
    /*texu_wnd_move(cb->lb, y + 1, x, height, width, TEXU_FALSE);*/
    if (TEXU_LB_ERROR == cursel && nitems > 0)
    {
        texu_wnd_send_msg(cb->lb, TEXU_LBM_SETCURSEL, (texu_lparam)cursel, 0);
    }
    texu_wnd_send_msg(cb->lb, TEXU_WM_SETFOCUS, 0, 0);
    texu_wnd_visible(cb->lb, TEXU_TRUE);
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
#if 0
    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
#endif
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
    texu_i32 rc = texu_wnd_send_msg(cb->lb, TEXU_LBM_ADDITEM, (texu_lparam)text, 0);
    texu_i32 cursel = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETCURSEL, 0, 0);
    if (TEXU_LB_ERROR == cursel)
    {
        texu_wnd_send_msg(cb->lb, TEXU_LBM_SETCURSEL, (texu_lparam)0, 0);
    }
    return rc;
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
        case TEXU_KEY_LEFT:
        case TEXU_KEY_RIGHT:
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
            if (TEXU_LB_ERROR == idx)
            {
                return 0;
            }
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
    /*texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_i32 nitems = texu_wnd_send_msg(cb->lb, TEXU_LBM_GETITEMCOUNT, 0, 0);
    texu_i32 height = TEXU_MIN(5, nitems);
    texu_urect rect;*/

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    switch (ch)
    {
        case ' ':
        {
            /*texu_wnd_send_msg(wnd, TEXU_WM_GETCLIENTRECT, (texu_lparam)&rect, 0);
            y = rect.r2.y;
            x = rect.r2.x;
            width = rect.r2.width;*/
            /*height = rect.r2.height;*/
            if (texu_wnd_is_visible(cb->lb))
            {
                /*texu_wnd_move(cb->lb, y + 1, x, 1, width, TEXU_FALSE);*/
                texu_wnd_visible(cb->lb, TEXU_FALSE);
                texu_wnd_set_focus(wnd);
                _TexuComboBoxProc_Notify(wnd, TEXU_CBN_LEAVE_LISTBOX, -1);
            }
            else
            {
                /*texu_wnd_move(cb->lb, y + 1, x, height, width, TEXU_FALSE);*/
                texu_wnd_visible(cb->lb, TEXU_TRUE);
                texu_wnd_set_focus(wnd);
                _TexuComboBoxProc_Notify(wnd, TEXU_CBN_ENTER_LISTBOX, -1);
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
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
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
    texu_printf_alignment3(buf, text, width, TEXU_ALIGN_LEFT, TEXU_TRUE, x, cx);
    if (TEXU_TRUE == cb->focused)
    {
        color = sel;
        bgcolor = selbg;
#if defined TEXU_CIO_COLOR_MONO
        texu_cio_draw_text2(dc, y, x, buf, color, bgcolor, TB_REVERSE,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
        /*texu_cio_draw_char2(dc, y, x + width - 1, expanding, color, bgcolor, TB_REVERSE,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));*/
#else
        texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
        /*texu_cio_draw_char(dc, y, x + width - 1, expanding, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));*/
#endif
    }
    else
    {
        texu_cio_draw_text(dc, y, x, buf, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));
        /*texu_cio_draw_char(dc, y, x + width - 1, expanding, color, bgcolor,
                            texu_wnd_get_clsname(wnd),
                            texu_wnd_get_id(wnd));*/
    }
    texu_cio_gotoyx(dc, y, x + width);
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
    //texu_i32 xparent = texu_wnd_get_x(parent);
    texu_i32 hparent = texu_wnd_get_height(parent);
    /*texu_i32 wparent = texu_wnd_get_width(parent);*/
    texu_i32 color = texu_env_get_syscolor(env,   TEXU_COLOR_COMBOBOX);
    texu_i32 discolor = texu_env_get_syscolor(env,   TEXU_COLOR_COMBOBOX_DISABLED);
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env,   TEXU_COLOR_COMBOBOX);
    texu_i32 disbg = texu_env_get_sysbgcolor(env,   TEXU_COLOR_COMBOBOX_DISABLED);

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
    attrs2.y                = 0;
    attrs2.x                = 0;/*attrs->x;*/
    attrs2.height           = 1;/*attrs->height;*/
    attrs2.width            = attrs->width;
    attrs2.enable           = TEXU_TRUE;
    attrs2.visible          = TEXU_TRUE;
    attrs2.text             = attrs->text;
    attrs2.normalcolor      = texu_env_get_syscolor(env,   TEXU_COLOR_LISTBOX);
    attrs2.disabledcolor    = texu_env_get_syscolor(env,   TEXU_COLOR_LISTBOX_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env,   TEXU_COLOR_LISTBOX_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env,   TEXU_COLOR_LISTBOX_FOCUSED);
    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_DISABLED);
    attrs2.selectedbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_LISTBOX_FOCUSED);

    attrs2.id = texu_env_get_wnd_id(env);
    attrs2.clsname = TEXU_LISTBOX_CLASS;
    attrs2.userdata = 0;
    attrs2.style = 0;/*TEXU_WS_POPUP;*/
    attrs2.exstyle = 0;

    rc = texu_wnd_create(lb, wnd/*parent*/, &attrs2);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(lb);
        return TEXU_ERROR;
    }

    texu_wnd_set_color(wnd, color, discolor);
    texu_wnd_set_bgcolor(wnd, bgcolor, disbg);
    /*texu_wnd_set_focused_color(wnd, attrs2.focusedcolor);
    texu_wnd_set_bgfocused_color(wnd, attrs2.focusedbg);
    texu_wnd_set_selected_color(wnd, attrs2.selectedcolor);
    texu_wnd_set_bgselected_color(wnd, attrs2.selectedbg);*/

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
        case TEXU_WM_QUERYKEYUP:
            return _TexuComboBoxProc_OnQueryKeyUp(wnd);

        case TEXU_WM_QUERYKEYPGUP:
            return _TexuComboBoxProc_OnQueryKeyPgUp(wnd);

        case TEXU_WM_QUERYKEYPGDOWN:
            return _TexuComboBoxProc_OnQueryKeyPgDown(wnd);

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
            return _TexuComboBoxProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

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
    texu_rect rect; /*extended to inform its area*/
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
texu_i32 _TexuStatusBarProc_OnAddPartRect(texu_wnd *, const texu_char *, const texu_rect* rect);
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
texu_i32 _TexuStatusBarProc_OnGetPartRect(texu_wnd *wnd, texu_i32 id, texu_rect* rect);

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

texu_i32 _TexuStatusBarProc_OnGetPartRect(texu_wnd *wnd, texu_i32 id, texu_rect* rect)
{
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_sbwnd_part *part = _texu_sbwnd_part_find(sb->parts, id);
    if (part && rect)
    {
        *rect = part->rect;
    }
    else
    {
        return -1;
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
    /*texu_sbwnd_part *lastpart = (texu_sbwnd_part *)texu_list_last(sb->parts);*/
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
    part->rect.cols = part->width;

    return nparts;
}

texu_i32
_TexuStatusBarProc_OnAddPartRect(texu_wnd *wnd, const texu_char *text, const texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_list_item *item = 0;
    texu_sbwnd *sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);
    texu_i32 nparts = texu_list_count(sb->parts);
    texu_sbwnd_part *firstpart = 0;
    texu_sbwnd_part *part = _texu_sbwnd_part_new(text, rect->cols,
                                                 texu_env_get_syscolor(env, TEXU_COLOR_STATUSBAR),
                                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_STATUSBAR),
                                                 TEXU_ALIGN_LEFT);

    part->id = nparts;
    part->rect = *rect;
    texu_list_add(sb->parts, (texu_lparam)part);

    item = texu_list_first(sb->parts);
    firstpart = (texu_sbwnd_part *)item->data;
    firstpart->width -= rect->cols;
    if (firstpart->width < 0)
    {
        firstpart->width = 0;
    }
    firstpart->rect.cols = firstpart->width;
    /*update the new part some fields*/
    part->rect.y = firstpart->rect.y;

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
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }
    if (rect)
    {
        y = rect->y;
        x = rect->x;
        /*width = rect->cols;*/
    }

    sb = (texu_sbwnd *)texu_wnd_get_userdata(wnd);

    item = texu_list_first(sb->parts);
    while (item)
    {
        part = (texu_sbwnd_part *)item->data;
        memset(filler, 0, sizeof(filler));
        memset(filler, ' ', part->width);
        texu_printf_alignment3(buf, filler, part->width, TEXU_ALIGN_RIGHT, TEXU_TRUE, x, cx);
#if defined TEXU_CIO_COLOR_MONO
        /*texu_cio_putstr_attr(dc, y, x, filler, texu_cio_get_reverse(dc, part->color));
        texu_cio_putstr_attr(dc, y, x, buf,
                             texu_cio_get_reverse(dc, part->color));*/
        texu_cio_draw_text2(dc, y, x, buf, part->color, part->bgcolor, TB_REVERSE,
                              texu_wnd_get_clsname(wnd),
                              texu_wnd_get_id(wnd));
#else
        texu_cio_draw_text(dc, y, x, buf, part->color, part->bgcolor,
                              texu_wnd_get_clsname(wnd),
                              texu_wnd_get_id(wnd));
#endif
        memset(buf, 0, sizeof(buf));
        texu_printf_alignment3(buf, part->text, part->width, part->align, TEXU_TRUE, x, cx);
#if defined TEXU_CIO_COLOR_MONO
        /*texu_cio_putstr_attr(dc, y, x, filler, texu_cio_get_reverse(dc, part->color));
        texu_cio_putstr_attr(dc, y, x, buf,
                             texu_cio_get_reverse(dc, part->color));*/
        texu_cio_draw_text2(dc, y, x, buf, part->color, part->bgcolor, TB_REVERSE,
                              texu_wnd_get_clsname(wnd),
                              texu_wnd_get_id(wnd));
#else
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
    sb->firstpart->rect.x = attrs->x;
    sb->firstpart->rect.y = attrs->y;
    sb->firstpart->rect.cols = attrs->width;
    sb->firstpart->rect.lines = attrs->height;

    texu_list_add(sb->parts, (texu_lparam)sb->firstpart);

    texu_wnd_set_color(wnd, color, color);
    texu_wnd_set_bgcolor(wnd, color, color);
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

    /*case TEXU_WM_ERASEBG:
        _TexuStatusBarProc_OnEraseBg(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;*/

    case TEXU_WM_PAINT:
        _TexuStatusBarProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
        return 0;

    case TEXU_WM_ENABLE:
        return TexuDefWndProc(wnd, msg, 0, 0);

    case TEXU_SBM_GETPARTCOUNT:
        return _TexuStatusBarProc_OnGetPartCount(wnd);

    case TEXU_SBM_ADDPART:
        return _TexuStatusBarProc_OnAddPart(wnd, (const texu_char *)param1, (texu_i32)param2);

    case TEXU_SBM_ADDPARTRECT:
        return _TexuStatusBarProc_OnAddPartRect(wnd, (const texu_char *)param1, (const texu_rect*)param2);

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

    case TEXU_SBM_GETPARTRECT:
        return _TexuStatusBarProc_OnGetPartRect(wnd, (texu_i32)param1, (texu_rect*)param2);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif