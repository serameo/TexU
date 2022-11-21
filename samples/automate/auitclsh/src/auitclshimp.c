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
#include "auitclsh.h"

/*global variables*/
auitclsh_t g_auitclsh;

/*this is a helper function to request and get response to/from TexU server*/
texu_i64    _auitclsh_request(int msgid, char* req, cJSON **res);
texu_i64    _auitclsh_request_json(int msgid, cJSON *req, cJSON **res);
char        *_auitclsh_get_minify(cJSON *json);

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

/*DO NOT CALL DIRECTLY IF NOT USE cJSON object build json string*/
/*PLEASE SEE ALTERNATIVELY _auitclsh_request_json() */
texu_i64
_auitclsh_request(int msgid, char* req, cJSON **res) 
{
    texu_env_msg msg;
    struct timeval tv = { 0, 5000 };
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
    
    if (objc < 4)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd ?i_msg ?i_p1 ?i_p2", (char*)NULL );
        return TCL_ERROR;
    }
    
    Tcl_GetLongFromObj(interp, objv[1], &lwnd);
    Tcl_GetLongFromObj(interp, objv[2], &lmsg);
    Tcl_GetLongFromObj(interp, objv[3], &lp1);
    Tcl_GetLongFromObj(interp, objv[4], &lp2);

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
    if (rescode != 0)
    {
    }
    
    cJSON_Delete(res);
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
    texu_char *text;
    int len = 256;
    
    texu_env_msg msg;
    cJSON *req = NULL;
    cJSON *parms = NULL;
    cJSON *res;
    texu_i64 rescode = 0;
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd ?s_text", (char*)NULL );
        return TCL_ERROR;
    }
    
    Tcl_GetLongFromObj(interp, objv[1], &lwnd);

    text = Tcl_GetStringFromObj(objv[2], &len);
    
    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "settext"); /* req = {cmd:'settext',parms:{wnd:%ld,text:%s}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddStringToObject(parms, "text", text);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json(auitclsh->msgid, req, &res); /*synchronous request and respond*/
    if (rescode != 0)
    {
    }
    
    cJSON_Delete(res);
    cJSON_Delete(req);
    return (rescode != 0 ? TCL_ERROR : TCL_OK);
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
    
    if (objc < 2)
    {
        Tcl_AppendStringsToObj( Tcl_GetObjResult( interp ),
            "shell> ", gblcmd, " ?i_wnd", (char*)NULL );
        return TCL_ERROR;
    }
    
    Tcl_GetLongFromObj(interp, objv[1], &lwnd);
    
    /*first write: to get texu_env object */
    req = cJSON_CreateObject();
    cJSON_AddStringToObject(req, "cmd", "gettext"); /* req = {cmd:'settext',parms:{wnd:%ld,text:%s}} */
    parms= cJSON_CreateObject();
    cJSON_AddNumberToObject(parms, "wnd", lwnd);
    cJSON_AddItemToObject(req, "parms", parms);

    rescode = _auitclsh_request_json(auitclsh->msgid, req, &res); /*synchronous request and respond*/
    if (rescode != 0)
    {
    }
    
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

    g_auitclsh.msgkey = ftok("autoui", 65); /*the name is the same as TexU server*/
    g_auitclsh.msgid  = msgget(g_auitclsh.msgkey, 0666 | IPC_CREAT);

    Tcl_CreateObjCommand( interp,
        "aui_sendmsg",
        aui_sendmsg,
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

    return rc;
}
