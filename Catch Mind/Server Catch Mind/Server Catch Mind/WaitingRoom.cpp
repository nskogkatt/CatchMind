#include "WaitingRoom.h"
#include "ClientInfo.h"


void WaitingRoom::Init(short nRoomNumber, char * szRoomName, SOCKET& clientSock, ClientInfo* clientInfo)
{
	m_nRoomNumber = nRoomNumber;
	strcpy(m_szRoomName, szRoomName);
	m_nSupervisorKey = clientSock;
	strcpy(m_szSuperVisorName, clientInfo->m_szName);
	clientInfo->m_roomNumber = nRoomNumber;
	clientInfo->m_JoinRoomSequence = 1;

	m_mapRoomClient.insert(make_pair(clientInfo->m_JoinRoomSequence, clientInfo));
}

void WaitingRoom::AddUser(ClientInfo * clientInfo)
{
	clientInfo->m_roomNumber = m_nRoomNumber;

	int i = 1;

	while (i <= MAX_HEADCOUNT)
	{
		auto iter = m_mapRoomClient.insert(make_pair(i, nullptr));
		if (iter.second)
		{
			iter.first->second = clientInfo;
			clientInfo->m_JoinRoomSequence = i;
			break;
		}
		else
		{

		}
		i++;
	}
}

void WaitingRoom::LeaveUser(ClientInfo* clientInfo)
{
	m_mapRoomClient[clientInfo->m_JoinRoomSequence]->m_isSuperVisor = false;
	m_mapRoomClient[clientInfo->m_JoinRoomSequence]->m_roomNumber = 0;
	m_mapRoomClient.erase(clientInfo->m_JoinRoomSequence);

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

void WaitingRoom::JoinRoomUserList()
{
	PACKET_JOINROOM_USERLIST packet;
	packet.header.wIndex = PACKET_INDEX_JOINROOM_USERLIST;
	packet.header.wLen = sizeof(packet);

	for (auto iter = m_mapRoomClient.begin(); iter != m_mapRoomClient.end();)
	{
		packet.userInfo.identifyKey = (*iter).second->m_sock;
		packet.userInfo.m_ePlayerType = (*iter).second->m_ePlayerType;
		strcpy(packet.userInfo.szNickName, (*iter).second->m_szName);
		strcpy(packet.userInfo.szLevel, (*iter).second->m_szLevel);
		strcpy(packet.userInfo.szPosition, (*iter).second->m_szPosition);
		packet.userInfo.joinRoomSequence = (*iter).second->m_JoinRoomSequence;
		++iter;
		if (iter != m_mapRoomClient.end())
			packet.bIsEnd = false;
		else
			packet.bIsEnd = true;

		for (auto iter = m_mapRoomClient.begin(); iter != m_mapRoomClient.end(); iter++)
			send(iter->second->m_sock, (const char*)&packet, sizeof(packet), 0);
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

	strcpy(packet.roomInfo.roomName, m_szRoomName);
	packet.roomInfo.roomNumber = m_nRoomNumber;
	packet.roomInfo.roomSize = m_mapRoomClient.size();
	strcpy(packet.roomInfo.superVisorName, m_szSuperVisorName);

	return packet;
}

WaitingRoom::WaitingRoom()
{
}


WaitingRoom::~WaitingRoom()
{
}
