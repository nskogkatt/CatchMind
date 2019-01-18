#pragma comment(lib, "ws2_32")
#pragma comment(lib, "msimg32.lib")

#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include "IOCP_ServerManager.h"
#include "../../Common/defineSize.h"

CRITICAL_SECTION g_cs;

// ������ �޽��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ������ ó�� �Լ�
unsigned int WINAPI WorkerThread(void* arg);
void MyBeginThreadEx(HANDLE hCp);

// ����ó��
void err_display(char * msg);

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&g_cs);

	// ���� �� ���� �ʱ�ȭ
	IOCP_ServerManager::GetInstance()->Init();

	MyBeginThreadEx(IOCP_ServerManager::GetInstance()->GetHandleCompletionPort());
	IOCP_ServerManager::GetInstance()->ProcessSocketMessage();

	// ���� ����
	IOCP_ServerManager::GetInstance()->Release();

	DeleteCriticalSection(&g_cs);
}


// Woker ������
unsigned int WINAPI WorkerThread(void* arg)
{
	int retval;
	HANDLE hCp = (HANDLE)arg;
	DWORD recvBytes;
	DWORD flags = 0;

	while (1)
	{
		// �񵿱� ����� �Ϸ� ��ٸ���
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO* ptr;
		retval = GetQueuedCompletionStatus(hCp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

		// Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

		EnterCriticalSection(&g_cs);

		// �񵿱� ����� ��� Ȯ��
		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
				
				//�������� Ŭ���̾�Ʈ ��������� ������ �� ������ �� Ŭ���̾�Ʈ ����
				IOCP_ServerManager::GetInstance()->LeaveGameRoom(ptr->sock);
				IOCP_ServerManager::GetInstance()->LeaveClient(ptr->sock);
			}
			closesocket(ptr->sock);
			printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ��ȣ = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			if (ptr != NULL)
			{
				delete ptr;
				ptr = NULL;
			}
			continue;
		}

		//���� ������ũ�� ����
		ptr->recvbytes += cbTransferred;


		while (1)
		{
			if (!IOCP_ServerManager::GetInstance()->ProcessPacket(ptr->sock, ptr->wsabuf.buf, ptr->recvbytes))
			{
				// ���� �����Ͱ� �� ������
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
					// ���� �����Ͱ� �� ������ asdfasdfds
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
						// ����������..�ȵ�������?
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
	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);


	HANDLE hThread;
	// (cpu ���� * 2) ���� �۾��� ������ ����
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