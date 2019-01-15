#pragma once
#include <Windows.h>
#include <map>

using std::map;
using std::make_pair;


class ClientInfo;
class WaitingRoom;
class WaitingRoomManager
{
	static WaitingRoomManager*		m_pThis;
	WaitingRoomManager();

	map<int, WaitingRoom*>			m_mapRoom;

	short							m_iRoomIndex;


public:
	static WaitingRoomManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new WaitingRoomManager;
		}
		return m_pThis;
	}

	void	Init();

	void	CreateRoom(char* roomName, SOCKET& clientSock, ClientInfo* clientInfo);
	void	JoinRoom(int roomNumber, ClientInfo* clientInfo);
	void	JoinRoomUserListInfo(SOCKET& clientSock, int roomNumber);
	int		LeaveRoom(int roomNumber, ClientInfo* clientInfo);


	void	SendRoomListToClient(SOCKET clientSock);
	void	SendRemoveRoomList(short roomNumber, SOCKET clientSock);
	int		GetRoomCount();

	void	Release();

	~WaitingRoomManager();
};

