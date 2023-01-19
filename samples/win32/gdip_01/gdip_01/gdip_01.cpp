#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <commctrl.h>
#include "g2tctls.h"
#include "texu.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"comctl32.lib")
VOID OnPaint(HWND hWnd, HDC hdc)
{
    Graphics graphics(hdc);
    Pen pen(Color(255, 0, 0, 255));
    graphics.DrawLine(&pen, 0, 0, 200, 100);
}
VOID OnPaint2(HWND hWnd, HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush brush(Color(255, 0, 0, 255));
    FontFamily fontFamily(L"Consolas");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 20.0f);
    graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);
}
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst = NULL;

INT WINAPI WinMain2(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    g_hInst = hInstance;
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");
    RegisterClass(&wndClass);
    hWnd = CreateWindow(
        TEXT("GettingStarted"), // window class name
        TEXT("Getting Started"), // window caption
        WS_OVERLAPPEDWINDOW, // window style
        CW_USEDEFAULT, // initial x position
        CW_USEDEFAULT, // initial y position
        CW_USEDEFAULT, // initial x size
        CW_USEDEFAULT, // initial y size
        NULL, // parent window handle
        NULL, // window menu handle
        hInstance, // program instance handle
        NULL); // creation parameters
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
} // WinMain

#define IDC_LABEL1              1001
#define IDC_LABEL2              1002
#define IDC_LABEL3              1003
#define IDC_LABEL4              1004
#define IDC_LABEL5              1005
#define IDC_LABEL6              1006
#define IDC_EDIT1               1011
#define IDC_EDIT2               1012
#define IDC_EDIT3               1013
#define IDC_EDIT4               1014
#define IDC_EDIT5               1015
#define IDC_EDIT6               1016
#define IDC_BUTTON1             1021
#define IDC_BUTTON2             1022
#define IDC_BUTTON3             1023
#define IDC_BUTTON4             1024
#define IDC_BUTTON5             1025
#define IDC_BUTTON6             1026



texu_wnd_template2 templ_login2[] =
{
    { TEXUTEXT("User Id  :"), TEXU_LABEL_CLASS, TEXU_WS_RIGHT, 0, 3, 17, 1, 10, IDC_LABEL1, 0 },
    { TEXUTEXT(""), TEXU_EDIT_CLASS, TEXU_ES_LEFT | TEXU_ES_UPPERCASE | TEXU_ES_AUTOHSCROLL,
        0, 3, 28, 1, 4, IDC_EDIT1,   /*10004*/
        0 },
    { TEXUTEXT("Password :"), TEXU_LABEL_CLASS, TEXU_ES_LEFT, 0, 4, 17, 1, 10, IDC_LABEL2, 0 },
    { TEXUTEXT(""), TEXU_EDIT_CLASS, TEXU_ES_LEFT | TEXU_ES_UPPERCASE | TEXU_ES_PASSWORD |
        TEXU_ES_AUTOHSCROLL,
        0, 4, 28, 1, 16, IDC_EDIT2, /*10005*/
        0 },
    { TEXUTEXT("Login"), TEXU_BUTTON_CLASS, TEXU_WS_CENTER, 0, 7, 20, 1, 10, IDC_LABEL3,   /* 10001*/
        0 },
    { TEXUTEXT("Cancel"), TEXU_BUTTON_CLASS, TEXU_WS_CENTER, 0, 7, 32, 1, 10, IDC_LABEL4,    /* 10002*/
        0 },
    { TEXUTEXT(""), TEXU_LABEL_CLASS, TEXU_WS_CENTER, 0, 8, 1, 1, 78, IDC_LABEL5,   /* 10006*/
        0 },
    /*last control is nil*/
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
#define LOGINWND_CLASS          TEXUTEXT("LoginWnd_Class")

texu_i64 Login_WndProc(texu_wnd *wnd, texu_ui32 msg, texu_i64 param1, texu_i64 param2)
{
    texu_wnd* edit = 0;
    switch (msg)
    {
        case TEXU_WM_CREATE:    /* 7 */
        {
            texu_status rc = TexuCreateControls2(wnd, templ_login2, 50);
            if (rc != TEXU_OK)
            {
                return rc;
            }
            return TEXU_OK;
        }
    }
    return TexuFrameWndProc(wnd, msg, param1, param2);
}


INT WINAPI WinMain3(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    texu_status rc = TEXU_OK;
    texu_wnd *wnd = 0;

    TexuStartup(hInstance, 25, 80);
    TexuCreateMainEnvWnd(0,
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                         0);


    /*register class*/
    rc = TexuRegisterClass(
        LOGINWND_CLASS,
        Login_WndProc);

    /*create the new main window*/
    wnd = TexuCreateWindow(
                TEXUTEXT("Auto UI App"),
                LOGINWND_CLASS,
                TEXU_WS_BORDER, /* style*/
                0, /* exstyle*/
                0, /* y */
                0, /* x */
                TexuGetMaxY(),
                TexuGetMaxX(),
                0, /* parent */
                1, /* id */
                0  /* user data */
                );

    TexuShowWindow(wnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(wnd);

    return TexuRun();
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
    HWND hChild = CreateWindow(
                    TEXT("STATIC"),
                    TEXT("Hello World"),
                    WS_CHILD|WS_VISIBLE|SS_CENTER|SS_OWNERDRAW, // window style
                    10, // initial x position
                    10, // initial y position
                    180, // initial x size
                    20, // initial y size
                    hWnd, // parent window handle
                    (HMENU)IDC_LABEL1, // window menu handle
                    g_hInst, // program instance handle
                    NULL); // creation parameters
    
    SetWindowSubclass(hChild, g2t_LabelProc, 0, 0);
    
    hChild = CreateWindow(
        TEXT("EDIT"),
        TEXT("Hello World"),
        WS_CHILD | WS_VISIBLE , // window style
        10, // initial x position
        30, // initial y position
        180, // initial x size
        20, // initial y size
        hWnd, // parent window handle
        (HMENU)IDC_EDIT1, // window menu handle
        g_hInst, // program instance handle
        NULL); // creation parameters

    texu_char fontname[TEXU_MAX_WNDTEXT + 1] = TEXUTEXT("Consolas");
    texu_long hspaces = 0;
    texu_i32 cHeight = 0;/* -MulDiv(18, GetDeviceCaps(env->hdc, LOGPIXELSY), 72);*/
    texu_i32 cWidth = 0;
    texu_i32 cEscapement = 0;
    texu_i32 cOrientation = 0;
    texu_i32 cWeight = 0;
    DWORD    bItalic = 0;
    DWORD    bUnderline = 0;
    DWORD    bStrikeOut = 0;
    DWORD    iCharSet = 0;
    DWORD    iOutPrecision = 0;
    DWORD    iClipPrecision = 0;
    DWORD    iQuality = 0;
    texu_i32 fontsize = 18;
    texu_i32 dpi = 72;

    HDC hdc = GetDC(hWnd);
    cHeight = -MulDiv(fontsize, GetDeviceCaps(hdc, LOGPIXELSY), dpi);
    static HFONT hfont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                            bItalic, bUnderline, bStrikeOut,
                            iCharSet, iOutPrecision, iClipPrecision, iQuality,
                            FF_DONTCARE | FIXED_PITCH,
                            fontname);
    SendMessage(hChild, WM_SETFONT, (WPARAM)hfont, 0);

    /*
    SetWindowSubclass(hChild, g2t_EditProc, 0, 0);
    */

    hChild = CreateWindow(
        TEXT("BUTTON"),
        TEXT("Hello World"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, // window style
        10, // initial x position
        50, // initial y position
        180, // initial x size
        20, // initial y size
        hWnd, // parent window handle
        (HMENU)IDC_BUTTON1, // window menu handle
        g_hInst, // program instance handle
        NULL); // creation parameters

    SetWindowSubclass(hChild, g2t_EditProc, 0, 0);
    return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hWnd, hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        OnCreate(hWnd, (LPCREATESTRUCT)lParam);
        return 1;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc
