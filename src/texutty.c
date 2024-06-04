/*
#
# File Name: texutty.c
# Author: Seree Rakwong
# Date: 09-NOV-2022
#
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined __UNIX__
#include <unistd.h>
#endif

#include "texumacros.h"
#include "texutypes.h"
#include "texuconst.h"
#include "texutty.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if !(defined VMS || defined __VMS__ || defined _WIN32)
#include <termios.h>

struct termios old_kbd_mode;    /* orig keyboard settings   */
struct termios new_kbd_mode;    /* new  keyboard settings   */

texu_i32 texu_tty_init()
{
    if (tcgetattr(0, &old_kbd_mode))
    { /* save orig settings   */
        /*fprintf(stderr, "%s() error: tcgetattr failed.\n", __func__);*/
        return -1;
    }   /* copy old to new */
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));

    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);  /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN]  = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode))
    {
        /*fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);*/
        return -1;
    }
    return 0;
}

texu_i32 texu_tty_release()
{
    texu_tty_echo();
    if (tcsetattr(0, TCSANOW, &old_kbd_mode))
    {
        /*fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);*/
        return -1;
    }
    return 0;
}

texu_i32 texu_tty_echo()
{
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));
    
    new_kbd_mode.c_lflag |= (ICANON | ECHO);  /* new kbd flags */
    /*new_kbd_mode.c_lflag |= ECHO;*/
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN]  = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode))
    {
        /*fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);*/
        return -1;
    }
    return 0;
}


texu_i32 texu_tty_noecho()
{
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));
    
    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);  /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN]  = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode))
    {
        /*fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);*/
        return -1;
    }
    return 0;
}

texu_i32 texu_tty_getch(texu_i32 sec, texu_i32 usec)
{
    texu_char buf = 0;
    struct timeval tv = { sec, usec };
    fd_set rfds;
    int rc = -1;
    texu_i32 ch = -1;
    
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    if (sec < 0 && usec < 0)
    {
        /*no timeout*/
        rc = select(1, &rfds, NULL, NULL, NULL);
    }
    else
    {
        rc = select(1, &rfds, NULL, NULL, &tv);
    }

    /*no errors and timeout */
    if (rc && (read(0, &buf, 1) > 0))
    {
        ch = (texu_i32)buf;
    }
    
    return ch;
}
#endif /*!__VMS__*/
struct _tty_color_pairs
{
    texu_i32     id;
    const texu_char    *color;
};
typedef struct _tty_color_pairs tty_color_pairs;

tty_color_pairs a_tty_color_pairs[] = 
{
    { TEXU_CIO_COLOR_BLACK_BLACK,       TTY_COLOR_BLACK_BLACK   },
    { TEXU_CIO_COLOR_BLACK_RED,         TTY_COLOR_BLACK_RED     },
    { TEXU_CIO_COLOR_BLACK_GREEN,       TTY_COLOR_BLACK_GREEN   },
    { TEXU_CIO_COLOR_BLACK_YELLOW,      TTY_COLOR_BLACK_YELLOW  },
    { TEXU_CIO_COLOR_BLACK_BLUE,        TTY_COLOR_BLACK_BLUE    },
    { TEXU_CIO_COLOR_BLACK_MAGENTA,     TTY_COLOR_BLACK_MAGENTA },
    { TEXU_CIO_COLOR_BLACK_CYAN,        TTY_COLOR_BLACK_CYAN  },
    { TEXU_CIO_COLOR_BLACK_WHITE,       TTY_COLOR_BLACK_WHITE },
    { TEXU_CIO_COLOR_RED_BLACK,         TTY_COLOR_RED_BLACK   },
    { TEXU_CIO_COLOR_RED_RED,           TTY_COLOR_RED_RED     },
    { TEXU_CIO_COLOR_RED_GREEN,         TTY_COLOR_RED_GREEN   },
    { TEXU_CIO_COLOR_RED_YELLOW,        TTY_COLOR_RED_YELLOW  },
    { TEXU_CIO_COLOR_RED_BLUE,          TTY_COLOR_RED_BLUE    },
    { TEXU_CIO_COLOR_RED_MAGENTA,       TTY_COLOR_RED_MAGENTA },
    { TEXU_CIO_COLOR_RED_CYAN,          TTY_COLOR_RED_CYAN    },
    { TEXU_CIO_COLOR_RED_WHITE,         TTY_COLOR_RED_WHITE   },
    { TEXU_CIO_COLOR_GREEN_BLACK,       TTY_COLOR_GREEN_BLACK   },
    { TEXU_CIO_COLOR_GREEN_RED,         TTY_COLOR_GREEN_RED     },
    { TEXU_CIO_COLOR_GREEN_GREEN,       TTY_COLOR_GREEN_GREEN   },
    { TEXU_CIO_COLOR_GREEN_YELLOW,      TTY_COLOR_GREEN_YELLOW  },
    { TEXU_CIO_COLOR_GREEN_BLUE,        TTY_COLOR_GREEN_BLUE    },
    { TEXU_CIO_COLOR_GREEN_MAGENTA,     TTY_COLOR_GREEN_MAGENTA },
    { TEXU_CIO_COLOR_GREEN_CYAN,        TTY_COLOR_GREEN_CYAN    },
    { TEXU_CIO_COLOR_GREEN_WHITE,       TTY_COLOR_GREEN_WHITE   },
    { TEXU_CIO_COLOR_YELLOW_BLACK,      TTY_COLOR_YELLOW_BLACK   },
    { TEXU_CIO_COLOR_YELLOW_RED,        TTY_COLOR_YELLOW_RED     },
    { TEXU_CIO_COLOR_YELLOW_GREEN,      TTY_COLOR_YELLOW_GREEN   },
    { TEXU_CIO_COLOR_YELLOW_YELLOW,     TTY_COLOR_YELLOW_YELLOW  },
    { TEXU_CIO_COLOR_YELLOW_BLUE,       TTY_COLOR_YELLOW_BLUE    },
    { TEXU_CIO_COLOR_YELLOW_MAGENTA,    TTY_COLOR_YELLOW_MAGENTA },
    { TEXU_CIO_COLOR_YELLOW_CYAN,       TTY_COLOR_YELLOW_CYAN    },
    { TEXU_CIO_COLOR_YELLOW_WHITE,      TTY_COLOR_YELLOW_WHITE   },
    { TEXU_CIO_COLOR_BLUE_BLACK,        TTY_COLOR_BLUE_BLACK   },
    { TEXU_CIO_COLOR_BLUE_RED,          TTY_COLOR_BLUE_RED     },
    { TEXU_CIO_COLOR_BLUE_GREEN,        TTY_COLOR_BLUE_GREEN   },
    { TEXU_CIO_COLOR_BLUE_YELLOW,       TTY_COLOR_BLUE_YELLOW  },
    { TEXU_CIO_COLOR_BLUE_BLUE,         TTY_COLOR_BLUE_BLUE    },
    { TEXU_CIO_COLOR_BLUE_MAGENTA,      TTY_COLOR_BLUE_MAGENTA },
    { TEXU_CIO_COLOR_BLUE_CYAN,         TTY_COLOR_BLUE_CYAN    },
    { TEXU_CIO_COLOR_BLUE_WHITE,        TTY_COLOR_BLUE_WHITE   },
    { TEXU_CIO_COLOR_MAGENTA_BLACK,     TTY_COLOR_MAGENTA_BLACK   },
    { TEXU_CIO_COLOR_MAGENTA_RED,       TTY_COLOR_MAGENTA_RED     },
    { TEXU_CIO_COLOR_MAGENTA_GREEN,     TTY_COLOR_MAGENTA_GREEN   },
    { TEXU_CIO_COLOR_MAGENTA_YELLOW,    TTY_COLOR_MAGENTA_YELLOW  },
    { TEXU_CIO_COLOR_MAGENTA_BLUE,      TTY_COLOR_MAGENTA_BLUE    },
    { TEXU_CIO_COLOR_MAGENTA_MAGENTA,   TTY_COLOR_MAGENTA_MAGENTA },
    { TEXU_CIO_COLOR_MAGENTA_CYAN,      TTY_COLOR_MAGENTA_CYAN    },
    { TEXU_CIO_COLOR_MAGENTA_WHITE,     TTY_COLOR_MAGENTA_WHITE   },
    { TEXU_CIO_COLOR_CYAN_BLACK,        TTY_COLOR_CYAN_BLACK    },
    { TEXU_CIO_COLOR_CYAN_RED,          TTY_COLOR_CYAN_RED      },
    { TEXU_CIO_COLOR_CYAN_GREEN,        TTY_COLOR_CYAN_GREEN    },
    { TEXU_CIO_COLOR_CYAN_YELLOW,       TTY_COLOR_CYAN_YELLOW   },
    { TEXU_CIO_COLOR_CYAN_BLUE,         TTY_COLOR_CYAN_BLUE     },
    { TEXU_CIO_COLOR_CYAN_MAGENTA,      TTY_COLOR_CYAN_MAGENTA  },
    { TEXU_CIO_COLOR_CYAN_CYAN,         TTY_COLOR_CYAN_CYAN     },
    { TEXU_CIO_COLOR_CYAN_WHITE,        TTY_COLOR_CYAN_WHITE    },
    { TEXU_CIO_COLOR_WHITE_BLACK,       TTY_COLOR_WHITE_BLACK   },
    { TEXU_CIO_COLOR_WHITE_RED,         TTY_COLOR_WHITE_RED     },
    { TEXU_CIO_COLOR_WHITE_GREEN,       TTY_COLOR_WHITE_GREEN   },
    { TEXU_CIO_COLOR_WHITE_YELLOW,      TTY_COLOR_WHITE_YELLOW  },
    { TEXU_CIO_COLOR_WHITE_BLUE,        TTY_COLOR_WHITE_BLUE    },
    { TEXU_CIO_COLOR_WHITE_MAGENTA,     TTY_COLOR_WHITE_MAGENTA },
    { TEXU_CIO_COLOR_WHITE_CYAN,        TTY_COLOR_WHITE_CYAN    },
    { TEXU_CIO_COLOR_WHITE_WHITE,       TTY_COLOR_WHITE_WHITE   },
    { TEXU_CIO_BRIGHT_WHITE_BLACK,      TTY_COLOR_BRIGHT_WHITE_BLACK   },
    { TEXU_CIO_BRIGHT_WHITE_RED,        TTY_COLOR_BRIGHT_WHITE_RED     },
    { TEXU_CIO_BRIGHT_WHITE_GREEN,      TTY_COLOR_BRIGHT_WHITE_GREEN   },
    { TEXU_CIO_BRIGHT_WHITE_YELLOW,     TTY_COLOR_BRIGHT_WHITE_YELLOW  },
    { TEXU_CIO_BRIGHT_WHITE_BLUE,       TTY_COLOR_BRIGHT_WHITE_BLUE    },
    { TEXU_CIO_BRIGHT_WHITE_MAGENTA,    TTY_COLOR_BRIGHT_WHITE_MAGENTA },
    { TEXU_CIO_BRIGHT_WHITE_CYAN,       TTY_COLOR_BRIGHT_WHITE_CYAN    },
    { TEXU_CIO_BRIGHT_WHITE_WHITE,      TTY_COLOR_BRIGHT_WHITE_WHITE   },
    { TEXU_CIO_BLACK_BRIGHT_WHITE,      TTY_COLOR_BLACK_BRIGHT_WHITE    },
    { TEXU_CIO_RED_BRIGHT_WHITE,        TTY_COLOR_RED_BRIGHT_WHITE      },
    { TEXU_CIO_GREEN_BRIGHT_WHITE,      TTY_COLOR_GREEN_BRIGHT_WHITE    },
    { TEXU_CIO_YELLOW_BRIGHT_WHITE,     TTY_COLOR_YELLOW_BRIGHT_WHITE   },
    { TEXU_CIO_BLUE_BRIGHT_WHITE,       TTY_COLOR_BLUE_BRIGHT_WHITE     },
    { TEXU_CIO_MAGENTA_BRIGHT_WHITE,    TTY_COLOR_MAGENTA_BRIGHT_WHITE  },
    { TEXU_CIO_CYAN_BRIGHT_WHITE,       TTY_COLOR_CYAN_BRIGHT_WHITE     },
};

void
texu_tty_init_colors()
{
}

texu_i32
texu_tty_clear()
{
    printf(TTY_CLEAR_SCREEN);
    texu_tty_gotoyx(0, 0);
    return 0;
}

texu_i32
texu_tty_clearln(texu_i32 y)
{
    texu_tty_gotoyx(y, 0);
    printf(TTY_CLEAR_LINE);
    texu_tty_gotoyx(y, 0);
    return 0;
}

texu_i32
texu_tty_get_color(texu_i32 clridx)
{
    /*return COLOR_PAIR(clridx);*/
    if (clridx < 0 || clridx >= TEXU_CIO_LAST_COLOR)
    {
        clridx = TEXU_CIO_BRIGHT_WHITE_BLACK;
    }
    printf(a_tty_color_pairs[clridx].color);
    return 0;
}

texu_i32
texu_tty_get_reverse(texu_i32 clridx)
{
    printf(TTY_ATTR_REVERSE);
    return texu_tty_get_color(clridx);
}

texu_i32
texu_tty_get_underline(texu_i32 clridx)
{
    printf(TTY_ATTR_UNDERLINE);
    return texu_tty_get_color(clridx);
}

texu_i32
texu_tty_get_blink(texu_i32 clridx)
{
    printf(TTY_ATTR_BLINK);
    return texu_tty_get_color(clridx);
}

texu_i32
texu_tty_get_italic(texu_i32 clridx)
{
    printf(TTY_ATTR_ITALIC);
    return texu_tty_get_color(clridx);
}

texu_i32
texu_tty_gotoyx(texu_i32 y, texu_i32 x)
{
    char sz[256];
    sprintf(sz, TTY_GOTOYX_LL, y, x);
    printf(sz);
    return 0;
}


texu_i32
texu_tty_attron(texu_i32 attrs)
{
    if (attrs < 0 || attrs >= TEXU_CIO_LAST_COLOR)
    {
        attrs = TEXU_CIO_BRIGHT_WHITE_BLACK;
    }
    printf(a_tty_color_pairs[attrs].color);
    return 0;
}

texu_i32
texu_tty_attroff()
{
    printf(TTY_ATTR_OFF);
    return 0;
}

texu_i32
texu_tty_putch(texu_i32 y, texu_i32 x, texu_i32 ch)
{
    /*char buf[2] = { ch, 0 }*/
    texu_tty_gotoyx(y, x);
    /*write(1, &buf[0], 1);*/
    printf("%c", ch);
    return 0;
}

texu_i32
texu_tty_putch_attr(texu_i32 y, texu_i32 x, texu_i32 ch, texu_i32 attrs)
{
    texu_tty_attron(attrs);
    texu_tty_putch(y, x, ch);
    return texu_tty_attroff();
}

texu_i32
texu_tty_putstr(texu_i32 y, texu_i32 x, const texu_char *str)
{
    texu_tty_gotoyx(y, x);
    /*write(1, &buf[0], 1);*/
    printf("%s", str);
    return 0;
}

texu_i32
texu_tty_putstr_attr(texu_i32 y, texu_i32 x, const texu_char *str, texu_i32 attrs)
{
    texu_tty_attron(attrs);
    texu_tty_putstr(y, x, str);
    return texu_tty_attroff();
}



#ifdef __cplusplus
}
#endif



