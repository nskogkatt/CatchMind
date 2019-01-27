#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "../../Common/defineSize.h"
#include <deque>
using std::deque;

struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	DWORD dwFlags;
	WSABUF wsabuf;
};



class TCPManager
{
private:
	static TCPManager*			m_pThis;
	TCPManager();

	WSADATA						m_wsa;
	SOCKET						m_Sock;
	HWND						m_hWnd;
	HANDLE						m_hCp;
	char						m_szBuf[BUFSIZE + 1];
	int							m_recvBytes;

	deque<UserInfo>				m_dequeJoinRoomUserInfo;
	deque<UserInfo>				m_dequeUIUserList;
	deque<RoomInfo>				m_dequeUIRoomList;

	// ���� ��� �Լ�
	void err_quit(char* msg);
	void err_display(char* msg);
	void SetUserInfo(UserInfo& userInfoSrc, UserInfo& userInfoDst);
	void SetRoomInfo(RoomInfo& roomInfoSrc, RoomInfo& roomInfoDst);

	void RequestRefreshRoomListToServer();

public:
	static TCPManager* GetInstance()
	{
		if (m_pThis == NULL)
			m_pThis = new TCPManager;

		return m_pThis;
	}

	// ���� �ʱ�ȭ
	void Init();
	void ConnectToServer();
	void Release();

	bool ProcessPacket(char* szBuf, int& recvBytes);

	int GetRecvByte()
	{
		return m_recvBytes;
	}
	HANDLE GetHandleCompletionPort()
	{
		return m_hCp;
	}

	void SendModifyPlayerTypeToServer(PLAYER_TYPE type);
	void SendTextMessageToServer(char* sendMessage);
	void SendCreateRoomToServer(char* roomName);
	void SendJoinRoomToServer(int roomNumber);

	~TCPManager();
};

