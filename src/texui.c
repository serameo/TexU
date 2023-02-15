/*
#
# File Name: texui.c
# Author: Seree Rakwong
# Date: 13-MAY-2020
#
# History
# Date              Author          Description
# ---------------------------------------------
# 01-JUL-2020       MEO             Used asynchonous input (UNIX Lecture Notes: Steward Weiss;
                                    Chapter 6 Event Driven Programming)
# 06-JAN-2022       MEO             Implement Windows as text ui
#
*/

#ifdef LINUX
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#endif
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define TEXU_THREAD_SAFE    1

#include "texulib.h"
#include "texutils.h"
#include "texui.h"
#include "texumenu.h"
#include "texust.h"
#include "cJSON.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*
# TexU environment object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
#define TEXU_ENV_SCRFILEW   L"./.texuenv"
#define TEXU_ENV_SCRFILE    TEXU_ENV_SCRFILEW
#else
#define TEXU_ENV_SCRFILEA   "./.texuenv"
#define TEXU_ENV_SCRFILE    TEXU_ENV_SCRFILEA
#endif

#define TEXU_MAX_COLOR      (TEXU_COLOR_DEFAULT + 1)
#define TEXU_ENV_INITIAL    (250)
#define TEXU_ENV_REPEAT     (250)

struct texu_msg
{
    texu_wnd        *wnd;
    texu_ui32       msg;
    texu_i64        param1;
    texu_i64        param2;
};
#if (defined WIN32 && defined _WINDOWS)
struct texu_fontmapping
{
    texu_char       clsname[TEXU_MAX_WNDTEXT + 1];
    texu_ui32       id;
    texu_char       fontname[TEXU_MAX_WNDTEXT + 1];
    texu_i32        size;
    texu_i32        dpi;
    HFONT           hfont;
};
#endif
struct texu_env
{
    texu_stack      *frames;    /* hold window frames */
    texu_cio        *cio;       /* console input/output */
    texu_wnd        *desktop;   /* the root of all window frames */
    texu_list       *wndcls;    /* the registered classes */
    texu_bool       exit;       /* main loop*/
    texu_i32        ypos;       /* cursor position */
    texu_i32        xpos;       /* cursor position */
#if 0
    FILE            *scrfp;     /* to write/read screen */
#endif
    /* system color */
    texu_ui32       syscolors[TEXU_MAX_COLOR];
    texu_ui32       sysbgcolors[TEXU_MAX_COLOR];
    texu_i32        cols;       /* width*/
    texu_i32        lines;      /* height*/
#ifdef USE_TCL_AUTOMATION
    key_t           msgkey;     /* commu key */
    int             msgid;      /* commu id */
    FILE            *fstdout;   /* save info from screen*/
#endif  /*USE_TCL_AUTOMATION*/
    texu_queue      *msgques;   /* hold the posted msg to be called after*/
    texu_wnd        *mainwnd;   /* the root of all window frames */
#if (defined WIN32 && defined _WINDOWS)
    /* window message */
    HINSTANCE   hinst;
    LPVOID      lpData;
    texu_bool   has_cursor;
    texu_i32    hidden_cursor;
    HWND        hwnd;
    HDC         hdc;
    HDC         hmemdc;
    HBITMAP     hbmp;
    HPEN        hpen;
    TEXTMETRIC  tm;
    HFONT       hfont;
    texu_i32    cxChar;
    texu_i32    cxCaps;
    texu_i32    cyChar;
    texu_i32    cyLine;
    texu_i32    cxScreen;
    texu_i32    cyScreen;
    cJSON       *prop;  /*keep configuration from texuenv.json*/
    texu_map    *fonts; /*create font from texuenv.json*/
#endif
    texu_i32    chNextKey;
    texu_i32    chPrevKey;
    texu_wnd    *focuswnd;
    texu_bool   locked_update;
};

void
texu_env_lock_update(texu_env *env, texu_bool locked)
{
    env->locked_update = locked;
}

texu_bool
texu_env_is_update_locked(texu_env *env)
{
    return env->locked_update;
}

#if (defined WIN32 && defined _WINDOWS)
cJSON*
texu_env_get_prop(texu_env *env)
{
    return env->prop;
}

HDC texu_env_get_hdc(texu_env *env)
{
    return env->hdc;
}

HFONT texu_env_get_hfont(texu_env *env)
{
    return env->hfont;
}

void
texu_env_set_memdc(texu_env *env, HDC hmemdc)
{
    if (hmemdc)
    {
        SelectObject(hmemdc, env->hfont);
        SelectObject(hmemdc, env->hpen);
    }
}

HDC texu_env_get_memdc(texu_env *env)
{
    return env->hmemdc;
}

texu_i64
texu_env_invalidate(texu_env *env)
{
    return InvalidateRect(env->hwnd, NULL, TRUE);
}

#endif

void
texu_env_set_movenext(texu_env *env, texu_i32 nextkey)
{
    env->chNextKey = nextkey;
}

texu_i32
texu_env_get_movenext(texu_env *env)
{
    return env->chNextKey;
}

void
texu_env_set_moveprev(texu_env *env, texu_i32 prevkey)
{
    env->chPrevKey = prevkey;
}

texu_i32
texu_env_get_moveprev(texu_env *env)
{
    return env->chPrevKey;
}

texu_wnd*
texu_env_get_focus(texu_env *env)
{
    return env->focuswnd;
}

void
texu_env_set_focus(texu_env *env, texu_wnd *wnd)
{
    env->focuswnd = wnd;
}

struct texu_env_wndcls
{
    const texu_char *clsname;
    texu_wndproc proc;
};
typedef struct texu_env_wndcls texu_env_wndcls;

/* internally window procedure */
/* see texuproc.c */
texu_i64 _TexuDesktopProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuMsgBoxProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

texu_i64 _TexuLabelProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuButtonProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuEditProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuListBoxProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuComboBoxProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuStatusBarProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

/* see texuctrl.c */
texu_i64 _TexuListCtrlProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuTreeCtrlProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuUpDownCtrlProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuProgressBarProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuPageCtrlProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuReBarProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

/* see texuctrlx.c */
texu_i64 _TexuIPAddressProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuEditMaskProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuEditPriceSpreadProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);


/* menu texumenu.c */
texu_i64 _TexuMenuProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
texu_i64 _TexuMenuWndProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

void _texu_env_init_cls(texu_env *);
texu_wndproc _texu_env_find_wndproc(texu_env *, const texu_char *);
FILE *_texu_env_init_screen(texu_env *env);
void _texu_env_init_syscolors(texu_env *env);

void        _texu_env_init_sysbgcolors(texu_env *env);


#if USE_TCL_AUTOMATION
void        _texu_env_init_tcl_automation(texu_env *env, const char* pathname);
texu_bool   _texu_env_read_tcl_input(texu_env *env, texu_env_msg *envmsg);
void        _texu_env_release_tcl_automation(texu_env *env);
void        _texu_env_reply_tcl_output(texu_env *env, texu_char* resjson);
void        _texu_env_handle_request(texu_env *env, texu_char* reqjson);
char*       _texu_env_get_minify(cJSON *root);
void        _texu_env_cmd_attach(texu_env *env);
void        _texu_env_cmd_detach(texu_env *env);
void        _texu_env_cmd_sendmsg(texu_env *env, cJSON *req);
void        _texu_env_cmd_sendcmd(texu_env *env, cJSON *req);
void        _texu_env_cmd_postmsg(texu_env *env, cJSON *req);
void        _texu_env_cmd_settext(texu_env *env, cJSON *req);
void        _texu_env_cmd_gettext(texu_env *env, cJSON *req);
void        _texu_env_cmd_enable(texu_env *env, cJSON *req, texu_bool fEnable);
#endif /*USE_TCL_AUTOMATION*/

#if USE_TCL_AUTOMATION
char*
_texu_env_get_minify(cJSON *root)
{
    /* declarations */
    char *out = NULL;

    /* formatted print */
    out = cJSON_Print(root);
    if (out)
    {
        cJSON_Minify(out);  /*remove white spaces*/
    }
    return out;
}
#if 0
void
_texu_env_cmd_attach(texu_env *env)
{
    /*accept the attach*/
    char *resjson = NULL;
    cJSON *res = cJSON_CreateObject();

    if (TEXU_TRUE == env->attached)
    {
        /*only 1 client can be attached */
        cJSON_AddNumberToObject(res, "code", 1);
        cJSON_AddStringToObject(res, "errmsg", "there is already client attached");
    }
    else
    {
        cJSON_AddNumberToObject(res, "code", 0);    /*succeeded*/
        env->attached = TEXU_TRUE;
    }

    
    resjson = _texu_env_get_minify(res);
    _texu_env_reply_tcl_output(env, resjson);

    free(resjson);
    cJSON_Delete(res);
}


void
_texu_env_cmd_detach(texu_env *env)
{
    /*accept the attach*/
    char *resjson = NULL;
    cJSON *res = cJSON_CreateObject();

    if (TEXU_FALSE == env->attached)
    {
        cJSON_AddNumberToObject(res, "code", 1);
        cJSON_AddStringToObject(res, "errmsg", "there is no client attached");
    }
    else
    {
        cJSON_AddNumberToObject(res, "code", 0);    /*succeeded*/
        env->attached = TEXU_FALSE;
    }

    
    resjson = _texu_env_get_minify(res);
    _texu_env_reply_tcl_output(env, resjson);

    
    free(resjson);
    cJSON_Delete(res);
}
#endif
void    _texu_env_cmd_sendmsg(texu_env *env, cJSON *req)
{
    char    *resjson = NULL;
    cJSON   *res = cJSON_CreateObject();
    texu_i64 lwnd = 0;
    texu_i32 lmsg = 0;
    texu_i64 lparam1 = 0;
    texu_i64 lparam2 = 0;
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;
    texu_i64 rc = 0;

    cJSON   *parms = cJSON_GetObjectItem(req, "parms");
    cJSON   *val = 0;

    if (!parms)
    {
        cJSON_AddNumberToObject(res, "code", -1);
        cJSON_AddStringToObject(res, "errmsg", "no parameters");
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "wnd");
        lwnd = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "msg");
        lmsg = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "param1");
        lparam1 = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "param2");
        lparam2 = (texu_i64)cJSON_GetNumberValue(val);

        if (lwnd <= 0)
        {
            child = wnd;
        }
        else
        {
            child = texu_wnd_find_child(wnd, lwnd);
        }
        if (child)
        {
            rc = texu_wnd_send_msg(child, lmsg, lparam1, lparam2);
        }
        else
        {
            rc = -1;
        }

        cJSON_AddNumberToObject(res, "code", 0);
        cJSON_AddNumberToObject(res, "rescode", rc);
    }
    
    resjson = _texu_env_get_minify(res);
    _texu_env_reply_tcl_output(env, resjson);

    free(resjson);
    cJSON_Delete(res);
}

void    _texu_env_cmd_postmsg(texu_env *env, cJSON *req)
{
    texu_i64 lwnd = 0;
    texu_i32 lmsg = 0;
    texu_i64 lparam1 = 0;
    texu_i64 lparam2 = 0;
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;

    cJSON *parms = cJSON_GetObjectItem(req, "parms");
    cJSON *val = 0;

    if (!parms)
    {
        return;
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "wnd");
        lwnd = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "msg");
        lmsg = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "param1");
        lparam1 = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "param2");
        lparam2 = (texu_i64)cJSON_GetNumberValue(val);

        if (lwnd <= 0)
        {
            child = wnd;
        }
        else
        {
            child = texu_wnd_find_child(wnd, lwnd);
        }
        if (child)
        {
            texu_wnd_post_msg(child, lmsg, lparam1, lparam2);
        }
    }
}

void    _texu_env_cmd_sendcmd(texu_env *env, cJSON *req)
{
    texu_i64 cmdid = 0;
    texu_wnd *wnd = texu_env_top_wnd(env);

    cJSON   *parms = cJSON_GetObjectItem(req, "parms");
    cJSON   *val = 0;

    if (!parms)
    {
        return;
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "cmdid");
        cmdid = (texu_i64)cJSON_GetNumberValue(val);

        if (wnd)
        {
            texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, cmdid, 0);
        }
    }
}

void    _texu_env_cmd_settext(texu_env *env, cJSON *req)
{
    texu_i64 lwnd = 0;
    texu_char *text;
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;

    cJSON *parms = cJSON_GetObjectItem(req, "parms");
    cJSON *val = 0;

    if (!parms)
    {
        return;
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "wnd");
        lwnd = (texu_i64)cJSON_GetNumberValue(val);

        val = cJSON_GetObjectItem(parms, "text");
        text = cJSON_GetStringValue(val);

        if (lwnd <= 0)
        {
            child = wnd;
        }
        else
        {
            child = texu_wnd_find_child(wnd, lwnd);
        }
        if (child)
        {
            texu_wnd_set_text(child, text);
        }
    }
}


void    _texu_env_cmd_gettext(texu_env *env, cJSON *req)
{
    char *resjson = NULL;
    cJSON *res = cJSON_CreateObject();
    texu_i64 lwnd = 0;
    texu_char text[256];
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;
    texu_ui32 style = 0;
    const texu_char* clsname = "";

    cJSON *parms = cJSON_GetObjectItem(req, "parms");
    cJSON *val = 0;

    if (!parms)
    {
        cJSON_AddNumberToObject(res, "code", -1);
        cJSON_AddStringToObject(res, "errmsg", "no parameters");
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "wnd");
        lwnd = (texu_i64)cJSON_GetNumberValue(val);

        if (lwnd <= 0)
        {
            child = wnd;
        }
        else
        {
            child = texu_wnd_find_child(wnd, lwnd);
        }
        texu_wnd_get_text(child, text, 256);
        style = texu_wnd_get_style(child);
        clsname = texu_wnd_get_clsname(child);
        
        cJSON_AddNumberToObject(res, "code", 0);
        if (child && clsname &&
            (0 == strcmp(clsname, TEXU_EDIT_CLASS) 
            && (style & TEXU_ES_PASSWORD)))
        {
            /* DO NOT SHOW THE PASSWORD BETWEEN INTER PROCESS COMMUNICATION */
            strcpy(text, "********");
        }
        else if (!child || !clsname)
        {
            strcpy(text, "TEXU_ERROR: NOT_FOUND_WINDOW");
            cJSON_AddNumberToObject(res, "code", -1);
        }

        cJSON_AddStringToObject(res, "text", text);
    }
    
    resjson = _texu_env_get_minify(res);
    _texu_env_reply_tcl_output(env, resjson);

        
    free(resjson);
    cJSON_Delete(res);
}

void    _texu_env_cmd_enable(texu_env *env, cJSON *req, texu_bool fEnable)
{
    texu_i64 lwnd = 0;
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;

    cJSON *parms = cJSON_GetObjectItem(req, "parms");
    cJSON *val = 0;

    if (!parms)
    {
        return;
    }
    else
    {
        val = cJSON_GetObjectItem(parms, "wnd");
        lwnd = (texu_i64)cJSON_GetNumberValue(val);

        if (lwnd <= 0)
        {
            child = wnd;
        }
        else
        {
            child = texu_wnd_find_child(wnd, lwnd);
        }
        if (child)
        {
            texu_wnd_enable(child, fEnable);
        }
    }
}

void
_texu_env_handle_request(texu_env *env, texu_char* reqjson)
{
    cJSON *req = cJSON_Parse(reqjson);
    cJSON *cmd = NULL;
    char *resjson = NULL;
    cJSON *res = NULL;
    char *cmdstr;
    FILE    *fstdout    = texu_env_get_stdout(env);
    fprintf(fstdout, "<<< %s\n", reqjson);
    fflush(fstdout);


    if (!req)
    {
        return;
    }
    cmd = cJSON_GetObjectItem(req, "cmd");
    if (!cmd)
    {
        /*invalid cmd*/
        res = cJSON_CreateObject();
        cJSON_AddNumberToObject(res, "code", -1);
        cJSON_AddStringToObject(res, "errmsg", "no cmd object");
        
        resjson = _texu_env_get_minify(res);
        _texu_env_reply_tcl_output(env, resjson);

        free(resjson);
        cJSON_Delete(res);
        return;
    }
    /*reply cmd here*/
    cmdstr = cJSON_GetStringValue(cmd);
    if (0 == strcmp("sendmsg", cmdstr))
    {
        _texu_env_cmd_sendmsg(env, req);
    }
    else if (0 == strcmp("postmsg", cmdstr))
    {
        _texu_env_cmd_postmsg(env, req);
    }
    else if (0 == strcmp("sendcmd", cmdstr))
    {
        _texu_env_cmd_sendcmd(env, req);
    }
    else if (0 == strcmp("settext", cmdstr))
    {
        _texu_env_cmd_settext(env, req);
    }
    else if (0 == strcmp("gettext", cmdstr))
    {
        _texu_env_cmd_gettext(env, req);
    }
    else if (0 == strcmp("enable", cmdstr))
    {
        _texu_env_cmd_enable(env, req, TEXU_TRUE);
    }
    else if (0 == strcmp("disable", cmdstr))
    {
        _texu_env_cmd_enable(env, req, TEXU_FALSE);
    }
    cJSON_Delete(req);
}

void _texu_env_release_tcl_automation(texu_env *env)
{
    msgctl(env->msgid, IPC_RMID, NULL);
}
void
_texu_env_init_tcl_automation(texu_env *env, const char* pathname)
{
    env->msgkey = ftok(pathname, 65);

    // msgget creates a message queue
    // and returns identifier
    env->msgid = msgget(env->msgkey, 0666 | IPC_CREAT);
}

void
_texu_env_reply_tcl_output(texu_env *env, texu_char* json)
{
    FILE    *fstdout    = texu_env_get_stdout(env);
    texu_env_msg envmsg;
    struct timeval tv = { 0, 1500 };
    memset(&envmsg, 0, sizeof(texu_env_msg));
    envmsg.type = TEXU_ENV_MSGTYPE_RESPONSE; /* msg type = 2 (response)*/
    strcpy(envmsg.json, json);

    fprintf(fstdout, ">>> %s\n", json);
    fflush(fstdout);

    msgsnd(env->msgid, &envmsg, sizeof(texu_env_msg), 0);
    /*make a little bit delay*/
    select(0, 0, 0, 0, &tv);
}

texu_bool _texu_env_read_tcl_input(texu_env *env, texu_env_msg *envmsg)
{
    struct timeval tv = { 0, 5000 };
    int rc = 0;
    
    memset(envmsg, 0, sizeof(texu_env_msg));
    select(0, 0, 0, 0, &tv); /*wait for 1 second*/
    rc = msgrcv(env->msgid, envmsg, sizeof(texu_env_msg),
            TEXU_ENV_MSGTYPE_REQUEST,  /*msg type = 1 (reqeust)*/
            IPC_NOWAIT  /*return immediately if no msg*/
            ); 
    return (rc > 0 ? TEXU_TRUE : TEXU_FALSE);
}
#endif /*USE_TCL_AUTOMATION*/

/* can override system color */
void texu_env_set_syscolors(texu_env *env, texu_i32 (*setcolor)(texu_i32))
{
    texu_i32 i = 0;
    texu_i32 color = 0;
    /* init all system colors */
    for (i = 0; i < TEXU_MAX_COLOR; ++i)
    {
        color = setcolor(i);
        if (color < 0)
        {
            continue;
        }
        env->syscolors[i] = color;
    }
}

void texu_env_set_sysbgcolors(texu_env *env, texu_i32(*setcolor)(texu_i32))
{
    texu_i32 i = 0;
    texu_i32 color = 0;
    /* init all system colors */
    for (i = 0; i < TEXU_MAX_COLOR; ++i)
    {
        color = setcolor(i);
        if (color < 0)
        {
            continue;
        }
        env->syscolors[i] = color;
    }
}

void
_texu_env_init_syscolors(texu_env *env)
{
    texu_i32 i = 0;
    /* init all system colors */
    for (i = 0; i < TEXU_MAX_COLOR; ++i)
    {
        env->syscolors[i] = TEXU_CIO_COLOR_WHITE_BLACK;
    }
    env->syscolors[TEXU_COLOR_WINDOW]                   = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_DESKTOP]                  = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_DIALOG]                   = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_TITLE_WINDOW]             = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_BORDER]                   = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_BUTTON_OK]                = TEXU_CIO_COLOR_BLACK_GREEN;
    env->syscolors[TEXU_COLOR_BUTTON_YES]               = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_BUTTON_NO]                = TEXU_CIO_COLOR_BLACK_RED;
    env->syscolors[TEXU_COLOR_BUTTON_CANCEL]            = TEXU_CIO_COLOR_BLACK_YELLOW;
    env->syscolors[TEXU_COLOR_MENU]                     = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_MENU_DISABLED]            = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_MENU_SELECTED]            = TEXU_CIO_COLOR_CYAN_BLUE;
    env->syscolors[TEXU_COLOR_MENU_FOCUSED]             = TEXU_CIO_COLOR_CYAN_BLUE;
    env->syscolors[TEXU_COLOR_MENUITEM]                 = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_MENUITEM_DISABLED]        = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_MENUITEM_SELECTED]        = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_MENUITEM_FOCUSED]         = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_LABEL]                    = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LABEL_DISABLED]           = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_STATUSBAR]                = TEXU_CIO_COLOR_WHITE_BLUE;
    env->syscolors[TEXU_COLOR_EDIT]                     = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDIT_DISABLED]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_EDIT_SELECTED]            = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_EDIT_FOCUSED]             = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDIT_INVALID]             = TEXU_CIO_COLOR_WHITE_RED;
    env->syscolors[TEXU_COLOR_LISTBOX]                  = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_LISTBOX_DISABLED]         = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTBOX_SELECTED]         = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_LISTBOX_FOCUSED]          = TEXU_CIO_COLOR_BLACK_BLUE;
    env->syscolors[TEXU_COLOR_COMBOBOX]                 = TEXU_CIO_COLOR_BLUE_WHITE;
    env->syscolors[TEXU_COLOR_COMBOBOX_DISABLED]        = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_COMBOBOX_SELECTED]        = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_COMBOBOX_FOCUSED]         = TEXU_CIO_COLOR_BLACK_BLUE;
    env->syscolors[TEXU_COLOR_LISTCTRL]                 = TEXU_CIO_COLOR_BLUE_WHITE;
    env->syscolors[TEXU_COLOR_LISTCTRL_DISABLED]        = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTCTRL_SELECTED]        = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTCTRL_FOCUSED]         = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTCTRL_ITEM]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTCTRL_ITEM_DISABLED]   = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_LISTCTRL_ITEM_SELECTED]   = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_LISTCTRL_ITEM_FOCUSED]    = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_TREECTRL]                 = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_DISABLED]        = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_SELECTED]        = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_FOCUSED]         = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_ITEM]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_ITEM_DISABLED]   = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_TREECTRL_ITEM_SELECTED]   = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_TREECTRL_ITEM_FOCUSED]    = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_UPDOWNCTRL]               = TEXU_CIO_COLOR_BLUE_WHITE;
    env->syscolors[TEXU_COLOR_UPDOWNCTRL_DISABLED]      = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_UPDOWNCTRL_SELECTED]      = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_UPDOWNCTRL_FOCUSED]       = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_PROGRESSBAR]              = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_PROGRESSBAR_DISABLED]     = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_PROGRESSBAR_SELECTED]     = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_PROGRESSBAR_FOCUSED]      = TEXU_CIO_COLOR_BLUE_CYAN;
    env->syscolors[TEXU_COLOR_REBAR]                    = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_REBAR_DISABLED]           = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_REBAR_SELECTED]           = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_REBAR_FOCUSED]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_BUTTON]                   = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_BUTTON_DISABLED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_BUTTON_SELECTED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_BUTTON_FOCUSED]           = TEXU_CIO_COLOR_BLACK_WHITE;
    env->syscolors[TEXU_COLOR_IPADDRESSCTRL]            = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_IPADDRESSCTRL_DISABLED]   = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_IPADDRESSCTRL_SELECTED]   = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_IPADDRESSCTRL_FOCUSED]    = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDITMASKCTRL]             = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDITMASKCTRL_DISABLED]    = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_EDITMASKCTRL_SELECTED]    = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_EDITMASKCTRL_FOCUSED]     = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL]          = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED] = TEXU_CIO_COLOR_WHITE_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED] = TEXU_CIO_COLOR_BLACK_CYAN;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED]  = TEXU_CIO_COLOR_CYAN_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_LOWER]    = TEXU_CIO_COLOR_RED_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL]    = TEXU_CIO_COLOR_YELLOW_BLACK;
    env->syscolors[TEXU_COLOR_EDITPRICESPREADCTRL_GREATER]  = TEXU_CIO_COLOR_GREEN_BLACK;
    /*default*/
    env->syscolors[TEXU_COLOR_DEFAULT]                  = TEXU_CIO_COLOR_WHITE_BLACK;
}


void _texu_env_init_sysbgcolors(texu_env *env)
{
    texu_i32 i = 0;
    /* init all system colors */
    for (i = 0; i < TEXU_MAX_COLOR; ++i)
    {
        env->sysbgcolors[i] = TEXU_CIO_COLOR_BLACK_BLACK;
    }
    env->sysbgcolors[TEXU_COLOR_WINDOW]                     = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_DESKTOP]                    = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_DIALOG]                     = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_TITLE_WINDOW]               = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_BORDER]                     = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_BUTTON_OK]                  = TEXU_CIO_COLOR_GREEN_BLACK;
    env->sysbgcolors[TEXU_COLOR_BUTTON_YES]                 = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_BUTTON_NO]                  = TEXU_CIO_COLOR_RED_BLACK;
    env->sysbgcolors[TEXU_COLOR_BUTTON_CANCEL]              = TEXU_CIO_COLOR_YELLOW_BLACK;
    env->sysbgcolors[TEXU_COLOR_MENU]                       = TEXU_CIO_COLOR_WHITE_BLUE;
    env->sysbgcolors[TEXU_COLOR_MENU_DISABLED]              = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_MENU_SELECTED]              = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_MENU_FOCUSED]               = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_MENUITEM]                   = TEXU_CIO_COLOR_WHITE_BLUE;
    env->sysbgcolors[TEXU_COLOR_MENUITEM_DISABLED]          = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_MENUITEM_SELECTED]          = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_MENUITEM_FOCUSED]           = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_LABEL]                      = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LABEL_DISABLED]             = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_STATUSBAR]                  = TEXU_CIO_COLOR_BLUE_WHITE;
    env->sysbgcolors[TEXU_COLOR_EDIT]                       = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDIT_DISABLED]              = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_EDIT_SELECTED]              = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_EDIT_FOCUSED]               = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDIT_INVALID]               = TEXU_CIO_COLOR_RED_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTBOX]                    = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_LISTBOX_DISABLED]           = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTBOX_SELECTED]           = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_LISTBOX_FOCUSED]            = TEXU_CIO_COLOR_BLUE_BLACK;
    env->sysbgcolors[TEXU_COLOR_COMBOBOX]                   = TEXU_CIO_COLOR_WHITE_BLUE;
    env->sysbgcolors[TEXU_COLOR_COMBOBOX_DISABLED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_COMBOBOX_SELECTED]          = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_COMBOBOX_FOCUSED]           = TEXU_CIO_COLOR_BLUE_BLACK;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL]                   = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_DISABLED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_SELECTED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_FOCUSED]           = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_ITEM]              = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_ITEM_DISABLED]     = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_ITEM_SELECTED]     = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_LISTCTRL_ITEM_FOCUSED]      = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL]                   = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_DISABLED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_SELECTED]          = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_FOCUSED]           = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_ITEM]              = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_ITEM_DISABLED]     = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_ITEM_SELECTED]     = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_TREECTRL_ITEM_FOCUSED]      = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_UPDOWNCTRL]                 = TEXU_CIO_COLOR_WHITE_BLUE;
    env->sysbgcolors[TEXU_COLOR_UPDOWNCTRL_DISABLED]        = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_UPDOWNCTRL_SELECTED]        = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_UPDOWNCTRL_FOCUSED]         = TEXU_CIO_COLOR_WHITE_BLUE;
    env->sysbgcolors[TEXU_COLOR_PROGRESSBAR]                = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_PROGRESSBAR_DISABLED]       = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_PROGRESSBAR_SELECTED]       = TEXU_CIO_COLOR_CYAN_BLUE;
    env->sysbgcolors[TEXU_COLOR_PROGRESSBAR_FOCUSED]        = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_REBAR]                      = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_REBAR_DISABLED]             = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_REBAR_SELECTED]             = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_REBAR_FOCUSED]              = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_BUTTON]                     = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_BUTTON_DISABLED]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_BUTTON_SELECTED]            = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_BUTTON_FOCUSED]             = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_IPADDRESSCTRL]              = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_IPADDRESSCTRL_DISABLED]     = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_IPADDRESSCTRL_SELECTED]     = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_IPADDRESSCTRL_FOCUSED]      = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDITMASKCTRL]               = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDITMASKCTRL_DISABLED]      = TEXU_CIO_COLOR_BLACK_WHITE;
    env->sysbgcolors[TEXU_COLOR_EDITMASKCTRL_SELECTED]      = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_EDITMASKCTRL_FOCUSED]       = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL]            = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_DISABLED]   = TEXU_CIO_COLOR_WHITE_BLACK;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_SELECTED]   = TEXU_CIO_COLOR_BLACK_CYAN;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_FOCUSED]    = TEXU_CIO_COLOR_CYAN_BLACK;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_LOWER]      = TEXU_CIO_COLOR_BLACK_RED;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_EQUAL]      = TEXU_CIO_COLOR_BLACK_YELLOW;
    env->sysbgcolors[TEXU_COLOR_EDITPRICESPREADCTRL_GREATER]    = TEXU_CIO_COLOR_BLACK_GREEN;
    /*default*/
    env->sysbgcolors[TEXU_COLOR_DEFAULT] = TEXU_CIO_COLOR_BLACK_WHITE;
}


void
_texu_env_init_cls(texu_env *env)
{
    texu_env_register_cls(env, TEXU_DESKTOP_CLASS,  _TexuDesktopProc);
    texu_env_register_cls(env, TEXU_MSGBOX_CLASS,   _TexuMsgBoxProc);
    /*controls*/
    texu_env_register_cls(env, TEXU_LABEL_CLASS,        _TexuLabelProc);        /*texuproc.c*/
    texu_env_register_cls(env, TEXU_BUTTON_CLASS,       _TexuButtonProc);       /*texuproc.c*/
    texu_env_register_cls(env, TEXU_EDIT_CLASS,         _TexuEditProc);         /*texuproc.c*/
    texu_env_register_cls(env, TEXU_LISTBOX_CLASS,      _TexuListBoxProc);      /*texuproc.c*/
    texu_env_register_cls(env, TEXU_COMBOBOX_CLASS,     _TexuComboBoxProc);     /*texuproc.c*/
    texu_env_register_cls(env, TEXU_LISTCTRL_CLASS,     _TexuListCtrlProc);     /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_TREECTRL_CLASS,     _TexuTreeCtrlProc);     /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_UPDOWNCTRL_CLASS,   _TexuUpDownCtrlProc);   /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_PROGRESSBAR_CLASS,  _TexuProgressBarProc);  /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_STATUSBAR_CLASS,    _TexuStatusBarProc);    /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_PAGECTRL_CLASS,     _TexuPageCtrlProc);     /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_REBAR_CLASS,        _TexuReBarProc);        /*texuctrl.c*/
    texu_env_register_cls(env, TEXU_IPADDRESSCTRL_CLASS,        _TexuIPAddressProc);         /*texuctrlx.c*/
    texu_env_register_cls(env, TEXU_EDITMASKCTRL_CLASS,         _TexuEditMaskProc);         /*texuctrlx.c*/
    texu_env_register_cls(env, TEXU_EDITPRICESPREADCTRL_CLASS,  _TexuEditPriceSpreadProc);  /*texuctrlx.c*/
    /*menu*/
    texu_env_register_cls(env, TEXU_MENU_CLASS,     _TexuMenuProc);
    texu_env_register_cls(env, TEXU_MENUWND_CLASS,  _TexuMenuWndProc);
}

FILE *
_texu_env_init_screen(texu_env *env)
{
#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
    FILE *fp;
    _wfopen_s(&fp, TEXU_ENV_SCRFILE, L"w");
#else
    FILE *fp = fopen(TEXU_ENV_SCRFILE, "w");
#endif
    return fp;
}

texu_wndproc
_texu_env_find_wndproc(texu_env *env, const texu_char *clsname)
{
    texu_wndproc wndproc = 0;
    texu_env_wndcls *wndcls = 0;
    texu_list_item *item = texu_list_first(env->wndcls);

    while (item != 0)
    {
        wndcls = (texu_env_wndcls *)item->data;
        if (0 == texu_strcmp(wndcls->clsname, clsname))
        {
            /* registered already */
            wndproc = wndcls->proc;
            break;
        }
        item = item->next;
    }
    return wndproc;
}

texu_status
_texu_env_create_desktop(texu_env *env)
{
    texu_status rc = TEXU_OK;
    texu_wnd_attrs attrs;
    texu_wnd *desktop = 0;

    desktop = texu_wnd_new(env); /* no parent */
    if (!desktop)
    {
        return TEXU_NOMEM;
    }

    memset(&attrs, 0, sizeof(attrs));
    attrs.y = 0;
    attrs.x = 0;
    attrs.height    = env->lines;
    attrs.width     = env->cols;
    attrs.enable    = TEXU_TRUE;
    attrs.visible   = TEXU_TRUE;
    attrs.text      = TEXUTEXT("");
    attrs.normalcolor   = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
    attrs.focusedcolor  = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);

    attrs.normalbg      = texu_env_get_sysbgcolor(env, TEXU_COLOR_DESKTOP);
    attrs.disabledbg    = texu_env_get_sysbgcolor(env, TEXU_COLOR_DESKTOP);
    attrs.focusedbg     = texu_env_get_sysbgcolor(env, TEXU_COLOR_DESKTOP);

    attrs.id = 1;
    attrs.clsname = TEXU_DESKTOP_CLASS;
    attrs.userdata = 0;
    attrs.style = 0;
    attrs.exstyle = 0;

    rc = texu_wnd_create(desktop, 0, &attrs);
    if (TEXU_OK == rc)
    {
        env->desktop = desktop;
    }
    texu_wnd_invalidate(desktop);
    return rc;
}

/*
 * which = ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF
 */
#if 0
texu_i32
texu_env_set_timer(texu_env *env, texu_i32 which, texu_i32 initial, texu_i32 repeat)
{
    struct itimerval itimer;
    texu_i32 secs;

    /*initialized initial delay */
    secs = initial / 1000;
    itimer.it_value.tv_sec = secs;
    itimer.it_value.tv_usec = (initial - secs * 1000) * 1000;
    /*initialized repeat interval */
    secs = repeat / 1000;
    itimer.it_interval.tv_sec = secs;
    itimer.it_interval.tv_usec = (repeat - secs * 1000) * 1000;
    /*
      env->initial = initial;
      env->repeat  = repeat;
    */
    return setitimer(which, &itimer, 0);
}
#endif
texu_ui32
texu_env_get_syscolor(texu_env *env, texu_i32 syscolor)
{
    if (syscolor < 0 || syscolor >= TEXU_MAX_COLOR)
    {
        syscolor = TEXU_COLOR_DEFAULT;
    }
    return env->syscolors[syscolor];
}


texu_ui32
texu_env_get_sysbgcolor(texu_env *env, texu_i32 syscolor)
{
    if (syscolor < 0 || syscolor >= TEXU_MAX_COLOR)
    {
        syscolor = TEXU_COLOR_DEFAULT;
    }
    return env->sysbgcolors[syscolor];
}


#if (defined WIN32 && defined _WINDOWS)
#define TEXU_MAINENVWNDCLS          TEXUTEXT("TEXU_MAINENVWNDCLS")
#define TEXU_CHILDENVWNDCLS         TEXUTEXT("TEXU_CHILDENVWNDCLS")

LRESULT CALLBACK _texu_MainEnvWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _texu_ChildEnvWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _texu_ChildEnvWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static texu_env *env = 0;
    static texu_char punctuations[] = TEXUTEXT("~!@#$%^&*_-+=,?/|.\\\'\"()[]{}<>");
    static texu_char digits[] = TEXUTEXT("0123456789");

    switch (message)
    {
        case WM_CREATE:
        {
            LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
            env = (texu_env*)lpcs->lpCreateParams;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG)env);
            break;
        }
        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpmmi = (LPMINMAXINFO)lParam;
            texu_i32 cx = 0;
            texu_i32 cy = 0;
            texu_pos pos = { cy, cx };
            texu_pos spos = { 0, 0 };

            if (!env)
            {
                break;
            }
            cx = texu_env_screen_width(env);
            cy = texu_env_screen_height(env);
            pos.y = cy + 2; /*extra 2 chars*/
            pos.x = cx + 2; /*extra 1 char*/

            texu_env_text_to_screen(env, &spos, &pos);
            lpmmi->ptMinTrackSize.x = spos.x;
            lpmmi->ptMinTrackSize.y = spos.y;
            lpmmi->ptMaxTrackSize.x = spos.x;
            lpmmi->ptMaxTrackSize.y = spos.y;
            break;
        }
        case WM_PAINT:
        {
            HDC hdc;
            PAINTSTRUCT ps;
            texu_wnd *wnd = texu_env_top_wnd(env);
            hdc = BeginPaint(hWnd, &ps);
            LONG cx = ps.rcPaint.right - ps.rcPaint.left;
            LONG cy = ps.rcPaint.bottom - ps.rcPaint.top;

            texu_wnd_invalidate(wnd);
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_CHAR:/*printable characters*/
        case WM_KEYDOWN:/*alphablet and special keys e.g. VK_F1, VK_RETURN but exclude numpad keys*/
        {
            texu_env_run(env, message, wParam, lParam);
            break;
        }
#if 0 /*it is not working properly*/
        case WM_LBUTTONDOWN:
        {
            /*find windows at mouse down*/
            texu_pos spos = { HIWORD(lParam), LOWORD(lParam) };
            texu_pos tpos = spos;
            texu_env_screen_to_text(env, &tpos, &spos);
            texu_wnd *wnd = texu_env_set_focus_from_pos(env, &tpos);
            break;
        }
#endif
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK _texu_MainEnvWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
    }
    return _texu_ChildEnvWndProc(hWnd, message, wParam, lParam);
}


texu_status
texu_env_register_envcls(texu_env *env, HINSTANCE hinst, texu_i32 lines, texu_i32 cols)
{
    WNDCLASS wndClass;

    env->hinst = hinst;
    env->lines = lines;
    env->cols = cols;
    /*basic windows environment structure*/
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hinst;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    /*create the texu_env object as the main window*/
    wndClass.lpfnWndProc = _texu_MainEnvWndProc;
    wndClass.lpszClassName = TEXU_MAINENVWNDCLS;
    RegisterClass(&wndClass);
    /*create the texu_env object as the other window*/
    wndClass.lpfnWndProc = _texu_ChildEnvWndProc;
    wndClass.lpszClassName = TEXU_CHILDENVWNDCLS;
    RegisterClass(&wndClass);

    return TEXU_OK;
}

texu_status
texu_env_easy_createwnd(texu_env *env,
                        HWND hWndParent,
                        UINT nID,
                        LPVOID lpData)
{
    texu_i32 cx = GetSystemMetrics(SM_CXSCREEN);
    texu_i32 cy = GetSystemMetrics(SM_CYSCREEN);
    return texu_env_create_childwnd(env,
                              0,
                              WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                              0, 0, cx, cy,
                              hWndParent,
                              nID,
                              lpData);
}

HWND
texu_env_get_hwnd(texu_env *env)
{
    return env->hwnd;
}


texu_i32    _texu_env_fontcmp(const void *v1, const void *v2);


void
_texu_env_insert_fontmapping(texu_env *env, 
    const texu_char *clsname,
    const texu_char *fontname,
    texu_i32    fontsize,
    texu_i32    fontdpi,
    texu_ui32   id)
{
    struct texu_fontmapping *mapping = 0;
    texu_i32 i = 0;
    texu_i32 arrsize = 0;
    cJSON *arr = 0;
    cJSON *idobj = 0;
    cJSON *fontobj = 0;
    char ascii[TEXU_MAX_WNDTEXT + 1] = "";
    wchar_t wide[TEXU_MAX_WNDTEXT + 1] = L"";

    texu_char str_clsname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");
    texu_char str_fontname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("Consolas");
    texu_char key[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");
    texu_i32  cHeight = 0;/* -MulDiv(18, GetDeviceCaps(env->hdc, LOGPIXELSY), 72);*/
    texu_i32  cWidth = 0;
    texu_i32  cEscapement = 0;
    texu_i32  cOrientation = 0;
    texu_i32  cWeight = 0;
    DWORD     bItalic = 0;
    DWORD     bUnderline = 0;
    DWORD     bStrikeOut = 0;
    DWORD     iCharSet = 0;
    DWORD     iOutPrecision = 0;
    DWORD     iClipPrecision = 0;
    DWORD     iQuality = 0;
    texu_char *newkey = 0;
    texu_i32  idsize = 0;
    texu_i64  value = 0;
    texu_status rc = TEXU_OK;

    texu_sprintf(key, TEXU_MAX_WNDTEXT, TEXUTEXT("%s_%d"), clsname, id);
    newkey = (texu_char*)malloc(sizeof(texu_char)* (TEXU_MAX_WNDTEXT + 1));
    texu_strcpy(newkey, key);
    rc = texu_map_cmp_find(env->fonts, (texu_i64)newkey, (texu_i64*)&value, _texu_env_fontcmp);
    if (rc == TEXU_OK)
    {
        /*already register*/
        free(newkey);
        return;
    }

    mapping = (struct texu_fontmapping*)malloc(sizeof(struct texu_fontmapping));
    memset(mapping, 0, sizeof(struct texu_fontmapping));
    mapping->id = id;
    mapping->dpi = fontdpi;
    mapping->size = fontsize;
    texu_strcpy(mapping->fontname, fontname);
    texu_strcpy(mapping->clsname, clsname);

    /*create font now*/
    cHeight = -MulDiv(mapping->size, GetDeviceCaps(env->hdc, LOGPIXELSY), mapping->dpi);
    mapping->hfont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                                bItalic, bUnderline, bStrikeOut,
                                iCharSet, iOutPrecision, iClipPrecision, iQuality,
                                FF_DONTCARE | FIXED_PITCH,
                                mapping->fontname);
    /*ready to insert into the map*/
    texu_map_cmp_insert(env->fonts, (texu_i64)newkey, (texu_i64)mapping, _texu_env_fontcmp);
}

void
_texu_env_load_fonts_from(texu_env *env, cJSON *obj)
{
    struct texu_fontmapping *mapping = 0;
    texu_i32 i = 0;
    texu_i32 arrsize = 0;
    cJSON *arr = 0;
    cJSON *clsname = 0;
    cJSON *idsobj = 0;
    cJSON *idobj = 0;
    cJSON *fontobj = 0;
    cJSON *fontsize = 0;
    cJSON *fontname = 0;
    cJSON *fontdpi = 0;
    char ascii[TEXU_MAX_WNDTEXT + 1] = "";
    wchar_t wide[TEXU_MAX_WNDTEXT + 1] = L"";

    texu_char str_clsname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("");
    texu_char str_fontname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("Consolas");

    texu_i32  idsize = 0;
    texu_i32  j = 0;
    texu_i32  size = 0;
    texu_i32  dpi = 0;
    texu_ui32   id = 0;

    arrsize = cJSON_GetArraySize(obj);

    for (i = 0; i < arrsize; ++i)
    {
        arr = cJSON_GetArrayItem(obj, i);
        idsize = 0;

        idsobj = cJSON_GetObjectItem(arr, "ids");
        if (idsobj)
        {
            idsize = (texu_ui32)cJSON_GetArraySize(idsobj);
        }


        clsname = cJSON_GetObjectItem(arr, "name");

        if (clsname)/*classes*/
        {
#if (defined WIN32)
            strcpy_s(ascii, TEXU_MAX_WNDTEXT, cJSON_GetStringValue(clsname));
#else
            strcpy(ascii, cJSON_GetStringValue(clsname));
#endif
            texu_a2w(wide, TEXU_MAX_WNDTEXT, ascii, strlen(ascii));
#if (defined WIN32 && defined UNICODE)
            texu_strcpy(str_clsname, wide);
#else
            texu_strcpy(str_clsname, ascii);
#endif
        }
        else
        {
            clsname = cJSON_GetObjectItem(arr, "class");
            if (clsname)/*instances*/
            {
#if (defined WIN32)
                strcpy_s(ascii, TEXU_MAX_WNDTEXT, cJSON_GetStringValue(clsname));
#else
                strcpy(ascii, cJSON_GetStringValue(clsname));
#endif
                texu_a2w(wide, TEXU_MAX_WNDTEXT, ascii, strlen(ascii));
#if (defined WIN32 && defined UNICODE)
                texu_strcpy(str_clsname, wide);
#else
                texu_strcpy(str_clsname, ascii);
#endif
            }
        }
        fontobj = cJSON_GetObjectItem(arr, "font");
        if (fontobj)
        {
            fontname = cJSON_GetObjectItem(fontobj, "name");
            if (fontname)
            {
#if (defined WIN32)
                strcpy_s(ascii, TEXU_MAX_WNDTEXT, cJSON_GetStringValue(fontname));
#else
                strcpy(ascii, cJSON_GetStringValue(fontname));
#endif
                texu_a2w(wide, TEXU_MAX_WNDTEXT, ascii, strlen(ascii));
#if (defined WIN32 && defined UNICODE)
                texu_strcpy(str_fontname, wide);
#else
                texu_strcpy(str_fontname, ascii);
#endif
            }
            fontsize = cJSON_GetObjectItem(fontobj, "size");
            if (fontsize)
            {
                size = (texu_i32)cJSON_GetNumberValue(fontsize);
            }
            fontdpi = cJSON_GetObjectItem(fontobj, "dpi");
            if (fontdpi)
            {
                dpi = (texu_i32)cJSON_GetNumberValue(fontdpi);
            }
        }

        
        
        if (idsize > 0)
        {
            /*register font instance*/
            for (j = 0; j < idsize; ++j)
            {
                /*ready to insert into the map*/
                idobj = cJSON_GetArrayItem(idsobj, j);
                id = (texu_ui32)cJSON_GetNumberValue(idobj);
                _texu_env_insert_fontmapping(env, str_clsname, str_fontname, size, dpi, id);
            }
        }
        else
        {
            /*id=0 means all instances are applied*/
            _texu_env_insert_fontmapping(env, str_clsname, str_fontname, size, dpi, 0);
        }


    }/*for-loop*/
}

texu_i32
_texu_env_fontcmp(const void *v1, const void *v2)
{
    texu_map_keyval *kv1 = (texu_map_keyval *)v1;
    texu_map_keyval *kv2 = (texu_map_keyval *)v2;
    if (kv1 && kv2 && kv1->used && kv2->used)
    {
        /*struct texu_fontmapping* k1 = (struct texu_fontmapping*)kv1->value;
        struct texu_fontmapping* k2 = (struct texu_fontmapping*)kv2->value;*/
        /*if (kv1 && kv1)*/
        {
            return texu_strcmp((const texu_char*)kv1->key, (const texu_char*)kv2->key);/* +(k1->id - k2->id);*/
        }
    }

    return TEXU_CMP_GT;
}

void
_texu_env_load_fonts(texu_env *env)
{
    if (!env->prop)
    {
        return;
    }
    /*classes*/
    {
        cJSON *classes = cJSON_GetObjectItem(env->prop, "classes");
        if (classes)
        {
            _texu_env_load_fonts_from(env, classes);
        }
    }
    /*instances*/
#if 0
    {
        cJSON *instances = cJSON_GetObjectItem(env->prop, "instances");
        if (instances)
        {
            _texu_env_load_fonts_from(env, instances);
        }
    }
#endif
}

texu_status
_texu_env_get_font(texu_env *env, const texu_char *clsname, texu_ui32 id, struct texu_fontmapping *mapping)
{
    texu_i64 value;
    texu_char key[TEXU_MAX_WNDTEXT + 1];
    texu_status rc = TEXU_OK;
    
    texu_sprintf(key, TEXU_MAX_WNDTEXT, TEXUTEXT("%s_%d"), clsname, id);
    rc = texu_map_cmp_find(env->fonts, (texu_i64)key, (texu_i64*)&value, _texu_env_fontcmp);
    if (TEXU_OK == rc && mapping)
    {
        memcpy(mapping, (struct texu_fontmapping *)value, sizeof(struct texu_fontmapping));
    }
    return rc;
}

struct _texu_clsname_mapping
{
    texu_char   *smallcls;
    texu_char   *capcls;
    texu_char   *clsname;
};
typedef struct _texu_clsname_mapping _texu_clsname_mapping;
_texu_clsname_mapping a_texu_clsname_mapping[] =
{
    { TEXUTEXT("texu_msgbox_class"),        TEXUTEXT("TEXU_MSGBOX_CLASS"),          TEXU_MSGBOX_CLASS },
    { TEXUTEXT("texu_desktop_class"),       TEXUTEXT("TEXU_DESKTOP_CLASS"),         TEXU_DESKTOP_CLASS },
    { TEXUTEXT("texu_label_class"),         TEXUTEXT("TEXU_LABEL_CLASS"),           TEXU_LABEL_CLASS },
    { TEXUTEXT("texu_button_class"),        TEXUTEXT("TEXU_BUTTON_CLASS"),          TEXU_BUTTON_CLASS },
    { TEXUTEXT("texu_statusbar_class"),     TEXUTEXT("TEXU_STATUSBAR_CLASS"),       TEXU_STATUSBAR_CLASS },
    { TEXUTEXT("texu_edit_class"),          TEXUTEXT("TEXU_EDIT_CLASS"),            TEXU_EDIT_CLASS },
    { TEXUTEXT("texu_listbox_class"),       TEXUTEXT("TEXU_LISTBOX_CLASS"),         TEXU_LISTBOX_CLASS },
    { TEXUTEXT("texu_combobox_class"),      TEXUTEXT("TEXU_COMBOBOX_CLASS"),        TEXU_COMBOBOX_CLASS },
    { TEXUTEXT("texu_listctrl_class"),      TEXUTEXT("TEXU_LISTCTRL_CLASS"),        TEXU_LISTCTRL_CLASS },
    { TEXUTEXT("texu_treectrl_class"),      TEXUTEXT("TEXU_TREECTRL_CLASS"),        TEXU_TREECTRL_CLASS },
    { TEXUTEXT("texu_updownctrl_class"),    TEXUTEXT("TEXU_UPDOWNCTRL_CLASS"),      TEXU_UPDOWNCTRL_CLASS },
    { TEXUTEXT("texu_progressbar_class"),   TEXUTEXT("TEXU_PROGRESSBAR_CLASS"),     TEXU_PROGRESSBAR_CLASS },
    { TEXUTEXT("texu_pagectrl_class"),      TEXUTEXT("TEXU_PAGECTRL_CLASS"),        TEXU_PAGECTRL_CLASS },
    { TEXUTEXT("texu_textctrl_class"),      TEXUTEXT("TEXU_TEXTCTRL_CLASS"),        TEXU_TEXTCTRL_CLASS },
    { TEXUTEXT("texu_rebar_class"),         TEXUTEXT("TEXU_REBAR_CLASS"),           TEXU_REBAR_CLASS },
    { TEXUTEXT("texu_ipaddressctrl_class"), TEXUTEXT("TEXU_IPADDRESSCTRL_CLASS"),   TEXU_IPADDRESSCTRL_CLASS },
    { TEXUTEXT("texu_editmaskctrl_class"),  TEXUTEXT("TEXU_EDITMASKCTRL_CLASS"),    TEXU_EDITMASKCTRL_CLASS },
    { TEXUTEXT("texu_editpricespreadctrl_class"), TEXUTEXT("TEXU_EDITPRICESPREADCTRL_CLASS"), TEXU_EDITPRICESPREADCTRL_CLASS },
    { TEXUTEXT("texu_menu_class"),          TEXUTEXT("TEXU_MENU_CLASS"),            TEXU_MENU_CLASS },
    { TEXUTEXT("texu_menuwnd_class"),       TEXUTEXT("TEXU_MENUWND_CLASS"),         TEXU_MENUWND_CLASS }
};

#define TEXU_MAX_CLSNAME_MAPPING    (sizeof(a_texu_clsname_mapping)/sizeof(a_texu_clsname_mapping[0]))

const _texu_clsname_mapping *
_texu_env_get_clsname_mapping(const texu_char *clsname)
{
    texu_i32 i = 0;
    for (i = 0; i < TEXU_MAX_CLSNAME_MAPPING; ++i)
    {
        if (0 == texu_strcmp(clsname, a_texu_clsname_mapping[i].clsname))
        {
            return &a_texu_clsname_mapping[i];
        }
    }
    return 0;
}

HFONT
texu_env_get_clsfont(texu_env *env, const texu_char *clsname)
{
    return texu_env_get_insfont(env, clsname, 0);
}

HFONT
texu_env_get_insfont(texu_env *env, const texu_char *clsname, texu_ui32 id)
{
    struct texu_fontmapping font;
    const _texu_clsname_mapping *mapping = _texu_env_get_clsname_mapping(clsname);
    texu_status rc = TEXU_OK;

    if (!mapping)
    {
        return 0;
    }
    rc = _texu_env_get_font(env, mapping->smallcls, id, &font);
    if (rc != TEXU_OK)
    {
        rc = _texu_env_get_font(env, mapping->capcls, id, &font);
        if (rc != TEXU_OK)
        {
            return 0;
        }
    }
    return font.hfont;
}

texu_status
_texu_env_createwnd(texu_env *env,
    DWORD dwExStyle,
    const texu_char *lpClassName,
    DWORD dwStyle,
    int x, int y, int cx, int cy,
    HWND hWndParent,
    UINT nID,
    LPVOID lpData)
{
    texu_status rc = TEXU_OK;
    texu_char fontname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("Consolas");
    texu_long hspaces = 0;
    texu_i32 cHeight = 0;/* -MulDiv(18, GetDeviceCaps(env->hdc, LOGPIXELSY), 72);*/
    texu_i32 cWidth = 0;
    texu_i32 cEscapement = 0;
    texu_i32 cOrientation = 0;
    texu_i32 cWeight = 0;
    DWORD    bItalic = 0;
    DWORD    bUnderline = 0;
    DWORD    bStrikeOut = 0;
    DWORD    iCharSet = 0;
    DWORD    iOutPrecision = 0;
    DWORD    iClipPrecision = 0;
    DWORD    iQuality = 0;
    texu_i32 fontsize = 18;
    texu_i32 dpi = 72;
    texu_i32 idle = 500;
    texu_i32 cxScr = GetSystemMetrics(SM_CXSCREEN);
    texu_i32 cyScr = GetSystemMetrics(SM_CYSCREEN);

    env->lpData = lpData;
    HWND hwnd = CreateWindowEx(
        dwExStyle,
        lpClassName,
        lpClassName,
        dwStyle,
        x, y, cx, cy,
        hWndParent,
        (HMENU)nID,
        env->hinst,
        env);
    if (!hwnd)
    {
        DWORD dw = GetLastError();
        rc = TEXU_ERROR;
        return rc;
    }
    if (!env->hwnd) /*one time to create windows objects*/
    {
        env->hwnd = hwnd;
        env->hdc = GetDC(hwnd);
        env->hmemdc = CreateCompatibleDC(env->hdc);
        env->hbmp = CreateCompatibleBitmap(env->hdc,
                                           cxScr,
                                           cyScr);
        SelectObject(env->hmemdc, env->hbmp);
        env->cxScreen = cxScr;
        env->cyScreen = cyScr;

        env->fonts = texu_map_new();

        /*env->props->GetString(env->props, ENV_FONT, fontname, GF_CONSOLAS);*/
        {
            FILE *fp = NULL;
            errno_t er = fopen_s(&fp, ".\\texuenv.json", "r");
            long filelen = 0;
            char *data = 0;
            if (fp)
            {
                fseek(fp, 0, SEEK_END);
                filelen = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                if (filelen > 0)
                {
                    data = (char*)malloc(filelen);
                    fread(data, filelen, 1, fp);
                }
                fclose(fp);
            }
            cJSON *json = cJSON_Parse(data);
            if (json)
            {
                cJSON *envjson = cJSON_GetObjectItem(json, "env");
                env->prop = envjson; /*safe property here*/
                _texu_env_load_fonts(env);  /*load all fonts*/

                if (envjson)
                {
                    cJSON *idlejson = cJSON_GetObjectItem(envjson, "idle");
                    cJSON *fontjson = cJSON_GetObjectItem(envjson, "font");
                    if (fontjson)
                    {
                        cJSON *fontprop = cJSON_GetObjectItem(fontjson, "size");
                        if (fontprop)
                        {
                            fontsize = (texu_i32)cJSON_GetNumberValue(fontprop);
                            if (fontsize < 12 || fontsize > 18)
                            {
                                fontsize = 18;
                            }
                        }
                        fontprop = cJSON_GetObjectItem(fontjson, "dpi");
                        if (fontprop)
                        {
                            dpi = (texu_i32)cJSON_GetNumberValue(fontprop);
                            if (dpi < 72 || dpi > 200)
                            {
                                dpi = 72;
                            }
                        }
                        fontprop = cJSON_GetObjectItem(fontjson, "name");
                        if (fontprop)
                        {
                            char *name = cJSON_GetStringValue(fontprop);
                            texu_a2w(fontname, TEXU_MAX_WNDTEXT, name, strlen(name));
                        }
                    }
                    if (idlejson)
                    {
                        idle = (texu_i32)cJSON_GetNumberValue(idlejson);
                        if (idle < 250 || fontsize > 1000)
                        {
                            idle = 500;
                        }
                    }
                }
            }
            if (data)
            {
                free(data);
            }
        }
        cHeight = -MulDiv(fontsize, GetDeviceCaps(env->hdc, LOGPIXELSY), dpi);
        env->hfont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                                bItalic, bUnderline, bStrikeOut,
                                iCharSet, iOutPrecision, iClipPrecision, iQuality,
                                FF_DONTCARE | FIXED_PITCH,
                                fontname);
        SelectObject(env->hdc, env->hfont);
        SelectObject(env->hmemdc, env->hfont);

        GetTextMetrics(env->hdc, &env->tm);
        env->cxChar = env->tm.tmAveCharWidth;
        env->cyChar = env->tm.tmHeight + env->tm.tmExternalLeading;
        env->cxCaps = (env->tm.tmPitchAndFamily & 1 ? 3 : 2) * env->cxChar / 2;

        hspaces = 0;
        /*env->props->GetInt(env->props, ENV_FONT_HSPACES, &hspaces, G2T_HTEXTSPACES);*/
        env->cyLine = env->cyChar + hspaces;

        SetTimer(env->hwnd, 1, 500, NULL);/*idle time*/

        cx = env->cols * env->cxCaps;
        cy = env->lines * env->cyLine;
        SetWindowPos(env->hwnd, HWND_TOP, x, y, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW);
    }
    return rc;
}

texu_status
texu_env_create_mainwnd(texu_env *env,
DWORD dwExStyle,
DWORD dwStyle,
int x, int y, int cx, int cy,
LPVOID lpData)
{
    return _texu_env_createwnd(env,
                              dwExStyle,
                              TEXU_MAINENVWNDCLS,
                              dwStyle,
                              x, y, cx, cy,
                              NULL,
                              0,
                              lpData);
}

texu_status
texu_env_create_childwnd(texu_env *env,
DWORD dwExStyle,
DWORD dwStyle,
int x, int y, int cx, int cy,
HWND hWndParent,
UINT nID,
LPVOID lpData)
{
    return _texu_env_createwnd(env,
                               dwExStyle,
                               TEXU_CHILDENVWNDCLS,
                               dwStyle,
                               x, y, cx, cy,
                               hWndParent,
                               nID,
                               lpData);
}


texu_i32
texu_env_get_cxcaps(texu_env *env)
{
    return env->cxCaps;
}
texu_i32
texu_env_get_cyline(texu_env *env)
{
    return env->cyLine;
}


texu_status texu_env_gotoyx(texu_env *env, texu_i32 y, texu_i32 x)
{
    texu_status rc = TEXU_OK;
    texu_pos pos;
    pos.x = x;
    pos.y = y;
    texu_env_text_to_screen(env, &pos, &pos);
    return rc;
}

texu_bool
texu_pos_in_rect(const texu_rect* rect, const texu_pos *pos)
{
    if (pos->x >= rect->x && pos->x <= (rect->x + rect->cols) &&
        pos->y >= rect->y && pos->y <= (rect->y + rect->lines))
    {
        return TRUE;
    }
    return TEXU_FALSE;
}

texu_wnd*
_texu_find_wnd_from_pos(texu_wnd *wnd, const texu_pos *pos)
{
    texu_rect rect;
    texu_wnd *childwnd = 0;
    texu_wnd *nextwnd = 0;
    texu_wnd *child = 0;

    if (!wnd)
    {
        return 0;
    }

    child = texu_wnd_firstchild(wnd);
    if (child)
    {
        childwnd = _texu_find_wnd_from_pos(child, pos);
        if (childwnd)
        {
            return childwnd;
        }
    }
    
    nextwnd = texu_wnd_nextwnd(wnd);
    if (nextwnd)
    {
        childwnd = _texu_find_wnd_from_pos(nextwnd, pos);
        if (childwnd)
        {
            return childwnd;
        }
    }

    texu_wnd_get_rect(wnd, &rect);
    if (texu_pos_in_rect(&rect, pos))
    {
        return wnd;
    }

    return 0;
}

texu_wnd*
texu_get_wnd_from_pos(texu_env *env, const texu_pos *pos)
{
    texu_wnd *activewnd = (texu_wnd *)texu_stack_top(env->frames);
    texu_wnd *wnd = texu_wnd_get_first_activechild(activewnd);
    texu_wnd *activechild = texu_wnd_get_activechild(wnd);

    return _texu_find_wnd_from_pos(activechild, pos);
}
#if 0
texu_wnd*
texu_env_set_focus_from_pos(texu_env *env, const texu_pos *pos)
{
    texu_wnd *activewnd = (texu_wnd *)texu_stack_top(env->frames);
    texu_wnd *wnd = texu_wnd_get_first_activechild(activewnd);
    texu_i64 rc = 0;
    texu_wnd *activechild = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = _texu_find_wnd_from_pos(activewnd, pos);

    if (!nextwnd || !texu_wnd_is_enable(nextwnd) || !texu_wnd_is_visible(nextwnd))
    {
        return 0;
    }
    /* kill and set the new active window */
    rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
    if (rc != TEXU_OK)
    {
        return 0;
    }
    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activechild, 0);

    /* the new active window */
    texu_wnd_set_activechild(wnd, nextwnd);
    return nextwnd;
}
#endif
texu_status texu_env_text_to_screen(texu_env *env, texu_pos* spos, const texu_pos* tpos)
{
    texu_status rc = TEXU_OK;
    spos->x = (tpos->x + 1) * texu_env_get_cxcaps(env);
    spos->y = (tpos->y + 1) * texu_env_get_cyline(env);
    return rc;
}

texu_status texu_env_screen_to_text(texu_env *env, texu_pos* tpos, const texu_pos* spos)
{
    texu_status rc = TEXU_OK;
    texu_i32 cxcaps = texu_env_get_cxcaps(env);
    texu_i32 cyline = texu_env_get_cyline(env);
    tpos->x = (cxcaps ? spos->x / cxcaps : 0);
    tpos->y = (cyline ? spos->y / cyline : 0);
    return rc;
}

texu_status
texu_env_draw_char(texu_env *env,
                    texu_i32 y,
                    texu_i32 x,
                    texu_char ch,
                    texu_ui32 textcolor,
                    texu_ui32 bgcolor)
{
    texu_char text[2] = { ch, 0 };
    return texu_env_draw_text(env, y, x, text, textcolor, bgcolor);
}

texu_status
texu_env_draw_char_ex(texu_env *env,
    texu_i32 y,
    texu_i32 x,
    texu_char ch,
    texu_ui32 textcolor,
    texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32   id)
{
    texu_char text[2] = { ch, 0 };
    return texu_env_draw_text_ex(env, y, x, text, textcolor, bgcolor, clsname, id);
}

texu_status
texu_env_draw_text_ex(texu_env *env,
    texu_i32 y,
    texu_i32 x,
    const texu_char * text,
    texu_ui32 textcolor,
    texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32       id)
{
    HFONT hfont = texu_env_get_insfont(env, clsname, id);
    HFONT holdfont = 0;
    if (!hfont)
    {
        hfont = texu_env_get_clsfont(env, clsname);
    }
    if (hfont)
    {
        holdfont = (HFONT)SelectObject(env->hmemdc, hfont);
        texu_env_draw_text(env, y, x, text, textcolor, bgcolor);
        SelectObject(env->hmemdc, holdfont);
    }
    else
    {
        texu_env_draw_text(env, y, x, text, textcolor, bgcolor);
    }
    return TEXU_OK;
}

texu_status 
texu_env_draw_text(texu_env *env,
                    texu_i32 y, 
                    texu_i32 x, 
                    const texu_char * text,
                    texu_ui32 textcolor, 
                    texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_pos tpos = { y, x };
    texu_pos spos;
    COLORREF oldtext = 0;
    COLORREF oldbg = 0;
    HDC hdc = env->hmemdc;

    texu_env_text_to_screen(env, &spos, &tpos);

    oldtext = SetTextColor(hdc, textcolor);
    oldbg = SetBkColor(hdc, bgcolor);
    SetTextAlign(hdc, TA_LEFT | VTA_TOP);

    TextOut(hdc, spos.x, spos.y, text, texu_strlen(text));

    SetTextColor(hdc, oldtext);
    SetBkColor(hdc, oldbg);
    return rc;
}

texu_status
texu_env_update_screen(texu_env *env)
{
    /*BitBlt(env->hdc, 0, 0, env->cxScreen, env->cyScreen, env->hmemdc, 0, 0, SRCCOPY);*/
    return TEXU_OK;
}

/*
texu_env_draw_frame_char()
env
x  - x-pos
y  - y-pos
ch is one of (TEXU_UPPER_LEFT, TEXU_UPPER_RIGHT, TEXU_LOWER_LEFT, TEXU_LOWER_RIGHT, TEXU_VERTICAL, TEXU_HORIZONTAL)
#define TEXU_UPPER_LEFT      (1)
#define TEXU_UPPER_RIGHT     (2)
#define TEXU_LOWER_LEFT      (3)
#define TEXU_LOWER_RIGHT     (4)
#define TEXU_VERTICAL        (5)
#define TEXU_HORIZONTAL      (6)
#define TEXU_VERT_LEFT       (7)
#define TEXU_VERT_RIGHT      (8)
#define TEXU_HORZ_DOWN       (9)
#define TEXU_HORZ_UP         (10)
#define TEXU_VERT_HORZ       (11)
*/
texu_status texu_env_draw_rect(texu_env *env, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_pos tpos;
    texu_pos spos;
    texu_pos spos2;
    texu_i32 x = rect->x;
    texu_i32 y = rect->y;
    texu_i32 cx = texu_env_screen_width(env);
    texu_i32 cy = texu_env_screen_height(env);
    /*upper left*/
    tpos.x = TEXU_MIN(rect->x, cx);
    tpos.y = TEXU_MIN(rect->y, cy);
    texu_env_text_to_screen(env, &spos, &tpos);
    /*lower right*/
    tpos.x = TEXU_MIN(rect->x + rect->cols, cx);
    tpos.y = TEXU_MIN(rect->y + rect->lines, cy);
    texu_env_text_to_screen(env, &spos2, &tpos);

    x = env->cxCaps / 2;
    y = env->cyLine / 2;
    HPEN hpen = CreatePen(PS_SOLID, 2, textcolor);
    HBRUSH hbrush = CreateSolidBrush(bgcolor);

    HDC hdc = env->hmemdc;
    HPEN hOldPen = (HPEN)SelectObject(hdc, hpen);
    /* upper line */
    MoveToEx(hdc, spos.x + x, spos.y + y, NULL);
    LineTo(hdc, spos2.x + x, spos.y + y);
    /* right line */
    LineTo(hdc, spos2.x + x, spos2.y + y);
    /* lower line */
    LineTo(hdc, spos.x + x, spos2.y + y);
    /* left line */
    LineTo(hdc, spos.x + x, spos.y + y);

    SelectObject(hdc, hOldPen);

    DeleteObject(hpen);
    DeleteObject(hbrush);
    return rc;
}

texu_status texu_env_draw_frame_char(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 ch, texu_ui32 textcolor,
                                     texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_pos tpos;
    texu_pos spos;
    tpos.x = x;
    tpos.y = y;
    texu_env_text_to_screen(env, &spos, &tpos);

    x = env->cxCaps / 2;
    y = env->cyLine / 2;
   
    switch (ch)
    {
        case TEXU_UPPER_LEFT:
            /* +- */
            /* |  */
            MoveToEx(env->hmemdc, spos.x + x, spos.y + env->cyChar, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + env->cxCaps, spos.y + y);
            break;
        case TEXU_UPPER_RIGHT:
            /* -+  */
            /*  |  */
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + x, spos.y + env->cyChar);
            break;
        case TEXU_LOWER_LEFT:
            /*  |  */
            /*  +- */
            MoveToEx(env->hmemdc, spos.x + x, spos.y, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + env->cxCaps, spos.y + y);
            break;
        case TEXU_LOWER_RIGHT:
            /*  |  */
            /* -+  */
            MoveToEx(env->hmemdc, spos.x /*+ x*/, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + x, spos.y /*+ env->cyChar*/);
            break;
        case TEXU_VERTICAL:
            /*  |  */
            MoveToEx(env->hmemdc, spos.x + x, spos.y /*+ y*/, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + env->cyChar);
            break;
        case TEXU_HORIZONTAL:
            /*  -  */
            MoveToEx(env->hmemdc, spos.x /*+ x*/, spos.y + y , NULL);
            LineTo(env->hmemdc, spos.x + env->cxCaps, spos.y + y);
            break;
        case TEXU_VERT_LEFT:
            /*  |  */
            /*  +- */
            /*  |  */
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + x, spos.y + env->cyChar);
            LineTo(env->hmemdc, spos.x + x, spos.y);
            break;
        case TEXU_VERT_RIGHT:
            /*  |  */
            /* -+  */
            /*  |  */
            MoveToEx(env->hmemdc, spos.x + x, spos.y + env->cyChar, NULL);
            LineTo(env->hmemdc, spos.x + x, spos.y + y);
            LineTo(env->hmemdc, spos.x + env->cxCaps, spos.y + y);
            LineTo(env->hmemdc, spos.x, spos.y + y);
            break;
        case TEXU_HORZ_DOWN:
            /* -+- */
            /*  |  */
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x, spos.y + env->cyChar);
            MoveToEx(env->hmemdc, spos.x + x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x, spos.y + env->cyChar);
            break;
        case TEXU_HORZ_UP:
            /*  |  */
            /* -+- */
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x, spos.y + env->cyChar);
            MoveToEx(env->hmemdc, spos.x + x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x + env->cxCaps, spos.y);
            break;
        case TEXU_VERT_HORZ:
            /*  |  */
            /* -+- */
            /*  |  */
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x, spos.y + env->cyChar);
            MoveToEx(env->hmemdc, spos.x, spos.y + y, NULL);
            LineTo(env->hmemdc, spos.x, spos.y + env->cyChar);
            break;
        default:
            rc = TEXU_ERROR;
            break;
    }
    return rc;
}

texu_status texu_env_draw_vline(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 blocks, texu_ui32 textcolor,
                           texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_i32 i = 0;
    for (i = 0; i < blocks; ++i)
    {
        texu_env_draw_frame_char(env, y + i, x, TEXU_VERTICAL, textcolor, bgcolor);
    }
    return rc;
}
texu_status texu_env_draw_hline(texu_env *env, texu_i32 y, texu_i32 x, texu_i32 blocks, texu_ui32 textcolor,
                           texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_i32 i = 0;
    for (i = 0; i < blocks; ++i)
    {
        texu_env_draw_frame_char(env, y, x + i, TEXU_HORIZONTAL, textcolor, bgcolor);
    }
    return rc;
}
#if 0
texu_status texu_env_draw_frame(texu_env *env, texu_rect* rect, texu_ui32 textcolor, texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    HPEN hpen = CreatePen(PS_SOLID, 2, textcolor);
    HBRUSH hbrush = CreateSolidBrush(bgcolor);

    HPEN hOldPen = (HPEN)SelectObject(env->hmemdc, hpen);
    /* upper line */
    texu_env_draw_hline(env, rect->y, rect->x, rect->cols, textcolor, bgcolor);
    /* lower line */
    texu_env_draw_hline(env, rect->y + rect->lines, rect->x, rect->cols, textcolor, bgcolor);
    /* left line */
    texu_env_draw_vline(env, rect->y, rect->x, rect->lines, textcolor, bgcolor);
    /* right line */
    texu_env_draw_vline(env, rect->y, rect->x + rect->cols, rect->lines, textcolor, bgcolor);
    /* upper left*/
    texu_env_draw_frame_char(env, rect->y, rect->x, TEXU_UPPER_LEFT, textcolor, bgcolor);
    /* lower left*/
    texu_env_draw_frame_char(env, rect->y + rect->lines, rect->x, TEXU_LOWER_LEFT, textcolor, bgcolor);
    /* upper right*/
    texu_env_draw_frame_char(env, rect->y, rect->x + rect->cols, TEXU_UPPER_RIGHT, textcolor, bgcolor);
    /* lower right*/
    texu_env_draw_frame_char(env, rect->y + rect->lines, rect->x + rect->cols, TEXU_LOWER_RIGHT, textcolor, bgcolor);

    /*SelectObject(env->hmemdc, hOldBrush);*/
    SelectObject(env->hmemdc, hOldPen);

    DeleteObject(hpen);
    DeleteObject(hbrush);
    return rc;
}
#endif
texu_status texu_env_draw_hframes(texu_env *env, texu_rect* rect, texu_i32* heights, texu_i32 nframes, texu_ui32 textcolor, texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_i32 i = 0;
    texu_rect rcframe = *rect;
    for (i = 0; heights && i < nframes; ++i)
    {
        rcframe.lines = /*rect->y + */heights[i];
        texu_env_draw_rect(env, &rcframe, textcolor, bgcolor);
        rcframe.y += heights[i];
    }
    return rc;
}

texu_status texu_env_draw_vframes(texu_env *env, texu_rect* rect, texu_i32* widths, texu_i32 nframes, texu_ui32 textcolor, texu_ui32 bgcolor)
{
    texu_status rc = TEXU_OK;
    texu_i32 i = 0;
    texu_rect rcframe = *rect;
    for (i = 0; widths && i < nframes; ++i)
    {
        rcframe.cols = /*rect->x + */widths[i];
        texu_env_draw_rect(env, &rcframe, textcolor, bgcolor);
        rcframe.x += widths[i];
    }
    return rc;
}

#endif /*WIN32*/



texu_bool texu_env_new_cursor(texu_env *env)
{
#if (defined WIN32 && defined _WINDOWS)
    if (TEXU_IS_FALSE(env->has_cursor))
    {
        texu_bool rb = (texu_bool)CreateCaret(env->hwnd, NULL, env->cxChar, env->cyChar);
        if (rb)
        {
            env->has_cursor = TEXU_TRUE;
        }
        else
        {
            env->has_cursor = TEXU_FALSE;
        }
        return rb;
    }
#endif
    return TEXU_FALSE;
}

texu_bool texu_env_set_cursor(texu_env *env, texu_i32 y, texu_i32 x)
{
#if (defined WIN32 && defined _WINDOWS)
    texu_pos tpos = { y, x };
    texu_pos spos;

    texu_env_text_to_screen(env, &spos, &tpos);
    if (env->has_cursor)
    {
        return (texu_bool)SetCaretPos(spos.x, spos.y);
    }
    else
    {
        texu_env_new_cursor(env);
        if (env->has_cursor)
        {
            return (texu_bool)SetCaretPos(spos.x, spos.y);
        }
    }
#endif
    return TEXU_FALSE;
}

texu_bool texu_env_show_cursor(texu_env *env, texu_bool show)
{
    texu_bool rb = TEXU_FALSE;
#if (defined WIN32 && defined _WINDOWS)
    if (env->has_cursor)
    {
        if (show)
        {
            rb = (texu_bool)ShowCaret(env->hwnd);
            while (env->hidden_cursor > 0)
            {
                --env->hidden_cursor;
                rb = (texu_bool)ShowCaret(env->hwnd);
                SetCaretBlinkTime(250);
            }
        }
        else
        {
            rb = (texu_bool)HideCaret(env->hwnd);
        }
    }
    else
    {
        texu_env_new_cursor(env);
        if (env->has_cursor)
        {
            if (show)
            {
                while (env->hidden_cursor > 0)
                {
                    --env->hidden_cursor;
                    rb = (texu_bool)ShowCaret(env->hwnd);
                }
                SetCaretBlinkTime(250);
            }
            else
            {
                rb = (texu_bool)HideCaret(env->hwnd);
            }
        }
    }
    if (!show)
    {
        env->hidden_cursor++;
    }
#endif
    return rb;
}

texu_bool texu_env_del_cursor(texu_env *env)
{
#if (defined WIN32 && defined _WINDOWS)
    if (env->has_cursor)
    {
        return (texu_bool)DestroyCaret();
    }
#endif
    return TEXU_FALSE;
}


texu_status
texu_env_register_cls(
    texu_env *env,
    const texu_char *clsname,
    texu_wndproc proc)
{
    texu_status rc = TEXU_OK;
    texu_env_wndcls *wndcls = 0;
    texu_wndproc wndproc = _texu_env_find_wndproc(env, clsname);

    /* registered already */
    if (wndproc)
    {
        return rc;
    }

    /* allocate the new memory */
    wndcls = (texu_env_wndcls *)malloc(sizeof(texu_env_wndcls));
    if (!wndcls)
    {
        return TEXU_NOMEM;
    }

    /* add the new registered class */
    wndcls->clsname = clsname;
    wndcls->proc = proc;
    texu_list_add(env->wndcls, (texu_i64)wndcls);

    return rc;
}

#ifdef USE_TCL_AUTOMATION
texu_env *
texu_env_new(texu_i32 lines, texu_i32 cols, const char* pathname)
#else
texu_env *
#if (defined WIN32 && defined _WINDOWS)
texu_env_new(HINSTANCE hinst, texu_i32 lines, texu_i32 cols)
#else
texu_env_new(texu_i32 lines, texu_i32 cols)
#endif
#endif /*USE_TCL_AUTOMATION*/
{
    texu_status rc = TEXU_OK;
    texu_env *env = (texu_env *)malloc(sizeof(texu_env));
    if (env)
    {
        memset(env, 0, sizeof(texu_env));
#if USE_TCL_AUTOMATION
        _texu_env_init_tcl_automation(env, pathname);
#endif /*USE_TCL_AUTOMATION*/

        /* console input/output */
        env->cio = texu_cio_new();
#if (defined WIN32 && defined _WINDOWS)
        texu_cio_init(env->cio, env);
        /*texu_env_createwnd(env, hWndParent, nID);*/
        env->has_cursor = TEXU_FALSE;
        texu_env_register_envcls(env, hinst, lines, cols);
#else
        texu_cio_init(env->cio, lines, cols);
        env->cols  = cols;
        env->lines = lines;
#endif
/*
        env->scrfp = _texu_env_init_screen(env);
*/
        _texu_env_init_syscolors(env);
#if (defined WIN32 && defined _WINDOWS)
        _texu_env_init_sysbgcolors(env);
        env->chNextKey = TEXU_KEY_NEXTWND;
        env->chPrevKey = TEXU_KEY_PREVWND;
#endif

        /* window classes */
        env->wndcls = texu_list_new();

        /* register internal classes */
        _texu_env_init_cls(env);

        /*update screen*/
        env->locked_update = TEXU_TRUE;

        /* desktop */
        rc = _texu_env_create_desktop(env);
        if (rc != TEXU_OK)
        {
            texu_list_del(env->wndcls);
            texu_cio_release(env->cio);
            free(env);
            env = 0;
            return env;
        }

        /* window frames */
        env->frames = texu_stack_new(TEXU_ENV_MAX_FRAMES + 1);
        texu_stack_push(env->frames, (texu_i64)env->desktop);
        
        /* msg ques */
        env->msgques = texu_queue_new();
    }
    return env;
}

void
texu_env_del(texu_env *env)
{
    if (env)
    {
#if USE_TCL_AUTOMATION
        _texu_env_release_tcl_automation(env);
        if (env->fstdout)
        {
            fflush(env->fstdout);
            fclose(env->fstdout);
        }
#endif  /*USE_TCL_AUTOMATION*/
/*            fclose(env->scrfp);*/
        texu_list_del(env->wndcls);
        texu_stack_del(env->frames);
        texu_wnd_del(env->desktop);
        texu_cio_release(env->cio);
        
        texu_queue_del(env->msgques);
#if (defined WIN32 && defined _WINDOWS)
        DeleteObject(env->hbmp);
        DeleteObject(env->hfont);
        DeleteDC(env->hmemdc);
        DeleteDC(env->hdc);
#endif
        free(env);
        env = 0;
    }
}

void
_texu_env_exec_postmsg(texu_env *env)
{
    struct texu_msg    *msg;
    /* if there are any posted msg*/
    while (TEXU_FALSE == texu_queue_empty(env->msgques))
    {
        msg = (struct texu_msg*)texu_queue_first(env->msgques);
        if (msg)
        {
            texu_wnd_send_msg(msg->wnd, msg->msg, msg->param1, msg->param2);
            free(msg);
        }
        texu_queue_dequeue(env->msgques);
    }
}
#ifdef USE_TCL_AUTOMATION
FILE*
texu_env_get_stdout(texu_env *env)
{
if (!env->fstdout)
{
    env->fstdout = fopen(".texustdout", "w");
}
return env->fstdout;
}
#endif

#if (defined WIN32 && defined _WINDOWS)
texu_status
texu_env_run(texu_env *env, UINT message, WPARAM wParam, LPARAM lParam)
{
    texu_wnd        *activewnd = 0;
    WPARAM          ch = wParam;
    texu_i32        altpressed = 0;
    texu_i32        ctlpressed = 0;
    texu_i32        shtpressed = 0;
    texu_ui32       msg = TEXU_WM_CHAR;

    _texu_env_exec_postmsg(env);
    switch (message)
    {
        case WM_CHAR:
        case WM_KEYDOWN:
        {
            ch = wParam;
            altpressed = (GetAsyncKeyState(VK_MENU)     ? TEXU_KEYPRESSED_ALT   : 0);
            ctlpressed = (GetAsyncKeyState(VK_CONTROL)  ? TEXU_KEYPRESSED_CTRL  : 0);
            shtpressed = (GetAsyncKeyState(VK_SHIFT)    ? TEXU_KEYPRESSED_SHIFT : 0);
            if (WM_KEYDOWN == message)
            {
                msg = TEXU_WM_KEYDOWN;
            }
            break;
        }
        case WM_TIMER:
        {
            activewnd = (texu_wnd *)texu_stack_top(env->frames); 
            texu_wnd_send_msg(activewnd, TEXU_WM_IDLE, 0, 0);
            return TEXU_OK;
        }
    }
    if (env->frames)
    {
        /*no key pressed*/
        /*handle idle time by calling TEXU_WM_IDLE at the top of window*/
        activewnd = (texu_wnd *)texu_stack_top(env->frames);
        if (!activewnd)
        {
            /*no more windows active*/
            return 0;
        }
        texu_wnd_send_msg(activewnd, msg, (texu_i64)ch,
                          (texu_i64)(altpressed | ctlpressed | shtpressed));
    }

    _texu_env_exec_postmsg(env);

    return TEXU_OK;
}
#else
texu_status
texu_env_run(texu_env *env)
{
#if USE_TCL_AUTOMATION
    texu_env_msg    envmsg;
    texu_bool       rb  = TEXU_TRUE;
#else
    texu_i32        ch  = 0;
    texu_i32        ch2 = 0;
    texu_i32        altpressed  = 0;
    texu_i32        ctlpressed  = 0;
    texu_char       *keypressed = 0;
#endif
    texu_wnd        *activewnd  = 0;

    while (!(env->exit))
    {
        _texu_env_exec_postmsg(env);
#if USE_TCL_AUTOMATION
        rb = _texu_env_read_tcl_input(env, &envmsg);
        if (TEXU_TRUE == rb)
        {
            /*attach from TexU client*/
            _texu_env_handle_request(env, envmsg.json);
        }

        if (env->frames)
        {
            /*no key pressed*/
            /*handle idle time by calling TEXU_WM_IDLE at the top of window*/
            activewnd = (texu_wnd *)texu_stack_top(env->frames);
            if (!activewnd)
            {
                /*no more windows active*/
                break;
            }
            texu_wnd_invalidate(activewnd);
        }
#else /*not USE_TCL_AUTOMATION*/
        altpressed = 0;
        ctlpressed = 0;
        ch = texu_cio_getch(env->cio);
        if (-1 == ch && env->frames)
        {
            /*no key pressed*/
            /*handle idle time by calling TEXU_WM_IDLE at the top of window*/
            activewnd = (texu_wnd *)texu_stack_top(env->frames);
            if (!activewnd)
            {
                /*no more windows active*/
                break;
            }
            texu_wnd_send_msg(activewnd, TEXU_WM_IDLE, 0, 0);
            continue;
        }
#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
#else
        keypressed = keyname(ch);
        if ('^' == keypressed[0] && keypressed[1] != 0)
        {
            if ('[' == keypressed[1])
            {
                texu_cio_nodelay(env->cio, TEXU_TRUE);
                ch2 = texu_cio_getch(env->cio);
                texu_cio_nodelay(env->cio, TEXU_FALSE);

                keypressed = keyname(ch2);
                if (-1 != ch2)
                {
                    altpressed = 1;
                    if ('^' == keypressed[0])
                    {
                        ctlpressed = 2;
                        ch = keypressed[1];
                    }
                    else
                    {
                        ch = ch2;
                    }
                }
            }
            else if ('J' == keypressed[1] || 10 == ch)
            {
                /* enter key LF */
            }
            else
            {
                ctlpressed = 2;
                ch = keypressed[1];
            }
        }
#endif
        if (env->frames)
        {
            activewnd = (texu_wnd *)texu_stack_top(env->frames);
            if (!activewnd)
            {
                /*no more windows active*/
                break;
            }
            texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch,
                              (texu_i64)(altpressed | ctlpressed));
        }
#endif /*#else NOT USE_TCL_AUTOMATION*/
    }
    _texu_env_exec_postmsg(env);

    return TEXU_OK;
}
#endif

texu_cio *
texu_env_get_cio(texu_env *env)
{
    return env->cio;
}

void
texu_env_exit(texu_env *env)
{
    env->exit = TEXU_TRUE;
}

texu_wnd*
texu_env_get_mainwnd(texu_env *env)
{
    return env->mainwnd;
}

texu_wnd *
texu_env_get_desktop(texu_env *env)
{
    return env->desktop;
}
texu_i32
texu_env_screen_height(texu_env* env)
{
    return env->lines;
}
texu_i32
texu_env_screen_width(texu_env* env)
{
    return env->cols;
}

texu_status
texu_env_push_wnd(texu_env *env, texu_wnd *wnd)
{
    texu_status rc = TEXU_ERROR;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    if (parent == env->desktop)
    {
        /* can push if the parent window is the desktop */
        texu_stack_push(env->frames, (texu_i64)wnd);
        
        if (0 == env->mainwnd)
        {
            env->mainwnd = wnd;
        }
        rc = TEXU_OK;
    }
    return rc;
}

texu_wnd *
texu_env_pop_wnd(texu_env *env)
{
    texu_wnd *topwnd = (texu_wnd *)texu_stack_top(env->frames);
    if (topwnd != env->desktop)
    {
        texu_stack_pop(env->frames);
        if (topwnd == env->mainwnd)
        {
            env->mainwnd = 0;
        }
        return topwnd;
    }
    return 0;
}

texu_wnd *
texu_env_top_wnd(texu_env *env)
{
    texu_wnd *topwnd = (texu_wnd *)texu_stack_top(env->frames);
    return topwnd;
}

texu_status
texu_env_save_curpos(texu_env *env, texu_i32 ypos, texu_i32 xpos)
{
    texu_status rc = texu_cio_getyx(env->cio, ypos, xpos);

    env->ypos = ypos;
    env->xpos = xpos;

    return rc;
}

texu_status
texu_env_restore_curpos(texu_env *env)
{
    texu_status rc = TEXU_OK;
    texu_i32 ypos = env->ypos;
    texu_i32 xpos = env->xpos;

    env->ypos = 0;
    env->xpos = 0;
    rc = texu_cio_gotoyx(env->cio, ypos, xpos);

    return rc;
}
/*
void
texu_env_save_screen(texu_env *env)
{
    texu_cio_save_screen(env->cio, env->scrfp);
}

void
texu_env_restore_screen(texu_env *env)
{
    texu_cio_restore_screen(env->cio, env->scrfp);
}
*/
/* cursor position */
/*
# TexU window object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

struct texu_wnd
{
    texu_wnd *parent;
    texu_list *children;
    texu_wnd *activechild;
    texu_wndproc wndproc;
    texu_env *env; /* console input/output */
    texu_list *keycmds;
    texu_bool lockedupdate;
    /*
      texu_wnd_attrs    attrs;
    */
    texu_i32 y;
    texu_i32 x;
    texu_i32 height;
    texu_i32 width;
    texu_ui32 style;
    texu_ui32 exstyle;
    texu_bool enable;
    texu_bool visible;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_i32 normalcolor;
    texu_i32 disabledcolor;
    texu_i32 selectedcolor;
    texu_i32 focusedcolor;

    texu_i32 normalbg;
    texu_i32 disabledbg;
    texu_i32 selectedbg;
    texu_i32 focusedbg;

    texu_ui32 id;
    const texu_char *clsname;
    void *userdata;

    texu_menu *menu;
};

/*
#-------------------------------------------------------------------------------
# TexU default window procedure
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
texu_i32    _TexuDefWndProc_OnKeyDown(texu_wnd *, texu_i32 ch, texu_i32 alt);
texu_i32    _TexuDefWndProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);
texu_i32    _TexuDefWndProc_OnKillFocus(texu_wnd *, texu_wnd *nextwnd);
void        _TexuDefWndProc_OnSetFocus(texu_wnd *, texu_wnd *prevwnd);
void        _TexuDefWndProc_OnSetText(texu_wnd *wnd, const texu_char *text);
texu_i64    _TexuDefWndProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i64 textlen);
texu_status _TexuDefWndProc_OnCreate(texu_wnd *, texu_wnd_attrs *);
void        _TexuDefWndProc_OnDestroy(texu_wnd *);
void        _TexuDefWndProc_OnPaint(texu_wnd *, texu_cio *);
texu_bool   _TexuDefWndProc_OnShow(texu_wnd *, texu_bool);
texu_bool   _TexuDefWndProc_OnEnable(texu_wnd *, texu_bool);
void        _TexuDefWndProc_OnEraseBg(texu_wnd *, texu_cio *);
texu_i64    _TexuDefWndProc_OnMsg(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2);
void        _TexuDefWndProc_OnMove(texu_wnd *, texu_rect *, texu_bool);
void        _TexuDefWndProc_OnCommand(texu_wnd *, texu_ui32);
void        _TexuDefWndProc_OnNotify(texu_wnd *, texu_wnd_notify *);
texu_menu  *_TexuDefWndProc_OnSetMenu(texu_wnd *, texu_menu *);
void        _TexuDefWndProc_OnRedrawMenu(texu_wnd *);
void        _TexuDefWndProc_OnEnterMenu(texu_wnd *, texu_i32, texu_i32);
void        _TexuDefWndProc_OnLeaveMenu(texu_wnd *);
texu_wnd*   _TexuDefWndProc_OnQueryNextWnd(texu_wnd* wnd);
texu_wnd*   _TexuDefWndProc_OnQueryPrevWnd(texu_wnd* wnd);
texu_i64    _TexuDefWndProc_OnQueryClose(texu_wnd *wnd);
texu_i64    _TexuDefWndProc_OnClose(texu_wnd *wnd);

texu_wnd *
_TexuDefWndProc_OpenMenuWnd(
    texu_wnd *owner,
    texu_menu *menu);

texu_menu *
_TexuDefWndProc_OnSetMenu(texu_wnd *wnd, texu_menu *menu)
{
    texu_menu *oldmenu = wnd->menu;
    if (!texu_wnd_is_enable(wnd))
    {
        return oldmenu;
    }
    wnd->menu = menu;
    return oldmenu;
}

void
_TexuDefWndProc_OnRedrawMenu(texu_wnd *wnd)
{
    texu_wnd_invalidate(texu_menu_get_menubar(wnd->menu));
}

texu_wnd *
_TexuDefWndProc_OpenMenuWnd(
    texu_wnd *owner,
    texu_menu *menu)
{
    texu_wnd *wnd = 0;
    texu_wnd_attrs attrs;
    texu_status rc = TEXU_OK;
    texu_env *env = texu_wnd_get_env(owner);
    texu_wnd *desktop = texu_env_get_desktop(env);

    wnd = texu_wnd_new(env);
    if (!wnd)
    {
        return 0;
    }
    memset(&attrs, 0, sizeof(attrs));
    attrs.y = 0;
    attrs.x = 0;
    attrs.height = 1;
    attrs.width = COLS;
    attrs.enable = TEXU_TRUE;
    attrs.visible = TEXU_TRUE;
    attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
    attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);

    attrs.normalbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    attrs.disabledbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU_DISABLED);
    attrs.focusedbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_MENU);
    attrs.text = TEXUTEXT("");

    attrs.id = -1;
    attrs.clsname = TEXU_MENUWND_CLASS;
    attrs.userdata = menu;
    attrs.style = 0;
    attrs.exstyle = 0;

    rc = texu_wnd_create(wnd, desktop, &attrs);

    if (rc != TEXU_OK)
    {
        texu_wnd_del(wnd);
        return 0;
    }

    texu_wnd_send_msg(wnd, TEXU_MBM_SETOWNER, (texu_i64)owner, 0);

    texu_env_push_wnd(env, wnd);

    texu_wnd_visible(wnd, TEXU_TRUE);
    texu_wnd_invalidate(wnd);

    return wnd;
}

void
_TexuDefWndProc_OnEnterMenu(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    /*open the new window to draw menu*/
    texu_i32 selmenu = 0;
    texu_tree_item *curitem = 0;
    texu_menu_item *menuitem = 0;

    selmenu = ch - '0' - 1;
    curitem = texu_menu_get_menu(wnd->menu, selmenu);
    if (!(curitem))
    {
        curitem = texu_menu_get_menu(wnd->menu, 0);
        if (!(curitem))
        {
            return;
        }
    }
    menuitem = (texu_menu_item *)curitem->data;
    if (!(menuitem->enable))
    {
        return;
    }

    texu_menu_set_curmenu(wnd->menu, curitem);
    texu_menu_set_curmenuitem(wnd->menu, curitem->firstchild);
    _TexuDefWndProc_OpenMenuWnd(wnd, wnd->menu);
}

void
_TexuDefWndProc_OnLeaveMenu(texu_wnd *wnd)
{
    /*open the new window to draw menu*/
    /*
    texu_env_restore_screen(wnd->env);
    */
    texu_wnd_send_msg(wnd, TEXU_WM_REDRAWMENU, 0, 0);
}

void
_TexuDefWndProc_OnCommand(texu_wnd *wnd, texu_ui32 id)
{
    if (!texu_wnd_is_executable(wnd, id))
    {
        /*id from child window but it is not active*/
        return;
    }
}

void
_TexuDefWndProc_OnNotify(texu_wnd *wnd, texu_wnd_notify *notify)
{
    if (!texu_wnd_is_enable(wnd))
    {
        return;
    }
}

texu_i64
_TexuDefWndProc_OnMsg(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    return 0;
}

void
_TexuDefWndProc_OnMove(texu_wnd *wnd, texu_rect *rect, texu_bool redraw)
{
    wnd->y = rect->y;
    wnd->x = rect->x;
    wnd->height = rect->lines;
    wnd->width  = rect->cols;
    if (redraw)
    {
        texu_wnd_invalidate(wnd);
    }
}

texu_bool
_TexuDefWndProc_OnShow(texu_wnd *wnd, texu_bool visible)
{
    texu_bool oldvisible = wnd->visible;
    wnd->visible = visible;
    if (visible != oldvisible)
    {
        /*texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)wnd->env->cio, 0);*/
        texu_wnd_invalidate(wnd);
    }
    return oldvisible;
}

texu_bool
_TexuDefWndProc_OnEnable(texu_wnd *wnd, texu_bool enable)
{
    texu_bool oldenable = wnd->enable;
    wnd->enable = enable;

    texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)wnd->env->cio, 0);
    return oldenable;
}

void
_TexuDefWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    if (!texu_wnd_is_visible(wnd))
    {
        return;
    }
    /*
    if (TEXU_WS_BORDER & style)
    {
        texu_rect rc;
        texu_ui32  color = texu_env_get_syscolor(wnd->env, TEXU_COLOR_BORDER);
        texu_wnd_get_rect(wnd, &rc);
        --rc.lines;
        --rc.cols;
        texu_cio_draw_rect(dc, &rc, texu_cio_get_color(dc, color));
    }
    */
}

void
_TexuDefWndProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio)
{
    /*texu_env *env = texu_wnd_get_env(wnd);*/
    texu_char zblank[TEXU_MAX_WNDTEXT + 1];
    texu_i32 width = 0;
    texu_i32 height = 0;
    texu_i32 line = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 maxwidth = texu_env_screen_width(env);

    width = (wnd->width > maxwidth ? maxwidth : wnd->width);
    height = wnd->height;

    memset(zblank, 0, sizeof(zblank));
    texu_memset(zblank, TEXUTEXT(' '), width);

    for (line = 0; line < height; ++line)
    {
        texu_cio_draw_text(
            cio,
            line + wnd->y, wnd->x,
            zblank,
            wnd->normalcolor,
            wnd->normalbg,
            texu_wnd_get_clsname(wnd),
            texu_wnd_get_id(wnd));
    }

}


texu_status
_TexuDefWndProc_OnCreate(texu_wnd *wnd, texu_wnd_attrs *attrs)
{
    return TEXU_OK;
}

void
_TexuDefWndProc_OnDestroy(texu_wnd *wnd)
{
    texu_menu *menu = wnd->menu;
    if (menu)
    {
        texu_menu_del(menu);
    }
}

void
_TexuDefWndProc_OnSetText(texu_wnd *wnd, const texu_char *text)
{
    texu_wnd *frame = texu_wnd_get_frame(wnd);
    texu_wnd *activewnd = texu_wnd_get_activechild(frame);
    texu_i32 y = 0;
    texu_i32 x = 0;
    texu_i32 width = 0;
    texu_cio *cio = 0;

    if (text)
    {
        texu_strcpy(wnd->text, text);
    }
    else
    {
        texu_strcpy(wnd->text, TEXUTEXT(""));
    }
    texu_wnd_invalidate(wnd);

    if (activewnd)
    {
        y = texu_wnd_get_y(activewnd);
        x = texu_wnd_get_x(activewnd);
        width = texu_wnd_get_width(activewnd);
        cio = texu_env_get_cio(activewnd->env);
        texu_cio_gotoyx(cio, y, x + width - 1);
    }
}

texu_i64
_TexuDefWndProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i64 textlen)
{
    texu_i64 len = texu_strlen(wnd->text);

    if (textlen <= 0 || 0 == text)
    {
        return len;
    }

    memset(text, 0, sizeof(texu_char)*textlen);
    textlen = TEXU_MIN(len, textlen);
/*    memcpy(text, wnd->text, sizeof(texu_char)*textlen);*/
    texu_strncpy(text, wnd->text, textlen);
    
    return textlen;
}

texu_i32
_TexuDefWndProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
{
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_env *env = texu_wnd_get_env(wnd);
    if (parent && nextwnd && texu_wnd_is_active(nextwnd))
    {
        parent->activechild = nextwnd;
    }
    texu_env_show_cursor(env, TEXU_FALSE);
    return TEXU_OK;
}

void
_TexuDefWndProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
{
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_env *env = texu_wnd_get_env(wnd);

    if (parent && texu_wnd_is_active(wnd))
    {
        parent->activechild = wnd;
    }
    texu_env_set_focus(env, wnd);
}

texu_wnd* _TexuDefWndProc_OnQueryNextWnd(texu_wnd* wnd)
{
    return 0;
}

texu_wnd* _TexuDefWndProc_OnQueryPrevWnd(texu_wnd* wnd)
{
    return 0;
}

texu_i32
_TexuDefWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_i64 rc = 0;
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = 0;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
    texu_wnd_keycmd *keycmd = 0;
    texu_menu *menu = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    /* the current frame window is active */
    if (activewnd && activewnd != wnd)
    {
        texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
    }
    else if (activewnd && activewnd == wnd)
    {
        activewnd = texu_wnd_get_activechild(activewnd);
        if (activewnd)
        {
            texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
        }
    }
    return 0;
}

texu_i32
_TexuDefWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_i64 rc = 0;
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = 0;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
    texu_wnd_keycmd *keycmd = 0;
    texu_menu *menu = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    if (!texu_wnd_is_enable(wnd))
    {
        return 0;
    }
    keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
    if (keycmd && parent == desktop)
    {
        /* if there are any hotkey registered */
        return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_i64)keycmd->cmd, alt);
    }
    if (parent == desktop)
    {
        if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
        {
            menu = wnd->menu;
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
                nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
            }
            else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
            {
                nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
            }

            /* kill and set the new active window */
            if (nextwnd)
            {
                /*active child may need to be done its action*/
                texu_wnd *activechild = texu_wnd_get_activechild(activewnd);
                if (activechild)
                {
                    rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_i64)activewnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_i64)activechild, 0);
                        return -1;
                    }
                }
                /*active window need to be done something by itself*/
                rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (rc != TEXU_OK)
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                    return -1;
                }
                rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                /* the new active window */
                wnd->activechild = nextwnd;
                return 1;
            }
            else
            {
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        } /* child window is active */
        else
        {
            /* the current frame window is active */
            if (activewnd)
            {
                activewnd = texu_wnd_get_activechild(activewnd);
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        }
    }
    else
    {
        if (activewnd)
        {
            return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
        }
    }
    return 0;
}

texu_i64
_TexuDefWndProc_OnClose(texu_wnd *wnd)
{
    texu_i64 rc = _TexuDefWndProc_OnQueryClose(wnd);
    /*if the returned code is not equal to TEXU_OK, that means*/
    /*we could destroy it then*/
    if (rc != TEXU_OK)
    {
        return rc;
    }
    return rc;
}

texu_i64
_TexuDefWndProc_OnQueryClose(texu_wnd *wnd)
{
    return TEXU_OK;
}

texu_i64
TexuDefWndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_SETMENU:
            _TexuDefWndProc_OnSetMenu(wnd, (texu_menu *)param1);
            return 0;

        case TEXU_WM_REDRAWMENU:
            _TexuDefWndProc_OnRedrawMenu(wnd);
            return 0;

        case TEXU_WM_ENTERMENU:
            _TexuDefWndProc_OnEnterMenu(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;

        case TEXU_WM_LEAVEMENU:
            _TexuDefWndProc_OnLeaveMenu(wnd);
            return 0;

        case TEXU_WM_NOTIFY:
            _TexuDefWndProc_OnNotify(wnd, (texu_wnd_notify *)param1);
            return 0;

        case TEXU_WM_COMMAND:
            _TexuDefWndProc_OnCommand(wnd, (texu_ui32)param1);
            return 0;

        case TEXU_WM_MOVE:
            _TexuDefWndProc_OnMove(wnd, (texu_rect *)param1, (texu_bool)param2);
            return 0;

        case TEXU_WM_SHOW:
            return _TexuDefWndProc_OnShow(wnd, (texu_bool)param1);

        case TEXU_WM_ENABLE:
            return _TexuDefWndProc_OnEnable(wnd, (texu_bool)param1);

        case TEXU_WM_CHAR:/*printable characters*/
            return _TexuDefWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);

        case TEXU_WM_KEYDOWN:/*virtual key*/
            return _TexuDefWndProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);

        case TEXU_WM_QUERYNEXTWND:
            return (texu_i64)_TexuDefWndProc_OnQueryNextWnd(wnd);

        case TEXU_WM_QUERYPREVWND:
            return (texu_i64)_TexuDefWndProc_OnQueryPrevWnd(wnd);
            
        case TEXU_WM_SETFOCUS:
            _TexuDefWndProc_OnSetFocus(wnd, (texu_wnd *)param1);
            break;

        case TEXU_WM_KILLFOCUS:
            return _TexuDefWndProc_OnKillFocus(wnd, (texu_wnd *)param1);

        case TEXU_WM_SETTEXT:
            _TexuDefWndProc_OnSetText(wnd, (const texu_char *)param1);
            break;

        case TEXU_WM_GETTEXT:
            return _TexuDefWndProc_OnGetText(wnd, (texu_char *)param1, param2);

        case TEXU_WM_PAINT:
            _TexuDefWndProc_OnPaint(wnd, (texu_cio *)param1);
            break;

        case TEXU_WM_ERASEBG:
            _TexuDefWndProc_OnEraseBg(wnd, (texu_cio *)param1);
            break;

        case TEXU_WM_CREATE:
            return _TexuDefWndProc_OnCreate(wnd, (texu_wnd_attrs *)param1);

        case TEXU_WM_DESTROY:
            _TexuDefWndProc_OnDestroy(wnd);
            break;

        case TEXU_WM_CLOSE:
            return _TexuDefWndProc_OnClose(wnd);

        case TEXU_WM_QUERYCLOSE:
            return _TexuDefWndProc_OnQueryClose(wnd);
    }
    return 0;
}
/*
#
#
*/

texu_list_item *_texu_wnd_find_child(texu_wnd *wnd, texu_ui32 id);
texu_wnd *_texu_wnd_is_window(texu_wnd *parent, texu_wnd *wnd);
texu_list_item *_texu_wnd_find_keycmd(texu_wnd *wnd, texu_i32 key, texu_i32 alt);

texu_wnd *
_texu_wnd_is_window(texu_wnd *parent, texu_wnd *wnd)
{
    texu_wnd *findwnd = 0;
    findwnd = texu_wnd_firstchild(parent);

    if (parent && wnd)
    {
        if (parent == wnd)
        {
            return parent;
        }
    }
    if (findwnd)
    {
        if (findwnd == wnd)
        {
            return findwnd;
        }
        findwnd = _texu_wnd_is_window(findwnd, wnd);
        if (findwnd)
        {
            return findwnd;
        }
    }
    else
    {
        findwnd = texu_wnd_nextwnd(parent);
        while (findwnd)
        {
            findwnd = _texu_wnd_is_window(findwnd, wnd);
            if (findwnd)
            {
                return findwnd;
            }
            findwnd = texu_wnd_nextwnd(findwnd);
        }
    }
    return 0;
}

texu_wnd *
texu_wnd_is_window(texu_wnd *wnd)
{
    texu_wnd *parent = (wnd ? wnd->parent : 0);
    /*  texu_wnd* findwnd = 0;*/
    if (!wnd)
    {
        return 0;
    }
    if (!parent)
    {
        parent = texu_env_get_desktop(wnd->env);
    }
    return _texu_wnd_is_window(parent, wnd);
}

texu_bool
texu_wnd_is_executable(texu_wnd *wnd, texu_ui32 id)
{
    texu_wnd *child = texu_wnd_find_child(wnd, id);
    if (child && TEXU_FALSE == texu_wnd_is_enable(child))
    {
        return TEXU_FALSE;
    }
    return TEXU_TRUE;
}

texu_wnd *
texu_wnd_new(texu_env *env)
{
    texu_wnd *wnd = (texu_wnd *)malloc(sizeof(texu_wnd));
    if (wnd)
    {
        memset(wnd, 0, sizeof(texu_wnd));
        wnd->env = env;
        wnd->children = texu_list_new();
        wnd->keycmds = texu_list_new();
    }
    return wnd;
}

texu_status
texu_wnd_create(texu_wnd *wnd, texu_wnd *parent, const texu_wnd_attrs *attrs)
{
    texu_status rc = TEXU_OK;
    texu_wndproc wndproc = 0;
/*    texu_i32 xparent = (parent ? texu_wnd_get_x(parent) : 0);
    texu_i32 yparent = (parent ? texu_wnd_get_y(parent) : 0);*/
    texu_env *env = wnd->env;
    texu_i32 cx = texu_env_screen_width(env);
    texu_i32 cy = texu_env_screen_height(env);
    texu_i32 width = 0;
    texu_i32 height = 0;
    texu_i32 ypos = 0;
    texu_i32 xpos = 0;

    /* find the window procedure */
    wndproc = _texu_env_find_wndproc(wnd->env, attrs->clsname);
    if (!wndproc)
    {
        return TEXU_NOTREGISTERED_CLASS;
    }
    wnd->y          = attrs->y;
    if (attrs->y >= cy)
    {
        wnd->y = cy - 1;
        wnd->height = 0;
    }
    else
    {
        wnd->y = attrs->y;
        ypos = attrs->y + attrs->height;
        if (ypos >= cy)
        {
            height = TEXU_MAX(0, cy - attrs->y);
        }
        else
        {
            height = attrs->height;
        }
        wnd->height = height;
    }
    if (attrs->x >= cx)
    {
        wnd->x = cx - 1;
        wnd->width = 0;
    }
    else
    {
        wnd->x = attrs->x;
        xpos = attrs->x + attrs->width;
        if (xpos >= cx)
        {
            width = TEXU_MAX(0, cx - attrs->x);
        }
        else
        {
            width = attrs->width;
        }
        wnd->width = width;
    }
    
    wnd->style      = attrs->style;
    wnd->exstyle    = attrs->exstyle;
    wnd->enable     = attrs->enable;
    wnd->visible    = attrs->visible;
    if (attrs->text && texu_strlen(attrs->text))
    {
        texu_strcpy(wnd->text, attrs->text);
    }
    wnd->normalcolor    = attrs->normalcolor;
    wnd->disabledcolor  = attrs->disabledcolor;
    wnd->focusedcolor   = attrs->focusedcolor;
    wnd->selectedcolor  = attrs->selectedcolor;

    wnd->normalbg       = attrs->normalbg;
    wnd->disabledbg     = attrs->disabledbg;
    wnd->focusedbg      = attrs->focusedbg;
    wnd->selectedbg     = attrs->selectedbg;

    wnd->id             = attrs->id;
    wnd->clsname        = attrs->clsname;
    wnd->userdata       = attrs->userdata;
    wnd->wndproc        = wndproc;
    wnd->parent         = parent;
    if (parent)
    {
        texu_wnd_add_child(parent, wnd);
    }
    wnd->lockedupdate = TEXU_FALSE;

    rc = texu_wnd_send_msg(wnd, TEXU_WM_CREATE, (texu_i64)attrs, 0);
    if (rc != TEXU_OK)
    {
        /**/
    }
    return rc;
}

void
texu_wnd_del(texu_wnd *wnd)
{
    if (wnd)
    {
        /*texu_wnd_destroy(wnd);*/      /*delete all children*/
        texu_list_del(wnd->children);
        texu_list_del(wnd->keycmds);
        free(wnd);
        wnd = 0;
    }
}

void
_texu_wnd_destroy(texu_wnd *wnd)
{
    texu_wnd *childwnd = 0;
    texu_wnd *prevwnd = 0;

    if (!wnd)
    {
        return;
    }
    childwnd = texu_wnd_lastchild(wnd);
    while (childwnd)
    {
        /* prev wnd */
        prevwnd = childwnd;
        childwnd = texu_wnd_prevwnd(childwnd);

        _texu_wnd_destroy(prevwnd);
    }
    texu_wnd_send_msg(wnd, TEXU_WM_DESTROY, 0, 0);
}

void
texu_wnd_destroy(texu_wnd *wnd)
{
    texu_wnd *parent = wnd->parent;

    _texu_wnd_destroy(wnd);
    if (parent)
    {
        texu_wnd_remove_child(parent, wnd);
    }
}

texu_i64
texu_wnd_post_msg(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    texu_i64 rc = 0;
    struct texu_msg* umsg = 0;

    if (wnd && wnd->wndproc)
    {
        texu_env *env = wnd->env;
        umsg = (struct texu_msg*)malloc(sizeof(struct texu_msg));
        if (umsg)
        {
            umsg->wnd = wnd;
            umsg->msg = msg;
            umsg->param1 = param1;
            umsg->param2 = param2;
            texu_queue_enqueue(env->msgques, (texu_i64)umsg);
        }
    }
    return rc;
}

texu_i64
texu_wnd_send_msg(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    texu_i64 rc = 0;

    if (wnd && wnd->wndproc)
    {
        rc = wnd->wndproc(wnd, msg, param1, param2);
    }
    return rc;
}

texu_i64
texu_wnd_close(texu_wnd *wnd)
{
    return texu_wnd_send_msg(wnd, TEXU_WM_CLOSE, 0, 0);
}


texu_i64
_texu_wnd_invalidate(texu_wnd *wnd)
{
    texu_wnd *childwnd = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    if (!wnd || !texu_wnd_is_visible(wnd) || texu_env_is_update_locked(env))
    {
        return -1;
    }
    /* firstly, paint itself */
    texu_wnd_send_msg(wnd, TEXU_WM_ERASEBG, (texu_i64)(env->cio), 0);
    texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)(env->cio), 0);

    childwnd = texu_wnd_firstchild(wnd);
    while (childwnd)
    {
        if (texu_wnd_is_visible(childwnd))
        {
            _texu_wnd_invalidate(childwnd);
        }
        childwnd = texu_wnd_nextwnd(childwnd);
    }

    return 0;
}


texu_i64
texu_wnd_invalidate(texu_wnd *wnd)
{
    if (!wnd)
    {
        return -1;
    }
    if (0 == _texu_wnd_invalidate(wnd))
    {
#if (defined WIN32 && defined _WINDOWS)
        /*Double buffering technique to prevent the freaking screen*/
        texu_env *env = texu_wnd_get_env(wnd);
        texu_i32 cx = env->cxScreen;
        texu_i32 cy = env->cyScreen;
        BitBlt(env->hdc, 0, 0, cx, cy, env->hmemdc, 0, 0, SRCCOPY);
#endif
    }
    return 0;
}

texu_wnd *
texu_wnd_get_frame(texu_wnd *wnd)
{
    texu_wnd *framewnd = wnd;
    texu_wnd *parent = texu_wnd_get_parent(framewnd);
    texu_wnd *desktop = texu_env_get_desktop(framewnd->env);

    if (!wnd || wnd == desktop)
    {
        return 0;
    }
    while (parent != desktop)
    {
        framewnd = parent;
        parent = texu_wnd_get_parent(framewnd);
    }
    return framewnd;
}

texu_wnd *
texu_wnd_get_parent(texu_wnd *wnd)
{
    texu_wnd *parent = (wnd ? wnd->parent : 0);
    return parent;
}

texu_ui32
texu_wnd_get_id(texu_wnd *wnd)
{
    texu_ui32 id = (wnd ? wnd->id : 0);
    return id;
}

texu_wnd *
texu_wnd_set_activechild(texu_wnd *wnd, texu_wnd *childwnd)
{
    texu_wnd *oldchild = 0;
    if (childwnd == wnd)
    {
        oldchild = wnd->activechild;
        wnd->activechild = childwnd;
    }
    return oldchild;
}

texu_wnd *
texu_wnd_get_activechild(texu_wnd *wnd)
{
    texu_wnd *activechild = (wnd ? wnd->activechild : 0);
    if (!activechild && wnd)
    {
        activechild = texu_wnd_get_first_activechild(wnd);
        if (activechild)
        {
            wnd->activechild = activechild;
        }
    }
    return activechild;
}

texu_wnd *
texu_wnd_get_first_activechild(texu_wnd *wnd)
{
    texu_list_item *item = (wnd && wnd->children ? texu_list_first(wnd->children) : 0);
    texu_wnd *activechild = 0;
    while (item)
    {
        activechild = (texu_wnd *)item->data;
        if (texu_wnd_is_visible(activechild) &&
            texu_wnd_is_enable(activechild))
        {
            return activechild;
        }
        item = item->next;
    }
    return 0;
}

texu_wnd *
texu_wnd_get_last_activechild(texu_wnd *wnd)
{
    texu_list_item *item = (wnd && wnd->children ? texu_list_last(wnd->children) : 0);
    texu_wnd *activechild = 0;
    while (item)
    {
        activechild = (texu_wnd *)item->data;
        if (texu_wnd_is_visible(activechild) &&
            texu_wnd_is_enable(activechild))
        {
            return activechild;
        }
        item = item->prev;
    }
    return 0;
}

texu_wnd *
texu_wnd_get_next_activechild(texu_wnd *wnd, texu_wnd *childwnd)
{
    texu_list_item *item = 0;
    texu_wnd *activechild = 0;
    if (!childwnd)
    {
        return texu_wnd_get_first_activechild(wnd);
    }
    /* find the current child window */
    item = texu_list_find_first(wnd->children, (texu_i64)childwnd);
    if (item)
    {
        item = item->next;
        while (item)
        {
            activechild = (texu_wnd *)item->data;
            if (texu_wnd_is_visible(activechild) &&
                texu_wnd_is_enable(activechild))
            {
                return activechild;
            }
            item = item->next;
        }
    }

    return 0;
}

texu_bool
texu_wnd_is_active(texu_wnd *wnd)
{
    if (wnd &&
        texu_wnd_is_visible(wnd) &&
        texu_wnd_is_enable(wnd))
    {
        return TEXU_TRUE;
    }
    return TEXU_FALSE;
}

texu_wnd *
texu_wnd_get_prev_activechild(texu_wnd *wnd, texu_wnd *childwnd)
{
    texu_list_item *item = 0;
    texu_wnd *activechild = 0;
    if (!childwnd)
    {
        return texu_wnd_get_last_activechild(wnd);
    }
    /* find */
    item = texu_list_rfind_last(wnd->children, (texu_i64)childwnd);
    if (item)
    {
        item = item->prev;
        while (item)
        {
            activechild = (texu_wnd *)item->data;
            if (texu_wnd_is_visible(activechild) &&
                texu_wnd_is_enable(activechild))
            {
                return activechild;
            }
            item = item->prev;
        }
    }

    return 0;
}

texu_i32
texu_wnd_children(texu_wnd *wnd)
{
    return (wnd ? texu_list_count(wnd->children) : 0);
}

texu_wnd *
texu_wnd_firstchild(texu_wnd *wnd)
{
    texu_list_item *item = (wnd && wnd->children ? texu_list_first(wnd->children) : 0);
    return (item ? (texu_wnd *)item->data : 0);
}

texu_wnd *
texu_wnd_lastchild(texu_wnd *wnd)
{
    texu_list_item *item = (wnd && wnd->children ? texu_list_last(wnd->children) : 0);
    return (item ? (texu_wnd *)item->data : 0);
}

texu_wnd *
texu_wnd_nextwnd(texu_wnd *wnd)
{
    texu_list_item *item = 0;
    texu_wnd *parent = (wnd ? wnd->parent : 0);
    if (!parent)
    {
        return 0;
    }
    /* find */
    item = texu_list_find_first(parent->children, (texu_i64)wnd);
    if (item)
    {
        item = item->next;
    }

    return (item ? (texu_wnd *)item->data : 0);
}

texu_wnd *
texu_wnd_prevwnd(texu_wnd *wnd)
{
    texu_list_item *item = 0;
    texu_wnd *parent = (wnd ? wnd->parent : 0);
    if (!parent)
    {
        return 0;
    }
    /* find */
    item = texu_list_rfind_last(parent->children, (texu_i64)wnd);
    if (item)
    {
        item = item->prev;
    }

    return (item ? (texu_wnd *)item->data : 0);
}

texu_status
texu_wnd_visible(texu_wnd *wnd, texu_bool visible)
{
    return texu_wnd_send_msg(wnd, TEXU_WM_SHOW, (texu_i64)visible, 0);
}

texu_status
texu_wnd_enable(texu_wnd *wnd, texu_bool enable)
{
    return texu_wnd_send_msg(wnd, TEXU_WM_ENABLE, (texu_i64)enable, 0);
}

texu_bool
texu_wnd_is_visible(texu_wnd *wnd)
{
    return (wnd ? wnd->visible : TEXU_FALSE);
}
texu_bool
texu_wnd_is_enable(texu_wnd *wnd)
{
    return (wnd ? wnd->enable : TEXU_FALSE);
}

texu_i32
texu_wnd_get_width(texu_wnd *wnd)
{
    return (wnd ? wnd->width : 0);
}

texu_i32
texu_wnd_get_height(texu_wnd *wnd)
{
    return (wnd ? wnd->height : 0);
}

texu_i32
texu_wnd_get_y(texu_wnd *wnd)
{
    return (wnd ? wnd->y : 0);
}

texu_i32
texu_wnd_get_x(texu_wnd *wnd)
{
    return (wnd ? wnd->x : 0);
}

texu_ui32
texu_wnd_get_style(texu_wnd *wnd)
{
    return (wnd ? wnd->style : 0);
}

void
texu_wnd_move(texu_wnd *wnd, texu_i32 y, texu_i32 x, texu_i32 h, texu_i32 w, texu_bool redraw)
{
    texu_rect rect = {y, x, h, w};
    texu_wnd_send_msg(wnd, TEXU_WM_MOVE, (texu_i64)&rect, redraw);
}

void
texu_wnd_set_style(texu_wnd *wnd, texu_ui32 style)
{
    if (wnd)
    {
        wnd->style = style;
        texu_wnd_invalidate(wnd);
    }
}

void
texu_wnd_set_exstyle(texu_wnd *wnd, texu_ui32 exstyle)
{
    if (wnd)
    {
        wnd->exstyle = exstyle;
        texu_wnd_invalidate(wnd);
    }
}

texu_ui32
texu_wnd_get_exstyle(texu_wnd *wnd)
{
    return (wnd ? wnd->exstyle : 0);
}

texu_list_item *
_texu_wnd_find_keycmd(texu_wnd *wnd, texu_i32 key, texu_i32 alt)
{
    texu_wnd_keycmd *keycmd = 0;
    texu_list_item *item = (wnd ? texu_list_first(wnd->keycmds) : 0);

    while (item)
    {
        keycmd = (texu_wnd_keycmd *)item->data;
        if (keycmd->key == key && keycmd->alt == alt)
        {
            return item;
        }
        item = item->next;
    }
    return 0;
}

texu_wnd_keycmd *
texu_wnd_find_keycmd(texu_wnd *wnd, texu_i32 key, texu_i32 alt)
{
    texu_list_item *item = (wnd ? _texu_wnd_find_keycmd(wnd, key, alt) : 0);
    return (item ? (texu_wnd_keycmd *)item->data : 0);
}

texu_status
texu_wnd_add_keycmd(texu_wnd *wnd, texu_i32 key, texu_ui32 cmd, texu_i32 alt)
{
    texu_status rc = TEXU_OK;
    if (wnd)
    {
        texu_wnd_keycmd *keycmd = (texu_wnd_keycmd *)malloc(sizeof(texu_wnd_keycmd));
        keycmd->key = key;
        keycmd->cmd = cmd;
        keycmd->alt = alt;
        
        texu_list_add(wnd->keycmds, (texu_i64)keycmd);
    }
    return rc;
}

texu_status
texu_wnd_add_child(texu_wnd *wnd, texu_wnd *childwnd)
{
    texu_status rc = TEXU_OK;
    texu_wnd *findwnd = 0;

    if (!wnd || !childwnd)
    {
        return TEXU_ERROR;
    }
    findwnd = texu_wnd_find_child(wnd, childwnd->id);
    if (findwnd)
    {
        return TEXU_DUPLICATED_ID;
    }

    texu_list_add(wnd->children, (texu_i64)childwnd);
    return rc;
}

texu_status
texu_wnd_remove_child(texu_wnd *wnd, texu_wnd *childwnd)
{
    texu_status rc = TEXU_OK;
    texu_list_item *item = 0;

    if (!wnd || !childwnd)
    {
        return TEXU_ERROR;
    }
    item = _texu_wnd_find_child(wnd, childwnd->id);
    if (!item)
    {
        return TEXU_NOTFOUND;
    }

    texu_list_remove(wnd->children, item);
    return rc;
}

texu_wnd *
texu_wnd_find_child(texu_wnd *wnd, texu_ui32 id)
{
    texu_list_item *item = (wnd ? _texu_wnd_find_child(wnd, id) : 0);
    return (item ? (texu_wnd *)item->data : 0);
}

texu_list_item *
_texu_wnd_find_child(texu_wnd *wnd, texu_ui32 id)
{
    texu_wnd *childwnd = 0;
    texu_list_item *item = (wnd ? texu_list_first(wnd->children) : 0);

    while (item)
    {
        childwnd = (texu_wnd *)item->data;
        if (childwnd->id == id)
        {
            return item;
        }
        item = item->next;
    }
    return 0;
}

void
texu_wnd_set_text(texu_wnd *wnd, const texu_char *text)
{
    texu_wnd_send_msg(wnd, TEXU_WM_SETTEXT, (texu_i64)text, 0);
}

texu_i32
texu_wnd_get_text(texu_wnd *wnd, texu_char *text, texu_i32 len)
{
    return (texu_i32)texu_wnd_send_msg(wnd, TEXU_WM_GETTEXT, (texu_i64)text, len);
}


void
texu_wnd_set_bgcolor(texu_wnd *wnd, texu_ui32 normalbg, texu_ui32 disabledbg)
{
    if (wnd)
    {
        wnd->normalbg = normalbg;
        wnd->disabledbg = disabledbg;
    }
}

texu_status
texu_wnd_get_bgcolor(texu_wnd *wnd, texu_ui32 *normalbg, texu_ui32 *disabledbg)
{
    if (wnd)
    {
        *normalbg = wnd->normalbg;
        *disabledbg = wnd->disabledbg;
    }
    return TEXU_OK;
}

void
texu_wnd_set_bgfocused_color(texu_wnd *wnd, texu_ui32 color)
{
    if (wnd)
    {
        wnd->focusedbg = color;
    }
}

texu_ui32
texu_wnd_get_bgfocused_color(texu_wnd *wnd)
{
    return (wnd ? wnd->focusedbg : 0);
}

void
texu_wnd_set_bgselected_color(texu_wnd *wnd, texu_ui32 color)
{
    if (wnd)
    {
        wnd->selectedbg = color;
    }
}

texu_ui32
texu_wnd_get_bgselected_color(texu_wnd *wnd)
{
    return (wnd ? wnd->selectedbg : 0);
}


void
texu_wnd_set_color(texu_wnd *wnd, texu_ui32 normalcolor, texu_ui32 disabledcolor)
{
    if (wnd)
    {
        wnd->normalcolor = normalcolor;
        wnd->disabledcolor = disabledcolor;
    }
}

texu_status
texu_wnd_get_color(texu_wnd *wnd, texu_ui32 *normalcolor, texu_ui32 *disabledcolor)
{
    if (wnd)
    {
        *normalcolor = wnd->normalcolor;
        *disabledcolor = wnd->disabledcolor;
    }
    return TEXU_OK;
}

void
texu_wnd_set_focused_color(texu_wnd *wnd, texu_ui32 color)
{
    if (wnd)
    {
        wnd->focusedcolor = color;
    }
}

texu_ui32
texu_wnd_get_focused_color(texu_wnd *wnd)
{
    return (wnd ? wnd->focusedcolor : 0);
}

void
texu_wnd_set_selected_color(texu_wnd *wnd, texu_ui32 color)
{
    if (wnd)
    {
        wnd->selectedcolor = color;
    }
}


texu_ui32
texu_wnd_get_selected_color(texu_wnd *wnd)
{
    return (wnd ? wnd->selectedcolor : 0);
}

const texu_char *
texu_wnd_get_clsname(texu_wnd *wnd)
{
    return (wnd ? wnd->clsname : 0);
}

void *
texu_wnd_get_userdata(texu_wnd *wnd)
{
    return (wnd ? wnd->userdata : 0);
}

void
texu_wnd_set_userdata(texu_wnd *wnd, void *userdata)
{
    if (wnd)
    {
        wnd->userdata = userdata;
    }
}

texu_env *
texu_wnd_get_env(texu_wnd *wnd)
{
    return (wnd ? wnd->env : 0);
}

void
texu_wnd_get_rect(texu_wnd *wnd, texu_rect *rect)
{
    if (rect && wnd)
    {
        rect->y = wnd->y;
        rect->x = wnd->x;
        rect->lines = wnd->height;
        rect->cols = wnd->width;
    }
}

texu_cio *
texu_wnd_get_cio(texu_wnd *wnd)
{
    return (wnd ? wnd->env->cio : 0);
}

void
texu_wnd_lock_update(texu_wnd *wnd, texu_bool locked)
{
    if (wnd && wnd->lockedupdate != locked)
    {
        if (!locked)
        {
            texu_wnd_invalidate(wnd);
        }
        wnd->lockedupdate = locked;
    }
}

texu_bool
texu_wnd_is_update_locked(texu_wnd *wnd)
{
    return (wnd ? wnd->lockedupdate : TEXU_FALSE);
}

texu_status
texu_wnd_save_curpos(texu_wnd *wnd)
{
    if (wnd)
    {
        texu_i32 y = texu_wnd_get_y(wnd);
        texu_i32 x = texu_wnd_get_x(wnd);
        texu_i32 width = texu_wnd_get_width(wnd);
        return texu_env_save_curpos(wnd->env, y, x + width - 1);
    }
    return TEXU_ERROR;
}

texu_status
texu_wnd_restore_curpos(texu_wnd *wnd)
{
    return (wnd ? texu_env_restore_curpos(wnd->env) : TEXU_ERROR);
}

texu_menu *
texu_wnd_set_menu(texu_wnd *wnd, texu_menu *newmenu)
{
    if (wnd)
    {
        texu_menu *oldmenu = wnd->menu;
        wnd->menu = newmenu;
        return oldmenu;
    }
    return 0;
}

texu_menu *
texu_wnd_get_menu(texu_wnd *wnd)
{
    return (wnd ? wnd->menu : 0);
}
/*
#-------------------------------------------------------------------------------
# TexU frame window procedure
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/

texu_i32    _TexuFrameWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
void        _TexuFrameWndProc_Notify(texu_wnd *frame, texu_wnd *child, texu_ui32 code);

void
_TexuFrameWndProc_Notify(texu_wnd *frame, texu_wnd *wnd, texu_ui32 code)
{
    texu_wnd_notify notify;

    notify.wnd = wnd;
    notify.id = texu_wnd_get_id(wnd);
    notify.code = code;
    texu_wnd_send_msg(frame, TEXU_WM_NOTIFY, (texu_i64)&notify, 0);
}

texu_i32
_TexuFrameWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_i64 rc = 0;
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = 0;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
    texu_wnd_keycmd *keycmd = 0;
    texu_menu *menu = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
    if (keycmd && parent == desktop)
    {
        /* if there are any hotkey registered */
        return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_i64)keycmd->cmd, alt);
    }
    if (parent == desktop)
    {
        if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
        {
            menu = wnd->menu;
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
                nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
            }
            else if (ch == chPrevKey || ((alt & TEXU_KEYPRESSED_SHIFT) && (ch == TEXU_KEY_TAB)))
            {
                nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
            }

            /* kill and set the new active window */
            if (nextwnd)
            {
                /*active child may need to be done its action*/
                texu_wnd *activechild = texu_wnd_get_activechild(activewnd);
                if (activechild)
                {
                    rc = texu_wnd_send_msg(activechild, TEXU_WM_KILLFOCUS, (texu_i64)activewnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activechild, TEXU_WM_SETFOCUS, (texu_i64)activechild, 0);
                        return -1;
                    }
                }
                /*active window need to be done something by itself*/
                rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (rc != TEXU_OK)
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                    return -1;
                }
                rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                /* the new active window */
                wnd->activechild = nextwnd;
                return 1;
            }
            else
            {
                /* rounded robin active child */
                if ((ch == chNextKey) || (ch == chPrevKey) || (ch == TEXU_KEY_TAB))
                {
                    texu_wnd *lastwnd = texu_wnd_get_last_activechild(wnd);
                    texu_wnd *firstwnd = texu_wnd_get_first_activechild(wnd);
                    texu_ui32 code = 0;
                    
                    if ((activewnd == lastwnd) && (ch == chNextKey))
                    {
                        /*move the next active window to the first active child*/
                        nextwnd = firstwnd;
                        code = TEXU_WN_GOFIRSTCHILD;
                    }
                    else if ((activewnd == firstwnd) && 
                             (ch == chPrevKey ||
                             (ch == TEXU_KEY_TAB && (alt & TEXU_KEYPRESSED_SHIFT)))) /*shift + tab*/
                    {
                        /*move the next active window to the last active child*/
                        nextwnd = lastwnd;
                        code = TEXU_WN_GOLASTCHILD;
                    }
                    if (NULL == nextwnd)
                    {
                        return 0;
                    }
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                        return -1;
                    }
                    rc = texu_wnd_send_msg((nextwnd ? nextwnd : wnd), TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                    /* the new active window */
                    wnd->activechild = nextwnd;
                    
                    /* send notification to the window*/
                    _TexuFrameWndProc_Notify(wnd, nextwnd, code);
                    return 1;
                }
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        } /* child window is active */
        else
        {
            /* the current frame window is active */
            if (activewnd)
            {
                activewnd = texu_wnd_get_activechild(activewnd);
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        }
    }
    else
    {
        if (activewnd)
        {
            return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
        }
    }
    return 0;
}

texu_i64
TexuFrameWndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_KEYDOWN:
            return _TexuFrameWndProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


texu_i32
_TexuManualFrameWndProc_OnKeyDown(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
{
    texu_i64 rc = 0;
    texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
    texu_wnd *nextwnd = 0;
    texu_wnd *parent = texu_wnd_get_parent(wnd);
    texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
    texu_wnd_keycmd *keycmd = 0;
    texu_menu *menu = 0;
    texu_env *env = texu_wnd_get_env(wnd);
    texu_i32 chNextKey = texu_env_get_movenext(env);
    texu_i32 chPrevKey = texu_env_get_moveprev(env);

    keycmd = texu_wnd_find_keycmd(wnd, ch, alt);
    if (keycmd && parent == desktop)
    {
        /* if there are any hotkey registered */
        return texu_wnd_send_msg(wnd, TEXU_WM_COMMAND, (texu_i64)keycmd->cmd, alt);
    }
    if (parent == desktop)
    {
        if (ch != -1 && (alt & TEXU_KEYPRESSED_ALT))
        {
            menu = wnd->menu;
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
                nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
            }
            else if ((ch == chPrevKey) || (ch == TEXU_KEY_TAB && (alt & TEXU_KEYPRESSED_SHIFT)))
            {
                nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
            }

            /* kill and set the new active window */
            if (nextwnd)
            {
                rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                if (rc != TEXU_OK)
                {
                    texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                    return -1;
                }
                rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                /* the new active window */
                wnd->activechild = nextwnd;
                return 1;
            }
            else
            {
                /* rounded robin active child */
                if ((ch == chNextKey) || (ch == chPrevKey) || (ch == TEXU_KEY_TAB))
                {
                    texu_wnd *lastwnd = texu_wnd_get_last_activechild(wnd);
                    texu_wnd *firstwnd = texu_wnd_get_first_activechild(wnd);
                    texu_ui32 code = 0;

                    if ((activewnd == lastwnd) && (ch == chNextKey))
                    {
                        /*move the next active window to the first active child*/
                        nextwnd = firstwnd;
                        code = TEXU_WN_GOFIRSTCHILD;
                    }
                    else if ((activewnd == firstwnd) &&
                             (ch == chPrevKey ||
                             (ch == TEXU_KEY_TAB && (alt & TEXU_KEYPRESSED_SHIFT)))) /*shift + tab*/
                    {
                        /*move the next active window to the last active child*/
                        nextwnd = lastwnd;
                        code = TEXU_WN_GOLASTCHILD;
                    }
                    if (NULL == nextwnd)
                    {
                        return 0;
                    }
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        texu_wnd_send_msg(activewnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);
                        return -1;
                    }
                    rc = texu_wnd_send_msg((nextwnd ? nextwnd : wnd), TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                    /* the new active window */
                    wnd->activechild = nextwnd;

                    /* send notification to the window*/
                    _TexuFrameWndProc_Notify(wnd, nextwnd, code);
                    return 1;
                }
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        } /* child window is active */
        else
        {
            /* the current frame window is active */
            if (activewnd)
            {
                activewnd = texu_wnd_get_activechild(activewnd);
                return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
            }
        }
    }
    else
    {
        if (activewnd)
        {
            return texu_wnd_send_msg(activewnd, TEXU_WM_KEYDOWN, (texu_i64)ch, alt);
        }
    }
    return 0;
}

void
_TexuManualDefWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
}

texu_i64
_TexuManualFrameWndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    switch (msg)
    {
        case TEXU_WM_PAINT:
            _TexuManualDefWndProc_OnPaint(wnd, (texu_cio*)param1);
            return 0;

        case TEXU_WM_KEYDOWN:
            return _TexuManualFrameWndProc_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

#ifdef __cplusplus
}
#endif
