/*
#
# File Name: texutty.h
# Author: Seree Rakwong
# Date: 09-NOV-2022
#
*/

#ifndef _TEXUTTY_H_
#define _TEXUTTY_H_

#include "texutypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * http://ascii-table.com/ansi-escape-sequences-vt-100.php
 * https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
 * https://en.wikipedia.org/wiki/ANSI_escape_code
 */
/* tty colors */
#define TTY_COLOR_DEFAULT             "\033[39;49m"
#define TTY_COLOR_BLACK               "\033[30;40m"
#define TTY_COLOR_RED                 "\033[31;41m"
#define TTY_COLOR_GREEN               "\033[32;42m"
#define TTY_COLOR_YELLOW              "\033[33;43m"
#define TTY_COLOR_BLUE                "\033[34;44m"
#define TTY_COLOR_MAGENTA             "\033[35;45m"
#define TTY_COLOR_CYAN                "\033[36;46m"
#define TTY_COLOR_WHITE               "\033[37;47m"
#define TTY_COLOR_BRIGHT_BLACK        "\033[90;100m"
#define TTY_COLOR_BRIGHT_RED          "\033[91;101m"
#define TTY_COLOR_BRIGHT_GREEN        "\033[92;102m"
#define TTY_COLOR_BRIGHT_YELLOW       "\033[93;103m"
#define TTY_COLOR_BRIGHT_BLUE         "\033[94;104m"
#define TTY_COLOR_BRIGHT_MAGENTA      "\033[95;105m"
#define TTY_COLOR_BRIGHT_CYAN         "\033[96;106m"
#define TTY_COLOR_BRIGHT_WHITE        "\033[97;107m"

#define TTY_MAKECOLOR(fg, bg)         (((fg)&0xff) | (((bg)&0xff)<<16))
#define TTY_FGCOLOR(cl)               ((cl)&0xff)
#define TTY_BGCOLOR(cl)               (((cl)>>16)&0xff)
#define TTY_FOREGROUND_BLACK          30
#define TTY_FOREGROUND_RED            31
#define TTY_FOREGROUND_GREEN          32
#define TTY_FOREGROUND_YELLOW         33
#define TTY_FOREGROUND_BLUE           34
#define TTY_FOREGROUND_MAGENTA        35
#define TTY_FOREGROUND_CYAN           36
#define TTY_FOREGROUND_WHITE          37
#define TTY_BACKGROUND_BLACK          40
#define TTY_BACKGROUND_RED            41
#define TTY_BACKGROUND_GREEN          42
#define TTY_BACKGROUND_YELLOW         43
#define TTY_BACKGROUND_BLUE           44
#define TTY_BACKGROUND_MAGENTA        45
#define TTY_BACKGROUND_CYAN           46
#define TTY_BACKGROUND_WHITE          47
#define TTY_FMT_COLOR_LL              "\033[%d;%dm"

#define TTY_COLOR_BLACK_BLACK         "\033[30;40m"
#define TTY_COLOR_BLACK_RED           "\033[30;41m"
#define TTY_COLOR_BLACK_GREEN         "\033[30;42m"
#define TTY_COLOR_BLACK_YELLOW        "\033[30;43m"
#define TTY_COLOR_BLACK_BLUE          "\033[30;44m"
#define TTY_COLOR_BLACK_MAGENTA       "\033[30;45m"
#define TTY_COLOR_BLACK_CYAN          "\033[30;46m"
#define TTY_COLOR_BLACK_WHITE         "\033[30;47m"
#define TTY_COLOR_RED_BLACK           "\033[31;40m"
#define TTY_COLOR_RED_RED             "\033[31;41m"
#define TTY_COLOR_RED_GREEN           "\033[31;42m"
#define TTY_COLOR_RED_YELLOW          "\033[31;43m"
#define TTY_COLOR_RED_BLUE            "\033[31;44m"
#define TTY_COLOR_RED_MAGENTA         "\033[31;45m"
#define TTY_COLOR_RED_CYAN            "\033[31;46m"
#define TTY_COLOR_RED_WHITE           "\033[31;47m"
#define TTY_COLOR_GREEN_BLACK         "\033[32;40m"
#define TTY_COLOR_GREEN_RED           "\033[32;41m"
#define TTY_COLOR_GREEN_GREEN         "\033[32;42m"
#define TTY_COLOR_GREEN_YELLOW        "\033[32;43m"
#define TTY_COLOR_GREEN_BLUE          "\033[32;44m"
#define TTY_COLOR_GREEN_MAGENTA       "\033[32;45m"
#define TTY_COLOR_GREEN_CYAN          "\033[32;46m"
#define TTY_COLOR_GREEN_WHITE         "\033[32;47m"
#define TTY_COLOR_YELLOW_BLACK        "\033[33;40m"
#define TTY_COLOR_YELLOW_RED          "\033[33;41m"
#define TTY_COLOR_YELLOW_GREEN        "\033[33;42m"
#define TTY_COLOR_YELLOW_YELLOW       "\033[33;43m"
#define TTY_COLOR_YELLOW_BLUE         "\033[33;44m"
#define TTY_COLOR_YELLOW_MAGENTA      "\033[33;45m"
#define TTY_COLOR_YELLOW_CYAN         "\033[33;46m"
#define TTY_COLOR_YELLOW_WHITE        "\033[33;47m"
#define TTY_COLOR_BLUE_BLACK          "\033[34;40m"
#define TTY_COLOR_BLUE_RED            "\033[34;41m"
#define TTY_COLOR_BLUE_GREEN          "\033[34;42m"
#define TTY_COLOR_BLUE_YELLOW         "\033[34;43m"
#define TTY_COLOR_BLUE_BLUE           "\033[34;44m"
#define TTY_COLOR_BLUE_MAGENTA        "\033[34;45m"
#define TTY_COLOR_BLUE_CYAN           "\033[34;46m"
#define TTY_COLOR_BLUE_WHITE          "\033[34;47m"
#define TTY_COLOR_MAGENTA_BLACK       "\033[35;40m"
#define TTY_COLOR_MAGENTA_RED         "\033[35;41m"
#define TTY_COLOR_MAGENTA_GREEN       "\033[35;42m"
#define TTY_COLOR_MAGENTA_YELLOW      "\033[35;43m"
#define TTY_COLOR_MAGENTA_BLUE        "\033[35;44m"
#define TTY_COLOR_MAGENTA_MAGENTA     "\033[35;45m"
#define TTY_COLOR_MAGENTA_CYAN        "\033[35;46m"
#define TTY_COLOR_MAGENTA_WHITE       "\033[35;47m"
#define TTY_COLOR_CYAN_BLACK          "\033[36;40m"
#define TTY_COLOR_CYAN_RED            "\033[36;41m"
#define TTY_COLOR_CYAN_GREEN          "\033[36;42m"
#define TTY_COLOR_CYAN_YELLOW         "\033[36;43m"
#define TTY_COLOR_CYAN_BLUE           "\033[36;44m"
#define TTY_COLOR_CYAN_MAGENTA        "\033[36;45m"
#define TTY_COLOR_CYAN_CYAN           "\033[36;46m"
#define TTY_COLOR_CYAN_WHITE          "\033[36;47m"
#define TTY_COLOR_WHITE_BLACK         "\033[37;40m"
#define TTY_COLOR_WHITE_RED           "\033[37;41m"
#define TTY_COLOR_WHITE_GREEN         "\033[37;42m"
#define TTY_COLOR_WHITE_YELLOW        "\033[37;43m"
#define TTY_COLOR_WHITE_BLUE          "\033[37;44m"
#define TTY_COLOR_WHITE_MAGENTA       "\033[37;45m"
#define TTY_COLOR_WHITE_CYAN          "\033[37;46m"
#define TTY_COLOR_WHITE_WHITE         "\033[37;47m"
#define TTY_COLOR_BRIGHT_WHITE_BLACK        "\033[38;5;15m\033[48;5;0m"
#define TTY_COLOR_BRIGHT_WHITE_RED          "\033[38;5;15m\033[48;5;1m"
#define TTY_COLOR_BRIGHT_WHITE_GREEN        "\033[38;5;15m\033[48;5;2m"
#define TTY_COLOR_BRIGHT_WHITE_YELLOW       "\033[38;5;15m\033[48;5;3m"
#define TTY_COLOR_BRIGHT_WHITE_BLUE         "\033[38;5;15m\033[48;5;4m"
#define TTY_COLOR_BRIGHT_WHITE_MAGENTA      "\033[38;5;15m\033[48;5;5m"
#define TTY_COLOR_BRIGHT_WHITE_CYAN         "\033[38;5;15m\033[48;5;6m"
#define TTY_COLOR_BRIGHT_WHITE_WHITE        "\033[38;5;15m\033[48;5;7m"
#define TTY_COLOR_BLACK_BRIGHT_WHITE        "\033[38;5;0m\033[48;5;15m"
#define TTY_COLOR_RED_BRIGHT_WHITE          "\033[38;5;1m\033[48;5;15m"
#define TTY_COLOR_GREEN_BRIGHT_WHITE        "\033[38;5;2m\033[48;5;15m"
#define TTY_COLOR_YELLOW_BRIGHT_WHITE       "\033[38;5;3m\033[48;5;15m"
#define TTY_COLOR_BLUE_BRIGHT_WHITE         "\033[38;5;4m\033[48;5;15m"
#define TTY_COLOR_MAGENTA_BRIGHT_WHITE      "\033[38;5;5m\033[48;5;15m"
#define TTY_COLOR_CYAN_BRIGHT_WHITE         "\033[38;5;6m\033[48;5;15m"





#define TTY_SET_FGCOLOR(r, g, b)           "\033[38;2;r;g;bm"
#define TTY_SET_BGCOLOR(r, g, b)           "\033[48;2;r;g;bm"

/* tty attributes */
#define TTY_ATTR_OFF                  "\033[0m"
#define TTY_ATTR_BOLD                 "\033[1m"
#define TTY_ATTR_LOW_INTENSITY        "\033[2m"
#define TTY_ATTR_ITALIC               "\033[3m"
#define TTY_ATTR_UNDERLINE            "\033[4m"
#define TTY_ATTR_BLINK                "\033[5m"
#define TTY_ATTR_REVERSE              "\033[7m"
#define TTY_ATTR_INVISIBLE            "\033[8m"
/* dynamic tty attributes */
#define TTY_FMT_ATTR_L                "\033[%dm"
/* dynamic tty color */
#define TTY_FMT_CURSOR_LL             "\033[%d;%dH"
#define TTY_GOTOYX_LL                 "\033[%d;%dH"

#define TTY_GOTO_LINE24               "\033[0;1m\033[24;H\033[K"   /* Set position at line 24 */
#define TTY_SET_APPKEYS               "\033[="                    /* Set keypad to application key*/
#define TTY_SET_NUMKEYS               "\033[>"                    /* Set keypad to numeric key */
#define TTY_SET_NOSCROLL              "\033[?7l"                 /* Set no scrolling */
#define TTY_ON_LINE25                 "\033[2$~\0331$}"            /* Set line 25 to use */
#define TTY_OFF_LINE25                "\033[0$}"                  /* Reset line 25 */
#define TTY_OFF_CURSOR                "\033[?25l"                /* Set cursor off */
#define TTY_ON_CURSOR                 "\033[?25h"                /* Set cursor on */
#define TTY_GET_CURPOS                "\033[\0336n"                   /* Get cursor position */

#define TTY_CLEAR_LINE                "\033[2K"                   /* Clear entire line */
#define TTY_CLEAR_SCREEN              "\033[2J"                   /* Clear entire scren */
#define TTY_SAVE_CURPOS               "\033[[s"                   /* Save Cursor Position */
#define TTY_RESTORE_CURPOS            "\033[[u"                   /* Restore Cursor Position */

#define TTY_SET_MODE_40X25_MONO_TEXT            "\033[=0h"
#define TTY_SET_MODE_40X25_COLOR_TEXT           "\033[=1h"
#define TTY_SET_MODE_80X25_MONO_TEXT            "\033[=2h"
#define TTY_SET_MODE_80X25_COLOR_TEXT           "\033[=3h"
#define TTY_SET_MODE_320X200_4COLOR_GRAPHIC     "\033[=4h"
#define TTY_SET_MODE_320X200_MONO_GRAPHIC       "\033[=5h"
#define TTY_SET_MODE_640X200_MONO_GRAPHIC       "\033[=6h"
#define TTY_SET_MODE_LINE_WRAPPING              "\033[=7h"
#define TTY_SET_MODE_320X200_COLOR_GRAPHIC      "\033[=13h"
#define TTY_SET_MODE_640X200_16COLOR_GRAPHIC    "\033[=14h"
#define TTY_SET_MODE_640X350_MONO_GRAPHIC       "\033[=15h"
#define TTY_SET_MODE_640X350_16COLOR_GRAPHIC    "\033[=16h"
#define TTY_SET_MODE_640X480_MONO_GRAPHIC       "\033[=17h"
#define TTY_SET_MODE_640X480_16COLOR_GRAPHIC    "\033[=18h"
#define TTY_SET_MODE_320X200_256COLOR_GRAPHIC   "\033[=19h"

#define TTY_RESET_MODE_40X25_MONO_TEXT            "\033[=0l"
#define TTY_RESET_MODE_40X25_COLOR_TEXT           "\033[=1l"
#define TTY_RESET_MODE_80X25_MONO_TEXT            "\033[=2l"
#define TTY_RESET_MODE_80X25_COLOR_TEXT           "\033[=3l"
#define TTY_RESET_MODE_320X200_4COLOR_GRAPHIC     "\033[=4l"
#define TTY_RESET_MODE_320X200_MONO_GRAPHIC       "\033[=5l"
#define TTY_RESET_MODE_640X200_MONO_GRAPHIC       "\033[=6l"
#define TTY_RESET_MODE_LINE_WRAPPING              "\033[=7l"
#define TTY_RESET_MODE_320X200_COLOR_GRAPHIC      "\033[=13l"
#define TTY_RESET_MODE_640X200_16COLOR_GRAPHIC    "\033[=14l"
#define TTY_RESET_MODE_640X350_MONO_GRAPHIC       "\033[=15l"
#define TTY_RESET_MODE_640X350_16COLOR_GRAPHIC    "\033[=16l"
#define TTY_RESET_MODE_640X480_MONO_GRAPHIC       "\033[=17l"
#define TTY_RESET_MODE_640X480_16COLOR_GRAPHIC    "\033[=18l"
#define TTY_RESET_MODE_320X200_256COLOR_GRAPHIC   "\033[=19l"

#if defined __REFLECTION__ || defined __HALTERM__
#define TTY_PRNSCR                    = "?i"
#define TTY_PRNLINE                   = "?1i"
#define TTY_STOP_PRN                  = "?4i"
#define TTY_START_PRN                 = "?5i"
#elif defined __USE_VMS__
#define TTY_PRNSCR                    = "\033[i"
#define TTY_PRNLINE                   = "\033[1i"
#define TTY_STOP_PRN                  = "\033[4i"
#define TTY_START_PRN                 = "\033[5i"
#endif


/* functions */
texu_i32    texu_tty_init();
texu_i32    texu_tty_release();
texu_i32    texu_tty_getch(texu_i32 sec, texu_i32 usec);
texu_i32    texu_tty_echo();
texu_i32    texu_tty_noecho();
void        texu_tty_init_colors();
texu_i32    texu_tty_clear();
texu_i32    texu_tty_clearln(texu_i32 y);
texu_i32    texu_tty_get_color(texu_i32 clridx);
texu_i32    texu_tty_gotoyx(texu_i32 y, texu_i32 x);
texu_i32    texu_tty_attron(texu_i32 attrs);
texu_i32    texu_tty_attroff();
texu_i32    texu_tty_putch(texu_i32 y, texu_i32 x, texu_i32 ch);
texu_i32    texu_tty_putch_attr(texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs);
texu_i32    texu_tty_putstr(texu_i32 y, texu_i32 x, const texu_char *str);
texu_i32    texu_tty_putstr_attr(texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs);
texu_i32    texu_tty_get_underline(texu_i32 clridx);
texu_i32    texu_tty_get_blink(texu_i32 clridx);
texu_i32    texu_tty_get_italic(texu_i32 clridx);
texu_i32    texu_tty_get_reverse(texu_i32 clridx);

#ifdef __cplusplus
}
#endif

#endif /*_TEXUTTY_H_*/
