#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#include "tcl.h"
#include "texu.h"
#include "texust.h"
#include "texuconst.h"
#include "auitclsh.h"

/*global variables*/
extern auitclsh_t g_auitclsh;



struct msg_map
{
    const char  *msgtext;
    texu_ui32   msgid;
};
struct msg_map a_msg_map[] = 
{
    { "TEXU_WM_COMMAND",        TEXU_WM_COMMAND },
    { "TEXU_BM_PUSH",           TEXU_BM_PUSH },
    { "TEXU_WM_CHAR",           TEXU_WM_CHAR },
    { "TEXU_WM_ENABLE",         TEXU_WM_ENABLE },
    { "TEXU_WM_SHOW",           TEXU_WM_SHOW },
};
#define MAX_MSG_MAP     (sizeof(a_msg_map) / sizeof(a_msg_map[0]))

/*this is a helper function to request and get response to/from TexU server*/
texu_i64    _auitclsh_request(int msgid, char* req, cJSON **res);
texu_i64    _auitclsh_request_json(int msgid, cJSON *req, cJSON **res);
texu_i64    _auitclsh_request_async(int msgid, char* req);
texu_i64    _auitclsh_request_json_async(int msgid, cJSON *req);
char        *_auitclsh_get_minify(cJSON *json);
texu_ui32   _auitclsh_getmsgid(const char *msgtext);

#include <ctype.h>
void        _auitclsh_toupper(char* upper, const char *any);
void        _auitclsh_tolower(char* lower, const char *any);

void
_auitclsh_toupper(char* upper, const char *any)
{
    char *psz = (char*)any;
    while (psz && *psz != 0)
    {
        *upper = toupper(*psz);
        ++psz;
        ++upper;
    }
    *upper = 0;
}

void
_auitclsh_tolower(char* lower, const char *any)
{
    char *psz = (char*)any;
    while (psz && *psz != 0)
    {
        *lower = tolower(*psz);
        ++psz;
        ++lower;
    }
    *lower = 0;
}

texu_ui32   _auitclsh_getmsgid(const char *msgtext)
{
    texu_i32 i = 0;
    char msg[32];
    
    _auitclsh_toupper(msg, msgtext);
    for (i = 0; i < MAX_MSG_MAP; ++i)
    {
        if (0 == strcmp(msg, a_msg_map[i].msgtext))
        {
            return a_msg_map[i].msgid;
        }
    }
    return 0;
}

char*
_auitclsh_get_minify(cJSON *root)
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


texu_i64
_auitclsh_request_json(int msgid, cJSON *req, cJSON **res)
{
    texu_i64 rc = -1;
    char *cmd = _auitclsh_get_minify(req);
    if (cmd)
    {
        rc = _auitclsh_request(msgid, cmd, res);
        free(cmd);
    }
    return rc;
}

texu_i64
_auitclsh_request_json_async(int msgid, cJSON *req)
{
    texu_i64 rc = -1;
    char *cmd = _auitclsh_get_minify(req);
    if (cmd)
    {
        rc = _auitclsh_request_async(msgid, cmd);
        free(cmd);
    }
    return rc;
}

/*DO NOT CALL DIRECTLY IF NOT USE cJSON object build json string*/
/*PLEASE SEE ALTERNATIVELY _auitclsh_request_json() */
texu_i64
_auitclsh_request(int msgid, char* req, cJSON **res) 
{
    texu_env_msg msg;
    struct timeval tv = { 0, 2000 };
    int rc = 0;
    texu_i64 rescode = 0;   /* 0 - should be ok*/
    
    memset(&msg, 0, sizeof(texu_env_msg));
    msg.type = TEXU_ENV_MSGTYPE_REQUEST; /*msg type = 1 (request)*/
    strcpy(msg.json, req);
    rc = msgsnd(msgid, &msg, sizeof(texu_env_msg), 0);

    rc = -1;
    while (rc < 0)
    {
        /*read response*/
        select(0, 0, 0, 0, &tv); /*wait for TexU server replied for a second*/

        memset(&msg, 0, sizeof(texu_env_msg));
        rc = msgrcv(msgid, &msg, sizeof(texu_env_msg),
                TEXU_ENV_MSGTYPE_RESPONSE,  /*msg type = 2 (response) */
                0); /*NOW, read replied msg from TexU server */
        if (rc > 0)
        {
            /* res={reply:{code:0,...}}*/
            cJSON *code = NULL;
            cJSON *json = cJSON_Parse(msg.json);
            do
            {
                if (!json)
                {
                    rescode = -1; /*no json format replied*/
                    break;
                }

                code = cJSON_GetObjectItem(json, "code");
                if (!code)
                {
                    rescode = -3; /*no code object returned*/
                    break;
                }

                rescode = (texu_i64)cJSON_GetNumberValue(code);
            } while (0); /*immediately break -- just only do 1 time*/
            /* set output if need*/
            if (*res)
            {
                *res = 0;
                if (0 == rescode)
                {
                    *res = json;
                }
            }
            else if (json)
            {
                cJSON_Delete(json);
            }
            rc = 1; /* break while loop*/
        }
        else
        {
            rc = -1;
        }
    } /*while (rc < 0)*/

    return rescode;
}

texu_i64
_auitclsh_request_async(int msgid, char* req) 
{
    texu_env_msg msg;
    struct timeval tv = { 0, 1500 };
    int rc = 0;
    texu_i64 rescode = 0;   /* 0 - should be ok*/
    
    memset(&msg, 0, sizeof(texu_env_msg));
    msg.type = TEXU_ENV_MSGTYPE_REQUEST; /*msg type = 1 (request)*/
    strcpy(msg.json, req);
    rc = msgsnd(msgid, &msg, sizeof(texu_env_msg), 0);
    select(0, 0, 0, 0, &tv);

    return rc;
}

int
aui_sendmsg(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    texu_i64 lmsg = 0;
    texu_i64 lp1 = 0;
    texu_i64 lp2 = 0;
    
    texu_env_msg msg;
    cJSON *req = NULL;
    cJSON *parms = NULL;
    cJSON *res;
    texu_i64 rescode = 0;
    char *wndmsg;
    char *parm;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd ?i_msg [?i_p1 [?i_p2]]", (char*)NULL );
        return TCL_ERROR;
    }
    
    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);

    wndmsg = Tcl_GetStringFromObj( objv[2], NULL );
    lmsg = atol(wndmsg);
    if (lmsg <= 0)
    {
        lmsg = _auitclsh_getmsgid(wndmsg);
    }
    
    if (objc > 3)
    {
        parm = Tcl_GetStringFromObj( objv[3], NULL );
        lp1 = atoll(parm);
    }
    else
    {
        lp1 = 0;
    }
    if (objc > 4)
    {
        parm = Tcl_GetStringFromObj( objv[4], NULL );
        lp2 = atoll(parm);
    }
    else
    {
        lp2 = 0;
    }

    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "sendmsg"); /* req = {cmd:'sendmsg',parms:{wnd:%ld,msg:%ld,param1:%ld,param2:%ld}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddNumberToObject(parms, "msg", lmsg);
    cJSON_AddNumberToObject(parms, "param1", lp1);
    cJSON_AddNumberToObject(parms, "param2", lp2);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json(auitclsh->msgid, req, &res); /*synchronous request and respond*/

    cJSON_Delete(res);
    cJSON_Delete(req);
    return (rescode != 0 ? TCL_ERROR : TCL_OK);
}


int
aui_postmsg(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    texu_i64 lmsg = 0;
    texu_i64 lp1 = 0;
    texu_i64 lp2 = 0;
    
    texu_env_msg msg;
    cJSON *req = NULL;
    cJSON *parms = NULL;
    texu_i64 rescode;
    char* wndmsg;
    char* parm;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd ?i_msg [?i_p1 [?i_p2]]", (char*)NULL );
        return TCL_ERROR;
    }
    
    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);

    wndmsg = Tcl_GetStringFromObj( objv[2], NULL );
    lmsg = atol(wndmsg);
    if (lmsg <= 0)
    {
        lmsg = _auitclsh_getmsgid(wndmsg);
    }
    
    if (objc > 3)
    {
        parm = Tcl_GetStringFromObj( objv[3], NULL );
        lp1 = atoll(parm);
    }
    else
    {
        lp1 = 0;
    }
    if (objc > 4)
    {
        parm = Tcl_GetStringFromObj( objv[4], NULL );
        lp2 = atoll(parm);
    }

    else
    {
        lp2 = 0;
    }
    

    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "postmsg"); /* req = {cmd:'postmsg',parms:{wnd:%ld,msg:%ld,param1:%ld,param2:%ld}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddNumberToObject(parms, "msg", lmsg);
    cJSON_AddNumberToObject(parms, "param1", lp1);
    cJSON_AddNumberToObject(parms, "param2", lp2);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json_async(auitclsh->msgid, req); /*asynchronous request*/
    
    cJSON_Delete(req);
    return TCL_OK;
}


int
aui_sendcmd(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    texu_i64 lmsg = 0;
    texu_i64 lp1 = 0;
    texu_i64 lp2 = 0;
    
    texu_env_msg msg;
    cJSON *req = NULL;
    cJSON *parms = NULL;
    texu_i64 rescode = 0;
    char *wndmsg;
    char *parm;
    
    if (objc < 1)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_cmdid", (char*)NULL );
        return TCL_ERROR;
    }
    
    lwnd = -1;
    lmsg = TEXU_WM_COMMAND;
    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lp1 = atoll(parm);
    lp2 = 0;

    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "sendcmd"); /* req = {cmd:'sendcmd',parms:{cmdid:%ld}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "cmdid", lp1);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json_async(auitclsh->msgid, req); /*asynchronous request*/
    
    cJSON_Delete(req);
    return (rescode != 0 ? TCL_ERROR : TCL_OK);
}

int
aui_settext(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    char *text;
    int len = 256;
    
    texu_env_msg msg;
    cJSON *parms = NULL;
    cJSON *req = NULL;
    texu_i64 rescode = 0;
    
    char *parm;
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd ?s_text", (char*)NULL );
        return TCL_ERROR;
    }
    
    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);

    if (objc > 2)
    {
        text = Tcl_GetStringFromObj(objv[2], NULL);
    }
    else
    {
        text = "";
    }
    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "settext"); /* req = {cmd:'settext',parms:{wnd:%ld,text:%s}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddStringToObject(parms, "text", text);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json_async(auitclsh->msgid, req); /*asynchronous request*/

    cJSON_Delete(req);
    return (TCL_OK);
}

int
aui_gettext(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    texu_char *text;
    long len = 256;
    
    texu_env_msg msg;
    cJSON *req = NULL;
    cJSON *parms = NULL;
    cJSON *res;
    texu_i64 rescode = 0;
    
    char *parm;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd", (char*)NULL );
        return TCL_ERROR;
    }
    
/*    Tcl_GetLongFromObj(interp, objv[1], &lwnd);*/
    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);
    
    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "gettext"); /* req = {cmd:'settext',parms:{wnd:%ld,text:%s}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json(auitclsh->msgid, req, &res); /*synchronous request and respond*/
    
    parms = cJSON_GetObjectItem(res, "text");
    if (parms)
    {
        text = cJSON_GetStringValue(parms);
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "", text, (char*)NULL );
    }
    else
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "", (char*)NULL );
    }

    cJSON_Delete(res);
    cJSON_Delete(req);
    return (rescode != 0 ? TCL_ERROR : TCL_OK);
}


int
aui_enable(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    cJSON *req = NULL;
    cJSON *parms = NULL;

    texu_i64 rescode = 0;
    
    char *parm;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd", (char*)NULL );
        return TCL_ERROR;
    }

    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);
    if (lwnd < 0)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "i_wnd MUST BE GREATER THAN ZERO", (char*)NULL );
        return TCL_ERROR;
    }
    
    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "enable"); /* req = {cmd:'enable',parms:{wnd:%ld}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json_async(auitclsh->msgid, req); /*asynchronous request*/

    cJSON_Delete(req);
    return (TCL_OK);
}


int
aui_disable(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    cJSON *req = NULL;
    cJSON *parms = NULL;
    texu_i64 rescode = 0;
    
    char *parm;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd", (char*)NULL );
        return TCL_ERROR;
    }

    parm = Tcl_GetStringFromObj( objv[1], NULL );
    lwnd = atoll(parm);
    if (lwnd < 0)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "i_wnd MUST BE GREATER THAN ZERO", (char*)NULL );
        return TCL_ERROR;
    }

    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "disable"); /* req = {cmd:'disable',parms:{wnd:%ld}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json_async(auitclsh->msgid, req); /*asynchronous request*/

    cJSON_Delete(req);
    return (TCL_OK);
}


int
aui_help(
    ClientData      cd,
    Tcl_Interp*     interp,
    int             objc,
    Tcl_Obj *CONST  objv[]
)
{
    char *gblcmd  = Tcl_GetStringFromObj( objv[0], NULL );
    auitclsh_t *auitclsh = (auitclsh_t*)cd;
    texu_i64 lwnd = 0;  /*control id of the current window*/
    cJSON *req = NULL;
    cJSON *parms = NULL;
    texu_i64 rescode = 0;
    
    char *parm;
    
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_sendmsg i_wnd i_msg [i_param1 [i_param2]]\n", 
        "PARAMETERS:\n",
        "\ti_wnd = control id, if i_wnd = -1, it means the frame window\n",
        "\ti_msg = msg id\n",
        "\ti_param1 = parameter 1 of msg id (optional)\n",
        "\ti_param2 = parameter 2 of msg id (optional)\n",
        "RETURN:\n",
        "\tNormally returned code successfully is 0, otherwise failed\n",
        "i_msg:\n",
        "\tTEXU_WM_COMMAND - to send the command to the frame window\n",
        "\t\te.g. aui_sendmsg -1 TEXU_WM_COMMAND 10001\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_postmsg i_wnd i_msg [i_param1 [i_param2]]\n", 
        "PARAMETERS:\n",
        "\tSee aui_sendmsg\n",
        "RETURN:\n",
        "\tNo return. This posts the message to the window.\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_sendcmd i_cmdid\n", 
        "PARAMETERS:\n",
        "\ti_cmdid = Command ID to post the frame window \n",
        "RETURN:\n",
        "\tNo return. This command is equivalent to [aui_sendmsg -1 TEXU_WM_COMMAND i_cmdid]\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_settext i_wnd text\n", 
        "PARAMETERS:\n",
        "\ti_wnd = control id, if i_wnd = -1, it means the frame window\n",
        "\ttext = text to set the window\n",
        "RETURN:\n",
        "\tNo return\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_gettext i_wnd\n", 
        "PARAMETERS:\n",
        "\ti_wnd = control id, if i_wnd = -1, it means the frame window\n",
        "RETURN:\n",
        "\tText to get from the window\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_enable i_wnd\n", 
        "PARAMETERS:\n",
        "\ti_wnd = control id, if i_wnd = -1, it means the frame window\n",
        "RETURN:\n",
        "\tNo return\n",
        (char*)NULL );
    Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
        "\naui_disable i_wnd\n", 
        "PARAMETERS:\n",
        "\ti_wnd = control id, if i_wnd = -1, it means the frame window\n",
        "RETURN:\n",
        "\tNo return\n",
        (char*)NULL );

    return TCL_OK;
}

int
auitclsh_init(
  Tcl_Interp* interp
)
{
    int rc = TCL_OK;

    /* TCL initial*/
    rc = Tcl_Init( interp );
    if ( TCL_ERROR == rc )
    {
        return rc;
    }
    /* set tcl_interactive, lets us run unix commands as from a shell*/
    Tcl_SetVar( interp, "tcl_interactive", "1", 0 );

    Tcl_CreateObjCommand( interp,
        "aui_sendmsg",
        aui_sendmsg,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_postmsg",
        aui_postmsg,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_sendcmd",
        aui_sendcmd,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_settext",
        aui_settext,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_gettext",
        aui_gettext,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_enable",
        aui_enable,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_disable",
        aui_disable,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );
    Tcl_CreateObjCommand( interp,
        "aui_help",
        aui_help,
        ( ClientData* )&g_auitclsh,
        ( Tcl_CmdDeleteProc* )NULL
        );

    return rc;
}
