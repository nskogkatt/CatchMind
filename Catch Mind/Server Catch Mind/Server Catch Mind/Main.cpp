#pragma comment(lib, "ws2_32")
#pragma comment(lib, "msimg32.lib")

#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include "IOCP_ServerManager.h"
#include "../../Common/defineSize.h"

CRITICAL_SECTION g_cs;

// 윈도우 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 스레드 처리 함수
unsigned int WINAPI WorkerThread(void* arg);
void MyBeginThreadEx(HANDLE hCp);

// 에러처리
void err_display(char * msg);

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&g_cs);

	// 윈속 및 소켓 초기화
	IOCP_ServerManager::GetInstance()->Init();

	MyBeginThreadEx(IOCP_ServerManager::GetInstance()->GetHandleCompletionPort());
	IOCP_ServerManager::GetInstance()->ProcessSocketMessage();

	// 윈속 종료
	IOCP_ServerManager::GetInstance()->Release();

	DeleteCriticalSection(&g_cs);
}


// Woker 쓰레드
unsigned int WINAPI WorkerThread(void* arg)
{
	int retval;
	HANDLE hCp = (HANDLE)arg;
	DWORD recvBytes;
	DWORD flags = 0;

	while (1)
	{
		// 비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO* ptr;
		retval = GetQueuedCompletionStatus(hCp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

		// 클라이언트 정보 얻기
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

		EnterCriticalSection(&g_cs);

		// 비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
				
				//오류나서 클라이언트 강제종료시 생성된 방 나가기 및 클라이언트 종료
				IOCP_ServerManager::GetInstance()->LeaveGameRoom(ptr->sock);
				IOCP_ServerManager::GetInstance()->LeaveClient(ptr->sock);
			}
			closesocket(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트번호 = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			if (ptr != NULL)
			{
				delete ptr;
				ptr = NULL;
			}
			continue;
		}

		//받은 데이터크기 저장
		ptr->recvbytes += cbTransferred;


		while (1)
		{
			if (!IOCP_ServerManager::GetInstance()->ProcessPacket(ptr->sock, ptr->wsabuf.buf, ptr->recvbytes))
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
				if (IOCP_ServerManager::GetInstance()->GetClientRecvBytes(ptr->sock) > 0)
				{
					continue;
				}
				else
				{
					// 받을 데이터가 더 있으면 asdfasdfds
					retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL);
					if (retval == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSA_IO_PENDING)
						{
							MessageBox(NULL, "WSARecv()", "Notice - Error", 0);
						}
						break;
					}
					else if (recvBytes > 0 && retval != 0)
					{
						// 정상적으로..안됐을때만?
						ptr->recvbytes += recvBytes;
						continue;
					}
				}

				if (IOCP_ServerManager::GetInstance()->GetIsClientLeave(ptr->sock))
				{
					IOCP_ServerManager::GetInstance()->LeaveClient(ptr->sock);
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
	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);


	HANDLE hThread;
	// (cpu 개수 * 2) 개의 작업자 스레드 생성
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, hCp, 0, NULL);
		if (hThread == NULL) exit(1);
		CloseHandle(hThread);
	}
}



void err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}