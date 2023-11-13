#include "global.hpp"

//Internal

#define PrintTextM(i, s) PrintOutputTextSixTab(i, s, #s)

#define PrintWindowMessage(m) \
	case m: { \
		printf("WindowMessage: "); \
		puts(#m); \
		break; \
	}

int d_count;

void PrintOutputTextSixTab (int _target, int _style, const char* _output) {
	if (_target & _style) {
		printf("%s | ", _output);
		d_count++;
	}
	if (d_count == 6) {
		printf("\n        ");
		d_count = 0;
	}
}

//External

void Debug_PrintWindowStyle (int _ws) {
	d_count = 0;
	PrintTextM(_ws, WS_BORDER);
	PrintTextM(_ws, WS_CAPTION);
	PrintTextM(_ws, WS_CHILD);
	PrintTextM(_ws, WS_CHILDWINDOW);
	PrintTextM(_ws, WS_CLIPCHILDREN);
	PrintTextM(_ws, WS_CLIPSIBLINGS);
	PrintTextM(_ws, WS_DISABLED);
	PrintTextM(_ws, WS_DLGFRAME);
	PrintTextM(_ws, WS_GROUP);
	PrintTextM(_ws, WS_HSCROLL);
	PrintTextM(_ws, WS_ICONIC);
	PrintTextM(_ws, WS_MAXIMIZE);
	PrintTextM(_ws, WS_MAXIMIZEBOX);
	PrintTextM(_ws, WS_MINIMIZE);
	PrintTextM(_ws, WS_MINIMIZEBOX);
	PrintTextM(_ws, WS_OVERLAPPED);
	PrintTextM(_ws, WS_POPUP);
	PrintTextM(_ws, WS_POPUPWINDOW);
	PrintTextM(_ws, WS_SIZEBOX);
	PrintTextM(_ws, WS_SYSMENU);
	PrintTextM(_ws, WS_TABSTOP);
	PrintTextM(_ws, WS_THICKFRAME);
	PrintTextM(_ws, WS_TILED);
	PrintTextM(_ws, WS_TILEDWINDOW);
	PrintTextM(_ws, WS_VISIBLE);
	PrintTextM(_ws, WS_VSCROLL);
}

void Debug_PrintWindowExStyle (int _ws) {
	d_count = 0;
	PrintTextM(_ws, WS_EX_ACCEPTFILES);
	PrintTextM(_ws, WS_EX_APPWINDOW);
	PrintTextM(_ws, WS_EX_CLIENTEDGE);
	PrintTextM(_ws, WS_EX_COMPOSITED);
	PrintTextM(_ws, WS_EX_CONTEXTHELP);
	PrintTextM(_ws, WS_EX_CONTROLPARENT);
	PrintTextM(_ws, WS_EX_DLGMODALFRAME);
	PrintTextM(_ws, WS_EX_LAYERED);
	PrintTextM(_ws, WS_EX_LAYOUTRTL);
	PrintTextM(_ws, WS_EX_LEFT);
	PrintTextM(_ws, WS_EX_LEFTSCROLLBAR);
	PrintTextM(_ws, WS_EX_LTRREADING);
	PrintTextM(_ws, WS_EX_MDICHILD);
	PrintTextM(_ws, WS_EX_NOACTIVATE);
	PrintTextM(_ws, WS_EX_NOINHERITLAYOUT);
	PrintTextM(_ws, WS_EX_NOPARENTNOTIFY);
	PrintTextM(_ws, WS_EX_NOREDIRECTIONBITMAP);
	PrintTextM(_ws, WS_EX_RIGHT);
	PrintTextM(_ws, WS_EX_RIGHTSCROLLBAR);
	PrintTextM(_ws, WS_EX_RTLREADING);
	PrintTextM(_ws, WS_EX_STATICEDGE);
	PrintTextM(_ws, WS_EX_TOOLWINDOW);
	PrintTextM(_ws, WS_EX_TOPMOST);
	PrintTextM(_ws, WS_EX_TRANSPARENT);
	PrintTextM(_ws, WS_EX_WINDOWEDGE);
}

void Debug_ConvertWindowMessage (UINT lp_msg) {
	#ifdef _RESTRICT_WM
	return;
	#endif
	
	switch (lp_msg) {
		PrintWindowMessage(WM_NULL);
		PrintWindowMessage(WM_CREATE);
		PrintWindowMessage(WM_DESTROY);
		PrintWindowMessage(WM_MOVE);
		PrintWindowMessage(WM_SIZE);
		PrintWindowMessage(WM_ACTIVATE);
		PrintWindowMessage(WM_SETFOCUS);
		PrintWindowMessage(WM_KILLFOCUS);
		PrintWindowMessage(WM_ENABLE);
		PrintWindowMessage(WM_SETREDRAW);
		PrintWindowMessage(WM_SETTEXT);
		PrintWindowMessage(WM_GETTEXT);
		PrintWindowMessage(WM_GETTEXTLENGTH);
		PrintWindowMessage(WM_PAINT);
		PrintWindowMessage(WM_CLOSE);
		PrintWindowMessage(WM_QUERYENDSESSION);
		PrintWindowMessage(WM_QUIT);
		PrintWindowMessage(WM_QUERYOPEN);
		PrintWindowMessage(WM_ERASEBKGND);
		PrintWindowMessage(WM_SYSCOLORCHANGE);
		PrintWindowMessage(WM_ENDSESSION);
		PrintWindowMessage(WM_SHOWWINDOW);
		PrintWindowMessage(WM_CTLCOLOR);
		PrintWindowMessage(WM_WININICHANGE);
		PrintWindowMessage(WM_DEVMODECHANGE);
		PrintWindowMessage(WM_ACTIVATEAPP);
		PrintWindowMessage(WM_FONTCHANGE);
		PrintWindowMessage(WM_TIMECHANGE);
		PrintWindowMessage(WM_CANCELMODE);
		PrintWindowMessage(WM_SETCURSOR);
		PrintWindowMessage(WM_MOUSEACTIVATE);
		PrintWindowMessage(WM_CHILDACTIVATE);
		PrintWindowMessage(WM_QUEUESYNC);
		PrintWindowMessage(WM_GETMINMAXINFO);
		PrintWindowMessage(WM_PAINTICON);
		PrintWindowMessage(WM_ICONERASEBKGND);
		PrintWindowMessage(WM_NEXTDLGCTL);
		PrintWindowMessage(WM_SPOOLERSTATUS);
		PrintWindowMessage(WM_DRAWITEM);
		PrintWindowMessage(WM_MEASUREITEM);
		PrintWindowMessage(WM_DELETEITEM);
		PrintWindowMessage(WM_VKEYTOITEM);
		PrintWindowMessage(WM_CHARTOITEM);
		PrintWindowMessage(WM_SETFONT);
		PrintWindowMessage(WM_GETFONT);
		PrintWindowMessage(WM_SETHOTKEY);
		PrintWindowMessage(WM_GETHOTKEY);
		PrintWindowMessage(WM_QUERYDRAGICON);
		PrintWindowMessage(WM_COMPAREITEM);
		PrintWindowMessage(WM_GETOBJECT);
		PrintWindowMessage(WM_COMPACTING);
		PrintWindowMessage(WM_COMMNOTIFY);
		PrintWindowMessage(WM_WINDOWPOSCHANGING);
		PrintWindowMessage(WM_WINDOWPOSCHANGED);
		PrintWindowMessage(WM_POWER);
		PrintWindowMessage(WM_COPYDATA);
		PrintWindowMessage(WM_CANCELJOURNAL);
		PrintWindowMessage(WM_NOTIFY);
		PrintWindowMessage(WM_INPUTLANGCHANGEREQUEST);
		PrintWindowMessage(WM_INPUTLANGCHANGE);
		PrintWindowMessage(WM_TCARD);
		PrintWindowMessage(WM_HELP);
		PrintWindowMessage(WM_USERCHANGED);
		PrintWindowMessage(WM_NOTIFYFORMAT);
		PrintWindowMessage(WM_CONTEXTMENU);
		PrintWindowMessage(WM_STYLECHANGING);
		PrintWindowMessage(WM_STYLECHANGED);
		PrintWindowMessage(WM_DISPLAYCHANGE);
		PrintWindowMessage(WM_GETICON);
		PrintWindowMessage(WM_SETICON);
		PrintWindowMessage(WM_NCCREATE);
		PrintWindowMessage(WM_NCDESTROY);
		PrintWindowMessage(WM_NCCALCSIZE);
		PrintWindowMessage(WM_NCHITTEST);
		PrintWindowMessage(WM_NCPAINT);
		PrintWindowMessage(WM_NCACTIVATE);
		PrintWindowMessage(WM_GETDLGCODE);
		PrintWindowMessage(WM_SYNCPAINT);
		PrintWindowMessage(WM_NCMOUSEMOVE);
		PrintWindowMessage(WM_NCLBUTTONDOWN);
		PrintWindowMessage(WM_NCLBUTTONUP);
		PrintWindowMessage(WM_NCLBUTTONDBLCLK);
		PrintWindowMessage(WM_NCRBUTTONDOWN);
		PrintWindowMessage(WM_NCRBUTTONUP);
		PrintWindowMessage(WM_NCRBUTTONDBLCLK);
		PrintWindowMessage(WM_NCMBUTTONDOWN);
		PrintWindowMessage(WM_NCMBUTTONUP);
		PrintWindowMessage(WM_NCMBUTTONDBLCLK);
		PrintWindowMessage(WM_NCXBUTTONDOWN);
		PrintWindowMessage(WM_NCXBUTTONUP);
		PrintWindowMessage(WM_NCXBUTTONDBLCLK);
		PrintWindowMessage(EM_GETSEL);
		PrintWindowMessage(EM_SETSEL);
		PrintWindowMessage(EM_GETRECT);
		PrintWindowMessage(EM_SETRECT);
		PrintWindowMessage(EM_SETRECTNP);
		PrintWindowMessage(EM_SCROLL);
		PrintWindowMessage(EM_LINESCROLL);
		PrintWindowMessage(EM_SCROLLCARET);
		PrintWindowMessage(EM_GETMODIFY);
		PrintWindowMessage(EM_SETMODIFY);
		PrintWindowMessage(EM_GETLINECOUNT);
		PrintWindowMessage(EM_LINEINDEX);
		PrintWindowMessage(EM_SETHANDLE);
		PrintWindowMessage(EM_GETHANDLE);
		PrintWindowMessage(EM_GETTHUMB);
		PrintWindowMessage(EM_LINELENGTH);
		PrintWindowMessage(EM_REPLACESEL);
		PrintWindowMessage(EM_GETLINE);
		PrintWindowMessage(EM_SETLIMITTEXT);
		PrintWindowMessage(EM_CANUNDO);
		PrintWindowMessage(EM_UNDO);
		PrintWindowMessage(EM_FMTLINES);
		PrintWindowMessage(EM_LINEFROMCHAR);
		PrintWindowMessage(EM_SETTABSTOPS);
		PrintWindowMessage(EM_SETPASSWORDCHAR);
		PrintWindowMessage(EM_EMPTYUNDOBUFFER);
		PrintWindowMessage(EM_GETFIRSTVISIBLELINE);
		PrintWindowMessage(EM_SETREADONLY);
		PrintWindowMessage(EM_SETWORDBREAKPROC);
		PrintWindowMessage(EM_GETWORDBREAKPROC);
		PrintWindowMessage(EM_GETPASSWORDCHAR);
		PrintWindowMessage(EM_SETMARGINS);
		PrintWindowMessage(EM_GETMARGINS);
		PrintWindowMessage(EM_GETLIMITTEXT);
		PrintWindowMessage(EM_POSFROMCHAR);
		PrintWindowMessage(EM_CHARFROMPOS);
		PrintWindowMessage(EM_SETIMESTATUS);
		PrintWindowMessage(EM_GETIMESTATUS);
		PrintWindowMessage(SBM_SETPOS);
		PrintWindowMessage(SBM_GETPOS);
		PrintWindowMessage(SBM_SETRANGE);
		PrintWindowMessage(SBM_GETRANGE);
		PrintWindowMessage(SBM_ENABLE_ARROWS);
		PrintWindowMessage(SBM_SETRANGEREDRAW);
		PrintWindowMessage(SBM_SETSCROLLINFO);
		PrintWindowMessage(SBM_GETSCROLLINFO);
		PrintWindowMessage(SBM_GETSCROLLBARINFO);
		PrintWindowMessage(BM_GETCHECK);
		PrintWindowMessage(BM_SETCHECK);
		PrintWindowMessage(BM_GETSTATE);
		PrintWindowMessage(BM_SETSTATE);
		PrintWindowMessage(BM_SETSTYLE);
		PrintWindowMessage(BM_CLICK);
		PrintWindowMessage(BM_GETIMAGE);
		PrintWindowMessage(BM_SETIMAGE);
		PrintWindowMessage(BM_SETDONTCLICK);
		PrintWindowMessage(WM_INPUT);
		PrintWindowMessage(WM_KEYDOWN);
		PrintWindowMessage(WM_KEYUP);
		PrintWindowMessage(WM_CHAR);
		PrintWindowMessage(WM_DEADCHAR);
		PrintWindowMessage(WM_SYSKEYDOWN);
		PrintWindowMessage(WM_SYSKEYUP);
		PrintWindowMessage(WM_SYSCHAR);
		PrintWindowMessage(WM_SYSDEADCHAR);
		PrintWindowMessage(WM_UNICHAR);
		PrintWindowMessage(WM_IME_STARTCOMPOSITION);
		PrintWindowMessage(WM_IME_ENDCOMPOSITION);
		PrintWindowMessage(WM_IME_COMPOSITION);
		PrintWindowMessage(WM_INITDIALOG);
		PrintWindowMessage(WM_COMMAND);
		PrintWindowMessage(WM_SYSCOMMAND);
		PrintWindowMessage(WM_TIMER);
		PrintWindowMessage(WM_HSCROLL);
		PrintWindowMessage(WM_VSCROLL);
		PrintWindowMessage(WM_INITMENU);
		PrintWindowMessage(WM_INITMENUPOPUP);
		PrintWindowMessage(WM_MENUSELECT);
		PrintWindowMessage(WM_MENUCHAR);
		PrintWindowMessage(WM_ENTERIDLE);
		PrintWindowMessage(WM_MENURBUTTONUP);
		PrintWindowMessage(WM_MENUDRAG);
		PrintWindowMessage(WM_MENUGETOBJECT);
		PrintWindowMessage(WM_UNINITMENUPOPUP);
		PrintWindowMessage(WM_MENUCOMMAND);
		PrintWindowMessage(WM_CHANGEUISTATE);
		PrintWindowMessage(WM_UPDATEUISTATE);
		PrintWindowMessage(WM_QUERYUISTATE);
		PrintWindowMessage(WM_CTLCOLORMSGBOX);
		PrintWindowMessage(WM_CTLCOLOREDIT);
		PrintWindowMessage(WM_CTLCOLORLISTBOX);
		PrintWindowMessage(WM_CTLCOLORBTN);
		PrintWindowMessage(WM_CTLCOLORDLG);
		PrintWindowMessage(WM_CTLCOLORSCROLLBAR);
		PrintWindowMessage(WM_CTLCOLORSTATIC);
		PrintWindowMessage(CB_GETEDITSEL);
		PrintWindowMessage(CB_LIMITTEXT);
		PrintWindowMessage(CB_SETEDITSEL);
		PrintWindowMessage(CB_ADDSTRING);
		PrintWindowMessage(CB_DELETESTRING);
		PrintWindowMessage(CB_DIR);
		PrintWindowMessage(CB_GETCOUNT);
		PrintWindowMessage(CB_GETCURSEL);
		PrintWindowMessage(CB_GETLBTEXT);
		PrintWindowMessage(CB_GETLBTEXTLEN);
		PrintWindowMessage(CB_INSERTSTRING);
		PrintWindowMessage(CB_RESETCONTENT);
		PrintWindowMessage(CB_FINDSTRING);
		PrintWindowMessage(CB_SELECTSTRING);
		PrintWindowMessage(CB_SETCURSEL);
		PrintWindowMessage(CB_SHOWDROPDOWN);
		PrintWindowMessage(CB_GETITEMDATA);
		PrintWindowMessage(CB_SETITEMDATA);
		PrintWindowMessage(CB_GETDROPPEDCONTROLRECT);
		PrintWindowMessage(CB_SETITEMHEIGHT);
		PrintWindowMessage(CB_GETITEMHEIGHT);
		PrintWindowMessage(CB_SETEXTENDEDUI);
		PrintWindowMessage(CB_GETEXTENDEDUI);
		PrintWindowMessage(CB_GETDROPPEDSTATE);
		PrintWindowMessage(CB_FINDSTRINGEXACT);
		PrintWindowMessage(CB_SETLOCALE);
		PrintWindowMessage(CB_GETLOCALE);
		PrintWindowMessage(CB_GETTOPINDEX);
		PrintWindowMessage(CB_SETTOPINDEX);
		PrintWindowMessage(CB_GETHORIZONTALEXTENT);
		PrintWindowMessage(CB_SETHORIZONTALEXTENT);
		PrintWindowMessage(CB_GETDROPPEDWIDTH);
		PrintWindowMessage(CB_SETDROPPEDWIDTH);
		PrintWindowMessage(CB_INITSTORAGE);
		PrintWindowMessage(WM_MOUSEFIRST | WM_MOUSEMOVE);
		PrintWindowMessage(WM_LBUTTONDOWN);
		PrintWindowMessage(WM_LBUTTONUP);
		PrintWindowMessage(WM_LBUTTONDBLCLK);
		PrintWindowMessage(WM_RBUTTONDOWN);
		PrintWindowMessage(WM_RBUTTONUP);
		PrintWindowMessage(WM_RBUTTONDBLCLK);
		PrintWindowMessage(WM_MBUTTONDOWN);
		PrintWindowMessage(WM_MBUTTONUP);
		PrintWindowMessage(WM_MBUTTONDBLCLK);
		PrintWindowMessage(WM_MOUSELAST | WM_XBUTTONDBLCLK);
		PrintWindowMessage(WM_MOUSEWHEEL);
		PrintWindowMessage(WM_XBUTTONDOWN);
		PrintWindowMessage(WM_XBUTTONUP);
		PrintWindowMessage(WM_MOUSEHWHEEL);
		PrintWindowMessage(WM_PARENTNOTIFY);
		PrintWindowMessage(WM_ENTERMENULOOP);
		PrintWindowMessage(WM_EXITMENULOOP);
		PrintWindowMessage(WM_NEXTMENU);
		PrintWindowMessage(WM_SIZING);
		PrintWindowMessage(WM_CAPTURECHANGED);
		PrintWindowMessage(WM_MOVING);
		PrintWindowMessage(WM_POWERBROADCAST);
		PrintWindowMessage(WM_DEVICECHANGE);
		PrintWindowMessage(WM_MDICREATE);
		PrintWindowMessage(WM_MDIDESTROY);
		PrintWindowMessage(WM_MDIACTIVATE);
		PrintWindowMessage(WM_MDIRESTORE);
		PrintWindowMessage(WM_MDINEXT);
		PrintWindowMessage(WM_MDIMAXIMIZE);
		PrintWindowMessage(WM_MDITILE);
		PrintWindowMessage(WM_MDICASCADE);
		PrintWindowMessage(WM_MDIICONARRANGE);
		PrintWindowMessage(WM_MDIGETACTIVE);
		PrintWindowMessage(WM_MDISETMENU);
		PrintWindowMessage(WM_ENTERSIZEMOVE);
		PrintWindowMessage(WM_EXITSIZEMOVE);
		PrintWindowMessage(WM_DROPFILES);
		PrintWindowMessage(WM_MDIREFRESHMENU);
		PrintWindowMessage(WM_IME_SETCONTEXT);
		PrintWindowMessage(WM_IME_NOTIFY);
		PrintWindowMessage(WM_IME_CONTROL);
		PrintWindowMessage(WM_IME_COMPOSITIONFULL);
		PrintWindowMessage(WM_IME_SELECT);
		PrintWindowMessage(WM_IME_CHAR);
		PrintWindowMessage(WM_IME_REQUEST);
		PrintWindowMessage(WM_IME_KEYDOWN);
		PrintWindowMessage(WM_IME_KEYUP);
		PrintWindowMessage(WM_NCMOUSEHOVER);
		PrintWindowMessage(WM_MOUSEHOVER);
		PrintWindowMessage(WM_NCMOUSELEAVE);
		PrintWindowMessage(WM_MOUSELEAVE);
		PrintWindowMessage(WM_CUT);
		PrintWindowMessage(WM_COPY);
		PrintWindowMessage(WM_PASTE);
		PrintWindowMessage(WM_CLEAR);
		PrintWindowMessage(WM_UNDO);
		PrintWindowMessage(WM_RENDERFORMAT);
		PrintWindowMessage(WM_RENDERALLFORMATS);
		PrintWindowMessage(WM_DESTROYCLIPBOARD);
		PrintWindowMessage(WM_DRAWCLIPBOARD);
		PrintWindowMessage(WM_PAINTCLIPBOARD);
		PrintWindowMessage(WM_VSCROLLCLIPBOARD);
		PrintWindowMessage(WM_SIZECLIPBOARD);
		PrintWindowMessage(WM_ASKCBFORMATNAME);
		PrintWindowMessage(WM_CHANGECBCHAIN);
		PrintWindowMessage(WM_HSCROLLCLIPBOARD);
		PrintWindowMessage(WM_QUERYNEWPALETTE);
		PrintWindowMessage(WM_PALETTEISCHANGING);
		PrintWindowMessage(WM_PALETTECHANGED);
		PrintWindowMessage(WM_HOTKEY);
		PrintWindowMessage(WM_PRINT);
		PrintWindowMessage(WM_PRINTCLIENT);
		PrintWindowMessage(WM_APPCOMMAND);
		PrintWindowMessage(WM_HANDHELDFIRST);
		PrintWindowMessage(WM_HANDHELDLAST);
		PrintWindowMessage(WM_AFXFIRST);
		PrintWindowMessage(WM_AFXLAST);
		PrintWindowMessage(WM_PENWINFIRST);
		PrintWindowMessage(WM_PENWINLAST);
		default: {
			printf("WindowMessage: %X\n", lp_msg);
			break;
		}
	}
}