#pragma once

#include "resource.h"

//後で清書

#include "stdafx.h"



//自作＿関数
//HWND CreateToolTip(HTREEITEM lpitem, HWND hWnd, PTSTR pszText);


int InitTreeViewImageLists(HWND hwndTV) {
	return TRUE;
}

int InitTreeViewItems(HWND hWnd) {
	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(tvis));
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT;

	//HTREEITEM AAA;
	tvis.item.pszText = (LPWSTR)L"ＨＴＴＰ接続";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);

	//AAA = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	//CreateToolTip(AAA, hWnd, (LPWSTR)L"AAAあああ");

	tvis.item.pszText = (LPWSTR) L"ｒｅｃｖ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR) L"ＷＳＡｒｅｃｖ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR) L"ＷＳＡＡｃｃｅｐｔ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR) L"ＷＳＡＣｏｎｎｅｃｔ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR) L"ＢＩＮＤ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR) L"ＷＳＡＳｅｎｄＴｏ関数";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR)L"アイテム８";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
	tvis.item.pszText = (LPWSTR)L"アイテム９";
	SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);

	return TRUE;
}



//　自作＿初期

//　自作＿関数

// Create a tree-view control. 
// Returns the handle to the new control if successful,
// or NULL otherwise. 
// hwndParent - handle to the control's parent window. 
// lpszFileName - name of the file to parse for tree-view items.
// g_hInst - the global instance handle.
// ID_TREEVIEW - the resource ID of the control.

HWND CreateATreeView(HWND hwndParent)
{
	RECT rcClient;  // dimensions of client area 
	HWND hwndTV;    // handle to tree-view control
//	HINSTANCE g_hInst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);
	HINSTANCE g_hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

#define ID_TREEVIEW 0001

	// Ensure that the common control DLL is loaded. 
	InitCommonControls();

	// Get the dimensions of the parent window's client area, and create 
	// the tree-view control. 
	GetClientRect(hwndParent, &rcClient);
	hwndTV = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | WS_THICKFRAME | TVS_LINESATROOT | TVS_SINGLEEXPAND | TVS_HASBUTTONS | TVS_INFOTIP,
		0,
		0,
		//rcClient.right,
		200,
		rcClient.bottom,
		hwndParent,
		(HMENU)ID_TREEVIEW,
		g_hInst,
		NULL);

	// Initialize the image list, and add items to the control. 
	// InitTreeViewImageLists and InitTreeViewItems are application- 
	// defined functions, shown later. 
	if (!InitTreeViewImageLists(hwndTV) ||
		!InitTreeViewItems(hwndTV))
	{
		DestroyWindow(hwndTV);
		return FALSE;
	}
	return hwndTV;
}

/* Define an example conditional function that depends on the pQos field */
int CALLBACK ConditionAcceptFunc(
	LPWSABUF lpCallerId,
	LPWSABUF lpCallerData,
	LPQOS pQos,
	LPQOS lpGQOS,
	LPWSABUF lpCalleeId,
	LPWSABUF lpCalleeData,
	GROUP FAR * g,
	DWORD_PTR dwCallbackData
)
{
	/*
	if (pQos != NULL) {
		RtlZeroMemory(pQos, sizeof(QOS));
		return CF_ACCEPT;
	}
	else
		return CF_REJECT;
		*/
	return 1;
}


//struct hWnd_strBuffer_struct
//{
//	HWND hWnd;
//};


DWORD WINAPI ThreadFunc(LPVOID arg);
/*
{
		TCHAR strBuffer[1024];
		hWnd_strBuffer_struct * AAA = (hWnd_strBuffer_struct *)arg;
		wsprintf(strBuffer, L"ThreadFunc \n");
		//MessageBox(AAA->hWnd, strBuffer, L"1", MB_OK);
		//printf("ThreadFunc %d\n", i);
		Sleep(5000);

	return 0;
}

*/


/*
// Description:
//   Creates a tooltip for an item in a dialog box. 
// Parameters:
//   idTool - identifier of an dialog box item.
//   nDlg - window handle of the dialog box.
//   pszText - string to use as the tooltip text.
// Returns:
//   The handle to the tooltip.
//
HWND CreateToolTip(HTREEITEM lpitem, HWND hWnd, PTSTR pszText)
{
	if (!lpitem || !hWnd || !pszText)
	{
		return FALSE;
	}
	// Get the window of the tool.

	HINSTANCE g_hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hWnd, NULL,
		g_hInst, NULL);

	if (!hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hWnd;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)lpitem;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	return hwndTip;
}
*/