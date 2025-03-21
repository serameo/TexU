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
#include "cjson.h"

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

void _TexuIPAddressProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuIPAddressProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void _TexuIPAddressProc_OnDestroy(texu_wnd *wnd);
void _TexuIPAddressProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32 _TexuIPAddressProc_OnKillFocus(texu_wnd *, texu_wnd *, texu_i32 state);
texu_i32 _TexuIPAddressProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuIPAddressProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void _TexuIPAddressProc_OnSetMinMax(texu_wnd *wnd, texu_i32, texu_i32);
texu_status _TexuIPAddressProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *, texu_i32 *);
void _TexuIPAddressProc_OnSetStep(texu_wnd *wnd, texu_i32);
texu_i32 _TexuIPAddressProc_OnGetStep(texu_wnd *wnd);
void _TexuIPAddressProc_OnStep(texu_wnd *wnd, texu_i32);
void _TexuIPAddressProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect);
void _TexuIPAddressProc_OnSetPage(texu_wnd *wnd, texu_i32);
texu_i32 _TexuIPAddressProc_OnGetPage(texu_wnd *wnd);
void _TexuIPAddressProc_OnPage(texu_wnd *wnd, texu_i32);
void        _TexuIPAddressProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw);
texu_bool   _TexuIPAddressProc_OnShow(texu_wnd *wnd, texu_bool visible);


texu_bool   _TexuIPAddressProc_OnShow(texu_wnd *wnd, texu_bool visible)
{
    texu_ipaddr *ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    texu_wnd_visible(ipctl->editwnd, visible);
    texu_wnd_visible(ipctl->editwnd2, visible);
    texu_wnd_visible(ipctl->editwnd3, visible);
    texu_wnd_visible(ipctl->editwnd4, visible);
    return visible;
}

void
_TexuIPAddressProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw)
{
    texu_ipaddr *ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_urect rc;
    rc.r1 = *rect;
    rc.r2.y = 0;
    rc.r2.x = 0;
    rc.r2.height = 1;
    rc.r2.width  = 3;
    texu_wnd_move(ipctl->editwnd,  rc.r2.y, rc.r2.x,      rc.r2.height, rc.r2.width, redraw);
    texu_wnd_move(ipctl->editwnd2, rc.r2.y, rc.r2.x + 4,  rc.r2.height, rc.r2.width, redraw);
    texu_wnd_move(ipctl->editwnd3, rc.r2.y, rc.r2.x + 8,  rc.r2.height, rc.r2.width, redraw);
    texu_wnd_move(ipctl->editwnd4, rc.r2.y, rc.r2.x + 12, rc.r2.height, rc.r2.width, redraw);
}

void
_TexuIPAddressProc_OnSetMinMax(texu_wnd *wnd, texu_i32 min, texu_i32 max)
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
    texu_wnd_send_msg(ipctl->editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd2, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd3, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd4, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
}

texu_status
_TexuIPAddressProc_OnGetMinMax(texu_wnd *wnd, texu_i32 *min, texu_i32 *max)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    *min = ipctl->min;
    *max = ipctl->max;
    return TEXU_OK;
}

void
_TexuIPAddressProc_OnSetStep(texu_wnd *wnd, texu_i32 step)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    step = TEXU_MAX(ipctl->min, step);
    step = TEXU_MIN(ipctl->max, step);
    ipctl->step = step;
}

texu_i32
_TexuIPAddressProc_OnGetStep(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    return ipctl->step;
}

void
_TexuIPAddressProc_OnStep(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuIPAddressProc_OnChar(wnd, TEXU_KEY_UP, 0);
    }
    else
    {
        _TexuIPAddressProc_OnChar(wnd, TEXU_KEY_DOWN, 0);
    }
}

void
_TexuIPAddressProc_OnSetPage(texu_wnd *wnd, texu_i32 page)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    page = TEXU_MAX(ipctl->min, page);
    page = TEXU_MIN(ipctl->max, page);
    ipctl->page = page;
}

texu_i32
_TexuIPAddressProc_OnGetPage(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    return ipctl->page;
}

void
_TexuIPAddressProc_OnPage(texu_wnd *wnd, texu_i32 updown)
{
    if (updown > 0)
    {
        _TexuIPAddressProc_OnChar(wnd, TEXU_KEY_NPAGE, 0);
    }
    else
    {
        _TexuIPAddressProc_OnChar(wnd, TEXU_KEY_PPAGE, 0);
    }
}

texu_status
_TexuIPAddressProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
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
    texu_editminmax vmm = { 0, 255 };

    editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
#if 1//defined _WIN32
    attrs2.y = 0;
    attrs2.x = 0;
#else
    attrs2.y = attrs->y;
    attrs2.x = attrs->x;
#endif
    attrs2.height = attrs->height;
    attrs2.width = 3;/* attrs->width - 1;*/
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = TEXUTEXT("127");
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
    texu_wnd_send_msg(editwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    /*second*/
    editwnd2 = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd2)
    {
        texu_wnd_del(editwnd);
        return TEXU_NOMEM;
    }
    attrs2.x += 4;
    attrs2.id = 2;
    attrs2.text = TEXUTEXT("0");
    rc = texu_wnd_create(editwnd2, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd2, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
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
    attrs2.text = TEXUTEXT("0");
    rc = texu_wnd_create(editwnd3, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        texu_wnd_del(editwnd3);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd3, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
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
    attrs2.text = TEXUTEXT("1");
    rc = texu_wnd_create(editwnd4, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        texu_wnd_del(editwnd2);
        texu_wnd_del(editwnd3);
        texu_wnd_del(editwnd4);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd4, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);

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

    texu_wnd_send_msg(ipctl->editwnd,  TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd2, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd3, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);
    texu_wnd_send_msg(ipctl->editwnd4, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&minmax);

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
_TexuIPAddressProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
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
_TexuIPAddressProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
{
    /*update value to window text */
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_ipaddr *ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    texu_ip_addr ip = { 0, 0, 0, 0 };

    texu_wnd_send_msg(ipctl->editwnd,  TEXU_WM_KILLFOCUS, 0, state);
    texu_wnd_send_msg(ipctl->editwnd2, TEXU_WM_KILLFOCUS, 0, state);
    texu_wnd_send_msg(ipctl->editwnd3, TEXU_WM_KILLFOCUS, 0, state);
    texu_wnd_send_msg(ipctl->editwnd4, TEXU_WM_KILLFOCUS, 0, state);

    texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_lparam)&ip, 0);

    texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th);
    texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/

    _TexuWndProc_Notify(wnd, TEXU_IPN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    texu_wnd_invalidate(wnd);
    return TEXU_CONTINUE;
}

void
_TexuIPAddressProc_OnDestroy(texu_wnd *wnd)
{
    texu_ipaddr *ipctl = 0;
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    free(ipctl);
}

texu_i32
_TexuIPAddressProc_OnGetIPAddress(texu_wnd *wnd, texu_ip_addr* ip)
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
_TexuIPAddressProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
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
    texu_env *env = texu_wnd_get_env(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    ipctl = (texu_ipaddr *)texu_wnd_get_userdata(wnd);
    editwnd = texu_wnd_get_activechild(wnd);
#if defined WIN32
    texu_char ch2 = texu_win32_oem_chars(ch);
    if (ch2 > 0)
    {
        ch = ch2;
    }
#endif
    switch (ch)
    {
        /*case TEXU_KEY_UP:
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
        }*/
#if 0
        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);

                if (prevwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
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
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            break;
#endif
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
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
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
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
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

        texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_lparam)&ip, 0);

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d.%d.%d.%d"), ip._1st, ip._2nd, ip._3rd, ip._4th); 
        texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/
    }
    else
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)ch, 0);
    }
}

texu_bool
_TexuIPAddressProc_IsValidIP(texu_wnd *wnd, const texu_char *text, texu_ip_addr *ip)
{
    texu_bool rc = TEXU_TRUE;
    int _1st = 0;
    int _2nd = 0;
    int _3rd = 0;
    int _4th = 0;

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
_TexuIPAddressProc_OnSetText(texu_wnd *wnd, const texu_char *text)
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
    if (TEXU_FALSE == _TexuIPAddressProc_IsValidIP(wnd, text, &ip))
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
    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)buf, 0);
}
void
_TexuIPAddressProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect)
{
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);

    texu_env *env = texu_wnd_get_env(wnd);
    static texu_char ipdots[] = TEXUTEXT("   .   .   .   ");
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_IPADDRESSCTRL);
    texu_ui32 discolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED);
    texu_ui32 color = normcolor;
    texu_ui32 normbgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL);
    texu_ui32 disbgcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_IPADDRESSCTRL_DISABLED);
#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 bgcolor = normbgcolor;
#endif
    texu_wnd_get_color(wnd, &normcolor, &discolor);
    texu_wnd_get_bgcolor(wnd, &normbgcolor, &disbgcolor);

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
#if 0 //defined TEXU_CIO_COLOR_MONO
    if (texu_wnd_is_enable(wnd))
    {
        color = normcolor;
    }
    else
    {
        color = discolor;
    }
    texu_cio_putstr_attr(dc, y, x, ipdots,
                        texu_cio_get_color(dc, color));
#else
    if (texu_wnd_is_enable(wnd))
    {
        color = normcolor;
        bgcolor = normbgcolor;
    }
    else
    {
        color = discolor;
        bgcolor = disbgcolor;
    }
    texu_cio_draw_text(dc, y, x, ipdots, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif
}

texu_longptr
_TexuIPAddressProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_SHOW:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuIPAddressProc_OnShow(wnd, (texu_bool)param1);
            return 0;

        case TEXU_WM_MOVE:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuIPAddressProc_OnMove(wnd, (texu_rect*)param1, (texu_bool)param2);
            return 0;

        case TEXU_WM_CHAR:
            _TexuIPAddressProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuIPAddressProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuIPAddressProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_DESTROY:
            _TexuIPAddressProc_OnDestroy(wnd);
            break;

        case TEXU_WM_SETFOCUS:
            _TexuIPAddressProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuIPAddressProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

        case TEXU_WM_SETTEXT:
            _TexuIPAddressProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_IPM_SETMINMAX:
        {
            _TexuIPAddressProc_OnSetMinMax(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;
        }
        case TEXU_IPM_GETMINMAX:
        {
            return _TexuIPAddressProc_OnGetMinMax(wnd, (texu_i32 *)param1, (texu_i32 *)param2);
        }
        case TEXU_IPM_SETSTEP:
        {
            _TexuIPAddressProc_OnSetStep(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETSTEP:
        {
            return _TexuIPAddressProc_OnGetStep(wnd);
        }
        case TEXU_IPM_STEP:
        {
            _TexuIPAddressProc_OnStep(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_SETPAGE:
        {
            _TexuIPAddressProc_OnSetPage(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETPAGE:
        {
            return _TexuIPAddressProc_OnGetPage(wnd);
        }
        case TEXU_IPM_PAGE:
        {
            _TexuIPAddressProc_OnPage(wnd, (texu_i32)param1);
            return 0;
        }
        case TEXU_IPM_GETIPADDRESS:
        {
            if (0 == param1)
            {
                return 0;
            }
            return _TexuIPAddressProc_OnGetIPAddress(wnd, (texu_ip_addr*)param1);
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
texu_i32 _TexuEditMaskProc_OnKillFocus(texu_wnd *, texu_wnd *, texu_i32 state);
texu_i32 _TexuEditMaskProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void _TexuEditMaskProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void _TexuEditMaskProc_OnSetMask(texu_wnd *wnd, const texu_char *mask);
texu_i32 _TexuEditMaskProc_OnGetMask(texu_wnd *wnd, texu_char *mask, texu_i32 masklen);
void _TexuEditMaskProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect);
texu_bool   _TexuEditMaskProc_IsValidString(texu_wnd *wnd, const texu_char *str);
void _TexuEditMaskProc_OnSetInfo(texu_wnd *wnd, const texu_char *text);
void        _TexuEditMaskProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw);
texu_bool   _TexuEditMaskProc_OnShow(texu_wnd *wnd, texu_bool visible);


texu_bool   _TexuEditMaskProc_OnShow(texu_wnd *wnd, texu_bool visible)
{
    texu_editmask *emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    return texu_wnd_visible(emctl->editwnd, visible);
}

void
_TexuEditMaskProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw)
{
    texu_editmask *emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    texu_urect rc;
    rc.r1 = *rect;
    rc.r2.y = 0;
    rc.r2.x = 0;
    texu_wnd_move(emctl->editwnd, rc.r2.y, rc.r2.x, rc.r2.height, rc.r2.width, redraw);
}


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

    attrs2.y = 0;
    attrs2.x = 0;
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

    /*texu_wnd_visible(editwnd, TEXU_FALSE);*/
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
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    /*set focust to the first edit window*/
    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    if (texu_strlen(text) == 0)
    {
        texu_strcpy(text, emctl->infobuf);
    }
    texu_wnd_set_text(emctl->editwnd, text);
    texu_wnd_visible(emctl->editwnd, TEXU_TRUE);
    texu_wnd_send_msg(emctl->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_EMN_SETFOCUS);
}

texu_i32
_TexuEditMaskProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
{
    /*update value to window text */
    texu_editmask *emctl = 0;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    texu_bool valid = TEXU_TRUE;

    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    texu_wnd_get_text(wnd, emctl->editbuf, TEXU_MAX_WNDTEXT);

    texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, state);
    texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
    valid = _TexuEditMaskProc_IsValidString(wnd, buf);
    if (TEXU_FALSE == valid)
    {
        if (0 != texu_strcmp(buf, emctl->infobuf))
        {
            _TexuWndProc_Notify(wnd, TEXU_EMN_INVALIDEXPR);

            /*change edit color*/
            texu_wnd_send_msg(emctl->editwnd, TEXU_EM_INVALIDATE, 0, 0);
            texu_wnd_invalidate(emctl->editwnd);
            return TEXU_ERROR;
        }
        /**/
        texu_wnd_set_text(editwnd, TEXUTEXT(""));
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

void _TexuEditMaskProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_editmask *emctl = 0;

    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);

    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);
    texu_i32 width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITMASKCTRL_DISABLED);
#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 bgcolor = normbg;
#endif
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

    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);

    emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);

    texu_wnd_get_text(wnd, buf, TEXU_MAX_WNDTEXT);
    if (texu_strlen(buf) == 0 && texu_strlen(emctl->infobuf) > 0)
    {
        texu_strcpy(buf, emctl->infobuf);
    }
    texu_printf_alignment3(text, buf, width, style, TEXU_FALSE, x, cx);

#if 0 //defined TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(cio, y, x, text,
                         texu_cio_get_reverse(cio, color));
#else
    bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);
    texu_cio_draw_text(cio, y, x, text, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif /* TEXU_CIO_COLOR_MONO*/
}

texu_bool
_TexuEditMaskProc_IsValidString(texu_wnd *wnd, const texu_char *str)
{
    texu_bool valid = TEXU_TRUE;
    texu_editmask *emctl = (texu_editmask *)texu_wnd_get_userdata(wnd);
    int matched = 0;
    char expr[TEXU_MAX_WNDTEXT + 1] = "";
    int len = 0;

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
    texu_env *env = texu_wnd_get_env(wnd);

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
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
    }
    else if (TEXU_KEY_UP == ch)
    {
        //case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);

                if (prevwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            
    }
#if 0
    else if (TEXU_KEY_DOWN == ch)
    {

        //case TEXU_KEY_DOWN:
            /*texu_wnd_invalidate(wnd);*/
            /*move next*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *nextwnd = texu_wnd_get_next_activechild(parent, wnd);

                if (nextwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
    }
#endif
    texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)ch, 0);
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

    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)text, 0);
}


texu_longptr
_TexuEditMaskProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_EMM_SETEMAILMASK:
            _TexuEditMaskProc_OnSetMask(wnd, (const texu_char *)TEXU_EDITMASK_EMAIL);
            return 0;

        case TEXU_WM_SHOW:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuEditMaskProc_OnShow(wnd, (texu_bool)param1);
            return 0;

        case TEXU_WM_MOVE:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuEditMaskProc_OnMove(wnd, (texu_rect*)param1, (texu_bool)param2);
            return 0;

        case TEXU_WM_CHAR:
            _TexuEditMaskProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuEditMaskProc_OnCreate(wnd, (texu_wnd_attrs *)param1);


        case TEXU_WM_DESTROY:
            _TexuEditMaskProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuEditMaskProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_SETFOCUS:
            _TexuEditMaskProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuEditMaskProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

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
    texu_wnd   *editwnd;    /*decimal*/
    texu_wnd   *editwnd2;   /*alphablet*/
    texu_char   editbuf[TEXU_MAX_WNDTEXT + 1];
    texu_map   *prices; /*hold texu_pricespread*/
    texu_baseprice    baseprice; /*spread = base price, min = base price x (1-x%), max = base price x (1+x%)*/
    texu_bool   allowed_invalid_ceiling_floor;
    texu_i32    decwidth;
    texu_bool   correct_floor_ceiling;
#if (defined WIN32 && defined _WINDOWS)
    HFONT       clsfont;    /*font class*/
    HFONT       insfont;    /*instance class*/
#endif
    void        *exparam;
};
typedef struct texu_edit_pricespread texu_edit_pricespread;

void        _TexuEPSProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void        _TexuEPSProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuEPSProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void        _TexuEPSProc_OnDestroy(texu_wnd *wnd);
void        _TexuEPSProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32    _TexuEPSProc_OnKillFocus(texu_wnd *, texu_wnd *, texu_i32 state);
texu_i32    _TexuEPSProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuEPSProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void        _TexuEPSProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect);
/*
texu_pricespread*    _TexuEPSProc_NewSpread(texu_i32 spread, texu_i32 min, texu_i32 max, texu_i32 multiplier);
*/
void        _TexuEPSProc_OnInsertSpread(texu_wnd *wnd, texu_i32 spread, const texu_pricespread *ps);
void        _TexuEPSProc_OnDeleteSpread(texu_wnd *wnd, texu_i32 spread);
texu_status _TexuEPSProc_OnSetBaseSpread(texu_wnd *wnd, const texu_baseprice *ps);
texu_status _TexuEPSProc_OnGetBaseSpread(texu_wnd *wnd, texu_baseprice *ps);
void        _TexuEPSProc_OnSetPriceDecimal(texu_wnd *wnd, texu_i32 decimal);
void        _TexuEPSProc_OnLoadDefaultSpreads(texu_wnd *wnd);
void        _TexuEPSProc_OnLoadSpreads(texu_wnd *wnd, const texu_pricespread *prices, texu_i32 max_prices);
texu_bool   _TexuEPSProc_GetBestSpread(texu_wnd *wnd, const texu_char *str, texu_pricespread* prcspread);
void        _TexuEPSProc_OnAllowOverCeilingFloor(texu_wnd *wnd, texu_bool fAllow);
void        _TexuEPSProc_OnCorrectCeilingFloor(texu_wnd *wnd, texu_bool fAllow);
texu_i32    _TexuEPSProc_GetNearestPrice(texu_wnd *wnd, texu_char *nearest, texu_i32 len, const texu_char* price, texu_i32 updown);
texu_bool   _TexuEPSProc_IsValidSpread(texu_wnd *wnd, const texu_char *str);
int          _TexuEPSProc_CmpSpread(const void *v1, const void *v2);
void        _TexuEPSProc_FreeSpread(texu_longptr key, texu_longptr value, void* user);
void        _TexuEPSProc_CreateFonts(texu_wnd *wnd);
texu_i32    _TexuEPSProc_TranslatePriceString(const texu_char *str, texu_i32 dec);

void
_TexuEPSProc_CreateFonts(texu_wnd *wnd)
{
#if (defined WIN32 && defined _WINDOWS)
    texu_env *env = texu_wnd_get_env(wnd);
    cJSON *prop = texu_env_get_prop(env);
    texu_edit_pricespread *eps = (texu_edit_pricespread*)texu_wnd_get_userdata(wnd);

    if (!prop)
    {
        eps->clsfont = texu_env_get_hfont(env);
        eps->insfont = eps->clsfont;
    }
    {
        cJSON *envjson = cJSON_GetObjectItem(prop, "env");
        if (envjson)
        {
            cJSON *classes = cJSON_GetObjectItem(envjson, "classes");
            cJSON *instances = cJSON_GetObjectItem(envjson, "instances");
            if (classes)
            {
            }
        }
    }
#endif
}

texu_status
_TexuEPSProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_edit_pricespread *eps = 0;
    texu_wnd_attrs attrs2;
    texu_wnd *editwnd = 0;
    texu_wnd* editwnd2 = 0;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(wnd);

    editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));

    attrs2.y = 0;
    attrs2.x = 0;

    attrs2.height = 0;//attrs->height;
    attrs2.width = 0;//attrs->width;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_FALSE;
    attrs2.text = attrs->text;
    attrs2.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_FOCUSED);

    attrs2.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_FOCUSED);

    attrs2.id = TEXU_EPS_EDIT_NUMBER_ID;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_LEFT | TEXU_ES_DECIMAL | TEXU_WS_HIDE;
    attrs2.exstyle = 0;

    rc = texu_wnd_create(editwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd, TEXU_EM_SETDECWIDTH, 2, 0); /*2-decimal points*/

    /*ATO ATC MP MO ML supported*/
    attrs2.id = TEXU_EPS_EDIT_ALPHA_ID;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_LEFT | TEXU_ES_UPPERCASE | TEXU_WS_HIDE;

    editwnd2 = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd2)
    {
        return TEXU_NOMEM;
    }

    rc = texu_wnd_create(editwnd2, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd2);
        return TEXU_ERROR;
    }
    /*if (TEXU_EPSS_ATOATC & attrs->style)
    {
        texu_wnd_visible(editwnd, TEXU_FALSE);
        texu_wnd_visible(editwnd2, TEXU_TRUE);
    }
    else
    {
        texu_wnd_visible(editwnd, TEXU_TRUE);
        texu_wnd_visible(editwnd2, TEXU_FALSE);
    }*/

    eps = (texu_edit_pricespread *)malloc(sizeof(texu_edit_pricespread));
    if (!eps)
    {
        return TEXU_ERROR;
    }
    memset(eps, 0, sizeof(texu_edit_pricespread));
    eps->editwnd = editwnd; /* no parameter */
    eps->editwnd2 = editwnd2;

    texu_wnd_send_msg(eps->editwnd, TEXU_EM_SETVALIDATION, (texu_lparam)attrs->on_validate, (texu_lparam)attrs->validate_data);
    texu_wnd_send_msg(eps->editwnd2, TEXU_EM_SETVALIDATION, (texu_lparam)attrs->on_validate, (texu_lparam)attrs->validate_data);
    //texu_wnd_visible(editwnd, TEXU_FALSE);
    //texu_wnd_visible(editwnd2, TEXU_FALSE);

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
_TexuEPSProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_urect rect;
    texu_wnd_send_msg(wnd, TEXU_WM_GETCLIENTRECT, (texu_lparam)&rect, 0);
    if (!texu_wnd_is_enable(wnd) || !texu_wnd_is_visible(wnd))
    {
        return;
    }
    /*set focus to the first edit window*/
    if (TEXU_EPSS_ATOATC & style)
    {
        texu_wnd_move(eps->editwnd, 0, 0, 0, 0, TEXU_FALSE);
        texu_wnd_visible(eps->editwnd, TEXU_FALSE);
        texu_wnd_move(eps->editwnd2, 0, 0, rect.r2.height, rect.r2.width, TEXU_FALSE);
        texu_wnd_visible(eps->editwnd2, TEXU_TRUE);
        texu_wnd_send_msg(eps->editwnd2, TEXU_WM_SETFOCUS, 0, 0);
    }
    else
    {
        texu_wnd_move(eps->editwnd2, 0, 0, 0, 0, TEXU_FALSE);
        texu_wnd_visible(eps->editwnd2, TEXU_FALSE);
        texu_wnd_move(eps->editwnd, 0, 0, rect.r2.height, rect.r2.width, TEXU_FALSE);
        texu_wnd_visible(eps->editwnd, TEXU_TRUE);
        texu_wnd_send_msg(eps->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    }
    _TexuWndProc_Notify(wnd, TEXU_EPSN_SETFOCUS);
}

texu_i32
_TexuEPSProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
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
    texu_longptr rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, 0, state);
    if (rc < TEXU_OK)
    {
        return rc;
    }
    texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
    if (0 == texu_strlen(buf))
    {
        /*an empty sting do not want to validate*/
        texu_wnd_invalidate(editwnd);
        return TEXU_OK;
    }
    valid = _TexuEPSProc_IsValidSpread(wnd, buf);
    if (TEXU_FALSE == valid)
    {
        _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDSPREAD);

        /*change edit color*/
        texu_wnd_send_msg(editwnd, TEXU_EM_INVALIDATE, 0, 0);
        texu_wnd_invalidate(editwnd);
        return TEXU_ERROR;
    }

    val = _TexuEPSProc_TranslatePriceString(buf, decwidth);
    if (val > 0)
    {
        /* check ceiling and floor */
        texu_fs2ls(buf, texu_strlen(buf), decwidth, longval);
        val = texu_atol(longval);

        ceiling = eps->baseprice.ceiling;
        floor = eps->baseprice.floor;
        if (val < floor && val > 0)
        {
            _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDFLOOR);
            if (!eps->allowed_invalid_ceiling_floor)
            {
                /*change edit color*/
                texu_wnd_send_msg(editwnd, TEXU_EM_INVALIDATE, 0, 0);
                texu_wnd_invalidate(editwnd);
                return TEXU_ERROR;
            }
        }
        else if (val > ceiling && val > 0)
        {
            _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDCEILING);
            if (!eps->allowed_invalid_ceiling_floor)
            {
                /*change edit color*/
                texu_wnd_send_msg(editwnd, TEXU_EM_INVALIDATE, 0, 0);
                texu_wnd_invalidate(editwnd);
                return TEXU_ERROR;
            }
        }
    }
    else if (0 == val)
    {
        _TexuWndProc_Notify(wnd, TEXU_EPSN_INVALIDPRICE);
        /*change edit color*/
        texu_wnd_send_msg(editwnd, TEXU_EM_INVALIDATE, 0, 0);
        texu_wnd_invalidate(editwnd);
        return TEXU_ERROR;
    }

    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_EPSN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    texu_wnd_move(eps->editwnd2, 0, 0, 0, 0, TEXU_FALSE);
    texu_wnd_visible(eps->editwnd2, TEXU_FALSE);
    texu_wnd_move(eps->editwnd, 0, 0, 0, 0, TEXU_FALSE);
    texu_wnd_visible(eps->editwnd, TEXU_FALSE);

    texu_wnd_invalidate(wnd);
    return TEXU_CONTINUE;
}

int
_TexuEPSProc_CmpSpread(const void *v1, const void *v2)
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
_TexuEPSProc_FreeSpread(texu_longptr key, texu_longptr value, void* user)
{
    if (value)
    {
        texu_pricespread* ps = (texu_pricespread*)value;
        free(ps);
    }
}

#if 0
texu_pricespread*
_TexuEPSProc_NewSpread(texu_i32 spread, texu_i32 min, texu_i32 max, texu_i32 multiplier)
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
_TexuEPSProc_CloneSpread(const texu_pricespread* src)
{
    texu_pricespread* ps = (texu_pricespread*)malloc(sizeof(texu_pricespread));
    if (ps)
    {
        memcpy(ps, src, sizeof(texu_pricespread));
    }
    return ps;
}

void
_TexuEPSProc_OnDestroy(texu_wnd *wnd)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);

    texu_map_cb_free(eps->prices, _TexuEPSProc_FreeSpread, 0);
    free(eps);
}

void _TexuEPSProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_char textcommas[TEXU_MAX_WNDTEXT + 1];
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
    texu_ui32 ltcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 eqcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL);
    texu_ui32 gtcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);

    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32  y = texu_wnd_get_y(wnd);
    texu_i32  x = texu_wnd_get_x(wnd);
    texu_i32  width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED);
#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 bgcolor = normbg;
    texu_ui32 ltbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_LOWER);
    texu_ui32 eqbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL);
    texu_ui32 gtbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITPRICESPREADCTRL_GREATER);
#endif
    texu_i32 cx = texu_env_screen_width(env);
    texu_i32  val = 0;
    texu_i32  price = 0;
    texu_char longval[TEXU_MAX_WNDTEXT + 1];
    texu_i32  change = 0;
    texu_char pct[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");

    texu_i32  decwidth = eps->decwidth;
    texu_char format[TEXU_MAX_WNDTEXT + 1];
    texu_char pctcommas[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");

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

    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);
    if (TEXU_EPSS_AUTOCOMMAS & style)
    {
        texu_add_commas(textcommas, TEXU_MAX_WNDTEXT, text);
    }
    else
    {
        texu_strcpy(textcommas, text);
    }

    texu_fs2ls(text, texu_strlen(text), decwidth, longval);
    val = texu_atol(longval); /*current price*/
    price = _TexuEPSProc_TranslatePriceString(text, decwidth);

    change = (val - eps->baseprice.price);
    color = eqcolor;
    if (price > 0 && change > 0)
    {
        color = gtcolor;
    }
    else if (price > 0 && change < 0)
    {
        color = ltcolor;
    }
#if !(defined TEXU_CIO_COLOR_MONO)
    bgcolor = eqbg;
    if (price > 0 && change > 0)
    {
        bgcolor = gtbg;
    }
    else if (price > 0 && change < 0)
    {
        bgcolor = ltbg;
    }
#endif
    /*show change price overrides show change percent if it is available*/
    if (TEXU_EPSS_SHOWCHANGEPRICE & style)
    {
        /*change = (val - eps->baseprice.price);*/
        if (change > 0)
        {
            texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("+%%.0%df"), decwidth);
        }
        else
        {
            texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
        }
        texu_sprintf(pct, TEXU_MAX_WNDTEXT,
                     format,
                     ((texu_f64)change / eps->baseprice.multiplier));

        if (TEXU_EPSS_AUTOCOMMAS & style)
        {
            texu_add_commas(pctcommas, TEXU_MAX_WNDTEXT, pct);
        }
        else
        {
            texu_strcpy(pctcommas, pct);
        }
        /*add percent*/
        texu_sprintf(text, TEXU_MAX_WNDTEXT, TEXUTEXT("%s(%s)"), textcommas, pctcommas);
        texu_strcpy(textcommas, text);
    }
    else if (TEXU_EPSS_SHOWCHANGE & style)
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
            pctchange = ((texu_f64)(change) / eps->baseprice.price)* eps->baseprice.multiplier;
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

    if (TEXU_EPSS_ATOATC & style)
    {
        /*ATO=-1 ATC=-2 MP=-3 MO=-4 ML=-5*/
    }
    texu_printf_alignment3(buf, textcommas, width, style, TEXU_TRUE, x, cx);
#if 0 //defined TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(cio, y, x, buf,
                         texu_cio_get_color(cio, color));
#else
    bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);
    texu_cio_draw_text(cio, y, x, buf, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

#endif
}

texu_i32
_TexuEPSProc_Foreach(texu_longptr key, texu_longptr value, void *user)
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
_TexuEPSProc_GetNearestPrice(texu_wnd *wnd, texu_char *nearest, texu_i32 len, const texu_char* price, texu_i32 updown)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread*)texu_wnd_get_userdata(wnd);
    texu_i32 decwidth = eps->decwidth;
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_pricespread ps = { -99, -99, -99 };
    texu_i32 value = 0;
    texu_bool rc = _TexuEPSProc_GetBestSpread(wnd, price, &ps);
    texu_ui32 style = texu_wnd_get_style(wnd);

    if (TEXU_EPSS_ATOATC & style)
    {
        value = _TexuEPSProc_TranslatePriceString(price, decwidth);
        if (value < 0)
        {
            return value;
        }
    }

    if (TEXU_TRUE == rc)
    {
        texu_i32  valuedown = 0;
        texu_char format[TEXU_MAX_WNDTEXT + 1];
        texu_i32  multiplier = (0 >= eps->baseprice.multiplier ? 1 : eps->baseprice.multiplier);/*ensure that the multiplier != zero*/

        texu_fs2ls(price, texu_strlen(price), decwidth, prclong);
        value = texu_atol(prclong);
        valuedown = value;

        value += (updown * ps.spread);

        texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
        texu_sprintf(buf, TEXU_MAX_WNDTEXT,
                     format,
                     ((texu_f64)value / multiplier));
        /*to ensure the new price is valid*/
        rc = _TexuEPSProc_IsValidSpread(wnd, buf);
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

            rc = _TexuEPSProc_IsValidSpread(wnd, buf);
        }
        if (updown < 0)
        {
            /*try getting the next spread if the previous spread is not equal the new spread*/
            texu_char buf2[TEXU_MAX_WNDTEXT + 1];
            texu_pricespread ps2 = { -99, -99, -99 };

            texu_strcpy(buf2, buf);
            texu_fs2ls(buf2, texu_strlen(buf2), decwidth, prclong);
            rc = _TexuEPSProc_GetBestSpread(wnd, buf2, &ps2);

            if (ps.spread != ps2.spread)
            {
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
_TexuEPSProc_GetBestSpread(texu_wnd *wnd, const texu_char *str, texu_pricespread* prcspread)
{

    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    texu_i32  value = texu_atol(str);
    texu_pricespread ps = { -99, -99, value };/*error checking min/max = -99*/
    texu_i32  decwidth = eps->decwidth;
    texu_ui32 style = texu_wnd_get_style(wnd);

    if (!str || 0 == texu_strlen(str))
    {
        return TEXU_FALSE;
    }
    if (TEXU_EPSS_ATOATC & style)
    {
        value = _TexuEPSProc_TranslatePriceString(str, decwidth);
        if (value < 0)
        {
            return TEXU_TRUE;
        }
    }
    texu_fs2ls(str, texu_strlen(str), decwidth, prclong);
    value = texu_atol(prclong);
    ps.spread = value;

    /*find range*/
    texu_map_foreach(eps->prices,
                     _TexuEPSProc_Foreach,
                     _TexuEPSProc_CmpSpread,
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

#define TEXU_ESP_ATO    (-1)
#define TEXU_ESP_ATC    (-2)
#define TEXU_ESP_MP     (-3)
#define TEXU_ESP_MO     (-4)
#define TEXU_ESP_ML     (-5)
texu_i32 _TexuEPSProc_TranslatePriceString(const texu_char *str, texu_i32 dec)
{
    texu_char prclong[TEXU_MAX_WNDTEXT + 1];
    if (0 == texu_strcmp("ATO", str))
    {
        return TEXU_ESP_ATO;
    }
    else if (0 == texu_strcmp("ATC", str))
    {
        return TEXU_ESP_ATC;
    }
    else if (0 == texu_strcmp("MP", str))
    {
        return TEXU_ESP_MP;
    }
    else if (0 == texu_strcmp("MO", str))
    {
        return TEXU_ESP_MO;
    }
    else if (0 == texu_strcmp("ML", str))
    {
        return TEXU_ESP_ML;
    }
    texu_fs2ls(str, texu_strlen(str), dec, prclong);
    return texu_atol(prclong);
}
texu_bool
_TexuEPSProc_IsValidSpread(texu_wnd *wnd, const texu_char *str)
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
    texu_ui32 style = texu_wnd_get_style(wnd);
    
    if (TEXU_EPSS_NOSPREAD & style)
    {
        return TEXU_TRUE;
    }

    if (!str || 0 == texu_strlen(str) || 0 == texu_strcmp("0.00", str))
    {
        return TEXU_TRUE;
    }
    if (TEXU_EPSS_ATOATC & style)
    {
        if (_TexuEPSProc_TranslatePriceString(str, decwidth) < 0)
        {
            return TEXU_TRUE;
        }
    }
    /*find range*/
    if (TEXU_FALSE == _TexuEPSProc_GetBestSpread(wnd, str, &ps))
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
_TexuEPSProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    texu_i32 updown = 0;
    texu_env *env = texu_wnd_get_env(wnd);
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
        texu_wnd_send_msg(editwnd, TEXU_WM_KEYDOWN, (texu_lparam)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
    }
    else
    {
        switch (ch)
        {
            case 0x7f:
            case TEXU_KEY_DELETE:
            case TEXU_KEY_BACKSPACE:
            {
                texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, ch, alt);
                return;
            }
        }
    }
    switch (ch)
    {
        case TEXU_KEY_LEFT:
        /*case TEXU_KEY_DOWN:*/
            --updown;
            break;

        case TEXU_KEY_RIGHT:
        /*case TEXU_KEY_UP:*/
            ++updown;
            break;

        case TEXUTEXT('-'):/*not accept value less than zero*/
            return;

        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);

                if (prevwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            break;
#if 0
        case TEXU_KEY_DOWN:
            /*texu_wnd_invalidate(wnd);*/
            /*move next*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *nextwnd = texu_wnd_get_next_activechild(parent, wnd);

                if (nextwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            break;
#endif
    }
    if (updown && !(style & TEXU_EPSS_NOSPREAD))
    {
        /*get the current spread*/
        texu_char buf[TEXU_MAX_WNDTEXT + 1];
        texu_char nearest[TEXU_MAX_WNDTEXT + 1];
        texu_pricespread ps = { -99, -99, -99 };
        texu_bool rc = TEXU_TRUE;
        texu_char prclong[TEXU_MAX_WNDTEXT + 1];
        texu_i32  value = 0;
        texu_i32  decwidth = eps->decwidth;
            
        /*to ensure the text is up-to-date*/
        texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, 0);
        texu_wnd_get_text(editwnd, buf, TEXU_MAX_WNDTEXT);
        texu_wnd_send_msg(wnd, TEXU_WM_SETFOCUS, 0, 0);
        
        rc = _TexuEPSProc_GetBestSpread(wnd, buf, &ps);
        if (TEXU_TRUE == rc)
        {

            texu_char format[TEXU_MAX_WNDTEXT + 1];
            texu_i32  multiplier = (0 >= eps->baseprice.multiplier ? 1 : eps->baseprice.multiplier);/*ensure that the multiplier != zero*/

            texu_fs2ls(buf, texu_strlen(buf), decwidth, prclong);
            value = texu_atol(prclong);

            texu_sprintf(format, TEXU_MAX_WNDTEXT, TEXUTEXT("%%.0%df"), decwidth);
            texu_sprintf(buf, TEXU_MAX_WNDTEXT, 
                         format,
                         ((texu_f64)value / multiplier));

            value = _TexuEPSProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, buf, updown);
            if (value > 0)
            {
                /*if (!(eps->allowed_invalid_ceiling_floor))*/
                if (value >= eps->baseprice.floor && value <= eps->baseprice.ceiling)
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
                else if (!(eps->allowed_invalid_ceiling_floor))
                {
                    return; /*not allowed out-of-bound*/
                }
                texu_wnd_set_text(wnd, buf);
                /*simulate CTRL+A (select all)*/
                texu_wnd_send_msg(editwnd, TEXU_WM_KEYDOWN, (texu_lparam)TEXUTEXT('A'), TEXU_KEYPRESSED_CTRL);
            }
        }
    }
}

void
_TexuEPSProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{

    texu_wnd *editwnd = texu_wnd_get_activechild(wnd);
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }

    texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)ch, 0);
}

void
_TexuEPSProc_OnSetText(texu_wnd *wnd, const texu_char *text)
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
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_wnd* editwnd = texu_wnd_get_activechild(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    if (0 == text || 0 == texu_strlen(text))
    {
        /*allow an empty text, also no need to validate it*/
        texu_wnd_set_text(editwnd, "");
        TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)"", 0);
        texu_wnd_invalidate(wnd);
        return;
    }

    if (TEXU_EPSS_ATOATC & style)
    {
        value = _TexuEPSProc_TranslatePriceString(text, decwidth);
        if (value < 0)
        {
            texu_ui32 norm = color;
            texu_ui32 dis = color;
            texu_ui32 bgnorm = bgcolor;
            texu_ui32 bgdis = bgcolor;
            texu_wnd_get_color(wnd, &norm, &dis);
            texu_wnd_get_bgcolor(wnd, &bgnorm, &bgdis);

            texu_wnd_set_color(wnd, norm, dis);
            texu_wnd_set_bgcolor(wnd, bgnorm, bgdis);

            texu_wnd_set_text(editwnd, text);

            TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)text, 0);
            return;
        }
    }
    valid = _TexuEPSProc_IsValidSpread(wnd, text);
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
    texu_wnd_set_color(editwnd, color, color);
    texu_wnd_set_bgcolor(editwnd, bgcolor, bgcolor);

    texu_wnd_set_text(eps->editwnd, buf);
    texu_wnd_set_text(eps->editwnd2, buf);

    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)buf, 0);
}

void
_TexuEPSProc_OnInsertSpread(texu_wnd *wnd, texu_i32 spread, const texu_pricespread *ps)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_pricespread *newps = _TexuEPSProc_CloneSpread(ps);
    if (!newps)
    {
        return;
    }

    texu_map_cmp_insert(eps->prices, spread, (texu_lparam)newps, 
                        _TexuEPSProc_CmpSpread);
}

void
_TexuEPSProc_OnDeleteSpread(texu_wnd *wnd, texu_i32 spread)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    texu_map_cb_remove(eps->prices, spread, 
                       _TexuEPSProc_CmpSpread, 
                       _TexuEPSProc_FreeSpread, 
                       0);
}

texu_status _TexuEPSProc_OnSetBaseSpread(texu_wnd *wnd, const texu_baseprice *ps)
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

            rc = _TexuEPSProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEPSProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, -1);
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.price = newprice;
            }
            /*floor*/
            texu_sprintf(price, TEXU_MAX_WNDTEXT, format,
                         (texu_f64)eps->baseprice.floor / multiplier);

            rc = _TexuEPSProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEPSProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, 1);/*not less than*/
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.floor = newprice;
            }
            /*ceiling*/
            texu_sprintf(price, TEXU_MAX_WNDTEXT, format,
                         (texu_f64)eps->baseprice.ceiling / multiplier);

            rc = _TexuEPSProc_IsValidSpread(wnd, price);
            if (TEXU_FALSE == rc)
            {
                _TexuEPSProc_GetNearestPrice(wnd, nearest, TEXU_MAX_WNDTEXT, price, -1);/*not greater than*/
                texu_fs2ls(nearest, texu_strlen(nearest), decwidth, prclong);
                newprice = texu_atol(prclong);

                eps->baseprice.ceiling = newprice;
            }
        }
        eps->baseprice.multiplier = TEXU_MIN(1000000, TEXU_MAX(1, eps->baseprice.multiplier));
    }
    return rc;
}

texu_status _TexuEPSProc_OnGetBaseSpread(texu_wnd *wnd, texu_baseprice *ps)
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
_TexuEPSProc_OnSetPriceDecimal(texu_wnd *wnd, texu_i32 decimal)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);

    decimal = TEXU_MIN(6, TEXU_MAX(0, decimal)); /*min=0, max=6*/
    eps->decwidth = decimal;
    texu_wnd_send_msg(eps->editwnd, TEXU_EM_SETDECWIDTH, decimal, 0);
    texu_wnd_send_msg(eps->editwnd2, TEXU_EM_SETDECWIDTH, decimal, 0);
}

void
_TexuEPSProc_OnAllowOverCeilingFloor(texu_wnd *wnd, texu_bool fAllow)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    eps->allowed_invalid_ceiling_floor = fAllow;
}

void
_TexuEPSProc_OnCorrectCeilingFloor(texu_wnd *wnd, texu_bool fAllow)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    eps->correct_floor_ceiling = fAllow;
}

void
_TexuEPSProc_OnLoadSpreads(texu_wnd *wnd, const texu_pricespread *prices, texu_i32 max_prices)
{
    texu_i32 i = 0;
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    /*requires remove all spreads*/
    texu_map_cb_free(eps->prices, _TexuEPSProc_FreeSpread, 0);
    /*load all new spreads*/
    for (i = 0; i < max_prices; ++i)
    {
        _TexuEPSProc_OnInsertSpread(wnd, prices[i].spread, &prices[i]);
    }
}

void
_TexuEPSProc_OnLoadDefaultSpreads(texu_wnd *wnd)
{
    /*SET: 2023-01-30 https://knowledge.bualuang.co.th/knowledge-base/pricespread/ */
    static texu_pricespread prices[] =
    {   /*{ min, max, spread, multiplier */
        {      1,    199,    1 },
        {    200,    499,    2 },
        {    500,    999,    5 },
        {   1000,   2499,   10 },
        {   2500,   9999,   25 },
        {  10000,  19999,   50 },
        {  20000,  39999,  100 },
        {  40000,   -100,  200 },
    };
    texu_i32 max_prices = sizeof(prices) / sizeof(prices[0]);
    _TexuEPSProc_OnLoadSpreads(wnd, prices, max_prices);
}

texu_i32 _TexuEPSProc_OnGetPriceAsInt(texu_wnd* wnd, texu_i32 decwidth)
{
    texu_char buf[BUFSIZ + 1];
    texu_char longval[BUFSIZ + 1];
    texu_i32 val = 0;
    texu_wnd_get_text(wnd, buf, BUFSIZ);
    texu_fs2ls(buf, texu_strlen(buf), decwidth, longval);
    val = texu_atol(longval);
    return val;
}

void _TexuEPSProc_OnLimitText(texu_wnd *wnd, texu_i32 len)
{
    texu_edit_pricespread *eps = (texu_edit_pricespread *)texu_wnd_get_userdata(wnd);
    if (len < 0)
    {
        len = 6;
    }
    texu_wnd_send_msg(eps->editwnd, TEXU_EM_LIMITTEXT, len, 0);
    texu_wnd_send_msg(eps->editwnd2, TEXU_EM_LIMITTEXT, len, 0);
}

texu_longptr
_TexuEditPriceSpreadProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CHAR:
            _TexuEPSProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_KEYDOWN:
            _TexuEPSProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuEPSProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_DESTROY:
            _TexuEPSProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuEPSProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_SETFOCUS:
            _TexuEPSProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuEPSProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

        case TEXU_WM_SETTEXT:
            _TexuEPSProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EPSM_INSERTSPREAD:
            _TexuEPSProc_OnInsertSpread(wnd, param1, (const texu_pricespread*)param2);
            return 0;

        case TEXU_EPSM_DELETESPREAD:
            _TexuEPSProc_OnDeleteSpread(wnd, param1);
            return 0;

        case TEXU_EPSM_SETBASESPREAD:
            return _TexuEPSProc_OnSetBaseSpread(wnd, (const texu_baseprice*)param1);

        case TEXU_EPSM_GETBASESPREAD:
            return _TexuEPSProc_OnGetBaseSpread(wnd, (texu_baseprice*)param1);

        case TEXU_EPSM_SETPRICEDECIMAL:
            _TexuEPSProc_OnSetPriceDecimal(wnd, param1);
            return 0;

        case TEXU_EPSM_LOADDEFAULTSPREADS:
            _TexuEPSProc_OnLoadDefaultSpreads(wnd);
            return 0;

        case TEXU_EPSM_LOADSPREADS:
            _TexuEPSProc_OnLoadSpreads(wnd, (const texu_pricespread*)param1, param2);
            return 0;

        case TEXU_EPSM_ALLOWOVERCEILINGFLOOR:
            _TexuEPSProc_OnAllowOverCeilingFloor(wnd, (texu_bool)param1);
            return 0;

        case TEXU_EPSM_CORRECTCEILINGFLOOR:
            _TexuEPSProc_OnCorrectCeilingFloor(wnd, (texu_bool)param1);
            return 0;

        case TEXU_EPSM_GETPRICEASINT:
            return _TexuEPSProc_OnGetPriceAsInt(wnd, (texu_i32)param1);

        case TEXU_EPSM_LIMITTEXT:
            _TexuEPSProc_OnLimitText(wnd, param1);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


/*EDIT VOLUME CONTROL*/
/*
#-------------------------------------------------------------------------------
# TexU EditVolumeCtrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_edit_volume
{
    /*texu_wnd   *editwnd;*/ /*to enter odd lot*/
    /*texu_char   editbuf[TEXU_MAX_WNDTEXT + 1];*/
    texu_bool   odd_lot;    /*in case of odd lot*/
    void        *exparam;
    texu_ui32   lot;
    texu_wnd   *volwnd;
    texu_i32    max;
};
typedef struct texu_edit_volume texu_edit_volume;
#define EDITVOL_EDIT_ID     1
#define EDITVOL_UPDOWN_ID   2


void        _TexuEVProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void        _TexuEVProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_status _TexuEVProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void        _TexuEVProc_OnDestroy(texu_wnd *wnd);
void        _TexuEVProc_OnSetFocus(texu_wnd *, texu_wnd *);
texu_i32    _TexuEVProc_OnKillFocus(texu_wnd *, texu_wnd *, texu_i32 state);
texu_i32    _TexuEVProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
void        _TexuEVProc_OnSetText(texu_wnd *wnd, const texu_char *text);
void        _TexuEVProc_OnPaint(texu_wnd *wnd, texu_cio *dc, texu_rect* rect);
void        _TexuEVProc_OnSetLot(texu_wnd* wnd, texu_ui32 lot);
texu_ui32   _TexuEVProc_OnGetLot(texu_wnd* wnd);
texu_bool   _TexuEVProc_OnSetVolume(texu_wnd* wnd, texu_ui32 vol);
texu_ui32   _TexuEVProc_OnGetVolume(texu_wnd* wnd);
texu_bool   _TexuEVProc_OnAllowOddLot(texu_wnd* wnd, texu_bool odd);
void        _TexuEVProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw);
texu_bool   _TexuEVProc_OnShow(texu_wnd *wnd, texu_bool visible);


texu_bool   _TexuEVProc_OnShow(texu_wnd *wnd, texu_bool visible)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    return texu_wnd_visible(ev->volwnd, visible);
}

void
_TexuEVProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_urect rc;
    rc.r1 = *rect;
    rc.r2.y = 0;
    rc.r2.x = 0;
    rc.r2.height   = 1;
    texu_wnd_move(ev->volwnd, rc.r2.y, rc.r2.x, rc.r2.height, rc.r2.width, redraw);
}

void _TexuEVProc_OnSetLot(texu_wnd* wnd, texu_ui32 lot)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    if (lot > 0)
    {
        ev->lot = lot;
    }
}
texu_ui32 _TexuEVProc_OnGetLot(texu_wnd* wnd)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    return (ev->lot);
}
texu_bool _TexuEVProc_OnSetVolume(texu_wnd* wnd, texu_ui32 vol)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    if (ev->odd_lot)
    {
        /*set now*/
        sprintf(buf, "%d", (int)vol);
        texu_wnd_send_msg(wnd, TEXU_WM_SETTEXT, (texu_lparam)buf, 0);
        return TEXU_TRUE;
    }
    else
    {
        /*validate lot first*/
        if ((vol % ev->lot) != 0)
        {
            return TEXU_FALSE;
        }
        else
        {
            /*set now*/
            sprintf(buf, "%d", (int)vol);
            texu_wnd_send_msg(wnd, TEXU_WM_SETTEXT, (texu_lparam)buf, 0);
            return TEXU_TRUE;
        }
    }
    return TEXU_FALSE;
}
texu_ui32    _TexuEVProc_OnGetVolume(texu_wnd* wnd)
{
    texu_char vol[TEXU_MAX_WNDTEXT + 1];
    texu_wnd_get_text(wnd, vol, TEXU_MAX_WNDTEXT);
    return texu_atol(vol);
}
texu_bool   _TexuEVProc_OnAllowOddLot(texu_wnd* wnd, texu_bool odd)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    texu_bool old_odd_lot = ev->odd_lot;
    ev->odd_lot = odd;
    if (ev->odd_lot)
    {
        texu_wnd_send_msg(ev->volwnd, TEXU_UDCM_SETSTEP, 1, 0);
        texu_wnd_send_msg(ev->volwnd, TEXU_UDCM_SETMINMAX, 1, ev->max);
    }
    else
    {
        texu_wnd_send_msg(ev->volwnd, TEXU_UDCM_SETSTEP, ev->lot, 0);
        texu_wnd_send_msg(ev->volwnd, TEXU_UDCM_SETMINMAX, ev->lot, ev->max);
    }
    return old_odd_lot;
}

void _TexuEVProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_env *env = texu_wnd_get_env(wnd);
    /*texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);*/
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    /*texu_char textcommas[TEXU_MAX_WNDTEXT + 1];*/
    texu_char buf[TEXU_MAX_WNDTEXT + 1];

    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITVOLUMECTRL);
    texu_ui32 discolor = texu_env_get_syscolor(env, TEXU_COLOR_EDITVOLUMECTRL_DISABLED);

    texu_ui32 style = texu_wnd_get_style(wnd);
    texu_i32  y = texu_wnd_get_y(wnd);
    texu_i32  x = texu_wnd_get_x(wnd);
    texu_i32  width = texu_wnd_get_width(wnd);
    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITVOLUMECTRL);
    texu_ui32 disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITVOLUMECTRL_DISABLED);

#if 1 //!(defined TEXU_CIO_COLOR_MONO)
    texu_ui32 bgcolor = normbg;
#endif
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

    color = (texu_wnd_is_enable(wnd) ? normcolor : discolor);

    texu_wnd_get_text(wnd, text, TEXU_MAX_WNDTEXT);

    texu_printf_alignment3(buf, text, width, style, TEXU_TRUE, x, cx);
#if 0 //defined TEXU_CIO_COLOR_MONO
    texu_cio_putstr_attr(cio, y, x, buf,
                         texu_cio_get_color(cio, color));
#else
    bgcolor = (texu_wnd_is_enable(wnd) ? normbg : disbg);
    texu_cio_draw_text(cio, y, x, buf, color, bgcolor,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
#endif
}

void
_TexuEVProc_OnDestroy(texu_wnd *wnd)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    free(ev);
}

texu_status
_TexuEVProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_edit_volume *ev = 0;
    texu_wnd_attrs attrs2;
    /*texu_wnd *editwnd = 0;*/
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd *volwnd = 0;
/*
    editwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!editwnd)
    {
        return TEXU_NOMEM;
    }
*/
    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y = 0;
    attrs2.x = 0;
    attrs2.height = attrs->height;
    attrs2.width = attrs->width;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = attrs->text;
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED);

    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_UPDOWNCTRL_FOCUSED);

    attrs2.id = EDITVOL_EDIT_ID;
    /*attrs2.clsname = TEXU_EDIT_CLASS;*/
    attrs2.userdata = 0;
    /*attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_LEFT | TEXU_ES_NUMBER | TEXU_WS_HIDE;*/
    attrs2.exstyle = 0;
    attrs2.on_validate = attrs->on_validate;
    attrs2.validate_data = attrs->validate_data;
    attrs2.userdata2 = attrs->userdata2;

    /*rc = texu_wnd_create(editwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(editwnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(editwnd, TEXU_EM_SETDECWIDTH, 2, 0);*//*2-decimal points*/

    volwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!volwnd)
    {
        return TEXU_NOMEM;
    }
    /*attrs2.id = EDITVOL_UPDOWN_ID;*/
    /*attrs2.visible = TEXU_FALSE;*//*will be activated if it was set odd lot to "FALSE"*/
    attrs2.clsname = TEXU_UPDOWNCTRL_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_UDS_SHOWPLUSMINUS;/* | TEXU_WS_HIDE;*/
    rc = texu_wnd_create(volwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        /*texu_wnd_del(editwnd);*/
        texu_wnd_del(volwnd);
        return TEXU_ERROR;
    }

    ev = (texu_edit_volume *)malloc(sizeof(texu_edit_volume));
    if (!ev)
    {
        return TEXU_ERROR;
    }
    memset(ev, 0, sizeof(texu_edit_volume));
    ev->lot = 100;
    ev->odd_lot = TEXU_FALSE;
    ev->max = 2000000000;
    /*ev->editwnd = editwnd;*/ /* no parameter */
    ev->volwnd = volwnd;
    /*texu_wnd_visible(ev->editwnd, TEXU_FALSE);*/ 
    /*texu_wnd_visible(ev->volwnd, TEXU_FALSE);*/
    texu_wnd_send_msg(volwnd, TEXU_UDCM_SETMINMAX, 100, ev->max);
    texu_wnd_send_msg(volwnd, TEXU_UDCM_SETSTEP, 100, 0);
    texu_wnd_send_msg(volwnd, TEXU_UDCM_SETPAGE, 1000, 0);

    /* save memory */
    texu_wnd_set_userdata(wnd, ev);
    /* set default color*/
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITVOLUMECTRL),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDITVOLUMECTRL_DISABLED));
    texu_wnd_set_focused_color(wnd,
                               texu_env_get_syscolor(env, TEXU_COLOR_EDITVOLUMECTRL_FOCUSED));

    texu_wnd_set_bgcolor(wnd,
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITVOLUMECTRL),
                         texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITVOLUMECTRL_DISABLED));
    texu_wnd_set_bgfocused_color(wnd,
                                 texu_env_get_sysbgcolor(env, TEXU_COLOR_EDITVOLUMECTRL_FOCUSED));

    return TEXU_OK;
}

void
_TexuEVProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    if (!texu_wnd_is_enable(wnd) || !texu_wnd_is_visible(wnd))
    {
        return;
    }
    /*set focust to the first edit window*/
#if 0
    if (ev->odd_lot)
    {
        texu_wnd_visible(ev->volwnd, TEXU_FALSE);
        texu_wnd_visible(ev->editwnd, TEXU_TRUE);
        texu_wnd_send_msg(ev->editwnd, TEXU_WM_SETFOCUS, 0, 0);
    }
    else
    {
        texu_wnd_visible(ev->editwnd, TEXU_FALSE);
        texu_wnd_visible(ev->volwnd, TEXU_TRUE);
        texu_wnd_send_msg(ev->volwnd, TEXU_WM_SETFOCUS, 0, 0);
    }
#else
    /*texu_wnd_visible(ev->editwnd, TEXU_FALSE);*/
    texu_wnd_visible(ev->volwnd, TEXU_TRUE);
    texu_wnd_send_msg(ev->volwnd, TEXU_WM_SETFOCUS, 0, 0);
#endif
    _TexuWndProc_Notify(wnd, TEXU_EVN_SETFOCUS);
}

texu_i32
_TexuEVProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
{
    /*update value to window text */
    texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd *child = texu_wnd_get_activechild(wnd);
    texu_ui32 vol = ev->lot;

    /*texu_wnd_get_text(wnd, ev->editbuf, TEXU_MAX_WNDTEXT);*/
    texu_wnd_send_msg(child, TEXU_WM_KILLFOCUS, 0, state);
    texu_wnd_get_text(child, buf, TEXU_MAX_WNDTEXT);

    if (TEXU_FALSE == ev->odd_lot)
    {
        vol = texu_atol(buf);
        if ((vol % ev->lot) != 0)
        {
            _TexuWndProc_Notify(wnd, TEXU_EVN_INVALIDLOT);
            return TEXU_BREAK;
        }
    }
    texu_wnd_set_text(wnd, buf);

    _TexuWndProc_Notify(wnd, TEXU_EVN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);

    /*texu_wnd_visible(ev->volwnd, TEXU_FALSE);*/
    /*texu_wnd_visible(ev->editwnd, TEXU_FALSE);*/
    texu_wnd_invalidate(wnd);
    return TEXU_CONTINUE;
}

void
_TexuEVProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    /*texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);*/
    texu_wnd *child = texu_wnd_get_activechild(wnd);
    texu_env *env = texu_wnd_get_env(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
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
                texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, (texu_lparam)wnd, (texu_lparam)texu_env_get_moveprev(env));
                if (rc >= TEXU_OK)
                {
                    texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                }
            }
            break;
        }
#if 0
        case TEXU_KEY_DOWN:
        /*texu_wnd_invalidate(wnd);*/
        /*move next*/
        {
            texu_wnd *parent = texu_wnd_get_parent(wnd);
            texu_wnd *nextwnd = texu_wnd_get_next_activechild(parent, wnd);
            if (nextwnd)
            {
                /*move previous means do nothing if there are any changes*/
                texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, (texu_lparam)wnd, (texu_lparam)texu_env_get_moveprev(env));
                if (rc >= TEXU_OK)
                {
                    texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                }
            }
            break;
        }
#endif
        default:
        /*if (TEXU_FALSE == ev->odd_lot)*/
        {
            texu_wnd_send_msg(child, TEXU_WM_KEYDOWN, (texu_lparam)ch, 0);
            break;
        }
    }
}

void
_TexuEVProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_wnd *child = texu_wnd_get_activechild(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    texu_wnd_send_msg(child, TEXU_WM_CHAR, (texu_lparam)ch, 0);
}

texu_bool _TexuEVProc_IsVolumeValid(texu_wnd* wnd, const char* vol)
{
    return TEXU_TRUE;
}
void
_TexuEVProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    /*texu_edit_volume *ev = (texu_edit_volume *)texu_wnd_get_userdata(wnd);*/
    texu_wnd *child = texu_wnd_get_activechild(wnd);
    /*texu_char buf[BUFSIZ + 1];*/

    if (TEXU_FALSE == _TexuEVProc_IsVolumeValid(wnd, text))
    {
        return;
    }
    texu_wnd_set_text(child, text);
    TexuDefWndProc(wnd, TEXU_WM_SETTEXT, (texu_lparam)text, 0);
}

texu_longptr
_TexuEditVolumeProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_SHOW:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuEVProc_OnShow(wnd, (texu_bool)param1);
            return 0;

        case TEXU_WM_MOVE:
            TexuDefWndProc(wnd, msg, param1, param2);
            _TexuEVProc_OnMove(wnd, (texu_rect*)param1, (texu_bool)param2);
            return 0;

        case TEXU_WM_KEYDOWN:
            _TexuEVProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuEVProc_OnCreate(wnd, (texu_wnd_attrs *)param1);


        case TEXU_WM_DESTROY:
            _TexuEVProc_OnDestroy(wnd);
            break;

        /*case TEXU_WM_PAINT:
            _TexuEVProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;*/

        case TEXU_WM_SETFOCUS:
            _TexuEVProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuEVProc_OnKillFocus(wnd, (texu_wnd *)param1, param2);

        case TEXU_WM_SETTEXT:
            _TexuEVProc_OnSetText(wnd, (const texu_char *)param1);
            return 0;

        case TEXU_EVM_SETLOT:
            _TexuEVProc_OnSetLot(wnd, (texu_ui32)param1);
            return 0;

        case TEXU_EVM_GETLOT:
            return _TexuEVProc_OnGetLot(wnd);

        case TEXU_EVM_SETVOLUME:
            return _TexuEVProc_OnSetVolume(wnd, (texu_ui32)param1);

        case TEXU_EVM_GETVOLUME:
            return _TexuEVProc_OnGetVolume(wnd);

        case TEXU_EVM_ALLOWODDLOT:
            _TexuEVProc_OnAllowOddLot(wnd, (texu_bool)param1);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}



/*CLOCKCTRL*/
/*
#-------------------------------------------------------------------------------
# TexU ClockCtrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#include <time.h>

void            _TexuClockCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect);
texu_status     _TexuClockCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void            _TexuClockCtrlProc_GetCurTime(texu_char* ptime );
void            _TexuClockCtrlProc_UpdateTime(texu_wnd *wnd);

void
_TexuClockCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
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
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
        colorbg = disbg;
    }
    /*clock should never got cursor */
    /*texu_env_save_curpos(env, -1, -1);*/
    texu_cio_draw_text(cio, y, x, buf, color, colorbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));
    /*texu_env_restore_curpos(env);*/

}

texu_status
_TexuClockCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_env *env = texu_wnd_get_env(wnd);
    texu_wnd_enable(wnd, TEXU_FALSE);
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_syscolor(env, TEXU_COLOR_LABEL_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_LABEL_DISABLED));

    return TEXU_OK;
}

void _TexuClockCtrlProc_GetCurTime(texu_char* ptime )
{
    time_t      time_location;
    struct tm   tmbuf, *tm_ptr;

    time( &time_location );
    tm_ptr = &tmbuf;
    tm_ptr = localtime( &time_location );
    strftime((char*)ptime, 12, "%H:%M:%S %p", tm_ptr);
}

void _TexuClockCtrlProc_UpdateTime(texu_wnd *wnd)
{
    texu_char now[16];
    
    _TexuClockCtrlProc_GetCurTime(now);
    texu_wnd_set_text(wnd, now);
}


texu_longptr
_TexuClockCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
            return _TexuClockCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_PAINT:
            _TexuClockCtrlProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_ENABLE:
            return TexuDefWndProc(wnd, msg, 0, 0);

        case TEXU_CLKM_UPDATE:
            _TexuClockCtrlProc_UpdateTime(wnd);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}
/*
#-------------------------------------------------------------------------------
# TexU DateCtrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_datewnd
{
    texu_wnd*       daywnd;
    texu_wnd*       monwnd;
    texu_wnd*       yearwnd;
    texu_wnd*       editwnd[3];
    texu_i32        activeidx;
    texu_i32        format; /*YYYYMMDD = 0 (default), DDMMYYYY = 1*/
};
typedef struct texu_datewnd texu_datewnd;
#define DATECTRL_DAY_ID     1
#define DATECTRL_MONTH_ID   2
#define DATECTRL_YEAR_ID    3

texu_status     _TexuDateCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void            _TexuDateCtrlProc_OnDestroy(texu_wnd *wnd);
void            _TexuDateCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect);
void _TexuDateCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd);
void _TexuDateCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_i32 _TexuDateCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state);

texu_i32
_TexuDateCtrlProc_OnKillFocus(texu_wnd *wnd, texu_wnd *prevwnd, texu_i32 state)
{
    /*update value to window text */
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    texu_i32 idx = datewnd->activeidx;
    texu_date date;
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    if (idx < 0 || idx > 2)
    {
        idx = 2;
    }
    texu_wnd_send_msg(datewnd->editwnd[idx], TEXU_WM_KILLFOCUS, 0, state);

    texu_wnd_send_msg(wnd, TEXU_DTM_GETDATE, (texu_lparam)&date, 0);
    
    /*check date*/
    if ((date.year % 4 == 0) || (date.year % 400 == 0))
    {
        /*leap year*/
        if ((date.month == 2) && (date.day > 29))
        {
            _TexuWndProc_Notify(wnd, TEXU_DTN_INVALIDDATE);
            return TEXU_BREAK;
        }
    }
    else
    {
        if ((date.month == 2) && (date.day > 28))
        {
            _TexuWndProc_Notify(wnd, TEXU_DTN_INVALIDDATE);
            return TEXU_BREAK;
        }
    }
    switch (date.month)
    {
        case 2:
            break;
        /*case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:*/
        default:
        {
            if (date.day > 31)
            {
                _TexuWndProc_Notify(wnd, TEXU_DTN_INVALIDDATE);
                return TEXU_BREAK;
            }
            break;
        }
        case 4:
        case 6:
        case 9:
        case 11:
        {
            if (date.day > 30)
            {
                _TexuWndProc_Notify(wnd, TEXU_DTN_INVALIDDATE);
                return TEXU_BREAK;
            }
            break;
        }
    }

    if (TEXU_DTFMT_DDMMYYYY == datewnd->format)
    {
        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d/%d/%d"), date.day, date.month, date.year);
    }
    else if (TEXU_DTFMT_YYYYMMDD == datewnd->format)
    {
        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d/%d/%d"), date.year, date.month, date.day);
    }
    else
    {
        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d/%d/%d"), date.day, date.month, date.year);
    }
    texu_wnd_set_text(wnd, buf); /*text should be AAA.BBB.CCC.DDD*/

    _TexuWndProc_Notify(wnd, TEXU_DTN_KILLFOCUS);
    texu_env_show_cursor(texu_wnd_get_env(wnd), TEXU_FALSE);
    return TEXU_CONTINUE;
}

texu_status
_TexuDateCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_wnd_attrs attrs2;
    texu_wnd *daywnd = 0;
    texu_wnd *monwnd = 0;
    texu_wnd *yearwnd = 0;

    texu_status rc = TEXU_OK;

    texu_env *env = texu_wnd_get_env(wnd);
    texu_editminmax vmm = { 1, 31 };
    texu_datewnd *datewnd = 0;

    daywnd = texu_wnd_new(env);
    if (!daywnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y = 0;//attrs->y;
    attrs2.x = 0;//attrs->x;
    attrs2.height = attrs->height;
    attrs2.width = 2;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = TEXUTEXT("1");
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_FOCUSED);

    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_FOCUSED);
    attrs2.id = DATECTRL_DAY_ID;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_NUMBER | TEXU_ES_RIGHT;
    attrs2.exstyle = 0;
    /*day*/
    rc = texu_wnd_create(daywnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(daywnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(daywnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    texu_wnd_send_msg(daywnd, TEXU_EM_LIMITTEXT, 2, 0);
    /*month*/
    monwnd = texu_wnd_new(env);
    if (!monwnd)
    {
        texu_wnd_del(daywnd);
        return TEXU_NOMEM;
    }

    attrs2.id = DATECTRL_MONTH_ID;
    attrs2.width = 2;
    attrs2.x += 3;
    rc = texu_wnd_create(monwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(daywnd);
        texu_wnd_del(monwnd);
        return TEXU_ERROR;
    }
    vmm.max = 12;
    texu_wnd_send_msg(monwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    texu_wnd_send_msg(monwnd, TEXU_EM_LIMITTEXT, 2, 0);
    /*year*/
    yearwnd = texu_wnd_new(env);
    if (!yearwnd)
    {
        texu_wnd_del(daywnd);
        texu_wnd_del(monwnd);
        return TEXU_NOMEM;
    }
    attrs2.id = DATECTRL_YEAR_ID;
    attrs2.width = 4;
    attrs2.x += 3;
    attrs2.text = TEXUTEXT("1970");

    rc = texu_wnd_create(yearwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(daywnd);
        texu_wnd_del(monwnd);
        texu_wnd_del(yearwnd);
        return TEXU_ERROR;
    }
    vmm.min = 0;
    vmm.max = 9999;
    texu_wnd_send_msg(yearwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    texu_wnd_send_msg(yearwnd, TEXU_EM_LIMITTEXT, 4, 0);

    datewnd = (texu_datewnd*)malloc(sizeof(texu_datewnd));
    if (!datewnd)
    {
        texu_wnd_del(daywnd);
        texu_wnd_del(monwnd);
        texu_wnd_del(yearwnd);
        return TEXU_ERROR;
    }
    datewnd->daywnd  = daywnd;
    datewnd->monwnd  = monwnd;
    datewnd->yearwnd = yearwnd;
    datewnd->format  = TEXU_DTFMT_DDMMYYYY;
    datewnd->editwnd[0] = daywnd;
    datewnd->editwnd[1] = monwnd;
    datewnd->editwnd[2] = yearwnd;
    datewnd->activeidx  = 0;

    texu_wnd_set_userdata(wnd, datewnd);
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED));

    return TEXU_OK;
}

void
_TexuDateCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    free(datewnd);
    texu_wnd_set_userdata(wnd, 0);
}

void
_TexuDateCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);

    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    texu_ui32 discolor  = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);

    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    texu_ui32 disbg  = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 colorbg = normbg;

    static texu_char ddmmyyyy[] = TEXUTEXT("  /  /    ");
    static texu_char yyyymmdd[] = TEXUTEXT("    /  /  ");
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }

    if (TEXU_DTFMT_YYYYMMDD == datewnd->format)
    {
        texu_strcpy(text, yyyymmdd);
    }
    else
    {
        texu_strcpy(text, ddmmyyyy);
    }

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = normcolor;
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
    }
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    colorbg = normbg;
    if (!(texu_wnd_is_enable(wnd)))
    {
        colorbg = disbg;
    }
    texu_cio_draw_text(cio, y, x, text, color, colorbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

void _TexuDateCtrlProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    datewnd->activeidx = 0;
    texu_wnd_send_msg(datewnd->editwnd[0], TEXU_WM_SETFOCUS, 0, 0);
    _TexuWndProc_Notify(wnd, TEXU_DTN_SETFOCUS);
}

void
_TexuDateCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 val = 0;
    texu_bool updown = TEXU_FALSE;
    texu_i32 added = 0;
    texu_wnd *editwnd = 0;
    texu_wnd *nextwnd = 0;
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    texu_status rc = TEXU_OK;
    texu_i32 idx = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
    idx = datewnd->activeidx;
    if (idx < 0 || idx > 2)
    {
        idx = 2;
        datewnd->activeidx = 2;
    }
    editwnd = datewnd->editwnd[idx];//texu_wnd_get_activechild(wnd);
    switch (ch)
    {
        /*case TEXU_KEY_UP:
        {
            added += 1;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_DOWN:
        {
            added -= 1;
            updown = TEXU_TRUE;
            break;
        }*/

        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);

                if (prevwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
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
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            break;
        case TEXU_KEY_NPAGE:
        {
            if (editwnd == datewnd->daywnd)
            {
                added -= 7;
            }
            else if (editwnd == datewnd->monwnd)
            {
                added -= 3;
            }
            else if (editwnd == datewnd->yearwnd)
            {
                added -= 10;
            }
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_PPAGE:
        {
            if (editwnd == datewnd->daywnd)
            {
                added += 7;
            }
            else if (editwnd == datewnd->monwnd)
            {
                added += 3;
            }
            else if (editwnd == datewnd->yearwnd)
            {
                added += 10;
            }
            updown = TEXU_TRUE;
            break;
        }

        case TEXU_KEY_LEFT:
        {
            /*move to the prev edit window*/
            idx = datewnd->activeidx - 1;
            if (idx < 0)
            {
                idx = 0;
            }
            nextwnd = datewnd->editwnd[idx];
            if (!nextwnd)
            {
                return;
            }
            else
            {
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
                    datewnd->activeidx = idx;
                }
                return;
            }
            break;
        }

        case TEXU_KEY_RIGHT:
        case TEXUTEXT('/'):
        {
            /*move to the next edit window*/
            idx = datewnd->activeidx + 1;
            if (idx > 2)
            {
                idx = 2;
            }
            nextwnd = datewnd->editwnd[idx];
            if (!nextwnd)
            {
                return;
            }
            else
            {
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
                    datewnd->activeidx = idx;
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

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
        texu_wnd_set_text(editwnd, buf);
        texu_wnd_send_msg(editwnd, TEXU_WM_SETFOCUS, 0, 0);

        texu_wnd_set_text(wnd, buf); /*text should be HH:MM:SS*/
    }
    else
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)ch, 0);
    }
}

void _TexuDateCtrlProc_OnSetFormat(texu_wnd *wnd, texu_i32 format)
{
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    texu_rect rcwnd;
    if (format == datewnd->format)
    {
        return;
    }
    
    texu_wnd_get_rect(wnd, &rcwnd);
    if (TEXU_DTFMT_DDMMYYYY == format)
    {
        datewnd->format = format;
        datewnd->editwnd[0] = datewnd->daywnd;
        datewnd->editwnd[1] = datewnd->monwnd;
        datewnd->editwnd[2] = datewnd->yearwnd;
        texu_wnd_move(datewnd->editwnd[0], rcwnd.y, rcwnd.x + 0, 1, 2, TEXU_TRUE);
        texu_wnd_move(datewnd->editwnd[1], rcwnd.y, rcwnd.x + 3, 1, 2, TEXU_TRUE);
        texu_wnd_move(datewnd->editwnd[2], rcwnd.y, rcwnd.x + 6, 1, 4, TEXU_TRUE);
        texu_wnd_send_msg(datewnd->editwnd[0], TEXU_WM_SETFOCUS, 0, 0);
    }
    else if (TEXU_DTFMT_YYYYMMDD == format)
    {
        datewnd->format = format;
        datewnd->editwnd[0] = datewnd->yearwnd;
        datewnd->editwnd[1] = datewnd->monwnd;
        datewnd->editwnd[2] = datewnd->daywnd;
        texu_wnd_move(datewnd->editwnd[0], rcwnd.y, rcwnd.x + 0, 1, 4, TEXU_TRUE);
        texu_wnd_move(datewnd->editwnd[1], rcwnd.y, rcwnd.x + 5, 1, 2, TEXU_TRUE);
        texu_wnd_move(datewnd->editwnd[2], rcwnd.y, rcwnd.x + 8, 1, 2, TEXU_TRUE);
        texu_wnd_send_msg(datewnd->editwnd[0], TEXU_WM_SETFOCUS, 0, 0);
    }
}

void _TexuDateCtrlProc_OnGetDate(texu_wnd *wnd, texu_date* date)
{
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_wnd_get_text(datewnd->daywnd, buf, TEXU_MAX_WNDTEXT);
    date->day = atoi(buf);
    
    texu_wnd_get_text(datewnd->monwnd, buf, TEXU_MAX_WNDTEXT);
    date->month = atoi(buf);

    texu_wnd_get_text(datewnd->yearwnd, buf, TEXU_MAX_WNDTEXT);
    date->year = atoi(buf);
}

void _TexuDateCtrlProc_OnSetDate(texu_wnd *wnd, const texu_date* date)
{
    texu_datewnd *datewnd = (texu_datewnd*)texu_wnd_get_userdata(wnd);
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    sprintf(buf, "%d", (int)date->day);
    texu_wnd_set_text(datewnd->daywnd, buf);
    
    sprintf(buf, "%d", (int)date->month);
    texu_wnd_set_text(datewnd->monwnd, buf);

    sprintf(buf, "%d", (int)date->year);
    texu_wnd_set_text(datewnd->yearwnd, buf);
}

texu_longptr _TexuDateCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_DTM_SETFORMAT:
            _TexuDateCtrlProc_OnSetFormat(wnd, (texu_i32)param1);
            return 0;
        case TEXU_DTM_SETDATE:
            _TexuDateCtrlProc_OnSetDate(wnd, (const texu_date*)param1);
            return 0;
        case TEXU_DTM_GETDATE:
            _TexuDateCtrlProc_OnGetDate(wnd, (texu_date*)param1);
            return 0;
            
        case TEXU_WM_CHAR:
            _TexuDateCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_CREATE:
            return _TexuDateCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_SETFOCUS:
            _TexuDateCtrlProc_OnSetFocus(wnd, (texu_wnd*)param1);
            return 0;
            
        case TEXU_WM_KILLFOCUS:
            return _TexuDateCtrlProc_OnKillFocus(wnd, (texu_wnd*)param1, (texu_i32)param2);

        case TEXU_WM_DESTROY:
            _TexuDateCtrlProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuDateCtrlProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}
/*
#-------------------------------------------------------------------------------
# TexU TimeCtrl
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
struct texu_timewnd
{
    texu_wnd*       hourwnd;
    texu_wnd*       minwnd;
    texu_wnd*       secwnd;
    texu_i32        format; /*HH_MM_SS = 0 (Default), HH_MM = 1*/
    texu_char       stime[16];
};
typedef struct texu_timewnd texu_timewnd;
texu_status     _TexuTimeCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs);
void            _TexuTimeCtrlProc_OnDestroy(texu_wnd *wnd);
void            _TexuTimeCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect);
void            _TexuTimeCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
texu_i32        _TexuTimeCtrlProc_OnGetTime(texu_wnd* wnd, texu_char* textout, texu_i32 textlen);

texu_status
_TexuTimeCtrlProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    texu_wnd_attrs attrs2;
    texu_wnd *hourwnd = 0;
    texu_wnd *minwnd = 0;
    texu_wnd *secwnd = 0;

    texu_status rc = TEXU_OK;

    texu_env *env = texu_wnd_get_env(wnd);
    texu_editminmax vmm = { 0, 23 };
    texu_timewnd *timewnd = 0;

    hourwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!hourwnd)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs2, 0, sizeof(attrs2));
    attrs2.y = attrs->y;
    attrs2.x = attrs->x;
    attrs2.height = attrs->height;
    attrs2.width = 2;
    attrs2.enable = TEXU_TRUE;
    attrs2.visible = TEXU_TRUE;
    attrs2.text = TEXUTEXT("0");
    attrs2.normalcolor      = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledcolor    = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor    = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedcolor     = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_FOCUSED);

    attrs2.normalbg         = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    attrs2.disabledbg       = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    attrs2.selectedcolor    = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_SELECTED);
    attrs2.focusedbg        = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_FOCUSED);

    attrs2.id = 1;
    attrs2.clsname = TEXU_EDIT_CLASS;
    attrs2.userdata = 0;
    attrs2.style = TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER | TEXU_ES_RIGHT;
    attrs2.exstyle = 0;
    /*hour*/
    rc = texu_wnd_create(hourwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(hourwnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(hourwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    /*minute*/
    minwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!minwnd)
    {
        return TEXU_NOMEM;
    }
    attrs2.id  = 2;
    attrs2.x  += 3;
    rc = texu_wnd_create(minwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(hourwnd);
        texu_wnd_del(minwnd);
        return TEXU_ERROR;
    }
    vmm.max = 59;
    texu_wnd_send_msg(minwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);
    /*second*/
    secwnd = texu_wnd_new(texu_wnd_get_env(wnd));
    if (!minwnd)
    {
        return TEXU_NOMEM;
    }
    attrs2.id = 3;
    attrs2.x  += 3;
    rc = texu_wnd_create(secwnd, wnd, &attrs2);
    if (rc != TEXU_OK)
    {
        texu_wnd_del(hourwnd);
        texu_wnd_del(minwnd);
        texu_wnd_del(secwnd);
        return TEXU_ERROR;
    }
    texu_wnd_send_msg(secwnd, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&vmm);

    timewnd = (texu_timewnd*)malloc(sizeof(texu_timewnd));
    if (!timewnd)
    {
        texu_wnd_del(hourwnd);
        texu_wnd_del(minwnd);
        texu_wnd_del(secwnd);
        return TEXU_ERROR;
    }
    timewnd->hourwnd = hourwnd;
    timewnd->minwnd  = minwnd;
    timewnd->secwnd  = secwnd;

    texu_wnd_set_userdata(wnd, timewnd);
    texu_wnd_set_color(wnd,
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED));

    texu_wnd_set_bgcolor(wnd,
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT),
                       texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED));

    return TEXU_OK;
}

void
_TexuTimeCtrlProc_OnDestroy(texu_wnd *wnd)
{
    texu_timewnd *timewnd = (texu_timewnd*)texu_wnd_get_userdata(wnd);
    free(timewnd);
    texu_wnd_set_userdata(wnd, 0);
}

void
_TexuTimeCtrlProc_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect* rect)
{

    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 y = texu_wnd_get_y(wnd);
    texu_i32 x = texu_wnd_get_x(wnd);

    texu_env *env = texu_wnd_get_env(wnd);
    texu_ui32 normcolor = texu_env_get_syscolor(env, TEXU_COLOR_EDIT);
    texu_ui32 discolor  = texu_env_get_syscolor(env, TEXU_COLOR_EDIT_DISABLED);

    texu_ui32 color = normcolor;

    texu_ui32 normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT);
    texu_ui32 disbg  = texu_env_get_sysbgcolor(env, TEXU_COLOR_EDIT_DISABLED);
    texu_ui32 colorbg = normbg;

    static texu_char hhmmss[] = TEXUTEXT("  :  :  ");
    static texu_char hhmm[] = TEXUTEXT("  :  ");
    texu_timewnd *timewnd = (texu_timewnd*)texu_wnd_get_userdata(wnd);

    if (!texu_wnd_can_paint(wnd))
    {
        return;
    }
    if (TEXU_FALSE == texu_wnd_is_visible(wnd) || TEXU_FALSE == texu_wnd_is_parent_visible(wnd))
    {
        return;
    }

    if (TEXU_TMFMT_HHMM == timewnd->format)
    {
        texu_strcpy(text, hhmm);
    }
    else
    {
        texu_strcpy(text, hhmmss);
    }

    texu_wnd_get_color(wnd, &normcolor, &discolor);
    color = normcolor;
    if (!(texu_wnd_is_enable(wnd)))
    {
        color = discolor;
    }
    texu_wnd_get_bgcolor(wnd, &normbg, &disbg);
    colorbg = normbg;
    if (!(texu_wnd_is_enable(wnd)))
    {
        colorbg = disbg;
    }
    texu_cio_draw_text(cio, y, x, text, color, colorbg,
                          texu_wnd_get_clsname(wnd),
                          texu_wnd_get_id(wnd));

}

void
_TexuTimeCtrlProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
/*    texu_timewnd *timewnd = 0;*/
    texu_char buf[TEXU_MAX_WNDTEXT + 1];
    texu_i32 val = 0;
    texu_bool updown = TEXU_FALSE;
    texu_i32 added = 0;
    texu_wnd *editwnd = 0;
    texu_wnd *nextwnd = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    texu_status rc = TEXU_OK;

    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
/*    timewnd = (texu_timewnd *)texu_wnd_get_userdata(wnd);*/
    editwnd = texu_wnd_get_activechild(wnd);
    switch (ch)
    {
        /*case TEXU_KEY_UP:
        {
            added += 1;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_DOWN:
        {
            added -= 1;
            updown = TEXU_TRUE;
            break;
        }*/

        case TEXU_KEY_UP:
            /*texu_wnd_invalidate(wnd);*/
            /*move previous*/
            {
                texu_wnd *parent = texu_wnd_get_parent(wnd);
                texu_wnd *prevwnd = texu_wnd_get_prev_activechild(parent, wnd);

                if (prevwnd)
                {
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_moveprev(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(prevwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
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
                    texu_i32 rc = texu_wnd_send_msg(wnd, TEXU_WM_KILLFOCUS, 0, texu_env_get_movenext(env));
                    if (rc >= TEXU_OK)
                    {
                        texu_wnd_post_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)wnd, 0);
                        return;
                    }
                }
            }
            break;
        case TEXU_KEY_NPAGE:
        {
            added -= 5;
            updown = TEXU_TRUE;
            break;
        }
        case TEXU_KEY_PPAGE:
        {
            added += 5;
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
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
                }
                return;
            }
            break;
        }

        case TEXU_KEY_RIGHT:
        case TEXUTEXT(':'):
        {
            /*move to the next edit window*/
            nextwnd = texu_wnd_get_next_activechild(wnd, editwnd);
            if (!nextwnd)
            {
                return;
            }
            else
            {
                rc = texu_wnd_send_msg(editwnd, TEXU_WM_KILLFOCUS, (texu_lparam)nextwnd, ch);
                if (rc >= 0)
                {
                    texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_lparam)editwnd, 0);
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
        /*if (val < ipctl->min)
        {
            val = ipctl->min;
        }
        if (val > ipctl->max)
        {
            val = ipctl->max;
        }*/

        /*_TexuWndProc_Notify(wnd, TEXU_IPN_STEP);*/

        texu_sprintf(buf, sizeof(buf), TEXUTEXT("%d"), val);
        texu_wnd_set_text(editwnd, buf);
        texu_wnd_send_msg(editwnd, TEXU_WM_SETFOCUS, 0, 0);

        texu_wnd_send_msg(wnd, TEXU_TMM_GETTIME, (texu_lparam)(texu_char*)buf, TEXU_MAX_WNDTEXT);

        texu_wnd_set_text(wnd, buf); /*text should be HH:MM:SS*/
    }
    else
    {
        texu_wnd_send_msg(editwnd, TEXU_WM_CHAR, (texu_lparam)ch, 0);
    }
}

texu_i32 _TexuTimeCtrlProc_OnGetTime(texu_wnd* wnd, texu_char* textout, texu_i32 textlen)
{
    texu_timewnd* timewnd = (texu_timewnd*)texu_wnd_get_userdata(wnd);
    texu_char hh[3];
    texu_char mm[3];
    texu_char ss[3];
    texu_wnd_get_text(timewnd->hourwnd, hh, 3);
    texu_wnd_get_text(timewnd->minwnd,  mm, 3);
    texu_wnd_get_text(timewnd->secwnd,  ss, 3);

    return texu_sprintf(textout, textlen, TEXUTEXT("%02d:%02d:%02d"), hh, mm, ss);
}

texu_longptr _TexuTimeCtrlProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
            return _TexuTimeCtrlProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_DESTROY:
            _TexuTimeCtrlProc_OnDestroy(wnd);
            break;

        case TEXU_WM_PAINT:
            _TexuTimeCtrlProc_OnPaint(wnd, (texu_cio *)param1, (texu_rect*)param2);
            return 0;

        case TEXU_WM_CHAR:
            _TexuTimeCtrlProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_TMM_GETTIME:
            return _TexuTimeCtrlProc_OnGetTime(wnd, (texu_char*)param1, (texu_i32)param2);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}



#ifdef __cplusplus
}
#endif