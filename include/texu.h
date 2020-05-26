/*
#
# File Name: texu.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXU_H_
#define _TEXU_H_

/*
# TexU version
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_MAJOR_VERSION             0x100
#define TEXU_MINOR_VERSION             0x0
#define TEXU_VERSION                   (TEXU_MAJOR_VERSION | TEXU_MINOR_VERSION)

/*
# TexU definitions and macros
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#include "texumacros.h"
/*
#define TEXU_FALSE                     0
#define TEXU_TRUE                      1
#define TEXU_IS_TRUE(e)                (0 != (e))
#define TEXU_IS_FALSE(e)               (0 == (e))
#define TEXT_IS_NULL(p)                (0 == (p))
#define TEXT_IS_NOT_NULL(p)            (!(p))
#define TEXU_MIN(a, b)                 ((a) < (b) ? (a) : (b))
#define TEXU_MAX(a, b)                 ((a) > (b) ? (a) : (b))
*/

/*
# TexU types and structures
#
*/
#include "texutypes.h"
/*
typedef char                           texu_i8;
typedef unsigned char                  texu_ui8;
typedef short                          texu_i16;
typedef unsigned short                 texu_ui16;
typedef long                           texu_i32;
typedef unsigned long                  texu_ui32;
typedef long long                      texu_i64;
typedef unsigned long long             texu_ui64;
typedef float                          texu_f4;
typedef double                         texu_f8;

typedef char                           texu_char;
typedef unsigned char                  texu_byte;
typedef short                          texu_short;
typedef long                           texu_long;
typedef long long                      texu_longlong;
typedef long long                      texu_bool;
typedef float                          texu_float;
typedef double                         texu_double;
typedef char*                          texu_string;
*/


/*
# TexU libraries
#
*/
#include "texulib.h"
#include "texui.h"
#include "texutils.h"
#include "texucio.h"

/*
# TexU interface functions
#
*/
texu_status        TexuStartup();
texu_status        TexuShutdown();
texu_status        TexuRun();

texu_env*          TexuGetEnv();
texu_cio*          TexuGetDC();
texu_i32           TexuGetMaxY();
texu_i32           TexuGetMaxX();

void               TexuExit();

texu_status        TexuRegisterClass(
                     texu_char*   clsname,
                     texu_wndproc wndproc
                   );
void               TexuPushWindow(
                     texu_wnd*  wnd
                   );
texu_wnd*          TexuPopWindow();
texu_wnd*          TexuTopWindow();
texu_wnd*          TexuCreateWindow(
                     texu_char* text,
                     texu_char* clsname,
                     texu_ui32  style,
                     texu_ui32  exstyle,
                     texu_i32   y,
                     texu_i32   x,
                     texu_i32   h,
                     texu_i32   w,
                     texu_wnd*  parent,
                     texu_i32   id,
                     void*      userdata
                   );
void               TexuDestroyWindow(
                     texu_wnd*  wnd
                   );

#define TEXU_WS_HIDE            0
#define TEXU_WS_SHOW            1
void               TexuShowWindow(
                     texu_wnd*  wnd,
                     texu_i32   show
                     );
void               TexuInvalidateWindow(
                     texu_wnd*  wnd
                     );

#endif /*_TEXU_H_*/

