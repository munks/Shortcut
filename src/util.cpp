#include "global.hpp"

//Variable

HICON* u_hicon;
int u_hiconCnt;
wchar_t** u_hotkey;
int u_hotkeyCnt;
bool u_hotkeyUse[8][256];

//Internal

bool CheckErrorFunc (void* checkVar, LPCSTR file, int line, LPCSTR targetValName) {
	char errmsg[150];
	
	if (checkVar == NULL) {
		sprintf(errmsg, "Window Processing Error\nOn File - %s, In Line - %d\nVariable Name - %s, Error Code - %d", file, line, targetValName, GetLastError());
		MessageBoxA(NULL, errmsg, "Error", MB_OK | MB_ICONERROR);
		Main_Close();
	}
	
	return true;
}

//External

void Util_Init () {
	u_hicon = (HICON*)malloc(100 * sizeof(HICON));
	u_hiconCnt = 0;
	u_hotkey = (wchar_t**)malloc(100 * sizeof(wchar_t*));
	u_hotkeyCnt = 0;
	ZeroMemory(u_hotkeyUse, sizeof(u_hotkeyUse));
}

void Util_DataRelease () {
	for (int i = 0; i < u_hiconCnt; i++) {
		DestroyIcon(u_hicon[i]);
	}
	u_hiconCnt = 0;
	
	for (int i = 0; i < u_hotkeyCnt; i++) {
		UnregisterHotKey(m_main, i);
		free(u_hotkey[i]);
	}
	u_hotkeyCnt = 0;
	ZeroMemory(u_hotkeyUse, sizeof(u_hotkeyUse));
}

void Util_PrintWindowsLastError () {
	LPWSTR msg;
	
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPWSTR)&msg, 0, NULL);
	
	MessageBox(NULL, msg, L"GetLastError", MB_OK);
	
	LocalFree(msg);
}

void Util_PrintInt (int i) {
	char txt[260];
	
	sprintf(txt, "%d", i);
	MessageBoxA(NULL, txt, "Num.", MB_OK);
}

void Util_PrintString (const wchar_t* str) {
	MessageBox(NULL, str, L"Str.", MB_OK);
}

int Util_GetIconInShortcut (LPCWSTR key, LPCWSTR type, HICON* icon) {
	wchar_t link[260];
	wchar_t id[260];
	wchar_t value[260];
	wchar_t defkey[260];
	DWORD len;
	SHSTOCKICONINFO sii = {};
	
	if (RegGetValue(m_regset, key, L"Link", RRF_RT_REG_SZ, NULL, &link, &(len = 260)) != ERROR_SUCCESS) {
		return 1;
	}
	
	if (wcscmp(type, L"exe") == 0) {
		u_hicon[u_hiconCnt] = ExtractIcon(m_hInstance, link, 0);
		*icon = u_hicon[u_hiconCnt];
		u_hiconCnt++;
	} else if (wcscmp(type, L"directory") == 0) {
		sii.cbSize = sizeof(SHSTOCKICONINFO);
		SHGetStockIconInfo(SIID_FOLDER, SHGSI_ICON, &sii);
		*icon = sii.hIcon;
	} else {
		swprintf(defkey, L"%ls\\UserChoice", type);
		if (RegGetValue(m_regdef, defkey, L"ProgId", RRF_RT_REG_SZ, NULL, &id, &(len = 260)) == ERROR_SUCCESS) {
			puts("Default Program");
			swprintf(value, L"%ls\\shell\\open\\command", id);
		} else {
			swprintf(value, L"%ls\\shell\\open\\command", type);
		}
		if (RegGetValue(HKEY_CLASSES_ROOT, value, NULL, RRF_RT_REG_SZ, NULL, &id, &(len = 260)) != ERROR_SUCCESS) {
			*icon = LoadIcon(NULL, IDI_APPLICATION);
			return 0;
		}
		if (id[0] == L'"') {
			wcscpy(id, &id[1]);
			*wcschr(id, L'"') = L'\0';
		} else {
			*wcschr(id, L' ') = L'\0';
		}
		u_hicon[u_hiconCnt] = ExtractIcon(m_hInstance, id, 0);
		*icon = u_hicon[u_hiconCnt];
		u_hiconCnt++;
	}
	
	return 0;
}

void Util_AddHotkey (DWORD hotkey, LPCWSTR key) {
	WORD data = LOWORD(hotkey);
	BYTE vk = LOBYTE(data);
	BYTE ak = HIBYTE(data) & 0x7;
	UINT flag = MOD_NOREPEAT;
	
	if (!hotkey) { return; }
	u_hotkey[u_hotkeyCnt] = (wchar_t*)malloc((wcslen(key) + 1) * sizeof(wchar_t));
	wcscpy(u_hotkey[u_hotkeyCnt], key);
	u_hotkeyUse[ak][vk] = true;
	if (ak & HOTKEYF_ALT) { flag |= MOD_ALT; }
	if (ak & HOTKEYF_CONTROL) { flag |= MOD_CONTROL; }
	if (ak & HOTKEYF_SHIFT) { flag |= MOD_SHIFT; }
	RegisterHotKey(m_main, u_hotkeyCnt, flag, vk);
	u_hotkeyCnt++;
}

bool Util_HotkeyCheck (DWORD hotkey) {
	WORD data = LOWORD(hotkey);
	BYTE vk = LOBYTE(data);
	BYTE ak = HIBYTE(data) & 0x7;
	
	if (vk == 0) { return true; }
	
	return u_hotkeyUse[ak][vk];
}

wchar_t* Util_GetHotkeyReg (DWORD hotkey) {
	return u_hotkey[hotkey];
}

void Util_RemoveSetting (LPCWSTR key) {
	#ifdef _DEBUG
	Util_PrintWindowsLastError();
	#endif
	RegDeleteTree(m_regset, key);
	RegDeleteKey(m_regset, key);
	Log_Message(LOG_FORMAT_NORMAL, LOG_EXECUTE_DELETE, key, NULL);
}