/*
#
# File Name: texutypes.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXUTYPES_H_
#define _TEXUTYPES_H_

/*
# TexU types and structures
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
typedef char                           texu_i8;
typedef unsigned char                  texu_ui8;
typedef short                          texu_i16;
typedef unsigned short                 texu_ui16;
#ifdef __LINUX64__
typedef int                            texu_i32;
typedef unsigned int                   texu_ui32;
typedef long                           texu_i64;
typedef unsigned long                  texu_ui64;
#else
typedef long                           texu_i32;
typedef unsigned long                  texu_ui32;
typedef long long                      texu_i64;
typedef unsigned long long             texu_ui64;
#endif

typedef float                          texu_f32;
typedef double                         texu_f64;

typedef unsigned char                  texu_byte;
typedef short                          texu_short;
typedef long                           texu_long;
typedef long long                      texu_longlong;
typedef long                           texu_bool;
typedef float                          texu_float;
typedef double                         texu_double;

typedef int                            texu_status;
typedef char                           texu_wchar;
typedef char*                          texu_wstring;
typedef char                           texu_achar;
typedef char*                          texu_astring;

#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
typedef texu_wchar                      texu_char;
typedef texu_wstring                    texu_string;
#else
typedef texu_achar                      texu_char;
typedef texu_astring                    texu_string;
#endif
#ifdef __LINUX64__
typedef texu_i64                        texu_lparam;
#else
typedef texu_i32                        texu_lparam;
#endif
typedef texu_lparam                     texu_longptr;
typedef texu_ui32                       texu_color;

#endif /*_TEXUTYPES_H_*/

