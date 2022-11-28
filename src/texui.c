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
#
*/

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
#define TEXU_ENV_SCRFILE    "./.texuenv"

#define TEXU_MAX_COLOR      (TEXU_COLOR_DEFAULT + 1)
#define TEXU_ENV_INITIAL    (250)
#define TEXU_ENV_REPEAT     (250)

    struct texu_msg
    {
        texu_wnd    *wnd;
        texu_ui32   msg;
        texu_i64    param1;
        texu_i64    param2;
    };

    struct texu_env
    {
        texu_stack  *frames;    /* hold window frames */
        texu_cio    *cio;       /* console input/output */
        texu_wnd    *desktop;   /* the root of all window frames */
        texu_list   *wndcls;    /* the registered classes */
        texu_bool   exit;       /* main loop*/
        texu_i32    ypos;       /* cursor position */
        texu_i32    xpos;       /* cursor position */
        FILE        *scrfp;     /* to write/read screen */
        /* system color */
        texu_i32 syscolors[TEXU_MAX_COLOR];
        texu_i32          cols; /* width*/
        texu_i32          lines;/* height*/
#ifdef USE_TCL_AUTOMATION
        key_t           msgkey; /* commu key */
        int             msgid;  /* commu id */
#endif
        texu_queue      *msgques;   /* hold the posted msg to be called after*/
    };

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
    /*texu_i64 _TexuTextCtrlProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);*/
    texu_i64 _TexuReBarProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

    /* menu texumenu.c */
    texu_i64 _TexuMenuProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);
    texu_i64 _TexuMenuWndProc(texu_wnd *, texu_ui32, texu_i64, texu_i64);

    void _texu_env_init_cls(texu_env *);
    texu_wndproc _texu_env_find_wndproc(texu_env *, const texu_char *);
    FILE *_texu_env_init_screen(texu_env *env);
    void _texu_env_init_syscolors(texu_env *env);

#if USE_TCL_AUTOMATION
    void    _texu_env_init_tcl_automation(texu_env *env, const char* pathname);
    texu_bool    _texu_env_read_tcl_input(texu_env *env, texu_env_msg *envmsg);
    void    _texu_env_release_tcl_automation(texu_env *env);
    void    _texu_env_reply_tcl_output(texu_env *env, texu_char* resjson);
    void    _texu_env_handle_request(texu_env *env, texu_char* reqjson);
    char*   _texu_env_get_minify(cJSON *root);
    void    _texu_env_cmd_attach(texu_env *env);
    void    _texu_env_cmd_detach(texu_env *env);
    void    _texu_env_cmd_sendmsg(texu_env *env, cJSON *req);
    void    _texu_env_cmd_postmsg(texu_env *env, cJSON *req);
    void    _texu_env_cmd_settext(texu_env *env, cJSON *req);
    void    _texu_env_cmd_gettext(texu_env *env, cJSON *req);
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
    char *resjson = NULL;
    cJSON *res = cJSON_CreateObject();
    texu_i64 lwnd = 0;
    texu_i32 lmsg = 0;
    texu_i64 lparam1 = 0;
    texu_i64 lparam2 = 0;
    texu_wnd *wnd = texu_env_top_wnd(env);
    texu_wnd *child = 0;
    texu_i64 rc = 0;

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
        rc = texu_wnd_send_msg(child, lmsg, lparam1, lparam2);

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
        texu_wnd_post_msg(child, lmsg, lparam1, lparam2);
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
        texu_wnd_set_text(child, text);
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

void
_texu_env_handle_request(texu_env *env, texu_char* reqjson)
{
    cJSON *req = cJSON_Parse(reqjson);
    cJSON *cmd = NULL;
    char *resjson = NULL;
    cJSON *res = NULL;
    
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
    if (0 == strcmp("sendmsg", cJSON_GetStringValue(cmd)))
    {
        _texu_env_cmd_sendmsg(env, req);
    }
    else if (0 == strcmp("postmsg", cJSON_GetStringValue(cmd)))
    {
        _texu_env_cmd_postmsg(env, req);
    }
    else if (0 == strcmp("settext", cJSON_GetStringValue(cmd)))
    {
        _texu_env_cmd_settext(env, req);
    }
    else if (0 == strcmp("gettext", cJSON_GetStringValue(cmd)))
    {
        _texu_env_cmd_gettext(env, req);
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
        texu_env_msg envmsg;
        struct timeval tv = { 0, 1500 };
        memset(&envmsg, 0, sizeof(texu_env_msg));
        envmsg.type = TEXU_ENV_MSGTYPE_RESPONSE; /* msg type = 2 (response)*/
        strcpy(envmsg.json, json);
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

    void
    _texu_env_init_syscolors(texu_env *env)
    {
        texu_i32 i = 0;
        /* init all system colors */
        for (i = 0; i < TEXU_MAX_COLOR; ++i)
        {
            env->syscolors[i] = TEXU_CIO_COLOR_WHITE_BLACK;
        }
        env->syscolors[TEXU_COLOR_WINDOW]               = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_DESKTOP]              = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_DIALOG]               = TEXU_CIO_COLOR_BLACK_WHITE;
        env->syscolors[TEXU_COLOR_TITLE_WINDOW]         = TEXU_CIO_COLOR_WHITE_BLUE;
        env->syscolors[TEXU_COLOR_BUTTON_OK]            = TEXU_CIO_COLOR_BLACK_GREEN;
        env->syscolors[TEXU_COLOR_BUTTON_YES]           = TEXU_CIO_COLOR_WHITE_BLUE;
        env->syscolors[TEXU_COLOR_BUTTON_NO]            = TEXU_CIO_COLOR_BLACK_RED;
        env->syscolors[TEXU_COLOR_BUTTON_CANCEL]        = TEXU_CIO_COLOR_BLACK_YELLOW;
        env->syscolors[TEXU_COLOR_MENU]                 = TEXU_CIO_COLOR_BLUE_WHITE;
        env->syscolors[TEXU_COLOR_MENU_DISABLED]        = TEXU_CIO_COLOR_BLACK_WHITE;
        env->syscolors[TEXU_COLOR_MENU_SELECTED]        = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_MENUITEM]             = TEXU_CIO_COLOR_BLUE_WHITE;
        env->syscolors[TEXU_COLOR_MENUITEM_DISABLED]    = TEXU_CIO_COLOR_BLACK_WHITE;
        env->syscolors[TEXU_COLOR_MENUITEM_SELECTED]    = TEXU_CIO_COLOR_WHITE_BLUE;
        env->syscolors[TEXU_COLOR_LABEL]                = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LABEL_DISABLED]       = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_STATUSBAR]            = TEXU_CIO_COLOR_WHITE_BLUE;
        env->syscolors[TEXU_COLOR_EDIT]                 = TEXU_CIO_COLOR_CYAN_BLACK;
        env->syscolors[TEXU_COLOR_EDIT_DISABLED]        = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_EDIT_SELECTED]        = TEXU_CIO_COLOR_BLACK_CYAN;
        env->syscolors[TEXU_COLOR_LISTBOX]              = TEXU_CIO_COLOR_CYAN_BLACK;
        env->syscolors[TEXU_COLOR_LISTBOX_DISABLED]     = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTBOX_SELECTED]     = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_COMBOBOX]             = TEXU_CIO_COLOR_BLUE_WHITE;
        env->syscolors[TEXU_COLOR_COMBOBOX_DISABLED]    = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_COMBOBOX_SELECTED]    = TEXU_CIO_COLOR_WHITE_BLUE;
        env->syscolors[TEXU_COLOR_LISTCTRL]             = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTCTRL_DISABLED]    = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTCTRL_SELECTED]    = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTCTRL_ITEM]        = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTCTRL_ITEM_DISABLED] = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_LISTCTRL_ITEM_SELECTED] = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_TREECTRL]             = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_TREECTRL_DISABLED]    = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_TREECTRL_SELECTED]    = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_TREECTRL_ITEM]        = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_TREECTRL_ITEM_DISABLED] = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_TREECTRL_ITEM_SELECTED] = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_UPDOWNCTRL]           = TEXU_CIO_COLOR_BLUE_WHITE;
        env->syscolors[TEXU_COLOR_UPDOWNCTRL_DISABLED]  = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_UPDOWNCTRL_SELECTED]  = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_PROGRESSBAR]          = TEXU_CIO_COLOR_CYAN_BLACK;
        env->syscolors[TEXU_COLOR_PROGRESSBAR_DISABLED] = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_PROGRESSBAR_SELECTED] = TEXU_CIO_COLOR_BLUE_CYAN;
        env->syscolors[TEXU_COLOR_REBAR]                = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_REBAR_DISABLED]       = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_REBAR_SELECTED]       = TEXU_CIO_COLOR_WHITE_BLACK;
        env->syscolors[TEXU_COLOR_BUTTON]               = TEXU_CIO_COLOR_BLACK_CYAN;
        env->syscolors[TEXU_COLOR_BUTTON_DISABLED]      = TEXU_CIO_COLOR_BLACK_WHITE;
        /*default*/
        env->syscolors[TEXU_COLOR_DEFAULT]              = TEXU_CIO_COLOR_WHITE_BLACK;
    }

    void
    _texu_env_init_cls(texu_env *env)
    {
        texu_env_register_cls(env, TEXU_DESKTOP_CLASS,  _TexuDesktopProc);
        texu_env_register_cls(env, TEXU_MSGBOX_CLASS,   _TexuMsgBoxProc);
        /*controls*/
        texu_env_register_cls(env, TEXU_LABEL_CLASS,    _TexuLabelProc);
        texu_env_register_cls(env, TEXU_BUTTON_CLASS,   _TexuButtonProc);
        texu_env_register_cls(env, TEXU_EDIT_CLASS,     _TexuEditProc);
        texu_env_register_cls(env, TEXU_LISTBOX_CLASS,  _TexuListBoxProc);
        texu_env_register_cls(env, TEXU_COMBOBOX_CLASS, _TexuComboBoxProc);
        texu_env_register_cls(env, TEXU_LISTCTRL_CLASS, _TexuListCtrlProc);
        texu_env_register_cls(env, TEXU_TREECTRL_CLASS, _TexuTreeCtrlProc);
        texu_env_register_cls(env, TEXU_UPDOWNCTRL_CLASS,   _TexuUpDownCtrlProc);
        texu_env_register_cls(env, TEXU_PROGRESSBAR_CLASS,  _TexuProgressBarProc);
        texu_env_register_cls(env, TEXU_STATUSBAR_CLASS,    _TexuStatusBarProc);
        texu_env_register_cls(env, TEXU_PAGECTRL_CLASS,     _TexuPageCtrlProc);
        texu_env_register_cls(env, TEXU_REBAR_CLASS,        _TexuReBarProc);
        /*menu*/
        texu_env_register_cls(env, TEXU_MENU_CLASS,     _TexuMenuProc);
        texu_env_register_cls(env, TEXU_MENUWND_CLASS,  _TexuMenuWndProc);
    }

    FILE *
    _texu_env_init_screen(texu_env *env)
    {
        FILE *fp = fopen(TEXU_ENV_SCRFILE, "w");
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
            if (0 == strcmp(wndcls->clsname, clsname))
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
        attrs.height = env->lines;
        attrs.width = env->cols;
        attrs.enable = TEXU_TRUE;
        attrs.visible = TEXU_TRUE;
        attrs.text = 0;
        attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
        attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
        attrs.focuscolor = texu_env_get_syscolor(env, TEXU_COLOR_DESKTOP);
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

    texu_i32
    texu_env_get_syscolor(texu_env *env, texu_i32 syscolor)
    {
        if (syscolor < 0 || syscolor >= TEXU_MAX_COLOR)
        {
            syscolor = TEXU_COLOR_DEFAULT;
        }
        return env->syscolors[syscolor];
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
    texu_env_new(texu_i32 lines, texu_i32 cols)
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
            texu_cio_init(env->cio, lines, cols);

            env->scrfp = _texu_env_init_screen(env);
            _texu_env_init_syscolors(env);

            /* window classes */
            env->wndcls = texu_list_new();

            /* register internal classes */
            _texu_env_init_cls(env);

            env->cols = cols;
            env->lines = lines;

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
#endif /*USE_TCL_AUTOMATION*/
            fclose(env->scrfp);
            texu_list_del(env->wndcls);
            texu_stack_del(env->frames);
            texu_wnd_del(env->desktop);
            texu_cio_release(env->cio);
            
            texu_queue_del(env->msgques);
            
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

    texu_status
    texu_env_run(texu_env *env)
    {
#if USE_TCL_AUTOMATION
        texu_env_msg envmsg;
        texu_bool rb = TEXU_TRUE;
#else
        texu_i32 ch = 0;
        texu_i32 ch2 = 0;
        texu_i32 altpressed = 0;
        texu_i32 ctlpressed = 0;
        texu_char *keypressed = 0;
#endif
        texu_wnd *activewnd = 0;

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
        texu_i32 focuscolor;
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
    texu_i32    _TexuDefWndProc_OnChar(texu_wnd *, texu_i32 ch, texu_i32 alt);
    texu_i32    _TexuDefWndProc_OnKillFocus(texu_wnd *, texu_wnd *nextwnd);
    void        _TexuDefWndProc_OnSetFocus(texu_wnd *, texu_wnd *prevwnd);
    void        _TexuDefWndProc_OnSetText(texu_wnd *wnd, const texu_char *text);
    texu_i32    _TexuDefWndProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen);
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
        attrs.text = "";
        attrs.normalcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU);
        attrs.disabledcolor = texu_env_get_syscolor(env, TEXU_COLOR_MENU_DISABLED);
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
        wnd->width = rect->cols;
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

        texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)wnd->env->cio, 0);
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
    _TexuDefWndProc_OnPaint(texu_wnd *wnd, texu_cio *cio)
    {
    }

    void
    _TexuDefWndProc_OnEraseBg(texu_wnd *wnd, texu_cio *cio)
    {
        /*texu_env *env = texu_wnd_get_env(wnd);*/
        texu_char zblank[TEXU_MAX_WNDTEXT + 1];
        texu_i32 width = 0;
        texu_i32 height = 0;
        texu_i32 line = 0;
        texu_i32 maxwidth = COLS;

        width = (wnd->width > maxwidth ? maxwidth : wnd->width);
        height = wnd->height;

        memset(zblank, 0, sizeof(zblank));
        memset(zblank, ' ', width);

        for (line = 0; line < height; ++line)
        {
            texu_cio_putstr_attr(
                cio,
                line + wnd->y, wnd->x,
                zblank,
                texu_cio_get_color(cio, wnd->normalcolor));
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
        texu_ui32 textlen = strlen(text);
        texu_wnd *frame = texu_wnd_get_frame(wnd);
        texu_wnd *activewnd = texu_wnd_get_activechild(frame);
        texu_i32 y = 0;
        texu_i32 x = 0;
        texu_i32 width = 0;
        texu_cio *cio = 0;
        /*
        if (!texu_wnd_is_enable(wnd))
        {
            return;
        }
*/
        if (text && textlen > 0)
        {
            strcpy(wnd->text, text);
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

    texu_i32
    _TexuDefWndProc_OnGetText(texu_wnd *wnd, texu_char *text, texu_i32 textlen)
    {
        texu_ui32 len = strlen(wnd->text);

        if (textlen < 0 || 0 == text)
        {
            return len;
        }

        memset(text, 0, textlen);
        textlen = TEXU_MIN(len, textlen);
        memcpy(text, wnd->text, textlen);

        return textlen;
    }

    texu_i32
    _TexuDefWndProc_OnKillFocus(texu_wnd *wnd, texu_wnd *nextwnd)
    {
        texu_wnd *parent = texu_wnd_get_parent(wnd);
        if (parent && nextwnd && texu_wnd_is_active(nextwnd))
        {
            parent->activechild = nextwnd;
        }
        return TEXU_OK;
    }

    void
    _TexuDefWndProc_OnSetFocus(texu_wnd *wnd, texu_wnd *prevwnd)
    {
        texu_wnd *parent = texu_wnd_get_parent(wnd);
        if (parent && texu_wnd_is_active(wnd))
        {
            parent->activechild = wnd;
        }
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
                if (ch == TEXU_KEY_NEXTWND)
                {
                    nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
                }
                else if (ch == TEXU_KEY_PREVWND)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
                }

                /* kill and set the new active window */
                if (nextwnd)
                {
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
                        return -1;
                    }
                    rc = texu_wnd_send_msg(nextwnd, TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                    /* the new active window */
                    wnd->activechild = nextwnd;
                    return 1;
                }
                else
                {
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                }
            } /* child window is active */
            else
            {
                /* the current frame window is active */
                if (activewnd)
                {
                    activewnd = texu_wnd_get_activechild(activewnd);
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                }
            }
        }
        else
        {
            if (activewnd)
            {
                return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
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
        texu_wnd_destroy(wnd);
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

            case TEXU_WM_CHAR:
                return _TexuDefWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);

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
                return _TexuDefWndProc_OnGetText(wnd, (texu_char *)param1, (texu_i32)param2);

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

        /* find the window procedure */
        wndproc = _texu_env_find_wndproc(wnd->env, attrs->clsname);
        if (!wndproc)
        {
            return TEXU_NOTREGISTERED_CLASS;
        }
        wnd->y  = attrs->y;
        wnd->x  = attrs->x;
        wnd->height     = attrs->height;
        wnd->width      = attrs->width;
        wnd->style      = attrs->style;
        wnd->exstyle    = attrs->exstyle;
        wnd->enable     = attrs->enable;
        wnd->visible    = attrs->visible;
        if (attrs->text && strlen(attrs->text))
        {
            strcpy(wnd->text, attrs->text);
        }
        wnd->normalcolor    = attrs->normalcolor;
        wnd->disabledcolor  = attrs->disabledcolor;
        wnd->focuscolor     = attrs->focuscolor;
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
        return rc;
    }

    void
    texu_wnd_del(texu_wnd *wnd)
    {
        if (wnd)
        {
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
    texu_wnd_invalidate(texu_wnd *wnd)
    {
        texu_wnd *childwnd = 0;

        if (!wnd || !texu_wnd_is_visible(wnd))
        {
            return 0;
        }
        /* firstly, paint itself */
        texu_wnd_send_msg(wnd, TEXU_WM_ERASEBG, (texu_i64)(wnd->env->cio), 0);
        texu_wnd_send_msg(wnd, TEXU_WM_PAINT, (texu_i64)(wnd->env->cio), 0);

        childwnd = texu_wnd_firstchild(wnd);
        if (childwnd)
        {
            texu_wnd_invalidate(childwnd);
            childwnd = texu_wnd_nextwnd(childwnd);
            while (childwnd)
            {
                texu_wnd_invalidate(childwnd);
                childwnd = texu_wnd_nextwnd(childwnd);
            }
        }

        texu_cio_refresh(wnd->env->cio);
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
    texu_wnd_set_color(texu_wnd *wnd, texu_i32 normalcolor, texu_i32 disabledcolor)
    {
        if (wnd)
        {
            wnd->normalcolor = normalcolor;
            wnd->disabledcolor = disabledcolor;
        }
    }

    texu_status
    texu_wnd_get_color(texu_wnd *wnd, texu_i32 *normalcolor, texu_i32 *disabledcolor)
    {
        if (wnd)
        {
            *normalcolor = wnd->normalcolor;
            *disabledcolor = wnd->disabledcolor;
        }
        return TEXU_OK;
    }

    void
    texu_wnd_set_focuscolor(texu_wnd *wnd, texu_i32 color)
    {
        if (wnd)
        {
            wnd->focuscolor = color;
        }
    }

    texu_i32
    texu_wnd_get_focuscolor(texu_wnd *wnd)
    {
        return (wnd ? wnd->focuscolor : 0);
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
        return (wnd ? wnd->lockedupdate : 0);
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

    texu_i32    _TexuFrameWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt);
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
    _TexuFrameWndProc_OnChar(texu_wnd *wnd, texu_i32 ch, texu_i32 alt)
    {
        texu_i64 rc = 0;
        texu_wnd *activewnd = texu_wnd_get_activechild(wnd);
        texu_wnd *nextwnd = 0;
        texu_wnd *parent = texu_wnd_get_parent(wnd);
        texu_wnd *desktop = (wnd ? texu_env_get_desktop(wnd->env) : 0);
        texu_wnd_keycmd *keycmd = 0;
        texu_menu *menu = 0;

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
                if (ch == TEXU_KEY_NEXTWND)
                {
                    nextwnd = texu_wnd_get_next_activechild(wnd, activewnd);
                }
                else if (ch == TEXU_KEY_PREVWND)
                {
                    nextwnd = texu_wnd_get_prev_activechild(wnd, activewnd);
                }

                /* kill and set the new active window */
                if (nextwnd)
                {
                    rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                    if (rc != TEXU_OK)
                    {
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
                    if ((ch == TEXU_KEY_NEXTWND) || (ch == TEXU_KEY_PREVWND))
                    {
                        texu_wnd *lastwnd = texu_wnd_get_last_activechild(wnd);
                        texu_wnd *firstwnd = texu_wnd_get_first_activechild(wnd);
                        texu_ui32 code = 0;
                        
                        if ((activewnd == lastwnd) && (ch == TEXU_KEY_NEXTWND))
                        {
                            /*move the next active window to the first active child*/
                            nextwnd = firstwnd;
                            code = TEXU_WN_GOFIRSTCHILD;
                        }
                        else if ((activewnd == firstwnd) && (ch == TEXU_KEY_PREVWND))
                        {
                            /*move the next active window to the last active child*/
                            nextwnd = lastwnd;
                            code = TEXU_WN_GOLASTCHILD;
                        }
                        rc = texu_wnd_send_msg(activewnd, TEXU_WM_KILLFOCUS, (texu_i64)nextwnd, 0);
                        if (rc != TEXU_OK)
                        {
                            return -1;
                        }
                        rc = texu_wnd_send_msg((nextwnd ? nextwnd : wnd), TEXU_WM_SETFOCUS, (texu_i64)activewnd, 0);

                        /* the new active window */
                        wnd->activechild = nextwnd;
                        
                        /* send notification to the window*/
                        _TexuFrameWndProc_Notify(wnd, nextwnd, code);
                        return 1;
                    }
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                }
            } /* child window is active */
            else
            {
                /* the current frame window is active */
                if (activewnd)
                {
                    activewnd = texu_wnd_get_activechild(activewnd);
                    return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
                }
            }
        }
        else
        {
            if (activewnd)
            {
                return texu_wnd_send_msg(activewnd, TEXU_WM_CHAR, (texu_i64)ch, alt);
            }
        }
        return 0;
    }

    texu_i64
    TexuFrameWndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
    {
        switch (msg)
        {
            case TEXU_WM_CHAR:
                return _TexuFrameWndProc_OnChar(wnd, (texu_i32)param1, (texu_i32)param2);
        }
        return TexuDefWndProc(wnd, msg, param1, param2);
    }

#ifdef __cplusplus
}
#endif
