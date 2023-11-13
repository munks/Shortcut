#include "global.hpp"

//Variable

HWND li_window;

//Internal

void List_AddText (HWND list, LPCWSTR message) {
	int lastPos = Edit_GetTextLength(list);
	
	Edit_SetSel(list, lastPos, lastPos);
	Edit_ReplaceSel(list, message);
}

void List_URLProtocol (HWND hwnd) {
	int idx = 0;
	wchar_t key[MAX_PATH];
	
	while (RegEnumKey(HKEY_CLASSES_ROOT, idx++, key, MAX_PATH) == ERROR_SUCCESS) {
		if (RegGetValue(HKEY_CLASSES_ROOT, key, L"URL Protocol", RRF_RT_ANY, NULL, NULL, NULL) == ERROR_SUCCESS) {
			List_AddText(hwnd, key);
			List_AddText(hwnd, L"\r\n");
		}
	}
}

LRESULT CALLBACK ListProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	WindowEventCase(uMsg) {
		WindowEvent(WM_SYSCOMMAND) {
			if (wParam == SC_CLOSE) {
				ShowWindow(hwnd, SW_HIDE);
				return 0;
			}
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//External

void List_CreateWindow (HWND main) {
	//Create Window Class (List)
	WNDCLASSEX wc = {};
	HWND tmp;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = ListProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = WINDOW_LIST_NAME;
	wc.hbrBackground = m_hbrush;
	wc.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(ID_ICON));

	Util_CheckError((void*)MAKELONG(RegisterClassEx(&wc), 0));
	
	//Create List Window
	li_window = CreateWindowEx(WS_EX_TOPMOST, WINDOW_LIST_NAME, WINDOW_LIST_CAPTION,
							  WS_POPUP | WS_CAPTION | WS_SYSMENU,
							  CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
							  main, NULL, m_hInstance, NULL);
	Util_CheckError(li_window);
	
	//Create List Message Box
	tmp = CreateWindow(L"EDIT", NULL,
							WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL |
							ES_MULTILINE | ES_READONLY,
							10,10,370,250,
							li_window, (HMENU)ID_EDIT_LIST, m_hInstance, NULL);
	SetWindowFont(tmp, m_font, FALSE);
	Util_CheckError(tmp);
	
	List_URLProtocol(tmp);
}