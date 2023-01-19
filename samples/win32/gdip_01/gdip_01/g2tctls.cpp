#include <windows.h>
#include <commctrl.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
VOID _g2t_Label_OnPaint(HWND hwnd, HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush brush(Color(255, 0, 0, 255));
    FontFamily fontFamily(L"Consolas");
    Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
    PointF pointF(0.0f, 0.0f);

    WCHAR szText[MAX_PATH];
    SendMessage(hwnd, WM_GETTEXT, (WPARAM)sizeof(szText), (LPARAM)szText);
    graphics.DrawString(szText, -1, &font, pointF, &brush);
}

LRESULT CALLBACK g2t_LabelProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (uMsg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        _g2t_Label_OnPaint(hWnd, hdc);
        EndPaint(hWnd, &ps);
        return TRUE;
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}


VOID _g2t_Edit_OnPaint(HWND hwnd, HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush brush(Color(255, 0, 0, 255));
    FontFamily fontFamily(L"Consolas");
    Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
    PointF pointF(0.0f, 0.0f);

    WCHAR szText[MAX_PATH];
    SendMessage(hwnd, WM_GETTEXT, (WPARAM)sizeof(szText), (LPARAM)szText);
    graphics.DrawString(szText, -1, &font, pointF, &brush);
}

LRESULT CALLBACK g2t_EditProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (uMsg)
    {
        case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            _g2t_Edit_OnPaint(hWnd, hdc);
            EndPaint(hWnd, &ps);
            return TRUE;
        }
        case WM_GETFONT:
        {
            break;
        }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}


VOID _g2t_Button_OnPaint(HWND hwnd, HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush brush(Color(255, 0, 0, 255));
    FontFamily fontFamily(L"Consolas");
    Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
    PointF pointF(0.0f, 0.0f);

    WCHAR szText[MAX_PATH];
    SendMessage(hwnd, WM_GETTEXT, (WPARAM)sizeof(szText), (LPARAM)szText);
    graphics.DrawString(szText, -1, &font, pointF, &brush);
}

LRESULT CALLBACK g2t_ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (uMsg)
    {
        case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            _g2t_Button_OnPaint(hWnd, hdc);
            EndPaint(hWnd, &ps);
            return TRUE;
        }
        case WM_NCPAINT:
        {
            HDC hdc;
            hdc = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
            // Paint into this DC 
            _g2t_Button_OnPaint(hWnd, hdc);
            ReleaseDC(hWnd, hdc);
        }
        case WM_GETFONT:
        {
            break;
        }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
