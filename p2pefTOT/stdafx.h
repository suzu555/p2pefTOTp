// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: プログラムに必要な追加ヘッダーをここで参照してください

#include <commctrl.h>
#pragma comment(lib, "httpapi.lib")
#include <http.h>
#include <winhttp.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winhttp.lib")
#include <locale.h>
//#pragma comment(lib, "User32.lib")
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

//関数に使うヘッダー書け
#include <Ws2tcpip.h>
