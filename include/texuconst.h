/*
#
# File Name: texuconst.h
# Author: Seree Rakwong
# Date: 06-MAY-2020
#
*/

#ifndef _TEXUCONST_H_
#define _TEXUCONST_H_
#ifdef _USE_CURSES_
#include <ncurses.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __USE_TERMIOS__
#include "termbox.h"
#elif defined __USE_TERMBOX2__
#include "termbox2.h"
#endif

#if (defined WIN32 && defined _WINDOWS)
enum
{
    TEXU_CIO_COLOR_BLACK_BLACK      = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_RED        = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_GREEN      = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_YELLOW     = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_BLUE       = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_MAGENTA    = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_CYAN       = RGB(0, 0, 0),
    TEXU_CIO_COLOR_BLACK_WHITE      = RGB(0, 0, 0),
    TEXU_CIO_COLOR_RED_BLACK        = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_RED          = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_GREEN        = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_YELLOW       = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_BLUE         = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_MAGENTA      = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_CYAN         = RGB(255, 0, 0),
    TEXU_CIO_COLOR_RED_WHITE        = RGB(255, 0, 0),
    TEXU_CIO_COLOR_GREEN_BLACK      = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_RED        = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_GREEN      = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_YELLOW     = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_BLUE       = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_MAGENTA    = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_CYAN       = RGB(0, 255, 0),
    TEXU_CIO_COLOR_GREEN_WHITE      = RGB(0, 255, 0),
    TEXU_CIO_COLOR_YELLOW_BLACK     = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_RED       = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_GREEN     = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_YELLOW    = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_BLUE      = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_MAGENTA   = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_CYAN      = RGB(255, 255, 0),
    TEXU_CIO_COLOR_YELLOW_WHITE     = RGB(255, 255, 0),
    TEXU_CIO_COLOR_BLUE_BLACK       = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_RED         = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_GREEN       = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_YELLOW      = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_BLUE        = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_MAGENTA     = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_CYAN        = RGB(0, 0, 255),
    TEXU_CIO_COLOR_BLUE_WHITE       = RGB(0, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_BLACK    = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_RED      = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_GREEN    = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_YELLOW   = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_BLUE     = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_MAGENTA  = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_CYAN     = RGB(255, 0, 255),
    TEXU_CIO_COLOR_MAGENTA_WHITE    = RGB(255, 0, 255),
    TEXU_CIO_COLOR_CYAN_BLACK       = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_RED         = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_GREEN       = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_YELLOW      = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_BLUE        = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_MAGENTA     = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_CYAN        = RGB(0, 255, 255),
    TEXU_CIO_COLOR_CYAN_WHITE       = RGB(0, 255, 255),
    TEXU_CIO_COLOR_WHITE_BLACK      = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_RED        = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_GREEN      = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_YELLOW     = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_BLUE       = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_MAGENTA    = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_CYAN       = RGB(192, 192, 192),
    TEXU_CIO_COLOR_WHITE_WHITE      = RGB(192, 192, 192),
    TEXU_CIO_BRIGHT_WHITE_BLACK     = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_RED       = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_GREEN     = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_YELLOW    = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_BLUE      = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_MAGENTA   = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_CYAN      = RGB(255, 255, 255),
    TEXU_CIO_BRIGHT_WHITE_WHITE     = RGB(255, 255, 255),
    TEXU_CIO_BLACK_BRIGHT_WHITE     = RGB(0, 0, 0),
    TEXU_CIO_RED_BRIGHT_WHITE       = RGB(255, 0, 0),
    TEXU_CIO_GREEN_BRIGHT_WHITE     = RGB(0, 255, 0),
    TEXU_CIO_YELLOW_BRIGHT_WHITE    = RGB(255, 255, 0),
    TEXU_CIO_BLUE_BRIGHT_WHITE      = RGB(0, 0, 255),
    TEXU_CIO_MAGENTA_BRIGHT_WHITE   = RGB(255, 0, 255),
    TEXU_CIO_CYAN_BRIGHT_WHITE      = RGB(0, 255, 255),
    TEXU_CIO_LAST_COLOR = 0
};
#else
enum
{
    TEXU_CIO_COLOR_BLACK_BLACK,
    TEXU_CIO_COLOR_BLACK_RED,
    TEXU_CIO_COLOR_BLACK_GREEN,
    TEXU_CIO_COLOR_BLACK_YELLOW,
    TEXU_CIO_COLOR_BLACK_BLUE,
    TEXU_CIO_COLOR_BLACK_MAGENTA,
    TEXU_CIO_COLOR_BLACK_CYAN,
    TEXU_CIO_COLOR_BLACK_WHITE,
    TEXU_CIO_COLOR_RED_BLACK,
    TEXU_CIO_COLOR_RED_RED,
    TEXU_CIO_COLOR_RED_GREEN,
    TEXU_CIO_COLOR_RED_YELLOW,
    TEXU_CIO_COLOR_RED_BLUE,
    TEXU_CIO_COLOR_RED_MAGENTA,
    TEXU_CIO_COLOR_RED_CYAN,
    TEXU_CIO_COLOR_RED_WHITE,
    TEXU_CIO_COLOR_GREEN_BLACK,
    TEXU_CIO_COLOR_GREEN_RED,
    TEXU_CIO_COLOR_GREEN_GREEN,
    TEXU_CIO_COLOR_GREEN_YELLOW,
    TEXU_CIO_COLOR_GREEN_BLUE,
    TEXU_CIO_COLOR_GREEN_MAGENTA,
    TEXU_CIO_COLOR_GREEN_CYAN,
    TEXU_CIO_COLOR_GREEN_WHITE,
    TEXU_CIO_COLOR_YELLOW_BLACK,
    TEXU_CIO_COLOR_YELLOW_RED,
    TEXU_CIO_COLOR_YELLOW_GREEN,
    TEXU_CIO_COLOR_YELLOW_YELLOW,
    TEXU_CIO_COLOR_YELLOW_BLUE,
    TEXU_CIO_COLOR_YELLOW_MAGENTA,
    TEXU_CIO_COLOR_YELLOW_CYAN,
    TEXU_CIO_COLOR_YELLOW_WHITE,
    TEXU_CIO_COLOR_BLUE_BLACK,
    TEXU_CIO_COLOR_BLUE_RED,
    TEXU_CIO_COLOR_BLUE_GREEN,
    TEXU_CIO_COLOR_BLUE_YELLOW,
    TEXU_CIO_COLOR_BLUE_BLUE,
    TEXU_CIO_COLOR_BLUE_MAGENTA,
    TEXU_CIO_COLOR_BLUE_CYAN,
    TEXU_CIO_COLOR_BLUE_WHITE,
    TEXU_CIO_COLOR_MAGENTA_BLACK,
    TEXU_CIO_COLOR_MAGENTA_RED,
    TEXU_CIO_COLOR_MAGENTA_GREEN,
    TEXU_CIO_COLOR_MAGENTA_YELLOW,
    TEXU_CIO_COLOR_MAGENTA_BLUE,
    TEXU_CIO_COLOR_MAGENTA_MAGENTA,
    TEXU_CIO_COLOR_MAGENTA_CYAN,
    TEXU_CIO_COLOR_MAGENTA_WHITE,
    TEXU_CIO_COLOR_CYAN_BLACK,
    TEXU_CIO_COLOR_CYAN_RED,
    TEXU_CIO_COLOR_CYAN_GREEN,
    TEXU_CIO_COLOR_CYAN_YELLOW,
    TEXU_CIO_COLOR_CYAN_BLUE,
    TEXU_CIO_COLOR_CYAN_MAGENTA,
    TEXU_CIO_COLOR_CYAN_CYAN,
    TEXU_CIO_COLOR_CYAN_WHITE,
    TEXU_CIO_COLOR_WHITE_BLACK,
    TEXU_CIO_COLOR_WHITE_RED,
    TEXU_CIO_COLOR_WHITE_GREEN,
    TEXU_CIO_COLOR_WHITE_YELLOW,
    TEXU_CIO_COLOR_WHITE_BLUE,
    TEXU_CIO_COLOR_WHITE_MAGENTA,
    TEXU_CIO_COLOR_WHITE_CYAN,
    TEXU_CIO_COLOR_WHITE_WHITE,
    TEXU_CIO_BRIGHT_WHITE_BLACK,
    TEXU_CIO_BRIGHT_WHITE_RED,
    TEXU_CIO_BRIGHT_WHITE_GREEN,
    TEXU_CIO_BRIGHT_WHITE_YELLOW,
    TEXU_CIO_BRIGHT_WHITE_BLUE,
    TEXU_CIO_BRIGHT_WHITE_MAGENTA,
    TEXU_CIO_BRIGHT_WHITE_CYAN,
    TEXU_CIO_BRIGHT_WHITE_WHITE,
    TEXU_CIO_BLACK_BRIGHT_WHITE,
    TEXU_CIO_RED_BRIGHT_WHITE,
    TEXU_CIO_GREEN_BRIGHT_WHITE,
    TEXU_CIO_YELLOW_BRIGHT_WHITE,
    TEXU_CIO_BLUE_BRIGHT_WHITE,
    TEXU_CIO_MAGENTA_BRIGHT_WHITE,
    TEXU_CIO_CYAN_BRIGHT_WHITE,
    TEXU_CIO_LAST_COLOR
};
#endif

#ifdef _USE_CURSES_
enum
{
    TEXU_CIO_COLOR_BLACK   = COLOR_BLACK,
    TEXU_CIO_COLOR_RED     = COLOR_RED,
    TEXU_CIO_COLOR_GREEN   = COLOR_GREEN,
    TEXU_CIO_COLOR_YELLOW  = COLOR_YELLOW,
    TEXU_CIO_COLOR_BLUE    = COLOR_BLUE,
    TEXU_CIO_COLOR_MAGENTA = COLOR_MAGENTA,
    TEXU_CIO_COLOR_CYAN    = COLOR_CYAN,
    TEXU_CIO_COLOR_WHITE   = COLOR_WHITE,
    #ifdef XTERM_256COLOR
    TEXU_CIO_BRIGHT_WHITE  = 15
    #else
    TEXU_CIO_BRIGHT_WHITE  = COLOR_WHITE
    #endif
};
#endif
/*
# TexU definitions and macros
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
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

/* status */
#define TEXU_EVALID                     (texu_i32)(-3)
#define TEXU_ENOMEM                     (texu_i32)(-2)
#define TEXU_NOMEM                      (texu_i32)(-2)
#define TEXU_ERROR                      (texu_i32)(-1)
#define TEXU_OK                         (texu_i32)(0)
#define TEXU_NOTFOUND                   (texu_i32)(1)
#define TEXU_NOTREGISTERED_CLASS        (texu_i32)(2)
#define TEXU_DUPLICATED_ID              (texu_i32)(3)

/* state */
#define TEXU_CONTINUE                   (texu_i32)(1)
#define TEXU_BREAK                      (texu_i32)(-1)

#define TEXU_ENV_STATE_STARTING         100
#define TEXU_ENV_STATE_RUNNING          200
#define TEXU_ENV_STATE_ENDING           0


/* boolean */
#define TEXU_FALSE                      (texu_i32)(0)
#define TEXU_TRUE                       (texu_i32)(1)

/* compare */
#define TEXU_CMP_LT                     (texu_i32)(-1)                /*less than*/
#define TEXU_CMP_EQ                     (texu_i32)(0)                 /*equal to*/
#define TEXU_CMP_GT                     (texu_i32)(1)                 /*greater than*/

#define TEXU_MAX_WNDTEXT                256
#define TEXU_MAX_BUFFER                 (TEXU_MAX_WNDTEXT * 2)
#define TEXU_1024_WNDTEXT               1024
#define TEXU_2048_WNDTEXT               2048

#define TEXU_SW_HIDE                    0
#define TEXU_SW_SHOW                    1

/*user message begin*/
#define TEXU_WM_USER                    10000
/*reserved messages*/
#define TEXU_WM_FIRST                   0
#define TEXU_WM_CHAR                    (TEXU_WM_FIRST +  1)
#define TEXU_WM_SETFOCUS                (TEXU_WM_FIRST +  2)
#define TEXU_WM_KILLFOCUS               (TEXU_WM_FIRST +  3)
#define TEXU_WM_SETTEXT                 (TEXU_WM_FIRST +  4)
#define TEXU_WM_GETTEXT                 (TEXU_WM_FIRST +  5)
#define TEXU_WM_PAINT                   (TEXU_WM_FIRST +  6)
#define TEXU_WM_CREATE                  (TEXU_WM_FIRST +  7)
#define TEXU_WM_DESTROY                 (TEXU_WM_FIRST +  8)
#define TEXU_WM_ENABLE                  (TEXU_WM_FIRST +  9)
#define TEXU_WM_SHOW                    (TEXU_WM_FIRST + 10)
#define TEXU_WM_ERASEBG                 (TEXU_WM_FIRST + 11)
#define TEXU_WM_COMMAND                 (TEXU_WM_FIRST + 12)
#define TEXU_WM_NOTIFY                  (TEXU_WM_FIRST + 13)
#define TEXU_WM_MOVE                    (TEXU_WM_FIRST + 14)
#define TEXU_WM_SETMENU                 (TEXU_WM_FIRST + 15)
#define TEXU_WM_REDRAWMENU              (TEXU_WM_FIRST + 16)
#define TEXU_WM_ENTERMENU               (TEXU_WM_FIRST + 17)
#define TEXU_WM_LEAVEMENU               (TEXU_WM_FIRST + 18)
#define TEXU_WM_IDLE                    (TEXU_WM_FIRST + 19)
#define TEXU_WM_QUERYNEXTWND            (TEXU_WM_FIRST + 20)
#define TEXU_WM_QUERYPREVWND            (TEXU_WM_FIRST + 21)
#define TEXU_WM_CLOSE                   (TEXU_WM_FIRST + 22)
#define TEXU_WM_QUERYCLOSE              (TEXU_WM_FIRST + 23)
#define TEXU_WM_KEYDOWN                 (TEXU_WM_FIRST + 24)
#define TEXU_WM_SIZE                    (TEXU_WM_FIRST + 25)
#define TEXU_WM_SETTEXTCOLOR            (TEXU_WM_FIRST + 26)
#define TEXU_WM_GETFIRSTACTIVECHILD     (TEXU_WM_FIRST + 27)
#define TEXU_WM_GETLASTACTIVECHILD      (TEXU_WM_FIRST + 28)
#define TEXU_WM_GETNEXTACTIVECHILD      (TEXU_WM_FIRST + 29)
#define TEXU_WM_GETPREVACTIVECHILD      (TEXU_WM_FIRST + 30)
#define TEXU_WM_SETACTIVECHILD          (TEXU_WM_FIRST + 31)
#define TEXU_WM_GETACTIVECHILD          (TEXU_WM_FIRST + 32)
#define TEXU_WM_GETFIRSTCHILD           (TEXU_WM_FIRST + 33)
#define TEXU_WM_GETLASTCHILD            (TEXU_WM_FIRST + 34)
#define TEXU_WM_GETNEXTWND              (TEXU_WM_FIRST + 35)
#define TEXU_WM_GETPREVWND              (TEXU_WM_FIRST + 36)
#define TEXU_WM_INVALIDATE              (TEXU_WM_FIRST + 37)
#define TEXU_WM_ACTIVATED               (TEXU_WM_FIRST + 38)
#define TEXU_WM_GETCLIENTRECT           (TEXU_WM_FIRST + 39)    /*drawable area, it will be applied in the container window, e.g. ReBar/Panel */
#define TEXU_WM_GETWINDOWRECT           (TEXU_WM_FIRST + 40)
#define TEXU_WM_GETSCREENRECT           (TEXU_WM_FIRST + 41)
#define TEXU_WM_QUERYKEYUP              (TEXU_WM_FIRST + 42)
#define TEXU_WM_QUERYKEYDOWN            (TEXU_WM_FIRST + 43)
#define TEXU_WM_QUERYKEYLEFT            (TEXU_WM_FIRST + 44)
#define TEXU_WM_QUERYKEYRIGHT           (TEXU_WM_FIRST + 45)
#define TEXU_WM_QUERYKEYPGUP            (TEXU_WM_FIRST + 46)
#define TEXU_WM_QUERYKEYPGDOWN          (TEXU_WM_FIRST + 47)
#define TEXU_WM_SETFGCOLOR              (TEXU_WM_FIRST + 48)
#define TEXU_WM_SETBGCOLOR              (TEXU_WM_FIRST + 49)
#define TEXU_WM_GETFGCOLOR              (TEXU_WM_FIRST + 50)
#define TEXU_WM_GETBGCOLOR              (TEXU_WM_FIRST + 51)

/*color idx*/
#define TEXU_COLOR_NORMAL     0
#define TEXU_COLOR_DISABLED   1
#define TEXU_COLOR_FOCUSED    2
#define TEXU_COLOR_SELECTED   3

/* notify */
#define TEXU_WN_FIRST                   0
#define TEXU_WN_GOFIRSTCHILD            (TEXU_WN_FIRST +  1)
#define TEXU_WN_GOLASTCHILD             (TEXU_WN_FIRST +  2)

/* LABEL */
#define TEXU_LM_FIRST                   (TEXU_WM_FIRST + 100)
#define TEXU_LM_HIGHLIGHT               (TEXU_LM_FIRST +  1)
#define TEXU_LM_FRAMECOLOR              (TEXU_LM_FIRST +  2)

/* EDIT */
#define TEXU_EM_FIRST                   (TEXU_WM_FIRST + 200)
#define TEXU_EM_LIMITTEXT               (TEXU_EM_FIRST +  1)
#define TEXU_EM_SETPASSWDCHAR           (TEXU_EM_FIRST +  2)
#define TEXU_EM_SETDECWIDTH             (TEXU_EM_FIRST +  3)
#define TEXU_EM_SHOWPASSWDCHAR          (TEXU_EM_FIRST +  4)
#define TEXU_EM_SETVALIDSTRING          (TEXU_EM_FIRST +  5)
#define TEXU_EM_SETVALIDMINMAX          (TEXU_EM_FIRST +  6)
#define TEXU_EM_INVALIDATE              (TEXU_EM_FIRST +  7)
#define TEXU_EM_GETINT                  (TEXU_EM_FIRST +  8)
#define TEXU_EM_GETFLOAT                (TEXU_EM_FIRST +  9)
#define TEXU_EM_SETVALIDATION           (TEXU_EM_FIRST + 10)

/* edit notification */
#define TEXU_EN_FIRST                   TEXU_EM_FIRST
#define TEXU_EN_SETFOCUS                (TEXU_EN_FIRST +  1)
#define TEXU_EN_KILLFOCUS               (TEXU_EN_FIRST +  2)
#define TEXU_EN_INVALIDMINMAX           (TEXU_EN_FIRST +  3)
#define TEXU_EN_INVALIDATE              (TEXU_EN_FIRST +  4)
#define TEXU_EN_PRESSEDENTER            (TEXU_EN_FIRST +  5)

/* BUTTON */
#define TEXU_BM_FIRST                   (TEXU_WM_FIRST + 250)
#define TEXU_BM_PUSH                    (TEXU_BM_FIRST +  1)

/* BUTTON notification */
#define TEXU_BN_FIRST                   TEXU_BM_FIRST
#define TEXU_BN_SETFOCUS                (TEXU_BN_FIRST +  1)
#define TEXU_BN_KILLFOCUS               (TEXU_BN_FIRST +  2)



/* LISTBOX */
#define TEXU_LBM_FIRST                  (TEXU_WM_FIRST + 300)
#define TEXU_LBM_ADDITEM                (TEXU_LBM_FIRST +  1)
#define TEXU_LBM_DELETEITEM             (TEXU_LBM_FIRST +  2)
#define TEXU_LBM_GETCURSEL              (TEXU_LBM_FIRST +  3)
#define TEXU_LBM_SETCURSEL              (TEXU_LBM_FIRST +  4)
#define TEXU_LBM_DELETEALLITEMS         (TEXU_LBM_FIRST +  5)
#define TEXU_LBM_GETITEMCOUNT           (TEXU_LBM_FIRST +  6)
#define TEXU_LBM_GETITEMDATA            (TEXU_LBM_FIRST +  7)
#define TEXU_LBM_SETITEMDATA            (TEXU_LBM_FIRST +  8)
#define TEXU_LBM_SETITEMTEXT            (TEXU_LBM_FIRST +  9)
#define TEXU_LBM_GETITEMTEXT            (TEXU_LBM_FIRST + 10)
#define TEXU_LBM_COUNTITEMCHECKED       (TEXU_LBM_FIRST + 11)
#define TEXU_LBM_GETITEMCHECKED         (TEXU_LBM_FIRST + 12)
#define TEXU_LBM_SETITEMCHECKED         (TEXU_LBM_FIRST + 13)
#define TEXU_LBM_ENABLEITEM             (TEXU_LBM_FIRST + 14)
#define TEXU_LBM_SETSELCOLOR            (TEXU_LBM_FIRST + 15)
#define TEXU_LBM_SETPAGEHEIGHT          (TEXU_LBM_FIRST + 16)

/* listbox notification */
#define TEXU_LBN_FIRST                  TEXU_LBM_FIRST
#define TEXU_LBN_SETFOCUS               (TEXU_LBN_FIRST +  1)
#define TEXU_LBN_KILLFOCUS              (TEXU_LBN_FIRST +  2)
#define TEXU_LBN_DELETEITEM             (TEXU_LBN_FIRST +  3)
#define TEXU_LBN_SELCHANGED             (TEXU_LBN_FIRST +  4)



/* listctrl notification */
#define LISTCTRL_STATE_NORMAL       0x0000
#define LISTCTRL_STATE_DISABLED     0x0001
#define LISTCTRL_STATE_FOCUSED      0x0002

#define TEXU_LCM_FIRST                  (TEXU_WM_FIRST + 400)
#define TEXU_LCM_SETEDITABLECOLS        (TEXU_LCM_FIRST +  1)
#define TEXU_LCM_GETITEMSPERPAGE        (TEXU_LCM_FIRST +  2)
#define TEXU_LCM_GETCURPAGE             (TEXU_LCM_FIRST +  3)
#define TEXU_LCM_SETCURPAGE             (TEXU_LCM_FIRST +  4)
#define TEXU_LCM_GETEDITBOX             (TEXU_LCM_FIRST +  5)
#define TEXU_LCM_SETCOLWIDTH            (TEXU_LCM_FIRST +  6)
#define TEXU_LCM_SETCURROW              (TEXU_LCM_FIRST +  7)
#define TEXU_LCM_GETCURROW              (TEXU_LCM_FIRST +  8)
#define TEXU_LCM_GETEDITSTYLE           (TEXU_LCM_FIRST +  9)
#define TEXU_LCM_SETEDITSTYLE           (TEXU_LCM_FIRST + 10)
#define TEXU_LCM_INVALIDATEITEM         (TEXU_LCM_FIRST + 11)
#define TEXU_LCM_GETITEMCOUNT           (TEXU_LCM_FIRST + 12)
#define TEXU_LCM_GETITEM                (TEXU_LCM_FIRST + 13)
#define TEXU_LCM_SETITEM                (TEXU_LCM_FIRST + 14)
#define TEXU_LCM_DELETEALLITEMS         (TEXU_LCM_FIRST + 15)
#define TEXU_LCM_DELETEITEM             (TEXU_LCM_FIRST + 16)
#define TEXU_LCM_ADDITEM                (TEXU_LCM_FIRST + 17)
#define TEXU_LCM_DELETEALLCOLUMNS       (TEXU_LCM_FIRST + 18)
#define TEXU_LCM_DELETECOLUMN           (TEXU_LCM_FIRST + 19)
#define TEXU_LCM_ADDCOLUMN              (TEXU_LCM_FIRST + 20)
#define TEXU_LCM_ADDITEMS               (TEXU_LCM_FIRST + 21)
#define TEXU_LCM_ADDITEMATFIRST         (TEXU_LCM_FIRST + 22)
#define TEXU_LCM_SETITEMTEXT            (TEXU_LCM_FIRST + 23)
#define TEXU_LCM_SETITEMDATA            (TEXU_LCM_FIRST + 24)
#define TEXU_LCM_GETITEMTEXT            (TEXU_LCM_FIRST + 25)
#define TEXU_LCM_GETITEMDATA            (TEXU_LCM_FIRST + 26)
#define TEXU_LCM_REFRESH                (TEXU_LCM_FIRST + 27)
#define TEXU_LCM_SETROWCOLOR            (TEXU_LCM_FIRST + 28)

#define TEXU_LCN_FIRST                  TEXU_LCM_FIRST
#define TEXU_LCN_SETFOCUS               (TEXU_LCN_FIRST +  1)
#define TEXU_LCN_KILLFOCUS              (TEXU_LCN_FIRST +  2)
#define TEXU_LCN_DELETEITEM             (TEXU_LCN_FIRST +  3)
#define TEXU_LCN_SELCHANGED             (TEXU_LCN_FIRST +  4)
#define TEXU_LCN_BEGINMOVING            (TEXU_LCN_FIRST +  5)
#define TEXU_LCN_ENDMOVING              (TEXU_LCN_FIRST +  6)
#define TEXU_LCN_ENDINSERTROWOK         (TEXU_LCN_FIRST +  7)
#define TEXU_LCN_ENDINSERTROWCANCEL     (TEXU_LCN_FIRST +  8)
#define TEXU_LCN_BEGININSERTROW         (TEXU_LCN_FIRST +  9)
#define TEXU_LCN_BEGINEDITROW           (TEXU_LCN_FIRST + 10)
#define TEXU_LCN_ENDEDITROWCANCEL       (TEXU_LCN_FIRST + 11)
#define TEXU_LCN_ENDEDITROWOK           (TEXU_LCN_FIRST + 12)
#define TEXU_LCN_BEGINEDIT              (TEXU_LCN_FIRST + 13)
#define TEXU_LCN_ENDEDITOK              (TEXU_LCN_FIRST + 14)
#define TEXU_LCN_ENDEDITCANCEL          (TEXU_LCN_FIRST + 15)
#define TEXU_LCN_PRESSEDENTER           (TEXU_LCN_FIRST + 16)
#define TEXU_LCN_PRESSEDDELETE          (TEXU_LCN_FIRST + 17)
#define TEXU_LCN_PRESSEDINSERT          (TEXU_LCN_FIRST + 18)

/*TREE CTRL*/
#define TEXU_TCM_FIRST                  (TEXU_WM_FIRST + 500)
#define TEXU_TCM_EXPANDALLITEMS         (TEXU_TCM_FIRST +  1)
#define TEXU_TCM_COLLAPSEALLITEMS       (TEXU_TCM_FIRST +  2)
#define TEXU_TCM_IMPORTFROMFILE         (TEXU_TCM_FIRST +  3)
#define TEXU_TCM_EXPORTTOFILE           (TEXU_TCM_FIRST +  4)
#define TEXU_TCM_GETSELITEM             (TEXU_TCM_FIRST +  5)
#define TEXU_TCM_SETSELITEM             (TEXU_TCM_FIRST +  6)
#define TEXU_TCM_SETFINDITEMPROC        (TEXU_TCM_FIRST +  7)
#define TEXU_TCM_COLLAPSEITEM           (TEXU_TCM_FIRST +  8)
#define TEXU_TCM_EXPANDITEM             (TEXU_TCM_FIRST +  9)
#define TEXU_TCM_FINDNEXTITEM           (TEXU_TCM_FIRST + 10)
#define TEXU_TCM_FINDITEM               (TEXU_TCM_FIRST + 11)
#define TEXU_TCM_GETITEM                (TEXU_TCM_FIRST + 12)
#define TEXU_TCM_SETITEM                (TEXU_TCM_FIRST + 13)
#define TEXU_TCM_DELETEITEM             (TEXU_TCM_FIRST + 14)
#define TEXU_TCM_INSERTITEM             (TEXU_TCM_FIRST + 15)



#define TEXU_TCN_FIRST                  TEXU_TCM_FIRST
#define TEXU_TCN_SETFOCUS               (TEXU_TCN_FIRST +  1)
#define TEXU_TCN_KILLFOCUS              (TEXU_TCN_FIRST +  2)
#define TEXU_TCN_ITEMCOLLAPSING         (TEXU_TCN_FIRST +  3)
#define TEXU_TCN_ITEMCOLLAPSED          (TEXU_TCN_FIRST +  4)
#define TEXU_TCN_ITEMEXPANDING          (TEXU_TCN_FIRST +  5)
#define TEXU_TCN_ITEMEXPANDED           (TEXU_TCN_FIRST +  6)
#define TEXU_TCN_BEGINEDIT              (TEXU_TCN_FIRST +  7)
#define TEXU_TCN_ENDEDITOK              (TEXU_TCN_FIRST +  8)
#define TEXU_TCN_ENDEDITCANCEL          (TEXU_TCN_FIRST +  9)



/*UP DOWN CTRL*/
#define TEXU_UDCM_FIRST                 (TEXU_WM_FIRST + 600)
#define TEXU_UDCM_SETMINMAX             (TEXU_UDCM_FIRST +  1)
#define TEXU_UDCM_GETMINMAX             (TEXU_UDCM_FIRST +  2)
#define TEXU_UDCM_SETSTEP               (TEXU_UDCM_FIRST +  3)
#define TEXU_UDCM_GETSTEP               (TEXU_UDCM_FIRST +  4)
#define TEXU_UDCM_STEP                  (TEXU_UDCM_FIRST +  5)
#define TEXU_UDCM_SETPAGE               (TEXU_UDCM_FIRST +  6)
#define TEXU_UDCM_GETPAGE               (TEXU_UDCM_FIRST +  7)
#define TEXU_UDCM_PAGE                  (TEXU_UDCM_FIRST +  8)
#define TEXU_UDCM_GETINT                (TEXU_UDCM_FIRST +  9)

#define TEXU_UDCN_FIRST                 TEXU_UDCM_FIRST
#define TEXU_UDCN_SETFOCUS              (TEXU_UDCN_FIRST +  1)
#define TEXU_UDCN_KILLFOCUS             (TEXU_UDCN_FIRST +  2)
#define TEXU_UDCN_STEP                  (TEXU_UDCN_FIRST +  3)
#define TEXU_UDCN_INVALIDATE            (TEXU_UDCN_FIRST +  4)


/*PROGRESS BAR*/
#define TEXU_PGBM_FIRST                 (TEXU_WM_FIRST + 700)
#define TEXU_PGBM_SETMAX                (TEXU_PGBM_FIRST +  1)
#define TEXU_PGBM_GETMAX                (TEXU_PGBM_FIRST +  2)
#define TEXU_PGBM_SETPOS                (TEXU_PGBM_FIRST +  3)
#define TEXU_PGBM_GETPOS                (TEXU_PGBM_FIRST +  4)
#define TEXU_PGBM_SETSTEP               (TEXU_PGBM_FIRST +  5)
#define TEXU_PGBM_GETSTEP               (TEXU_PGBM_FIRST +  6)
#define TEXU_PGBM_STEP                  (TEXU_PGBM_FIRST +  7)
#define TEXU_PGBM_GETPERCENT            (TEXU_PGBM_FIRST +  8)
#define TEXU_PGBM_SETDECIMAL            (TEXU_PGBM_FIRST +  9)
#define TEXU_PGBM_GETDECIMAL            (TEXU_PGBM_FIRST + 10)

#define TEXU_PGBN_FIRST                 TEXU_PGBM_FIRST


/*STATUS BAR*/
#define TEXU_SBM_FIRST                  (TEXU_WM_FIRST + 800)
#define TEXU_SBM_ADDPART                (TEXU_SBM_FIRST +  1)
#define TEXU_SBM_SETTEXT                (TEXU_SBM_FIRST +  2)
#define TEXU_SBM_GETTEXT                (TEXU_SBM_FIRST +  3)
#define TEXU_SBM_SETWIDTH               (TEXU_SBM_FIRST +  4)
#define TEXU_SBM_GETWIDTH               (TEXU_SBM_FIRST +  5)
#define TEXU_SBM_SETALIGN               (TEXU_SBM_FIRST +  6)
#define TEXU_SBM_GETALIGN               (TEXU_SBM_FIRST +  7)
#define TEXU_SBM_SETCOLOR               (TEXU_SBM_FIRST +  8)
#define TEXU_SBM_GETCOLOR               (TEXU_SBM_FIRST +  9)
#define TEXU_SBM_SETBGCOLOR             (TEXU_SBM_FIRST + 10)
#define TEXU_SBM_GETBGCOLOR             (TEXU_SBM_FIRST + 11)
#define TEXU_SBM_GETPARTCOUNT           (TEXU_SBM_FIRST + 12)
#define TEXU_SBM_ADDPARTRECT            (TEXU_SBM_FIRST + 13)
#define TEXU_SBM_GETPARTRECT            (TEXU_SBM_FIRST + 14)

#define TEXU_SBN_FIRST                  TEXU_SBM_FIRST

/*MESSAGE BOX*/
#define TEXU_MBM_FIRST                  (TEXU_WM_FIRST + 900)
#define TEXU_MBM_SETOWNER               (TEXU_MBM_FIRST +  1)
#define TEXU_MBM_SETCAPTION             (TEXU_MBM_FIRST +  2)
#define TEXU_MBM_SETTITLECOLOR          (TEXU_MBM_FIRST +  3)

#define TEXU_MBN_FIRST                  TEXU_MBM_FIRST
#define TEXU_MBN_ENDDIALOG              (TEXU_MBN_FIRST +  1)


/*PAGECTRL*/
#define TEXU_PGM_FIRST                  (TEXU_WM_FIRST + 1000)
#define TEXU_PGM_ADDPAGE                (TEXU_PGM_FIRST +  1)
#define TEXU_PGM_SETCURPAGE             (TEXU_PGM_FIRST +  2)
#define TEXU_PGM_GETCURPAGE             (TEXU_PGM_FIRST +  3)
#define TEXU_PGM_COUNTPAGE              (TEXU_PGM_FIRST +  4)
#define TEXU_PGM_GETPAGEINDEX           (TEXU_PGM_FIRST +  5)
#define TEXU_PGM_REMOVEPAGE             (TEXU_PGM_FIRST +  6)
#define TEXU_PGM_REMOVEALLPAGES         (TEXU_PGM_FIRST +  7)

#define TEXU_PGN_FIRST                  TEXU_PGM_FIRST

/* COMBOXBOX */
#define TEXU_CBM_FIRST                  (TEXU_WM_FIRST + 1100)
#define TEXU_CBM_ADDITEM                (TEXU_CBM_FIRST +  1)
#define TEXU_CBM_DELETEITEM             (TEXU_CBM_FIRST +  2)
#define TEXU_CBM_GETCURSEL              (TEXU_CBM_FIRST +  3)
#define TEXU_CBM_SETCURSEL              (TEXU_CBM_FIRST +  4)
#define TEXU_CBM_DELETEALLITEMS         (TEXU_CBM_FIRST +  5)
#define TEXU_CBM_GETITEMCOUNT           (TEXU_CBM_FIRST +  6)
#define TEXU_CBM_GETITEMDATA            (TEXU_CBM_FIRST +  7)
#define TEXU_CBM_SETITEMDATA            (TEXU_CBM_FIRST +  8)
#define TEXU_CBM_SETITEMTEXT            (TEXU_CBM_FIRST +  9)
#define TEXU_CBM_GETITEMTEXT            (TEXU_CBM_FIRST + 10)
#define TEXU_CBM_COUNTITEMCHECKED       (TEXU_CBM_FIRST + 11)
#define TEXU_CBM_GETITEMCHECKED         (TEXU_CBM_FIRST + 12)
#define TEXU_CBM_SETITEMCHECKED         (TEXU_CBM_FIRST + 13)
#define TEXU_CBM_ENABLEITEM             (TEXU_CBM_FIRST + 14)
#define TEXU_CBM_GETLISTBOX             (TEXU_CBM_FIRST + 15)

/* combobox notification */
#define TEXU_CBN_FIRST                  TEXU_CBM_FIRST
#define TEXU_CBN_SETFOCUS               (TEXU_CBN_FIRST +  1)
#define TEXU_CBN_KILLFOCUS              (TEXU_CBN_FIRST +  2)
#define TEXU_CBN_DELETEITEM             (TEXU_CBN_FIRST +  3)
#define TEXU_CBN_SELCHANGED             (TEXU_CBN_FIRST +  4)
#define TEXU_CBN_ENTER_LISTBOX          (TEXU_CBN_FIRST +  5)
#define TEXU_CBN_LEAVE_LISTBOX          (TEXU_CBN_FIRST +  6)

/* TEXT CTRL */
#define TEXU_TXCM_FIRST                 (TEXU_WM_FIRST + 1200)
#define TEXU_TXCM_ADDSENTENCE           (TEXU_TXCM_FIRST +  1)
#define TEXU_TXCM_REMOVESENTENCE        (TEXU_TXCM_FIRST +  2)
#define TEXU_TXCM_INSERTSENTENCE        (TEXU_TXCM_FIRST +  3)
#define TEXU_TXCM_SETSENTENCE           (TEXU_TXCM_FIRST +  4)
#define TEXU_TXCM_GETSENTENCE           (TEXU_TXCM_FIRST +  5)
#define TEXU_TXCM_ADDPARAGRAPH          (TEXU_TXCM_FIRST +  6)
#define TEXU_TXCM_REMOVEPARAGRAPH       (TEXU_TXCM_FIRST +  7)
#define TEXU_TXCM_INSERTPARAGRAPH       (TEXU_TXCM_FIRST +  8)
#define TEXU_TXCM_SETPARAGRAPH          (TEXU_TXCM_FIRST +  9)
#define TEXU_TXCM_GETPARAGRAPH          (TEXU_TXCM_FIRST + 10)
#define TEXU_TXCM_SETCURPARAGRAPH       (TEXU_TXCM_FIRST + 11)
#define TEXU_TXCM_GETCURPARAGRAPH       (TEXU_TXCM_FIRST + 12)

#define TEXU_TXCN_FIRST                 TEXU_TXCM_FIRST

/* REBAR */
#define TEXU_RBF_COLOR      0x0001
#define TEXU_RBF_ALIGNMENT  0x0002
#define TEXU_RBF_CAPTION    0x0004
#define TEXU_RBF_UNIT       0x0008
#define TEXU_RBF_ENABLE     0x0010
#define TEXU_RBF_VISIBLE    0x0020
#define TEXU_RBF_ALL        (TEXU_RBF_COLOR | TEXU_RBF_ALIGNMENT | TEXU_RBF_CAPTION | TEXU_RBF_UNIT | TEXU_RBF_ENABLE | TEXU_RBF_VISIBLE)


#define TEXU_RBM_FIRST                  (TEXU_WM_FIRST + 1300)
#define TEXU_RBM_ADDBAND                (TEXU_RBM_FIRST +   1)
#define TEXU_RBM_REMOVEBAND             (TEXU_RBM_FIRST +   2)
#define TEXU_RBM_SETINFOWIDTH           (TEXU_RBM_FIRST +   3)
#define TEXU_RBM_SETUNITWIDTH           (TEXU_RBM_FIRST +   4)
#define TEXU_RBM_SETBANDWIDTH           (TEXU_RBM_FIRST +   5)
#define TEXU_RBM_GETBAND                (TEXU_RBM_FIRST +   6)
#define TEXU_RBM_GETNEXTBAND            (TEXU_RBM_FIRST +   7)
#define TEXU_RBM_GETPREVBAND            (TEXU_RBM_FIRST +   8)
#define TEXU_RBM_GETFIRSTBAND           (TEXU_RBM_FIRST +   9)
#define TEXU_RBM_GETLASTBAND            (TEXU_RBM_FIRST +  10)
#define TEXU_RBM_SETBAND                (TEXU_RBM_FIRST +  11)
#define TEXU_RBM_GETNEXTACTIVEBAND      (TEXU_RBM_FIRST +  12)
#define TEXU_RBM_GETPREVACTIVEBAND      (TEXU_RBM_FIRST +  13)
#define TEXU_RBM_GETFIRSTACTIVEBAND     (TEXU_RBM_FIRST +  14)
#define TEXU_RBM_GETLASTACTIVEBAND      (TEXU_RBM_FIRST +  15)
#define TEXU_RBM_GETNEXTVISIBLEBAND     (TEXU_RBM_FIRST +  16)
#define TEXU_RBM_GETPREVVISIBLEBAND     (TEXU_RBM_FIRST +  17)
#define TEXU_RBM_GETFIRSTVISIBLEBAND    (TEXU_RBM_FIRST +  18)
#define TEXU_RBM_GETLASTVISIBLEBAND     (TEXU_RBM_FIRST +  19)
#define TEXU_RBM_SETMAXROW              (TEXU_RBM_FIRST +  20)
#define TEXU_RBM_GETMAXROW              (TEXU_RBM_FIRST +  21)
#define TEXU_RBM_GOTOFIRSTCOLUMN        (TEXU_RBM_FIRST +  22)
#define TEXU_RBM_GOTOSECONDCOLUMN       (TEXU_RBM_FIRST +  23)
#define TEXU_RBM_GOTONEXTPAGE           (TEXU_RBM_FIRST +  24)
#define TEXU_RBM_GOTOPREVPAGE           (TEXU_RBM_FIRST +  25)
#define TEXU_RBM_GOTOPAGE               (TEXU_RBM_FIRST +  26)
#define TEXU_RBM_LOADTEMPLATE           (TEXU_RBM_FIRST +  27)
#define TEXU_RBM_GETCURPAGE             (TEXU_RBM_FIRST +  28)
#define TEXU_RBM_GETMAXPAGE             (TEXU_RBM_FIRST +  29)

#define TEXU_RBN_FIRST                  TEXU_RBM_FIRST
#define TEXU_RBN_FIRSTCHILD             (TEXU_RBM_FIRST +  1)
#define TEXU_RBN_LASTCHILD              (TEXU_RBM_FIRST +  2)
#define TEXU_RBN_NEXTCHILD              (TEXU_RBM_FIRST +  3)
#define TEXU_RBN_PREVCHILD              (TEXU_RBM_FIRST +  4)
#define TEXU_RBN_FIRST_VISIBLE_CHILD    (TEXU_RBM_FIRST +  5)
#define TEXU_RBN_MOVEDNEXTCOLUMN        (TEXU_RBM_FIRST +  6)
#define TEXU_RBN_MOVEDPREVCOLUMN        (TEXU_RBM_FIRST +  7)
#define TEXU_RBN_MOVEDNEXTPAGE          (TEXU_RBM_FIRST +  8)
#define TEXU_RBN_MOVEDPREVPAGE          (TEXU_RBM_FIRST +  9)
#define TEXU_RBN_PAGECHANGING           (TEXU_RBM_FIRST + 10)
#define TEXU_RBN_PAGECHANGED            (TEXU_RBM_FIRST + 11)

/*IP ADDRESS CTRL*/
#define TEXU_IPM_FIRST                 (TEXU_WM_FIRST + 1400)
#define TEXU_IPM_SETMINMAX             (TEXU_IPM_FIRST +  1)
#define TEXU_IPM_GETMINMAX             (TEXU_IPM_FIRST +  2)
#define TEXU_IPM_SETSTEP               (TEXU_IPM_FIRST +  3)
#define TEXU_IPM_GETSTEP               (TEXU_IPM_FIRST +  4)
#define TEXU_IPM_STEP                  (TEXU_IPM_FIRST +  5)
#define TEXU_IPM_SETPAGE               (TEXU_IPM_FIRST +  6)
#define TEXU_IPM_GETPAGE               (TEXU_IPM_FIRST +  7)
#define TEXU_IPM_PAGE                  (TEXU_IPM_FIRST +  8)
#define TEXU_IPM_GETIPADDRESS          (TEXU_IPM_FIRST +  9)

#define TEXU_IPN_FIRST                 TEXU_IPM_FIRST
#define TEXU_IPN_SETFOCUS              (TEXU_IPM_FIRST +  1)
#define TEXU_IPN_KILLFOCUS             (TEXU_IPM_FIRST +  2)
#define TEXU_IPN_STEP                  (TEXU_IPM_FIRST +  3)


/*EDIT MASK CTRL*/
#define TEXU_EMM_FIRST                  (TEXU_WM_FIRST + 1500)
#define TEXU_EMM_SETMASK                (TEXU_EMM_FIRST +  1)
#define TEXU_EMM_GETMASK                (TEXU_EMM_FIRST +  2)
#define TEXU_EMM_SETINFO                (TEXU_EMM_FIRST +  3)
#define TEXU_EMM_SETEMAILMASK           (TEXU_EMM_FIRST +  4)

#define TEXU_EMN_FIRST                  (TEXU_WM_FIRST + TEXU_EMM_FIRST)
#define TEXU_EMN_SETFOCUS               (TEXU_EMM_FIRST +  1)
#define TEXU_EMN_KILLFOCUS              (TEXU_EMM_FIRST +  2)
#define TEXU_EMN_INVALIDEXPR            (TEXU_EMM_FIRST +  3)

#define TEXU_EDITMASK_EMAIL             TEXUTEXT("^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+.[a-zA-Z0-9-.]+$")

/*EDIT PRICE SPREAD CTRL*/
#define TEXU_EPSM_FIRST                 (TEXU_WM_FIRST + 1600)
#define TEXU_EPSM_INSERTSPREAD          (TEXU_EPSM_FIRST +  1)
#define TEXU_EPSM_DELETESPREAD          (TEXU_EPSM_FIRST +  2)
#define TEXU_EPSM_SETBASESPREAD         (TEXU_EPSM_FIRST +  3)
#define TEXU_EPSM_GETBASESPREAD         (TEXU_EPSM_FIRST +  4)
#define TEXU_EPSM_SETPRICEDECIMAL       (TEXU_EPSM_FIRST +  5)
#define TEXU_EPSM_LOADDEFAULTSPREADS    (TEXU_EPSM_FIRST +  6)
#define TEXU_EPSM_LOADSPREADS           (TEXU_EPSM_FIRST +  7)
#define TEXU_EPSM_ALLOWOVERCEILINGFLOOR (TEXU_EPSM_FIRST +  8)
#define TEXU_EPSM_CORRECTCEILINGFLOOR   (TEXU_EPSM_FIRST +  9)
#define TEXU_EPSM_GETPRICEASINT         (TEXU_EPSM_FIRST + 10)
#define TEXU_EPSM_LIMITTEXT             (TEXU_EPSM_FIRST + 11)

#define TEXU_EPSN_FIRST                 TEXU_EPSM_FIRST
#define TEXU_EPSN_SETFOCUS              (TEXU_EPSN_FIRST +  1)
#define TEXU_EPSN_KILLFOCUS             (TEXU_EPSN_FIRST +  2)
#define TEXU_EPSN_INVALIDSPREAD         (TEXU_EPSN_FIRST +  3)
#define TEXU_EPSN_INVALIDCEILING        (TEXU_EPSN_FIRST +  4)
#define TEXU_EPSN_INVALIDFLOOR          (TEXU_EPSN_FIRST +  5)
#define TEXU_EPSN_INVALIDPRICE          (TEXU_EPSN_FIRST +  6)

/*child*/
#define TEXU_EPS_EDIT_NUMBER_ID         (1)
#define TEXU_EPS_EDIT_ALPHA_ID          (2)

/*CLOCK CTRL*/
#define TEXU_CLKM_FIRST                 (TEXU_WM_FIRST + 1700)
#define TEXU_CLKM_UPDATE                (TEXU_CLKM_FIRST +  1)

#define TEXU_CLKN_FIRST                 TEXU_CLKM_FIRST

/*DATE CTRL*/
#define TEXU_DTM_FIRST                  (TEXU_WM_FIRST + 1800)
#define TEXU_DTM_GETDATE                (TEXU_DTM_FIRST +  1)
#define TEXU_DTM_SETDATE                (TEXU_DTM_FIRST +  2)
#define TEXU_DTM_SETFORMAT              (TEXU_DTM_FIRST +  3)

#define TEXU_DTN_FIRST                  TEXU_DTM_FIRST
#define TEXU_DTN_SETFOCUS               (TEXU_DTN_FIRST +  1)
#define TEXU_DTN_KILLFOCUS              (TEXU_DTN_FIRST +  2)
#define TEXU_DTN_INVALIDDAY             (TEXU_DTN_FIRST +  3)
#define TEXU_DTN_INVALIDMONTH           (TEXU_DTN_FIRST +  4)
#define TEXU_DTN_INVALIDYEAR            (TEXU_DTN_FIRST +  5)
#define TEXU_DTN_INVALIDDATE            (TEXU_DTN_FIRST +  6)

/*TIME CTRL*/
#define TEXU_TMM_FIRST                  (TEXU_WM_FIRST + 1900)
#define TEXU_TMM_GETTIME                (TEXU_TMM_FIRST +  1)
#define TEXU_TMM_SETTIME                (TEXU_TMM_FIRST +  2)


#define TEXU_TMN_FIRST                  TEXU_TMM_FIRST


/*Edit Volume*/
#define TEXU_EVM_FIRST                  (TEXU_WM_FIRST + 2000)
#define TEXU_EVM_SETLOT                 (TEXU_EVM_FIRST +  1)
#define TEXU_EVM_GETLOT                 (TEXU_EVM_FIRST +  2)
#define TEXU_EVM_ALLOWODDLOT            (TEXU_EVM_FIRST +  3)
#define TEXU_EVM_SETVOLUME              (TEXU_EVM_FIRST +  4)
#define TEXU_EVM_GETVOLUME              (TEXU_EVM_FIRST +  5)

#define TEXU_EVN_FIRST                  TEXU_EVM_FIRST
#define TEXU_EVN_SETFOCUS               (TEXU_EVN_FIRST +  1)
#define TEXU_EVN_KILLFOCUS              (TEXU_EVN_FIRST +  2)
#define TEXU_EVN_INVALIDLOT             (TEXU_EVM_FIRST +  3)

/*MENU */
#define TEXU_MNM_FIRST                  (TEXU_WM_FIRST + 10000)

#define TEXU_MNN_FIRST                  TEXU_MNM_FIRST
#define TEXU_MNN_ITEMCHANGED            (TEXU_MNN_FIRST +  1)

/*PANEL*/
#define TEXU_PNM_FIRST                  (TEXU_WM_FIRST + 10100)
#define TEXU_PNM_SETTITLECOLOR          (TEXU_PNM_FIRST +    1)
#define TEXU_PNM_SETTITLEALIGNMENT      (TEXU_PNM_FIRST +    2)
#define TEXU_PNM_GETSTATUSBAR           (TEXU_PNM_FIRST +    3)
#define TEXU_PNM_GETTITLE               (TEXU_PNM_FIRST +    4)
#define TEXU_PNM_SETTITLEBGCOLOR        (TEXU_PNM_FIRST +    5)

#define TEXU_PNN_FIRST                  TEXU_PNM_FIRST
#define TEXU_PNN_KEYDOWN                TEXU_PNN_FIRST


#ifdef __USE_CURSES__
#define TEXU_KEY_SELMENU                10
#define TEXU_KEY_NEXTWND                10
#define TEXU_KEY_ENTER                  10
#define TEXU_KEY_TAB                    9
#define TEXU_KEY_PREVWND                KEY_BTAB
#define TEXU_KEY_BACKSPACE              KEY_BACKSPACE
#define TEXU_KEY_IC                     KEY_IC
#define TEXU_KEY_DC                     KEY_DC
#define TEXU_KEY_LEFT                   KEY_LEFT
#define TEXU_KEY_RIGHT                  KEY_RIGHT
#define TEXU_KEY_DOWN                   KEY_DOWN
#define TEXU_KEY_UP                     KEY_UP
#define TEXU_KEY_PPAGE                  KEY_PPAGE
#define TEXU_KEY_NPAGE                  KEY_NPAGE
#define TEXU_KEY_ESCAPE                 27
#elif (defined __USE_TERMIOS__ || defined __USE_TERMBOX2__)
#define TEXU_KEY_SELMENU                TB_KEY_ENTER
#define TEXU_KEY_NEXTWND                TB_KEY_ENTER
#define TEXU_KEY_ENTER                  TB_KEY_ENTER
#define TEXU_KEY_NUMENTER               270
#define TEXU_KEY_TAB                    TB_KEY_TAB
#define TEXU_KEY_PREVWND                511
#define TEXU_KEY_BACKSPACE              TB_KEY_BACKSPACE
#define TEXU_KEY_IC                     TB_KEY_INSERT
#define TEXU_KEY_DC                     TB_KEY_DELETE
#define TEXU_KEY_LEFT                   TB_KEY_ARROW_LEFT
#define TEXU_KEY_RIGHT                  TB_KEY_ARROW_RIGHT
#define TEXU_KEY_DOWN                   TB_KEY_ARROW_DOWN
#define TEXU_KEY_UP                     TB_KEY_ARROW_UP
#define TEXU_KEY_PPAGE                  TB_KEY_PGUP
#define TEXU_KEY_NPAGE                  TB_KEY_PGDN
#define TEXU_KEY_ESCAPE                 TB_KEY_ESC
#define TEXU_KEY_HOME                   TB_KEY_HOME
#define TEXU_KEY_END                    TB_KEY_END
#elif (defined __VMS__) || (defined __USE_TTY__)
#define TEXU_KEY_SELMENU                10
#define TEXU_KEY_NEXTWND                13
#define TEXU_KEY_ENTER                  13
#define TEXU_KEY_NUMENTER               270
#define TEXU_KEY_TAB                    9
#define TEXU_KEY_PREVWND                511
#define TEXU_KEY_BACKSPACE              127
#define TEXU_KEY_IC                     312
#define TEXU_KEY_DC                     313
#define TEXU_KEY_LEFT                   276
#define TEXU_KEY_RIGHT                  277
#define TEXU_KEY_DOWN                   275
#define TEXU_KEY_UP                     274
#define TEXU_KEY_PPAGE                  315
#define TEXU_KEY_NPAGE                  316
#define TEXU_KEY_ESCAPE                 27
#elif defined WIN32
#define TEXU_KEY_ENTER                  VK_RETURN
#define TEXU_KEY_SELMENU                VK_RETURN
#define TEXU_KEY_NEXTWND                VK_RETURN
#define TEXU_KEY_TAB                    VK_TAB
#define TEXU_KEY_PREVWND                VK_TAB
#define TEXU_KEY_BACKSPACE              VK_BACK
#define TEXU_KEY_IC                     VK_INSERT
#define TEXU_KEY_DC                     VK_DELETE
#define TEXU_KEY_LEFT                   VK_LEFT
#define TEXU_KEY_RIGHT                  VK_RIGHT
#define TEXU_KEY_DOWN                   VK_DOWN
#define TEXU_KEY_UP                     VK_UP
#define TEXU_KEY_PPAGE                  VK_PRIOR
#define TEXU_KEY_NPAGE                  VK_NEXT
#define TEXU_KEY_ESCAPE                 VK_ESCAPE
#else
#define TEXU_KEY_ENTER                  0
#define TEXU_KEY_SELMENU                0
#define TEXU_KEY_NEXTWND                0
#define TEXU_KEY_TAB                    0
#define TEXU_KEY_PREVWND                0
#define TEXU_KEY_BACKSPACE              0
#define TEXU_KEY_IC                     0
#define TEXU_KEY_DC                     0
#define TEXU_KEY_LEFT                   0
#define TEXU_KEY_RIGHT                  0
#define TEXU_KEY_DOWN                   0
#define TEXU_KEY_UP                     0
#define TEXU_KEY_PPAGE                  0
#define TEXU_KEY_NPAGE                  0
#define TEXU_KEY_ESCAPE                 0
#endif

/*simply defines*/
#define TEXU_KEY_PGUP                   TEXU_KEY_PPAGE
#define TEXU_KEY_PGDOWN                 TEXU_KEY_NPAGE
#define TEXU_KEY_INSERT                 TEXU_KEY_IC
#define TEXU_KEY_DELETE                 TEXU_KEY_DC


#if defined __USE_TERMBOX2__ // TEXU_KEY_F1 && (mod & TB_MOD_SHIFT)
#define TEXU_KEY_MOD_ALT                TB_MOD_ALT
#define TEXU_KEY_MOD_CTRL               TB_MOD_CTRL
#define TEXU_KEY_MOD_SHIFT              TB_MOD_SHIFT
#define TEXU_KEY_F1                     TB_KEY_F1 
#define TEXU_KEY_F2                     TB_KEY_F2 
#define TEXU_KEY_F3                     TB_KEY_F3 
#define TEXU_KEY_F4                     TB_KEY_F4 
#define TEXU_KEY_F5                     TB_KEY_F5 
#define TEXU_KEY_F6                     TB_KEY_F6 
#define TEXU_KEY_F7                     TB_KEY_F7 
#define TEXU_KEY_F8                     TB_KEY_F8 
#define TEXU_KEY_F9                     TB_KEY_F9 
#define TEXU_KEY_F10                    TB_KEY_F10
#define TEXU_KEY_F11                    TB_KEY_F11
#define TEXU_KEY_F12                    TB_KEY_F12
/* F13 = F1 + TB_MOD_SHIFT*/
/* F14 = F2 + TB_MOD_SHIFT*/
/*...*/
/* F23 = F11 + TB_MOD_SHIFT*/
/* F44 = F12 + TB_MOD_SHIFT*/
#define TEXU_KEY_USER_FIRST             (100)

#define TEXU_KEY_F13                    (0xffff - (TEXU_KEY_USER_FIRST + 0 ))
#define TEXU_KEY_F14                    (0xffff - (TEXU_KEY_USER_FIRST + 1 ))
#define TEXU_KEY_F15                    (0xffff - (TEXU_KEY_USER_FIRST + 2 ))
#define TEXU_KEY_F16                    (0xffff - (TEXU_KEY_USER_FIRST + 3 ))
#define TEXU_KEY_F17                    (0xffff - (TEXU_KEY_USER_FIRST + 4 ))
#define TEXU_KEY_F18                    (0xffff - (TEXU_KEY_USER_FIRST + 5 ))
#define TEXU_KEY_F19                    (0xffff - (TEXU_KEY_USER_FIRST + 6 ))
#define TEXU_KEY_F20                    (0xffff - (TEXU_KEY_USER_FIRST + 7 ))
#define TEXU_KEY_F21                    (0xffff - (TEXU_KEY_USER_FIRST + 8 ))
#define TEXU_KEY_F22                    (0xffff - (TEXU_KEY_USER_FIRST + 9 ))
#define TEXU_KEY_F23                    (0xffff - (TEXU_KEY_USER_FIRST + 10))
#define TEXU_KEY_F24                    (0xffff - (TEXU_KEY_USER_FIRST + 11))
#else
#define TEXU_KEY_MOD_ALT                1
#define TEXU_KEY_MOD_CTRL               2
#define TEXU_KEY_MOD_SHIFT              4
#define TEXU_KEY_F1                     TVK_F1 
#define TEXU_KEY_F2                     TVK_F2 
#define TEXU_KEY_F3                     TVK_F3 
#define TEXU_KEY_F4                     TVK_F4 
#define TEXU_KEY_F5                     TVK_F5 
#define TEXU_KEY_F6                     TVK_F6 
#define TEXU_KEY_F7                     TVK_F7 
#define TEXU_KEY_F8                     TVK_F8 
#define TEXU_KEY_F9                     TVK_F9 
#define TEXU_KEY_F10                    TVK_F10
#define TEXU_KEY_F11                    TVK_F11
#define TEXU_KEY_F12                    TVK_F12
#define TEXU_KEY_F13                    TVK_F13
#define TEXU_KEY_F14                    TVK_F14
#define TEXU_KEY_F15                    TVK_F15
#define TEXU_KEY_F16                    TVK_F16
#define TEXU_KEY_F17                    TVK_F17
#define TEXU_KEY_F18                    TVK_F18
#define TEXU_KEY_F19                    TVK_F19
#define TEXU_KEY_F20                    TVK_F20
#define TEXU_KEY_F21                    TVK_F21
#define TEXU_KEY_F22                    TVK_F22
#define TEXU_KEY_F23                    TVK_F23
#define TEXU_KEY_F24                    TVK_F24
#endif

#define TEXU_KEYPRESSED_ALT             1
#define TEXU_KEYPRESSED_CTRL            2
#define TEXU_KEYPRESSED_SHIFT           4

#ifdef _USE_CURSES_
#define TEXU_ACS_PLMINUS                ACS_PLMINUS
#else
#if (defined WIN32 && defined UNICODE)
#define TEXU_ACS_PLUS                   L'+'
#define TEXU_ACS_MINUS                  L'-'
#define TEXU_EMPTY_STR                  L""
#else
#define TEXU_ACS_PLUS                   '+'
#define TEXU_ACS_MINUS                  '-'
#define TEXU_EMPTY_STR                  ""
#endif
#endif

#define TEXU_ENV_MAX_FRAMES            256

#if (defined WIN32 && (defined UNICODE || defined _UNICODE))
#define TEXU_MSGBOX_CLASS               L"TexuMsgBoxClassW"
#define TEXU_DESKTOP_CLASS              L"TexuDesktopClassW"
#define TEXU_LABEL_CLASS                L"TexuLabelClassW"
#define TEXU_PANEL_CLASS                L"TexuPanelClassW"
#define TEXU_BUTTON_CLASS               L"TexuButtonClassW"
#define TEXU_STATUSBAR_CLASS            L"TexuStatusBarClassW"
#define TEXU_EDIT_CLASS                 L"TexuEditClassW"
#define TEXU_LISTBOX_CLASS              L"TexuListBoxClassW"
#define TEXU_COMBOBOX_CLASS             L"TexuComboBoxClassW"
#define TEXU_LISTCTRL_CLASS             L"TexuListCtrlClassW"
#define TEXU_TREECTRL_CLASS             L"TexuTreeCtrlClassW"
#define TEXU_UPDOWNCTRL_CLASS           L"TexuUpDownCtrlClassW"
#define TEXU_PROGRESSBAR_CLASS          L"TexuProgressBarClassW"
#define TEXU_PAGECTRL_CLASS             L"TexuPageCtrlClassW"
#define TEXU_TEXTCTRL_CLASS             L"TexuTextCtrlClassW"
#define TEXU_REBAR_CLASS                L"TexuReBarClassW"
#define TEXU_IPADDRESSCTRL_CLASS        L"TexuIPAddressCtrlClassW"
#define TEXU_EDITMASKCTRL_CLASS         L"TexuEditMaskCtrlClassW"
#define TEXU_EDITPRICESPREADCTRL_CLASS  L"TexuEditPriceSpreadCtrlClassW"
#define TEXU_CLOCKCTRL_CLASS            L"TexuClockCtrlClassW"
#define TEXU_DATECTRL_CLASS             L"TexuDateCtrlClassW"
#define TEXU_TIMECTRL_CLASS             L"TexuTimeCtrlClassW"
#define TEXU_EDITVOLUMECTRL_CLASS       L"TexuEditVolumeCtrlClassW"
/*menu*/
#define TEXU_MENU_CLASS                 L"TexuMenuClassW"
#define TEXU_MENUWND_CLASS              L"TexuMenuWndClassW"
#define TEXU_POPUPMENUWND_CLASS         "TexuPopupMenuWndClassW"
#else
#define TEXU_MSGBOX_CLASS               "TexuMsgBoxClassA"
#define TEXU_DESKTOP_CLASS              "TexuDesktopClassA"
#define TEXU_LABEL_CLASS                "TexuLabelClassA"
#define TEXU_PANEL_CLASS                "TexuPanelClassA"
#define TEXU_BUTTON_CLASS               "TexuButtonClassA"
#define TEXU_STATUSBAR_CLASS            "TexuStatusBarClassA"
#define TEXU_EDIT_CLASS                 "TexuEditClassA"
#define TEXU_LISTBOX_CLASS              "TexuListBoxClassA"
#define TEXU_COMBOBOX_CLASS             "TexuComboBoxClassA"
#define TEXU_LISTCTRL_CLASS             "TexuListCtrlClassA"
#define TEXU_TREECTRL_CLASS             "TexuTreeCtrlClassA"
#define TEXU_UPDOWNCTRL_CLASS           "TexuUpDownCtrlClassA"
#define TEXU_PROGRESSBAR_CLASS          "TexuProgressBarClassA"
#define TEXU_PAGECTRL_CLASS             "TexuPageCtrlClassA"
#define TEXU_TEXTCTRL_CLASS             "TexuTextCtrlClassA"
#define TEXU_REBAR_CLASS                "TexuReBarClassA"
#define TEXU_IPADDRESSCTRL_CLASS        "TexuIPAddressCtrlClassA"
#define TEXU_EDITMASKCTRL_CLASS         "TexuEditMaskCtrlClassA"
#define TEXU_EDITPRICESPREADCTRL_CLASS  "TexuEditPriceSpreadCtrlClassA"
#define TEXU_CLOCKCTRL_CLASS            "TexuClockCtrlClassA"
#define TEXU_DATECTRL_CLASS             "TexuDateCtrlClassA"
#define TEXU_TIMECTRL_CLASS             "TexuTimeCtrlClassA"
#define TEXU_EDITVOLUMECTRL_CLASS       "TexuEditVolumeCtrlClassA"
/*menu*/
#define TEXU_MENU_CLASS                 "TexuMenuClassA"
#define TEXU_MENUWND_CLASS              "TexuMenuWndClassA"
#define TEXU_POPUPMENUWND_CLASS         "TexuPopupMenuWndClassA"
#endif


/* window styles */
#define TEXU_WS_LEFT                    0x00000001          /* shared controls style */
#define TEXU_WS_CENTER                  0x00000002
#define TEXU_WS_RIGHT                   0x00000004
#define TEXU_WS_DISABLED                0x00000008          /* disable when created */
#define TEXU_WS_HIDE                    0x00000010          /* hide when created */
#define TEXU_WS_BORDER                  0x00000020          /* draw border */
#define TEXU_WS_CLIPWINDOW              0x00000040          /* clip window if it is out-of-bound of parent window*/
#define TEXU_WS_POPUP                   0x00000080          /* allow window to display out area of the parent window*/
#define TEXU_WS_TOP                     0x00000100          /* shared controls style */
#define TEXU_WS_VCENTER                 0x00000200
#define TEXU_WS_BOTTOM                  0x00000400

#define TEXU_ALIGN_LEFT                TEXU_WS_LEFT
#define TEXU_ALIGN_CENTER              TEXU_WS_CENTER
#define TEXU_ALIGN_RIGHT               TEXU_WS_RIGHT
#define TEXU_ALIGN_TOP                 TEXU_WS_TOP
#define TEXU_ALIGN_VCENTER             TEXU_WS_VCENTER
#define TEXU_ALIGN_BOTTOM              TEXU_WS_BOTTOM

#define TEXU_IDOK                      0x00000001
#define TEXU_IDYES                     0x00000002
#define TEXU_IDNO                      0x00000004
#define TEXU_IDCANCEL                  0x00000008
#define TEXU_IDOKCANCEL                (TEXU_IDOK  | TEXU_IDCANCEL)
#define TEXU_IDYESNO                   (TEXU_IDYES | TEXU_IDNO)
#define TEXU_IDYESNOCANCEL             (TEXU_IDYES | TEXU_IDNO | TEXU_IDCANCEL)

/* MSGBOX */
#define TEXU_MBS_OK                    0x00010000
#define TEXU_MBS_CANCEL                0x00020000
#define TEXU_MBS_YES                   0x00040000
#define TEXU_MBS_NO                    0x00080000
#define TEXU_MBS_OKCANCEL              (TEXU_MBS_OK  | TEXU_MBS_CANCEL)
#define TEXU_MBS_YESNO                 (TEXU_MBS_YES | TEXU_MBS_NO)
#define TEXU_MBS_YESNOCANCEL           (TEXU_MBS_YES | TEXU_MBS_NO | TEXU_MBS_CANCEL)

/*LABEL*/
#define TEXU_LS_FRAME                  0x00010000

/* EDIT */
#define TEXU_ES_LEFT                    TEXU_WS_LEFT
#define TEXU_ES_CENTER                  TEXU_WS_CENTER
#define TEXU_ES_RIGHT                   TEXU_WS_RIGHT
#define TEXU_ES_AUTOHSCROLL             0x00010000
#define TEXU_ES_APPENDMODE              0x00020000
#define TEXU_ES_PASSWORD                0x00040000
#define TEXU_ES_NUMBER                  0x00080000
#define TEXU_ES_DECIMAL                 0x00100000
#define TEXU_ES_AUTODECIMALCOMMA        0x00200000
#define TEXU_ES_UPPERCASE               0x00400000
#define TEXU_ES_LOWERCASE               0x00800000
#define TEXU_ES_A2Z                     0x01000000
#define TEXU_ES_AUTOCOMMAS              0x02000000
#define TEXU_ES_AUTODECIMALCOMMAS       TEXU_ES_AUTODECIMALCOMMA


/* LISTBOX */
#define TEXU_LBS_CHECKBOX               0x00010000
#define TEXU_LBS_RADIOBOX               0x00020000
#define TEXU_LBS_OWNERCOLOR             0x00040000

#define TEXU_LB_UNCHECKED               0
#define TEXU_LB_CHECKED                 1

#define TEXU_LB_OK                      0
#define TEXU_LB_ERROR                   -1

/* LIST CTRL */
#define TEXU_LCS_EDITABLE               0x00010000
#define TEXU_LCS_NOHEADER               0x00020000
#define TEXU_LCS_NOBORDER               0x00040000
#define TEXU_LCS_NOSELECTION            0x00080000
#define TEXU_LCS_LINEEDIT               0x00100000
#define TEXU_LCS_SIMPLEHEADER           0x00200000
/*enum*/
#define TEXU_LC_ENDEDITOK               0x00000000
#define TEXU_LC_ENDEDITCANCEL           0x00000001
/*states*/
#define TEXU_LCT_EDITING                0x00000001
#define TEXU_LCT_VIEW                   0x00000002
#define TEXU_LCT_MOVINGCURSOR           0x00000003
#define TEXU_LCT_INSERTING              0x00000004
#define TEXU_LCT_ENDINSERTING           0x00000005
#define TEXU_LCT_BEGINEDITING           0x00000006
#define TEXU_LCT_ENDMOVING              0x00000007
#define TEXU_LCT_ENDEDITING             0x00000008

/*cell format*/
#define TEXU_LCFM_TEXT                  0x0001
#define TEXU_LCFM_DATA                  0x0002
#define TEXU_LCFM_COLOR                 0x0004
#define TEXU_LCFM_ALL                   (TEXU_LCFM_TEXT | TEXU_LCFM_DATA | TEXU_LCFM_COLOR)


/*TREE CTRL*/
#define TEXU_TCS_SHOWNODE               0x00000001
#define TEXU_TCS_NOHIGHLIGHT            0x00000002
#define TEXU_TCS_FULLSECROW             0x00000004
#define TEXU_TCS_EDITABLE               0x00000008
/*enum*/
#define TEXU_TPO_PRE                    -1
#define TEXU_TPO_IN                     0
#define TEXU_TPO_POST                   1

/*states*/
#define TEXU_TCT_EDITING                0x00000001
#define TEXU_TCT_VIEW                   0x00000002
#define TEXU_TCT_MOVINGCURSOR           0x00000003
#define TEXU_TCT_INSERTING              0x00000004
#define TEXU_TCT_ENDINSERTING           0x00000005
#define TEXU_TCT_BEGINEDITING           0x00000006
#define TEXU_TCT_ENDMOVING              0x00000007
#define TEXU_TCT_ENDEDITING             0x00000008

/*REBAR STYLES*/
#define TEXU_RBS_NOCAPTION              0x00010000
#define TEXU_RBS_HASUNIT                0x00020000
#define TEXU_RBS_HIGHLIGHT              0x00040000
#define TEXU_RBS_TWOCOLUMNS             0x00080000

/*rebar band type*/
#define TEXU_RBT_WINDOW                 1
#define TEXU_RBT_LINEBREAK              0
#define TEXU_RBT_SENTINEL               -1

/*EDIT PRICE SPREAD CTRL*/
#define TEXU_EPSS_SHOWCHANGE            0x00010000
#define TEXU_EPSS_SHOWCHANGEPRICE       0x00020000
#define TEXU_EPSS_AUTOCOMMAS            0x00040000
#define TEXU_EPSS_ATOATC                0x00080000
#define TEXU_EPSS_NOSPREAD              0x00100000

/*EDIT VOLUME CONTROL*/
#define TEXU_EVS_AUTOCOMMAS             0x00010000

/*UP-DOWN CTRL*/
#define TEXU_UDS_SHOWPLUSMINUS          0x00010000
#define TEXU_UDS_SHOWSHOWCOMMAS         0x00020000

/*PAGE CTRL*/
#define TEXU_PCS_CREATEPAGEWHENACTIVATED    0x00010000

/*TIME CTRL*/
#define TEXU_TMFMT_HHMMSS               0
#define TEXU_TMFMT_HHMM                 1


/*DATE CTRL*/
#define TEXU_DTFMT_YYYYMMDD             0
#define TEXU_DTFMT_DDMMYYYY             1

/* PANEL */
#define TEXU_PNS_TITLE                  0x00010000
#define TEXU_PNS_BORDER                 0x00020000
#define TEXU_PNS_STATUS                 0x00040000

/* PROGRESSBAR */
#define TEXU_PGBS_SHOWPERCENT           0x00010000

#endif /*_TEXUCONST_H_*/
