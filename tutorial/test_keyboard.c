/*       Title   : test_keyboard.c
 *       System  : Broker Office Trading system
 *       Purpose : Find the key value for each TERM. (LINUX)
 *       By      : Seree Meo Rakwong
 *       Date    : 23-FEB-1995 03:30:18.58
 *
 *       Restrictions:   ** This module is specific to VAX/VMS **
 *
 *       Edit history:
 *
 *       Date      By    Reason
 *       ========  ====  ================================================
 *
 *       COPYRIGHT (C) 2024, The Securities Technology International, Inc.
 *       ALL RIGHTS RESERVED.
 *
 *       This software is furnished under a license and may be used
 *       and copied only in accordance with the terms of such license
 *       and with the inclusion of the above copyright notice.  Neither
 *       this software nor any copies or derivative products thereof
 *       may be provided or otherwise made available to any other person.
 *       No title to or ownership of this software is hereby transferred.
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*libtexu*/
#include "texu.h"
#include "texukeys.h"
/*bos*/
#include <msesysid.h>
#include <msedef.h>
#include <stidef.h>

#include "b_bostypes.h"
#include "b_boscom.h"

DECLARE_PROCEDURE_ID("test_keyboard")

/*control id*/
#define IDC_LABEL1          101
#define IDC_LABEL2          102
#define IDC_LABEL3          103
#define IDC_LABEL4          104
#define IDC_LABEL5          105
#define IDC_LABEL6          106
#define IDC_LABEL7          107
#define IDC_LABEL8          108
#define IDC_LABEL9          109
#define IDC_LABEL10         110
#define IDC_LABEL11         111
#define IDC_LABEL12         112
#define IDC_LABEL13         113
#define IDC_LABEL14         114
#define IDC_LABEL15         115
#define IDC_LABEL16         116
#define IDC_LABEL17         117
#define IDC_LABEL18         118
#define IDC_LABEL19         119
#define IDC_CLOCK1          150
#define IDC_HEADER1         151
#define IDC_HEADER2         152
#define IDC_PANEL1          200
#define IDC_PANEL2          201
#define IDC_PANEL3          202
#define IDC_REBAR1          250
#define IDC_REBAR2          251
#define IDC_PROGRESS1       252
#define IDC_LISTBOX1        253
#define IDC_COMBOBOX1       254
#define IDC_PROGRESSBAR1    255
#define IDC_IPADDRESS1      256
#define IDC_EDITMASK1       257
#define IDC_PAGECTRL1       258


#define IDC_VIEWORDER     300
#define IDC_USERID        301
#define IDC_PASSWORD      302
#define IDC_SIDE          303
#define IDC_SECURITY      304
#define IDC_SECURITY_INFO 305
#define IDC_VOLUME        306
#define IDC_LOT           307
#define IDC_PRICE         308
#define IDC_PRIOR_CLOSE   309
#define IDC_ACCOUNT       310
#define IDC_ACCOUNT_INFO  311
#define IDC_BOARD         312
#define IDC_BOARD_INFO    313
#define IDC_ACCTYPE       314
#define IDC_ACCTYPE_INFO  315
#define IDC_EXPDATE       316
#define IDC_PUBLIC        317
#define IDC_TTF           318
#define IDC_STATUS        319
#define IDC_QUERYCLOSE    320
#define IDC_QUERYCANCEL   321
#define IDC_SHOWINFO        322

/*command id*/
#define ID_QUIT           1000
#define ID_INSERT         1001
#define ID_DELETE         1002
#define ID_UPDATE         1003
#define ID_EXIT           (ID_QUIT)
#define ID_LOGIN          1005
#define ID_GOTO_SIDE      1006
#define ID_GOTO_VIEWORDER 1007
#define ID_FIND           1008
#define ID_REFRESH        1009
#define ID_HELP           1010
#define ID_EXPORT         1011


/*class*/
texu_i32 _DemoWndProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
    texu_i32  cx = TexuGetMaxX();
    texu_i32  cy = TexuGetMaxY();
    texu_wnd *pgctl = 0;
    texu_page pg;
    
    pgctl = TexuCreateWindow("Page1", /*title*/
                TEXU_PAGECTRL_CLASS, /*class*/
                0, /*style*/
                0, /*ex-style*/
                0, /*y*/
                0, /*x*/
                cy, /*height*/
                cx, /*width*/
                wnd,
                IDC_PAGECTRL1,
                0);
    memset(&pg, 0, sizeof(pg));
    pg.clsname = "DEMOWND_P1_CLASS";
    pg.id = 1;
    pg.caption = "Page1";
    TexuSendMessage(pgctl, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

    pg.clsname = "DEMOWND_P2_CLASS";
    pg.id = 2;
    pg.caption = "Page2";
    TexuSendMessage(pgctl, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);

    pg.clsname = "DEMOWND_P3_CLASS";
    pg.id = 3;
    pg.caption = "Page";
    TexuSendMessage(pgctl, TEXU_PGM_ADDPAGE, (texu_lparam)&pg, 0);
    
#if defined __USE_TERMBOX2__
    TexuAddHotKey(wnd, TEXU_KEY_F22, ID_QUIT, 0);
#else
    TexuAddHotKey(wnd, TEXU_KEY_F12, ID_QUIT, 0);
#endif
    TexuAddHotKey(wnd, TEXU_KEY_F5, ID_REFRESH, 0);
    

                
    return TEXU_OK;
}

texu_longptr DemoWndProc(texu_wnd* wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            return _DemoWndProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
        }

        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            else if (ID_REFRESH == param1)
            {
                TexuInvalidateWindow(wnd);
            }
            return 0;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}


texu_i32 _DemoP1WndProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
    texu_wnd* child    = 0;
    texu_wnd* panel    = 0;
    texu_wnd* panel2   = 0;
    texu_wnd* status   = 0;
    texu_wnd* userid   = 0;
    texu_i32  cxScreen = TexuGetMaxX();
    texu_i32  cyScreen = TexuGetMaxY();
    texu_wnd *rebar = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    /*TODO: login panel shall be the first visible*/
    do
    {
        panel = TexuCreateWindow("Demo Rebar - P1",                                               /*title*/
                                 TEXU_PANEL_CLASS,                                                        /*class*/
                                 TEXU_WS_CLIPWINDOW | TEXU_PNS_TITLE | /*TEXU_PNS_BORDER |*/ TEXU_PNS_STATUS, /* style*/
                                 0,                                                                       /* exstyle*/
                                 0,                                                                       /* y */
                                 0,                                                                       /* x */
                                 cyScreen,                                                                /* height */
                                 cxScreen,                                                                /* width */
                                 wnd,                                                                     /* parent */
                                 IDC_PANEL1,                                                              /* id */
                                 0                                                                        /* user data */
        );
        //TexuSetColor2(panel, TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
        TexuSetColor2(panel, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLEBGCOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);

        status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
#if defined __USE_TERMBOX2__
        TexuSetWindowText(status, "[Exit-F22]");
#else
        TexuSetWindowText(status, "[Exit-F12]");
#endif
        TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);
        TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);

        panel2 = TexuCreateWindow("Demo - P1",                                               /*title*/
                                  TEXU_PANEL_CLASS,                                      /*class*/
                                  TEXU_WS_CLIPWINDOW | TEXU_PNS_BORDER,/*| TEXU_PNS_TITLE , /* style*/
                                  0,                                                     /* exstyle*/
                                  1,                                                     /* y */
                                  0,                                                    /* x */
                                  11,                                                    /* height */
                                  60,                                                    /* width */
                                  panel,                                                 /* parent */
                                  IDC_PANEL2,                                            /* id */
                                  0                                                      /* user data */
        );
        TexuSetColor2(panel2, TEXU_CIO_COLOR_BLACK_CYAN, TEXU_CIO_COLOR_BLACK_CYAN);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLEBGCOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
        
        rebar = TexuCreateWindow("Rebar", /*title*/
                                TEXU_REBAR_CLASS, /*class*/
                                TEXU_WS_CLIPWINDOW | TEXU_RBS_HIGHLIGHT | TEXU_RBS_HASUNIT, /*style*/
                                0, /*ex-style*/
                                1, /*y*/
                                1, /*x*/
                                9, /*height*/
                                58, /*width*/
                                panel2, /*parent*/
                                IDC_REBAR1, /*id*/
                                0 /*user data*/
                                );
        TexuSendMessage(rebar, TEXU_RBM_SETINFOWIDTH, 10, 0);
        TexuSendMessage(rebar, TEXU_RBM_SETBANDWIDTH, 20, 0);
        {
            /*add band*/
            texu_rbwnd_band band;
            memset(&band, 0, sizeof(band));
            /*initialized fixing attributes*/
            band.align      = TEXU_ALIGN_RIGHT;
            band.enable     = TEXU_TRUE;
            band.visible    = TEXU_TRUE;
            band.normcolor  = TEXU_CIO_COLOR_WHITE_BLACK;
            band.discolor   = TEXU_CIO_COLOR_WHITE_BLACK;
            band.selcolor   = TEXU_CIO_COLOR_WHITE_BLACK;
            band.focuscolor = TEXU_CIO_COLOR_WHITE_BLACK;
            band.normbg     = TEXU_CIO_COLOR_BLACK_WHITE;
            band.disbg      = TEXU_CIO_COLOR_BLACK_WHITE;
            band.selbg      = TEXU_CIO_COLOR_BLACK_WHITE;
            band.focusbg    = TEXU_CIO_COLOR_BLACK_WHITE;
            //texu_strcpy(band.unit, "");
            if (1)
            {
                band.caption = "COMBOBOX";
                band.unit = "(Mr/Mrs/Miss)";
                band.width = 10;
                band.height = 1;

                band.childwnd = TexuCreateWindow3("", TEXU_COMBOBOX_CLASS,                                   /*class*/
                                      TEXU_WS_LEFT, /* style*/
                                      0,                                                     /* exstyle*/
                                      0,                                                     /* y: last line of the panel */
                                      0,                                                    /* x */
                                      0,                                                     /* height */
                                      0,                                       /* width */
                                      rebar,                                                /* parent */
                                      IDC_COMBOBOX1,                                          /* id */
                                      0,                                                     /* user data */
                                      0,                                       /*validation data*/
                                      0                                      /*validation*/
                );
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Mr", 0);
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Mrs", 0);
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Miss", 0);
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Mr/Mrs/Miss/etc", 0);
                TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            }
            
            /*texu_strcpy(band.caption, "User ID");
            texu_strcpy(band.unit, "Trader");*/
            band.caption = "User ID";
            band.unit = "Trader ID";
            band.width = MW_TID_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow("1234", TEXU_EDIT_CLASS,              /*class*/
                                      TEXU_WS_LEFT | TEXU_ES_UPPERCASE, /* style*/
                                      0,                                /* exstyle*/
                                      0,                                /* y: last line of the panel */
                                      0,                               /* x */
                                      0,                                /* height */
                                      0,                       /* width */
                                      rebar,                           /* parent */
                                      IDC_USERID,                       /* id */
                                      0                                 /* user data */
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_TID_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            if (1)
            {
                band.caption = "LISTBOX";
                band.unit = "(Mr/Mrs/Miss)";
                band.width = 10;
                band.height = 3;
                band.childwnd = TexuCreateWindow3("", TEXU_LISTBOX_CLASS,                                   /*class*/
                                      TEXU_WS_LEFT, /* style*/
                                      0,                                                     /* exstyle*/
                                      0,                                                     /* y: last line of the panel */
                                      0,                                                    /* x */
                                      0,                                                     /* height */
                                      0,                                       /* width */
                                      rebar,                                                /* parent */
                                      IDC_LISTBOX1,                                          /* id */
                                      0,                                                     /* user data */
                                      0,                                       /*validation data*/
                                      0                                      /*validation*/
                );
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mr", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mrs", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Miss", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mr/Mrs/Miss/etc", 0);
                TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            }

            
            /*texu_strcpy(band.caption, "Password");
            texu_strcpy(band.unit, "Trader Password");*/
            band.caption = "Password";
            band.unit = "Trader Password";
            band.width = 5;//MW_USERPASS_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_PASSWORD | TEXU_ES_AUTOHSCROLL, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PASSWORD,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_USERPASS_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Side");
            texu_strcpy(band.unit, "[B/S/C/H]");*/
            band.caption = "Side";
            band.unit = "[B/S/C/H]";
            band.width = 1;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_SIDE,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Security");
            texu_strcpy(band.unit, "Stock");*/
            band.caption = "Security";
            band.unit = "Stock Name";
            band.width = MW_STOCK_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_SECURITY,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_STOCK_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Volume");
            texu_strcpy(band.unit, "");*/
            band.caption = "Volume";
            band.unit = "Shares";
            band.width = MW_VOLUME_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("100", TEXU_EDITVOLUMECTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_VOLUME,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_VOLUME_LEN, 0);*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Public");
            texu_strcpy(band.unit, "Public Volume");*/
            band.caption = "Public";
            band.unit = "Public Volume";
            band.width = MW_VOLUME_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("100", TEXU_EDITVOLUMECTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PUBLIC,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_VOLUME_LEN, 0);*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Account");
            texu_strcpy(band.unit, "");*/
            band.caption = "Account";
            band.unit = "Customer ID";
            band.width = MW_CUST_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_ACCOUNT,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_CUST_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "Board";
            band.unit = "[M/O/F]";
            band.width = 1;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_BOARD,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
#if 0
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "PROGRESS";
            band.unit = "";
            band.width = 20;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("50", TEXU_PROGRESSBAR_CLASS,                                   /*class*/
                                  TEXU_PGBS_SHOWPERCENT, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PROGRESS1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);*/
            TexuSendMessage(band.childwnd, TEXU_PGBM_SETMAX, 100, 0);
            TexuSendMessage(band.childwnd, TEXU_PGBM_SETPOS, 50, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
#endif 
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "Host";
            band.unit = "IP Address";
            band.width = 15;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("10.138.23.106", TEXU_IPADDRESSCTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_IPADDRESS1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);*/
            TexuSetWindowText(band.childwnd, "10.138.23.106");
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "email";
            band.unit = "EMail";
            band.width = 15;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("example: abc@g.com", TEXU_EDITMASKCTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_EDITMASK1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EMM_SETINFO, (texu_lparam)"example: abc@g.com", 0);
            TexuSendMessage(band.childwnd, TEXU_EMM_SETMASK, (texu_lparam)"^[a-zA-Z0–9. _%+-]+@[a-zA-Z0–9. -]+. [a-zA-Z]{2,}$", 0);
            /*TexuSetWindowText(band.childwnd, "10.138.23.106");*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
        }
        /*set focus*/
        TexuMoveCenterWindow(panel2, TEXU_TRUE);
    } while (0);
#if defined __USE_TERMBOX2__
    TexuAddHotKey(wnd, TEXU_KEY_F22, ID_QUIT, 0);
#else
    TexuAddHotKey(wnd, TEXU_KEY_F12, ID_QUIT, 0);
#endif
    TexuAddHotKey(wnd, TEXU_KEY_F5, ID_REFRESH, 0);

    return TEXU_OK;
}

texu_longptr DemoP1WndProc(texu_wnd* wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            return _DemoP1WndProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
        }

        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            else if (ID_REFRESH == param1)
            {
                TexuInvalidateWindow(wnd);
            }
            return 0;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

texu_i32 _DemoP2WndProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
    texu_wnd* child    = 0;
    texu_wnd* panel    = 0;
    texu_wnd* panel2   = 0;
    texu_wnd* status   = 0;
    texu_wnd* userid   = 0;
    texu_i32  cxScreen = TexuGetMaxX();
    texu_i32  cyScreen = TexuGetMaxY();
    texu_wnd *rebar = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    /*TODO: login panel shall be the first visible*/
    do
    {
        panel = TexuCreateWindow("Demo Rebar - P2",                                               /*title*/
                                 TEXU_PANEL_CLASS,                                                        /*class*/
                                 TEXU_WS_CLIPWINDOW | TEXU_PNS_TITLE | TEXU_PNS_BORDER | TEXU_PNS_STATUS, /* style*/
                                 0,                                                                       /* exstyle*/
                                 0,                                                                       /* y */
                                 0,                                                                       /* x */
                                 cyScreen,                                                                /* height */
                                 cxScreen,                                                                /* width */
                                 wnd,                                                                     /* parent */
                                 IDC_PANEL1,                                                              /* id */
                                 0                                                                        /* user data */
        );
        TexuSetColor2(panel, TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK);
        TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLEBGCOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);

        status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
#if defined __USE_TERMBOX2__
        TexuSetWindowText(status, "[Exit-F22]");
#else
        TexuSetWindowText(status, "[Exit-F12]");
#endif
        TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);
        TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);

        panel2 = TexuCreateWindow("Demo",                                               /*title*/
                                  TEXU_PANEL_CLASS,                                      /*class*/
                                  TEXU_WS_CLIPWINDOW | TEXU_PNS_TITLE | TEXU_PNS_BORDER, /* style*/
                                  0,                                                     /* exstyle*/
                                  5,                                                     /* y */
                                  5,                                                    /* x */
                                  11,                                                    /* height */
                                  60,                                                    /* width */
                                  panel,                                                 /* parent */
                                  IDC_PANEL2,                                            /* id */
                                  0                                                      /* user data */
        );
        TexuSetColor2(panel2, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLEBGCOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel2, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
        
        rebar = TexuCreateWindow("Rebar", /*title*/
                                TEXU_REBAR_CLASS, /*class*/
                                TEXU_WS_CLIPWINDOW | TEXU_RBS_HIGHLIGHT | TEXU_RBS_HASUNIT, /*style*/
                                0, /*ex-style*/
                                1, /*y*/
                                1, /*x*/
                                4, /*height*/
                                58, /*width*/
                                panel2, /*parent*/
                                IDC_REBAR1, /*id*/
                                0 /*user data*/
                                );
        TexuSendMessage(rebar, TEXU_RBM_SETINFOWIDTH, 10, 0);
        TexuSendMessage(rebar, TEXU_RBM_SETBANDWIDTH, 20, 0);
        {
            /*add band*/
            texu_rbwnd_band band;
            memset(&band, 0, sizeof(band));
            /*initialized fixing attributes*/
            band.align      = TEXU_ALIGN_RIGHT;
            band.enable     = TEXU_TRUE;
            band.visible    = TEXU_TRUE;
            band.normcolor  = TEXU_CIO_COLOR_WHITE_BLACK;
            band.discolor   = TEXU_CIO_COLOR_WHITE_BLACK;
            band.selcolor   = TEXU_CIO_COLOR_WHITE_BLACK;
            band.focuscolor = TEXU_CIO_COLOR_WHITE_BLACK;
            band.normbg     = TEXU_CIO_COLOR_BLACK_WHITE;
            band.disbg      = TEXU_CIO_COLOR_BLACK_WHITE;
            band.selbg      = TEXU_CIO_COLOR_BLACK_WHITE;
            band.focusbg    = TEXU_CIO_COLOR_BLACK_WHITE;
            //texu_strcpy(band.unit, "");
            if (1)
            {
                band.caption = "COMBOBOX";
                band.unit = "(Mr/Mrs/Miss)";
                band.width = 10;
                band.height = 1;

                band.childwnd = TexuCreateWindow3("", TEXU_COMBOBOX_CLASS,                                   /*class*/
                                      TEXU_WS_LEFT, /* style*/
                                      0,                                                     /* exstyle*/
                                      0,                                                     /* y: last line of the panel */
                                      0,                                                    /* x */
                                      0,                                                     /* height */
                                      0,                                       /* width */
                                      rebar,                                                /* parent */
                                      IDC_COMBOBOX1,                                          /* id */
                                      0,                                                     /* user data */
                                      0,                                       /*validation data*/
                                      0                                      /*validation*/
                );
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Mr", 0);
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Mrs", 0);
                TexuSendMessage(band.childwnd, TEXU_CBM_ADDITEM, (texu_lparam)"Miss", 0);
                TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            }
            
            /*texu_strcpy(band.caption, "User ID");
            texu_strcpy(band.unit, "Trader");*/
            band.caption = "User ID";
            band.unit = "Trader ID";
            band.width = MW_TID_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow("1234", TEXU_EDIT_CLASS,              /*class*/
                                      TEXU_WS_LEFT | TEXU_ES_UPPERCASE, /* style*/
                                      0,                                /* exstyle*/
                                      0,                                /* y: last line of the panel */
                                      0,                               /* x */
                                      0,                                /* height */
                                      0,                       /* width */
                                      rebar,                           /* parent */
                                      IDC_USERID,                       /* id */
                                      0                                 /* user data */
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_TID_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            if (1)
            {
                band.caption = "LISTBOX";
                band.unit = "(Mr/Mrs/Miss)";
                band.width = 10;
                band.height = 3;
                band.childwnd = TexuCreateWindow3("", TEXU_LISTBOX_CLASS,                                   /*class*/
                                      TEXU_WS_LEFT, /* style*/
                                      0,                                                     /* exstyle*/
                                      0,                                                     /* y: last line of the panel */
                                      0,                                                    /* x */
                                      0,                                                     /* height */
                                      0,                                       /* width */
                                      rebar,                                                /* parent */
                                      IDC_LISTBOX1,                                          /* id */
                                      0,                                                     /* user data */
                                      0,                                       /*validation data*/
                                      0                                      /*validation*/
                );
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mr", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mrs", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Miss", 0);
                TexuSendMessage(band.childwnd, TEXU_LBM_ADDITEM, (texu_lparam)"Mr/Mrs/Miss/etc", 0);
                TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            }

            
            /*texu_strcpy(band.caption, "Password");
            texu_strcpy(band.unit, "Trader Password");*/
            band.caption = "Password";
            band.unit = "Trader Password";
            band.width = 5;//MW_USERPASS_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_PASSWORD | TEXU_ES_AUTOHSCROLL, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PASSWORD,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_USERPASS_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Side");
            texu_strcpy(band.unit, "[B/S/C/H]");*/
            band.caption = "Side";
            band.unit = "[B/S/C/H]";
            band.width = 1;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_SIDE,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Security");
            texu_strcpy(band.unit, "Stock");*/
            band.caption = "Security";
            band.unit = "Stock Name";
            band.width = 8;//MW_STOCK_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_SECURITY,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_STOCK_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Volume");
            texu_strcpy(band.unit, "");*/
            band.caption = "Volume";
            band.unit = "Shares";
            band.width = MW_VOLUME_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("100", TEXU_EDITVOLUMECTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_VOLUME,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_VOLUME_LEN, 0);*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Public");
            texu_strcpy(band.unit, "Public Volume");*/
            band.caption = "Public";
            band.unit = "Public Volume";
            band.width = MW_VOLUME_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("100", TEXU_EDITVOLUMECTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PUBLIC,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_VOLUME_LEN, 0);*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Account");
            texu_strcpy(band.unit, "");*/
            band.caption = "Account";
            band.unit = "Customer ID";
            band.width = MW_CUST_LEN;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_ACCOUNT,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, MW_CUST_LEN, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "Board";
            band.unit = "[M/O/F]";
            band.width = 1;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("", TEXU_EDIT_CLASS,                                   /*class*/
                                  TEXU_WS_LEFT | TEXU_ES_AUTOHSCROLL | TEXU_ES_UPPERCASE, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_BOARD,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "PROGRESS";
            band.unit = "";
            band.width = 10;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("50", TEXU_PROGRESSBAR_CLASS,                                   /*class*/
                                  TEXU_PGBS_SHOWPERCENT, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_PROGRESS1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);*/
            TexuSendMessage(band.childwnd, TEXU_PGBM_SETMAX, 100, 0);
            TexuSendMessage(band.childwnd, TEXU_PGBM_SETPOS, 50, 0);
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
            
            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "Host";
            band.unit = "IP Address";
            band.width = 15;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("10.138.23.106", TEXU_IPADDRESSCTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_IPADDRESS1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            /*TexuSendMessage(band.childwnd, TEXU_EM_LIMITTEXT, 1, 0);*/
            TexuSetWindowText(band.childwnd, "10.138.23.106");
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);

            /*texu_strcpy(band.caption, "Board");
            texu_strcpy(band.unit, "[M/O/F]");*/
            band.caption = "email";
            band.unit = "EMail";
            band.width = 15;
            band.height = 1;
            band.childwnd = TexuCreateWindow3("example: abc@g.com", TEXU_EDITMASKCTRL_CLASS,                                   /*class*/
                                  0, /* style*/
                                  0,                                                     /* exstyle*/
                                  0,                                                     /* y: last line of the panel */
                                  0,                                                    /* x */
                                  0,                                                     /* height */
                                  0,                                       /* width */
                                  rebar,                                                /* parent */
                                  IDC_EDITMASK1,                                          /* id */
                                  0,                                                     /* user data */
                                  0,                                       /*validation data*/
                                  0                                      /*validation*/
            );
            TexuSendMessage(band.childwnd, TEXU_EMM_SETINFO, (texu_lparam)"example: abc@g.com", 0);
            TexuSendMessage(band.childwnd, TEXU_EMM_SETMASK, (texu_lparam)"^[a-zA-Z0–9. _%+-]+@[a-zA-Z0–9. -]+. [a-zA-Z]{2,}$", 0);
            /*TexuSetWindowText(band.childwnd, "10.138.23.106");*/
            TexuSendMessage(rebar, TEXU_RBM_ADDBAND, (texu_lparam)&band, 0);
        }
        /*set focus*/
        /*TexuMoveCenterWindow(panel2, TEXU_TRUE);*/
    } while (0);
#if defined __USE_TERMBOX2__
    TexuAddHotKey(wnd, TEXU_KEY_F22, ID_QUIT, 0);
#else
    TexuAddHotKey(wnd, TEXU_KEY_F12, ID_QUIT, 0);
#endif
    TexuAddHotKey(wnd, TEXU_KEY_F5, ID_REFRESH, 0);

    return TEXU_OK;
}

texu_longptr DemoP2WndProc(texu_wnd* wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_CREATE:
        {
            return _DemoP2WndProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
        }

        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            else if (ID_REFRESH == param1)
            {
                TexuInvalidateWindow(wnd);
            }
            return 0;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

void _DemoP3Wnd_OnPaint(texu_wnd *wnd, texu_cio *cio, texu_rect *rect)
{
    texu_char sz[TEXU_MAX_WNDTEXT + 1] = "";
    texu_cio_draw_text(cio, 10, 10, sz, TEXU_CIO_COLOR_WHITE_BLACK, TEXU_CIO_COLOR_WHITE_BLACK, "axx", 0);
}

texu_i32 translate_keydown(texu_i32 key, texu_i32 alt)
{
    texu_i32 ch = 0;
    if (key >= TB_KEY_CTRL_A && key <= TB_KEY_CTRL_Z)
    {
        ch = 'a' + (key - 1);
    }
    else
    {
        switch (key)
        {
            case TB_KEY_CTRL_2: ch = '2'; break;
            case TB_KEY_CTRL_3: ch = '3'; break;
            case TB_KEY_CTRL_4: ch = '4'; break;
            case TB_KEY_CTRL_5: ch = '5'; break;
            case TB_KEY_CTRL_6: ch = '6'; break;
            case TB_KEY_CTRL_7: ch = '7'; break;
            case TB_KEY_CTRL_8: ch = '8'; break;
            break;
        }
    }
    return ch;
}

void _DemoP3Wnd_OnKeyDown(texu_wnd *wnd, texu_i32 key, texu_i32 alt)
{
    texu_char sz[TEXU_MAX_WNDTEXT + 1] = "";
    texu_char fn[TEXU_MAX_WNDTEXT + 1] = "F";
    texu_i32 ch = '0';
    texu_i32 f1 = TB_KEY_F1;
    texu_i32 f12 = TB_KEY_F12;
    texu_i32 f13 = TEXU_KEY_F13;
    texu_i32 f22 = TEXU_KEY_F22;
    if (key >= 20 && key <= 0x7f)
    {
        sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)key, (int)alt);
    }
    else if (key >= f12 && key <= f1)
    {
        if (0)
        {
            //ch = TEXU_KEY_F13 - (TB_KEY_F1 - key + 1);
            texu_i32 newkey = 10 + (TB_KEY_F1 - key + 1);
            sprintf(fn, "F%d", newkey);
            newkey = TEXU_KEY_F13 - (TB_KEY_F1 - key);
            /*sprintf(sz, "key:[%d, %x], function:[%s], alt:[%d]", (int)key, (int)key, fn, (int)alt);*/
            sprintf(sz, "newkey:[%d, %x], key:[%d, %x], function:[%s], alt:[%d]", (int)newkey, (int)newkey, (int)key, (int)key, fn, (int)alt);
        }
        else
        {
            sprintf(fn, "F%d", (TB_KEY_F1 - key + 1));
            sprintf(sz, "key:[%d, %x], function:[%s], alt:[%d]", (int)key, (int)key, fn, (int)alt);
        }
    }
#if defined __USE_TERMBOX2__
    else if (key >= f22 && key <= f13)
    {
        sprintf(fn, "F%d", (12 + (TB_KEY_F1 - key + 1))-TEXU_KEY_USER_FIRST);
        sprintf(sz, "key:[%d, %x], function:[%s], alt:[%d]", (int)key, (int)key, fn, (int)alt);
    }
#endif
    else if (key >= TB_KEY_ARROW_RIGHT && key <= TB_KEY_INSERT)
    {
        switch (key)
        {
            case TB_KEY_INSERT:         sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "INS",    (int)alt); break;
            case TB_KEY_DELETE:         sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "DEL",    (int)alt); break;
            case TB_KEY_HOME:           sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "HOME",   (int)alt); break;
            case TB_KEY_END:            sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "END",    (int)alt); break;
            case TB_KEY_PGUP:           sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "PGUP",   (int)alt); break;
            case TB_KEY_PGDN:           sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "PGDN",   (int)alt); break;
            case TB_KEY_ARROW_UP:       sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "UP",     (int)alt); break;
            case TB_KEY_ARROW_DOWN:     sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "DOWN",   (int)alt); break;
            case TB_KEY_ARROW_LEFT:     sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "LEFT",   (int)alt); break;
            case TB_KEY_ARROW_RIGHT:    sprintf(sz, "key:[%d, %x], str:[%s], alt:[%d]", (int)key, (int)key, "RIGHT",  (int)alt); break;
        }
    }
    else if (key >= TB_KEY_CTRL_A && key <= TB_KEY_CTRL_Z)
    {
        sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)('a' + (key - 1)), (int)alt);
    }
    else
    {
        if (key < 0x7f)
        {
            switch (key)
            {
                case TB_KEY_CTRL_2: ch = '2'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_3: ch = '3'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_4: ch = '4'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_5: ch = '5'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_6: ch = '6'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_7: ch = '7'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                case TB_KEY_CTRL_8: ch = '8'; sprintf(sz, "key:[%d, %x], char:[%c], alt:[%d]", (int)key, (int)key, (char)ch, (int)alt); break;
                default:
                sprintf(sz, "key:[%d, %x], alt:[%d]", (int)key, (int)key, (int)alt); break;
            }
        }
        else
        {
            sprintf(sz, "key:[%d, %x], alt:[%d]", (int)key, (int)key, (int)alt);
        }
    }
    TexuSetWindowText(wnd, sz);
    
    texu_wnd *panel = TexuGetWindowItem(wnd, IDC_PANEL1);
    /*TexuSetWindowText(panel, sz);*/

    texu_wnd *label = TexuGetWindowItem(panel, IDC_LABEL1);
    TexuSetWindowText(label, sz);
    texu_wnd_invalidate(wnd);
}

texu_i32 _DemoP3WndProc_OnCreate(texu_wnd* wnd, texu_wnd_attrs* attrs)
{
    texu_wnd* child    = 0;
    texu_wnd* panel    = 0;
    texu_wnd* panel2   = 0;
    texu_wnd* status   = 0;
    texu_wnd* userid   = 0;
    texu_i32  cxScreen = TexuGetMaxX();
    texu_i32  cyScreen = TexuGetMaxY();
    texu_wnd *rebar = 0;
    texu_env *env = texu_wnd_get_env(wnd);

    /*TODO: login panel shall be the first visible*/
    do
    {
        panel = TexuCreateWindow("Demo Rebar - P3",                                               /*title*/
                                 TEXU_PANEL_CLASS,                                                        /*class*/
                                 TEXU_WS_CLIPWINDOW | TEXU_PNS_TITLE | TEXU_PNS_BORDER | TEXU_PNS_STATUS, /* style*/
                                 0,                                                                       /* exstyle*/
                                 0,                                                                       /* y */
                                 0,                                                                       /* x */
                                 cyScreen,                                                                /* height */
                                 cxScreen,                                                                /* width */
                                 wnd,                                                                     /* parent */
                                 IDC_PANEL1,                                                              /* id */
                                 0                                                                        /* user data */
        );
        TexuSetColor2(panel, TEXU_CIO_COLOR_BLACK_WHITE, TEXU_CIO_COLOR_BLACK_WHITE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLECOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        //TexuSendMessage(panel, TEXU_PNM_SETTITLEBGCOLOR, TEXU_CIO_COLOR_WHITE_BLUE, TEXU_CIO_COLOR_WHITE_BLUE);
        TexuSendMessage(panel, TEXU_PNM_SETTITLEALIGNMENT, TEXU_ALIGN_CENTER, 0);
        
        status = (texu_wnd*)TexuSendMessage(panel, TEXU_PNM_GETSTATUSBAR, 0, 0);
#if defined __USE_TERMBOX2__
        TexuSetWindowText(status, "[Exit-F22]");
#else
        TexuSetWindowText(status, "[Exit-F12]");
#endif
        TexuSendMessage(status, TEXU_SBM_SETCOLOR, 0, TEXU_CIO_COLOR_BLUE_YELLOW);
        TexuSendMessage(status, TEXU_SBM_SETBGCOLOR, 0, TEXU_CIO_COLOR_YELLOW_MAGENTA);

        texu_wnd *label = TexuCreateWindow("",
            TEXU_LABEL_CLASS,
            TEXU_WS_CENTER,
            0,
            10,
            10,
            1,
            100,
            panel,
            IDC_LABEL1,
            0);
        TexuMoveCenterWindow(label, TEXU_TRUE);
        
#if defined __USE_TERMBOX2__
    TexuAddHotKey(wnd, TEXU_KEY_F22, ID_QUIT, 0);
#else
    TexuAddHotKey(wnd, TEXU_KEY_F12, ID_QUIT, 0);
#endif
        TexuAddHotKey(wnd, TEXU_KEY_F5, ID_REFRESH, 0);

    }
    while (0);
    return TEXU_OK;
}

texu_longptr DemoP3WndProc(texu_wnd* wnd, texu_ui32 msg, texu_lparam param1, texu_lparam param2)
{
    switch (msg)
    {
        case TEXU_WM_NOTIFY:
        {
            texu_wnd_notify *notify = (texu_wnd_notify*)param1;
            if (notify->code == TEXU_PNN_KEYDOWN)
            {
                texu_panel_notify *panel_notify = (texu_panel_notify*)notify;
                texu_wnd_send_msg(wnd, TEXU_WM_KEYDOWN, panel_notify->ch, panel_notify->alt);
            }
            return 0;
        }
        
        case TEXU_WM_CREATE:
        {
            return _DemoP3WndProc_OnCreate(wnd, (texu_wnd_attrs*)param1);
        }

        case TEXU_WM_PAINT:
        {
            _DemoP3Wnd_OnPaint(wnd, (texu_cio*)param1, (texu_rect*)param2);
            return 0;
        }
        
        case TEXU_WM_KEYDOWN:
        {
            _DemoP3Wnd_OnKeyDown(wnd, (texu_i32)param1, (texu_i32)param2);
            return 0;
        }
        
        case TEXU_WM_COMMAND:
        {
            if (ID_QUIT == param1)
            {
                TexuExit();
            }
            else if (ID_REFRESH == param1)
            {
                TexuInvalidateWindow(wnd);
            }
            return 0;
        }
    }
    return TexuDefWndProc(wnd, msg, param1, param2);
}

int main(int argc, char *argv[])
{
    texu_status rc       = 0;
    texu_wnd*   wnd      = 0;
#if defined SIZE_132X25
    texu_i32    cxScreen = 132;
    texu_i32    cyScreen = 25;
#else
    texu_i32    cxScreen = 80;
    texu_i32    cyScreen = 24;
#endif


    /*initialize libtexu*/
    rc = TexuStartup(cyScreen, cxScreen); /*25 lines x 80 characters per line*/
    if (rc != TEXU_OK)
    {
        printf("cannot initialized environment\n");
        return -1;
    }

    /*register class*/
    rc = TexuRegisterClass("DEMOWND_CLASS", DemoWndProc);
    rc = TexuRegisterClass("DEMOWND_P1_CLASS", DemoP1WndProc);
    rc = TexuRegisterClass("DEMOWND_P2_CLASS", DemoP2WndProc);
    rc = TexuRegisterClass("DEMOWND_P3_CLASS", DemoP3WndProc);

    wnd      = TexuCreateWindow("Demo",            /*title*/
                                "DEMOWND_CLASS",      /*class*/
                                TEXU_WS_CLIPWINDOW, /* style*/
                                0,                  /* exstyle*/
                                0,                  /* y */
                                0,                  /* x */
                                cyScreen,           /*height*/
                                cxScreen,           /*width*/
                                0,                  /* parent */
                                3,        /* id */
                                0      /* user data */
         );

    TexuShowWindow(wnd, TEXU_SW_SHOW);
    TexuInvalidateWindow(wnd);
    TexuRun(); /*main loop*/

    TexuShutdown(); /*release*/
    

    return 0;
}
