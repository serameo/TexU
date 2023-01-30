# TexU
Text based user interface library using ncurses library. To see, how to create a simple application under src/test_texu.c.

To create your own controls, see src/texuproc.c and src/texuctrl.c

These files are included many text objects by the following:
- Window
- Label (TEXU_LABEL_CLASS)
- Button (TEXU_BUTTON_CLASS)
- Edit (TEXU_EDIT_CLASS)
- List box (radio (TEXU_LBS_RADIOBOX), check box (TEXU_LBS_CHECKBOK), or simple list box) (TEXU_LISTBOX_CLASS)
- Combo box (TEXU_COMBOBOX_CLASS)
- Status bar (TEXU_STATUSBAR_CLASS)
- List control (TEXU_LISTCTRL_CLASS)
- Tree control (TEXU_TREECTRL_CLASS)
- Up/down control (TEXU_UPDOWNCTRL_CLASS)
- Progress bar control (TEXU_PROGRESSBAR_CLASS)
- Menu (TEXU_MENU_CLASS / TEXU_MENUWND_CLASS)
- Page control (TEXU_PAGECTRL_CLASS) -- This class could hold many windows as same as a book object.
- ReBar control (TEXU_REBAR_CLASS) -- This class is a simple container to add a common control to be scrollable controls in its bound.
- IP Address control (TEXU_IPADDRESSCTRL_CLASS)
- Edit Mask control (TEXU_EDITMASKCTRL_CLASS) -- Using regular expression to validate text (See also https://github.com/kokke/tiny-regex-c)
- Edit Price Spread control (TEXU_EDITPRICESPREADCTRL_CLASS) -- This class an enhanced Edit control to automatically highlight color when the base price is available
- Simple console I/O (e.g. writing/reading a character, drawing a line, drawing a rectangle etc.)

All text objects are implemented by using a simple library (TexU_List, TexU_Stack, TexU_Queue, TexU_Tree, etc.) See also include/texulib.h. Also, some utilities such as texu_dblog object is using sqlite library to handle a database log file.

This library concept is the same as Win32 API concept. All window classes are required to register into the TexU environment. And then, you could implement your message by your procedure.

TexU could simply be applied with Tcl script to run simple automate test. See samples/TermUI and samples/auitclsh.

How to build:
-------------
- $ make

How to implement a simple TexU application:
-------------------------------------------
0. Call TexuStartup()
1. Register all classes calls TexuRegisterClass()
2. Create a main window calls TexuCreateWindow()
3. Create a page control under the main window
4. Add pages that already registered classed above
5. Add a rebar object if you application would like to be able to scroll many controls.
6. Update the main window calls TexuShowWindow() and then TexuInvalidateWindow() to display the first time
7. Run TexuRun()
8. End of program calls TexuShutdown()

<p align="center">
  <img src="https://github.com/serameo/TexU/blob/master/samples/TexU_sample06.png" width="640" title="Sample1">
</p>
<p align="center">
  <img src="https://github.com/serameo/TexU/blob/master/samples/TexU_sample07.png" width="640" title="Sample2">
</p>
