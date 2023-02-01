/*
#
# File Name: texuctrlx.c
# Author: Seree Rakwong
# Date: 27-JAN-2023
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
# TexU IP Address ctrl
#
1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_ipaddr
{
    texu_wnd *editwnd;
    texu_wnd *editwnd2;
    texu_wnd *editwnd3;
    texu_wnd *editwnd4;
    texu_i32 step;
    texu_i32 page;
    texu_i32 min;
    texu_i32 max;
    void *exparam;
};
typedef struct texu_ipaddr texu_ipaddr;

void _TexuIPAdressProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuIPAdressProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuIPAdressProc_OnDestroy(texu_wnd *wnd);
void _TexuIPAdressProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuIPAdressProc_OnKillFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuIPAdressProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuIPAdressProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void _TexuIPAdressProc_OnSetMinMax(texu_wnd *wnd, texu_i32, texu_i32);
texu_status _TexuIPAdressProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *, texu_i32 *);
void _TexuIPAdressProc_OnSetStep(texu_wnd *wnd, texu_i32);
texu_i32 _TexuIPAdressProc_OnGetStep(texu_wnd *wnd);
void _TexuIPAdressProc_OnStep(texu_wnd *wnd, texu_i32);
void _TexuIPAdressProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
void _TexuIPAdressProc_OnSetPage(texu_wnd *wnd, texu_i32);
texu_i32 _TexuIPAdressProc_OnGetPage(texu_wnd *wnd);
void _TexuIPAdressProc_OnPage(texu_wnd *wnd, texu_i32);

void
_TexuIPAdressProc_OnSetMinMax(texu_wnd *wnd, texu_i32 min, texu_i32 max)
{
    texu_ipaddr *ipctl = 0;
    texu_editminmax minmax;

    min = TEXU_MAX(0, min);
    max = TEXU_MIN(255, max);

    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    ipctl->min = min;
    ipctl->max = max;

    minmax.min = min;
    minmax.max = max;
    texu_wnd_send_msg(ipctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd2, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd3, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd4, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
}

texu_status
_TexuIPAdressProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *min, texu_i32 *max)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    *min = ipctl->min;
    *max = ipctl->max;
    return TEXU_OK;
}

void
_TexuIPAdressProc_OnSetStep(texu_wnd *wnd, texu_i32 step)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    step = TEXU_MAX(ipctl->min, step);
    step = TEXU_MIN(ipctl->max, step);
    ipctl->step = step;
}

texu_i32
_TexuIPAdressProc_OnGetStep(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    return ipctl->step;
}

void
_TexuIPAdressProc_OnStep(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuIPAdressProc_OnChar(wnd, TEXU_KEY_UP, 0);
    }
    else
    {
        _TexuIPAdressProc_OnChar(wnd, TEXU_KEY_DOWN, 0);
    }
}

void
_TexuIPAdressProc_OnSetPage(texu_wnd *wnd, texu_i32 page)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    page = TEXU_MAX(ipctl->min, page);
    page = TEXU_MIN(ipctl->max, page);
    ipctl->page = page;
}

texu_i32
_TexuIPAdressProc_OnGetPage(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    return ipctl->page;
}

void
_TexuIPAdressProc_OnPage(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuIPAdressProc_OnChar(wnd, TEXU_KEY_NPAGE, 0);
    }
    else
    {
        _TexuIPAdressProc_OnChar(wnd, TEXU_KEY_PPAGE, 0);
    }
}

texu_status
_TexuIPAdressProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_ipaddr *ipctl = 0;
    texu_wnd_attrs attrs2;
    texu_wnd *editwnd = 0;
    texu_wnd *editwnd2 = 0;
    texu_wnd *editwnd3 = 0;
    texu_wnd *editwnd4 = 0;
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
    attrs2.x = attrs->x;
    attrs2.height = attrs->height;
    attrs2.width = 3;/* attrs->width - 1;*/
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = TEXT("127");
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED);

    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED);

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
    /*second*/
    editwnd2 = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd2)
    {
        texu_wnd_del(editwnd);
        return TEXU_NOMEM;
    }
    attrs2.x += 4;
    attrs2.id = 2;
    attrs2.text = TEXT("0");
    rc = texu_wnd_create(editwnd2, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        return TEXU_ERROR;
    }
    /*third*/
    editwnd3 = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd3)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        return TEXU_NOMEM;
    }
    attrs2.x += 4;
    attrs2.id = 3;
    attrs2.text = TEXT("0");
    rc = texu_wnd_create(editwnd3, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        texu_wnd_del(editwnd3);
        return TEXU_ERROR;
    }
    /*third*/
    editwnd4 = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd4)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        texu_wnd_del(editwnd3);
        return TEXU_NOMEM;
    }
    attrs2.x += 4;
    attrs2.id = 4;
    attrs2.text = TEXT("1");
    rc = texu_wnd_create(editwnd4, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        texu_wnd_del(editwnd3);
        texu_wnd_del(editwnd4);
        return TEXU_ERROR;
    }

    ipctl = (texu_ipaddr *)malloc(sizeof(texu_ipaddr));
    if (!ipctl)
    {
        texu_wnd_del(editwnd);
        return TEXU_NOMEM;
    }

    memset(ipctl, 0, sizeof(texu_ipaddr));
    ipctl->editwnd = editwnd; /* no parameter */
    ipctl->step = 1;
    ipctl->page = 16;
    ipctl->min = 0;
    ipctl->max = 255;

    minmax.min = ipctl->min;
    minmax.max = ipctl->max;

    ipctl->editwnd  = editwnd;
    ipctl->editwnd2 = editwnd2;
    ipctl->editwnd3 = editwnd3;
    ipctl->editwnd4 = editwnd4;

    texu_wnd_send_msg(ipctl->editwnd,  TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd2, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd3, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);
    texu_wnd_send_msg(ipctl->editwnd4, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_i64)&minmax);

    /* save memory */
    texu_wnd_set_userdata(wnd, ipctl);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED));

    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED));

    return TEXU_OK;
}

void
_TexuIPAdressProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_ipaddr *ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    /*set focust to the first edit window*/
    texu_wnd_send_msg(ipctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_IPN_SETFOCUS);
}

texu_i32
_TexuIPAdressProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    /*update value to window text */
    texu_ipaddr *ipctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    texu_ip_addr ip = { 0, 0, 0, 0 };

    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);

    texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, 0);

    /*texu_wnd_get_text(ipctl->editwnd, buf, TEXU_MAX_WNDTEXT);*/
    texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_i64)&ip, 0);

    texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
    texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/

    _TexuWndProc_Notify(wnd, TEXU_IPN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_CONTINUE;
}

void
_TexuIPAdressProc_OnDestroy(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    free(ipctl);
}

texu_i64
_TexuIPAdressProc_OnGetIPAddress(texu_wnd *wnd, texu_ip_addr* ip)
{
    texu_ipaddr *ipctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(ipctl->editwnd, buf, TEXU_MAX_WNDTEXT);
    ip->_1st = texu_atol(buf);
    /*second*/
    texu_wnd_get_text(ipctl->editwnd2, buf, TEXU_MAX_WNDTEXT);
    ip->_2nd = texu_atol(buf);
    /*third*/
    texu_wnd_get_text(ipctl->editwnd3, buf, TEXU_MAX_WNDTEXT);
    ip->_3rd = texu_atol(buf);
    /*fourth*/
    texu_wnd_get_text(ipctl->editwnd4, buf, TEXU_MAX_WNDTEXT);
    ip->_4th = texu_atol(buf);

    return ((ip->_1st & 0xff) << 24 |
            (ip->_2nd & 0xff) << 16 |
            (ip->_3rd & 0xff) <<  8 |
            (ip->_4th & 0xff));
}

void
_TexuIPAdressProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_ipaddr *ipctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 val = 0;
    texu_bool updown = TEXU_FALSE;
    texu_i32 added = 0;
    texu_wnd *editwnd = 0;
    texu_wnd *nextwnd = 0;
    texu_ip_addr ip;
    texu_status rc = TEXU_OK;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    editwnd = texu_wnd_get_activechild(wnd);
    switch (ch)
    {
        case TEXU_KEY_UP:
        {
            added += ipctl->step;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_DOWN:
        {
            added -= ipctl->step;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_NPAGE:
        {
            added -= ipctl->page;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_PPAGE:
        {
            added += ipctl->page;
            updown = TEXU_TRUE;
            break;
        }

        case TEXU_KEY_LEFT:
        {
            /*move to the prev edit window*/
            nextwnd = texu_wnd_get_prev_activechild(wnd, editwnd);
            if (!nextwnd)
            {
                return;
            }
            else
            {
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (TEXU_OK == rc)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)editwnd, 0);
                }
                return;
            }
            break;
        }

        case TEXU_KEY_RIGHT:
        case TEXUTEXT('.'):
        {
            /*move to the next edit window*/
            nextwnd = texu_wnd_get_next_activechild(wnd, editwnd);
            if (!nextwnd)
            {
                return;
            }
            else
            {
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (TEXU_OK == rc)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)editwnd, 0);
                }
                return;
            }
            break;
        }
    }

    editwnd = texu_wnd_get_activechild(wnd);
    if (updown)
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, 0); /*make sure the last buffer was safed*/

        texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT); /*get the last buffer*/

        val = texu_atol(buf);   /*convert text to integer*/
        val += added;
        if (val < ipctl->min)
        {
            val = ipctl->min;
        }
        if (val > ipctl->max)
        {
            val = ipctl->max;
        }

        _TexuWndProc_Notify(wnd, TEXU_IPN_STEP);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
        texu_wnd_set_text(editwnd, buf);
        texu_wnd_send_msg(editwnd, TEXU_WM_SETFOCUS, 0, 0);

        texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_i64)&ip, 0);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th); 
        texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/
    }
    else
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)ch, 0);
    }
}

texu_bool
_TexuIPAdressProc_IsValidIP(texu_wnd *wnd, const texu_char *text, texu_ip_addr *ip)
{
    texu_bool rc = TEXU_TRUE;
    texu_i32 _1st = 0;
    texu_i32 _2nd = 0;
    texu_i32 _3rd = 0;
    texu_i32 _4th = 0;

#if (defined WIN32 && defined UNICODE)
    rc = swscanf_s(text, TEXUTEXT("%d.%d.%d.%d"), &_1st, &_2nd, &_3rd, &_4th);
#else
    rc = sscanf(text, TEXUTEXT("%d.%d.%d.%d"), &_1st, &_2nd, &_3rd, &_4th);
#endif
    if (rc > 0)
    {
        _1st = TEXU_MIN(255, TEXU_MAX(0, _1st));
        _2nd = TEXU_MIN(255, TEXU_MAX(0, _2nd));
        _3rd = TEXU_MIN(255, TEXU_MAX(0, _3rd));
        _4th = TEXU_MIN(255, TEXU_MAX(0, _4th));
        ip->_1st = _1st;
        ip->_2nd = _2nd;
        ip->_3rd = _3rd;
        ip->_4th = _4th;
    }
    else
    {
        rc = TEXU_FALSE;
    }

    return rc;
}

void
_TexuIPAdressProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_ipaddr *ipctl = 0;
    texu_char buf[8];
    texu_char buf2[8];
    texu_char buf3[8];
    texu_char buf4[8];
    texu_ip_addr ip = { 0, 0, 0, 0 };

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);

    /*validate all parts*/
    if (TEXU_FALSE == _TexuIPAdressProc_IsValidIP(wnd, text, &ip))
    {
        return;
    }
    /*split to 4 parts*/

    texu_sprintf(buf,  sizeof(buf),  TEXUTEXT("%d"), ip._1st);
    texu_sprintf(buf2, sizeof(buf2), TEXUTEXT("%d"), ip._2nd);
    texu_sprintf(buf3, sizeof(buf3), TEXUTEXT("%d"), ip._3rd);
    texu_sprintf(buf4, sizeof(buf4), TEXUTEXT("%d"), ip._4th);

    texu_wnd_set_text(ipctl->editwnd, buf);
    texu_wnd_set_text(ipctl->editwnd2, buf2);
    texu_wnd_set_text(ipctl->editwnd3, buf3);
    texu_wnd_set_text(ipctl->editwnd4, buf4);
    /*default set text*/
    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)buf, 0);
}

void
_TexuIPAdressProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    static texu_char ipdots[] = TEXUTEXT("   .   .   .   ");
    texu_i32 color = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL);

    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL);

    if (TEXU_FALSE == texu_wnd_is_visible(wnd))
    {
        return;
    }
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(dc, y, x, ipdots,
                        texu_cio_get_color(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, y, x, ipdots, color, bgcolor);
#else
    texu_cio_putstr_attr(dc, y, x, ipdots,
                        texu_cio_get_color(dc, color));
#endif
#endif
}

texu_i64
_TexuIPAdressProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_CHAR:
            _TexuIPAdressProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuIPAdressProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuIPAdressProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuIPAdressProc_OnDestroy(wnd);
            break;

        case TEXU_WM_SETFOCUS:
            _TexuIPAdressProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuIPAdressProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_SETTEXT:
            _TexuIPAdressProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_IPM_SETMINMAX:
        {
            _TexuIPAdressProc_OnSetMinMax(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;
        }
        case TEXU_IPM_GETMINMAX:
        {
            return _TexuIPAdressProc_OnGetMinMax(wnd, (texu_i32 *)param1, (texu_i32 *)param2);
        }
        case TEXU_IPM_SETSTEP:
        {
            _TexuIPAdressProc_OnSetStep(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETSTEP:
        {
            return _TexuIPAdressProc_OnGetStep(wnd);
        }
        case TEXU_IPM_STEP:
        {
            _TexuIPAdressProc_OnStep(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_SETPAGE:
        {
            _TexuIPAdressProc_OnSetPage(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETPAGE:
        {
            return _TexuIPAdressProc_OnGetPage(wnd);
        }
        case TEXU_IPM_PAGE:
        {
            _TexuIPAdressProc_OnPage(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETIPADDRESS:
        {
            if (0 == param1)
            {
                return 0;
            }
            return _TexuIPAdressProc_OnGetIPAddress(wnd, (texu_ip_addr*)param1);
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


/*
#-------------------------------------------------------------------------------
# TexU Edit Mask Ctrl
#
1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#if (defined SLRE)
#include "slre.h"
#else
#include "re.h"
#endif

struct texu_editmask
{
    texu_wnd *editwnd;
    char pattern[TEXU_MAX_BUFFER + 1];/*2023-01-27: re.h supports only ascii*/
    texu_char editbuf[TEXU_MAX_WNDTEXT + 1];
    texu_char infobuf[TEXU_MAX_WNDTEXT + 1];
    void *exparam;
};
typedef struct texu_editmask texu_editmask;

void _TexuEditMaskProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuEditMaskProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuEditMaskProc_OnDestroy(texu_wnd *wnd);
void _TexuEditMaskProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuEditMaskProc_OnKillFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuEditMaskProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuEditMaskProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void _TexuEditMaskProc_OnSetMask(texu_wnd *wnd, const texu_char *mask);
texu_i32 _TexuEditMaskProc_OnGetMask(texu_wnd *wnd, texu_char *mask, texu_i32 masklen);
void _TexuEditMaskProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_bool   _TexuEditMaskProc_IsValidString(texu_wnd *wnd, const texu_char *str);
void _TexuEditMaskProc_OnSetInfo(texu_wnd *wnd, const texu_char *text);


texu_status
_TexuEditMaskProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_editmask *emctl = 0;
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
    attrs2.y = attrs->y;
    attrs2.x = attrs->x;
    attrs2.height = attrs->height;
    attrs2.width = attrs->width;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = attrs->text;
    attrs2.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL);
    attrs2.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_SELECTED);
    attrs2.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_FOCUSED);

    attrs2.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL);
    attrs2.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_SELECTED);
    attrs2.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_FOCUSED);

    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_LEFT;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }

    emctl = (texu_editmask *)malloc(sizeof(texu_editmask));
    if (!emctl)
    {
        return TEXU_ERROR;
    }
    memset(emctl, 0, sizeof(texu_editmask));
    emctl->editwnd = editwnd; /* no parameter */

    texu_wnd_visible(editwnd, TEXU_FALSE);
    /* save memory */
    texu_wnd_set_userdata(wnd, emctl);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_FOCUSED));

    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_FOCUSED));

    return TEXU_OK;
}

void
_TexuEditMaskProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_editmask *emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    /*set focust to the first edit window*/
    texu_wnd_visible(emctl->editwnd, TEXU_TRUE);
    texu_wnd_send_msg(emctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_EMN_SETFOCUS);
}

texu_i32
_TexuEditMaskProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    /*update value to window text */
    texu_editmask *emctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    texu_bool valid = TEXU_TRUE;

    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(wnd, emctl->editbuf, TEXU_MAX_WNDTEXT);

    texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, 0);
    texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
    valid = _TexuEditMaskProc_IsValidString(wnd, buf);
    if (TEXU_FALSE == valid)
    {
        _TexuWndProc_Notify(wnd, TEXU_EMN_INVALIDEXPR);

        /*change edit color*/
        texu_wnd_send_msg(emctl->editwnd, TEXU_EM_INVALIDATE, 0, 0);
        texu_wnd_invalidate(emctl->editwnd);
        return TEXU_ERROR;
    }

    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_EMN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    texu_wnd_visible(emctl->editwnd, TEXU_FALSE);
    texu_wnd_invalidate(wnd);
    return TEXU_CONTINUE;
}

void
_TexuEditMaskProc_OnDestroy(texu_wnd *wnd)
{
    texu_editmask *emctl = 0;
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    free(emctl);
}

void _TexuEditMaskProc_OnPaint(texu_wnd *wnd, texu_cio *cio)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_editmask *emctl = 0;
    texu_i64 len = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_SELECTED);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_SELECTED);
    texu_ui32 bgcolor = normbg;

    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);

    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    texu_wnd_get_text(wnd, buf, TEXU_MAX_WNDTEXT);
    if (texu_strlen(buf) == 0)
    {
        texu_strcpy(buf, emctl->infobuf);
    }
    texu_printf_alignment(text, buf, width, style);

#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(cio, y, x, text,
                         texu_cio_get_reverse(cio, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, y, x, text, color, bgcolor);
#else
    texu_cio_putstr_attr(cio, y, x, text,
                         texu_cio_get_color(cio, color));
#endif
#endif /* TEXU_CIO_COLOR_MONO*/
}

texu_bool
_TexuEditMaskProc_IsValidString(texu_wnd *wnd, const texu_char *str)
{
    texu_bool valid = TEXU_TRUE;
    texu_editmask *emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    texu_i32 matched = 0;
    char expr[TEXU_MAX_WNDTEXT + 1] = "";
    texu_i32 len = 0;

    if (!str || 0 == texu_strlen(str) || 0 == strlen(emctl->pattern))
    {
        return valid;
    }
    /*2023-01-27: re.h supports only ascii pattern*/
#if (defined UNICODE)
    texu_w2a(expr, TEXU_MAX_WNDTEXT, str, TEXU_MAX_WNDTEXT);
#else
    strcpy(expr, str);
#endif
    len = strlen(expr);
#if (defined SLRE)
    matched = slre_match(emctl->pattern, expr, len, NULL, 0, 0);
    valid = (matched > 0);
#else
    matched = re_match(emctl->pattern, expr, &len);
    valid = (matched != -1);
#endif
    return valid;
}

void
_TexuEditMaskProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_editmask *emctl = 0;
    texu_wnd *editwnd = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    editwnd = texu_wnd_get_activechild(wnd);

    if (TEXU_KEY_ESCAPE == ch)
    {
        /*restore the old valid text*/
        texu_wnd_get_text(wnd, emctl->editbuf, TEXU_MAX_WNDTEXT);
        texu_wnd_set_text(editwnd, emctl->editbuf);
        /*simulate CTRL+A (select all)*/
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
    }
    texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)ch, 0);
}

texu_i32
_TexuEditMaskProc_OnGetMask(texu_wnd *wnd, texu_char *mask, texu_i32 len)
{
    texu_editmask *emctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return -1;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    if (!mask)
    {
        return -2;
    }
    else
    {
#if (defined UNICODE)
        texu_a2w(mask, TEXU_MAX_WNDTEXT, emctl->pattern, TEXU_MAX_WNDTEXT);
#else
        strcpy(mask, emctl->pattern);
#endif
    }
    return texu_strlen(mask);
}

void
_TexuEditMaskProc_OnSetMask(texu_wnd *wnd, const texu_char *mask)
{
    texu_editmask *emctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    if (!mask || texu_strlen(mask) == 0)
    {
        memset(emctl->pattern, 0, sizeof(emctl->pattern));
    }
    else
    {
#if (defined UNICODE)
        texu_w2a(emctl->pattern, TEXU_MAX_WNDTEXT, mask, TEXU_MAX_WNDTEXT);
#else
        strcpy(emctl->pattern, mask);
#endif
    }
}

void _TexuEditMaskProc_OnSetInfo(texu_wnd *wnd, const texu_char *text)
{
    texu_editmask *emctl = 0;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    texu_strcpy(emctl->infobuf, text);
}

void
_TexuEditMaskProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_editmask *emctl = 0;
    texu_bool valid = TEXU_TRUE;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    /*validate using regular expression*/
    valid = _TexuEditMaskProc_IsValidString(wnd, text);
    if (TEXU_FALSE == valid)
    {
        return;
    }
    texu_wnd_set_text(emctl->editwnd, text);

    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)text, 0);
}


texu_i64
_TexuEditMaskProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_CHAR:
            _TexuEditMaskProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuEditMaskProc_OnCreate(wnd, (texu_wnd_attrs *)param1);


        case TEXU_WM_DESTROY:
            _TexuEditMaskProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuEditMaskProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_SETFOCUS:
            _TexuEditMaskProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuEditMaskProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_SETTEXT:
            _TexuEditMaskProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EMM_SETMASK:
            _TexuEditMaskProc_OnSetMask(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EMM_GETMASK:
            return _TexuEditMaskProc_OnGetMask(wnd, (texu_char *)param1, param2);

        case TEXU_EMM_SETINFO:
            _TexuEditMaskProc_OnSetInfo(wnd, (const texu_char *)param1);
            return 0;

    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


/*
#-------------------------------------------------------------------------------
# TexU Edit Price Spread Ctrl
#
1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#if 0
struct texu_pricespread
{
    texu_f64    min;    /*e.g. 0.01*/
    texu_f64    max;    /*e.g. 0.99*/
    texu_f64    spread; /*e.g. 0.01*/
};
typedef struct texu_pricespread texu_pricespread;
#endif

struct texu_edit_pricespread
{
    texu_wnd   *editwnd;
    texu_char   editbuf[TEXU_MAX_WNDTEXT + 1];
    texu_map   *prices; /*hold texu_pricespread*/
    texu_baseprice    baseprice; /*spread = base price, min = base price x (1-x%), max = base price x (1+x%)*/
    texu_bool   allowed_invalid_ceiling_floor;
    texu_i32    decwidth;
    texu_bool   correct_floor_ceiling;
    void        *exparam;
};
typedef struct texu_edit_pricespread texu_edit_pricespread;

void        _TexuEditPriceSpreadProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuEditPriceSpreadProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void        _TexuEditPriceSpreadProc_OnDestroy(texu_wnd *wnd);
void        _TexuEditPriceSpreadProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32    _TexuEditPriceSpreadProc_OnKillFocus(texu_wnd *, texu_wnd *);
texu_i32    _TexuEditPriceSpreadProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuEditPriceSpreadProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void        _TexuEditPriceSpreadProc_OnPaint(texu_wnd *wnd, texu_cio *dc);
texu_bool   _TexuEditPriceSpreadProc_IsValidSpread(texu_wnd *wnd, const texu_char *str);
texu_i32    _TexuEditPriceSpreadProc_CmpSpread(const void *v1, const void *v2);
void        _TexuEditPriceSpreadProc_FreeSpread(texu_i64 key, texu_i64 value, void* user);
/*
texu_pricespread*    _TexuEditPriceSpreadProc_NewSpread(texu_i32 spread, texu_i32 min, texu_i32 max, texu_i32 multiplier);
*/
void        _TexuEditPriceSpreadProc_OnInsertSpread(texu_wnd *wnd, texu_i32 spread, const texu_pricespread *ps);
void        _TexuEditPriceSpreadProc_OnDeleteSpread(texu_wnd *wnd, texu_i32 spread);
texu_status _TexuEditPriceSpreadProc_OnSetBaseSpread(texu_wnd *wnd, const texu_baseprice *ps);
texu_status _TexuEditPriceSpreadProc_OnGetBaseSpread(texu_wnd *wnd, texu_baseprice *ps);
void        _TexuEditPriceSpreadProc_OnSetPriceDecimal(texu_wnd *wnd, texu_i32 decimal);
void        _TexuEditPriceSpreadProc_OnLoadDefaultSpreads(texu_wnd *wnd);
void        _TexuEditPriceSpreadProc_OnLoadSpreads(texu_wnd *wnd, const texu_pricespread *prices, texu_i32 max_prices);
texu_bool   _TexuEditPriceSpreadProc_GetBestSpread(texu_wnd *wnd, const texu_char *str, texu_pricespread* prcspread);
void        _TexuEditPriceSpreadProc_OnAllowOverCeilingFloor(texu_wnd *wnd, texu_bool fAllow);
void        _TexuEditPriceSpreadProc_OnCorrectCeilingFloor(texu_wnd *wnd, texu_bool fAllow);
texu_i32    _TexuEditPriceSpreadProc_GetNearestPrice(texu_wnd *wnd, texu_char *nearest, texu_i32 len, const texu_char* price, texu_i32 updown);

texu_status
_TexuEditPriceSpreadProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_edit_pricespread *eps = 0;
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
    attrs2.y = attrs->y;
    attrs2.x = attrs->x;
    attrs2.height = attrs->height;
    attrs2.width = attrs->width;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = attrs->text;
    attrs2.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    attrs2.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED);
    attrs2.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED);

    attrs2.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    attrs2.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED);
    attrs2.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED);

    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_LEFT | TEXU_ES_DECIMAL;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd, TEXU_EM_SETDECWIDTH, 2, 0); /*2-decimal points*/

    eps = (texu_edit_pricespread *)malloc(sizeof(texu_edit_pricespread));
    if (!eps)
    {
        return TEXU_ERROR;
    }
    memset(eps, 0, sizeof(texu_edit_pricespread));
    eps->editwnd = editwnd; /* no parameter */
    texu_wnd_visible(editwnd, TEXU_FALSE);

    eps->baseprice.price        = 5000; /*use to indicate the price*/
    eps->baseprice.ceiling      = 10000;
    eps->baseprice.floor        = 1;
    eps->baseprice.multiplier   = 100;

    eps->prices = texu_map_new();

    eps->decwidth = 2;  /*2-decimal points*/
    /* save memory */
    texu_wnd_set_userdata(wnd, eps);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED));

    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED));

    return TEXU_OK;
}

void
_TexuEditPriceSpreadProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    /*set focust to the first edit window*/
    texu_wnd_visible(eps->editwnd, TEXU_TRUE);
    texu_wnd_send_msg(eps->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_EPSN_SETFOCUS);
}

texu_i32
_TexuEditPriceSpreadProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    /*update value to window text */
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    texu_bool valid = TEXU_TRUE;
    texu_char longval[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("0");
    texu_i32 val = texu_atol(longval);
    texu_i32 ceiling = 0;
    texu_i32 floor = 0;
    texu_i32 decwidth = eps->decwidth;

    texu_wnd_get_text(wnd, eps->editbuf, TEXU_MAX_WNDTEXT);

    texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, 0);
    texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
    valid = _TexuEditPriceSpreadProc_IsValidSpread(wnd, buf);
    if (TEXU_FALSE == valid)
    {
        _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDSPREAD);

        /*change edit color*/
        texu_wnd_send_msg(eps->editwnd, TEXU_EM_INVALIDATE, 0, 0);
        texu_wnd_invalidate(eps->editwnd);
        return TEXU_ERROR;
    }

    /* check ceiling and floor */
    texu_fs2ls(buf, texu_strlen(buf), decwidth, longval);
    val = texu_atol(longval);

    ceiling = eps->baseprice.ceiling;
    floor = eps->baseprice.floor;
    if (val < floor)
    {
        _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDFLOOR);
        if (!eps->allowed_invalid_ceiling_floor)
        {
            /*change edit color*/
            texu_wnd_send_msg(eps->editwnd, TEXU_EM_INVALIDATE, 0, 0);
            texu_wnd_invalidate(eps->editwnd);
            return TEXU_ERROR;
        }
    }
    else if (val > ceiling)
    {
        _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDCEILING);
        if (!eps->allowed_invalid_ceiling_floor)
        {
            /*change edit color*/
            texu_wnd_send_msg(eps->editwnd, TEXU_EM_INVALIDATE, 0, 0);
            texu_wnd_invalidate(eps->editwnd);
            return TEXU_ERROR;
        }
    }

    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_EPSN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    texu_wnd_visible(eps->editwnd, TEXU_FALSE);
    texu_wnd_invalidate(wnd);
    return TEXU_CONTINUE;
}

texu_i32
_TexuEditPriceSpreadProc_CmpSpread(const void *v1, const void *v2)
{
    texu_map_keyval *kv1 = (texu_map_keyval *)v1;
    texu_map_keyval *kv2 = (texu_map_keyval *)v2;
    if (kv1->used && kv2->used)
    {
        texu_pricespread* k1 = (texu_pricespread*)kv1->value;
        texu_pricespread* k2 = (texu_pricespread*)kv2->value;
        if (k1 && k2)
        {
            return (k1->spread - k2->spread);
        }
    }
    return TEXU_CMP_GT;
}

void
_TexuEditPriceSpreadProc_FreeSpread(texu_i64 key, texu_i64 value, void* user)
{
    if (value)
    {
        texu_pricespread* ps = (texu_pricespread*)value;
        free(ps);
    }
}

#if 0
texu_pricespread*
_TexuEditPriceSpreadProc_NewSpread(texu_i32 spread, texu_i32 min, texu_i32 max, texu_i32 multiplier)
{
    texu_pricespread* ps = (texu_pricespread*)malloc(sizeof(texu_pricespread));
    if (ps)
    {
        ps->spread = spread;    /*key*/
        ps->max = max;
        ps->min = min;
        ps->multiplier = multiplier;
    }
    return ps;
}
#endif

texu_pricespread*
_TexuEditPriceSpreadProc_CloneSpread(const texu_pricespread* src)
{
    texu_pricespread* ps = (texu_pricespread*)malloc(sizeof(texu_pricespread));
    if (ps)
    {
        memcpy(ps, src, sizeof(texu_pricespread));
    }
    return ps;
}


void
_TexuEditPriceSpreadProc_OnDestroy(texu_wnd *wnd)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);

    texu_map_cb_free(eps->prices, _TexuEditPriceSpreadProc_FreeSpread, 0);
    free(eps);
}

void _TexuEditPriceSpreadProc_OnPaint(texu_wnd *wnd, texu_cio *cio)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_char textcommas[TEXU_MAX_WNDTEXT + 1];
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_bool enable = texu_wnd_is_enable(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
    texu_ui32 selcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED);
    texu_ui32 ltcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 gtcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32  y = texu_wnd_get_y(wnd);
    texu_i32  x = texu_wnd_get_x(wnd);
    texu_i32  width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
    texu_ui32 selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED);
    texu_ui32 ltbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 gtbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);
    texu_ui32 bgcolor = normbg;


    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    color = normcolor;
    bgcolor = normbg;

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    if (TEXU_EPSS_AUTOCOMMAS & style)
    {
        texu_add_commas(textcommas, TEXU_MAX_WNDTEXT, text);
    }
    else
    {
        texu_strcpy(textcommas, text);
    }
    if (TEXU_EPSS_SHOWCHANGE & style)
    {
        texu_i32  val = 0;
        texu_char longval[TEXU_MAX_WNDTEXT + 1];
        texu_i32  change = 0;
        texu_char pct[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");
        texu_f64  pctchange = 0.0;
        texu_i32  decwidth = eps->decwidth;
        texu_char format[TEXU_MAX_WNDTEXT + 1];
        texu_char pctcommas[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");

        texu_fs2ls(text, texu_strlen(text), decwidth, longval);
        val = texu_atol(longval); /*current price*/
        change = (val - eps->baseprice.price);
        if (eps->baseprice.price > 0)
        {
            pctchange = ((texu_f64)(change)* eps->baseprice.multiplier / eps->baseprice.price);
            if (pctchange > 0)
            {
                texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("+%%.0%df"), decwidth);
            }
            else
            {
                texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
            }
            texu_sprintf(pct, TEXU_MAX_WNDTEXT, 
                         format, 
                         pctchange);

            if (TEXU_EPSS_AUTOCOMMAS & style)
            {
                texu_add_commas(pctcommas, TEXU_MAX_WNDTEXT, pct);
            }
            else
            {
                texu_strcpy(pctcommas, pct);
            }
            /*add percent*/
            texu_sprintf(textcommas, TEXU_MAX_WNDTEXT, TEXUTEXT("%s(%s%%)"), textcommas, pctcommas);

        }
    }
    texu_printf_alignment2(buf, textcommas, width, style, TEXU_TRUE);
#ifdef TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_color(dc, color));
#else
#if (defined WIN32 && defined _WINDOWS)
    texu_env_draw_text(env, y, x, buf, color, bgcolor);
#else
    texu_cio_putstr_attr(dc, y, x, buf,
                         texu_cio_get_color(dc, color));
#endif
#endif
}

texu_i32
_TexuEditPriceSpreadProc_Foreach(texu_i64 key, texu_i64 value, void *user)
{
    texu_pricespread *spread = (texu_pricespread*)user;
    texu_pricespread *ps = (texu_pricespread*)value;

    if (ps && spread)
    {
        if ( spread->spread >= ps->min  && 
            (spread->spread <= ps->max  ||
                    ps->max < 0)
            )
        {
            memcpy(spread, ps, sizeof(texu_pricespread));
            return TEXU_BREAK;
        }
    }
    return TEXU_OK;
}

texu_i32
_TexuEditPriceSpreadProc_GetNearestPrice(texu_wnd *wnd, texu_char *nearest, texu_i32 len, const texu_char* price, texu_i32 updown)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread*)texu_wnd_get_userdata(wnd);
    texu_i32 decwidth = eps->decwidth;
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_pricespread ps = { -99, -99, -99, 100 };
    texu_i32 value = 0;
    texu_bool rc = _TexuEditPriceSpreadProc_GetBestSpread(wnd, price, &ps);
    if (TEXU_TRUE == rc)
    {
        texu_i32  valuedown = 0;
        texu_char format[TEXU_MAX_WNDTEXT + 1];
        texu_i32  multiplier = (0 >= ps.multiplier ? 1 : ps.multiplier);/*ensure that the multiplier != zero*/

        texu_fs2ls(price, texu_strlen(price), decwidth, prclong);
        value = texu_atol(prclong);
        valuedown = value;

        value += (updown * ps.spread);

        texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
        texu_sprintf(buf, TEXU_MAX_WNDTEXT,
                     format,
                     ((texu_f64)value / multiplier));
        /*to ensure the new price is valid*/
        rc = _TexuEditPriceSpreadProc_IsValidSpread(wnd, buf);
        if (TEXU_FALSE == rc)
        {
            /*invalid spread from input*/
            texu_i32 bestval = 0;

            value = texu_atol(prclong);
            bestval = (value / multiplier) * multiplier;
            /*find the nearest valid input value*/
            while (bestval < value)
            {
                bestval += ps.spread;
            }
            if (updown > 0)
            {
                value = bestval;
            }
            else
            {
                value = bestval -= ps.spread;
            }

            /*valuedown = value;*/
            texu_sprintf(buf, TEXU_MAX_WNDTEXT,
                         format,
                         ((texu_f64)value / multiplier));

            rc = _TexuEditPriceSpreadProc_IsValidSpread(wnd, buf);
        }
        if (updown < 0)
        {
            /*try getting the next spread if the previous spread is not equal the new spread*/
            texu_char buf2[TEXU_MAX_WNDTEXT + 1];
            texu_pricespread ps2 = { -99, -99, -99, 100 };

            texu_strcpy(buf2, buf);
            texu_fs2ls(buf2, texu_strlen(buf2), decwidth, prclong);
            rc = _TexuEditPriceSpreadProc_GetBestSpread(wnd, buf2, &ps2);

            if (ps.spread != ps2.spread)
            {
                multiplier = (0 >= ps2.multiplier ? 1 : ps2.multiplier);
                valuedown -= ps2.spread;

                texu_sprintf(buf, TEXU_MAX_WNDTEXT,
                             format,
                             ((texu_f64)valuedown / multiplier));
                /*set the value*/
                value = valuedown;
            }
        }
        if (nearest)
        {
            texu_sprintf(nearest, len,
                         format,
                         ((texu_f64)value / multiplier));
        }
    }
    return value;
}

texu_bool
_TexuEditPriceSpreadProc_GetBestSpread(texu_wnd *wnd, const texu_char *str, texu_pricespread* prcspread)
{
    texu_bool valid = TEXU_FALSE;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    texu_i32  value = texu_atol(str);
    texu_pricespread ps = { -99, -99, value, eps->baseprice.multiplier };/*error checking min/max = -99*/
    texu_i32  decwidth = eps->decwidth;

    if (!str || 0 == texu_strlen(str))
    {
        return TEXU_FALSE;
    }

    texu_fs2ls(str, texu_strlen(str), decwidth, prclong);
    value = texu_atol(prclong);
    ps.spread = value;

    /*find range*/
    texu_map_foreach(eps->prices,
                     _TexuEditPriceSpreadProc_Foreach,
                     _TexuEditPriceSpreadProc_CmpSpread,
                     &ps);

    if (prcspread)
    {
        memcpy(prcspread, &ps, sizeof(texu_pricespread));
    }
    if (-99 == ps.min && -99 == ps.max)
    {
        /*not found the spread*/
        return TEXU_FALSE;
    }
    return TEXU_TRUE;
}

texu_bool
_TexuEditPriceSpreadProc_IsValidSpread(texu_wnd *wnd, const texu_char *str)
{
    texu_bool valid = TEXU_FALSE;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_i32  value = texu_atol(str);
    texu_pricespread ps = { -99, -99, value };/*error checking min/max = -99*/
    texu_i32  price = 0;
    texu_i32  min = value;
    texu_i32  max = min + 1;
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    texu_i32  decwidth = eps->decwidth;

    if (!str || 0 == texu_strlen(str))
    {
        return TEXU_TRUE;
    }
    /*find range*/
    if (TEXU_FALSE == _TexuEditPriceSpreadProc_GetBestSpread(wnd, str, &ps))
    {
        /*not found the spread*/
        return TEXU_FALSE;
    }
    texu_fs2ls(str, texu_strlen(str), decwidth, prclong);
    value = texu_atol(prclong);
    /*find the best min/max to validate the value*/
    if (0 >= ps.min)
    {
        min = 0;
    }
    else
    {
        min = (texu_i32)(ps.min * (value / ps.min));
    }
    if (ps.max < 0)
    {
        max = value + ps.spread;
    }
    else
    {
        max = ps.max;
    }
    /*optimize the spread validation*/
    value -= min;
    min = ps.spread * (value / ps.spread);
    max = value + ps.spread;

    for (price = min; price <= max; price += ps.spread)
    {
        if (value == price)
        {
            valid = TEXU_TRUE;
            break;
        }
        else if (price > value)
        {
            /*invalid spread immediately*/
            break;
        }
    }
    return valid;
}

void
_TexuEditPriceSpreadProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_wnd *editwnd = editwnd = texu_wnd_get_activechild(wnd);
    texu_i32 updown = 0;
    texu_ui32 style = texu_wnd_get_style(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }

    if (TEXU_KEY_ESCAPE == ch)
    {
        /*restore the old valid text*/
        texu_wnd_get_text(wnd, eps->editbuf, TEXU_MAX_WNDTEXT);
        texu_wnd_set_text(editwnd, eps->editbuf);
        /*simulate CTRL+A (select all)*/
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
    }
    switch (ch)
    {
        case TEXU_KEY_LEFT:
        case TEXU_KEY_DOWN:
            --updown;
            break;

        case TEXU_KEY_RIGHT:
        case TEXU_KEY_UP:
            ++updown;
            break;

        case TEXUTEXT('-'):/*not accept value less than zero*/
            return;
    }
    if (0 == updown)
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)ch, 0);
    }
    else
    {
        /*get the current spread*/
        texu_char buf[TEXU_MAX_WNDTEXT + 1];
        texu_char nearest[TEXU_MAX_WNDTEXT + 1];
        texu_pricespread ps = { -99, -99, -99, 100 };
        texu_bool rc = TEXU_TRUE;
        texu_char prclong[TEXU_MAX_WNDTEXT + 1];
        texu_i32  value = 0;
        texu_i32  decwidth = eps->decwidth;
            
        /*to ensure the text is up-to-date*/
        texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, 0);
        texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
        texu_wnd_send_msg(wnd, TEXU_WM_SETFOCUS, 0, 0);
        
        rc = _TexuEditPriceSpreadProc_GetBestSpread(wnd, buf, &ps);
        if (TEXU_TRUE == rc)
        {
            texu_i32  valuedown = 0;
            texu_char format[TEXU_MAX_WNDTEXT + 1];
            texu_i32  multiplier = (0 >= ps.multiplier ? 1 : ps.multiplier);/*ensure that the multiplier != zero*/

            texu_fs2ls(buf, texu_strlen(buf), decwidth, prclong);
            value = texu_atol(prclong);
            valuedown = value;

            texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
            texu_sprintf(buf, TEXU_MAX_WNDTEXT, 
                         format,
                         ((texu_f64)value / multiplier));

            value = _TexuEditPriceSpreadProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, buf, updown);
            
            if (!(eps->allowed_invalid_ceiling_floor))
            {
                /*not allowed to over/under ceiling/floor*/
                if (eps->correct_floor_ceiling)
                {
                    if (value < eps->baseprice.floor)
                    {
                        value = eps->baseprice.floor;
                    }
                    else if (value > eps->baseprice.ceiling)
                    {
                        value = eps->baseprice.ceiling;
                    }
                    texu_sprintf(buf, TEXU_MAX_WNDTEXT,
                                 format,
                                 ((texu_f64)value / multiplier));
                }
                else
                {
                    if (value < eps->baseprice.floor || value > eps->baseprice.ceiling)
                    {
                        return;
                    }
                }
            }
            texu_wnd_set_text(wnd, buf);
            /*simulate CTRL+A (select all)*/
            texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_i64)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
        }
    }
}

void
_TexuEditPriceSpreadProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_f64 floatvalue = texu_atof(text);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 color = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL);
    texu_ui32 ltcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 gtcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);
    texu_ui32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL);
    texu_ui32 ltbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 gtbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);
    texu_bool valid = TEXU_FALSE;
    texu_i32 value = 0;
    texu_i32  decwidth = eps->decwidth;
    texu_char format[TEXU_MAX_WNDTEXT + 1];

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }

    valid = _TexuEditPriceSpreadProc_IsValidSpread(wnd, text);
    if (!valid)
    {
        return;
    }
    texu_fs2ls(text, texu_strlen(text), decwidth, buf);
    value = texu_atol(buf);

    texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);/*format*/
    texu_sprintf(buf, TEXU_MAX_WNDTEXT, format, floatvalue);

    /*set color*/
    if (value < eps->baseprice.price)
    {
        color = ltcolor;
        bgcolor = ltbg;
    }
    else if (value > eps->baseprice.price)
    {
        color = gtcolor;
        bgcolor = gtbg;
    }
    texu_wnd_set_color(wnd, color, color);
    texu_wnd_set_bgcolor(wnd, bgcolor, bgcolor);
    texu_wnd_set_text(eps->editwnd, buf);

    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_i64)buf, 0);
}

void
_TexuEditPriceSpreadProc_OnInsertSpread(texu_wnd *wnd, texu_i32 spread, const texu_pricespread *ps)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_pricespread *newps = _TexuEditPriceSpreadProc_CloneSpread(ps);
    if (!newps)
    {
        return;
    }

    texu_map_cmp_insert(eps->prices, spread, (texu_i64)newps, 
                        _TexuEditPriceSpreadProc_CmpSpread);
}

void
_TexuEditPriceSpreadProc_OnDeleteSpread(texu_wnd *wnd, texu_i32 spread)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_map_cb_remove(eps->prices, spread, 
                       _TexuEditPriceSpreadProc_CmpSpread, 
                       _TexuEditPriceSpreadProc_FreeSpread, 
                       0);
}

texu_status _TexuEditPriceSpreadProc_OnSetBaseSpread(texu_wnd *wnd, const texu_baseprice *ps)
{
    texu_status rc = TEXU_OK;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    if (ps)
    {

        memcpy(&eps->baseprice, ps, sizeof(texu_baseprice));
        if (eps->correct_floor_ceiling)
        {
            texu_char nearest[TEXU_MAX_WNDTEXT + 1];
            texu_char price[TEXU_MAX_WNDTEXT + 1];
            texu_char prclong[TEXU_MAX_WNDTEXT + 1];
            texu_char format[TEXU_MAX_WNDTEXT + 1];
            texu_bool rc = TEXU_FALSE;
            texu_i32  decwidth = eps->decwidth;
            texu_i32  multiplier = (ps->multiplier <= 0 ? 1 : ps->multiplier);
            texu_i32  newprice = eps->baseprice.price;
            /*correct price & ceiling & floor*/
            texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
            /*price*/
            texu_sprintf(price, TEXU_MAX_WNDTEXT, format,
                         (texu_f64)eps->baseprice.price / multiplier);

            rc = _TexuEditPriceSpreadProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEditPriceSpreadProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, -1);
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.price = newprice;
            }
            /*floor*/
            texu_sprintf(price, TEXU_MAX_WNDTEXT, format,
                         (texu_f64)eps->baseprice.floor / multiplier);

            rc = _TexuEditPriceSpreadProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEditPriceSpreadProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, 1);/*not less than*/
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.floor = newprice;
            }
            /*ceiling*/
            texu_sprintf(price, TEXU_MAX_WNDTEXT, format,
                         (texu_f64)eps->baseprice.ceiling / multiplier);

            rc = _TexuEditPriceSpreadProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEditPriceSpreadProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, -1);/*not greater than*/
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.ceiling = newprice;
            }
        }
        eps->baseprice.multiplier = TEXU_MIN(1000000, TEXU_MAX(1, eps->baseprice.multiplier));
    }
    return rc;
}

texu_status _TexuEditPriceSpreadProc_OnGetBaseSpread(texu_wnd *wnd, texu_baseprice *ps)
{
    texu_status rc = TEXU_OK;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    if (ps)
    {
        memcpy(ps, &eps->baseprice, sizeof(texu_baseprice));
    }
    return rc;
}

void
_TexuEditPriceSpreadProc_OnSetPriceDecimal(texu_wnd *wnd, texu_i32 decimal)
{
    texu_status rc = TEXU_OK;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);

    decimal = TEXU_MIN(6, TEXU_MAX(0, decimal)); /*min=0, max=6*/
    eps->decwidth = decimal;
    texu_wnd_send_msg(eps->editwnd, TEXU_EM_SETDECWIDTH, decimal, 0);
}

void
_TexuEditPriceSpreadProc_OnAllowOverCeilingFloor(texu_wnd *wnd, texu_bool fAllow)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    eps->allowed_invalid_ceiling_floor = fAllow;
}

void
_TexuEditPriceSpreadProc_OnCorrectCeilingFloor(texu_wnd *wnd, texu_bool fAllow)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    eps->correct_floor_ceiling = fAllow;
}

void
_TexuEditPriceSpreadProc_OnLoadSpreads(texu_wnd *wnd, const texu_pricespread *prices, texu_i32 max_prices)
{
    texu_i32 i = 0;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    /*requires remove all spreads*/
    texu_map_cb_free(eps->prices, _TexuEditPriceSpreadProc_FreeSpread, 0);
    /*load all new spreads*/
    for (i = 0; i < max_prices; ++i)
    {
        _TexuEditPriceSpreadProc_OnInsertSpread(wnd,
                                                prices[i].spread,
                                                &prices[i]);
    }
}

void
_TexuEditPriceSpreadProc_OnLoadDefaultSpreads(texu_wnd *wnd)
{
    /*SET: 2023-01-30 https://knowledge.bualuang.co.th/knowledge-base/pricespread/ */
    static texu_pricespread prices[] =
    {   /*{ min, max, spread, multiplier */
        {      1,    199,    1, 100 },
        {    200,    499,    2, 100 },
        {    500,    999,    5, 100 },
        {   1000,   2499,   10, 100 },
        {   2500,   9999,   25, 100 },
        {  10000,  19999,   50, 100 },
        {  20000,  39999,  100, 100 },
        {  40000,   -100,  200, 100 },
    };
    texu_i32 max_prices = sizeof(prices) / sizeof(prices[0]);
    _TexuEditPriceSpreadProc_OnLoadSpreads(wnd, prices, max_prices);
}



texu_i64
_TexuEditPriceSpreadProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_CHAR:
            _TexuEditPriceSpreadProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuEditPriceSpreadProc_OnCreate(wnd, (texu_wnd_attrs *)param1);


        case TEXU_WM_DESTROY:
            _TexuEditPriceSpreadProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuEditPriceSpreadProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_SETFOCUS:
            _TexuEditPriceSpreadProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuEditPriceSpreadProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_SETTEXT:
            _TexuEditPriceSpreadProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EPSM_INSERTSPREAD:
            _TexuEditPriceSpreadProc_OnInsertSpread(wnd, param1, (const texu_pricespread*)param2);
            return 0;

        case TEXU_EPSM_DELETESPREAD:
            _TexuEditPriceSpreadProc_OnDeleteSpread(wnd, param1);
            return 0;

        case TEXU_EPSM_SETBASESPREAD:
            return _TexuEditPriceSpreadProc_OnSetBaseSpread(wnd, (const texu_baseprice*)param1);

        case TEXU_EPSM_GETBASESPREAD:
            return _TexuEditPriceSpreadProc_OnGetBaseSpread(wnd, (texu_baseprice*)param1);

        case TEXU_EPSM_SETPRICEDECIMAL:
            _TexuEditPriceSpreadProc_OnSetPriceDecimal(wnd, param1);
            return 0;

        case TEXU_EPSM_LOADDEFAULTSPREADS:
            _TexuEditPriceSpreadProc_OnLoadDefaultSpreads(wnd);
            return 0;

        case TEXU_EPSM_LOADSPREADS:
            _TexuEditPriceSpreadProc_OnLoadSpreads(wnd, (const texu_pricespread*)param1, param2);
            return 0;

        case TEXU_EPSM_ALLOWOVERCEILINGFLOOR:
            _TexuEditPriceSpreadProc_OnAllowOverCeilingFloor(wnd, (texu_bool)param1);
            return 0;

        case TEXU_EPSM_CORRECTCEILINGFLOOR:
            _TexuEditPriceSpreadProc_OnCorrectCeilingFloor(wnd, (texu_bool)param1);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
