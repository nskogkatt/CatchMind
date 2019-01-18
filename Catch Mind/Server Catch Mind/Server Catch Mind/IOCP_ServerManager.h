#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include "../../Common/defineSize.h"

using std::map;
using std::vector;


struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};


class ClientInfo;
class IOCP_ServerManager
{
	static IOCP_ServerManager*			m_pThis;
	IOCP_ServerManager();

	WSADATA								m_wsa;
	HANDLE								m_hCp;
	SOCKET								m_listen_sock;

	int									m_iClientIndex;

	map<SOCKET, ClientInfo*>			m_mapClient;
	vector<char*>						m_vRoomList;

	//오류 출력 함수
	void err_quit(char* msg);
	void err_display(char* msg);
	void err_display(int errcode);


public:
	static IOCP_ServerManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new IOCP_ServerManager;
		}
		return m_pThis;
	}

	void Init();
	void Release();

	// 소켓 관련 메시지 처리
	void ProcessSocketMessage();
	bool ProcessPacket(SOCKET& clientSock, char* szBuf, int& recvBytes);

	int GetClientRecvBytes(SOCKET& clientSock);
	bool GetIsClientLeave(SOCKET& clientSock);
	void LeaveClient(SOCKET& clientSock);
	void LeaveGameRoom(SOCKET clientSock);

	HANDLE GetHandleCompletionPort() const
	{
		return m_hCp;
	}


	~IOCP_ServerManager();
};

