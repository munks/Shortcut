#include "global.hpp"

//Variable

HMODULE c_comctlModule;
ILD IL_Destroy;
ILC IL_Create;
ILRI IL_ReplaceIcon;

HIMAGELIST c_imageList;

HWND c_listView;
int c_listViewIndex;

LVINFO c_listViewInfo[LV_MAX_COL] = {
	{L"Shortcut Name", 0.6},
	{L"Type", 0.18},
	{L"Hotkey", 0.18}
};

//Internal

void CreateTooltip (HWND hwnd, HWND hitem, LPCWSTR tooltip) {
	HWND tmphwnd;
	wchar_t txt[MAX_PATH];
	TOOLINFO ti = { 0 };
	
	tmphwnd = CreateWindow(TOOLTIPS_CLASS, NULL,
							WS_POPUP | TTS_ALWAYSTIP,
							CW_USEDEFAULT, CW_USEDEFAULT,
							CW_USEDEFAULT, CW_USEDEFAULT,
							hwnd, NULL, 
							m_hInstance, NULL);
	Util_CheckError(tmphwnd);
	
	wcscpy(txt, tooltip);
	
	ti.cbSize = sizeof(ti) - sizeof(void*);
	ti.hwnd = hwnd;
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.uId = (UINT_PTR)hitem;
	ti.lpszText = txt;
	
	SendMessage(tmphwnd, TTM_ADDTOOL, 0, (LPARAM)&ti);
}

void Control_GetHotkeyString (LPWSTR out, DWORD hotkey) {
	WORD data = LOWORD(hotkey);
	BYTE vk = LOBYTE(data);
	BYTE ak = HIBYTE(data);
	
	if (!hotkey) {
		wcscpy(out, VK_NONE);
		return;
	}
	out[0] = L'\0';
	if (ak & HOTKEYF_CONTROL) { wcscat(out, L"C"); }
	if (ak & HOTKEYF_SHIFT) { wcscat(out, L"S"); }
	if (ak & HOTKEYF_ALT) { wcscat(out, L"A"); }
	if (ak) { wcscat(out, L" + "); }
	wcscat(out, VirtualKeyCodeText(vk));
}

//External

void Control_InitDLL () {
	c_comctlModule = LoadLibraryA("comctl32.dll");
	
	IL_Destroy = (ILD)GetProcAddress(c_comctlModule, "ImageList_Destroy");
	IL_Create = (ILC)GetProcAddress(c_comctlModule, "ImageList_Create");
	IL_ReplaceIcon = (ILRI)GetProcAddress(c_comctlModule, "ImageList_ReplaceIcon");
}

HWND Control_CreateButton (HWND hwnd, LPCWSTR caption, LPCWSTR tooltip, bool checkbox, int x, int y, int w, int h, int idtf) {
	HWND tmphwnd;
	
	tmphwnd = CreateWindow(L"BUTTON", caption,
							WS_BORDER | WS_CHILD | WS_VISIBLE | WS_TABSTOP |
							BS_CENTER | BS_VCENTER | checkbox * BS_AUTOCHECKBOX,
							x, y, w, h,
							hwnd, (HMENU)idtf, m_hInstance, NULL);
	Util_CheckError(tmphwnd);
	
	#ifdef _DEBUG
	wprintf(L"CreateButton: %ls\n", caption);
	#endif
	
	if (wcslen(tooltip) != 0) {
		CreateTooltip(hwnd, tmphwnd, tooltip);
	}
	
	SetWindowFont(tmphwnd, m_font, FALSE);
	
	return tmphwnd;
}

HWND Control_CreateEdit (HWND hwnd, LPCWSTR tooltip, int x, int y, int w, int h, int idtf, LPCWSTR txt) {
	HWND tmphwnd;
	
	tmphwnd = CreateWindow(L"EDIT", NULL,
							WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
							x, y, w, h,
							hwnd, (HMENU)idtf, m_hInstance, NULL);
	Util_CheckError(tmphwnd);
	
	#ifdef _DEBUG
	wprintf(L"CreateEdit: %ls\n", txt);
	#endif
	
	if (wcslen(tooltip) != 0) {
		CreateTooltip(hwnd, tmphwnd, tooltip);
	}
	
	SetWindowFont(tmphwnd, m_font, FALSE);
	Edit_SetText(tmphwnd, txt);
	
	return tmphwnd;
}

void Control_CreateListView (HWND hwnd, LPCWSTR tooltip, int x, int y, int w, int h, int idtf) {
	LVCOLUMNW col = {0, };
	
	//Make Control
	c_listView = CreateWindow(WC_LISTVIEWW, NULL,
							LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS |
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							x, y, w, h,
							hwnd, (HMENU)idtf, m_hInstance, NULL);
	Util_CheckError(c_listView);
	
	#ifdef _DEBUG
	_putws(L"CreateList");
	#endif
	
	if (wcslen(tooltip) != 0) {
		CreateTooltip(hwnd, c_listView, tooltip);
	}
	
	SendMessage(c_listView, CCM_SETVERSION, 5, 0);
	SetWindowFont(c_listView, m_font, FALSE);
	
	//Init List-View Header
	ListView_SetExtendedListViewStyle(c_listView, LVS_EX_FULLROWSELECT);
	col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	
	_repeat (i, LV_MAX_COL) {
		col.cx = c_listViewInfo[i].ratio * w;
		col.pszText = c_listViewInfo[i].text;
		ListView_InsertColumn(c_listView, i, &col);
	}
}

void Control_RefreshListView (HWND hwnd) {
	wchar_t outputText[MAX_PATH];
	LVITEM item;
	HICON icon;
	int idx, regidx;
	wchar_t key[MAX_PATH];
	wchar_t type[260];
	DWORD len, hotkey;
	
	IL_Destroy(c_imageList);
	c_imageList = IL_Create(16, 16, ILC_MASK | ILC_COLOR32, 1, 1);
	idx = ListView_GetTopIndex(c_listView);
	
	//Clear List
	ListView_DeleteAllItems(c_listView);
	Util_DataRelease();
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.iItem = 0;
	item.iImage = 0;
	
	//Loop (Registry)
	regidx = 0;
	while (true) {
		LOOPBEGIN:
		if (RegEnumKey(m_regset, regidx, key, MAX_PATH) != ERROR_SUCCESS) { break; }
		outputText[0] = L'\0';
		_repeat (i, LV_MAX_COL) {
			switch (i) {
				case 0: { //Item 0(Shortcut Name)
					wcscpy(outputText, key);
					break;
				}
				case 1: { //Item 1(Shortcut Type)
					if (RegGetValue(m_regset, key, L"Type", RRF_RT_REG_SZ, NULL, type, &(len = 260)) != ERROR_SUCCESS) {
						Util_RemoveSetting(key);
						ListView_DeleteItem(c_listView, item.iItem);
						goto LOOPBEGIN;
					}
					if (Util_GetIconInShortcut(key, type, &icon)) {
						Util_RemoveSetting(key);
						ListView_DeleteItem(c_listView, item.iItem);
						goto LOOPBEGIN;
					}
					IL_ReplaceIcon(c_imageList, -1, icon ? icon : LoadIcon(NULL, IDI_APPLICATION));
					wcscpy(outputText, type);
					break;
				}
				case 2: { //Item 2(Hotkey)
					if (RegGetValue(m_regset, key, L"Hotkey", RRF_RT_REG_DWORD, NULL, &hotkey, &(len = sizeof(DWORD))) != ERROR_SUCCESS) {
						Util_RemoveSetting(key);
						ListView_DeleteItem(c_listView, item.iItem);
						goto LOOPBEGIN;
					}
					Control_GetHotkeyString(outputText, hotkey);
					Util_AddHotkey(hotkey, key);
					break;
				}
			}
			item.iSubItem = i;
			item.pszText = outputText;
			if (i == 0) {
				ListView_InsertItem(c_listView, &item);
			} else {
				ListView_SetItem(c_listView, &item);
			}
		}

		//Next
		item.iItem++;
		item.iImage++;
		regidx++;
	}
	
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_RUN), item.iItem);
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_MODIFY), item.iItem);
	EnableWindow(GetDlgItem(hwnd, ID_BUTTON_DELETE), item.iItem);
	
	if (c_listViewIndex < 0 || c_listViewIndex > item.iItem) {
		c_listViewIndex = 0;
	}
	
	ListView_SetItemState(c_listView, c_listViewIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	ListView_SetImageList(c_listView, c_imageList, LVSIL_SMALL);
	SetFocus(c_listView);
	ListView_Scroll(c_listView, 0, 17 * idx);
}

DWORD Control_InitInputDialog (HWND dlg, int idtf, LPCWSTR key) {
	wchar_t str[260];
	DWORD len;
	DWORD hotkey = 0;
	wchar_t* type;
	
	switch (idtf) {
		case FUNC_ADD: {
			type = DLG_INPUT_ADD;
			EnableWindow(GetDlgItem(dlg, ID_BUTTON_DLG_OK), FALSE);
			break;
		}
		case FUNC_MODIFY: {
			type = DLG_INPUT_MODIFY;
			Edit_SetText(GetDlgItem(dlg, ID_EDIT_NAME), key);
			RegGetValue(m_regset, key, L"Link", RRF_RT_REG_SZ, NULL, &str, &(len = 260));
			Edit_SetText(GetDlgItem(dlg, ID_EDIT_LINK), str);
			RegGetValue(m_regset, key, L"Param", RRF_RT_REG_SZ, NULL, &str, &(len = 260));
			Edit_SetText(GetDlgItem(dlg, ID_EDIT_PARAM), str);
			RegGetValue(m_regset, key, L"Hotkey", RRF_RT_REG_DWORD, NULL, &hotkey, &(len = sizeof(DWORD)));
			SendDlgItemMessage(dlg, ID_HOTKEY, HKM_SETHOTKEY, hotkey, 0);
			break;
		}
	}
	
	SetWindowText(dlg, type);
	SetWindowText(GetDlgItem(dlg, ID_BUTTON_DLG_OK), type);
	SetWindowText(GetDlgItem(dlg, ID_BUTTON_DLG_CANCEL), DLG_INPUT_CANCEL);
	
	return hotkey;
}