#include "global.hpp"

//Variable

wchar_t* p_key;
wchar_t p_name[260];
wchar_t p_type[260];
wchar_t p_link[260];
wchar_t p_param[260];
DWORD p_hotkey;
HWND p_ctrl;
int p_currentFunc;
int p_enableFlag;

#define FLAG_NAME (1 << 0)
#define FLAG_TYPE (1 << 1)
#define FLAG_HOTKEY (1 << 2)

//Internal
void Process_CheckKey (HWND hwnd, HWND ctrl) {
	wchar_t txt[260];
	HKEY key;
	
	Edit_GetText(ctrl, txt, 260);
	
	if (p_currentFunc == FUNC_MODIFY) {
		if (wcscmp(txt, p_key) == 0) {
			goto ENABLE;
		}
	}
	if (wcschr(txt, L'\\')) {
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_NAME), L"Name (Invalid name)");
		goto DISABLE;
	}
	if (RegOpenKeyEx(m_regset, txt, 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
		RegCloseKey(key);
		SetWindowText(GetDlgItem(hwnd, ID_STATIC_NAME), L"Name (Already exist)");
		goto DISABLE;
	}
	
	ENABLE:
	p_enableFlag |= FLAG_NAME;
	SetWindowText(GetDlgItem(hwnd, ID_STATIC_NAME), L"Name");
	if (p_enableFlag == (FLAG_TYPE | FLAG_NAME | FLAG_HOTKEY)) {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), TRUE);
	} else {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	}
	return;
	
	DISABLE:
	p_enableFlag &= ~FLAG_NAME;
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	return;
}

void Process_CheckType (HWND hwnd, HWND ctrl) {
	wchar_t txt[260];
	wchar_t protocol[260];
	DWORD type;
	
	Edit_GetText(ctrl, txt, 260);
	if (wcsncmp(&txt[1], L":\\", 2) == 0) {
		type = GetFileAttributes(txt);
		wprintf(L"%ls: %d\n", txt, type);
		if (type == INVALID_FILE_ATTRIBUTES) { goto DISABLE; }
		if (type & FILE_ATTRIBUTE_ARCHIVE) {
			GetBinaryType(txt, &type);
			if (!type) { goto DISABLE; }
			SetWindowText(GetDlgItem(hwnd, ID_STATIC_TYPE), L"(exe)");
			wcscpy(p_type, L"exe");
			goto ENABLE;
		}
		if (type & FILE_ATTRIBUTE_DIRECTORY) {
			SetWindowText(GetDlgItem(hwnd, ID_STATIC_TYPE), L"(directory)");
			wcscpy(p_type, L"directory");
			goto ENABLE;
		}
	} else if (wcsstr(txt, L"://")) {
		*wcsstr(txt, L"://") = L'\0';
		if (RegGetValue(HKEY_CLASSES_ROOT, txt, L"URL Protocol", RRF_RT_ANY, NULL, NULL, NULL) == ERROR_SUCCESS) {
			swprintf(protocol, L"(%ls)", txt);
			SetWindowText(GetDlgItem(hwnd, ID_STATIC_TYPE), protocol);
			wcscpy(p_type, txt);
			goto ENABLE;
		} else {
			goto DISABLE;
		}
	} else {
		goto DISABLE;
	}
	
	ENABLE:
	p_enableFlag |= FLAG_TYPE;
	if (p_enableFlag == (FLAG_TYPE | FLAG_NAME | FLAG_HOTKEY)) {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), TRUE);
	} else {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	}
	return;
	
	DISABLE:
	p_enableFlag &= ~FLAG_TYPE;
	SetWindowText(GetDlgItem(hwnd, ID_STATIC_TYPE), L"");
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	return;
}

void Process_CheckHotkey (HWND hwnd, DWORD hotkey) {
	DWORD val;
	DWORD len;
	
	if (p_currentFunc == FUNC_MODIFY) {
		RegGetValue(m_regset, p_key, L"Hotkey", RRF_RT_REG_DWORD, NULL, &val, &(len = sizeof(DWORD)));
		if (hotkey == val) { goto ENABLE; }
	}
	
	if (hotkey == 0) { goto ENABLE;}
	
	if (!Util_HotkeyCheck(hotkey)) {
		goto ENABLE;
	} else {
		goto DISABLE;
	}
	
	ENABLE:
	p_hotkey = hotkey;
	p_enableFlag |= FLAG_HOTKEY;
	if (p_enableFlag == (FLAG_TYPE | FLAG_NAME | FLAG_HOTKEY)) {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), TRUE);
	} else {
		EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	}
	return;
	
	DISABLE:
	p_enableFlag &= ~FLAG_HOTKEY;
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DLG_OK), FALSE);
	return;
}

LRESULT CALLBACK InputProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	WindowEventCase(uMsg) {
		WindowEvent(WM_INITDIALOG) {
			m_hkDisable = TRUE;
			p_enableFlag = 0;
			p_hotkey = Control_InitInputDialog(hwnd, p_currentFunc, p_key);
			p_ctrl = hwnd;
			Process_CheckKey(hwnd, GetDlgItem(hwnd, ID_EDIT_NAME));
			Process_CheckType(hwnd, GetDlgItem(hwnd, ID_EDIT_LINK));
			Process_CheckHotkey(hwnd, SendDlgItemMessage(hwnd, ID_HOTKEY, HKM_GETHOTKEY, 0, 0));
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		WindowEvent(WM_COMMAND) {
			DialogEventCase(EventDialog()) {
				DialogEvent(ID_BUTTON_DLG_OK) {
					if (EventMessage() == BN_CLICKED) {
						Edit_GetText(GetDlgItem(hwnd, ID_EDIT_NAME), p_name, 260);
						Edit_GetText(GetDlgItem(hwnd, ID_EDIT_LINK), p_link, 260);
						Edit_GetText(GetDlgItem(hwnd, ID_EDIT_PARAM), p_param, 260);
						m_hkDisable = FALSE;
						EndDialog(hwnd, 0);
						return DefWindowProc(hwnd, uMsg, wParam, lParam);
					}
				}
				DialogEvent(ID_BUTTON_DLG_CANCEL) {
					if (EventMessage() == BN_CLICKED) {
						m_hkDisable = FALSE;
						EndDialog(hwnd, 1);
						return DefWindowProc(hwnd, uMsg, wParam, lParam);
					}
				}
				DialogEvent(ID_EDIT_NAME) {
					if (EventMessage() == EN_UPDATE) {
						Process_CheckKey(hwnd, (HWND)lParam);
					}
					break;
				}
				DialogEvent(ID_EDIT_LINK) {
					if (EventMessage() == EN_UPDATE) {
						Process_CheckType(hwnd, (HWND)lParam);
					}
					break;
				}
				DialogEvent(ID_HOTKEY) {
					Process_CheckHotkey(hwnd, SendDlgItemMessage(hwnd, ID_HOTKEY, HKM_GETHOTKEY, 0, 0));
					break;
				}
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		WindowEvent(WM_HOTKEYCHANGE) {
			BYTE data = LOWORD(lParam);
			BYTE flag = 0;
			DWORD result;
			
			if (data & MOD_ALT) { flag |= HOTKEYF_ALT; }
			if (data & MOD_CONTROL) { flag |= HOTKEYF_CONTROL; }
			if (data & MOD_SHIFT) { flag |= HOTKEYF_SHIFT; }
			
			result = MAKELONG(MAKEWORD(HIWORD(lParam), flag), 0);
			SendDlgItemMessage(hwnd, ID_HOTKEY, HKM_SETHOTKEY, result, 0);
			Process_CheckHotkey(hwnd, result);
			
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		WindowEvent(WM_CLOSE) {
			m_hkDisable = FALSE;
			EndDialog(hwnd, 1);
			return 0;
		}
	}
	
	return 0;
}

//External

void Process_RunShortcut (LPWSTR key) {
	DWORD len;
	
	RegGetValue(m_regset, key, L"Link", RRF_RT_REG_SZ, NULL, &p_link, &(len = 260));
	RegGetValue(m_regset, key, L"Type", RRF_RT_REG_SZ, NULL, &p_type, &(len = 260));
	RegGetValue(m_regset, key, L"Param", RRF_RT_REG_SZ, NULL, &p_param, &(len = 260));
	
	if ((wcscmp(p_type, L"exe") == 0) ||
		(wcscmp(p_type, L"directory") == 0)) {
		if (_waccess(p_link, 0)) {
			Menu_InfoNotifyIcon(NOTIFY_RUN_FAILED, NOTIFY_NO_EXIST, 3000);
			return;
		}
	}
	ShellExecute(NULL, L"open", p_link, !wcscmp(p_type, L"exe") ? p_param : NULL, NULL, SW_SHOWNORMAL);
	Log_Message(LOG_FORMAT_NORMAL, LOG_EXECUTE_RUN, key, NULL);
}

void Process_ModifyShortcut (LPWSTR key) {
	HKEY reg;
	
	p_key = key;
	p_currentFunc = FUNC_MODIFY;
	
	if (!DialogBox(m_hInstance, MAKEINTRESOURCE(ID_DLG_INPUT), m_main, InputProc)) {
		RegRenameKey(m_regset, key, p_name);
		if (RegOpenKeyEx(m_regset, p_name, 0, KEY_ALL_ACCESS, &reg) != ERROR_SUCCESS) {
			Menu_InfoNotifyIcon(NOTIFY_REGOPEN_FAILED, NOTIFY_REG_NO_EXIST, 3000);
			return;
		}
		RegSetValueEx(reg, L"Link", 0, REG_SZ, (BYTE*)p_link, (wcslen(p_link) + 1) * 2);
		RegSetValueEx(reg, L"Type", 0, REG_SZ, (BYTE*)p_type, (wcslen(p_type) + 1) * 2);
		RegSetValueEx(reg, L"Param", 0, REG_SZ, (BYTE*)p_param, (wcslen(p_param) + 1) * 2);
		RegSetValueEx(reg, L"Hotkey", 0, REG_DWORD, (BYTE*)&p_hotkey, sizeof(DWORD));
		RegCloseKey(reg);
		Log_Message(LOG_FORMAT_MODIFY, LOG_EXECUTE_MODIFY, key, p_name);
	}
}

void Process_AddShortcut (LPWSTR key) {
	HKEY reg;
	
	p_key = key;
	p_currentFunc = FUNC_ADD;
	
	if (!DialogBox(m_hInstance, MAKEINTRESOURCE(ID_DLG_INPUT), m_main, InputProc)) {
		if (RegCreateKeyEx(m_regset, p_name, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &reg, NULL) != ERROR_SUCCESS) {
			Menu_InfoNotifyIcon(NOTIFY_REGOPEN_FAILED, NOTIFY_REG_NO_CREATE, 3000);
			return;
		}
		RegSetValueEx(reg, L"Link", 0, REG_SZ, (BYTE*)p_link, (wcslen(p_link) + 1) * 2);
		RegSetValueEx(reg, L"Type", 0, REG_SZ, (BYTE*)p_type, (wcslen(p_link) + 1) * 2);
		RegSetValueEx(reg, L"Param", 0, REG_SZ, (BYTE*)p_param, (wcslen(p_param) + 1) * 2);
		RegSetValueEx(reg, L"Hotkey", 0, REG_DWORD, (BYTE*)&p_hotkey, sizeof(DWORD));
		RegCloseKey(reg);
		Log_Message(LOG_FORMAT_NORMAL, LOG_EXECUTE_ADD, key, NULL);
	}
}

void Process_DeleteShortcut (LPWSTR key) {
	Util_RemoveSetting(key);
}