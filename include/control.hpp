#pragma once

#define LV_MAX_COL 3

//Typedef Function
typedef HIMAGELIST (*ILC)(int, int, UINT, int, int);
typedef int (*ILRI)(HIMAGELIST, int, HICON);
typedef BOOL (*ILD)(HIMAGELIST);

//List-View Header Information
typedef struct LVINFO {
	wchar_t text[20];
	double ratio;
} LVINFO;

void Control_InitDLL ();

HWND Control_CreateButton (HWND, LPCWSTR, LPCWSTR, bool, int, int, int, int, int);
HWND Control_CreateEdit (HWND, LPCWSTR, int, int, int, int, int, LPCWSTR);
void Control_CreateListView (HWND, LPCWSTR, int, int, int, int, int);
void Control_RefreshListView (HWND);
DWORD Control_InitInputDialog (HWND, int, LPCWSTR);

extern HMODULE c_comctlModule;
extern HWND c_listView;
extern int c_listViewIndex;