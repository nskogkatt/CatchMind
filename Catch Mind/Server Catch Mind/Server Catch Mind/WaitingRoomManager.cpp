#include "WaitingRoomManager.h"
#include "WaitingRoom.h"
#include "ClientInfo.h"
#include "../../Common/PACKET.h"
//#include "IOCP_ServerManager.h"




WaitingRoomManager* WaitingRoomManager::m_pThis = NULL;


WaitingRoomManager::WaitingRoomManager()
{
	m_iRoomIndex = 1;
}

void WaitingRoomManager::CreateRoom(char* roomName, SOCKET & clientSock, ClientInfo * clientInfo)
{
	WaitingRoom* pRoom = new WaitingRoom;
	pRoom->Init(m_iRoomIndex, roomName, clientSock, clientInfo);
	m_mapRoom.insert(make_pair(m_iRoomIndex, pRoom));

	printf("[방생성] 방이름: %s, 방장: %s\n", roomName, clientInfo->m_szName);

	m_iRoomIndex++;
}

bool WaitingRoomManager::JoinRoom(int roomNumber, ClientInfo * clientInfo)
{
	auto iter = m_mapRoom.find(roomNumber);
	if (iter == m_mapRoom.end())
		return false;

	m_mapRoom[roomNumber]->AddUser(clientInfo);
	printf("[방참여] 방번호: %d, 참여자: %s, 총인원: %d / 8\n", roomNumber, clientInfo->m_szName,m_mapRoom[roomNumber]->GetRoomHeadCount());

	return true;
}

void WaitingRoomManager::JoinRoomUserListInfo(SOCKET& clientSock, int roomNumber)
{
	m_mapRoom[roomNumber]->JoinRoomUserList();
}

int WaitingRoomManager::LeaveRoom(int roomNumber, ClientInfo* clientInfo)
{
	if (m_mapRoom.size() <= 0 || roomNumber == 0)
		return 0;

	m_mapRoom[roomNumber]->LeaveUser(clientInfo);

	printf("[방나감] 방번호: %d, 나간사람: %s, 남은인원: %d / 8\n", roomNumber, clientInfo->m_szName, m_mapRoom[roomNumber]->GetRoomHeadCount());
	if (m_mapRoom[roomNumber]->GetRoomHeadCount() == 0)
	{
		delete m_mapRoom[roomNumber];
		m_mapRoom[roomNumber] = NULL;
		m_mapRoom.erase(roomNumber);
		return roomNumber;
	}

}

void WaitingRoomManager::SendRoomListToClient(SOCKET clientSock)
{
	PACKET_ROOM_LIST packet;

	for (auto iter = m_mapRoom.begin(); iter != m_mapRoom.end();)
	{
		packet = iter->second->GetRoomInfo();

		if (++iter != m_mapRoom.end())
			packet.bIsEnd = false;
		else
			packet.bIsEnd = true;

		send(clientSock, (const char*)&packet, sizeof(PACKET_ROOM_LIST), 0);
	}
}

void WaitingRoomManager::SendRemoveRoomList(short roomNumber, SOCKET clientSock)
{
	PACKET_REMOVE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_REMOVE_ROOM;
	packet.header.wLen = sizeof(PACKET_REMOVE_ROOM);
	packet.roomNumber = roomNumber;

	send(clientSock, (const char*)&packet, sizeof(PACKET_REMOVE_ROOM), 0);
}

int WaitingRoomManager::GetRoomCount()
{
	return m_mapRoom.size();
}

void WaitingRoomManager::Release()
{
	delete m_pThis;
	m_pThis = NULL;
}

WaitingRoomManager::~WaitingRoomManager()
{
}
