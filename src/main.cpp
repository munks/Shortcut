#include "global.hpp"

//Variable

HWND m_main;
HBRUSH m_hbrush;
HINSTANCE m_hInstance;
HFONT m_font;
HKEY m_regkey;
HKEY m_regset;
HKEY m_regdef;
BOOL m_hkDisable = FALSE;

//External

void Main_Close () {
	DestroyMenu(me_menu);
	DeleteObject(m_font);
	RegCloseKey(m_regkey);
	RegCloseKey(m_regset);
	RegCloseKey(m_regdef);
	Util_DataRelease();
	Menu_RemoveNotifyIcon();
	FreeLibrary(c_comctlModule);
	PostQuitMessage(0);
}

//Internal
#define CreateButtonMacro(h, id, cb, x, y, cx, cy) Control_CreateButton(h, BUTTON_##id##_CAPTION, BUTTON_##id##_TOOLTIP, cb, x, y, cx, cy, ID_BUTTON_##id)

void Main_VersionCheck (DWORD ver) {
	DWORD reg, size;
	
	RegGetValue(m_regkey, NULL, L"Version", RRF_RT_REG_DWORD, NULL, &(reg = 0), &(size = sizeof(DWORD)));
	
	switch (reg) {
		case 0: 
		case 1:
		case 2:
		case 3: break;
	}
	
	RegSetValueEx(m_regkey, L"Version", 0, REG_DWORD, (BYTE*)&ver, sizeof(DWORD));
}

LRESULT CALLBACK MainProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	wchar_t key[50];
	POINT cursor;
	RECT rect;
	
	#ifdef _DEBUG
	//Debug_ConvertWindowMessage(uMsg);
	#endif
	
	WindowEventCase(uMsg) {
		WindowEvent(WM_NOTIFY) {
			NotifyEventCase(ListViewMessage()) {
				NotifyEvent(HDN_BEGINTRACKW)
				NotifyEvent(HDN_ITEMCHANGINGW) {
					//Prevent List-View Header Control
					return true;
				}
				NotifyEvent(LVN_ITEMCHANGED) {
					//Set Selected Specific Value
					c_listViewIndex = ListView_GetNextItem(ListViewDialog(), -1, LVNI_FOCUSED | LVNI_SELECTED);
				}
			}
			break;
		}
		WindowEvent(WM_SYSCOMMAND) {
			if (wParam != SC_RESTORE) {
				break;
			}
		}
		WindowEvent(WM_COMMAND) {
			DialogEventCase(EventDialog()) {
				DialogEvent(ID_BUTTON_RUN)
				DialogEvent(ID_BUTTON_MODIFY)
				DialogEvent(ID_BUTTON_DELETE) {
					if (c_listViewIndex == -1) { break; }
					ListView_GetItemText(GetDlgItem(hwnd, ID_LIST), c_listViewIndex, 0, key, 50);
				}
				DialogEvent(ID_BUTTON_ADD) {
					if (EventMessage() == BN_CLICKED) {
						//Change Window Property
						void (*executionFunc)(LPWSTR) = NULL;
						
						
						switch (EventDialog()) {
							case ID_BUTTON_RUN:
								executionFunc = Process_RunShortcut; break;
							case ID_BUTTON_MODIFY:
								executionFunc = Process_ModifyShortcut; break;
							case ID_BUTTON_ADD:
								executionFunc = Process_AddShortcut; break;
							case ID_BUTTON_DELETE:
								executionFunc = Process_DeleteShortcut; break;
						}
						
						if (executionFunc == NULL) { break; }
						//Selected Process Execution
						executionFunc(key);
						Control_RefreshListView(hwnd);
					}
					break;
				}
				DialogEvent(ID_BUTTON_LIST) {
					if(!IsWindowVisible(li_window)) {
						//Set List Window Position
						GetWindowRect(m_main, &rect);
						SetWindowPos(li_window, HWND_TOPMOST, rect.left + 20, rect.top + 20, 0, 0, SWP_NOSIZE);
					}
					//Show List Window
					ShowWindow(li_window, SW_SHOW);
					break;
				}
				DialogEvent(ID_BUTTON_ICON) {
					Menu_ExecuteNotifyEvent(EventMessage());
					break;
				}
			}
			return 0;
		}
		//System Tray Notify Icon
		WindowEvent(WM_ICONNOTIFY) {
			if (lParam == WM_LBUTTONUP) {
				ShowWindow(m_main, SW_RESTORE);
			}
			if (lParam == WM_RBUTTONUP) {
				GetCursorPos(&cursor);
				SetForegroundWindow(hwnd);
				TrackPopupMenu(me_menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, cursor.x, cursor.y, 0, hwnd, NULL);
			}
			break;
		}
		//Prevent Sizing
		WindowEvent(WM_SIZING)
		WindowEvent(WM_SIZE)
		WindowEvent(WM_NCLBUTTONDBLCLK) {
			return true;
		}
		//Hide Window Instead Of Closing
		WindowEvent(WM_CLOSE) {
			AnimateWindow(hwnd, 200, AW_HIDE | AW_BLEND);
			return 0;
		}
		WindowEvent(WM_HOTKEY) {
			if (!m_hkDisable) {
				Process_RunShortcut(Util_GetHotkeyReg(wParam));
			} else {
				SendMessage(p_ctrl, WM_HOTKEYCHANGE, 0, lParam);
			}
			break;
		}
		//Not Used
		WindowEvent(WM_DESTROY) {
			Menu_RemoveNotifyIcon();
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	BYTE regval;
	DWORD size;
	WNDCLASSEX wc = {};
	
	//Duplicate Prevent
	if (FindWindow(WINDOW_MAIN_NAME, NULL) != NULL) {
		ShowWindow(FindWindow(WINDOW_MAIN_NAME, NULL), SW_RESTORE);
		SetForegroundWindow(FindWindow(WINDOW_MAIN_NAME, NULL));
		return 0;
	}
	
	#ifdef _DEBUG
	#if LANG == KOKR
	setlocale(LC_ALL, "ko-KR");
	#endif
	#endif
	
	//Open Registry
	RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Duality\\Shortcut", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &m_regkey, NULL);
	RegCreateKeyEx(m_regkey, L"Settings", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &m_regset, NULL);
	RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations", 0 , NULL, 0, KEY_ALL_ACCESS, NULL, &m_regdef, NULL);
	
	Main_VersionCheck(0);
	
	//Set Global Variables
	m_hInstance = hInstance;
	m_hbrush = CreateSolidBrush(RGB(240,240,240));
	m_font = CreateFont(16,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
						VARIABLE_PITCH | FF_ROMAN, L"Ebrima");
	
	//Control Init
	Control_InitDLL();
	Util_Init();
	
	//Create Window Class (Main)
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MainProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_MAIN_NAME;
	wc.hbrBackground = m_hbrush;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ID_ICON));
	
	Util_CheckError((void*)(MAKELONG(RegisterClassEx(&wc), 0)));
	
	//Create Main Window
	m_main = CreateWindowEx(WS_EX_TOPMOST, WINDOW_MAIN_NAME, WINDOW_MAIN_CAPTION,
							WS_OVERLAPPED | WS_CAPTION | WS_POPUPWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 485, 405,
							NULL, NULL, hInstance, NULL);
	Util_CheckError(m_main);
	
	//Create Log/List Window
	Log_CreateWindow(m_main);
	List_CreateWindow(m_main);
	
	//Create Button (OPEN)
	CreateButtonMacro(m_main, RUN, false, 380, 20, 90, 30);
	
	//Create Button (MODIFY)
	CreateButtonMacro(m_main, MODIFY, false, 380, 60, 90, 30);
	
	//Create Button (ADD)
	CreateButtonMacro(m_main, ADD, false, 380, 100, 90, 30);
	
	//Create Button (DELETE)
	CreateButtonMacro(m_main, DELETE, false, 380, 140, 90, 30);
	
	//Create Button (LIST)
	CreateButtonMacro(m_main, LIST, false, 380, 330, 90, 30);
	
	//Create List-View
	Control_CreateListView(m_main, LIST_TOOLTIP, 10, 10, 360, 350, ID_LIST);
	Control_RefreshListView(m_main);
	
	//Add System Tray Notify Icon
	Menu_AddNotifyIcon();
	Menu_MakeMenu();
	
	//Get Registry (INIT)
	Menu_SetMenuState(TN_MENU_INIT, !RegGetValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Shortcut", RRF_RT_REG_SZ, NULL, NULL, NULL));
	
	//Show Window (Main)
	UpdateWindow(m_main);
	if (strcmp(pCmdLine, "-hide")) {
		ShowWindow(m_main, SW_SHOW);
	}
	
	//Message Loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}