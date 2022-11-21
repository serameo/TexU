#ifndef _AUITCLSH_H_
#define _AUITCLSH_H_

#include <sys/ipc.h>
#include <sys/msg.h>

#include "tcl.h"
#include "texu.h"
#include "cJSON.h"

struct _auitclsh
{
    int         msgid;
    key_t       msgkey;
    texu_bool   attached;
};
typedef struct _auitclsh auitclsh_t;

int
atr_sendmsg(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);
int
atr_settext(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);
int
atr_gettext(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);

#endif /*_AUITCLSH_H_*/
