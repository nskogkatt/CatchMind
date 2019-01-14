#include "WaitingRoom.h"
#include "ClientInfo.h"
//#include "IOCP_ServerManager.h"


void WaitingRoom::Init(short nRoomNumber, char * szRoomName, SOCKET& clientSock, ClientInfo* clientInfo)
{
	m_nRoomNumber = nRoomNumber;
	strcpy(m_szRoomName, szRoomName);
	m_nSupervisorKey = clientSock;
	strcpy(m_szSuperVisorName, clientInfo->m_szName);
	clientInfo->m_roomNumber = nRoomNumber;

	m_mapRoomClient.insert(make_pair(clientSock, clientInfo));
}

void WaitingRoom::AddUser(SOCKET & clientSock, ClientInfo * clientInfo)
{
	clientInfo->m_roomNumber = m_nRoomNumber;
	m_mapRoomClient.insert(make_pair(clientSock, clientInfo));
}

void WaitingRoom::LeaveUser(SOCKET& clientSock)
{
	m_mapRoomClient[clientSock]->m_isSuperVisor = false;
	m_mapRoomClient[clientSock]->m_roomNumber = 0;
	m_mapRoomClient.erase(clientSock);

	if (m_mapRoomClient.size() > 0)
	{
		auto iter = m_mapRoomClient.begin();
		iter->second->m_isSuperVisor = true;
		strcpy(m_szSuperVisorName, iter->second->m_szName);
	}
	else
	{
		strcpy(m_szSuperVisorName, "");
	}
}

void WaitingRoom::RefreshUserList(SOCKET& clientSock)
{
	PACKET_JOINROOM_USERLIST packet;
	packet.header.wIndex = PACKET_INDEX_JOINROOM_USERLIST;
	packet.header.wLen = sizeof(packet);
	
	for (auto iter = m_mapRoomClient.begin(); iter != m_mapRoomClient.end(); iter++)
	{
		packet.userInfo.identifyKey = (*iter).second->m_sock;
		packet.userInfo.m_ePlayerType = (*iter).second->m_ePlayerType;
		strcpy(packet.userInfo.szNickName,(*iter).second->m_szName);
		strcpy(packet.userInfo.szLevel,(*iter).second->m_szLevel);
		strcpy(packet.userInfo.szPosition,(*iter).second->m_szPosition);

		send(clientSock, (const char*)&packet, sizeof(packet), 0);
	}
}

short WaitingRoom::GetRoomHeadCount() const
{
	return m_mapRoomClient.size();
}

char * WaitingRoom::GetRoomName() 
{
	return m_szRoomName;
}

PACKET_ROOM_LIST & WaitingRoom::GetRoomInfo()
{
	PACKET_ROOM_LIST packet;
	packet.header.wIndex = PACKET_INDEX_ROOM_LIST;
	packet.header.wLen = sizeof(PACKET_ROOM_LIST);

	strcpy(packet.roomName, m_szRoomName);
	packet.roomNumber = m_nRoomNumber;
	packet.roomSize = m_mapRoomClient.size();
	strcpy(packet.superVisorName, m_szSuperVisorName);

	return packet;
}

WaitingRoom::WaitingRoom()
{
}


WaitingRoom::~WaitingRoom()
{
}
