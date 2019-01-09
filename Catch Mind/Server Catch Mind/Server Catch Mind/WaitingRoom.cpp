#include "WaitingRoom.h"
#include "ClientInfo.h"



void WaitingRoom::Init(short nRoomNumber, char * szRoomName,SOCKET& clientSock, ClientInfo* clientInfo)
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

short WaitingRoom::GetRoomHeadCount() const
{
	return m_mapRoomClient.size();
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
