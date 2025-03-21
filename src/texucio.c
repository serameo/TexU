/*
#
# File Name: texuio.c
# Author: Seree Rakwong
# Date: 14-MAY-2020
#
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texulib.h"
#include "texutils.h"
#include "texucio.h"
#include "texui.h"
#include "texuconst.h"

#if 1 /*(defined __USE_TTY__ || defined __VMS__)*/
#include "texutty.h"
#endif
#if (defined WIN32)
#include <windows.h>
#endif

#if (defined __USE_TERMIOS__ || defined __USE_TERMBOX2__)
#include "texutermios.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
# TexU io object
#
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
*/
enum
{
    BLACK_BLACK = TEXU_CIO_COLOR_BLACK_BLACK,      /*0*/
    BLACK_RED,
    BLACK_GREEN,
    BLACK_YELLOW,
    BLACK_BLUE,
    BLACK_MAGENTA,
    BLACK_CYAN,
    BLACK_WHITE,
    RED_BLACK,        /*8*/
    RED_RED,
    RED_GREEN,
    RED_YELLOW,
    RED_BLUE,
    RED_MAGENTA,
    RED_CYAN,
    RED_WHITE,
    GREEN_BLACK,      /*16*/
    GREEN_RED,
    GREEN_GREEN,
    GREEN_YELLOW,
    GREEN_BLUE,
    GREEN_MAGENTA,
    GREEN_CYAN,
    GREEN_WHITE,
    YELLOW_BLACK,     /*24*/
    YELLOW_RED,
    YELLOW_GREEN,
    YELLOW_YELLOW,
    YELLOW_BLUE,
    YELLOW_MAGENTA,
    YELLOW_CYAN,
    YELLOW_WHITE,
    BLUE_BLACK,       /*32*/
    BLUE_RED,
    BLUE_GREEN,
    BLUE_YELLOW,
    BLUE_BLUE,
    BLUE_MAGENTA,
    BLUE_CYAN,
    BLUE_WHITE,
    MAGENTA_BLACK,    /*40*/
    MAGENTA_RED,
    MAGENTA_GREEN,
    MAGENTA_YELLOW,
    MAGENTA_BLUE,
    MAGENTA_MAGENTA,
    MAGENTA_CYAN,
    MAGENTA_WHITE,
    CYAN_BLACK,       /*56*/
    CYAN_RED,
    CYAN_GREEN,
    CYAN_YELLOW,
    CYAN_BLUE,
    CYAN_MAGENTA,
    CYAN_CYAN,
    CYAN_WHITE,
    WHITE_BLACK,
    WHITE_RED,
    WHITE_GREEN,
    WHITE_YELLOW,
    WHITE_BLUE,
    WHITE_MAGENTA,
    WHITE_CYAN,
    WHITE_WHITE,
    /* no color */
    COLOR_MAX
};

struct _cio_color_s
{
    texu_i32   fg;
    texu_i32   bg;
    texu_i32   rfg;
    texu_i32   rbg;
};
typedef struct _cio_color_s cio_colorpair;
cio_colorpair gcolors[COLOR_MAX];
void cio__init_pair(texu_i32 idx, texu_i32 fg, texu_i32 bg, texu_i32 rfg, texu_i32 rbg)
{
    gcolors[idx].fg = fg;
    gcolors[idx].bg = bg;
    gcolors[idx].rfg = rfg;
    gcolors[idx].rbg = rbg;
}
texu_i32 cio__color_pair(texu_i32 idx)
{
    if (idx < 0 || idx >= COLOR_MAX)
    {
        idx = 0;
    }
#ifdef __USE_VMS__
    return TTY_MAKECOLOR(gcolors[idx].fg, gcolors[idx].bg);
#elif (defined __VMS__)
    return (gcolors[idx].fg);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
    return (gcolors[idx].fg);
#else
#if defined __LINUX__
    return (gcolors[idx].fg | gcolors[idx].bg);
#elif (defined WIN32 && defined _CONSOLE)
    return (gcolors[idx].fg | gcolors[idx].bg);
#else
    return (gcolors[idx].fg);
#endif
#endif
}

texu_i32 cio__bgcolor_pair(texu_i32 idx)
{
    if (idx < 0 || idx >= COLOR_MAX)
    {
        idx = 0;
    }
    return (gcolors[idx].bg);
}

texu_i32 cio__reverse_color_pair(texu_i32 idx)
{
    if (idx < 0 || idx >= COLOR_MAX)
    {
        idx = 0;
    }
#ifdef __USE_VMS__
    return TTY_MAKECOLOR(gcolors[idx].rfg, gcolors[idx].rbg);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
    return gcolors[idx].rfg;
#else
    return (gcolors[idx].rfg | gcolors[idx].rbg);
#endif
}

texu_i32 cio__reverse_bgcolor_pair(texu_i32 idx)
{
    if (idx < 0 || idx >= COLOR_MAX)
    {
        idx = 0;
    }
    return (gcolors[idx].rbg);
}

#if (defined VMS || defined __VMS__)
#include <descrip.h>
#include <descrip.h>
#include <ssdef.h>
#include <smgdef.h>
#include <smg$routines.h>
#include <stsdef.h>
#include <starlet.h>
#include <lib$routines.h>
#include <libdtdef.h>
#include <iodef.h>

struct _smg$r_attribute_info_block 
{
    unsigned int        smg$l_dev_char;         /* Device characteristics           */
    unsigned int        smg$l_dev_depend;       /* Specific characteristics (1)     */
    unsigned int        smg$l_dev_depend2;      /* Specific characteristics (2)     */
    unsigned char       smg$b_dev_class;        /* Device class (e.g. DC$_TERM)     */
    unsigned char       smg$r_dev_overlay;
    unsigned char       smg$b_dev_type;         /* Physical device type (e.g. DT$_VT100) */
    unsigned char       smg$r_row_overlay;
    unsigned short int  smg$w_num_columns;      /* Terminal width                 */
    unsigned short int  smg$r_count_overlay;
    unsigned short int  smg$w_dev_speed;        /* Terminal Speed (+)               */
    unsigned short int  smg$w_dev_fill;         /* Fill characteristics (+)         */
    unsigned short int  smg$w_phys_cursor_row;  /* Row where physical cursor is */
    unsigned short int  smg$w_phys_cursor_col;  /* Col where physical cursor is */
    unsigned int        smg$l_display_id;       /* Display containing phy cursor    */
    unsigned int        smg$l_dev_depend3;      /* Specific characteristics (3)     */
};
typedef struct _smg$r_attribute_info_block smg$r_attribute_info_block;

long texu__cio_get_on_display(texu_cio* cio);
long texu__cio_get_off_display(texu_cio* cio);


#endif

struct texu_cio
{
    texu_env    *env;
#ifdef __USE_TTY__
    FILE    *wndscr;
#elif (defined WIN32 && defined _WINDOWS)
    /*texu_env    *env;*/
#elif (defined WIN32 && defined _CONSOLE)
    /*texu_env* env;*/
    HANDLE win;
    HANDLE wout;
#elif (defined VMS || defined __VMS__)
    long smg_pastebd_id;
    long smg_display_id;
    long smg_keybd_id;
    long smg_offscreens[2];
    long smg_flipflop;  /*only 0 or 1 of sgm_offscreens[]*/
    void(*on_timeout)(texu_cio* cio);
    unsigned long sysin_chan;
    iosb_t iosb;
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
    struct tb_event ev;
    /*texu_env* env;*/
    fn_sigint_callback sigint_handler;
    void *sig_userdata;
#else
    WINDOW  *wndscr;
#endif
};

void _texu_cio_init_colors(texu_cio *cio);
void _texu_cio_init_colors_mono(texu_cio *cio);

#if (defined VMS || defined __VMS__)

int
vms_getint(unsigned long* kbid)
{
  unsigned short key;                        /* key var */

  smg$read_keystroke(kbid, &key);
  return(key);
}

int
vms_getch(unsigned long* kbid)
{
  int c;

  do
  {
    c = vms_getint(kbid);
  } while (c == SS$_NORMAL);

  return c;
}

unsigned long vms_writeio(unsigned long wchannel, iosb_t* iosb, const char* text, unsigned long len)
{
  return sys$qiow(0,
                wchannel,
                (IO$_WRITEVBLK | IO$M_NOFORMAT),
                iosb,
                0, 0,
                text, len,
                0, 0, 0, 0);
}

long vms_get_sysio(unsigned long*  iochan, char* sysio)
{
    long status = SS$_NORMAL;
    
    struct dsc$descriptor_s sys_input;
    memset(&sys_input, 0, sizeof(sys_input));
    sys_input.dsc$a_pointer = sysio;
    sys_input.dsc$w_length  = strlen(sysio);
    
    status = sys$assign(&sys_input, iochan, 0, 0);
    return status;
}

long texu__cio_get_on_display(texu_cio* cio)
{
    return cio->smg_offscreens[cio->smg_flipflop];
}

long texu__cio_get_off_display(texu_cio* cio)
{
    long flipflop = (cio->smg_flipflop ? 0 : 1);
    return cio->smg_offscreens[flipflop];
}

void  texu_cio_flip_display(texu_cio* cio)
{
    /*unpasted*/
    smg$unpaste_virtual_display(&cio->smg_offscreens[cio->smg_flipflop], &cio->smg_pastebd_id);
    cio->smg_flipflop = !cio->smg_flipflop;
    smg$paste_virtual_display(&cio->smg_offscreens[cio->smg_flipflop], &cio->smg_pastebd_id);
}

void texu_cio_begin_update(texu_cio* cio)
{
    smg$begin_display_update(&cio->smg_display_id);
    /*smg$begin_pasteboard_update(&cio->smg_pastebd_id);*/
}
void texu_cio_end_update(texu_cio* cio)
{
    smg$end_display_update(&cio->smg_display_id);
/*    smg$end_pasteboard_update(&cio->smg_pastebd_id);*/
}
#endif

#if (defined WIN32 && defined _WINDOWS)
texu_env
*texu_cio_get_env(texu_cio* cio)
{
    return cio->env;
}
#endif

#if (defined VMS || defined __VMS__)
void texu_cio_set_timeout(texu_cio* cio, void(*on_timeout)(texu_cio*))
{
    cio->on_timeout = on_timeout;
}
#endif



texu_cio *
texu_cio_new()
{
    texu_cio *cio = (texu_cio *)malloc(sizeof(texu_cio));
    if (cio)
    {
        memset(cio, 0, sizeof(texu_cio));
    }
    return cio;
}

void
texu_cio_del(texu_cio *cio)
{
    if (cio)
    {
        texu_cio_release(cio);
        free(cio);
        cio = 0;
    }
}

#if (defined __USE_TTY__ || defined WIN32 || defined VMS || defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_nodelay(texu_cio *cio, texu_i32 delay)
{
    return 0;
}
#else
texu_i32
texu_cio_nodelay(texu_cio *cio, texu_i32 delay)
{
    return nodelay(cio->wndscr, delay);
}
#endif

void
_texu_cio_init_keys(texu_cio *cio)
{
    /*
    define_key ("\033X", Alt_X);
    define_key ("\033x", Alt_x);

    define_key ("\033F1", ALT_F(1));
    */
}

#if defined __LINUX__ && defined __USE_CURSES__
void
_texu_cio_init_colors_mono(texu_cio *cio)
{
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    init_pair(TEXU_CIO_COLOR_BLACK_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_BLACK,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_RED,           COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_GREEN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_YELLOW,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_BLUE,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_MAGENTA,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_CYAN,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_WHITE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_RED,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_BLACK,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_RED,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_GREEN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_BLUE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_CYAN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_RED,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA,   COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_BLACK,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_RED,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_GREEN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_BLUE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_CYAN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_WHITE,       COLOR_WHITE,    COLOR_BLACK);

    /*BRIGHT WHITE*/
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLACK,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_RED,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_GREEN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_YELLOW,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLUE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_MAGENTA,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_CYAN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BLACK_BRIGHT_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_RED_BRIGHT_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_GREEN_BRIGHT_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_YELLOW_BRIGHT_WHITE,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BLUE_BRIGHT_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_MAGENTA_BRIGHT_WHITE,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_CYAN_BRIGHT_WHITE,       COLOR_WHITE,    COLOR_BLACK);
}
void
_texu_cio_init_colors(texu_cio *cio)
{
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    init_pair(TEXU_CIO_COLOR_BLACK_BLACK,   COLOR_BLACK,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_RED,     COLOR_BLACK,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_BLACK_GREEN,   COLOR_BLACK,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,  COLOR_BLACK,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_BLACK_BLUE,    COLOR_BLACK,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA, COLOR_BLACK,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_BLACK_CYAN,    COLOR_BLACK,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_BLACK_WHITE,   COLOR_BLACK,    COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_RED_BLACK,     COLOR_RED,      COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_RED,       COLOR_RED,      COLOR_RED);
    init_pair(TEXU_CIO_COLOR_RED_GREEN,     COLOR_RED,      COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_RED_YELLOW,    COLOR_RED,      COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_RED_BLUE,      COLOR_RED,      COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_RED_MAGENTA,   COLOR_RED,      COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_RED_CYAN,      COLOR_RED,      COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_RED_WHITE,     COLOR_RED,      COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_GREEN_BLACK,   COLOR_GREEN,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_RED,     COLOR_GREEN,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_GREEN_GREEN,   COLOR_GREEN,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,  COLOR_GREEN,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_GREEN_BLUE,    COLOR_GREEN,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA, COLOR_GREEN,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_GREEN_CYAN,    COLOR_GREEN,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_GREEN_WHITE,   COLOR_GREEN,    COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,  COLOR_YELLOW,   COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_RED,    COLOR_YELLOW,   COLOR_RED);
    init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,  COLOR_YELLOW,   COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW, COLOR_YELLOW,   COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,   COLOR_YELLOW,   COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA, COLOR_YELLOW,  COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,   COLOR_YELLOW,   COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,  COLOR_YELLOW,   COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_BLUE_BLACK,    COLOR_BLUE,     COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_RED,      COLOR_BLUE,     COLOR_RED);
    init_pair(TEXU_CIO_COLOR_BLUE_GREEN,    COLOR_BLUE,     COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,   COLOR_BLUE,     COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_BLUE_BLUE,     COLOR_BLUE,     COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,  COLOR_BLUE,     COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_BLUE_CYAN,     COLOR_BLUE,     COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_BLUE_WHITE,    COLOR_BLUE,     COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK, COLOR_MAGENTA,  COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_RED,   COLOR_MAGENTA,  COLOR_RED);
    init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN, COLOR_MAGENTA,  COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,  COLOR_MAGENTA,  COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,  COLOR_MAGENTA,  COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE, COLOR_MAGENTA,  COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_CYAN_BLACK,    COLOR_CYAN,     COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_RED,      COLOR_CYAN,     COLOR_RED);
    init_pair(TEXU_CIO_COLOR_CYAN_GREEN,    COLOR_CYAN,     COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,   COLOR_CYAN,     COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_CYAN_BLUE,     COLOR_CYAN,     COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,  COLOR_CYAN,     COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_CYAN_CYAN,     COLOR_CYAN,     COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_CYAN_WHITE,    COLOR_CYAN,     COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_WHITE_BLACK,   COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_RED,     COLOR_WHITE,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_WHITE_GREEN,   COLOR_WHITE,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,  COLOR_WHITE,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_WHITE_BLUE,    COLOR_WHITE,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA, COLOR_WHITE,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_WHITE_CYAN,    COLOR_WHITE,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_WHITE_WHITE,   COLOR_WHITE,    COLOR_WHITE);

    /*BRIGHT WHITE*/
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLACK,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_RED,    TEXU_CIO_BRIGHT_WHITE_WHITE,      COLOR_RED);
    init_pair(TEXU_CIO_BRIGHT_WHITE_GREEN,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_GREEN);
    init_pair(TEXU_CIO_BRIGHT_WHITE_YELLOW, TEXU_CIO_BRIGHT_WHITE_WHITE,   COLOR_YELLOW);
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLUE,   TEXU_CIO_BRIGHT_WHITE_WHITE,     COLOR_BLUE);
    init_pair(TEXU_CIO_BRIGHT_WHITE_MAGENTA,TEXU_CIO_BRIGHT_WHITE_WHITE,  COLOR_MAGENTA);
    init_pair(TEXU_CIO_BRIGHT_WHITE_CYAN,   TEXU_CIO_BRIGHT_WHITE_WHITE,     COLOR_CYAN);
    init_pair(TEXU_CIO_BRIGHT_WHITE_WHITE,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_WHITE);
    init_pair(TEXU_CIO_BLACK_BRIGHT_WHITE,  COLOR_BLACK,    TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_RED_BRIGHT_WHITE,    COLOR_RED,      TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_GREEN_BRIGHT_WHITE,  COLOR_GREEN,    TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_YELLOW_BRIGHT_WHITE, COLOR_YELLOW,   TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_BLUE_BRIGHT_WHITE,   COLOR_BLUE,     TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_MAGENTA_BRIGHT_WHITE,COLOR_MAGENTA,  TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_CYAN_BRIGHT_WHITE,   COLOR_CYAN,     TEXU_CIO_BRIGHT_WHITE_WHITE);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)

#define FOREGROUND_BLACK            TB_BLACK
#define FOREGROUND_RED              TB_RED
#define FOREGROUND_GREEN            TB_GREEN
#define FOREGROUND_YELLOW           TB_YELLOW
#define FOREGROUND_BLUE             TB_BLUE
#define FOREGROUND_MAGENTA          TB_MAGENTA
#define FOREGROUND_CYAN             TB_CYAN
#define FOREGROUND_WHITE            TB_WHITE
#define BACKGROUND_BLACK            TB_BLACK
#define BACKGROUND_RED              TB_RED
#define BACKGROUND_GREEN            TB_GREEN
#define BACKGROUND_YELLOW           TB_YELLOW
#define BACKGROUND_BLUE             TB_BLUE
#define BACKGROUND_MAGENTA          TB_MAGENTA
#define BACKGROUND_CYAN             TB_CYAN
#define BACKGROUND_WHITE            TB_WHITE

#if (defined __USE_TERMBOX2__)
#endif

void _texu_cio_init_colors_mono(texu_cio * cio)
{
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_RED,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_GREEN,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_CYAN,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_WHITE,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLACK,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_RED,         FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_GREEN,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_YELLOW,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLUE,        FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_MAGENTA,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_CYAN,        FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_RED_WHITE,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_RED,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_GREEN,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_CYAN,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_WHITE,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_RED,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,  FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLACK,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_RED,        FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_GREEN,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLUE,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_CYAN,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_WHITE,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_RED,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,  FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLACK,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_RED,        FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_GREEN,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLUE,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_CYAN,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_WHITE,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_RED,       FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_GREEN,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,    FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_CYAN,      FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_WHITE,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
}

void     _texu_cio_init_colors(texu_cio * cio)
{
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLACK,     FOREGROUND_BLACK,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_RED,       FOREGROUND_BLACK,   BACKGROUND_RED,     FOREGROUND_BLACK,   BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_GREEN,     FOREGROUND_BLACK,   BACKGROUND_GREEN,   FOREGROUND_BLACK,   BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,    FOREGROUND_BLACK,   BACKGROUND_YELLOW,  FOREGROUND_BLACK,   BACKGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLUE,      FOREGROUND_BLACK,   BACKGROUND_BLUE,    FOREGROUND_BLACK,   BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,   FOREGROUND_BLACK,   BACKGROUND_MAGENTA, FOREGROUND_BLACK,   BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_CYAN,      FOREGROUND_BLACK,   BACKGROUND_CYAN,    FOREGROUND_BLACK,   BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_BLACK_WHITE,     FOREGROUND_BLACK,   BACKGROUND_WHITE,   FOREGROUND_BLACK,   BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_RED_BLACK,       FOREGROUND_RED,     BACKGROUND_BLACK,   FOREGROUND_RED,     BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_RED_RED,         FOREGROUND_RED,     BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_RED_GREEN,       FOREGROUND_RED,     BACKGROUND_GREEN,   FOREGROUND_RED,     BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_RED_YELLOW,      FOREGROUND_RED,     BACKGROUND_YELLOW,  FOREGROUND_RED,     BACKGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_RED_BLUE,        FOREGROUND_RED,     BACKGROUND_BLUE,    FOREGROUND_RED,     BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_RED_MAGENTA,     FOREGROUND_RED,     BACKGROUND_MAGENTA, FOREGROUND_RED,     BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_RED_CYAN,        FOREGROUND_RED,     BACKGROUND_CYAN,    FOREGROUND_RED,     BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_RED_WHITE,       FOREGROUND_RED,     BACKGROUND_WHITE,   FOREGROUND_RED,     BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLACK,     FOREGROUND_GREEN,   BACKGROUND_BLACK,   FOREGROUND_GREEN,   BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_RED,       FOREGROUND_GREEN,   BACKGROUND_RED,     FOREGROUND_GREEN,   BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_GREEN,     FOREGROUND_GREEN,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,    FOREGROUND_GREEN,   BACKGROUND_YELLOW,  FOREGROUND_GREEN,   BACKGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLUE,      FOREGROUND_GREEN,   BACKGROUND_BLUE,    FOREGROUND_GREEN,   BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,   FOREGROUND_GREEN,   BACKGROUND_MAGENTA, FOREGROUND_GREEN,   BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_CYAN,      FOREGROUND_GREEN,   BACKGROUND_CYAN,    FOREGROUND_GREEN,   BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_GREEN_WHITE,     FOREGROUND_GREEN,   BACKGROUND_WHITE,   FOREGROUND_GREEN,   BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,    FOREGROUND_YELLOW,  BACKGROUND_BLACK,   FOREGROUND_YELLOW,  BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_RED,      FOREGROUND_YELLOW,  BACKGROUND_RED,     FOREGROUND_YELLOW,  BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,    FOREGROUND_YELLOW,  BACKGROUND_GREEN,   FOREGROUND_YELLOW,  BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,   FOREGROUND_YELLOW,  BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,     FOREGROUND_YELLOW,  BACKGROUND_BLUE,    FOREGROUND_YELLOW,  BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,  FOREGROUND_YELLOW,  BACKGROUND_MAGENTA, FOREGROUND_YELLOW,  BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,     FOREGROUND_YELLOW,  BACKGROUND_CYAN,    FOREGROUND_YELLOW,  BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,    FOREGROUND_YELLOW,  BACKGROUND_WHITE,   FOREGROUND_YELLOW,  BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLACK,      FOREGROUND_BLUE,    BACKGROUND_BLACK,   FOREGROUND_BLUE,    BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_RED,        FOREGROUND_BLUE,    BACKGROUND_RED,     FOREGROUND_BLUE,    BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_GREEN,      FOREGROUND_BLUE,    BACKGROUND_GREEN,   FOREGROUND_BLUE,    BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,     FOREGROUND_BLUE,    BACKGROUND_YELLOW,  FOREGROUND_BLUE,    BACKGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLUE,       FOREGROUND_BLUE,    BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,    FOREGROUND_BLUE,    BACKGROUND_MAGENTA, FOREGROUND_BLUE,    BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_CYAN,       FOREGROUND_BLUE,    BACKGROUND_CYAN,    FOREGROUND_BLUE,    BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_BLUE_WHITE,      FOREGROUND_BLUE,    BACKGROUND_WHITE,   FOREGROUND_BLUE,    BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,   FOREGROUND_MAGENTA, BACKGROUND_BLACK,   FOREGROUND_MAGENTA, BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_RED,     FOREGROUND_MAGENTA, BACKGROUND_RED,     FOREGROUND_MAGENTA, BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,   FOREGROUND_MAGENTA, BACKGROUND_GREEN,   FOREGROUND_MAGENTA, BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,  FOREGROUND_MAGENTA, FOREGROUND_YELLOW,  FOREGROUND_MAGENTA, FOREGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,    FOREGROUND_MAGENTA, BACKGROUND_BLUE,    FOREGROUND_MAGENTA, BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,    FOREGROUND_MAGENTA, BACKGROUND_CYAN,    FOREGROUND_MAGENTA, BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,   FOREGROUND_MAGENTA, BACKGROUND_WHITE,   FOREGROUND_MAGENTA, BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLACK,      FOREGROUND_CYAN,    BACKGROUND_BLACK,   FOREGROUND_CYAN,    BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_RED,        FOREGROUND_CYAN,    BACKGROUND_RED,     FOREGROUND_CYAN,    BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_GREEN,      FOREGROUND_CYAN,    BACKGROUND_GREEN,   FOREGROUND_CYAN,    BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,     FOREGROUND_CYAN,    FOREGROUND_YELLOW,  FOREGROUND_CYAN,    FOREGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLUE,       FOREGROUND_CYAN,    BACKGROUND_BLUE,    FOREGROUND_CYAN,    BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,    FOREGROUND_CYAN,    BACKGROUND_MAGENTA, FOREGROUND_CYAN,    BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_CYAN,       FOREGROUND_CYAN,    BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_CYAN_WHITE,      FOREGROUND_CYAN,    BACKGROUND_WHITE,   FOREGROUND_CYAN,    BACKGROUND_WHITE   );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_WHITE,   BACKGROUND_BLACK   );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_RED,       FOREGROUND_WHITE,   BACKGROUND_RED,     FOREGROUND_WHITE,   BACKGROUND_RED     );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_GREEN,     FOREGROUND_WHITE,   BACKGROUND_GREEN,   FOREGROUND_WHITE,   BACKGROUND_GREEN   );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,    FOREGROUND_WHITE,   FOREGROUND_YELLOW,  FOREGROUND_WHITE,   FOREGROUND_YELLOW  );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLUE,    FOREGROUND_WHITE,   BACKGROUND_BLUE    );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_MAGENTA, FOREGROUND_WHITE,   BACKGROUND_MAGENTA );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_CYAN,      FOREGROUND_WHITE,   BACKGROUND_CYAN,    FOREGROUND_WHITE,   BACKGROUND_CYAN    );
    cio__init_pair(TEXU_CIO_COLOR_WHITE_WHITE,     FOREGROUND_WHITE,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_WHITE   );
}

#elif (defined __USE_TTY__ || defined __VMS__ || defined __UNIX__)
void
_texu_cio_init_colors_mono(texu_cio *cio)
{
    cio__init_pair(BLACK_BLACK,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_RED,       TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_GREEN,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_YELLOW,    TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_BLUE,      TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_MAGENTA,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_CYAN,      TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_WHITE,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLACK);
    cio__init_pair(RED_BLACK,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_RED);
    cio__init_pair(RED_RED,         TTY_FOREGROUND_RED,     TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_RED);
    cio__init_pair(RED_GREEN,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_RED);
    cio__init_pair(RED_YELLOW,      TTY_FOREGROUND_RED,     TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_RED);
    cio__init_pair(RED_BLUE,        TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_RED);
    cio__init_pair(RED_MAGENTA,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_RED);
    cio__init_pair(RED_CYAN,        TTY_FOREGROUND_RED,     TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_RED);
    cio__init_pair(RED_WHITE,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_RED);
    cio__init_pair(GREEN_BLACK,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_RED,       TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_GREEN,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_YELLOW,    TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_BLUE,      TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_MAGENTA,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_CYAN,      TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_WHITE,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_GREEN);
    cio__init_pair(YELLOW_BLACK,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_RED,      TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_GREEN,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_YELLOW,   TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_BLUE,     TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_MAGENTA,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_CYAN,     TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_WHITE,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(BLUE_BLACK,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_RED,        TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_GREEN,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_YELLOW,     TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_BLUE,       TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_MAGENTA,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_CYAN,       TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_WHITE,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLUE);
    cio__init_pair(MAGENTA_BLACK,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_RED,     TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_GREEN,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_YELLOW,  TTY_FOREGROUND_MAGENTA, TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_BLUE,    TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_CYAN,    TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_WHITE,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(CYAN_BLACK,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_RED,        TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_GREEN,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_YELLOW,     TTY_FOREGROUND_CYAN,    TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_BLUE,       TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_MAGENTA,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_CYAN,       TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_WHITE,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_CYAN);
    cio__init_pair(WHITE_BLACK,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_RED,       TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_GREEN,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_YELLOW,    TTY_FOREGROUND_WHITE,   TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_BLUE,      TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_MAGENTA,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_CYAN,      TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_WHITE,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_WHITE);
}
void
_texu_cio_init_colors(texu_cio *cio)
{
    cio__init_pair(BLACK_BLACK,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_RED,       TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_GREEN,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_YELLOW,    TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_BLUE,      TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_MAGENTA,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_CYAN,      TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLACK);
    cio__init_pair(BLACK_WHITE,     TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLACK);
    cio__init_pair(RED_BLACK,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_RED);
    cio__init_pair(RED_RED,         TTY_FOREGROUND_RED,     TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_RED);
    cio__init_pair(RED_GREEN,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_RED);
    cio__init_pair(RED_YELLOW,      TTY_FOREGROUND_RED,     TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_RED);
    cio__init_pair(RED_BLUE,        TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_RED);
    cio__init_pair(RED_MAGENTA,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_RED);
    cio__init_pair(RED_CYAN,        TTY_FOREGROUND_RED,     TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_RED);
    cio__init_pair(RED_WHITE,       TTY_FOREGROUND_RED,     TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_RED);
    cio__init_pair(GREEN_BLACK,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_RED,       TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_GREEN,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_YELLOW,    TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_BLUE,      TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_MAGENTA,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_CYAN,      TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_GREEN);
    cio__init_pair(GREEN_WHITE,     TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_GREEN);
    cio__init_pair(YELLOW_BLACK,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_RED,      TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_GREEN,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_YELLOW,   TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_BLUE,     TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_MAGENTA,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_CYAN,     TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_YELLOW);
    cio__init_pair(YELLOW_WHITE,    TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_YELLOW);
    cio__init_pair(BLUE_BLACK,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_RED,        TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_GREEN,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_YELLOW,     TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_BLUE,       TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_MAGENTA,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_CYAN,       TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLUE);
    cio__init_pair(BLUE_WHITE,      TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLUE);
    cio__init_pair(MAGENTA_BLACK,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_RED,     TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_GREEN,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_YELLOW,  TTY_FOREGROUND_MAGENTA, TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_BLUE,    TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_CYAN,    TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_MAGENTA);
    cio__init_pair(MAGENTA_WHITE,   TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_MAGENTA);
    cio__init_pair(CYAN_BLACK,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_RED,        TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_GREEN,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_YELLOW,     TTY_FOREGROUND_CYAN,    TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_BLUE,       TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_MAGENTA,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_CYAN,       TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_CYAN);
    cio__init_pair(CYAN_WHITE,      TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_CYAN);
    cio__init_pair(WHITE_BLACK,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLACK,   TTY_FOREGROUND_BLACK,   TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_RED,       TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_RED,     TTY_FOREGROUND_RED,     TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_GREEN,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_GREEN,   TTY_FOREGROUND_GREEN,   TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_YELLOW,    TTY_FOREGROUND_WHITE,   TTY_FOREGROUND_YELLOW,  TTY_FOREGROUND_YELLOW,  TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_BLUE,      TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_BLUE,    TTY_FOREGROUND_BLUE,    TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_MAGENTA,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_MAGENTA, TTY_FOREGROUND_MAGENTA, TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_CYAN,      TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_CYAN,    TTY_FOREGROUND_CYAN,    TTY_BACKGROUND_WHITE);
    cio__init_pair(WHITE_WHITE,     TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_WHITE,   TTY_FOREGROUND_WHITE,   TTY_BACKGROUND_WHITE);
}
#elif (defined WIN32 && defined _WINDOWS)
void
_texu_cio_init_colors_mono(texu_cio *cio)
{
}
void
_texu_cio_init_colors(texu_cio *cio)
{
}
#elif (defined WIN32 && defined _CONSOLE)
#define FOREGROUND_BLACK            0
#define FOREGROUND_YELLOW           (FOREGROUND_RED  | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA          (FOREGROUND_RED  | FOREGROUND_BLUE)
#define FOREGROUND_CYAN             (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_WHITE            (FOREGROUND_RED  | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define BACKGROUND_BLACK            0
#define BACKGROUND_YELLOW           (BACKGROUND_RED  | BACKGROUND_GREEN)
#define BACKGROUND_MAGENTA          (BACKGROUND_RED  | BACKGROUND_BLUE)
#define BACKGROUND_CYAN             (BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BACKGROUND_WHITE            (BACKGROUND_RED  | BACKGROUND_BLUE | BACKGROUND_GREEN)

void _texu_cio_init_colors_mono(texu_cio * cio)
{
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLACK,     FOREGROUND_BLACK,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_RED,       FOREGROUND_BLACK,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_GREEN,     FOREGROUND_BLACK,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,    FOREGROUND_BLACK,   BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLUE,      FOREGROUND_BLACK,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,   FOREGROUND_BLACK,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_CYAN,      FOREGROUND_BLACK,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_WHITE,     FOREGROUND_BLACK,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLACK,       FOREGROUND_RED,     BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_RED,         FOREGROUND_RED,     BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_GREEN,       FOREGROUND_RED,     BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_YELLOW,      FOREGROUND_RED,     BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLUE,        FOREGROUND_RED,     BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_MAGENTA,     FOREGROUND_RED,     BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_CYAN,        FOREGROUND_RED,     BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_WHITE,       FOREGROUND_RED,     BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLACK,     FOREGROUND_GREEN,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_RED,       FOREGROUND_GREEN,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_GREEN,     FOREGROUND_GREEN,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,    FOREGROUND_GREEN,   BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLUE,      FOREGROUND_GREEN,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,   FOREGROUND_GREEN,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_CYAN,      FOREGROUND_GREEN,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_WHITE,     FOREGROUND_GREEN,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,    FOREGROUND_YELLOW,  BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_RED,      FOREGROUND_YELLOW,  BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,    FOREGROUND_YELLOW,  BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,   FOREGROUND_YELLOW,  BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,     FOREGROUND_YELLOW,  BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,  FOREGROUND_YELLOW,  BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,     FOREGROUND_YELLOW,  BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,    FOREGROUND_YELLOW,  BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLACK,      FOREGROUND_BLUE,    BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_RED,        FOREGROUND_BLUE,    BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_GREEN,      FOREGROUND_BLUE,    BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,     FOREGROUND_BLUE,    BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLUE,       FOREGROUND_BLUE,    BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,    FOREGROUND_BLUE,    BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_CYAN,       FOREGROUND_BLUE,    BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_WHITE,      FOREGROUND_BLUE,    BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,   FOREGROUND_MAGENTA, BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_RED,     FOREGROUND_MAGENTA, BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,   FOREGROUND_MAGENTA, BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,  FOREGROUND_MAGENTA, FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,    FOREGROUND_MAGENTA, BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,    FOREGROUND_MAGENTA, BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,   FOREGROUND_MAGENTA, BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLACK,      FOREGROUND_CYAN,    BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_RED,        FOREGROUND_CYAN,    BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_GREEN,      FOREGROUND_CYAN,    BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,     FOREGROUND_CYAN,    FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLUE,       FOREGROUND_CYAN,    BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,    FOREGROUND_CYAN,    BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_CYAN,       FOREGROUND_CYAN,    BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_WHITE,      FOREGROUND_CYAN,    BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_RED,       FOREGROUND_WHITE,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_GREEN,     FOREGROUND_WHITE,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,    FOREGROUND_WHITE,   FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_CYAN,      FOREGROUND_WHITE,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_WHITE,     FOREGROUND_WHITE,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_WHITE);
}
void     _texu_cio_init_colors(texu_cio * cio)
{
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLACK,     FOREGROUND_BLACK,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_RED,       FOREGROUND_BLACK,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_GREEN,     FOREGROUND_BLACK,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,    FOREGROUND_BLACK,   BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_BLUE,      FOREGROUND_BLACK,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,   FOREGROUND_BLACK,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_CYAN,      FOREGROUND_BLACK,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_BLACK_WHITE,     FOREGROUND_BLACK,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_BLACK);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLACK,       FOREGROUND_RED,     BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_RED,         FOREGROUND_RED,     BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_GREEN,       FOREGROUND_RED,     BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_YELLOW,      FOREGROUND_RED,     BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_BLUE,        FOREGROUND_RED,     BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_MAGENTA,     FOREGROUND_RED,     BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_CYAN,        FOREGROUND_RED,     BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_RED_WHITE,       FOREGROUND_RED,     BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_RED);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLACK,     FOREGROUND_GREEN,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_RED,       FOREGROUND_GREEN,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_GREEN,     FOREGROUND_GREEN,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,    FOREGROUND_GREEN,   BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_BLUE,      FOREGROUND_GREEN,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,   FOREGROUND_GREEN,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_CYAN,      FOREGROUND_GREEN,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_GREEN_WHITE,     FOREGROUND_GREEN,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_GREEN);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,    FOREGROUND_YELLOW,  BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_RED,      FOREGROUND_YELLOW,  BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,    FOREGROUND_YELLOW,  BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,   FOREGROUND_YELLOW,  BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,     FOREGROUND_YELLOW,  BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,  FOREGROUND_YELLOW,  BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,     FOREGROUND_YELLOW,  BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,    FOREGROUND_YELLOW,  BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_YELLOW);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLACK,      FOREGROUND_BLUE,    BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_RED,        FOREGROUND_BLUE,    BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_GREEN,      FOREGROUND_BLUE,    BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,     FOREGROUND_BLUE,    BACKGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_BLUE,       FOREGROUND_BLUE,    BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,    FOREGROUND_BLUE,    BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_CYAN,       FOREGROUND_BLUE,    BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_BLUE_WHITE,      FOREGROUND_BLUE,    BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_BLUE);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,   FOREGROUND_MAGENTA, BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_RED,     FOREGROUND_MAGENTA, BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,   FOREGROUND_MAGENTA, BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,  FOREGROUND_MAGENTA, FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,    FOREGROUND_MAGENTA, BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,    FOREGROUND_MAGENTA, BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,   FOREGROUND_MAGENTA, BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_MAGENTA);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLACK,      FOREGROUND_CYAN,    BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_RED,        FOREGROUND_CYAN,    BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_GREEN,      FOREGROUND_CYAN,    BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,     FOREGROUND_CYAN,    FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_BLUE,       FOREGROUND_CYAN,    BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,    FOREGROUND_CYAN,    BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_CYAN,       FOREGROUND_CYAN,    BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_CYAN_WHITE,      FOREGROUND_CYAN,    BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_CYAN);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLACK,     FOREGROUND_WHITE,   BACKGROUND_BLACK,   FOREGROUND_BLACK,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_RED,       FOREGROUND_WHITE,   BACKGROUND_RED,     FOREGROUND_RED,     BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_GREEN,     FOREGROUND_WHITE,   BACKGROUND_GREEN,   FOREGROUND_GREEN,   BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,    FOREGROUND_WHITE,   FOREGROUND_YELLOW,  FOREGROUND_YELLOW,  BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_BLUE,      FOREGROUND_WHITE,   BACKGROUND_BLUE,    FOREGROUND_BLUE,    BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,   FOREGROUND_WHITE,   BACKGROUND_MAGENTA, FOREGROUND_MAGENTA, BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_CYAN,      FOREGROUND_WHITE,   BACKGROUND_CYAN,    FOREGROUND_CYAN,    BACKGROUND_WHITE);
    cio__init_pair(TEXU_CIO_COLOR_WHITE_WHITE,     FOREGROUND_WHITE,   BACKGROUND_WHITE,   FOREGROUND_WHITE,   BACKGROUND_WHITE);
}

#elif (defined __USE_CURSES__)
void
_texu_cio_init_colors_mono(texu_cio *cio)
{
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    init_pair(TEXU_CIO_COLOR_BLACK_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_BLACK,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_RED,           COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_GREEN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_YELLOW,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_BLUE,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_MAGENTA,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_CYAN,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_WHITE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_RED,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_BLACK,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_RED,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_GREEN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_BLUE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_CYAN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_RED,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA,   COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_BLACK,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_RED,          COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_GREEN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_BLUE,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_CYAN,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_BLACK,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_RED,         COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_GREEN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_BLUE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_CYAN,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_WHITE,       COLOR_WHITE,    COLOR_BLACK);

    /*BRIGHT WHITE*/
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLACK,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_RED,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_GREEN,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_YELLOW,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLUE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_MAGENTA,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_CYAN,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BLACK_BRIGHT_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_RED_BRIGHT_WHITE,        COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_GREEN_BRIGHT_WHITE,      COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_YELLOW_BRIGHT_WHITE,     COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BLUE_BRIGHT_WHITE,       COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_MAGENTA_BRIGHT_WHITE,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_CYAN_BRIGHT_WHITE,       COLOR_WHITE,    COLOR_BLACK);
}
void
_texu_cio_init_colors(texu_cio *cio)
{
    init_color(COLOR_WHITE, 1000, 1000, 1000);

    init_pair(TEXU_CIO_COLOR_BLACK_BLACK,   COLOR_BLACK,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLACK_RED,     COLOR_BLACK,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_BLACK_GREEN,   COLOR_BLACK,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_BLACK_YELLOW,  COLOR_BLACK,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_BLACK_BLUE,    COLOR_BLACK,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_BLACK_MAGENTA, COLOR_BLACK,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_BLACK_CYAN,    COLOR_BLACK,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_BLACK_WHITE,   COLOR_BLACK,    COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_RED_BLACK,     COLOR_RED,      COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_RED_RED,       COLOR_RED,      COLOR_RED);
    init_pair(TEXU_CIO_COLOR_RED_GREEN,     COLOR_RED,      COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_RED_YELLOW,    COLOR_RED,      COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_RED_BLUE,      COLOR_RED,      COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_RED_MAGENTA,   COLOR_RED,      COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_RED_CYAN,      COLOR_RED,      COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_RED_WHITE,     COLOR_RED,      COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_GREEN_BLACK,   COLOR_GREEN,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_GREEN_RED,     COLOR_GREEN,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_GREEN_GREEN,   COLOR_GREEN,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_GREEN_YELLOW,  COLOR_GREEN,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_GREEN_BLUE,    COLOR_GREEN,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_GREEN_MAGENTA, COLOR_GREEN,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_GREEN_CYAN,    COLOR_GREEN,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_GREEN_WHITE,   COLOR_GREEN,    COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLACK,  COLOR_YELLOW,   COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_YELLOW_RED,    COLOR_YELLOW,   COLOR_RED);
    init_pair(TEXU_CIO_COLOR_YELLOW_GREEN,  COLOR_YELLOW,   COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_YELLOW_YELLOW, COLOR_YELLOW,   COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_YELLOW_BLUE,   COLOR_YELLOW,   COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_YELLOW_MAGENTA, COLOR_YELLOW,  COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_YELLOW_CYAN,   COLOR_YELLOW,   COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_YELLOW_WHITE,  COLOR_YELLOW,   COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_BLUE_BLACK,    COLOR_BLUE,     COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_BLUE_RED,      COLOR_BLUE,     COLOR_RED);
    init_pair(TEXU_CIO_COLOR_BLUE_GREEN,    COLOR_BLUE,     COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_BLUE_YELLOW,   COLOR_BLUE,     COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_BLUE_BLUE,     COLOR_BLUE,     COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_BLUE_MAGENTA,  COLOR_BLUE,     COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_BLUE_CYAN,     COLOR_BLUE,     COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_BLUE_WHITE,    COLOR_BLUE,     COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLACK, COLOR_MAGENTA,  COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_MAGENTA_RED,   COLOR_MAGENTA,  COLOR_RED);
    init_pair(TEXU_CIO_COLOR_MAGENTA_GREEN, COLOR_MAGENTA,  COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_MAGENTA_YELLOW, COLOR_MAGENTA, COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_MAGENTA_BLUE,  COLOR_MAGENTA,  COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_MAGENTA_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_MAGENTA_CYAN,  COLOR_MAGENTA,  COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_MAGENTA_WHITE, COLOR_MAGENTA,  COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_CYAN_BLACK,    COLOR_CYAN,     COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_CYAN_RED,      COLOR_CYAN,     COLOR_RED);
    init_pair(TEXU_CIO_COLOR_CYAN_GREEN,    COLOR_CYAN,     COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_CYAN_YELLOW,   COLOR_CYAN,     COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_CYAN_BLUE,     COLOR_CYAN,     COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_CYAN_MAGENTA,  COLOR_CYAN,     COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_CYAN_CYAN,     COLOR_CYAN,     COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_CYAN_WHITE,    COLOR_CYAN,     COLOR_WHITE);
    init_pair(TEXU_CIO_COLOR_WHITE_BLACK,   COLOR_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_COLOR_WHITE_RED,     COLOR_WHITE,    COLOR_RED);
    init_pair(TEXU_CIO_COLOR_WHITE_GREEN,   COLOR_WHITE,    COLOR_GREEN);
    init_pair(TEXU_CIO_COLOR_WHITE_YELLOW,  COLOR_WHITE,    COLOR_YELLOW);
    init_pair(TEXU_CIO_COLOR_WHITE_BLUE,    COLOR_WHITE,    COLOR_BLUE);
    init_pair(TEXU_CIO_COLOR_WHITE_MAGENTA, COLOR_WHITE,    COLOR_MAGENTA);
    init_pair(TEXU_CIO_COLOR_WHITE_CYAN,    COLOR_WHITE,    COLOR_CYAN);
    init_pair(TEXU_CIO_COLOR_WHITE_WHITE,   COLOR_WHITE,    COLOR_WHITE);

    /*BRIGHT WHITE*/
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLACK,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_BLACK);
    init_pair(TEXU_CIO_BRIGHT_WHITE_RED,    TEXU_CIO_BRIGHT_WHITE_WHITE,      COLOR_RED);
    init_pair(TEXU_CIO_BRIGHT_WHITE_GREEN,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_GREEN);
    init_pair(TEXU_CIO_BRIGHT_WHITE_YELLOW, TEXU_CIO_BRIGHT_WHITE_WHITE,   COLOR_YELLOW);
    init_pair(TEXU_CIO_BRIGHT_WHITE_BLUE,   TEXU_CIO_BRIGHT_WHITE_WHITE,     COLOR_BLUE);
    init_pair(TEXU_CIO_BRIGHT_WHITE_MAGENTA,TEXU_CIO_BRIGHT_WHITE_WHITE,  COLOR_MAGENTA);
    init_pair(TEXU_CIO_BRIGHT_WHITE_CYAN,   TEXU_CIO_BRIGHT_WHITE_WHITE,     COLOR_CYAN);
    init_pair(TEXU_CIO_BRIGHT_WHITE_WHITE,  TEXU_CIO_BRIGHT_WHITE_WHITE,    COLOR_WHITE);
    init_pair(TEXU_CIO_BLACK_BRIGHT_WHITE,  COLOR_BLACK,    TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_RED_BRIGHT_WHITE,    COLOR_RED,      TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_GREEN_BRIGHT_WHITE,  COLOR_GREEN,    TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_YELLOW_BRIGHT_WHITE, COLOR_YELLOW,   TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_BLUE_BRIGHT_WHITE,   COLOR_BLUE,     TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_MAGENTA_BRIGHT_WHITE,COLOR_MAGENTA,  TEXU_CIO_BRIGHT_WHITE_WHITE);
    init_pair(TEXU_CIO_CYAN_BRIGHT_WHITE,   COLOR_CYAN,     TEXU_CIO_BRIGHT_WHITE_WHITE);
}

#endif

#if (defined VMS || defined __VMS__)
void texu__cio_on_timeout(void* data)
{
    texu_cio* cio = (texu_cio*)data;
    /*trig the registered function by the application*/
    if (cio->on_timeout)
    {
        cio->on_timeout(cio);
    }
}
#endif
void
texu_cio_interval(texu_cio* cio, texu_i32 msec)
{
#ifdef __USE_TTY__
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
#elif (defined VMS || defined __VMS__)
#define TIMER_INTERVAL 5
    long timer = TIMER_INTERVAL * 100; // Convert seconds to 1/100th of seconds
    long status = 0;
    long timeout = 0;
    status = sys$setimr(3, timer, &texu__cio_on_timeout, cio, &timeout);
    status = smg$repaint_screen(&cio->smg_pastebd_id);
#else
    if (msec <= 0)
    {
        msec = 500; /*0.5 sec*/
    }
    timeout(msec);
    refresh();
#endif
}

#ifdef __USE_TTY__
texu_i32
texu_cio_init(texu_cio *cio, texu_i32 lines, texu_i32 cols)
{
    texu_tty_init();
    texu_tty_init_colors();
    
    cio->wndscr = stdout;
    return TEXU_OK;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_init(texu_cio *cio, texu_env *env)
{
#ifdef TEXU_CIO_COLOR_MONO
    _texu_cio_init_colors_mono(cio);
#else
    _texu_cio_init_colors(cio);
#endif
    cio->env = env;
    return TEXU_OK;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_init(texu_cio* cio, texu_env* env, texu_i32 lines, texu_i32 cols)
{
#ifdef TEXU_CIO_COLOR_MONO
    _texu_cio_init_colors_mono(cio);
#else
    _texu_cio_init_colors(cio);
#endif
    cio->env = env;
    cio->win = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(cio->win, ENABLE_WINDOW_INPUT);

    cio->wout = GetStdHandle(STD_OUTPUT_HANDLE);
    return TEXU_OK;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_init(texu_cio *cio, texu_i32 lines, texu_i32 cols)
{
    long status = 0;
    long rows = lines;
    long columns = cols;
    long smg_pastebd_id = 0;
    long smg_display_id = 0;
    long smg_display_id2 = 0;
    long smg_keybd_id = 0;
    long start_row = 1;
    long start_column = 1;
    smg$r_attribute_info_block pbinfo;
    long pbsz = sizeof(pbinfo);
#ifdef TEXU_CIO_COLOR_MONO
    _texu_cio_init_colors_mono(cio);
#else
    _texu_cio_init_colors(cio);
#endif
    status = smg$create_pasteboard(&smg_pastebd_id);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating pasteboard: %ld\n", status);
        return status;
    }
    status = smg$get_pasteboard_attributes(&smg_pastebd_id, &pbinfo, &pbsz);
    if (!(status & 1))
    {
        fprintf(stderr, "Error get pasteboard attributes: %ld\n", status);
        return status;
    }
    
    rows = pbinfo.smg$r_row_overlay;
    columns = pbinfo.smg$w_num_columns;
    status = smg$create_virtual_display(&rows, &columns, &smg_display_id);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating display: %ld\n", status);
        return status;
    }
    /*status = smg$copy_virtual_display(&smg_display_id, &smg_display_id2);*/
    status = smg$create_virtual_display(&rows, &columns, &smg_display_id2);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating display: %ld\n", status);
        return status;
    }
#if 0
    status = smg$paste_virtual_display(&smg_display_id, &smg_pastebd_id);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating paste display: %ld\n", status);
        return status;
    }
#endif
    /*status = smg$set_display_scroll_region(&smg_display_id, &start_row, &start_column);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating scroll: %ld\n", status);
        return status;
    }*/
    status = smg$create_virtual_keyboard(&smg_keybd_id);
    if (!(status & 1))
    {
        fprintf(stderr, "Error creating keyboard: %ld\n", status);
        return status;
    }

    cio->smg_pastebd_id = smg_pastebd_id;
    cio->smg_display_id = smg_display_id;
    cio->smg_keybd_id   = smg_keybd_id;
    /*double-buffering technic*/
    cio->smg_flipflop   = 0;
    cio->smg_offscreens[0] = smg_display_id;
    cio->smg_offscreens[1] = smg_display_id2;

    status = vms_get_sysio(&cio->sysin_chan, "sys$input");
    if (status != SS$_NORMAL)
    {
        //free(genvptr);
        return TEXU_ERROR;
    }


    texu_cio_echo(cio, TEXU_FALSE);
    
    smg$save_physical_screen(&cio->smg_pastebd_id, &cio->smg_display_id);

    return TEXU_OK;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_init(texu_cio* cio, texu_i32 lines, texu_i32 cols)
{
    return 0;
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
void texu_cio_sigint(texu_cio *cio)
{
    if (cio->sigint_handler)
    {
        cio->sigint_handler(cio->sig_userdata);
    }
}
void texu_cio_set_cbreak(texu_cio *cio, int cbreak, fn_sigint_callback sigint_handler, void *userdata)
{
    if (cbreak)
    {
        cio->sigint_handler = sigint_handler;
        cio->sig_userdata = userdata;
    }
    else
    {
        cio->sigint_handler = 0;
        cio->sig_userdata = 0;
    }
}

void cio__sigint_handler(void *userdata)
{
    texu_cio *cio = (texu_cio*)userdata;
    texu_env *env = cio->env;
    texu_env_del(env);
}
texu_i32
texu_cio_init(texu_cio *cio, texu_env* env, texu_i32 lines, texu_i32 cols)
{
    texu_i32 rc = 0;
    cio->env = env;
#ifdef TEXU_CIO_COLOR_MONO
    _texu_cio_init_colors_mono(cio);
#else
    _texu_cio_init_colors(cio);
#endif

#if defined __USE_TERMBOX2__
    rc = texu_tb2_init(lines, cols);
    texu_tb2_cbreak(cio, 1);/*, cio__sigint_handler, cio->env);*/
#else
    rc = texu_termios_init(lines, cols);
    texu_termios_cbreak(cio, 1);/*, cio__sigint_handler, cio->env);*/
#endif
    texu_cio_set_cbreak(cio, 1, cio__sigint_handler, cio);
    return rc;
}
#else
texu_i32
texu_cio_init(texu_cio *cio, texu_i32 lines, texu_i32 cols)
{
    int colors = 0;
    initscr();
    /*raw();*/
    cbreak();
    keypad(stdscr, 1);
    noecho();
    timeout(500); /*if there is no key pressed, make TEXU_MW_IDLE msg sent to the top window*/

    colors = has_colors();
    if (!colors)
    {
    }
    else
    {
        start_color();
    }
#ifdef TEXU_CIO_COLOR_MONO
    _texu_cio_init_colors_mono(cio);
#else
    _texu_cio_init_colors(cio);
#endif

#ifdef __LINUX__
    if (lines > 0 && cols > 0)
    {
        resizeterm(lines, cols);
    }
#endif
    /*save window*/
    cio->wndscr = stdscr;
    return TEXU_OK;
}
#endif /*__USE_TTY__*/

#ifdef __USE_TTY__
void
texu_cio_release(texu_cio *cio)
{
    texu_tty_release();
}
#elif (defined WIN32 && defined _WINDOWS)
void
texu_cio_release(texu_cio *cio)
{
}
#elif (defined WIN32 && defined _CONSOLE)
void
texu_cio_release(texu_cio* cio)
{
}
#elif (defined VMS || defined __VMS__)
void texu_cio_erase_chars(texu_cio* cio, texu_i32 no_chars, texu_i32 y, texu_i32 x)
{
    /*vms start from (1, 1)*/
    long smg_display_id = texu__cio_get_on_display(cio);
    y++;
    x++;
/*    smg$erase_chars(&cio->smg_display_id, &no_chars, &y, &x);*/
    smg$erase_chars(&smg_display_id, &no_chars, &y, &x);
}

void
texu_cio_release(texu_cio *cio)
{
    smg$restore_physical_screen(&cio->smg_pastebd_id, &cio->smg_display_id);
    smg$delete_virtual_keyboard(&cio->smg_keybd_id);
/*    smg$delete_virtual_display(&cio->smg_display_id);*/
    smg$delete_virtual_display(&cio->smg_offscreens[0]);
    smg$delete_virtual_display(&cio->smg_offscreens[1]);
    smg$delete_pasteboard(&cio->smg_pastebd_id);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
void
texu_cio_release(texu_cio *cio)
{
#if  defined __USE_TERMBOX2__
    texu_tb2_release();
#else
    texu_termios_release();
#endif
}
#else
void
texu_cio_release(texu_cio *cio)
{
    endwin();
    /* release window */
    cio->wndscr = 0;
}
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_echo(texu_cio *cio, texu_bool echo_)
{
    if (echo_)
    {
        return texu_tty_echo();
    }
    return texu_tty_noecho();
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_echo(texu_cio* cio, texu_bool echo_)
{
    return TEXU_OK;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_echo(texu_cio* cio, texu_bool echo_)
{
    return TEXU_OK;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_echo(texu_cio* cio, texu_bool echo_)
{
    return TEXU_OK;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_echo(texu_cio *cio, texu_bool echo_)
{
    long smg_cursor_mode = SMG$M_CURSOR_OFF;
    long status = 0;
    if (echo_)
    {
        smg_cursor_mode = SMG$M_CURSOR_ON;
    }
    status = smg$set_cursor_mode(&cio->smg_pastebd_id, &smg_cursor_mode);
    return status;
}
#else
texu_i32
texu_cio_echo(texu_cio *cio, texu_bool echo_)
{
    if (echo_)
    {
        return echo();
    }
    return noecho();
}
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_getch(texu_cio *cio)
{
    return texu_tty_getch(0, 5000);
}
texu_i32
texu_cio_getch2(texu_cio *cio, texu_i32 timeout)
{
    return texu_tty_getch(0, timeout);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_getch(texu_cio *cio)
{
#if defined __USE_TERMBOX2__
    return texu_tb2_getch(cio, &cio->ev, 500);
#else
    return texu_termios_getch(cio, &cio->ev, 500);
#endif
}
texu_i32
texu_cio_getch2(texu_cio *cio, texu_i32 timeout)
{
#if defined __USE_TERMBOX2__
    return texu_tb2_getch(cio, &cio->ev, timeout);
#else
    return texu_termios_getch(cio, &cio->ev, timeout);
#endif
}
texu_i32
texu_cio_dispatch(texu_cio *cio)
{
    texu_i32 alt = 0;
    texu_i32 ctrl = 0;
    texu_i32 shft = 0;
    if (TB_MOD_ALT & cio->ev.mod)
    {
        alt = TB_MOD_ALT;
    }
#if (defined __USE_TERMBOX2__)
    if (TB_MOD_CTRL & cio->ev.mod)
    {
        ctrl = TB_MOD_CTRL;
    }
    if (TB_MOD_SHIFT & cio->ev.mod)
    {
        shft = TB_MOD_SHIFT;
    }
#else
    if (cio->ev.ch < 128)
    {
        if (cio->ev.ch == 0 && cio->ev.key < 128)
        {
            switch (cio->ev.key)
            {
                case TB_KEY_BACKSPACE:
                case TB_KEY_TAB:
                case TB_KEY_ENTER:
                case TB_KEY_ESC:
                    break;
                default:
                    ctrl = 2;
                    break;
            }
        }
    }
#endif
    return (alt | ctrl | shft);
}
/*translate the control key to char*/

#if defined __USE_TERMBOX2__
texu_i32 texu_cio_getraw(texu_cio *cio, texu_i32 *pkey, texu_i32 *pch, texu_i32 *pmod)
{
    if (pkey)
    {
        *pkey = cio->ev.key;
    }
    if (pch)
    {
        *pch = cio->ev.ch;
    }
    if (pmod)
    {
        *pmod = cio->ev.mod;
    }
    return 0;
}
texu_i32 texu_cio_translate2(texu_cio *cio, texu_i32 key, texu_i32 *mod)
{
    texu_i32 ch = key;
    /*not translate the key*/
    switch (key)
    {
        case TB_KEY_BACKSPACE:
        case TB_KEY_TAB:
        case TB_KEY_ENTER:
        case TB_KEY_ESC:
        case TB_KEY_BACKSPACE2:
            *mod = (~TB_MOD_CTRL) & (*mod);
            return key;
    }
    if (key >= TB_KEY_CTRL_A && key <= TB_KEY_CTRL_Z)
    {
        ch = ('A' + (key - 1));
    }
    else if (key >= TB_KEY_F12 && key <= TB_KEY_F1)
    {
        if (TB_MOD_SHIFT & (*mod))
        {
            /*TEXU_KEY_11 = TEXU_KEY_F1 + SHIFT*/
            /*TEXU_KEY_12 = TEXU_KEY_F2 + SHIFT*/
            /*TEXU_KEY_13 = TEXU_KEY_F3 + SHIFT*/
            texu_i32 delta = (TB_KEY_F1 - key);
            if (TEXU_KEY_F1 == key || TEXU_KEY_F2 == key)
            {
                ch = TEXU_KEY_F11 - delta;
                *mod = (*mod) & (~TB_MOD_SHIFT);
            }
            else if (delta > 1)
            {
                ch = TEXU_KEY_F13 - (delta - 2);
                *mod = (*mod) & (~TB_MOD_SHIFT);
            }
        }
    }
    else
    {
        switch (key)
        {
            /*termbox does not translate '0','1','9'*/
            case TB_KEY_CTRL_2: ch = '2'; break;
            case TB_KEY_CTRL_3: ch = '3'; break;
            case TB_KEY_CTRL_4: ch = '4'; break;
            case TB_KEY_CTRL_5: ch = '5'; break;
            case TB_KEY_CTRL_6: ch = '6'; break;
            case TB_KEY_CTRL_7: ch = '7'; break;
            case TB_KEY_CTRL_8: ch = '8'; break;
            default:
                break;
        }
    }
    return ch;
}
#endif

texu_i32 texu_cio_translate(texu_cio *cio, texu_i32 key)
{
    texu_i32 ch = key;
    /*not translate the key*/
    switch (key)
    {
        case TB_KEY_BACKSPACE:
        case TB_KEY_TAB:
        case TB_KEY_ENTER:
        case TB_KEY_ESC:
        case TB_KEY_BACKSPACE2:
            return key;
    }
    if (key >= TB_KEY_CTRL_A && key <= TB_KEY_CTRL_Z)
    {
        ch = ('A' + (key - 1));
    }
    else
    {
        switch (key)
        {
            /*termbox does not translate '0','1','9'*/
            case TB_KEY_CTRL_2: ch = '2'; break;
            case TB_KEY_CTRL_3: ch = '3'; break;
            case TB_KEY_CTRL_4: ch = '4'; break;
            case TB_KEY_CTRL_5: ch = '5'; break;
            case TB_KEY_CTRL_6: ch = '6'; break;
            case TB_KEY_CTRL_7: ch = '7'; break;
            case TB_KEY_CTRL_8: ch = '8'; break;
            default:
                break;
        }
    }
    return ch;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_getch(texu_cio *cio)
{
    return 0;
}
texu_i32
texu_cio_getch2(texu_cio *cio, texu_i32 timeout)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_win32_getch(texu_cio* cio, texu_char* ascii, texu_i32* alt, texu_i32* ctrl, texu_i32* shift)
{
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    texu_bool bDone = 0;
    texu_env* env = cio->env;
    while (!bDone)
    {
        ReadConsoleInput(
            cio->win,      // input buffer handle 
            irInBuf,     // buffer to read into 
            128,         // size of read buffer 
            &cNumRead); // number of records read
        for (i = 0; i < cNumRead; ++i)
        {
            if (KEY_EVENT == irInBuf[i].EventType)
            {
                if (irInBuf[i].Event.KeyEvent.bKeyDown)
                {
                    //return (texu_i32)irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
                    DWORD dwState = irInBuf[i].Event.KeyEvent.dwControlKeyState;
                    if (dwState & (RIGHT_ALT_PRESSED |LEFT_ALT_PRESSED))
                    {
                        if (alt)
                        {
                            *alt = 1;
                        }
                    }
                    if (dwState & (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED))
                    {
                        if (ctrl)
                        {
                            *ctrl = 2;
                        }
                    }
                    if (dwState & (SHIFT_PRESSED))
                    {
                        if (ctrl)
                        {
                            *shift = 4;
                        }
                    }
                    if (ascii)
                    {
                        *ascii = irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
                    }
                    return (texu_i32)irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
                }
            }
        }
    }
    return -1;
}
texu_i32
texu_cio_getch(texu_cio* cio)
{
    texu_i32 alt   = 0;
    texu_i32 ctrl  = 0;
    texu_i32 shift = 0;
    texu_char ch   = 0;
    return texu_cio_win32_getch(cio, &ch, &alt, &ctrl, &shift);
}
texu_i32
texu_cio_getch2(texu_cio* cio, texu_i32 timeout)
{
    texu_i32 alt   = 0;
    texu_i32 ctrl  = 0;
    texu_i32 shift = 0;
    texu_char ch   = 0;
    return texu_cio_win32_getch(cio, &ch, &alt, &ctrl, &shift);
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_getch(texu_cio *cio)
{
    unsigned short key;
    int timeout = 1;
    long status = smg$read_keystroke(&cio->smg_keybd_id, &key, 0, &timeout);
    if (SS$_TIMEOUT == status)
    {
        return -1;
    }
    return key;
}
texu_i32
texu_cio_getch2(texu_cio *cio, texu_i32 timeout)
{
    unsigned short key;
    /*int timeout = 1;*/
    long status = smg$read_keystroke(&cio->smg_keybd_id, &key, 0, &timeout);
    if (SS$_TIMEOUT == status)
    {
        return -1;
    }
    return key;
}
#else
texu_i32
texu_cio_getch(texu_cio *cio)
{
    return getch();
}
texu_i32
texu_cio_getch2(texu_cio *cio, texu_i32 timeout)
{
    if (timeout < 0)
    {
        timeout = 0;
    }
    texu_cio_interval(cio, timeout);
    return getch();
}
#endif


#ifdef __USE_TTY__
texu_i32
texu_cio_getstr(texu_cio *cio, texu_char *str)
{
    return 0;
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_getstr(texu_cio *cio, texu_char *str)
{
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_getstr(texu_cio *cio, texu_char *str)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_getstr(texu_cio* cio, texu_char* str)
{
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_getstr(texu_cio *cio, texu_char *str)
{
    long status = smg$read_string(&cio->smg_keybd_id, str);
    return status;
}
#else
texu_i32
texu_cio_getstr(texu_cio *cio, texu_char *str)
{
    return wgetstr(cio->wndscr, str);
}
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_gotoyx(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    texu_env_save_curpos(cio->env, y, x);
    return texu_tty_gotoyx(y, x);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_gotoyx(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    texu_env_save_curpos(cio->env, y, x);
#if defined __USE_TERMBOX2__
    texu_tb2_gotoyx(y, x);
#else
    texu_termios_gotoyx(y, x);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_status     texu_env_gotoyx(texu_env *env, texu_i32 y, texu_i32 x);

texu_i32
texu_cio_gotoyx(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    texu_env_save_curpos(cio->env, y, x);
    return texu_env_gotoyx(cio->env, y, x);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_gotoyx(texu_cio* cio, texu_i32 y, texu_i32 x)
{
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;

    texu_env_save_curpos(cio->env, y, x);
    SetConsoleCursorPosition(cio->wout, coord);
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_gotoyxXX(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    long smg_display_id = texu__cio_get_on_display(cio);
    long x_coord = x + 1;
    long y_coord = y + 1;
/*    long status = smg$set_cursor_abs(&cio->smg_display_id, &y_coord, &x_coord);*/
    long status = smg$set_cursor_abs(&smg_display_id, &y_coord, &x_coord);
    texu_env_save_curpos(cio->env, y, x);
    return status;
}
texu_i32
texu_cio_gotoyx(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    long x_coord = x + 1;
    long y_coord = y + 1;
    texu_env_save_curpos(cio->env, y, x);
    return texu_tty_gotoyx(y_coord, x_coord);
}
#else
texu_i32
texu_cio_gotoyx(texu_cio *cio, texu_i32 y, texu_i32 x)
{
    texu_env_save_curpos(cio->env, y, x);
    return wmove(cio->wndscr, y, x);
}
#endif /*__USE_TTY__*/


#ifdef __USE_TTY__
texu_i32
texu_cio_getyx(texu_cio *cio, texu_i32* y, texu_i32* x)
{
    return 0;
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_getyx(texu_cio *cio, texu_i32* y, texu_i32* x)
{
#if defined __USE_TERMBOX2__
    texu_tb2_getyx((int*)y, (int*)x);
#else
    texu_termios_getyx((int*)y, (int*)x);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_getyx(texu_cio *cio, texu_i32* y, texu_i32* x)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_getyx(texu_cio* cio, texu_i32* y, texu_i32* x)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(cio->wout, &csbi);
    *x = csbi.dwCursorPosition.X;
    *y = csbi.dwCursorPosition.Y;
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_getyx(texu_cio *cio, texu_i32* y, texu_i32* x)
{
    long smg_display_id = texu__cio_get_on_display(cio);
/*    return smg$return_cursor_pos(cio->smg_display_id, x, y);*/
    return smg$return_cursor_pos(smg_display_id, x, y);
}
#else
texu_i32
texu_cio_getyx(texu_cio *cio, texu_i32* y, texu_i32* x)
{
    int _y = 0, _x = 0;
    getyx(cio->wndscr, _y, _x);
    *y = _y;
    *x = _x;
    return 0;
}
#endif


#ifdef __USE_TTY__
texu_i32
texu_cio_clear(texu_cio *cio)
{
    return texu_tty_clear();
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_clear(texu_cio *cio)
{
#if defined __USE_TERMBOX2__
    texu_tb2_clear();
#else
    texu_termios_clear();
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_clear(texu_cio* cio)
{
    COORD coordScreen = { 0, 0 };    /* home for the cursor */
    DWORD cbBytesWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    /*Get the number of character cells in the current buffer. */

    if (!GetConsoleScreenBufferInfo(cio->wout, &csbi))
    {
        return -1;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire screen with blanks.*/

    if (!FillConsoleOutputCharacter(cio->wout,        /* Handle to console screen buffer */
        (texu_char)' ',      /* character to write to the buffer */
        dwConSize,       /* Number of cells to write */
        coordScreen,     /* Coordinates of first cell */
        &cbBytesWritten))/* Receive number of characters written */
    {
        return -2;
    }

    /* Get the current text attribute.*/

    if (!GetConsoleScreenBufferInfo(cio->wout, &csbi))
    {
        return -3;
    }

    /* Set the buffer's attributes accordingly. */

    if (!FillConsoleOutputAttribute(cio->wout,         /* Handle to console screen buffer */
        csbi.wAttributes, /* character attributes to use */
        dwConSize,        /* Number of cells to set attribute */
        coordScreen,      /* Coordinates of first cell */
        &cbBytesWritten)) /* Receive number of characters written */
    {
        return -4;
    }

    /* Put the cursor at its home coordinates.*/

    SetConsoleCursorPosition(cio->wout, coordScreen);
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_clear(texu_cio *cio)
{
    long smg_display_id = texu__cio_get_on_display(cio);
    return smg$erase_display(&smg_display_id);
/*    return smg$erase_display(&cio->smg_display_id);*/
}
#else
texu_i32
texu_cio_clear(texu_cio *cio)
{
    return wclear(cio->wndscr);
}
#endif /*__USE_TTY__*/

#ifdef __USE_TTY__
texu_i32
texu_cio_clearln(texu_cio *cio, texu_i32 y)
{
    return texu_tty_clearln(y);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_clearln(texu_cio* cio, texu_i32 y)
{
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_clearln(texu_cio* cio, texu_i32 y)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_clearln(texu_cio* cio, texu_i32 y)
{

    COORD coordScreen = { 0, 0 };    /* home for the cursor */
    DWORD cbBytesWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    /*Get the number of character cells in the current buffer. */

    if (!GetConsoleScreenBufferInfo(cio->wout, &csbi))
    {
        return -1;
    }

    //dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    dwConSize = 1 * csbi.dwSize.Y;
    coordScreen.Y = (SHORT)y;

    /* Fill the entire screen with blanks.*/

    if (!FillConsoleOutputCharacter(cio->wout,        /* Handle to console screen buffer */
        (texu_char)' ',      /* character to write to the buffer */
        dwConSize,       /* Number of cells to write */
        coordScreen,     /* Coordinates of first cell */
        &cbBytesWritten))/* Receive number of characters written */
    {
        return -1;
    }

    /* Get the current text attribute.*/

    if (!GetConsoleScreenBufferInfo(cio->wout, &csbi))
    {
        return -2;
    }

    /* Set the buffer's attributes accordingly. */

    if (!FillConsoleOutputAttribute(cio->wout,         /* Handle to console screen buffer */
        csbi.wAttributes, /* character attributes to use */
        dwConSize,        /* Number of cells to set attribute */
        coordScreen,      /* Coordinates of first cell */
        &cbBytesWritten)) /* Receive number of characters written */
    {
        return -3;
    }

    /* Put the cursor at its home coordinates.*/

    SetConsoleCursorPosition(cio->wout, coordScreen);
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_clearln(texu_cio *cio, texu_i32 y)
{
    long smg_display_id = texu__cio_get_on_display(cio);
    return smg$erase_line(&smg_display_id, &y);
/*    return smg$erase_line(&cio->smg_display_id, &y);*/
}
#else
texu_i32
texu_cio_clearln(texu_cio *cio, texu_i32 y)
{
    texu_cio_gotoyx(cio, y, 0);
    return wclrtoeol(cio->wndscr);
}
#endif /*__USE_TTY__*/

#ifdef __USE_TTY__
texu_i32
texu_cio_putch_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
    return texu_tty_putch(y, x, ch);
}
texu_i32
texu_cio_putch(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
    return texu_cio_putch_erase(cio, y, x, ch, TEXU_FALSE);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putch_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch, -1, -1);
#else
    texu_termios_putch(y, x, ch, -1, -1);
#endif
    return 0;
}
texu_i32
texu_cio_putch(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch, -1, -1);
#else
    texu_termios_putch(y, x, ch, -1, -1);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putch_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
    return 0;
}
texu_i32
texu_cio_putch(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
    return texu_cio_putch_erase(cio, y, x, ch, TEXU_FALSE);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putch_erase(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
    texu_char str[2] = { (texu_char)ch, 0 };
    return texu_cio_putstr_attr(cio, y, x, str, 0);
}
texu_i32
texu_cio_putch(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
    return texu_cio_putch_erase(cio, y, x, ch, TEXU_FALSE);
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_putch_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
    char sz[2] = { ch, 0 };
    $DESCRIPTOR(text, sz);
    texu_i32 status = 0;
    long smg_display_id = texu__cio_get_on_display(cio);
    
    y++;    /*curses start (0, 0)*/
    x++;    /*vms start (1, 1)*/
    status = smg$put_chars(&cio->smg_display_id, &text, &y, &x,
                &erase,     /*0 - Does not erase line (the default), 1 - Erase */
                &0, &0);    /*Set   Complement  Action*/
                            /*0     0           Attribute set to default*/
                            /*1     0           Attribute on*/
                            /*0     1           Attribute set to complement of default setting*/
                            /*1     1           Attribute off*/

    texu_cio_get_color(cio, WHITE_BLACK);
    return status;
}
texu_i32
texu_cio_putch(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
    return texu_cio_putch_erase(cio, y, x, ch, TEXU_FALSE);
}
#else
texu_i32
texu_cio_putch_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_bool erase)
{
    return mvwaddch(cio->wndscr, y, x, ch);
}
texu_i32
texu_cio_putch(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch)
{
    return texu_cio_putch_erase(cio, y, x, ch, TEXU_FALSE);
}
#endif /*__USE_TTY__*/

#ifdef __USE_TTY__
texu_i32
texu_cio_putch_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    texu_tty_attron(attrs);
    texu_tty_putch(y, x, ch);
    return texu_tty_attroff(cio, attrs);
}
texu_i32
texu_cio_putch_attr(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    return texu_cio_putch_attr_erase(cio, y, x, ch, attrs, TEXU_FALSE);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putch_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch,
        texu_cio_get_color(cio, cio__color_pair(attrs)), 
        texu_cio_get_color(cio, cio__color_pair(attrs)));
#else
    texu_termios_putch(y, x, ch,
        texu_cio_get_color(cio, cio__color_pair(attrs)), 
        -1);
#endif
    return 0;
}
texu_i32
texu_cio_putch_attr(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch, 
        texu_cio_get_color(cio, cio__color_pair(attrs)), 
        texu_cio_get_color(cio, cio__color_pair(attrs)));
#else
    texu_termios_putch(y, x, ch, 
        texu_cio_get_color(cio, cio__color_pair(attrs)), 
        -1);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putch_attr_erase(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    return 0;
}
texu_i32
texu_cio_putch_attr(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    return texu_cio_putch_attr_erase(cio, y, x, ch, attrs, TEXU_FALSE);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putch_attr_erase(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    texu_char str[2] = { (texu_char)ch, 0 };
    return texu_cio_putstr_attr(cio, y, x, str, attrs);
}
texu_i32
texu_cio_putch_attr(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    return texu_cio_putch_attr_erase(cio, y, x, ch, attrs, TEXU_FALSE);
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_putch_attr_eraseXX(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    char sz[2] = { ch, 0 };
    $DESCRIPTOR(text, sz);
    texu_i32 status = 0;
    
    /*texu_tty_attron(attrs);*/
    /*status = texu_cio_gotoyx(cio, y+1, x+1);*/
#if 0
    texu_cio_gotoyx(cio, y, x);
/*    texu_cio_get_color(cio, attrs);*/
    printf(sz);
#else
    y++;    /*curses start (0, 0)*/
    x++;    /*vms start (1, 1)*/
    status = smg$put_chars(&cio->smg_display_id, &text, &y, &x,/*&x, &y,*/
                &erase,     /*Does not erase line (the default)*/
                &1, &0);    /*Set   Complement  Action*/
                            /*0     0           Attribute set to default*/
                            /*1     0           Attribute on*/
                            /*0     1           Attribute set to complement of default setting*/
                            /*1     1           Attribute off*/
#endif
    /*texu_tty_attroff(cio, attrs);*/
    texu_cio_get_color(cio, WHITE_BLACK);
    return status;
}
texu_i32
texu_cio_putch_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    char text[2] = { ch, 0 };
    texu_cio_gotoyx(cio, y, x);
    texu_cio_attron(cio, attrs);
    vms_writeio(cio->sysin_chan, &cio->iosb, text, 1);
    return texu_cio_attroff(cio, attrs);
}
texu_i32
texu_cio_putch_attr(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    return texu_cio_putch_attr_erase(cio, y, x, ch, attrs, TEXU_FALSE);
}
#else
texu_i32
texu_cio_putch_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs, texu_bool erase)
{
    texu_cio_attron(cio, attrs);
    mvwaddch(cio->wndscr, y, x, (char)ch);
    return texu_cio_attroff(cio, attrs);
}
texu_i32
texu_cio_putch_attr(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    return texu_cio_putch_attr_erase(cio, y, x, ch, attrs, TEXU_FALSE);
}

#endif /*__USE_TTY__*/


#ifdef __USE_TTY__
texu_i32
texu_cio_putch_attr2(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putch_attr(cio, y, x, ch, attrs);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putch_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 fg, texu_i32 bg)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch, 
        texu_cio_get_color(cio, cio__color_pair(fg)), 
        texu_cio_get_color(cio, cio__color_pair(bg)));
#else
    texu_termios_putch(y, x, ch, 
        texu_cio_get_color(cio, cio__color_pair(fg)), 
        texu_cio_get_color(cio, cio__color_pair(bg)));
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putch_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putch_attr(cio, y, x, ch, attrs);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putch_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putch_attr(cio, y, x, ch, attrs);
}
#else
texu_i32
texu_cio_putch_attr2(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putch_attr(cio, y, x, ch, color | attrs);
}
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_putstr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_bool erase)
{
    return texu_tty_putstr(y, x, str);
}
texu_i32
texu_cio_putstr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str)
{
    return texu_tty_putstr(y, x, str);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putstr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_bool erase)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putstr(y, x, str, 0, 0);
#else
    texu_termios_putstr(y, x, str, -1, -1);
#endif
    return 0;
}
texu_i32
texu_cio_putstr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putstr(y, x, str, 0, 0);
#else
    texu_termios_putstr(y, x, str, -1, -1);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putstr_erase(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_bool erase)
{
    return 0;
}
texu_i32
texu_cio_putstr(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str)
{
    return texu_cio_putstr_erase(cio, y, x, str, TEXU_FALSE);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putstr_erase(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_bool erase)
{
    return texu_cio_putstr_attr(cio, y, x, str, 0);
}
texu_i32
texu_cio_putstr(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str)
{
    return texu_cio_putstr_erase(cio, y, x, str, TEXU_FALSE);
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_putstr_eraseXX(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_bool erase)
{
    /*char* psz = (char*)str;*/
    texu_char sz[TEXU_MAX_WNDTEXT + 1];
    $DESCRIPTOR(text, sz);
    texu_i32 status = 0;
    texu_i32 newx = x;
    texu_i32 newy = y;

    memset(sz, 0, sizeof(sz));
    strcpy(sz, str);
    /*status = texu_cio_gotoyx(cio, y+1, x+1);*/
#if 0
    texu_cio_gotoyx(cio, y, x);
    texu_cio_get_color(cio, WHITE_BLACK);
    printf(sz);
#else
    texu_cio_getyx(cio, &newy, &newx);
    if (y < 0)
    {
        y = newy;
    }
    else
    {
        ++y;
    }
    if (x < 0)
    {
        x = newx;
    }
    else
    {
        ++x;
    }
//    y++;    /*curses start (0, 0)*/
//    x++;    /*vms start (1, 1)*/
    status = smg$put_chars(&cio->smg_display_id, &text, &y, &x,/*&x, &y,*/
                &erase,         /*Does not erase line (the default)*/
                &0, &0);    /*Set   Complement  Action*/
                            /*0     0           Attribute set to default*/
                            /*1     0           Attribute on*/
                            /*0     1           Attribute set to complement of default setting*/
                            /*1     1           Attribute off*/
#endif
    texu_cio_get_color(cio, WHITE_BLACK);
    return status;
}

texu_i32
texu_cio_putstr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_bool erase)
{
    texu_cio_gotoyx(cio, y, x);
    return vms_writeio(cio->sysin_chan, &cio->iosb, str, strlen(str));
}

texu_i32
texu_cio_putstr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str)
{
    return texu_cio_putstr_erase(cio, y, x, str, TEXU_FALSE);
}
#else
texu_i32
texu_cio_putstr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_bool erase)
{
    return mvwprintw(cio->wndscr, y, x, str);
}
texu_i32
texu_cio_putstr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str)
{
    return texu_cio_putstr_erase(cio, y, x, str, TEXU_FALSE);
}
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_putstr_attr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs)
{
    texu_tty_attron(attrs);
    texu_tty_putstr(y, x, str);
    return texu_tty_attroff(cio, attrs);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putstr_attr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 fg)
{
#if defined __USE_TERMBOX2__
    texu_tb2_putstr(y, x, str,
        texu_cio_get_color(cio, cio__color_pair(fg)), 
        texu_cio_get_color(cio, cio__color_pair(fg)));
#else
    texu_termios_putstr(y, x, str,
        texu_cio_get_color(cio, cio__color_pair(fg)), 
        -1);
#endif
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putstr_attr(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_i32 attrs)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putstr_attr(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_i32 attrs)
{
    DWORD dwWritten = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    texu_cio_gotoyx(cio, y, x);
    GetConsoleScreenBufferInfo(cio->wout, &csbi);
    SetConsoleTextAttribute(cio->wout, (WORD)attrs);
    WriteConsole(cio->wout, str, strlen(str), &dwWritten, 0);
    SetConsoleTextAttribute(cio->wout, csbi.wAttributes);
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_putstr_attr_eraseXX(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs, texu_bool erase)
{
//    texu_char* psz = (char*)malloc(strlen(str)+1);
    texu_char sz[TEXU_MAX_WNDTEXT + 1];
    $DESCRIPTOR(text, sz);
    texu_i32 status = 0;
    texu_i32 newx = x;
    texu_i32 newy = y;

    memset(sz, 0, sizeof(sz));
    strcpy(sz, str);
    /*texu_tty_attron(attrs);*/
    /*status = texu_cio_gotoyx(cio, y+1, x+1);*/
#if 0
    texu_cio_gotoyx(cio, y, x);
/*    texu_cio_get_color(cio, attrs);*/
    printf(sz);
#else
    texu_cio_getyx(cio, &newy, &newx);
    if (y < 0)
    {
        y = newy;
    }
    else
    {
        ++y;
    }
    if (x < 0)
    {
        x = newx;
    }
    else
    {
        ++x;
    }
//    y++;    /*curses start (0, 0)*/
//    x++;    /*vms start (1, 1)*/
    status = smg$put_chars(&cio->smg_display_id, &text, &y, &x,/*&x, &y,*/
                &erase,     /*0 - do not erase, 1-erase before paint*/
                &0, &0);    /*Set   Complement  Action*/
                            /*0     0           Attribute set to default*/
                            /*1     0           Attribute on*/
                            /*0     1           Attribute set to complement of default setting*/
                            /*1     1           Attribute off*/
#endif
    /*texu_tty_attroff(cio, attrs);*/
    texu_cio_get_color(cio, WHITE_BLACK);
    return status;
}

texu_i32
texu_cio_putstr_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs, texu_bool erase)
{
    texu_cio_gotoyx(cio, y, x);
    //texu_cio_attron(cio, attrs);
    vms_writeio(cio->sysin_chan, &cio->iosb, str, strlen(str));
    return texu_cio_get_color(cio, WHITE_BLACK);
}

texu_i32
texu_cio_putstr_attr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs)
{
    return texu_cio_putstr_attr_erase(cio, y, x, str, attrs, TEXU_FALSE);
}
#else

texu_i32
texu_cio_putstr_attr_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs, texu_bool erase)
{
    texu_cio_attron(cio, attrs);
    mvwprintw(cio->wndscr, y, x, str);
    return texu_cio_attroff(cio, attrs);
}
texu_i32
texu_cio_putstr_attr(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs)
{
    return texu_cio_putstr_attr_erase(cio, y, x, str, attrs, TEXU_FALSE);
}

#endif

#if defined WIN32
#include <strsafe.h>

void GetLastErrorMsg(LPCTSTR lpszFunction, DWORD dwError)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
 
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dwError, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

}
#endif
texu_i32 texu_cio_get_keyname(texu_cio* cio, texu_i32 keycode, texu_char* outname, texu_i32 len)
{
    texu_i32 rc = -1;
#if (defined __VMS__)
    texu_char           key_name[32];
    $DESCRIPTOR(keydesc, key_name);

    memset(key_name, 0, sizeof(key_name));
    smg$keycode_to_name(&keycode, &keydesc);
    
    texu_strncpy(outname, key_name, len);
    rc = texu_strlen(outname);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
    return rc;
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
    UINT scancode = MapVirtualKey(keycode, MAPVK_VK_TO_VSC);
    rc = GetKeyNameText(scancode << 16, outname, len);
    if (0 == rc)
    {
        GetLastErrorMsg("texu_cio_keyname", GetLastError());
    }
#else
    texu_char* keypressed = keyname(keycode);
    texu_strncpy(outname, keypressed, len);
    rc = texu_strlen(outname);
#endif
    return rc;
}

texu_i32
texu_cio_draw_text(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
    texu_ui32 color, texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32 id)
{
#if (defined WIN32 && defined _WINDOWS)
    return texu_env_draw_text_ex(cio->env, y, x, text, color, bgcolor,
                                    clsname, id);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#if defined __USE_TERMBOX2__
    texu_tb2_putstr(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#else
    texu_termios_putstr(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#endif
    return 0;
#elif (defined __USE_TTY__)
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, cio__color_pair(color)));
#elif (defined __VMS__)
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, color));
#else
#if defined __LINUX__
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, color));
#else
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, color));
#endif
#endif
}

texu_i32
texu_cio_draw_text2(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
    texu_ui32 color, texu_ui32 bgcolor, texu_ui32 attrs,
    const texu_char *clsname,
    texu_ui32 id)
{
#if (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#if defined __USE_TERMBOX2__
    texu_tb2_putstr_attrs(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor),
        attrs);
#else
    texu_termios_putstr_attrs(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor),
        attrs);
#endif
    return 0;
#else
    return texu_cio_draw_text(cio, y, x, text, color, bgcolor, clsname, id);
#endif
}

texu_i32
texu_cio_draw_text_erase(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *text,
    texu_ui32 color, texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32 id,
    texu_bool erase)
{
#if (defined WIN32 && defined _WINDOWS)
    return texu_env_draw_text_ex(cio->env, y, x, text, color, bgcolor,
                                    clsname, id);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#if defined __USE_TERMBOX2__
    texu_tb2_putstr(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#else
    texu_termios_putstr(y, x, text, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#endif
    return 0;
#elif (defined __USE_TTY__)
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, cio__color_pair(color)));
#elif (defined __VMS__)
    return texu_cio_putstr_attr_erase(cio, y, x, text,
            texu_cio_get_color(cio, color), erase);
#else
#if defined __LINUX__
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, color));
#else
    return texu_cio_putstr_attr(cio, y, x, text,
            texu_cio_get_color(cio, color));
#endif
#endif
}

texu_i32
texu_cio_draw_char(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
    texu_ui32 color, texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32 id)
{
#if (defined WIN32 && defined _WINDOWS)
    return texu_env_draw_char_ex(cio->env, y, x, ch, color, bgcolor,
                                 clsname, id);
#elif (defined __USE_TERMIOS__)
    texu_termios_putch(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
    return 0;
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
    texu_tb2_putch(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
    return 0;
#elif (defined __USE_TTY__)
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, cio__color_pair(color)));
#elif (defined __VMS__)
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, color));
#else
#if defined __LINUX__
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, color));
#else
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, color));
#endif
#endif
}

texu_i32
texu_cio_draw_char2(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
    texu_ui32 color, texu_ui32 bgcolor, texu_ui32 attrs,
    const texu_char *clsname,
    texu_ui32 id)
{
#if (defined __USE_TERMIOS__)
    texu_termios_putch_attrs(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor),
        attrs);
    return 0;
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
    texu_tb2_putch_attrs(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor),
        attrs);
    return 0;
#else
    return texu_cio_draw_char(cio, y, x, ch, color, bgcolor, clsname, id);
#endif
}

texu_i32
texu_cio_draw_char_erase(texu_cio *cio, texu_i32 y, texu_i32 x, texu_char ch,
    texu_ui32 color, texu_ui32 bgcolor,
    const texu_char *clsname,
    texu_ui32 id,
    texu_bool erase)
{
#if (defined WIN32 && defined _WINDOWS)
    return texu_env_draw_char_ex(cio->env, y, x, ch, color, bgcolor,
                                 clsname, id);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#if defined __USE_TERMBOX2__
    texu_tb2_putch(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#else
    texu_termios_putch(y, x, ch, 
        cio__color_pair(color), 
        cio__bgcolor_pair(bgcolor));
#endif
    return 0;
#elif (defined __USE_TTY__)
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, cio__color_pair(color)));
#elif (defined __VMS__)
    return texu_cio_putch_attr_erase(cio, y, x, ch, 
            texu_cio_get_color(cio, color), erase);
#else
#if defined __LINUX__
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, color));
#else
    return texu_cio_putch_attr(cio, y, x, ch, 
            texu_cio_get_color(cio, color));
#endif
#endif
}

#ifdef __USE_TTY__
texu_i32
texu_cio_putstr_attr2(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putstr_attr(cio, y, x, str, attrs);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_putstr_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_i32 fg, texu_i32 bg)
{
#if defined __USE_TERMBOX2__
    return texu_tb2_putstr(y, x, str, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
#else
    return texu_termios_putstr(y, x, str, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
#endif
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_putstr_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putstr_attr(cio, y, x, str, attrs);
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_putstr_attr2(texu_cio* cio, texu_i32 y, texu_i32 x, const texu_char* str, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putstr_attr(cio, y, x, str, attrs);
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_putstr_attr2(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putstr_attr(cio, y, x, str, attrs);
}
#else
texu_i32
texu_cio_putstr_attr2(texu_cio *cio, texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 color, texu_i32 attrs)
{
    return texu_cio_putstr_attr(cio, y, x, str, color | attrs);
}
#endif


#if (defined __USE_TTY__ || defined __VMS__)
texu_i32
texu_cio_attron(texu_cio *cio, texu_i32 attrs)
{
    return texu_tty_attron(attrs);
}

texu_i32
texu_cio_attroff(texu_cio *cio, texu_i32 attrs)
{
    return texu_tty_attroff();
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_attron(texu_cio *cio, texu_i32 attrs)
{
    return 0;
}

texu_i32
texu_cio_attroff(texu_cio *cio, texu_i32 attrs)
{
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_attron(texu_cio *cio, texu_i32 attrs)
{
    return 0;
}

texu_i32
texu_cio_attroff(texu_cio *cio, texu_i32 attrs)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_attron(texu_cio* cio, texu_i32 attrs)
{
    return 0;
}

texu_i32
texu_cio_attroff(texu_cio* cio, texu_i32 attrs)
{
    return 0;
}
#else
#if /*defined __LINUX__ &&*/ defined __USE_CURSES__
texu_i32
texu_cio_attron(texu_cio *cio, texu_i32 attrs)
{
    return wattron(cio->wndscr, attrs);
}

texu_i32
texu_cio_attroff(texu_cio *cio, texu_i32 attrs)
{
    return wattroff(cio->wndscr, attrs);
}
#else
texu_i32
texu_cio_attron(texu_cio *cio, texu_i32 attrs)
{
    return texu_tty_attron(attrs);
}

texu_i32
texu_cio_attroff(texu_cio *cio, texu_i32 attrs)
{
    return texu_tty_attroff();
}
#endif
#endif

#ifdef __USE_TTY__
texu_i32
texu_cio_refresh(texu_cio *cio)
{
    return 0;
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_refresh(texu_cio* cio)
{
#if defined __USE_TERMBOX2__
    return texu_tb2_refresh();
#else
    return texu_termios_refresh();
#endif
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_refresh(texu_cio* cio)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_refresh(texu_cio *cio)
{
    return 0;
}
#elif (defined VMS || defined __VMS__)
texu_i32
texu_cio_refresh(texu_cio *cio)
{
    return smg$repaint_screen(&cio->smg_display_id);
}
#else
texu_i32
texu_cio_refresh(texu_cio *cio)
{
    return wrefresh(cio->wndscr);
}
#endif

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_frame(texu_cio *cio, const texu_char *text, texu_rect *rect, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_frame(texu_cio *cio, const texu_char *text, texu_rect *rect, texu_i32 attrs)
#endif
{
    texu_i32 len = 0;

    texu_cio_draw_rect(cio, rect, fg, bg, attrs);

    if (text)
    {
#if defined TEXU_CIO_COLOR_MONO
        texu_char caption[TEXU_MAX_WNDTEXT + 1];
#endif
        len = texu_strlen(text);

#if (defined __USE_TTY__)
        texu_cio_get_reverse(cio, attrs);
        texu_cio_putstr_attr(cio, rect->y,
                             rect->x + (rect->cols - len) / 2, text, attrs);
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
#if defined TEXU_CIO_COLOR_MONO
        sprintf(caption, "[ %s ]", text);
        len = texu_strlen(caption);
#if defined __USE_TERMBOX2__
        texu_tb2_putstr_attrs(rect->y,
            rect->x + (rect->cols - len) / 2, caption, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#else
        texu_termios_putstr(rect->y,
            rect->x + (rect->cols - len) / 2, caption, 
            cio__color_pair(attrs), 
            cio__bgcolor_pair(attrs));
#endif
#else
#if defined __USE_TERMBOX2__
        texu_tb2_putstr_attrs(rect->y,
            rect->x + (rect->cols - len) / 2, text,
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#else
        texu_termios_putstr(rect->y,
            rect->x + (rect->cols - len) / 2, text,
            cio__color_pair(attrs), 
            cio__bgcolor_pair(attrs));
#endif
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
        texu_cio_get_reverse(cio, attrs);
        texu_cio_putstr_attr(cio, rect->y,
            rect->x + (rect->cols - len) / 2, text, attrs);
#elif (defined __VMS__)
        texu_cio_putstr_attr(cio, rect->y,
                             rect->x + (rect->cols - len) / 2, text, 
                             texu_cio_get_color(cio, cio__reverse_color_pair(attrs)));
#else
#if 0//defined __LINUX__
        texu_cio_putstr_attr(cio, rect->y,
                             rect->x + (rect->cols - len) / 2, text, attrs | A_REVERSE);
#else
#if 0 //defined TEXU_CIO_COLOR_MONO
        sprintf(caption, "[ %s ]", text);
        len = texu_strlen(caption);
        //texu_strcpy(text, caption);

//        texu_cio_attron(cio, A_REVERSE);
        texu_cio_putstr_attr(cio, rect->y,
                             rect->x + (rect->cols - len) / 2, caption, 
                             /*attrs | */A_REVERSE);
                             //texu_cio_get_color(cio, cio__reverse_color_pair(attrs)));
//        texu_cio_attroff(cio, A_REVERSE);
#else
        texu_cio_putstr_attr(cio, rect->y,
                             rect->x + (rect->cols - len) / 2, text, 
                             texu_cio_get_color(cio, cio__reverse_color_pair(attrs)));
#endif
#endif
#endif
    }
    return TEXU_OK;
}

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_rect(texu_cio *cio, texu_rect *rect, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_rect(texu_cio *cio, texu_rect *rect, texu_i32 attrs)
#endif
{
#if 0//(defined __VMS__)
    texu_cio_get_color(cio, cio__color_pair(attrs));
    texu_i32 status = smg$draw_rectangle(&cio->smg_display_id, &rect->y+1, &rect->x+1, &rect->cols, &rect->lines, &0, &0);
    texu_cio_get_color(cio, WHITE_BLACK);
#else
    texu_cio_attron(cio, attrs);
#if (defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
    /*draw top line*/
    texu_cio_draw_line(cio, rect->y, rect->x, rect->cols, fg, bg, attrs);
    /*draw bottom line*/
    texu_cio_draw_line(cio, rect->y + rect->lines - 1, rect->x, rect->cols, fg, bg, attrs);
    /* draw left vertical line */
    texu_cio_draw_vline(cio, rect->y, rect->x, rect->lines, fg, bg, attrs);
    /* draw right vertical line */
    texu_cio_draw_vline(cio, rect->y, rect->x + rect->cols - 1, rect->lines, fg, bg, attrs);
    /* draw upper left*/
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    /* draw lower left*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, '+', attrs);
    /* draw upper right*/
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, '+', attrs);
    /* draw lower right*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x + rect->cols - 1, '+', attrs);
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
#if defined __UTF8__
    /*draw top line*/
    texu_cio_draw_line_utf8(cio, rect->y, rect->x, rect->cols, fg, bg, TEXU_TB2_HORZLINE);
    /*draw bottom line*/
    texu_cio_draw_line_utf8(cio, rect->y + rect->lines - 1, rect->x, rect->cols, fg, bg, TEXU_TB2_HORZLINE);
    /* draw left vertical line */
    texu_cio_draw_vline_utf8(cio, rect->y, rect->x, rect->lines, fg, bg, TEXU_TB2_VERTLINE);
    /* draw right vertical line */
    texu_cio_draw_vline_utf8(cio, rect->y, rect->x + rect->cols - 1, rect->lines, fg, bg, TEXU_TB2_VERTLINE);
    /* draw upper left*/
    tb_set_cell(rect->x, rect->y, TEXU_TB2_UPPERLEFT, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
    /* draw lower left*/
    tb_set_cell(rect->x, rect->y + rect->lines - 1, TEXU_TB2_LOWERLEFT, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
    /* draw upper right*/
    tb_set_cell(rect->x + rect->cols - 1, rect->y, TEXU_TB2_UPPERRIGHT,
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
    /* draw lower right*/
    tb_set_cell(rect->x + rect->cols - 1, rect->y + rect->lines - 1, TEXU_TB2_LOWERRIGHT,
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
#else
    /*draw top line*/
    texu_cio_draw_line(cio, rect->y, rect->x, rect->cols, fg, bg, attrs);
    /*draw bottom line*/
    texu_cio_draw_line(cio, rect->y + rect->lines - 1, rect->x, rect->cols, fg, bg, attrs);
    /* draw left vertical line */
    texu_cio_draw_vline(cio, rect->y, rect->x, rect->lines, fg, bg, attrs);
    /* draw right vertical line */
    texu_cio_draw_vline(cio, rect->y, rect->x + rect->cols - 1, rect->lines, fg, bg, attrs);
    /* draw upper left*/
    texu_tb2_putstr_attrs(
        rect->y, rect->x, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
    /* draw lower left*/
    texu_tb2_putstr_attrs(
        rect->y + rect->lines - 1, rect->x, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
    /* draw upper right*/
    texu_tb2_putstr_attrs(
        rect->y, rect->x + rect->cols - 1, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
    /* draw lower right*/
    texu_tb2_putstr_attrs(
        rect->y + rect->lines - 1, rect->x + rect->cols - 1, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
    /* draw upper left*/
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    /* draw lower left*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, '+', attrs);
    /* draw upper right*/
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, '+', attrs);
    /* draw lower right*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x + rect->cols - 1, '+', attrs);
#else
    /* draw upper left*/
    texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
    /* draw lower left*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, ACS_LLCORNER, attrs);
    /* draw upper right*/
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, ACS_URCORNER, attrs);
    /* draw lower right*/
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x + rect->cols - 1, ACS_LRCORNER, attrs);
#endif

    return texu_cio_attroff(cio, attrs);
#endif /*__VMS__*/
    return 0;
}

#if defined __UTF8__
texu_i32 texu_cio_draw_line_utf8(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 fg, texu_i32 bg, texu_i32 ch)
{
    texu_i32 i = 0;
    for (i = 0; i < width; ++i)
    {
        tb_set_cell(x + i, y, ch, cio__color_pair(fg), cio__bgcolor_pair(bg));
    }
    return 0;
}
texu_i32 texu_cio_draw_vline_utf8(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 ch)
{
    texu_i32 i = 0;
    for (i = 0; i < height; ++i)
    {
        tb_set_cell(x, y + i, ch, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
    }
    return 0;
}
texu_i32 texu_cio_draw_box_utf8(texu_cio* cio, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 ch)
{
    texu_i32 i = 0;
    for (i = 0; i < height; ++i)
    {
        texu_cio_draw_line_utf8(cio, y + i, x, width, fg, bg, ch);
    }
    return 0;
}

#endif

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_line(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_line(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 width, texu_i32 attrs)
#endif
{
    texu_char line[TEXU_MAX_WNDTEXT + 1];
    memset(line, 0, sizeof(line));
#if (defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
    memset(line, '-', width);
    texu_cio_putstr_attr(cio, y, x, line, attrs);
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
#if (defined __UTF8__)
    texu_i32 i = 0;
    for (i = 0; i < width; ++i)
    {
        tb_set_cell(x + i, y, TEXU_TB2_HORZLINE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
    }
    return 0;
#else
    memset(line, '-', width);
    texu_tb2_putstr_attrs(y, x, line,
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
    memset(line, '-', width);
    texu_cio_putstr_attr(cio, y, x, line, attrs);
#else
    memset(line, ACS_HLINE, width);
    texu_cio_putstr_attr(cio, y, x, line, attrs);
#endif

    return TEXU_OK;//texu_cio_attroff(cio, attrs);
}

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_vline(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_vline(texu_cio *cio, texu_i32 y, texu_i32 x, texu_i32 height, texu_i32 attrs)
#endif
{
    texu_i32 i = 0;
    texu_cio_attron(cio, attrs);
    for (i = 0; i < height; ++i)
    {
#if (defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
        texu_cio_putch_attr(cio, y + i, x, '|', attrs);
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
#if (defined __UTF8__)
        tb_set_cell(x, y + i, TEXU_TB2_VERTLINE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
#else
        texu_tb2_putstr_attrs(y + i, x, "|",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
        texu_cio_putch_attr(cio, y + i, x, '|', attrs);
#else
        texu_cio_putch_attr(cio, y + i, x, ACS_VLINE, attrs);
#endif
    }
    return texu_cio_attroff(cio, attrs);
}

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_hrects(texu_cio *cio, texu_rect *rect, texu_i32 *widths, texu_i32 nwidth, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_hrects(texu_cio *cio, texu_rect *rect, texu_i32 *widths, texu_i32 nwidth, texu_i32 attrs)
#endif
{
    texu_i32 i = 0;
    texu_rect rc = *rect;

    for (i = 0; i < nwidth; ++i)
    {
        rc.cols = widths[i];
        texu_cio_draw_rect(cio, &rc, fg, bg, attrs);
#if (defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
        texu_cio_putch_attr(cio, rc.y, rc.x, '+', attrs);
        texu_cio_putch_attr(cio, rc.y + rc.lines - 1, rc.x, '+', attrs);
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
#if (defined __UTF8__)
        tb_set_cell(rc.x, rc.y, TEXU_TB2_TOPTEE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
        tb_set_cell(rc.x, rc.y + rc.lines - 1, TEXU_TB2_BOTTOMTEE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
#else
        texu_tb2_putstr_attrs(rc.y, rc.x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
        texu_tb2_putstr_attrs(rc.y + rc.lines - 1, rc.x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
        texu_cio_putch_attr(cio, rc.y, rc.x, '+', attrs);
        texu_cio_putch_attr(cio, rc.y + rc.lines - 1, rc.x, '+', attrs);
#else
        texu_cio_putch_attr(cio, rc.y, rc.x, ACS_TTEE, attrs);
        texu_cio_putch_attr(cio, rc.y + rc.lines - 1, rc.x, ACS_BTEE, attrs);
#endif
        rc.x += widths[i] - 1;
    }
#if (defined TEXU_CIO_COLOR_MONO || defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, '+', attrs);
#elif (defined __USE_TERMBOX2__)
#if (defined __UTF8__)
    tb_set_cell(rect->x, rect->y, TEXU_TB2_UPPERLEFT, 
        cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
    tb_set_cell(rect->x, rect->y + rect->lines - 1, TEXU_TB2_LOWERLEFT, 
        cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
#else
        texu_tb2_putstr_attrs(rect->y, rect->x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
        texu_tb2_putstr_attrs(rect->y + rect->lines - 1, rect->x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, '+', attrs);
#else
    texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
    texu_cio_putch_attr(cio, rect->y + rect->lines - 1, rect->x, ACS_LLCORNER, attrs);
#endif
    return TEXU_OK;
}

texu_i32
#if 1//(defined __USE_TERMBOX2__)
texu_cio_draw_vrects(texu_cio *cio, texu_rect *rect, texu_i32 *heights, texu_i32 nheight, texu_i32 fg, texu_i32 bg, texu_i32 attrs)
#else
texu_cio_draw_vrects(texu_cio *cio, texu_rect *rect, texu_i32 *heights, texu_i32 nheight, texu_i32 attrs)
#endif
{
    texu_i32 i = 0;
    texu_rect rc = *rect;

    for (i = 0; i < nheight; ++i)
    {
        rc.lines = heights[i];
        texu_cio_draw_rect(cio, &rc, fg, bg, attrs);

#if (defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
        texu_cio_putch_attr(cio, rc.y, rc.x, '+', attrs);
        texu_cio_putch_attr(cio, rc.y + rc.lines - 1, rc.x, '+', attrs);
#elif (defined __USE_TERMBOX2__ || defined TEXU_CIO_COLOR_MONO)
#if (defined __UTF8__)
        tb_set_cell(rc.x, rc.y, TEXU_TB2_LEFTTEE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
        tb_set_cell(rc.x, rc.y + rc.lines - 1, TEXU_TB2_RIGHTTEE, 
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg));
#else
        texu_tb2_putstr_attrs(rc.y, rc.x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
        texu_tb2_putstr_attrs(rc.y + rc.lines - 1, rc.x, "+",
            cio__color_pair(fg), 
            cio__bgcolor_pair(bg),
            attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
        texu_cio_putch_attr(cio, rc.y, rc.x, '+', attrs);
        texu_cio_putch_attr(cio, rc.y + rc.lines - 1, rc.x, '+', attrs);
#else
        texu_cio_putch_attr(cio, rc.y, rc.x, ACS_LTEE, attrs);
        texu_cio_putch_attr(cio, rc.y, rc.x + rc.cols - 1, ACS_RTEE, attrs);
#endif
        rc.y += heights[i] - 1;
    }
#if (defined TEXU_CIO_COLOR_MONO || defined __USE_TTY__ || defined __VMS__ || defined __USE_TERMIOS__)
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, '+', attrs);
#elif (defined __USE_TERMBOX2__)
#if (defined __UTF8__)
    tb_set_cell(rect->x, rect->y, TEXU_TB2_UPPERLEFT, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
    tb_set_cell(rect->x + rect->cols - 1, rect->y, TEXU_TB2_UPPERRIGHT, 
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg));
#else
    texu_tb2_putstr_attrs(rect->y, rect->x, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
    texu_tb2_putstr_attrs(rect->y, rect->x + rect->cols - 1, "+",
        cio__color_pair(fg), 
        cio__bgcolor_pair(bg),
        attrs);
#endif
#elif (defined WIN32 && defined _WINDOWS)
#elif (defined WIN32 && defined _CONSOLE)
    texu_cio_putch_attr(cio, rect->y, rect->x, '+', attrs);
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, '+', attrs);
#else
    texu_cio_putch_attr(cio, rect->y, rect->x, ACS_ULCORNER, attrs);
    texu_cio_putch_attr(cio, rect->y, rect->x + rect->cols - 1, ACS_URCORNER, attrs);
#endif
    return TEXU_OK;
}
#if (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_get_bgcolor(texu_cio *cio, texu_i32 clridx)
{
    return cio__bgcolor_pair(clridx);
}

texu_i32
texu_cio_get_bgreverse(texu_cio *cio, texu_i32 clridx)
{
    return cio__bgcolor_pair(clridx);/*|TB_REVERSE;*/
}

#endif

#if (defined __USE_TTY__)
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_color(clridx);
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_underline(clridx);
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_reverse(clridx);
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_blink(clridx);
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    return cio__color_pair(clridx);
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    return cio__color_pair(clridx)|TB_UNDERLINE;
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    return cio__bgcolor_pair(clridx);//|TB_REVERSE;
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    return cio__bgcolor_pair(clridx);//|TB_UNDERLINE;
}

#elif (defined __VMS__)
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    /*return texu_tty_get_color(clridx);*/
    texu_char color[32];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].fg, gcolors[clridx].bg);
    printf(color);
/*//core dump
    texu_cio_putstr(cio, -1, -1, color);
    */
    return 0;
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_underline(clridx);
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    /*return texu_tty_get_reverse(clridx);*/
    texu_char color[16];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].rfg, gcolors[clridx].rbg);
    printf(color);
    return 0;
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    return texu_tty_get_blink(clridx);
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_get_color(texu_cio* cio, texu_i32 clridx)
{
    return 0;
}

texu_i32
texu_cio_get_underline(texu_cio* cio, texu_i32 clridx)
{
    return 0;
}

texu_i32
texu_cio_get_reverse(texu_cio* cio, texu_i32 clridx)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    return cio__color_pair(clridx);
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    return cio__color_pair(clridx);
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    return cio__reverse_color_pair(clridx);
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    return 0;
}
#else/*curses*/

#if defined __LINUX__
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    return COLOR_PAIR(clridx);
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    return COLOR_PAIR(clridx) | A_UNDERLINE;
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    return COLOR_PAIR(clridx) | A_REVERSE;
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    return COLOR_PAIR(clridx) | A_BLINK;
}
#else
texu_i32
texu_cio_get_color(texu_cio *cio, texu_i32 clridx)
{
    /*return texu_tty_get_color(clridx);*/
    texu_char color[16];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].fg, gcolors[clridx].bg);
    printf(color);
    return gcolors[clridx].fg;
}

texu_i32
texu_cio_get_underline(texu_cio *cio, texu_i32 clridx)
{
    texu_char color[16];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].fg, gcolors[clridx].bg);
    printf(color);
    return gcolors[clridx].fg | A_UNDERLINE;
}

texu_i32
texu_cio_get_reverse(texu_cio *cio, texu_i32 clridx)
{
    /*return texu_tty_get_reverse(clridx);*/
    texu_char color[16];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].rfg, gcolors[clridx].rbg);
    printf(color);
    return gcolors[clridx].rfg | A_REVERSE;
}

texu_i32
texu_cio_get_blink(texu_cio *cio, texu_i32 clridx)
{
    texu_char color[16];
    sprintf(color, TTY_FMT_COLOR_LL, gcolors[clridx].fg, gcolors[clridx].bg);
    printf(color);
    return gcolors[clridx].fg | A_BLINK;
}

#endif
#endif

#if (defined __USE_TTY__ || defined __VMS__)
texu_i32
texu_cio_get_color_attr(texu_cio *cio, texu_i32 clridx, texu_i32 attrs)
{
    return texu_tty_get_color(clridx);
}

texu_i32
texu_cio_save_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}

texu_i32
texu_cio_restore_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}
#elif (defined __USE_TERMIOS__ || defined TEXU_CIO_COLOR_MONO || defined __USE_TERMBOX2__)
texu_i32
texu_cio_get_color_attr(texu_cio *cio, texu_i32 clridx, texu_i32 attrs)
{
    return cio__color_pair(clridx)|attrs;
}

texu_i32
texu_cio_save_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}

texu_i32
texu_cio_restore_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}
#elif (defined WIN32 && defined _WINDOWS)
texu_i32
texu_cio_get_color_attr(texu_cio *cio, texu_i32 clridx, texu_i32 attrs)
{
    return 0;
}

texu_i32
texu_cio_save_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}

texu_i32
texu_cio_restore_screen(texu_cio *cio, FILE *fp)
{
    return 0;
}
#elif (defined WIN32 && defined _CONSOLE)
texu_i32
texu_cio_get_color_attr(texu_cio* cio, texu_i32 clridx, texu_i32 attrs)
{
    return 0;
}

texu_i32
texu_cio_save_screen(texu_cio* cio, FILE* fp)
{
    return 0;
}

texu_i32
texu_cio_restore_screen(texu_cio* cio, FILE* fp)
{
    return 0;
}
#else
texu_i32
texu_cio_get_color_attr(texu_cio *cio, texu_i32 clridx, texu_i32 attrs)
{
    return COLOR_PAIR(clridx) | attrs;
}

texu_i32
texu_cio_save_screen(texu_cio *cio, FILE *fp)
{
    return putwin(cio->wndscr, fp);
}

texu_i32
texu_cio_restore_screen(texu_cio *cio, FILE *fp)
{
    WINDOW *wndscr = getwin(fp);
    return wrefresh(wndscr);
}
#endif

#ifdef __cplusplus
}
#endif
