// p2pefTOT.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "p2pefTOT.h"


#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。


    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_P2PEFTOT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P2PEFTOT));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_P2PEFTOT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_P2PEFTOT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hTview;		//親ウインドウの操作で子ウインドウの操作を気楽にするため
	TCHAR strBuffer[1024];
	int chage_state;
	#define WM_SOCKET WM_APP
	LPTOOLTIPTEXT lpttt;    // TOOLTIPTEXT構造体

    switch (message)
    {
	case WM_NOTIFY:
	{
		switch (wParam)
		{
		case 1:
		{
			LPNMHDR lpnmh = (LPNMHDR)lParam;
			switch (lpnmh->code)
			{
			case NM_SETFOCUS:
			{
				HTREEITEM Selected = (HTREEITEM)SendMessage(hTview, TVM_GETNEXTITEM, TVGN_CARET, 0);
				char buf[64];
				TV_ITEM tvi;
				HTREEITEM hitem = Selected;
				tvi.mask = TVIF_TEXT;
				tvi.pszText = (LPWSTR)buf;
				tvi.cchTextMax = MAX_PATH;
				tvi.hItem = hitem;
				
				int r = (int)SendMessage(hTview, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi);
				chage_state = wcscmp(tvi.pszText, L"ＨＴＴＰ接続");
				if(chage_state == 0){
					MessageBox(hWnd, L"本文", L"タイトル", MB_OK);

					DWORD dwSize = 0;
					DWORD dwDownloaded = 0;
					LPSTR pszOutBuffer;
					BOOL  bResults = FALSE;
					HINTERNET  hSession = NULL,
						hConnect = NULL,
						hRequest = NULL;

					// Use WinHttpOpen to obtain a session handle.
					hSession = WinHttpOpen(L"WinHTTP Example/555.0",
						WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
						WINHTTP_NO_PROXY_NAME,
						WINHTTP_NO_PROXY_BYPASS, 0);

					// Specify an HTTP server.
					if (hSession)
						hConnect = WinHttpConnect(hSession, L"127.0.0.1",
							INTERNET_DEFAULT_HTTPS_PORT, 0);
							//INTERNET_DEFAULT_HTTPS_PORT, 0);

					// Create an HTTP request handle.
					if (hConnect)
						hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL,
							NULL, WINHTTP_NO_REFERER,
							WINHTTP_DEFAULT_ACCEPT_TYPES,
							WINHTTP_FLAG_SECURE);

					// Send a request.
					if (hRequest)
						bResults = WinHttpSendRequest(hRequest,
							WINHTTP_NO_ADDITIONAL_HEADERS,
							0, WINHTTP_NO_REQUEST_DATA, 0,
							0, 0);


					// End the request.
					if (bResults)
						bResults = WinHttpReceiveResponse(hRequest, NULL);

					// Keep checking for data until there is nothing left.
					if (bResults)
						do
						{
							// Check for available data.
							dwSize = 0;
							if (!WinHttpQueryDataAvailable(hRequest, &dwSize))

								wsprintf(strBuffer, L"Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
								MessageBox(hWnd, strBuffer, L"Allocate space for the buffer.", MB_OK);
								/////////////////////////////////////////printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

							// Allocate space for the buffer.
							pszOutBuffer = new char[dwSize + 1];
							if (!pszOutBuffer)
							{
								wsprintf(strBuffer, L"Out of memory\n");
								MessageBox(hWnd, strBuffer, L"Allocate space for the buffer.", MB_OK);
								//////////////////////////////////////////printf("Out of memory\n");
								dwSize = 0;
							}
							else
							{
								// Read the Data.
								ZeroMemory(pszOutBuffer, dwSize + 1);

								if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
									dwSize, &dwDownloaded)) {

									wsprintf(strBuffer, L"Error %u in WinHttpReadData.\n", GetLastError());
									MessageBox(hWnd, strBuffer, L"Read the Data.", MB_OK);
									//////////////////////////////////printf("Error %u in WinHttpReadData.\n", GetLastError());
								}
								else {

									wsprintf(strBuffer, L"%s\n", pszOutBuffer);
									MessageBox(hWnd, strBuffer, L"Read the Data.", MB_OK);
									/////////////////////////////////printf("%s\n", pszOutBuffer);
								}
								// Free the memory allocated to the buffer.
								delete[] pszOutBuffer;
							}

						} while (dwSize > 0);


						// Report any errors.
						if (!bResults)

							wsprintf(strBuffer, L"Error %d has occurred.\n", GetLastError());
							MessageBox(hWnd, strBuffer, L"Report any errors.", MB_OK);
							//printf("Error %d has occurred.\n", GetLastError());

						// Close any open handles.
						if (hRequest) WinHttpCloseHandle(hRequest);
						if (hConnect) WinHttpCloseHandle(hConnect);
						if (hSession) WinHttpCloseHandle(hSession);

					}
				chage_state = wcscmp(tvi.pszText, L"ｒｅｃｖ関数");
				if (chage_state == 0) {
					MessageBox(hWnd, L"HTTPサーバー簡易", L"AAA", MB_OK);

					#define DEFAULT_BUFLEN 512
					#define DEFAULT_PORT "27015"


					//----------------------
					// Declare and initialize variables.
					WSADATA wsaData;
					int iResult;

					SOCKET ConnectSocket = INVALID_SOCKET;
					struct sockaddr_in clientService;

					char sendbuf[] = "this is a test";
					char recvbuf[DEFAULT_BUFLEN];
					int recvbuflen = DEFAULT_BUFLEN;

					//----------------------
					// Initialize Winsock
					iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
					if (iResult != NO_ERROR) {

						wsprintf(strBuffer, L"WSAStartup failed: %d\n", iResult);
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//printf("WSAStartup failed: %d\n", iResult);
						return 1;
					}

					//----------------------
					// Create a SOCKET for connecting to server
					ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if (ConnectSocket == INVALID_SOCKET) {

						wsprintf(strBuffer, L"Error at socket(): %ld\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"2", MB_OK);
						//printf("Error at socket(): %ld\n", WSAGetLastError());
						WSACleanup();
						return 1;
					}

					//----------------------
					// The sockaddr_in structure specifies the address family,
					// IP address, and port of the server to be connected to.
					clientService.sin_family = AF_INET;
					//clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
					InetPton(clientService.sin_family, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
					//void * AAA;
					//inet_pton(AF_INET, "127.0.0.1", AAA);
					//clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
					clientService.sin_port = htons(27015);

					//----------------------
					// Connect to server.
					iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
					if (iResult == SOCKET_ERROR) {
						closesocket(ConnectSocket);

						wsprintf(strBuffer, L"Unable to connect to server: %ld\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"3", MB_OK);
						//printf("Unable to connect to server: %ld\n", WSAGetLastError());
						WSACleanup();
						return 1;
					}

					// Send an initial buffer
					iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
					if (iResult == SOCKET_ERROR) {

						wsprintf(strBuffer, L"send failed: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"4", MB_OK);
						//printf("send failed: %d\n", WSAGetLastError());
						closesocket(ConnectSocket);
						WSACleanup();
						return 1;
					}


					wsprintf(strBuffer, L"Bytes Sent: %ld\n", iResult);
					MessageBox(hWnd, strBuffer, L"5", MB_OK);
					//printf("Bytes Sent: %ld\n", iResult);

					// shutdown the connection since no more data will be sent
					iResult = shutdown(ConnectSocket, SD_SEND);
					if (iResult == SOCKET_ERROR) {

						wsprintf(strBuffer, L"shutdown failed: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"6", MB_OK);
						//printf("shutdown failed: %d\n", WSAGetLastError());
						closesocket(ConnectSocket);
						WSACleanup();
						return 1;
					}

					// Receive until the peer closes the connection
					do {

						iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
						if (iResult > 0)
						{
							wsprintf(strBuffer, L"Bytes received: %d\n", iResult);
							MessageBox(hWnd, strBuffer, L"7", MB_OK);
							//printf("Bytes received: %d\n", iResult);
						}
						else if (iResult == 0)
						{
							wsprintf(strBuffer, L"Connection closed\n");
							MessageBox(hWnd, strBuffer, L"8", MB_OK);
							//printf("Connection closed\n");
						}
						else
						{
							wsprintf(strBuffer, L"recv failed: %d\n", WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"9", MB_OK);
							//printf("recv failed: %d\n", WSAGetLastError());
						}

					} while (iResult > 0);

					// cleanup
					closesocket(ConnectSocket);
					WSACleanup();

					return 0;
				}
				chage_state = wcscmp(tvi.pszText, L"ＷＳＡｒｅｃｖ関数");
				if (chage_state == 0){
					MessageBox(hWnd, L"WSArecv", L"WSArecv", MB_OK);


					#define DATA_BUFSIZE 4096


					WSADATA wsd;
					struct addrinfo *result = NULL, *ptr = NULL, hints;
					WSAOVERLAPPED RecvOverlapped;
					SOCKET ConnSocket = INVALID_SOCKET;
					WSABUF DataBuf;
					DWORD RecvBytes, Flags;
					char buffer[DATA_BUFSIZE];

					int err = 0;
					int rc;

					if (0) {

						wsprintf(strBuffer, L"usage: %s server-name\n", L"AAA");
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//wprintf(L"usage: %s server-name\n", argv[0]);

						return 1;
					}
						// Load Winsock
					rc = WSAStartup(MAKEWORD(2, 2), &wsd);
					if (rc != 0) {

						wsprintf(strBuffer, L"Unable to load Winsock: %d\n", rc);
						MessageBox(hWnd, strBuffer, L"2", MB_OK);
						//wprintf(L"Unable to load Winsock: %d\n", rc);
						return 1;
					}
					// Make sure the hints struct is zeroed out
					SecureZeroMemory((PVOID)& hints, sizeof(struct addrinfo));

					// Initialize the hints to retrieve the server address for IPv4
					hints.ai_family = AF_INET;
					hints.ai_socktype = SOCK_STREAM;
					hints.ai_protocol = IPPROTO_TCP;

					rc = getaddrinfo("", "27015", &hints, &result);
					if (rc != 0) {
						
						wsprintf(strBuffer, L"getaddrinfo failed with error: %d\n", rc);
						MessageBox(hWnd, strBuffer, L"3", MB_OK);
						//wprintf(L"getaddrinfo failed with error: %d\n", rc);
						return 1;
					}

					for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

						ConnSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
						if (ConnSocket == INVALID_SOCKET) {

							wsprintf(strBuffer, L"socket failed with error: %d\n", WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"4", MB_OK);
							//wprintf(L"socket failed with error: %d\n", WSAGetLastError());
							freeaddrinfo(result);
							return 1;
						}

						rc = connect(ConnSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
						if (rc == SOCKET_ERROR) {

							if (WSAECONNREFUSED == (err = WSAGetLastError())) {
								closesocket(ConnSocket);
								ConnSocket = INVALID_SOCKET;
								continue;
							}

							wsprintf(strBuffer, L"connect failed with error: %d\n", err);
							MessageBox(hWnd, strBuffer, L"5", MB_OK);
							//wprintf(L"connect failed with error: %d\n", err);
							freeaddrinfo(result);
							closesocket(ConnSocket);
							return 1;
						}
						break;
					}
					if (ConnSocket == INVALID_SOCKET) {

						wsprintf(strBuffer, L"Unable to establish connection with the server!\n");
						MessageBox(hWnd, strBuffer, L"6", MB_OK);
						//wprintf(L"Unable to establish connection with the server!\n");
						freeaddrinfo(result);
						return 1;
					}


					wsprintf(strBuffer, L"Client connected...\n");
					MessageBox(hWnd, strBuffer, L"7", MB_OK);
					//wprintf(L"Client connected...\n");

					// Make sure the RecvOverlapped struct is zeroed out
					SecureZeroMemory((PVOID)& RecvOverlapped, sizeof(WSAOVERLAPPED));

					// Create an event handle and setup an overlapped structure.
					RecvOverlapped.hEvent = WSACreateEvent();
					if (RecvOverlapped.hEvent == NULL) {

						wsprintf(strBuffer, L"WSACreateEvent failed: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"8", MB_OK);
						//wprintf(L"WSACreateEvent failed: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						closesocket(ConnSocket);
						return 1;
					}

					DataBuf.len = DATA_BUFSIZE;
					DataBuf.buf = buffer;

						// Call WSARecv until the peer closes the connection
						// or until an error occurs
					while (1) {

						Flags = 0;
						rc = WSARecv(ConnSocket, &DataBuf, 1, &RecvBytes, &Flags, &RecvOverlapped, NULL);
						if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) {


							wsprintf(strBuffer, L"WSARecv failed with error: %d\n", err);
							MessageBox(hWnd, strBuffer, L"9", MB_OK);
							//wprintf(L"WSARecv failed with error: %d\n", err);
							break;
						}

						rc = WSAWaitForMultipleEvents(1, &RecvOverlapped.hEvent, TRUE, INFINITE, TRUE);
						if (rc == WSA_WAIT_FAILED) {

							wsprintf(strBuffer, L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"10", MB_OK);
							//wprintf(L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
							break;
						}

						rc = WSAGetOverlappedResult(ConnSocket, &RecvOverlapped, &RecvBytes, FALSE, &Flags);
						if (rc == FALSE) {

							wsprintf(strBuffer, L"WSARecv operation failed with error: %d\n", WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"11", MB_OK);
							//wprintf(L"WSARecv operation failed with error: %d\n", WSAGetLastError());
							break;
						}


						wsprintf(strBuffer, L"Read %d bytes\n", RecvBytes);
						MessageBox(hWnd, strBuffer, L"12", MB_OK);
						//wprintf(L"Read %d bytes\n", RecvBytes);

						WSAResetEvent(RecvOverlapped.hEvent);

							// If 0 bytes are received, the connection was closed5
						if (RecvBytes == 0)
							break;
					}

					WSACloseEvent(RecvOverlapped.hEvent);
					closesocket(ConnSocket);
					freeaddrinfo(result);

					WSACleanup();

					return 0;
				}
				chage_state = wcscmp(tvi.pszText, L"ＷＳＡＡｃｃｅｐｔ関数"); 
				if (chage_state == 0) {
					MessageBox(hWnd, L"WSAAccept", L"WSAAccept", MB_OK);

					HANDLE hThread;
					DWORD dwThreadId;

					//hWnd_strBuffer_struct CCC;
					//CCC.hWnd = hWnd;


					//CCC.strBuffer = strBuffer;



					//スレッド起動
					hThread = CreateThread(
						NULL, //セキュリティ属性
						0, //スタックサイズ
						ThreadFunc, //スレッド関数
						NULL, //スレッド関数に渡す引数
						0, //作成オプション(0またはCREATE_SUSPENDED)
						&dwThreadId);//スレッドID
									 //map();list();[][][];

					Sleep(1000);

					//スレッドを停止
					SuspendThread(hThread);

					Sleep(2000);

					//スレッドを再開
					ResumeThread(hThread);

					//スレッドの終了を待つ
					WaitForSingleObject(hThread, 100);

					//スレッドのハンドルを閉じる
					CloseHandle(hThread);





				}
				chage_state = wcscmp(tvi.pszText, L"ＷＳＡＣｏｎｎｅｃｔ関数"); 
				if (chage_state == 0) {

					MessageBox(hWnd, L"ＷＳＡＣｏｎｎｅｃｔ関数", L"ＷＳＡＣｏｎｎｅｃｔ関数", MB_OK);


					#define DATA_BUFSIZE 4096
					#define SEND_COUNT   10

					WSADATA wsd;

					struct addrinfo *result = NULL;
					struct addrinfo hints;

					SOCKET ListenSocket = INVALID_SOCKET;
					SOCKET AcceptSocket = INVALID_SOCKET;


					int err = 0;
					int rc;

					// Load Winsock
					rc = WSAStartup(MAKEWORD(2, 2), &wsd);
					if (rc != 0) {
						wsprintf(strBuffer, L"Unable to load Winsock: %d\n", rc);
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//printf("Unable to load Winsock: %d\n", rc);
						return 1;
					}

						// Make sure the hints struct is zeroed out
					SecureZeroMemory((PVOID)& hints, sizeof(struct addrinfo));

						// Initialize the hints to obtain the 
						// wildcard bind address for IPv4
					hints.ai_family = AF_INET;
					hints.ai_socktype = SOCK_STREAM;
					hints.ai_protocol = IPPROTO_TCP;
					hints.ai_flags = AI_PASSIVE;

					rc = getaddrinfo(NULL, "27015", &hints, &result);
					if (rc != 0) {
						wsprintf(strBuffer, L"getaddrinfo failed with error: %d\n", rc);
						MessageBox(hWnd, strBuffer, L"2", MB_OK);
						//printf("getaddrinfo failed with error: %d\n", rc);
						return 1;
					}

					ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
					if (ListenSocket == INVALID_SOCKET) {
						wsprintf(strBuffer, L"socket failed with error: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"3", MB_OK);
						//printf("socket failed with error: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						return 1;
					}

					// Declare variables


					// Create a listening socket
					//ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

					// Get the local host information


					// Set up the sockaddr structure
					in_addr AAA;
					sockaddr_in service;
					service.sin_family = AF_INET;
					inet_pton(AF_INET, "192.168.6.1", &AAA);
					service.sin_addr.s_addr = AAA.S_un.S_addr;

					service.sin_port = htons(81);
					SecureZeroMemory(service.sin_zero, sizeof(service.sin_zero));


					int nRes = WSAConnect(ListenSocket, (SOCKADDR*)&service, sizeof(service), NULL, NULL, NULL, NULL);
					wsprintf(strBuffer, L"実験中: %d\n", WSAGetLastError());
					MessageBox(hWnd, strBuffer, L"実験中", MB_OK);
					//////////////////////////////////////////////////////////////////////////////////
					/*
					rc = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
					if (rc == SOCKET_ERROR) {

						wsprintf(strBuffer, L"bind failed with error: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"4", MB_OK);
						//printf("bind failed with error: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						closesocket(ListenSocket);
						return 1;
					}

					rc = listen(ListenSocket, 1);
					if (rc == SOCKET_ERROR) {

						wsprintf(strBuffer, L"listen failed with error: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"5", MB_OK);
						//printf("listen failed with error: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						closesocket(ListenSocket);
						return 1;
					}
					// Accept an incoming connection request
					AcceptSocket = accept(ListenSocket, NULL, NULL);
					if (AcceptSocket == INVALID_SOCKET) {

						wsprintf(strBuffer, L"accept failed with error: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"6", MB_OK);
						//printf("accept failed with error: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						closesocket(ListenSocket);
						return 1;
					}


					wsprintf(strBuffer, L"Client Accepted...\n");
					MessageBox(hWnd, strBuffer, L"7", MB_OK);
					//printf("Client Accepted...\n");

					// Make sure the SendOverlapped struct is zeroed out
					SecureZeroMemory((PVOID)& SendOverlapped, sizeof(WSAOVERLAPPED));

					// Create an event handle and setup the overlapped structure.
					SendOverlapped.hEvent = WSACreateEvent();
					if (SendOverlapped.hEvent == NULL) {


						wsprintf(strBuffer, L"WSACreateEvent failed with error: %d\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"8", MB_OK);
						//printf("WSACreateEvent failed with error: %d\n", WSAGetLastError());
						freeaddrinfo(result);
						closesocket(ListenSocket);
						closesocket(AcceptSocket);
						return 1;
					}

					DataBuf.len = DATA_BUFSIZE;
					DataBuf.buf = buffer;

					for (i = 0; i < SEND_COUNT; i++) {

						rc = WSASend(AcceptSocket, &DataBuf, 1, &SendBytes, 0, &SendOverlapped, NULL);
						if ((rc == SOCKET_ERROR) &&
							(WSA_IO_PENDING != (err = WSAGetLastError()))) {

							wsprintf(strBuffer, L"WSASend failed with error: %d\n", err);
							MessageBox(hWnd, strBuffer, L"9", MB_OK);
							//printf("WSASend failed with error: %d\n", err);
							break;
						}

						rc = WSAWaitForMultipleEvents(1, &SendOverlapped.hEvent, TRUE, INFINITE, TRUE);
						if (rc == WSA_WAIT_FAILED) {

							wsprintf(strBuffer, L"WSAWaitForMultipleEvents failed with error: %d\n",WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"10", MB_OK);
							//printf("WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
							break;
						}

						rc = WSAGetOverlappedResult(AcceptSocket, &SendOverlapped, &SendBytes, FALSE, &Flags);
						if (rc == FALSE) {

							wsprintf(strBuffer, L"WSASend failed with error: %d\n", WSAGetLastError());
							MessageBox(hWnd, strBuffer, L"11", MB_OK);
							//printf("WSASend failed with error: %d\n", WSAGetLastError());
							break;
						}

						wsprintf(strBuffer, L"Wrote %d bytes\n", SendBytes);
						MessageBox(hWnd, strBuffer, L"12", MB_OK);

						//printf("Wrote %d bytes\n", SendBytes);

						WSAResetEvent(SendOverlapped.hEvent);

					}



					WSACloseEvent(SendOverlapped.hEvent);
					*/
					//////////////////////////////////////////////////////////////////////////////////////////


					closesocket(AcceptSocket);
					closesocket(ListenSocket);
					freeaddrinfo(result);

					WSACleanup();

					return 0;
				}
				chage_state = wcscmp(tvi.pszText, L"ＢＩＮＤ関数");
				if (chage_state == 0){
					// Declare some variables
					WSADATA wsaData;

					int iResult = 0;            // used to return function results

					// the listening socket to be created
					SOCKET ListenSocket = INVALID_SOCKET;

					// The socket address to be passed to bind
					sockaddr_in service;

					//----------------------
					// Initialize Winsock
					iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
					if (iResult != NO_ERROR) {

						wsprintf(strBuffer, L"Error at WSAStartup()\n");
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//wprintf(L"Error at WSAStartup()\n");
						return 1;
					}
					//----------------------
					// Create a SOCKET for listening for 
					// incoming connection requests
					ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if (ListenSocket == INVALID_SOCKET) {

						wsprintf(strBuffer, L"socket function failed with error: %u\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
						WSACleanup();
						return 1;
					}
					//----------------------
					// The sockaddr_in structure specifies the address family,
					// IP address, and port for the socket that is being bound.
					service.sin_family = AF_INET;
					//service.sin_addr.s_addr = inet_addr("127.0.0.1");
					in_addr AAA;
					inet_pton(AF_INET, "127.0.0.1", &AAA);
					service.sin_addr.s_addr = AAA.S_un.S_addr;
					service.sin_port = htons(27015);

					//----------------------
					// Bind the socket.
					iResult = bind(ListenSocket, (SOCKADDR *)&service, sizeof(service));
					if (iResult == SOCKET_ERROR) {

						wsprintf(strBuffer, L"bind failed with error %u\n", WSAGetLastError());
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//wprintf(L"bind failed with error %u\n", WSAGetLastError());
						closesocket(ListenSocket);
						WSACleanup();
						return 1;
					}
					else {

						wsprintf(strBuffer, L"bind returned success\n");
						MessageBox(hWnd, strBuffer, L"1", MB_OK);
						//wprintf(L"bind returned success\n");
					}


					WSACleanup();
					return 0;

				
				}
				chage_state = wcscmp(tvi.pszText, L"ＷＳＡＳｅｎｄＴｏ関数");
				if (chage_state == 0){}



				SetWindowText(hWnd, (LPCWSTR)buf);
				MessageBox(hWnd, (LPCWSTR)buf, L"AAA", MB_OK);
				MessageBeep(-1);

			}
			break;
			}

			switch (((LPNMHDR)lParam)->code)
			{
			case TVN_GETINFOTIP:
				lpttt = (LPTOOLTIPTEXT)lParam;
				lpttt->hinst = hInst;

				switch (lpttt->hdr.idFrom)
				{
				case 1:
					lpttt->lpszText = (LPWSTR)L"もう良いやこれで、リストビューか？";
					break;

				}
				break;
			}
		}
		break;
		}
		break;


		}
		break;
	case WM_SOCKET:
		{
		switch (wParam) {
		case FD_ACCEPT: {int AAA = 1; }break;
		case FD_READ: {int AAA = 1; }break;
		case FD_CLOSE: {int AAAA = 1; }break;
		case FD_CONNECT: {int AAA = 1; }break;
		case FD_WRITE: {int AAA = 1; }break;
		case '555': {
			//???if (bool a = 0: == 0: a = 1)
			//int AA = SendMessage(hWnd, WM_SOCKET, '555', lParam);
			//PostMessage();
		}break;


		}break;

		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		{
			//自作
			hTview = CreateATreeView(hWnd);
			//InvalidateRect(NULL, NULL, TRUE);
			MessageBeep(-1);
			//自作終わり
		}
		break;
	case WM_SIZE:
		{
		/*
		BOOL SetClientSize(HWND hWnd, int width, int height)
		{
			RECT rw, rc;
			::GetWindowRect(hWnd, &rw);
			::GetClientRect(hWnd, &rc);

			int new_width = (rw.right - rw.left) - (rc.right - rc.left) + width;
			int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

			return ::SetWindowPos(hWnd, NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
		}
		*/


		RECT rcClient;
		RECT rcTV;
		//int width;
		//int height;
		GetClientRect(hWnd, &rcClient);
		//width = LOWORD(lParam);
		//height = HIWORD(lParam);
		GetClientRect(hTview, &rcTV);
		//if (width - 200) { rcTV.right = 200; }
		RECT rw, rc;
		GetWindowRect(hTview, &rw); // ウィンドウ全体のサイズ
		GetClientRect(hTview, &rc); // クライアント領域のサイズ
		int new_width = (rw.right - rw.left) - (rc.right - rc.left);
		MoveWindow(hTview, 0, 0, rcTV.right + new_width, rcClient.bottom, true);

		//SetWindowPos(hWnd, HWND_TOP, 0, 0, rcClient.right, rcClient.bottom, SWP_NOSIZE);
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);

        }
        break;
    case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//////////////////////自作関数///////////////////////////////////////////////////
DWORD WINAPI ThreadFunc(LPVOID arg)
{
	TCHAR strBuffer[1024];
//hWnd_strBuffer_struct * AAA = (hWnd_strBuffer_struct *)arg;
	wsprintf(strBuffer, L"ThreadFunc \n");
	MessageBox(NULL, strBuffer, L"1", MB_OK);
//printf("ThreadFunc %d\n", i);

/* Declare and initialize variables */
	WSADATA wsaData;
	SOCKET ListenSocket = 0, AcceptSocket = 0;
	struct sockaddr_in saClient;
	int iClientSize = sizeof(saClient);
	u_short port = 27015;
	char ip[] = "192.168.6.8";
	sockaddr_in service;
	int error;

	/* Initialize Winsock */
	error = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (error) {

		wsprintf(strBuffer, L"WSAStartup() failed with error : %d\n", error);
		MessageBox(NULL, strBuffer, L"1", MB_OK);
		//printf("WSAStartup() failed with error: %d\n", error);
		return 1;
	}

	/* Create a TCP listening socket */
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {

		wsprintf(strBuffer, L"socket() failed with error: %d\n", WSAGetLastError());
		MessageBox(NULL, strBuffer, L"2", MB_OK);
		//printf("socket() failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	/*-----------------------------------------
	*  Set up the sock addr structure that the listening socket
	*  will be bound to. In this case, the structure holds the
	/ local IP address and the port specified.
	*/
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	//hostent* thisHost;

	////getaddrinfo("", port, &hints, &result);
	//thisHost = gethostbyname("");

	//service.sin_addr.s_addr = inet_addr(ip);
	in_addr AAA;
	inet_pton(AF_INET, ip, &AAA);
	service.sin_addr.s_addr = AAA.S_un.S_addr;

	/*-----------------------------------------
	*  Bind the listening socket to the IP address.
	and port number specified by the sockaddr structure.
	*/
	error = bind(ListenSocket, (SOCKADDR *)&service, sizeof(SOCKADDR));
	if (error == SOCKET_ERROR) {

		wsprintf(strBuffer, L"bind() failed with error: %d\n", WSAGetLastError());
		MessageBox(NULL, strBuffer, L"3", MB_OK);
		//printf("bind() failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	/* Make the socket listen for incoming connection requests */
	error = listen(ListenSocket, 1);
	if (error == SOCKET_ERROR) {
		wsprintf(strBuffer, L"listen() failed with error: %d\n", WSAGetLastError());
		MessageBox(NULL, strBuffer, L"4", MB_OK);
		//printf("listen() failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	wsprintf(strBuffer, L"Listening...\n");
	MessageBox(NULL, strBuffer, L"5", MB_OK);
	//printf("Listening...\n");


	/*-----------------------------------------
	*  Accept an incoming connnection request on the
	*  listening socket and transfer control to the
	accepting socket.
	*/


	while (1) {
	/*	while (1) {
		SleepEx(10000, TRUE);
	}
*/
		//WSAEVENT NewEvent;
		//NewEvent = WSACreateEvent();
		//int rc = WSAAsyncSelect(ListenSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE | FD_ACCEPT | FD_CONNECT | FD_WRITE);
		//int rc = WSAEventSelect(ListenSocket, NewEvent, FD_READ | FD_CLOSE | FD_ACCEPT | FD_CONNECT | FD_WRITE);
		AcceptSocket = WSAAccept(ListenSocket, (SOCKADDR*)&saClient, &iClientSize, NULL, NULL);
		//AcceptSocket = WSAAccept(ListenSocket, (SOCKADDR*)&saClient, &iClientSize, &ConditionAcceptFunc, NULL);
		//int AA = SendMessage(hWnd, WM_SOCKET, '555', AcceptSocket);
		/*  Now do some work with the AcceptSocket
		*  At this point, the application could
		*  handle data transfer on the socket, or other socket
		functionality. /
		*/

		WSABUF buf;
		char RecvBuffer[2048];
		ULONG MESSAGE_SIZE = sizeof(RecvBuffer);

		buf.buf = RecvBuffer;
		buf.len = MESSAGE_SIZE;

		DWORD RecvBytes;
		DWORD flag = 0;
		// クライアントから受信
		// WSARecv Function
		int hr = WSARecv(AcceptSocket,
			&buf,
			1,
			&RecvBytes,
			&flag,
			NULL,
			NULL
		);
		


			SleepEx(10, true);

		}

	/* Then clean up and quit */

	closesocket(AcceptSocket);
	closesocket(ListenSocket);
	WSACleanup();



	Sleep(500);

	return 0;
}


