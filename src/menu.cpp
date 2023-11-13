#include "global.hpp"

//Variable

HMENU me_menu;
GUID me_guid = {0xEF0D4B8F, 0xE581, 0x49D5, {0x99, 0x45, 0x28, 0xBF, 0x1F, 0xF5, 0x82, 0x85}};
MENUITEMINFO me_mi = {sizeof(MENUITEMINFO), 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0};
NOTIFYICONDATA me_nid = {sizeof(NOTIFYICONDATA), NULL, 0, 0, WM_ICONNOTIFY, NULL, WINDOW_MAIN_CAPTION,
						0, 0, {0}, 0, {0}, 0, me_guid, NULL};
int me_infoCnt = 0;
UINT_PTR me_timer = 0;

//Internal

void SetStartup (BOOL add) {
	HKEY tmpkey;
	wchar_t path[MAX_PATH];
	wchar_t regval[MAX_PATH];
	
	GetModuleFileName(NULL, path, MAX_PATH);
	swprintf(regval, L"\"%s\" -hide", path);
	
	RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &tmpkey, NULL);
	if (!add) {
		RegDeleteValue(tmpkey, L"Shortcut");
	} else {
		RegSetValueEx(tmpkey, L"Shortcut", 0, REG_SZ, (BYTE*)regval, (wcslen(regval) + 1) * 2);
	}
	RegCloseKey(tmpkey);
}

void Menu_DeleteNotifyIcon (HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	//Remove Info
	ZeroMemory(me_nid.szInfo, sizeof(me_nid.szInfo));
	Shell_NotifyIcon(NIM_MODIFY, &me_nid);
	
	KillTimer(NULL, me_timer);
	me_timer = 0;
}

//External

void Menu_SetMenuState (WORD message, BOOL on) {
	me_mi.fMask = MIIM_STATE;
	GetMenuItemInfo(me_menu, MAKELONG(ID_BUTTON_ICON, message), false, &me_mi);
	if (on) {
		me_mi.fState |= MFS_CHECKED;
	} else {
		me_mi.fState &= ~MFS_CHECKED;
	}
	SetMenuItemInfo(me_menu, MAKELONG(ID_BUTTON_ICON, message), false, &me_mi);
}

void Menu_SetMenuString (WORD message, LPWSTR str) {
	me_mi.fMask = MIIM_STRING;
	me_mi.dwTypeData = str;
	SetMenuItemInfo(me_menu, MAKELONG(ID_BUTTON_ICON, message), false, &me_mi);
}

void Menu_ExecuteNotifyEvent (WORD message) {
	RECT rect;
	BOOL changed;
	
	switch (message) {
		case TN_MENU_CLOSE: {
			Main_Close();
			return;
		}
		case TN_MENU_LOG: {
			if(!IsWindowVisible(l_window)) {
				//Set Log Window Position
				GetWindowRect(m_main, &rect);
				SetWindowPos(l_window, HWND_TOPMOST, rect.left + 20, rect.top + 20, 0, 0, SWP_NOSIZE);
			}
			//Show Log Window
			ShowWindow(l_window, SW_SHOW);
			break;
		}
		case TN_MENU_INIT: {
			me_mi.fMask = MIIM_STATE;
			GetMenuItemInfo(me_menu, MAKELONG(ID_BUTTON_ICON, message), false, &me_mi);
			me_mi.fState ^= MFS_CHECKED;
			SetMenuItemInfo(me_menu, MAKELONG(ID_BUTTON_ICON, message), false, &me_mi);
			changed = ((me_mi.fState & MFS_CHECKED) == MFS_CHECKED) ? BST_CHECKED : BST_UNCHECKED;
			SetStartup(changed);
			break;
		}
	}
}

void Menu_AddNotifyIcon (HWND main) {
	me_nid.hWnd = main;
	me_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_GUID;
	me_nid.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(ID_ICON));
	
	Shell_NotifyIcon(NIM_ADD, &me_nid);
}

void Menu_RemoveNotifyIcon () {
	me_nid.uFlags = NIF_GUID;
	
	Shell_NotifyIcon(NIM_DELETE, &me_nid);
}

void Menu_InfoNotifyIcon (LPCWSTR title, LPCWSTR info, int len) {
	//Log
	Log_Message(LOG_FORMAT_NORMAL, title, info, NULL);
	
	//Init
	me_infoCnt++;
	me_nid.uFlags = NIF_INFO | NIF_GUID;
	me_nid.dwInfoFlags = NIIF_INFO | NIIF_NOSOUND;
	
	//Add Info
	wcscpy(me_nid.szInfoTitle, title);
	wcscpy(me_nid.szInfo, info);
	
	Shell_NotifyIcon(NIM_MODIFY, &me_nid);
	
	//Wait
	if (me_timer) {
		KillTimer(NULL, me_timer);
	}
	me_timer = SetTimer(NULL, NOTIFY_DELETE, len, Menu_DeleteNotifyIcon);
}

void Menu_MakeMenu () {
	me_menu = CreatePopupMenu();
	
	AppendMenu(me_menu, MF_STRING | MF_UNCHECKED, MAKELONG(ID_BUTTON_ICON, TN_MENU_INIT), MENU_START_TEXT);
	AppendMenu(me_menu, MF_SEPARATOR, 0, NULL);
	AppendMenu(me_menu, MF_STRING | MF_UNCHECKED, MAKELONG(ID_BUTTON_ICON, TN_MENU_LOG), MENU_LOG_TEXT);
	AppendMenu(me_menu, MF_SEPARATOR, 0, NULL);
	AppendMenu(me_menu, MF_STRING | MF_UNCHECKED, MAKELONG(ID_BUTTON_ICON, TN_MENU_CLOSE), MENU_CLOSE_TEXT);
}