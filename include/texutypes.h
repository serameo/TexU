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
typedef int                            texu_i32;
typedef unsigned int                   texu_ui32;
#ifdef _X64_
typedef long long                      texu_i64;
typedef unsigned long long             texu_ui64;
#else
typedef long                           texu_i64;
typedef unsigned long                  texu_ui64;
#endif
typedef float                          texu_f32;
typedef double                         texu_f64;

typedef char                           texu_char;
typedef unsigned char                  texu_byte;
typedef short                          texu_short;
typedef long                           texu_long;
typedef long long                      texu_longlong;
typedef long long                      texu_bool;
typedef float                          texu_float;
typedef double                         texu_double;
typedef char*                          texu_string;

typedef long                           texu_status;

#endif /*_TEXUTYPES_H_*/

