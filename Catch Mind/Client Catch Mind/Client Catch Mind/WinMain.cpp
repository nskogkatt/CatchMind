#pragma comment(lib, "ws2_32")
#pragma comment(lib, "msimg32.lib")

#include <WinSock2.h>
#include <process.h>
#include "GameManager.h"
#include "TCPManager.h"

#include "../../Common/PACKET.h"

#define BUFSIZE		512

CRITICAL_SECTION g_cs;

// 스레드 처리 함수
unsigned int WINAPI ClientWorkerThread(void* arg);
void MyBeginThreadEx(HANDLE hCp);

// 에러처리 asdf
void err_display(char * msg);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;

char* lpszClass = "WinMain";


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, "CatchMind Client", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1024, 768, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	InitializeCriticalSection(&g_cs);

	GameManager::GetInstance()->Init(hWnd, hInstance, (LONG)EditSubProc);

	TCPManager::GetInstance()->Init();
	MyBeginThreadEx(TCPManager::GetInstance()->GetHandleCompletionPort());
	TCPManager::GetInstance()->ConnectToServer();



	while (true)
	{
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			GameManager::GetInstance()->Update();
		}
	}

	GameManager::GetInstance()->Release();
	TCPManager::GetInstance()->Release();

	DeleteCriticalSection(&g_cs);

	return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	char szBuf[256];

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		GetCursorPos(&ptMouse);
		ScreenToClient(hWnd, &ptMouse);
		sprintf(szBuf, "X: %d, Y: %d", ptMouse.x, ptMouse.y);
		SetWindowText(hWnd, szBuf);

		GameManager::GetInstance()->InputMouseLButtonDown(ptMouse);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}



unsigned int WINAPI ClientWorkerThread(void* arg)
{
	int retval;
	HANDLE hCp = (HANDLE)arg;
	DWORD cbTransferred;
	DWORD sendBytes, recvBytes;
	DWORD flags = 0;
	ULONG CompletionKey;
	SOCKETINFO* ptr;

	while (1)
	{
		retval = GetQueuedCompletionStatus(hCp, &cbTransferred, &CompletionKey, (LPOVERLAPPED*)&ptr, INFINITE);
		
		EnterCriticalSection(&g_cs);

		// 비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				MessageBox(NULL, "WSAGetOverlappedResult()", "Notice! - Error", 0);
			}
			closesocket(ptr->sock);
			delete ptr;
			continue;
		}

		//받은 데이터크기 저장
		ptr->recvbytes += cbTransferred;

		while (1)
		{
			if (!TCPManager::GetInstance()->ProcessPacket(ptr->wsabuf.buf, ptr->recvbytes))
			{
				// 받을 데이터가 더 있으면
				retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL);
				if (retval == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSA_IO_PENDING)
					{
						MessageBox(NULL, "WSARecv()", "Notice - Error", 0);
					}
					break;
				}
				else
				{
					ptr->recvbytes += recvBytes;
				}
			}
			else
			{
				if (TCPManager::GetInstance()->GetRecvByte() > 0)
				{
					continue;
				}
				else
				{
					// 다음 데이터 받기
					retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL);
					if (retval == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSA_IO_PENDING)
						{
							MessageBox(NULL, "WSARecv()", "Notice - Error", 0);
						}
						break;
					}
					else if (recvBytes > 0)
					{
						ptr->recvbytes += recvBytes;
						continue;
					}
				}
				break;
			}
		}
		LeaveCriticalSection(&g_cs);
	}

	_endthreadex(0);
	return 0;
}

void MyBeginThreadEx(HANDLE hCp)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	HANDLE hThread;
	// (cpu 개수 * 2) 개의 작업자 스레드 생성
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, ClientWorkerThread, hCp, 0, NULL);
		if (hThread == NULL) exit(1);
		CloseHandle(hThread);
	}
}


void err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}


LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int   numLine;
	char  szStr[1025], szStrOut[1025];
	RECT rcRect;
	HDC hdc;
	SetRect(&rcRect, 0, 200, 100, 300);		

	switch (iMessage)
	{
	case WM_CHAR:
		// 엔터를 누를때 나는 소리를 잠재우기 위해
		// 엔터가 눌릴때 발생되는 WM_CHAR메시지는 막습니다.
		if (wParam == VK_RETURN)
			return 0;
		break;

	case WM_KEYDOWN:

		if (wParam == VK_RETURN)  // 엔터가 눌리면...
		{
			// Edit 입력창에 문자열 가져오기
			GetWindowText(hWnd, szStr, 1024);

			sprintf(szStrOut, "%s : %s", GameManager::GetInstance()->GetPlayerNickName(), szStr);

			// 브로드랑 방이랑 나눠서 보내면될듯
			TCPManager::GetInstance()->SendTextMessageToServer(szStrOut);


			// Edit 창 비우기
			SetWindowText(GameManager::GetInstance()->GetEditHandleUI(EDIT_TYPE_WAITING_ROOM_CHATTING), "");

			// Edit 컨트롤에 포커스를 줍니다.
			SetFocus(hWnd);

			return 0;
		}
		break;
	}

	return CallWindowProc(GameManager::GetInstance()->GetEditOldHandleUI(EDIT_TYPE_WAITING_ROOM_CHATTING), hWnd, iMessage, wParam, lParam);
}