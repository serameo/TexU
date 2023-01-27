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
#if (defined WIN32 && defined _WINDOWS)
    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED);
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
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED));
#endif
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
#if (defined WIN32 && defined UNICODE)
    swprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
#else
    sprintf(buf, TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
#endif
    texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/


    /*texu_wnd_set_text(wnd, buf);*/

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
#if (defined WIN32 && defined UNICODE)
        swprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
#else
        sprintf(buf, TEXUTEXT("%d"), val);
#endif
        texu_wnd_set_text(editwnd, buf);
        texu_wnd_send_msg(editwnd, TEXU_WM_SETFOCUS, 0, 0);

        texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_i64)&ip, 0);
#if (defined WIN32 && defined UNICODE)
        swprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
#else
        sprintf(buf, TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
#endif
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
#if (defined WIN32 && defined UNICODE)
    swprintf(buf,  sizeof(buf),  TEXUTEXT("%d"), ip._1st);
    swprintf(buf2, sizeof(buf2), TEXUTEXT("%d"), ip._2nd);
    swprintf(buf3, sizeof(buf3), TEXUTEXT("%d"), ip._3rd);
    swprintf(buf4, sizeof(buf4), TEXUTEXT("%d"), ip._4th);
#else
    sprintf(buf,   TEXUTEXT("%d"), ip._1st);
    sprintf(buf2,  TEXUTEXT("%d"), ip._2nd);
    sprintf(buf3,  TEXUTEXT("%d"), ip._3rd);
    sprintf(buf4,  TEXUTEXT("%d"), ip._4th);
#endif
    texu_wnd_set_text(ipctl->editwnd,  buf);
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
#if (defined WIN32 && defined _WINDOWS)
    texu_i32 bgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL);
#endif
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
#if (defined WIN32 && defined _WINDOWS)
    attrs2.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL);
    attrs2.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
    attrs2.selectedcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_SELECTED);
    attrs2.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_FOCUSED);
#endif
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

    emctl->editwnd = editwnd;

    /* save memory */
    texu_wnd_set_userdata(wnd, emctl);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED));
#if (defined WIN32 && defined _WINDOWS)
    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_FOCUSED));
#endif
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
        return TEXU_ERROR;
    }

    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_EMN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_CONTINUE;
}

void
_TexuEditMaskProc_OnDestroy(texu_wnd *wnd)
{
    texu_editmask *emctl = 0;
    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    free(emctl);
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
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


#ifdef __cplusplus
}
#endif
