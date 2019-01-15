#pragma once
#include "../../Common/defineSize.h"
#include "../../Common/PACKET.h"
#include <Windows.h>
#include <map>


using std::map;
using std::make_pair;

class ClientInfo;
class WaitingRoom
{
	short							m_nRoomNumber;

	int								m_nSupervisorKey;

	char							m_szRoomName[ROOM_NAMESIZE];
	char							m_szSuperVisorName[NAMESIZE];

	map<SOCKET, ClientInfo*>		m_mapRoomClient;


public:

	void Init(short nRoomNumber, char* szRoomName, SOCKET& clientSock, ClientInfo* clientInfo);

	void AddUser(SOCKET& clientSock, ClientInfo* clientInfo);
	void LeaveUser(SOCKET& clientSock);
	void JoinRoomUserList(SOCKET& clientSock);
	short GetRoomHeadCount() const;
	char* GetRoomName();
	PACKET_ROOM_LIST&	GetRoomInfo();

	WaitingRoom();
	~WaitingRoom();
};

