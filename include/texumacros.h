/*
#
# File Name: texumacros.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXUMACROS_H_
#define _TEXUMACROS_H_

/*
# TexU definitions and macros
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
#define TEXU_IS_TRUE(e)                (0 != (e))
#define TEXU_IS_FALSE(e)               (0 == (e))
#define TEXU_IS_NULL(p)                (0 == (p))
#define TEXU_IS_NOT_NULL(p)            (!(p))
#define TEXU_MIN(a, b)                 ((a) < (b) ? (a) : (b))
#define TEXU_MAX(a, b)                 ((a) > (b) ? (a) : (b))


#endif /*_TEXUMACROS_H_*/

