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
    texu_xcnf   *xcnf;
};
typedef struct _auitclsh auitclsh_t;

/*initial Tcl command interfaces*/
int
auitclsh_init(
  Tcl_Interp* interp
);

/* Send message to the autoui app*/
int
aui_sendmsg(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);

/* Post message to the autoui app*/
int
aui_postmsg(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);

/* set text to a window object in the autoui app*/
int
aui_settext(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);

/* get text from a window object in the autoui app*/
int
aui_gettext(
  ClientData      cd,
  Tcl_Interp*     interp,
  int             objc,
  Tcl_Obj *CONST  objv[]
);

#endif /*_AUITCLSH_H_*/
