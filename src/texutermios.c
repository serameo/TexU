#include "texutermios.h"
#include <stdio.h>
#include <stdarg.h>

#if defined __USE_TERMIOS__

#ifdef __cplusplus
extern "C" {
#endif
static void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg)
{
    while (*str) 
    {
        struct tb_cell cell = { *str, fg, bg };
        uint32_t uni;
        uint16_t fg2 = fg;
        uint16_t bg2 = bg;
        str += tb_utf8_char_to_unicode(&uni, str);

        tb_get_cell(x, y, &cell);
        if((uint16_t)(-1) == fg) fg2 = cell.fg;
        if((uint16_t)(-1) == bg) bg2 = cell.bg;
        tb_change_cell(x, y, uni, fg2, bg2);
        x++;
    }
}
/*
    struct tb_cell cell = { ch, fg, bg };
    tb_get_cell(x, y, &cell);

    if((uint16_t)(-1) == fg) fg = cell.fg;
    if((uint16_t)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, ch, fg, bg);
*/
static void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt, ...)
{
    char buf[4096];
    va_list vl;
    va_start(vl, fmt);
    vsnprintf(buf, sizeof(buf), fmt, vl);
    va_end(vl);
    print_tb(buf, x, y, fg, bg);
}

int texu_termios_init(int lines, int cols)
{
    int rc = tb_init();
    if (0 == rc)
    {
        /*tb_set_term_size(cols, lines);*/
    }
    return rc;
}

void texu_termios_release()
{
    tb_shutdown();
}

int
texu_termios_getch(struct tb_event *ev, int timeout)
{
    while (tb_peek_event(ev, timeout))
    {
        switch (ev->type)
        {
            case TB_EVENT_KEY:
                /*return (ev->key > 0 ? ev->key : ev->ch);*/
                {
                    return (ev->key > 0 ? ev->key : ev->ch);
                }
            case TB_EVENT_RESIZE:
            case TB_EVENT_MOUSE:
                break;
        }
    }
    return -1;
}

void texu_termios_gotoyx(int y, int x)
{
    tb_set_cursor(x, y);
}

void texu_termios_clear()
{
    tb_clear();
}

int texu_termios_putch(int y, int x, char ch, uint16_t fg, uint16_t bg)
{
    uint32_t uni;
    struct tb_cell cell = { ch, fg, bg };
    char str[4] = { ch, 0, 0, 0 };
    tb_utf8_char_to_unicode(&uni, str);

    tb_get_cell(x, y, &cell);

    if((uint16_t)(-1) == fg) fg = cell.fg;
    if((uint16_t)(-1) == bg) bg = cell.bg;
    tb_change_cell(x, y, uni, fg, bg);
    return 0;
}


int texu_termios_putstr(int y, int x, const char* str, uint16_t fg, uint16_t bg)
{
    print_tb(str, x, y, fg, bg);
    return 0;
}

int texu_termios_refresh()
{
    tb_present();
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /*__USE_TERMIOS__*/