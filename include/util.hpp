#pragma once

#define Util_CheckError(t) CheckErrorFunc(t, __FILE__, __LINE__, #t)
bool CheckErrorFunc (void*, LPCSTR, int, LPCSTR);
void Util_Init ();
void Util_DataRelease ();
void Util_PrintWindowsLastError ();
void Util_PrintInt (int);
void Util_PrintString (const wchar_t*);
int Util_GetIconInShortcut (LPCWSTR, LPCWSTR, HICON*);
void Util_AddHotkey (DWORD, LPCWSTR);
bool Util_HotkeyCheck (DWORD);
wchar_t* Util_GetHotkeyReg (DWORD);
void Util_RemoveSetting (LPCWSTR);