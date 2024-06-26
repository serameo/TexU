/*
 #
 # File Name: texuix.h
 # Author: Seree Rakwong
 # Date: 17-JUN-2019
 #
 */

#ifndef _TEXUIX_H_
#define _TEXUIX_H_

/*
 # TexU definitions and macros
 #
         1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
 */

/*EDIT MACROS*/
#define TexuEdit_LimitText(wnd, limit) \
    texu_wnd_send_msg(wnd, TEXU_EM_LIMITTEXT, (texu_i32)(limit), 0)

#define TexuEdit_SetPasswordChar(wnd, ch) \
    texu_wnd_send_msg(wnd, TEXU_EM_SETPASSWDCHAR, (texu_lparam)(texu_char)(ch), 0)

#define TexuEdit_SetDecimalWidth(wnd, width) \
    texu_wnd_send_msg(wnd, TEXU_EM_SETDECWIDTH, (texu_lparam)(texu_i32)(width), 0)

#define TexuEdit_ShowPasswordChar(wnd, show) \
    texu_wnd_send_msg(wnd, TEXU_EM_SHOWPASSWDCHAR, (texu_lparam)(texu_bool)(show), 0)

#define TexuEdit_SetValidString(wnd, valid) \
    texu_wnd_send_msg(wnd, TEXU_EM_SETVALIDSTRING, (texu_lparam)(const texu_char *)(valid), 0)

#define TexuEdit_SetValidMinMax(wnd, valid) \
    texu_wnd_send_msg(wnd, TEXU_EM_SETVALIDMINMAX, (texu_lparam)(const texu_editminmax *)(valid), 0)

/*LISTBOX MACROS*/
#define TexuListBox_AddItem(wnd, text) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LBM_ADDITEM, (texu_lparam)(const texu_char *)(text), 0)

#define TexuListBox_DeleteItem(wnd, idx) \
    texu_wnd_send_msg(wnd, TEXU_LBM_DELETEITEM, (texu_lparam)(texu_i32)(idx), 0)

#define TexuListBox_GetCurSel(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LBM_GETCURSEL, 0, 0)

#define TexuListBox_SetCurSel(wnd, idx) \
    texu_wnd_send_msg(wnd, TEXU_LBM_SETCURSEL, (texu_lparam)(texu_i32)(idx), 0)

#define TexuListBox_DeleteAllItems(wnd) \
    texu_wnd_send_msg(wnd, TEXU_LBM_DELETEALLITEMS, 0, 0)

#define TexuListBox_GetItemCount(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LBM_GETITEMCOUNT, 0, 0)

#define TexuListBox_GetItemData(wnd, idx) \
    (texu_longptr) texu_wnd_send_msg(wnd, TEXU_LBM_GETITEMDATA, (texu_lparam)(texu_i32)(idx), 0)

#define TexuListBox_SetItemData(wnd, idx, data) \
    texu_wnd_send_msg(wnd, TEXU_LBM_SETITEMDATA, (texu_i32)(idx), (texu_lparam)(data))

#define TexuListBox_SetItemText(wnd, idx, text) \
    texu_wnd_send_msg(wnd, TEXU_LBM_SETITEMTEXT, (texu_i32)(idx), (texu_lparam)(const texu_char *)(text))

#define TexuListBox_GetItemText(wnd, idx, text) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LBM_GETITEMTEXT, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_char *)(text))

#define TexuListBox_CountItemChecked(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LBM_COUNTITEMCHECKED, 0, 0)

#define TexuListBox_SetItemChecked(wnd, idx, checked) \
    texu_wnd_send_msg(wnd, TEXU_LBM_SETITEMCHECKED, (texu_i32)(idx), (texu_lparam)(texu_bool)(checked))

#define TexuListBox_GetItemChecked(wnd, idx) \
    (texu_bool) texu_wnd_send_msg(wnd, TEXU_LBM_GETITEMCHECKED, (texu_lparam)(texu_i32)(idx), 0)

#define TexuListBox_EnableItem(wnd, idx, enable) \
    texu_wnd_send_msg(wnd, TEXU_LBM_ENABLEITEM, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_bool)(enable))

#define TexuListBox_SetSelColor(wnd, color) \
    texu_wnd_send_msg(wnd, TEXU_LBM_SETSELCOLOR, (texu_lparam)(texu_i32)(color), 0)

/*COMBOBOX MACROS*/
#define TexuComboBox_AddItem(wnd, text) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_CBM_ADDITEM, (texu_lparam)(const texu_char *)(text), 0)

#define TexuComboBox_DeleteItem(wnd, idx) \
    texu_wnd_send_msg(wnd, TEXU_CBM_DELETEITEM, (texu_lparam)(texu_i32)(idx), 0)

#define TexuComboBox_GetCurSel(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_CBM_GETCURSEL, 0, 0)

#define TexuComboBox_SetCurSel(wnd, idx) \
    texu_wnd_send_msg(wnd, TEXU_CBM_SETCURSEL, (texu_lparam)(texu_i32)(idx), 0)

#define TexuComboBox_DeleteAllItems(wnd) \
    texu_wnd_send_msg(wnd, TEXU_CBM_DELETEALLITEMS, 0, 0)

#define TexuComboBox_GetItemCount(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_CBM_GETITEMCOUNT, 0, 0)

#define TexuComboBox_GetItemData(wnd, idx) \
    (texu_lparam) texu_wnd_send_msg(wnd, TEXU_CBM_GETITEMDATA, (texu_i32)(idx), 0)

#define TexuComboBox_SetItemData(wnd, idx, data) \
    texu_wnd_send_msg(wnd, TEXU_CBM_SETITEMDATA, (texu_lparam)(texu_i32)(idx), (texu_lparam)(data))

#define TexuComboBox_SetItemText(wnd, idx, text) \
    texu_wnd_send_msg(wnd, TEXU_CBM_SETITEMTEXT, (texu_lparam)(texu_i32)(idx), (texu_lparam)(const texu_char *)(text))

#define TexuComboBox_GetItemText(wnd, idx, text) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_CBM_GETITEMTEXT, (texu_i32)(idx), (texu_lparam)(texu_char *)(text))

#define TexuComboBox_CountItemChecked(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_CBM_COUNTITEMCHECKED, 0, 0)

#define TexuComboBox_SetItemChecked(wnd, idx, checked) \
    texu_wnd_send_msg(wnd, TEXU_CBM_SETITEMCHECKED, (texu_i32)(idx), (texu_lparam)(texu_bool)(checked))

#define TexuComboBox_GetItemChecked(wnd, idx) \
    (texu_bool) texu_wnd_send_msg(wnd, TEXU_CBM_GETITEMCHECKED, (texu_lparam)(texu_i32)(idx), 0)

#define TexuComboBox_EnableItem(wnd, idx, enable) \
    texu_wnd_send_msg(wnd, TEXU_CBM_ENABLEITEM, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_bool)(enable))

#define TexuComboBox_GetListBox(wnd, color) \
    (texu_wnd *)texu_wnd_send_msg(wnd, TEXU_CBM_GETLISTBOX, 0, 0)

/*STATUSBAR*/
#define TexuStatusBar_AddPart(wnd, text, width) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_SBM_ADDPART, (texu_lparam)(const texu_char *)(text), (texu_lparam)(texu_i32)(width))

#define TexuStatusBar_SetText(wnd, idx, text) \
    texu_wnd_send_msg(wnd, TEXU_SBM_SETTEXT, (texu_lparam)(texu_i32)(idx), (texu_lparam)(const texu_char *)(text))

#define TexuStatusBar_GetText(wnd, idx) \
    (const texu_char *)texu_wnd_send_msg(wnd, TEXU_SBM_GETTEXT, (texu_lparam)(texu_i32)(idx), 0)

#define TexuStatusBar_SetWidth(wnd, idx, width) \
    texu_wnd_send_msg(wnd, TEXU_SBM_SETWIDTH, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_i32)(width))

#define TexuStatusBar_GetWidth(wnd, idx) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_SBM_GETWIDTH, (texu_lparam)(texu_i32)(idx), 0)

#define TexuStatusBar_SetAlign(wnd, idx, align) \
    texu_wnd_send_msg(wnd, TEXU_SBM_SETALIGN, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_i32)(align))

#define TexuStatusBar_GetAlign(wnd, idx) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_SBM_GETALIGN, (texu_lparam)(texu_i32)(idx), 0)

#define TexuStatusBar_SetColor(wnd, idx, color) \
    texu_wnd_send_msg(wnd, TEXU_SBM_SETCOLOR, (texu_lparam)(texu_i32)(idx), (texu_lparam)(texu_i32)(color))

#define TexuStatusBar_GetColor(wnd, idx) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_SBM_GETCOLOR, (texu_lparam)(texu_i32)(idx), 0)

/*LISTCTRL*/
#define TexuListCtrl_AddColumn(wnd, header) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_ADDCOLUMN, (texu_lparam)(const texu_wnd_header *)(header), 0)

#define TexuListCtrl_DeleteColumn(wnd, col) \
    texu_wnd_send_msg(wnd, TEXU_LCM_DELETECOLUMN, (texu_lparam)(texu_i32)(col), 0)

#define TexuListCtrl_DeleteAllColumns(wnd) \
    texu_wnd_send_msg(wnd, TEXU_LCM_DELETEALLCOLUMNS, 0, 0)

#define TexuListCtrl_AddItem(wnd, text) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_ADDITEM, (texu_lparam)(const texu_char *)(text), 0)

#define TexuListCtrl_AddItems(wnd, text, nitems) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_ADDITEMS, (texu_lparam)(texu_char *)(text), (texu_lparam)(texu_i32)(nitems))

#define TexuListCtrl_DeleteItem(wnd, row) \
    texu_wnd_send_msg(wnd, TEXU_LCM_DELETEITEM, (texu_lparam)(texu_i32)(row), 0)

#define TexuListCtrl_DeleteAllItems(wnd) \
    texu_wnd_send_msg(wnd, TEXU_LCM_DELETEALLITEMS, 0, 0)

#define TexuListCtrl_GetItem(wnd, row, subitem) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_GETITEM, (texu_lparam)(texu_i32)(row), (texu_lparam)(texu_wnd_subitem *)subitem)

#define TexuListCtrl_SetItem(wnd, row, subitem) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_SETITEM, (texu_lparam)(texu_i32)(row), (texu_lparam)(const texu_wnd_subitem *)(subitem))

#define TexuListCtrl_GetCurRow(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_GETCURROW, 0, 0)

#define TexuListCtrl_SetCurRow(wnd, row) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_SETCURROW, (texu_lparam)(texu_i32)(row), 0)

#define TexuListCtrl_GetCurPage(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_GETCURPAGE, 0, 0)

#define TexuListCtrl_SetCurPage(wnd, page) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_SETCURPAGE, (texu_lparam)(texu_i32)(row), 0)

#define TexuListCtrl_GetItemsPerPage(wnd, nitems) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_GETITEMSPERPAGE, (texu_lparam)(texu_i32)(nitems), 0)

#define TexuListCtrl_SetEditableCols(wnd, nitems, pcols) \
    texu_wnd_send_msg(wnd, TEXU_LCM_SETEDITABLECOLS, (texu_lparam)(texu_i32)(nitems), (texu_lparam)(const texu_i32 *)(pcols))

#define TexuListCtrl_GetEditBox(wnd) \
    (texu_wnd *)texu_wnd_send_msg(wnd, TEXU_LCM_GETEDITBOX, 0, 0)

#define TexuListCtrl_GetEditStyle(wnd) \
    (texu_ui32) texu_wnd_send_msg(wnd, TEXU_LCM_GETEDITSTYLE, 0, 0)

#define TexuListCtrl_SetEditStyle(wnd, style) \
    texu_wnd_send_msg(wnd, TEXU_LCM_SETEDITSTYLE, (texu_lparam)(texu_ui32)(style), 0)

#define TexuListCtrl_GetItemCount(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_LCM_GETITEMCOUNT, 0, 0)

#define TexuListCtrl_SetColWidth(wnd, col, width) \
    texu_wnd_send_msg(wnd, TEXU_LCM_SETEDITSTYLE, (texu_lparam)(texu_i32)(col), (texu_lparam)(texu_i32)(width))

#define TexuListCtrl_InvalidateItem(wnd, row, col) \
    texu_wnd_send_msg(wnd, TEXU_LCM_INVALIDATEITEM, (texu_lparam)(texu_i32)(row), (texu_i32)(col))

/*TREECTRL*/
#define TexuTreeCtrl_ExpandAllItems(wnd) \
    texu_wnd_send_msg(wnd, TEXU_TCM_EXPANDALLITEMS, 0, 0);

#define TexuTreeCtrl_CollapseAllItems(wnd) \
    texu_wnd_send_msg(wnd, TEXU_TCM_COLLAPSEALLITEMS, 0, 0);

#define TexuTreeCtrl_ImportFromFile(wnd, fp, proc) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_IMPORTFROMFILE, (texu_lparam)(FILE *)(fp), (texu_lparam)(texu_tree_imp_proc)(proc));

#define TexuTreeCtrl_ExportFromFile(wnd, fp, proc) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_EXPORTFROMFILE, (texu_lparam)(FILE *)(fp), (texu_lparam)(texu_tree_exp_proc)(proc));

#define TexuTreeCtrl_GetSelItem(wnd) \
    (texu_tree_item *)texu_wnd_send_msg(wnd, TEXU_TCM_GETSELITEM, 0, 0)

#define TexuTreeCtrl_SetSelItem(wnd, item) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_SETSELITEM, (texu_lparam)(texu_tree_item *)(item), 0)

#define TexuTreeCtrl_SetFindItemProc(wnd, proc) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_SETFINDITEMPROC, (texu_lparam)(texu_tree_find_proc *)(proc), 0)

#define TexuTreeCtrl_ExpandItem(wnd, item) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_EXPANDITEM, (texu_lparam)(texu_tree_item *)(item), 0)

#define TexuTreeCtrl_CollapseItem(wnd, item) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_COLLAPSEITEM, (texu_lparam)(texu_tree_item *)(item), 0)

#define TexuTreeCtrl_FindItem(wnd, item, userdata) \
    (texu_tree_item *)texu_wnd_send_msg(wnd, TEXU_TCM_FINDITEM, (texu_lparam)(texu_treewnd_item *)(item), (texu_lparam)(void *)(userdata))

#define TexuTreeCtrl_FindNextItem(wnd, previtem, item) \
    (texu_tree_item *)texu_wnd_send_msg(wnd, TEXU_TCM_FINDNEXTITEM, (texu_lparam)(texu_tree_item *)(previtem), (texu_lparam)(texu_treewnd_item *)(item))

#define TexuTreeCtrl_GetItem(wnd, item, wnditem) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_GETITEM, (texu_lparam)(texu_tree_item *)(item), (texu_lparam)(texu_treewnd_item *)(wnditem))

#define TexuTreeCtrl_SetItem(wnd, item, wnditem) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_SETITEM, (texu_lparam)(texu_tree_item *)(item), (texu_lparam)(const texu_treewnd_item *)(wnditem))

#define TexuTreeCtrl_DeleteItem(wnd, item) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_DELETEITEM, (texu_lparam)(texu_tree_item *)(item), 0)

#define TexuTreeCtrl_DeleteItem(wnd, item) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_DELETEITEM, (texu_lparam)(texu_tree_item *)(item), 0)

#define TexuTreeCtrl_InsertItem(wnd, item, wnditem) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_TCM_INSERTITEM, (texu_lparam)(texu_tree_item *)(item), (texu_lparam)(const texu_treewnd_item *)(wnditem))

/*UPDOWN CONTROL*/
#define TexuUpDownCtrl_SetMinMax(wnd, min, max) \
    texu_wnd_send_msg(wnd, TEXU_UDCM_SETMINMAX, (texu_lparam)(texu_i32)(min), (texu_lparam)(texu_i32)(max))

#define TexuUpDownCtrl_GetMinMax(wnd, min, max) \
    (texu_status) texu_wnd_send_msg(wnd, TEXU_UDCM_GETMINMAX, (texu_lparam)(texu_i32 *)(min), (texu_lparam)(texu_i32 *)(max))

#define TexuUpDownCtrl_SetStep(wnd, updown) \
    texu_wnd_send_msg(wnd, TEXU_UDCM_SETSTEP, (texu_lparam)(texu_i32)(updown), 0)

#define TexuUpDownCtrl_GetStep(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_UDCM_GETSTEP, 0, 0)

#define TexuUpDownCtrl_Step(wnd, updown) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_UDCM_STEP, (texu_lparam)(texu_i32)(updown), 0)

#define TexuUpDownCtrl_SetPage(wnd, page) \
    texu_wnd_send_msg(wnd, TEXU_UDCM_SETPAGE, (texu_lparam)(texu_i32)(page), 0)

#define TexuUpDownCtrl_GetPage(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_UDCM_GETPAGE, 0, 0)

#define TexuUpDownCtrl_Page(wnd, page) \
    texu_wnd_send_msg(wnd, TEXU_UDCM_PAGE, (texu_lparam)(texu_i32)(page), 0)

#define TexuUpDownCtrl_GetInt(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_UDCM_GETINT, 0, 0)

/* PROGRESSBAR CONTROL*/
#define TexuProgressBar_SetMax(wnd, max) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_SETMAX, (texu_lparam)(texu_i32)(max), 0)

#define TexuProgressBar_GetMax(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_PGBM_GETMAX, 0, 0)

#define TexuProgressBar_SetPos(wnd, pos) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_SETPOS, (texu_lparam)(texu_i32)(pos), 0)

#define TexuProgressBar_GetPos(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_PGBM_GETPOS, 0, 0)

#define TexuProgressBar_SetStep(wnd, step) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_SETSTEP, (texu_lparam)(texu_i32)(step), 0)

#define TexuProgressBar_GetStep(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_PGBM_GETSTEP, 0, 0)

#define TexuProgressBar_Step(wnd, updown) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_STEP, (texu_lparam)(texu_i32)(updown), 0)

#define TexuProgressBar_GetPercent(wnd, pct) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_GETPERCENT, (texu_lparam)(texu_f32 *)(pct), 0)

#define TexuProgressBar_GetDecimal(wnd) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_GETDECIMAL, 0, 0)

#define TexuProgressBar_SetDecimal(wnd, dec) \
    texu_wnd_send_msg(wnd, TEXU_PGBM_SETDECIMAL, (texu_i32)dec, 0)

/*PAGECTRL*/
#define TexuPageCtrl_AddPage(wnd, clsname, id) \
    (texu_status) texu_wnd_send_msg(wnd, TEXU_PGM_ADDPAGE, (texu_lparam)(const texu_char *)(pct), (texu_lparam)(texu_ui32)(id))

#define TexuPageCtrl_SetCurPage(wnd, pageidx) \
    (texu_wnd *)texu_wnd_send_msg(wnd, TEXU_PGM_SETCURPAGE, (texu_lparam)(texu_i32)(pageidx), 0)

#define TexuPageCtrl_GetCurPage(wnd) \
    (texu_wnd *)texu_wnd_send_msg(wnd, TEXU_PGM_GETCURPAGE, 0, 0)

#define TexuPageCtrl_CountPage(wnd) \
    (texu_i32) texu_wnd_send_msg(wnd, TEXU_PGM_COUNTPAGE, 0, 0)

#define TexuPageCtrl_GetPageIndex(wnd, page) \
    (texu_i32)texu_wnd_send_msg(wnd, TEXU_PGM_GETPAGEINDEX, (texu_lparam)(texu_i32)(texu_i32)(page), 0)

/*REBARCTRL*/
#define TexuReBarCtrl_AddBand(wnd, band) \
    (texu_i32)texu_wnd_send_msg(wnd, TEXU_RBM_ADDBAND, (texu_lparam)(const texu_rbwnd_band*)(band), 0)

#define TexuReBarCtrl_SetInfoWidth(wnd, width) \
    (texu_i32)texu_wnd_send_msg(wnd, TEXU_RBM_SETINFOWIDTH, (texu_lparam)(texu_i32)(width), 0)

/*IPADDRESSCTRL*/
#define TexuIpAddressCtrl_SetMinMax(wnd, min, max) \
    texu_wnd_send_msg(wnd, TEXU_IPM_SETMINMAX, (texu_lparam)(texu_i32)(min), (texu_lparam)(texu_i32)(max))

#define TexuIpAddressCtrl_GetMinMax(wnd, min, max) \
    (texu_status)texu_wnd_send_msg(wnd, TEXU_IPM_SETMINMAX, (texu_lparam)(texu_i32*)(min), (texu_lparam)(texu_i32*)(max))

#define TexuIpAddressCtrl_SetStep(wnd, step) \
    (texu_status)texu_wnd_send_msg(wnd, TEXU_IPM_SETSTEP, (texu_lparam)(texu_i32)(step), 0)

#define TexuIpAddressCtrl_GetStep(wnd) \
    (texu_i32)texu_wnd_send_msg(wnd, TEXU_IPM_GETSTEP, 0, 0)

#define TexuIpAddressCtrl_Step(wnd) \
    texu_wnd_send_msg(wnd, TEXU_IPM_STEP, 0, 0)

#define TexuIpAddressCtrl_SetPage(wnd, page) \
    (texu_status)texu_wnd_send_msg(wnd, TEXU_IPM_SETPAGE, (texu_lparam)(texu_i32)(page), 0)

#define TexuIpAddressCtrl_GetPage(wnd) \
    (texu_i32)texu_wnd_send_msg(wnd, TEXU_IPM_GETPAGE, 0, 0)

#define TexuIpAddressCtrl_Page(wnd) \
    texu_wnd_send_msg(wnd, TEXU_IPM_PAGE, 0, 0)

#define TexuIpAddressCtrl_GetIpAddress(wnd, ip) \
    texu_wnd_send_msg(wnd, TEXU_IPM_GETIPADDRESS, (texu_lparam)(texu_ip_addr*)(ip), 0)

/*EDITMASKCTRL*/
#define TexuEditMaskCtrl_SetMask(wnd, mask) \
    texu_wnd_send_msg(wnd, TEXU_EMM_SETMASK, (texu_lparam)(const texu_char *)(mask), 0)

#define TexuEditMaskCtrl_GetMask(wnd, mask, len) \
    texu_wnd_send_msg(wnd, TEXU_EMM_SETMASK, (texu_lparam)(texu_char *)(mask), (texu_lparam)(texu_i32)(len))

#define TexuEditMaskCtrl_SetInfo(wnd, info) \
    texu_wnd_send_msg(wnd, TEXU_EMM_SETINFO, (texu_lparam)(const texu_char *)(info), 0)

/*EDITPRICESPREAD*/
#define TexuEditPriceSpread_InsertSpread(wnd, spread, ps) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_INSERTSPREAD, (texu_lparam)(texu_i32)(spread), (texu_lparam)(const texu_pricespread*)(ps))

#define TexuEditPriceSpread_DeleteSpread(wnd, spread) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_DELETESPREAD, (texu_lparam)(texu_i32)(spread), 0)

#define TexuEditPriceSpread_SetBaseSpread(wnd, base) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_SETBASESPREAD, (texu_lparam)(const texu_baseprice*)(base), 0)

#define TexuEditPriceSpread_GetBaseSpread(wnd, base) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_GETBASESPREAD, (texu_lparam)(texu_baseprice*)(base), 0)

#define TexuEditPriceSpread_SetPriceDecimal(wnd, dec) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_SETPRICEDECIMAL, (texu_lparam)(texu_i32)(dec), 0)

#define TexuEditPriceSpread_LoadDefaultSpreads(wnd) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_LOADDEFAULTSPREADS, 0, 0)

#define TexuEditPriceSpread_LoadSpreads(wnd, ps, nprices) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_LOADSPREADS, (texu_lparam)(const texu_pricespread*)(ps), (texu_lparam)(texu_i32)(nprices))

#define TexuEditPriceSpread_AllowOverCeilingFloor(wnd, allow) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_ALLOWOVERCEILINGFLOOR, (texu_lparam)(texu_bool)(allow), 0)

#define TexuEditPriceSpread_CorrectCeilingFloor(wnd, correct) \
    texu_wnd_send_msg(wnd, TEXU_EPSM_CORRECTCEILINGFLOOR, (texu_lparam)(texu_bool)(correct), 0)






#endif /*_TEXUIX_H_*/
