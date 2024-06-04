#if defined _WIN32
#include <windows.h>
#include <commctrl.h>
#endif
#include "texu.h"
#include "texukeys.h"
#include <stdlib.h>
#include <string.h>

#define MyWndClass  TEXUTEXT("MyWndClass")
#define MyWndClass2 TEXUTEXT("MyWndClass2")
#define MyWndClass3 TEXUTEXT("MyWndClass3")
#define MyWndClass4 TEXUTEXT("MyWndClass4")
#define MyWndClass5 TEXUTEXT("MyWndClass5")
#define MyWndClass5_Page1 TEXUTEXT("MyWndClass5_Page1")
#define MyWndClass5_Page2 TEXUTEXT("MyWndClass5_Page2")

#define ID_HELP     (TEXU_WM_USER + 1)
#define ID_ADD      (TEXU_WM_USER + 2)
#define ID_DELETE   (TEXU_WM_USER + 3)
#define ID_FIND     (TEXU_WM_USER + 4)
#define ID_MSGBOX   (TEXU_WM_USER + 5)
#define IDC_STATUSBAR       12
#define IDC_UPDOWN          15
#define IDC_PROGRESSBAR     16
#define ID_MAINMENU         1000
/*
#define IDC_LIST1           7
#define IDC_LIST2           8
#define IDC_LIST3           9
*/
#define IDC_LABEL1          201
#define IDC_LABEL2          202
#define IDC_LABEL3          203
#define IDC_LABEL4          204
#define IDC_LABEL5          205
#define IDC_LABEL6          206
#define IDC_LABEL7          207
#define IDC_LABEL8          208
#define IDC_LABEL9          209
#define IDC_LABEL10         210
#define IDC_PANEL1          211
#define IDC_PANEL2          212
#define IDC_PANEL3          213
#define IDC_PANEL4          214
#define IDC_PANEL5          215
#define IDC_PANEL6          216
#define IDC_PANEL7          217
#define IDC_PANEL8          218
#define IDC_PANEL9          219
#define IDC_PANEL10         220

/*#define IDC_REBAR1          221
#define IDC_REBAR2          222
#define IDC_REBAR3          223
#define IDC_REBAR4          224
#define IDC_REBAR5          225
#define IDC_REBAR6          226
#define IDC_REBAR7          227
#define IDC_REBAR8          228
#define IDC_REBAR9          229
#define IDC_REBAR10         230
*/

texu_lparam MyWndProc(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc2(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc3(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc4(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc5(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc5_Page1(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
texu_lparam MyWndProc5_Page2(texu_wnd *, texu_ui32, texu_lparam, texu_lparam);
/*
struct texu_wnd_template
{
const texu_char* text;
const texu_char* clsname;
texu_ui32  style;
texu_ui32  exstyle;
texu_i32   y;
texu_i32   x;
texu_i32   h;
texu_i32   w;
texu_i32   id;
};
*/

//#define IDC_LABEL1      200
#define IDC_EDIT1       201
#define IDC_UPDOWN1     202
//#define IDC_LABEL2      203
#define IDC_EDIT2       204
#define IDC_PROGRESSBAR1 205
//#define IDC_LABEL3      206
#define IDC_EDIT3       207
#define IDC_LIST1       208
#define IDC_LIST2       209
#define IDC_LIST3       210
#define IDC_BUTTON1     211
#define IDC_BUTTON2     212
#define IDC_BUTTON3     213
#define IDC_BUTTON4     214
#define IDC_STATUSBAR1  215
#define IDC_PAGECTRL1   216
#define IDC_PAGE1       217
#define IDC_PAGE2       218
#define IDC_PAGE3       219
#define IDC_PAGE4       220
#define IDC_COMBO1      221
#define IDC_IPADDRESS1  222
#define IDC_EDITMASK1   223

#define IDC_REBAR1 300
#define IDC_REBAR2 301
#define IDC_CHILD_REBAR1    311
#define IDC_CHILD_REBAR2    312
#define IDC_CHILD_REBAR3    313
#define IDC_CHILD_REBAR4    314
#define IDC_CHILD_REBAR5    315
#define IDC_CHILD_REBAR6    316 
#define IDC_CHILD_REBAR7    317 
#define IDC_CHILD_REBAR8    318 
#define IDC_CHILD_REBAR9    319 
#define IDC_CHILD_REBAR10   3110

texu_wnd_template templ5[] =
{
    { TEXUTEXT("Label1:"), TEXU_LABEL_CLASS, TEXU_WS_RIGHT, 0, 1, 0, 1, 20, IDC_LABEL1 },
    { TEXUTEXT("Edit1"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_A2Z | TEXU_ES_CENTER, 0, 1, 21, 1, 10, IDC_EDIT1 },
    { TEXUTEXT("0"), TEXU_UPDOWNCTRL_CLASS, 0, 0, 1, 42, 1, 10, IDC_UPDOWN1 },
    { TEXUTEXT("0.0.0.0"), TEXU_IPADDRESSCTRL_CLASS, 0, 0, 1, 52, 1, 16, IDC_IPADDRESS1 },
    { TEXUTEXT("Label2:"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
    { TEXUTEXT("Edit2"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
    { TEXUTEXT(""), TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
    { TEXUTEXT("Label3:"), TEXU_LABEL_CLASS, TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
    { TEXUTEXT("Edit3"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, 0, 0, 4, 0, 5, 20, IDC_LIST1 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, TEXU_LBS_RADIOBOX, 0, 4, 21, 5, 20, IDC_LIST2 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, TEXU_LBS_CHECKBOX, 0, 4, 42, 5, 20, IDC_LIST3 },
    { TEXUTEXT("F1 - Help"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
    { TEXUTEXT("F2 - Add"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
    { TEXUTEXT("F3 - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
    { TEXUTEXT("Alt+X - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
    { TEXUTEXT(""), TEXU_STATUSBAR_CLASS, 0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
    /*last control is nil*/
    { 0, 0, 0, 0, 0, 0, 0, 0 } };

texu_wnd_template templ5_p1[] =
{
    { TEXUTEXT("Label2:"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
    { TEXUTEXT("Edit2"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
    { TEXUTEXT(""), TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
    { TEXUTEXT("Label3:"), TEXU_LABEL_CLASS, TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
    { TEXUTEXT("Edit3"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, 0, 0, 4, 0, 5, 20, IDC_LIST1 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, TEXU_LBS_RADIOBOX, 0, 4, 21, 5, 20, IDC_LIST2 },
    { TEXUTEXT(""), TEXU_LISTBOX_CLASS, TEXU_LBS_CHECKBOX, 0, 4, 42, 5, 20, IDC_LIST3 },
    { TEXUTEXT("F1 - Help"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
    { TEXUTEXT("F2 - Add"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
    { TEXUTEXT("F3 - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
    { TEXUTEXT("Alt+X - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
    { TEXUTEXT(""), TEXU_STATUSBAR_CLASS, 0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
    /*last control is nil*/
    { 0, 0, 0, 0, 0, 0, 0, 0 } };

texu_wnd_template templ5_p2[] =
{
    { TEXUTEXT("Label1:"), TEXU_LABEL_CLASS, TEXU_WS_RIGHT, 0, 1, 0, 1, 20, IDC_LABEL1 },
    { TEXUTEXT("Edit1"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_A2Z, 0, 1, 21, 1, 10, IDC_EDIT1 },
    { TEXUTEXT("0"), TEXU_UPDOWNCTRL_CLASS, 0, 0, 1, 42, 1, 10, IDC_UPDOWN1 },
    { TEXUTEXT("0.0.0.0"), TEXU_IPADDRESSCTRL_CLASS, 0, 0, 1, 52, 1, 16, IDC_IPADDRESS1 },
    { TEXUTEXT("Label2:"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 2, 1, 1, 20, IDC_LABEL2 },
    { TEXUTEXT("Edit2"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER, 0, 2, 21, 1, 10, IDC_EDIT2 },
    { TEXUTEXT(""), TEXU_PROGRESSBAR_CLASS, 0, 0, 2, 42, 1, 30, IDC_PROGRESSBAR1 },
    { TEXUTEXT("Label3:"), TEXU_LABEL_CLASS, TEXU_WS_LEFT, 0, 3, 0, 1, 20, IDC_LABEL3 },
    { TEXUTEXT("Edit3"), TEXU_EDIT_CLASS, TEXU_ES_AUTOHSCROLL | TEXU_ES_DECIMAL, 0, 3, 21, 1, 10, IDC_EDIT3 },
    { TEXUTEXT("F1 - Help"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 0, 1, 16, IDC_BUTTON1 },
    { TEXUTEXT("F2 - Add"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 20, 1, 16, IDC_BUTTON2 },
    { TEXUTEXT("F3 - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 40, 1, 16, IDC_BUTTON3 },
    { TEXUTEXT("Alt+X - Exit"), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 23, 60, 1, 16, IDC_BUTTON4 },
    { TEXUTEXT(""), TEXU_STATUSBAR_CLASS, 0, 0, 24, 0, 1, 100, IDC_STATUSBAR1 },
    /*last control is nil*/
    { 0, 0, 0, 0, 0, 0, 0, 0 } };

texu_i32
SetMyThemeColor(texu_i32 sysidx)
{
    switch (sysidx)
    {
        /*
        case TEXU_COLOR_WINDOW:                   return TEXU_CIO_COLOR_WHITE_BLACK;
        case TEXU_COLOR_DESKTOP:                  return TEXU_CIO_COLOR_WHITE_BLACK;
        case TEXU_COLOR_DIALOG:                   return TEXU_CIO_COLOR_BLACK_WHITE;
        case TEXU_COLOR_TITLE_WINDOW:             return TEXU_CIO_COLOR_WHITE_BLUE;
        case TEXU_COLOR_BUTTON_OK:                return TEXU_CIO_COLOR_BLACK_GREEN;
        case TEXU_COLOR_BUTTON_YES:               return TEXU_CIO_COLOR_WHITE_BLUE;
        case TEXU_COLOR_BUTTON_NO:                return TEXU_CIO_COLOR_BLACK_RED;
        case TEXU_COLOR_BUTTON_CANCEL:            return TEXU_CIO_COLOR_BLACK_YELLOW;
        */
        case TEXU_COLOR_MENU:
            return TEXU_CIO_COLOR_RED_WHITE;
        case TEXU_COLOR_MENU_DISABLED:
            return TEXU_CIO_COLOR_BLACK_WHITE;
        case TEXU_COLOR_MENU_SELECTED:
            return TEXU_CIO_COLOR_RED_CYAN;
        case TEXU_COLOR_MENUITEM:
            return TEXU_CIO_COLOR_RED_WHITE;
        case TEXU_COLOR_MENUITEM_DISABLED:
            return TEXU_CIO_COLOR_BLACK_WHITE;
        case TEXU_COLOR_MENUITEM_SELECTED:
            return TEXU_CIO_COLOR_WHITE_RED;
            /*
            case TEXU_COLOR_LABEL:                    return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LABEL_DISABLED:           return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_STATUSBAR:                return TEXU_CIO_COLOR_WHITE_BLUE;
            case TEXU_COLOR_EDIT:                     return TEXU_CIO_COLOR_CYAN_BLACK;
            case TEXU_COLOR_EDIT_DISABLED:            return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_EDIT_SELECTED:            return TEXU_CIO_COLOR_BLACK_CYAN;
            case TEXU_COLOR_LISTBOX:                  return TEXU_CIO_COLOR_CYAN_BLACK;
            case TEXU_COLOR_LISTBOX_DISABLED:         return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTBOX_SELECTED:         return TEXU_CIO_COLOR_BLUE_CYAN;
            case TEXU_COLOR_COMBOBOX:                 return TEXU_CIO_COLOR_BLUE_WHITE;
            case TEXU_COLOR_COMBOBOX_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_COMBOBOX_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLUE;
            case TEXU_COLOR_LISTCTRL:                 return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTCTRL_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTCTRL_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTCTRL_ITEM:            return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTCTRL_ITEM_DISABLED:   return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_LISTCTRL_ITEM_SELECTED:   return TEXU_CIO_COLOR_BLUE_CYAN;
            case TEXU_COLOR_TREECTRL:                 return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_TREECTRL_DISABLED:        return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_TREECTRL_SELECTED:        return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_TREECTRL_ITEM:            return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_TREECTRL_ITEM_DISABLED:   return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_TREECTRL_ITEM_SELECTED:   return TEXU_CIO_COLOR_BLUE_CYAN;
            case TEXU_COLOR_UPDOWNCTRL:               return TEXU_CIO_COLOR_BLUE_WHITE;
            case TEXU_COLOR_UPDOWNCTRL_DISABLED:      return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_UPDOWNCTRL_SELECTED:      return TEXU_CIO_COLOR_BLUE_CYAN;
            case TEXU_COLOR_PROGRESSBAR:              return TEXU_CIO_COLOR_CYAN_BLACK;
            case TEXU_COLOR_PROGRESSBAR_DISABLED:     return TEXU_CIO_COLOR_WHITE_BLACK;
            case TEXU_COLOR_PROGRESSBAR_SELECTED:     return TEXU_CIO_COLOR_BLUE_CYAN;
            case TEXU_COLOR_DEFAULT:                  return TEXU_CIO_COLOR_WHITE_BLACK;
            */
    }
    return -1;
}

#if defined _WIN32
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PSTR pszCmdLine, INT iCmdShow)
{
    texu_wnd *wnd = 0;
    texu_status rc = TEXU_OK;

    TexuStartup(hInstance, 25, 100);
    TexuCreateMainEnvWnd(0,
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                         0);
#else
int main(int argc, char* argv[])
{
    texu_wnd *wnd = 0;
    texu_status rc = TEXU_OK;

    TexuStartup(25, 100);
#endif
    TexuSetThemeColor(SetMyThemeColor);

    rc = TexuRegisterClass(
        MyWndClass,
        MyWndProc);
    rc = TexuRegisterClass(
        MyWndClass2,
        MyWndProc2);
    rc = TexuRegisterClass(
        MyWndClass3,
        MyWndProc3);
    rc = TexuRegisterClass(
        MyWndClass4,
        MyWndProc4);

    rc = TexuRegisterClass(
        MyWndClass5,
        MyWndProc5);
    rc = TexuRegisterClass(
        MyWndClass5_Page1,
        MyWndProc5_Page1);
    rc = TexuRegisterClass(
        MyWndClass5_Page2,
        MyWndProc5_Page2);

    wnd = TexuCreateWindow(
        TEXUTEXT("Test TexU App"),
        MyWndClass5,
        0, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        1, /* id */
        0  /* user data */
        );

    /*TexuUnlockUpdate();*/
    TexuShowWindow(wnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(wnd);

    TexuRun();

    TexuShutdown();
    return 0;
}

void _MyWndProc_OnHelp(texu_wnd *wnd)
{
    texu_wnd *ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
    texu_wnd *newwnd = 0;

    TexuSetWindowText(ctl, TEXUTEXT("Pressed F1 to help"));

    newwnd = TexuCreateWindow(
        TEXUTEXT("Test TexU App - window 2"),
        MyWndClass5,
        0, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        2, /* id */
        0  /* user data */
        );

    TexuShowWindow(newwnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(newwnd);
}

void _MyWndProc_OnMsgBox(texu_wnd *wnd)
{
    TexuMessageBox(
        TEXUTEXT("Hello world"),
        TEXUTEXT("Do you want to update user?"),
        wnd,
        1001, /* id */
        TEXU_MBS_OKCANCEL,
        0 /* user data */
        );
}

void _MyWndProc_OnAdd_test(texu_wnd *wnd)
{
    texu_wnd *ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
    texu_wnd *newwnd = 0;

    TexuSetWindowText(ctl, TEXUTEXT("Pressed F2 to add"));

    newwnd = TexuCreateWindow(
        TEXUTEXT("Test TexU App - window 2"),
        MyWndClass2,
        0, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        2, /* id */
        0  /* user data */
        );

    TexuShowWindow(newwnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(newwnd);
}

void _MyWndProc_OnExit(texu_wnd *wnd)
{
    TexuExit();
}

void _MyWndProc_OnNotify(texu_wnd *wnd, texu_wnd_notify *notify)
{
    texu_lbwnd_notify *lbntf = (texu_lbwnd_notify *)notify;
    texu_char text[TEXU_MAX_WNDTEXT + 1];
    texu_wnd* panel = TexuGetWindowItem(wnd, IDC_PANEL1);
    texu_wnd *status = TexuGetWindowItem(panel, IDC_STATUSBAR);
    texu_wnd *lb = TexuGetWindowItem(panel, IDC_LIST1);
    texu_wnd *udctl = TexuGetWindowItem(panel, IDC_UPDOWN);
    texu_wnd *pgb = TexuGetWindowItem(panel, IDC_PROGRESSBAR1);
    texu_i32 val = 0;
    texu_msgbox_notify *msg = 0;
    texu_menuitem_notify *menuitem = 0;

    memset(text, 0, sizeof(text));
    if (notify->id == IDC_LIST1)
    {
        TexuSendMessage(lb, TEXU_LBM_GETITEMTEXT, (texu_lparam)lbntf->index, (texu_lparam)text);
        TexuSetWindowText(status, text);
    }

    if (notify->id == IDC_UPDOWN)
    {
        val = TexuSendMessage(udctl, TEXU_UDCM_GETINT, 0, 0);
        /*TexuSaveCursorPosition(udctl);*/
        TexuSendMessage(pgb, TEXU_PGBM_SETPOS, val, 0);
        /*TexuRestoreCursorPosition(udctl);*/

        TexuSendMessage(pgb, TEXU_WM_GETTEXT, (texu_lparam)text, TEXU_MAX_WNDTEXT);
        TexuSendMessage(status, TEXU_SBM_SETTEXT, 1, (texu_lparam)text);
    }

    if (notify->code == TEXU_MBN_ENDDIALOG)
    {
        msg = (texu_msgbox_notify *)notify;
        val = msg->id;

        switch (val)
        {
            case TEXU_IDOK:
                texu_strcpy(text, TEXUTEXT("PRESSED OK"));
                break;
            case TEXU_IDCANCEL:
                texu_strcpy(text, TEXUTEXT("PRESSED CANCEL"));
                break;
            case TEXU_IDYES:
                texu_strcpy(text, TEXUTEXT("PRESSED YES"));
                break;
            case TEXU_IDNO:
                texu_strcpy(text, TEXUTEXT("PRESSED NO"));
                break;
        }
        TexuSetWindowText(status, text);
    }

    if (notify->code == TEXU_MNN_ITEMCHANGED)
    {
        menuitem = (texu_menuitem_notify *)notify;
        texu_strcpy(text, menuitem->info);
        TexuSetWindowText(status, text);
    }
}

void _MyWndProc_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_rect rect = { 9, 0, 4, 99 };
    texu_i32 widths[4] = { 10, 20, 30, 39 };

    texu_rect rect2 = { 16, 3, 5, 90 };
    texu_i32 heights[2] = { 4, 4 };

    texu_rect rect3 = { 14, 0, 10, 99 };
#if (defined WIN32 && defined _WINDOWS)
    /*    TexuDrawRect(dc, &rect, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
    TexuDrawFrame(dc, TEXUTEXT("hello world"), &rect, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);*/
    TexuDrawHRects(dc, &rect2, heights, 2,
                   TEXU_CIO_COLOR_RED_WHITE,
                   TEXU_CIO_COLOR_WHITE_BLUE);

    TexuDrawVRects(dc, &rect, widths, 4,
                   TEXU_CIO_COLOR_GREEN_WHITE,
                   TEXU_CIO_COLOR_WHITE_BLUE);

    TexuDrawFrame(dc, TEXUTEXT("hello world"), &rect3,
                  TEXU_CIO_COLOR_BLUE_WHITE,
                  TEXU_CIO_COLOR_WHITE_BLUE);
#else
    TexuDrawHRects(dc, &rect, widths, 4,
                   TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));

    TexuDrawVRects(dc, &rect2, heights, 2,
                   TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_YELLOW));
#endif
}

texu_status _MyWndProc_OnCreate1(texu_wnd *wnd)
{
    /* to create a simple control */
    TexuCreateControls(wnd, templ5, 50);
    return TEXU_OK;
}

texu_menu_template mainmenu1[] =
{
    {/*File*/
        TEXUTEXT(" File "),
        TEXU_TRUE,
        TEXUTEXT("File"),
        6,
        {
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| New          |"), ID_ADD, TEXU_TRUE, TEXUTEXT("New window") },
            { TEXUTEXT("| Open         |"), 2, TEXU_TRUE, TEXUTEXT("Open window") },
            { TEXUTEXT("|--------------|"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| Exit   Alt+X |"), ID_DELETE, TEXU_TRUE, TEXUTEXT("Exit a program") },
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
        }
    },
    {/*Edit*/
        TEXUTEXT(" Edit "),
        TEXU_TRUE,
        TEXUTEXT("Edit"),
        5,
        {
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| Cut          |"), 11, TEXU_TRUE, TEXUTEXT("Cut selection") },
            { TEXUTEXT("| Copy         |"), 12, TEXU_TRUE, TEXUTEXT("Copy selection") },
            { TEXUTEXT("| Paste        |"), 13, TEXU_TRUE, TEXUTEXT("Paste selection copied") },
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
        }
    },
    {/*Search*/
        TEXUTEXT(" Search "),
        TEXU_TRUE,
        TEXUTEXT("Search"),
        4,
        {
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| Find         |"), 21, TEXU_TRUE, TEXUTEXT("Find text") },
            { TEXUTEXT("| Find Next    |"), 22, TEXU_TRUE, TEXUTEXT("Find next text") },
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
        }
    },
    {/*View*/
        TEXUTEXT(" View "),
        TEXU_TRUE,
        TEXUTEXT("View"),
        5,
        {
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| Help      F1 |"), ID_HELP, TEXU_TRUE, TEXUTEXT("") },
            { TEXUTEXT("|--------------|"), 0, TEXU_FALSE, TEXUTEXT("") },
            { TEXUTEXT("| About        |"), ID_MSGBOX, TEXU_TRUE, TEXUTEXT("About test_texu demo") },
            { TEXUTEXT("+--------------+"), 0, TEXU_FALSE, TEXUTEXT("") },
        }
    }
};

texu_status _MyWndProc_OnCreate(texu_wnd *wnd)
{
    texu_wnd *child = 0;
    texu_i32 starty = 1;
    texu_menu *menu = 0;
    /*texu_popup_menu *item = 0;*/

    /*    menu = TexuCreateMenus(wnd, ID_MAINMENU, mainmenu1, 4);*/
#if 1
    menu = TexuCreateMenu(wnd, ID_MAINMENU);
    texu_popup_menu *item = TexuAddPopupMenu(menu, TEXUTEXT(" File "), TEXU_TRUE, TEXUTEXT("File"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" New          "), ID_ADD, TEXU_TRUE, TEXUTEXT("New window"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Open         "), 2, TEXU_TRUE, TEXUTEXT("Open window"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT("--------------"), -1, TEXU_FALSE, TEXUTEXT(""));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Exit   Alt+X "), ID_DELETE, TEXU_TRUE, TEXUTEXT("Exit a program"));

    item = TexuAddPopupMenu(menu, TEXUTEXT(" Edit "), TEXU_TRUE, TEXUTEXT("Edit"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Cut          "), 11, TEXU_TRUE, TEXUTEXT("Cut selection"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Copy         "), 12, TEXU_TRUE, TEXUTEXT("Copy selection"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Paste        "), 13, TEXU_TRUE, TEXUTEXT("Paste selection copied"));

    item = TexuAddPopupMenu(menu, TEXUTEXT(" Search "), TEXU_TRUE, TEXUTEXT("Search"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Find         "), 21, TEXU_TRUE, TEXUTEXT("Find text"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Find Next    "), 22, TEXU_TRUE, TEXUTEXT("Find next text"));
    TexuEnablePopupMenu(menu, 2, TEXU_FALSE);

    item = TexuAddPopupMenu(menu, TEXUTEXT(" View "), TEXU_TRUE, TEXUTEXT("View"));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" Help      F1 "), ID_HELP, TEXU_TRUE, TEXUTEXT(""));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT("--------------"), -1, TEXU_FALSE, TEXUTEXT(""));
    TexuAddPopupMenuItem(menu, item, TEXUTEXT(" About        "), ID_MSGBOX, TEXU_TRUE, TEXUTEXT("About test_texu demo"));
#endif
    /*TexuEnablePopupMenu(menu, 2, TEXU_FALSE);*/

    texu_wnd* panel = TexuCreateWindow(
        TEXUTEXT("Text:"),
        TEXU_PANEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        0,         /* y */
        0,              /* x */
        25,
        100,
        wnd, /* parent */
        IDC_PANEL1,   /* id */
        0    /* user data */
        );

    child = TexuCreateWindow(
        TEXUTEXT("Text:"),
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        1,         /* y */
        0,              /* x */
        1,
        20,
        panel, /* parent */
        1,   /* id */
        0    /* user data */
        );
    child = TexuCreateWindow(
        TEXUTEXT("Text"),
        TEXU_EDIT_CLASS,
        TEXU_ES_AUTOHSCROLL | TEXU_ES_A2Z | TEXU_ES_CENTER, /* style*/
        0,                                                  /* exstyle*/
        starty,                                             /* y */
        21,                                                 /* x */
        1,
        10,
        panel, /* parent */
        2,   /* id */
        0    /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
    TexuSetBgColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

    child = TexuCreateWindow(
        TEXUTEXT("Text"),
        TEXU_UPDOWNCTRL_CLASS,
        TEXU_UDS_SHOWPLUSMINUS,      /* style*/
        0,      /* exstyle*/
        starty, /* y */
        42,     /* x */
        1,
        5,
        panel,        /* parent */
        IDC_UPDOWN, /* id */
        0           /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
    TexuSetBgColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

    child = TexuCreateWindow(
        TEXUTEXT("Text"),
        TEXU_IPADDRESSCTRL_CLASS,
        0,      /* style*/
        0,      /* exstyle*/
        starty, /* y */
        53,     /* x */
        1,
        16,
        panel,        /* parent */
        IDC_IPADDRESS1, /* id */
        0           /* user data */
        );

    child = TexuCreateWindow(
        TEXUTEXT(""),
        TEXU_EDITMASKCTRL_CLASS,
        0,      /* style*/
        0,      /* exstyle*/
        starty, /* y */
        70,     /* x */
        1,
        30,
        panel,        /* parent */
        IDC_EDITMASK1, /* id */
        0           /* user data */
        );
    texu_wnd_send_msg(child, TEXU_EMM_SETMASK, (texu_lparam)TEXU_EDITMASK_EMAIL, 0);
    texu_wnd_send_msg(child, TEXU_EMM_SETINFO, (texu_lparam)TEXUTEXT("email:abc@xyz.com"), 0);


    child = TexuCreateWindow(
        TEXUTEXT("Number:"),
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 1,     /* y */
        0,              /* x */
        1,
        20,
        panel, /* parent */
        3,   /* id */
        0    /* user data */
        );
    child = TexuCreateWindow(
        TEXUTEXT("-1234"),
        TEXU_EDIT_CLASS,
        TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER | TEXU_ES_AUTOCOMMAS, /* style*/
        0,                                    /* exstyle*/
        starty + 1,                           /* y */
        21,                                   /* x */
        1,
        10,
        panel, /* parent */
        4,   /* id */
        0    /* user data */
        );

    child = TexuCreateWindow(
        TEXUTEXT(""),
        TEXU_PROGRESSBAR_CLASS,
        0,          /* style*/
        0,          /* exstyle*/
        starty + 1, /* y */
        42,         /* x */
        1,
        30,
        panel,             /* parent */
        IDC_PROGRESSBAR1, /* id */
        0                /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
    TexuSendMessage(child, TEXU_PGBM_SETMAX, 255, 0);

    child = TexuCreateWindow(
        TEXUTEXT("Decimal:"),
        TEXU_LABEL_CLASS,
        0,          /* style*/
        0,          /* exstyle*/
        starty + 2, /* y */
        0,          /* x */
        1,
        10,
        panel, /* parent */
        5,   /* id */
        0    /* user data */
        );
    child = TexuCreateWindow(
        TEXUTEXT("1.234"),
        TEXU_EDIT_CLASS,
        TEXU_ES_AUTOHSCROLL | TEXU_ES_DECIMAL | TEXU_ES_AUTOCOMMAS, /* style*/
        0,                                     /* exstyle*/
        starty + 2,                            /* y */
        21,                                    /* x */
        1,                                     /* height */
        10,                                    /* width */
        panel,                                   /* parent */
        6,                                     /* id */
        0                                      /* user data */
        );
    texu_editminmax mm = { -5000000.0, 5000000.0 };
    TexuSendMessage(child, TEXU_EM_SETVALIDMINMAX, TEXU_TRUE, (texu_lparam)&mm);

    child = TexuCreateWindow(
        TEXUTEXT("combo1"),
        TEXU_COMBOBOX_CLASS,
        0,          /* style*/
        0,          /* exstyle*/
        starty + 2, /* y */
        42,         /* x */
        1,          /* height */
        20,         /* width */
        panel,        /* parent */
        IDC_COMBO1, /* id */
        0           /* user data */
        );
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo1"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo2"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo3"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo4"), 0);

    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo5"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo6"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo7"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo8"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("combo9"), 0);
    TexuSendMessage(child, TEXU_CBM_ADDITEM, (texu_lparam)TEXUTEXT("comboA"), 0);
    /*
    child = (texu_wnd *)TexuSendMessage(child, TEXU_CBM_GETLISTBOX, 0, 0);
    TexuSetColor(child, TEXU_CIO_COLOR_BLUE_WHITE, TEXU_CIO_COLOR_BLUE_WHITE);
    TexuSetBgColor(child, TEXU_CIO_COLOR_BLUE_WHITE, TEXU_CIO_COLOR_BLUE_WHITE);
    TexuSendMessage(child, TEXU_LBM_SETSELCOLOR, TEXU_CIO_COLOR_BLUE_CYAN, 0);*/

    child = TexuCreateWindow(
        TEXUTEXT(""),
        TEXU_LISTBOX_CLASS,
        0,          /* style*/
        0,          /* exstyle*/
        starty + 3, /* y */
        0,          /* x */
        5,          /* height */
        20,         /* width */
        panel,        /* parent */
        IDC_LIST1,  /* id */
        0           /* user data */
        );
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

    TexuSetColor(child, TEXU_CIO_COLOR_BLUE_WHITE, TEXU_CIO_COLOR_WHITE_BLACK);
    TexuSetBgColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_BLACK_WHITE);
    /*TexuSendMessage(child, TEXU_LBM_SETSELCOLOR, TEXU_CIO_COLOR_CYAN_BLUE, TEXU_CIO_COLOR_CYAN_BLUE);*/

    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)2, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)3, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)5, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)7, (texu_lparam)TEXU_FALSE);

    child = TexuCreateWindow(
        TEXUTEXT("1.234"),
        TEXU_LISTBOX_CLASS,
        TEXU_LBS_RADIOBOX, /* style*/
        0,                 /* exstyle*/
        starty + 3,        /* y */
        21,                /* x */
        5,                 /* height */
        20,                /* width */
        panel,               /* parent */
        8,                 /* id */
        0                  /* user data */
        );
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)2, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)3, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)5, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)7, (texu_lparam)TEXU_FALSE);
#if 1
    child = TexuCreateWindow(
        TEXUTEXT("1.234"),
        TEXU_LISTBOX_CLASS,
        TEXU_LBS_CHECKBOX, /* style*/
        0,                 /* exstyle*/
        starty + 3,        /* y */
        42,                /* x */
        5,                 /* height */
        20,                /* width */
        panel,               /* parent */
        9,                 /* id */
        0                  /* user data */
        );
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
    TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)2, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)3, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)5, (texu_lparam)TEXU_FALSE);
    TexuSendMessage(child, TEXU_LBM_ENABLEITEM, (texu_lparam)7, (texu_lparam)TEXU_FALSE);
#endif
#if 0
    child = TexuCreateWindow(
        "F1 - Help",
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        0,              /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        10,             /* id */
        0               /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);

#else
    child = TexuCreateWindow(
        TEXUTEXT("F1 - Help"),
        TEXU_BUTTON_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        0,              /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        10,             /* id */
        0               /* user data */
        );
#endif

#if 0
    child = TexuCreateWindow(
        "F2 - Add",
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        20,             /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        11,             /* id */
        0               /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_YELLOW_BLUE, TEXU_CIO_COLOR_YELLOW_BLUE);

#else
    child = TexuCreateWindow(
        TEXUTEXT("F2 - Add"),
        TEXU_BUTTON_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        20,             /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        11,             /* id */
        0               /* user data */
        );
#endif

#if 0
    child = TexuCreateWindow(
        "F3 - Exit",
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        40,             /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        13,             /* id */
        0               /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);
#else
    child = TexuCreateWindow(
        TEXUTEXT("F3 - Exit"),
        TEXU_BUTTON_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        40,             /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        13,             /* id */
        0               /* user data */
        );
    /*TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);*/
    TexuEnableWindow(child, TEXU_FALSE);
#endif

    child = TexuCreateWindow(
        TEXUTEXT("Alt + X - Exit"),
        TEXU_LABEL_CLASS,
        TEXU_WS_CENTER, /* style*/
        0,              /* exstyle*/
        starty + 22,    /* y */
        60,             /* x */
        1,              /* height */
        16,             /* width */
        panel,            /* parent */
        14,             /* id */
        0               /* user data */
        );
    TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);

    TexuAddHotKey(wnd, TEXUTEXT('X'), ID_DELETE, 1);
    TexuAddHotKey(wnd, TEXUTEXT('x'), ID_DELETE, 1);

    TexuAddHotKey(wnd, TVK_F1, ID_HELP, 0);
    TexuAddHotKey(wnd, TVK_F2, ID_ADD, 0);
    TexuAddHotKey(wnd, TVK_F3, ID_DELETE, 0);
    TexuAddHotKey(wnd, TVK_F4, ID_MSGBOX, 0);


    child = TexuCreateWindow(
        TEXUTEXT(""),
        TEXU_STATUSBAR_CLASS,
        0,             /* style*/
        0,             /* exstyle*/
        starty + 23,   /* y */
        0,             /* x */
        1,             /* height */
        100,           /* width */
        panel,           /* parent */
        IDC_STATUSBAR, /* id */
        0              /* user data */
        );
    TexuSendMessage(child, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_BLUE_YELLOW);
    TexuSendMessage(child, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_YELLOW_BLUE);

    TexuSendMessage(child, TEXU_SBM_ADDPART, (texu_lparam)TEXUTEXT("Part 2"), 30);
    TexuSendMessage(child, TEXU_SBM_SETCOLOR, 1, TEXU_CIO_COLOR_WHITE_BLUE);
    TexuSendMessage(child, TEXU_SBM_SETBGCOLOR, 1, TEXU_CIO_COLOR_BLUE_WHITE);
    TexuSendMessage(child, TEXU_SBM_SETALIGN, 1, TEXU_ALIGN_CENTER);

    TexuSendMessage(child, TEXU_SBM_ADDPART, (texu_lparam)TEXUTEXT("Part 3"), 20);
    TexuSendMessage(child, TEXU_SBM_SETCOLOR, 2, TEXU_CIO_COLOR_WHITE_RED);
    TexuSendMessage(child, TEXU_SBM_SETBGCOLOR, 2, TEXU_CIO_COLOR_RED_WHITE);
    TexuSendMessage(child, TEXU_SBM_SETALIGN, 2, TEXU_ALIGN_RIGHT);

    return TEXU_OK;
}

void _MyWndProc_OnCommand(texu_wnd *wnd, texu_i32 id)
{
    texu_wnd* panel = TexuGetWindowItem(wnd, IDC_PANEL1);
    texu_wnd *status = TexuGetWindowItem(panel, IDC_STATUSBAR);
    texu_char text[TEXU_MAX_WNDTEXT + 1];

    memset(text, 0, sizeof(text));
    texu_sprintf(text, sizeof(text), TEXUTEXT("enter id: %d from menu"), id);
    TexuSetWindowText(status, text);
}

texu_lparam
MyWndProc(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_PAINT:
        {
            _MyWndProc_OnPaint(wnd, (texu_cio *)param1);
            return 0;
        }

        case TEXU_WM_NOTIFY:
        {
            _MyWndProc_OnNotify(wnd, (texu_wnd_notify *)param1);
            break;
        }

        case TEXU_WM_COMMAND:
        {
            switch (param1)
            {
                case ID_DELETE:
                    _MyWndProc_OnExit(wnd);
                    break;
                case ID_HELP:
                    _MyWndProc_OnHelp(wnd);
                    break;
                case ID_ADD:
                    /*_MyWndProc_OnAdd(wnd);*/
                    break;
                case ID_MSGBOX:
                    _MyWndProc_OnMsgBox(wnd);
                    break;
                default:
                    _MyWndProc_OnCommand(wnd, param1);
                    break;
            }
            break;
        }

        case TEXU_WM_CREATE:
        {
            _MyWndProc_OnCreate(wnd);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
    /*return TexuFrameWndProc(wnd, msg, param1, param2);*/
}

void _MyWndProc2_OnAdd_test(texu_wnd *wnd)
{
    texu_wnd* panel = TexuGetWindowItem(wnd, IDC_PANEL1);
    texu_wnd *ctl = TexuGetWindowItem(panel, IDC_STATUSBAR);
    texu_wnd *newwnd = 0;

    TexuSetWindowText(ctl, TEXUTEXT("Pressed F2 to add"));

    newwnd = TexuCreateWindow(
        TEXUTEXT("Test TexU App - window 3"),
        MyWndClass3,
        0, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        3, /* id */
        0  /* user data */
        );

    TexuShowWindow(newwnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(newwnd);
}

void _MyWndProc2_OnHelp(texu_wnd *wnd)
{
    texu_wnd* panel = TexuGetWindowItem(wnd, IDC_PANEL1);
    texu_wnd *ctl = TexuGetWindowItem(panel, IDC_STATUSBAR);
    TexuSetWindowText(ctl, TEXUTEXT("Pressed F12 to exit"));
}

void _MyWndProc2_OnExit(texu_wnd *wnd)
{
    /*TexuDestroyWindow(wnd);*/
}

void _MyWndProc2_OnPaint(texu_wnd *wnd, texu_cio *dc)
{
    texu_rect rect = { 9, 0, 13, 99 };
#if (defined WIN32 && defined _WINDOWS)
    /*    texu_env_draw_frame(
    texu_wnd_get_env(wnd),
    &rect,
    TEXU_CIO_COLOR_WHITE_BLUE,
    TEXU_CIO_COLOR_BLUE_WHITE
    );*/
#else
    texu_cio_draw_frame(dc, TEXUTEXT("Hello World"), &rect,
                        TexuGetColor(dc, TEXU_CIO_COLOR_WHITE_BLUE));
#endif
}

texu_lparam
MyWndProc2(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    texu_env *env = TexuGetEnv();
    switch (msg)
    {
        case TEXU_WM_PAINT:
            _MyWndProc2_OnPaint(wnd, (texu_cio *)param1);
            return 0;

        case TEXU_WM_COMMAND:
        {
            switch (param1)
            {
                case ID_DELETE:
                    _MyWndProc2_OnExit(wnd);
                    break;

                case ID_ADD:
                    /*_MyWndProc2_OnAdd(wnd);*/
                    break;

                case ID_HELP:
                    _MyWndProc2_OnHelp(wnd);
                    break;
            }
            break;
        }

        case TEXU_WM_CREATE:
        {
            texu_wnd* panel = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_PANEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                0,         /* y */
                0,              /* x */
                25,
                100,
                wnd, /* parent */
                IDC_PANEL1,   /* id */
                0    /* user data */
                );
            texu_wnd *rebar = 0;
            texu_wnd *child = 0;
            child = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_LABEL_CLASS,
                TEXU_WS_RIGHT, /* style*/
                0,             /* exstyle*/
                0,             /* y */
                0,             /* x */
                1,
                20,
                panel, /* parent */
                1,   /* id */
                0    /* user data */
                );
            child = TexuCreateWindow(
                TEXUTEXT("Text"),
                TEXU_EDIT_CLASS,
                TEXU_ES_AUTOHSCROLL | TEXU_ES_A2Z, /* style*/
                0,                                 /* exstyle*/
                0,                                 /* y */
                20,                                /* x */
                1,
                10,
                panel, /* parent */
                2,   /* id */
                0    /* user data */
                );
            /*TexuEnableWindow(child, TEXU_FALSE);*/
            child = TexuCreateWindow(
                TEXUTEXT("50.0"),
                TEXU_EDITPRICESPREADCTRL_CLASS,
                TEXU_WS_RIGHT | TEXU_EPSS_SHOWCHANGEPRICE | TEXU_EPSS_AUTOCOMMAS, /* style*/
                0,             /* exstyle*/
                0,             /* y */
                30,             /* x */
                1,
                20,
                panel, /* parent */
                100,   /* id */
                0    /* user data */
                );
            TexuSendMessage(child, TEXU_EPSM_LOADDEFAULTSPREADS, 0, 0);
            /*texu_baseprice ps = { 3500, 6500, 5000, 100 };*//*texu_wnd_set_text(child, TEXUTEXT("51.00"));*/
            texu_baseprice ps = { 2083, 3868, 2976, 100 };/*texu_wnd_set_text(child, TEXUTEXT("36.00"));*/
            /*texu_baseprice ps = { 208300, 386800, 297600, 10000 };*//*texu_wnd_set_text(child, TEXUTEXT("36.00"));*/
            /*texu_baseprice ps = { 2, 4, 3, 100 };*//*texu_wnd_set_text(child, TEXUTEXT("0.03"));*/
            /*texu_baseprice ps = { 141600, 263200, 202400, 100 };*//*texu_wnd_set_text(child, TEXUTEXT("2100.00"));*/
            /*texu_wnd_send_msg(child, TEXU_EPSM_ALLOWOVERCEILINGFLOOR, TEXU_TRUE, 0);*/
            TexuSendMessage(child, TEXU_EPSM_CORRECTCEILINGFLOOR, TEXU_TRUE, 0);
            TexuSendMessage(child, TEXU_EPSM_SETPRICEDECIMAL, 2, 0);
            TexuSendMessage(child, TEXU_EPSM_SETBASESPREAD, (texu_lparam)&ps, 0);
            TexuSetWindowText(child, TEXUTEXT("36"));


            child = TexuCreateWindow(
                TEXUTEXT("Number:"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                1,              /* y */
                0,              /* x */
                1,
                20,
                panel, /* parent */
                3,   /* id */
                0    /* user data */
                );
            child = TexuCreateWindow(
                TEXUTEXT("123412341234234"),
                TEXU_EDIT_CLASS,
                TEXU_ES_AUTOHSCROLL | TEXU_ES_NUMBER | TEXU_ES_AUTODECIMALCOMMA, /* style*/
                0,                                    /* exstyle*/
                1,                                    /* y */
                21,                                   /* x */
                1,
                10,
                panel, /* parent */
                4,   /* id */
                0    /* user data */
                );
            child = TexuCreateWindow(
                TEXUTEXT("Decimal:"),
                TEXU_LABEL_CLASS,
                0, /* style*/
                0, /* exstyle*/
                2, /* y */
                0, /* x */
                1,
                10,
                panel, /* parent */
                5,   /* id */
                0    /* user data */
                );
            child = TexuCreateWindow(
                TEXUTEXT("1.234"),
                TEXU_EDIT_CLASS,
                TEXU_ES_AUTOHSCROLL | TEXU_ES_DECIMAL | TEXU_ES_AUTODECIMALCOMMA, /* style*/
                0,                                     /* exstyle*/
                2,                                     /* y */
                21,                                    /* x */
                1,                                     /* height */
                10,                                    /* width */
                panel,                                   /* parent */
                6,                                     /* id */
                0                                      /* user data */
                );
            child = TexuCreateWindow(
                TEXUTEXT("1.234"),
                TEXU_LISTBOX_CLASS,
                0,   /* style*/
                0,   /* exstyle*/
                3,   /* y */
                0,   /* x */
                5,   /* height */
                20,  /* width */
                panel, /* parent */
                7,   /* id */
                0    /* user data */
                );
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

            child = TexuCreateWindow(
                TEXUTEXT("1.234"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_RADIOBOX, /* style*/
                0,                 /* exstyle*/
                3,                 /* y */
                21,                /* x */
                5,                 /* height */
                20,                /* width */
                panel,               /* parent */
                8,                 /* id */
                0                  /* user data */
                );
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

            child = TexuCreateWindow(
                TEXUTEXT("1.234"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_CHECKBOX, /* style*/
                0,                 /* exstyle*/
                3,                 /* y */
                42,                /* x */
                5,                 /* height */
                20,                /* width */
                panel,               /* parent */
                9,                 /* id */
                0                  /* user data */
                );
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 1"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 2"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 3"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 4"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 5"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 6"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 7"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 8"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 9"), 0);
            TexuSendMessage(child, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Item 10"), 0);

            child = TexuCreateWindow(
                TEXUTEXT("F1 - Help"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                0,              /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                10,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            child = TexuCreateWindow(
                TEXUTEXT("F2 - List Demo"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                20,             /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                11,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            child = TexuCreateWindow(
                TEXUTEXT("F3 - Exit"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                40,             /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                13,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_RED, TEXU_CIO_COLOR_WHITE_RED);

            TexuAddHotKey(wnd, TVK_F1, ID_HELP, 0);
            TexuAddHotKey(wnd, TVK_F2, ID_ADD, 0);
            TexuAddHotKey(wnd, TVK_F3, ID_DELETE, 0);

            rebar = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_REBAR_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                10,            /* y */
                0,             /* x */
                12,             /* height */
                50,           /* width */
                panel,           /* parent */
                IDC_REBAR1, /* id */
                0              /* user data */
                );
            TexuSendMessage(rebar, TEXU_RBM_SETINFOWIDTH, 20, 0);

            texu_wnd* child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_EDIT_CLASS,
                TEXU_ES_A2Z,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                1,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR1, /* id */
                0              /* user data */
                );

            texu_rbwnd_band band;
            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band1:"));
            band.align = TEXU_ALIGN_RIGHT;
            band.normcolor = TexuGetSysColor(TEXU_COLOR_REBAR);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.discolor = TexuGetSysColor(TEXU_COLOR_REBAR_DISABLED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.selcolor = TexuGetSysColor(TEXU_COLOR_REBAR_SELECTED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.focuscolor = TexuGetSysColor(TEXU_COLOR_REBAR_FOCUSED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.normbg = TexuGetSysBgColor(TEXU_COLOR_REBAR);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.disbg = TexuGetSysBgColor(TEXU_COLOR_REBAR_DISABLED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.selbg = TexuGetSysBgColor(TEXU_COLOR_REBAR_SELECTED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.focusbg = TexuGetSysBgColor(TEXU_COLOR_REBAR_FOCUSED);/* TEXU_CIO_COLOR_BLACK_CYAN;*/
            band.childwnd = child2;
            band.width = 30;
            band.height = 1;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_LISTBOX_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR2, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);
            TexuEnableWindow(child2, TEXU_FALSE);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band2:"));
            band.align = TEXU_ALIGN_RIGHT;
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);



            child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_CHECKBOX,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR3, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band3:"));
            band.align = TEXU_ALIGN_RIGHT;
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);



            child2 = TexuCreateWindow(
                TEXUTEXT("This is the third window"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_RADIOBOX,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR4, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band4:"));
            band.align = TEXU_ALIGN_RIGHT;
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);



            /*right rebar*/

            rebar = TexuCreateWindow(
                TEXUTEXT("This is the sixth window"),
                TEXU_REBAR_CLASS,
                0 & TEXU_RBS_NOCAPTION,             /* style*/
                0,             /* exstyle*/
                10,            /* y */
                50,             /* x */
                12,             /* height */
                50,           /* width */
                panel,           /* parent */
                IDC_REBAR2, /* id */
                0              /* user data */
                );
            TexuSendMessage(rebar, TEXU_RBM_SETINFOWIDTH, 20, 0);

            child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_EDIT_CLASS,
                TEXU_ES_AUTOHSCROLL,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                1,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR1, /* id */
                0              /* user data */
                );

            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band1:"));
            band.align = TEXU_ALIGN_LEFT;
            band.childwnd = child2;
            band.width = 30;
            band.height = 1;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_LISTBOX_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                30,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR2, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);
            TexuShowWindow(child2, TEXU_SW_HIDE);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band2:"));
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);



            child2 = TexuCreateWindow(
                TEXUTEXT("This is the second window"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_CHECKBOX,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                20,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR3, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band3:"));
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);



            child2 = TexuCreateWindow(
                TEXUTEXT("This is the third window"),
                TEXU_LISTBOX_CLASS,
                TEXU_LBS_RADIOBOX,             /* style*/
                0,             /* exstyle*/
                0,            /* y */
                0,             /* x */
                5,             /* height */
                20,           /* width */
                rebar,           /* parent */
                IDC_CHILD_REBAR4, /* id */
                0              /* user data */
                );
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello world"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Java"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C/C++"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Python"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello javascript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello typescript"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello Ruby"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello PL/SQL"), 0);
            TexuSendMessage(child2, TEXU_LBM_ADDITEM, (texu_lparam)TEXUTEXT("Hello C#"), 0);


            memset(&band, 0, sizeof(band));
            texu_strcpy(band.caption, TEXUTEXT("Band4:"));
            band.childwnd = child2;
            band.width = 30;
            band.height = 5;
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);


            child = TexuCreateWindow(
                TEXUTEXT("This is the fourth window"),
                TEXU_LABEL_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                24,            /* y */
                0,             /* x */
                1,             /* height */
                100,           /* width */
                panel,           /* parent */
                IDC_STATUSBAR, /* id */
                0              /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);

            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
    /*return TexuFrameWndProc(wnd, msg, param1, param2);*/
}

void _MyWndProc3_OnAdd_test(texu_wnd *wnd)
{
    texu_wnd *ctl = TexuGetWindowItem(wnd, IDC_STATUSBAR);
    texu_wnd *newwnd = 0;

    TexuSetWindowText(ctl, TEXUTEXT("Pressed F2 to add"));

    newwnd = TexuCreateWindow(
        TEXUTEXT("Test TexU App - window 4"),
        MyWndClass4,
        0, /* style*/
        0, /* exstyle*/
        0, /* y */
        0, /* x */
        TexuGetMaxY(),
        TexuGetMaxX(),
        0, /* parent */
        4, /* id */
        0  /* user data */
        );

    TexuShowWindow(newwnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(newwnd);
}

void _MyWndProc3_OnExit(texu_wnd *wnd)
{
    /*TexuDestroyWindow(wnd);*/
}

texu_lparam
MyWndProc3(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_COMMAND:
        {
            switch (param1)
            {
                case ID_DELETE:
                    _MyWndProc3_OnExit(wnd);
                    break;

                case ID_ADD:
                    /*_MyWndProc3_OnAdd(wnd);*/
                    break;
            }
            break;
        }

        case TEXU_WM_CREATE:
        {
            texu_wnd_header header;
            texu_wnd *child = 0;
            texu_wnd* panel = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_PANEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                0,         /* y */
                0,              /* x */
                25,
                100,
                wnd, /* parent */
                IDC_PANEL1,   /* id */
                0    /* user data */
                );

            child = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_LISTCTRL_CLASS,
                TEXU_LCS_EDITABLE, /* style*/
                0, /* exstyle*/
                0, /* y */
                0, /* x */
                20,
                78,
                panel, /* parent */
                1,   /* id */
                0    /* user data */
                );
#if 1
            {
                texu_wnd_subitem subitem;
                memset(&header, 0, sizeof(header));
                header.caption = TEXUTEXT("Header 1");
                header.cols = 20;
                header.align = TEXU_ALIGN_RIGHT;
                header.normcolor = TEXU_CIO_COLOR_BLUE_YELLOW;
                header.discolor = TEXU_CIO_COLOR_BLUE_YELLOW;
                header.selcolor = TEXU_CIO_COLOR_YELLOW_BLUE;
                header.normbg = TEXU_CIO_COLOR_YELLOW_BLUE;
                header.disbg = TEXU_CIO_COLOR_YELLOW_BLUE;
                header.selbg = TEXU_CIO_COLOR_BLUE_YELLOW;

                header.editstyle = TEXU_ES_AUTOHSCROLL;
                header.decwidth = 2;
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                header.align = TEXU_ALIGN_CENTER;
                header.caption = TEXUTEXT("Header 2");
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                header.align = TEXU_ALIGN_LEFT;
                header.caption = TEXUTEXT("Header 3");
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                header.caption = TEXUTEXT("Header 4");
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                header.caption = TEXUTEXT("Header 5");
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                header.caption = TEXUTEXT("Header 6");
                TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 1\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 2\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 3\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 4\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 5\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 6\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 7\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 8\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t 9\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);
                TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t10\tHello world\tSawasdee\tAloha\tMeo\tMatt"), 6);

                subitem.idx = 0;
                subitem.col = 0;
                subitem.normcolor = TEXU_CIO_COLOR_GREEN_BLUE;
                subitem.discolor = TEXU_CIO_COLOR_GREEN_BLUE;
                subitem.selcolor = TEXU_CIO_COLOR_BLUE_GREEN;
#if (defined WIN32 && defined _WINDOWS)
                subitem.normbg = TEXU_CIO_COLOR_BLUE_GREEN;
                subitem.disbg = TEXU_CIO_COLOR_BLUE_GREEN;
                subitem.selbg = TEXU_CIO_COLOR_GREEN_BLUE;
#endif
                TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_lparam)&subitem);

                subitem.idx = 1;
                subitem.col = 1;
                subitem.normcolor = TEXU_CIO_COLOR_GREEN_MAGENTA;
                subitem.discolor = TEXU_CIO_COLOR_GREEN_MAGENTA;
                subitem.selcolor = TEXU_CIO_COLOR_MAGENTA_GREEN;
#if (defined WIN32 && defined _WINDOWS)
                subitem.normbg = TEXU_CIO_COLOR_MAGENTA_GREEN;
                subitem.disbg = TEXU_CIO_COLOR_MAGENTA_GREEN;
                subitem.selbg = TEXU_CIO_COLOR_GREEN_MAGENTA;
#endif

                TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_lparam)&subitem);

                subitem.idx = 2;
                subitem.col = 2;
                subitem.normcolor = TEXU_CIO_COLOR_RED_YELLOW;
                subitem.discolor = TEXU_CIO_COLOR_RED_YELLOW;
                subitem.selcolor = TEXU_CIO_COLOR_YELLOW_RED;
#if (defined WIN32 && defined _WINDOWS)
                subitem.normbg = TEXU_CIO_COLOR_YELLOW_RED;
                subitem.disbg = TEXU_CIO_COLOR_YELLOW_RED;
                subitem.selbg = TEXU_CIO_COLOR_RED_YELLOW;
#endif

                TexuSendMessage(child, TEXU_LCM_SETITEM, TEXU_LCFM_COLOR, (texu_lparam)&subitem);
            }
#endif
#if 0
            memset(&header, 0, sizeof(header));
            header.normcolor = TEXU_CIO_COLOR_BLUE_CYAN;
            header.discolor = TEXU_CIO_COLOR_BLUE_YELLOW;
            header.selcolor = TEXU_CIO_COLOR_YELLOW_BLUE;
            header.editstyle = TEXU_ES_AUTOHSCROLL;
            header.decwidth = 2;

            header.caption = TEXUTEXT("No");
            header.normcolor = TEXU_CIO_COLOR_BLUE_CYAN;
            header.align = TEXU_ALIGN_RIGHT;
            header.cols = 4;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.caption = TEXUTEXT("IP Address");
            header.normcolor = TEXU_CIO_COLOR_BLUE_YELLOW;
            header.align = TEXU_ALIGN_CENTER;
            header.cols = 12;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.align = TEXU_ALIGN_RIGHT;
            header.normcolor = TEXU_CIO_COLOR_BLUE_CYAN;
            header.caption = TEXUTEXT("Port");
            header.cols = 8;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.align = TEXU_ALIGN_CENTER;
            header.caption = TEXUTEXT("St");
            header.cols = 4;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.caption = TEXUTEXT("Total Packets");
            header.cols = 16;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.caption = TEXUTEXT("Recv Packets");
            header.cols = 16;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.caption = TEXUTEXT("Start Packet");
            header.cols = 16;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.align = TEXU_ALIGN_RIGHT;
            header.caption = TEXUTEXT("Start");
            header.cols = 8;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            header.align = TEXU_ALIGN_RIGHT;
            header.caption = TEXUTEXT("End");
            header.cols = 8;
            TexuSendMessage(child, TEXU_LCM_ADDCOLUMN, (texu_lparam)&header, 0);

            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t1\t10.138.61.107\t49305\tA\t71869\t845  \t71024\t105954\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t2\t10.138.61.107\t65408\tA\t71869\t28534\t43335\t102309\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t3\t10.138.61.107\t65387\tA\t71869\t31193\t40676\t102028\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t4\t10.242.11.6  \t51949\tA\t71869\t28996\t42873\t102242\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t5\t10.138.61.118\t50000\tA\t71869\t29408\t42461\t102212\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t6\t10.138.61.118\t50001\tA\t71869\t29408\t42461\t102212\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t7\t10.138.61.118\t49987\tA\t71869\t31193\t40676\t102028\t0"), 9);
            TexuSendMessage(child, TEXU_LCM_ADDITEMS, (texu_lparam)TEXUTEXT("\t8\t10.242.11.6  \t51940\tA\t71869\t31192\t40677\t102028\t0"), 9);
#endif
            child = TexuCreateWindow(
                TEXUTEXT("F2 - Tree Demo"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                20,             /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                11,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            child = TexuCreateWindow(
                TEXUTEXT("F3 - Exit"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                40,             /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                13,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            TexuAddHotKey(wnd, TVK_F1, ID_HELP, 0);
            TexuAddHotKey(wnd, TVK_F2, ID_ADD, 0);
            TexuAddHotKey(wnd, TVK_F3, ID_DELETE, 0);


            child = TexuCreateWindow(
                TEXUTEXT("This is the third window"),
                TEXU_LABEL_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                24,            /* y */
                0,             /* x */
                1,             /* height */
                100,           /* width */
                panel,           /* parent */
                IDC_STATUSBAR, /* id */
                0              /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

void _MyWndProc4_OnExit(texu_wnd *wnd)
{

}

texu_lparam
MyWndProc4(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    texu_env *env = texu_wnd_get_env(wnd);
    switch (msg)
    {
        case TEXU_WM_COMMAND:
        {
            switch (param1)
            {
                case ID_DELETE:
                    _MyWndProc3_OnExit(wnd);
                    break;
            }
            break;
        }

        case TEXU_WM_CREATE:
        {
            texu_treewnd_item data;
            texu_tree_item *item;
            texu_tree_item *parent;
            texu_wnd *child = 0;
            texu_wnd* panel = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_PANEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                0,         /* y */
                0,              /* x */
                25,
                100,
                wnd, /* parent */
                IDC_PANEL1,   /* id */
                0    /* user data */
                );

            child = TexuCreateWindow(
                TEXUTEXT("Text:"),
                TEXU_TREECTRL_CLASS,
                TEXU_TCS_SHOWNODE, /* style*/
                0,                 /* exstyle*/
                0,                 /* y */
                0,                 /* x */
                20,
                120,
                panel, /* parent */
                1,   /* id */
                0    /* user data */
                );

            memset(&data, 0, sizeof(data));
            data.normcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM);
            data.discolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM_DISABLED);
            data.selcolor = texu_env_get_syscolor(env, TEXU_COLOR_TREECTRL_ITEM_SELECTED);
            data.normbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_ITEM);
            data.disbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_ITEM_DISABLED);
            data.selbg = texu_env_get_sysbgcolor(env, TEXU_COLOR_TREECTRL_ITEM_SELECTED);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 1"));
            item = (texu_tree_item *)TexuSendMessage(child, TEXU_TCM_INSERTITEM, 0, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 1 - 1"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 1 - 2"));
            parent = item;
            item = (texu_tree_item *)TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)parent, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 1 - 3"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 1 - 4"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 2"));
            item = (texu_tree_item *)TexuSendMessage(child, TEXU_TCM_INSERTITEM, 0, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 2 - 1"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 2 - 2"));
            parent = item;
            item = (texu_tree_item *)TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)parent, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 2 - 3"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            texu_strcpy(data.itemtext, TEXUTEXT("Item 2 - 4"));
            TexuSendMessage(child, TEXU_TCM_INSERTITEM, (texu_lparam)item, (texu_lparam)&data);

            child = TexuCreateWindow(
                TEXUTEXT("F3 - Exit"),
                TEXU_LABEL_CLASS,
                TEXU_WS_CENTER, /* style*/
                0,              /* exstyle*/
                23,             /* y */
                40,             /* x */
                1,              /* height */
                16,             /* width */
                panel,            /* parent */
                13,             /* id */
                0               /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);

            TexuAddHotKey(wnd, TVK_F1, ID_HELP, 0);
            TexuAddHotKey(wnd, TVK_F3, ID_DELETE, 0);


            child = TexuCreateWindow(
                TEXUTEXT("This is the third window"),
                TEXU_LABEL_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                24,            /* y */
                0,             /* x */
                1,             /* height */
                100,           /* width */
                panel,           /* parent */
                IDC_STATUSBAR, /* id */
                0              /* user data */
                );
            TexuSetColor(child, TEXU_CIO_COLOR_BLUE_YELLOW, TEXU_CIO_COLOR_BLUE_YELLOW);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);

}

void _MyWndProc5_OnHelp(texu_wnd *wnd)
{
}

void _MyWndProc5_OnExit(texu_wnd *wnd)
{
    TexuExit();
}

texu_lparam
MyWndProc5(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_COMMAND:
        {
            switch (param1)
            {
                case ID_HELP:
                    _MyWndProc5_OnHelp(wnd);
                    break;
                case ID_DELETE:
                    _MyWndProc5_OnExit(wnd);
                    break;
            }
            break;
        }
        case TEXU_WM_CREATE:
        {
            texu_wnd *child = TexuCreateWindow(
                TEXUTEXT(""),
                TEXU_PAGECTRL_CLASS,
                0,             /* style*/
                0,             /* exstyle*/
                0,             /* y */
                0,             /* x */
                25,            /* height */
                100,           /* width */
                wnd,           /* parent */
                IDC_PAGECTRL1, /* id */
                0              /* user data */
                );
            /*
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)MyWndClass5_Page1, IDC_PAGE1);
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)MyWndClass5_Page2, IDC_PAGE2);
            */
            texu_page pg;
            pg.clsname = MyWndClass;
            pg.id = IDC_PAGE1;
            pg.caption = TEXUTEXT("Main menu");
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

            pg.clsname = MyWndClass2;
            pg.id = IDC_PAGE2;
            pg.caption = TEXUTEXT("2nd Page");
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

            pg.clsname = MyWndClass3;
            pg.id = IDC_PAGE3;
            pg.caption = TEXUTEXT("3rd Page");
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

            pg.clsname = MyWndClass4;
            pg.id = IDC_PAGE4;
            pg.caption = TEXUTEXT("4th Page");
            TexuSendMessage(child, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

            /*TexuSendMessage(child, TEXU_PGM_SETCURPAGE, 1, 0);*/

            /*TexuSendMessage(child, TEXU_PGM_REMOVEALLPAGES, 0, 0);*/
            /*
            TexuSendMessage(child, TEXU_PGM_REMOVEPAGE, 1, 0);
            TexuSendMessage(child, TEXU_PGM_REMOVEPAGE, 2, 0);*/

            TexuAddHotKey(wnd, TVK_F1, ID_HELP, 0);
            TexuAddHotKey(wnd, TVK_F3, ID_DELETE, 0);

            TexuAddHotKey(wnd, 'x', ID_DELETE, 1);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

texu_lparam
MyWndProc5_Page1(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            TexuCreateControls(wnd, templ5_p1, 50);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

texu_lparam
MyWndProc5_Page2(texu_wnd *wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            TexuCreateControls(wnd, templ5_p2, 50);
            return TEXU_OK;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}
