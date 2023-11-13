/*
Window Shortcut

Global Variable Naming Rule:
main.cpp - m_#Var
menu.cpp - me_#Var
control.cpp - c_#Var
util.cpp - u_#Var
log.cpp - l_#Var
process.cpp - p_#Var
*/

#pragma once

//Definition
#define UNICODE
#define WINVER 0x0606
#undef __STRICT_ANSI__
#define _RESTRICT_WM

//Include Standard
#include <stdio.h>
#include <wchar.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

//Include Custom
#include "main.hpp"
#include "control.hpp"
#include "menu.hpp"
#include "log.hpp"
#include "util.hpp"
#include "process.hpp"
#include "vk.hpp"
#include "resource.h"

#ifdef _DEBUG
#include "debug.hpp" //Console Print Utilities
#endif

//Macro

#define _repeat(v, i) for (unsigned int v = 0; v < i; v++)

//Variable Definition

#define WM_ICONNOTIFY 0x8001
#define WM_HOTKEYCHANGE 0x8002
#define FUNC_ADD 0
#define FUNC_MODIFY 1