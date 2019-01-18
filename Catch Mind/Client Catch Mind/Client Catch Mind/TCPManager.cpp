#include "TCPManager.h"
#include "../../Common/PACKET.h"
#include <stdio.h>
#include "GameManager.h"
#include "UIManager.h"


TCPManager* TCPManager::m_pThis = NULL;


#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define WM_SOCKET	(WM_USER+1)


TCPManager::TCPManager()
{
	m_recvBytes = 0;
}


void TCPManager::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void TCPManager::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void TCPManager::SetUserInfo(UserInfo & userInfoSrc, UserInfo & userInfoDst)
{
	userInfoSrc.identifyKey = userInfoDst.identifyKey;
	userInfoSrc.m_ePlayerType = userInfoDst.m_ePlayerType;
	strcpy(userInfoSrc.szLevel, userInfoDst.szLevel);
	strcpy(userInfoSrc.szNickName, userInfoDst.szNickName);
	strcpy(userInfoSrc.szPosition, userInfoDst.szPosition);
	userInfoSrc.joinRoomSequence = userInfoDst.joinRoomSequence;
}




void TCPManager::Init()
{
	// ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		err_quit("WSAStartup() err!");

	// CompletionPort ����
	m_hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!m_hCp) exit(1);

	// ���� ����
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_Sock == INVALID_SOCKET)
	{
		err_quit("socket() err!");
	}

}

void TCPManager::ConnectToServer()
{
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	char opt_val = true;

	setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));

	// ���ϰ� CompletionPort ����
	CreateIoCompletionPort((HANDLE)m_Sock, m_hCp, 0, 0);


	// ���� ����
	int retval = connect(m_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect() err!");
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	ZeroMemory(ptr, sizeof(SOCKETINFO));

	ptr->sock = m_Sock;
	//ptr->overlapped.hEvent = WSACreateEvent();
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = sizeof(ptr->buf);

	DWORD dwNumRecv;
	if (WSARecv(m_Sock, &(ptr->wsabuf), 1, &dwNumRecv, &ptr->dwFlags, &ptr->overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete ptr;
			err_quit("WSARecv() err!!");
		}
	}
}

void TCPManager::Release()
{
	PACKET_PROGRAM_EXIT packet;
	packet.header.wIndex = PACKET_INDEX_PROGRAM_EXIT;
	packet.header.wLen = sizeof(PACKET_PROGRAM_EXIT);
	packet.identifyKey = 0;

	//������ ���� �˸�
	send(m_Sock, (const char*)&packet, sizeof(PACKET_PROGRAM_EXIT),0);

	//���� ����
	shutdown(m_Sock, SD_BOTH);
	closesocket(m_Sock);
	WSACleanup();
}

bool TCPManager::ProcessPacket(char * szBuf, int& recvBytes)
{
	// ó�� ������ ���ۿ� ���� or ���� ������ �����̱�
	if (recvBytes > 0)
	{
		memcpy(&m_szBuf[m_recvBytes], szBuf, recvBytes);
		m_recvBytes += recvBytes;
		recvBytes = 0;
	}

	// ���� �����Ͱ� ��� ��Ŷ ������ ũ�⺸�� ������ false
	if (m_recvBytes < sizeof(PACKET_HEADER))
		return false;

	// ��Ŷ�� ũ�⺸�� ���� ������ ũ�Ⱑ ������ false ���� - �����Ͱ� �� �� ���
	PACKET_HEADER header;
	memcpy(&header, m_szBuf, sizeof(PACKET_HEADER));
	if (header.wLen > m_recvBytes)
		return false;

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		//�α��ν� �г���, ����, ������ġ ����
		PACKET_LOGIN_RET packet;
		memcpy(&packet, m_szBuf, sizeof(PACKET_LOGIN_RET));
		GameManager::GetInstance()->SetPlayerInfo(packet.userInfo.szNickName, packet.userInfo.szLevel, packet.userInfo.szPosition);
	}
	break;
	case PACKET_INDEX_USER_LIST:
	{
		// ���� ���
		PACKET_USER_LIST packet;
		memcpy(&packet, m_szBuf, header.wLen);
		
		GameManager::GetInstance()->AddUserList(packet.userInfo.identifyKey, packet.userInfo.szNickName, packet.userInfo.szLevel, packet.userInfo.szPosition);
	}
	break;
	case PACKET_INDEX_LEAVE_CLIENT:
	{
		PACKET_LEAVE_CLIENT packet;
		memcpy(&packet, m_szBuf, header.wLen);

		GameManager::GetInstance()->RemoveUserList(packet.identifyKey);
	}
	break;
	case PACKET_INDEX_SEND_TEXTMESSAGE:
	{
		PACKET_SEND_TEXTMESSAGE packet;
		memcpy(&packet, m_szBuf, header.wLen);

		GameManager::GetInstance()->AddTextChatting(packet.userText);
	}
	break;
	case PACKET_INDEX_ROOM_LIST:
	{
		PACKET_ROOM_LIST packet;
		memcpy(&packet, m_szBuf, header.wLen);

		UIManager::GetInstance()->AddRoomList(packet.roomNumber, packet.roomName, packet.roomSize, packet.superVisorName);
	}
	break;
	case PACKET_INDEX_REMOVE_ROOM:
	{
		PACKET_REMOVE_ROOM packet;
		memcpy(&packet, m_szBuf, header.wLen);

		UIManager::GetInstance()->RemoveRoomList(packet.roomNumber);
	}
	break;
	case PACKET_INDEX_JOINROOM_USERLIST:
	{
		PACKET_JOINROOM_USERLIST packet;
		memcpy(&packet, m_szBuf, header.wLen);

		UserInfo userInfo;
		SetUserInfo(userInfo, packet.userInfo);

		m_dequeJoinRoomUserInfo.push_back(userInfo);

		if (packet.bIsEnd)
		{
			// ���ӷ� Player UI Refresh.
			UIManager::GetInstance()->RefreshJoinRoomUserList(m_dequeJoinRoomUserInfo);
			m_dequeJoinRoomUserInfo.clear();
		}
	}
	break;
	}


	// �����Ͱ� �� ���� ������ ( ��Ŷ 2���̻� �پ �°�� )
	if (m_recvBytes - header.wLen > 0)
	{
		memcpy(m_szBuf, &m_szBuf[header.wLen], m_recvBytes - header.wLen);
	}

	m_recvBytes -= header.wLen;

	return true;
}




void TCPManager::SendModifyPlayerTypeToServer(PLAYER_TYPE type)
{
	PACKET_MODIFY_PLAYERTYPE packet;
	packet.header.wIndex = PACKET_INDEX_MODIFY_PLAYERTYPE;
	packet.header.wLen = sizeof(PACKET_MODIFY_PLAYERTYPE);
	packet.type = (PLAYER_TYPE)GameManager::GetInstance()->GetPlayerCharacterType();
	send(m_Sock, (const char*)&packet, packet.header.wLen, 0);
}

void TCPManager::SendTextMessageToServer(char * sendMessage)
{
	PACKET_SEND_TEXTMESSAGE packet;
	packet.header.wIndex = PACKET_INDEX_SEND_TEXTMESSAGE;
	packet.header.wLen = sizeof(PACKET_HEADER) + strlen(sendMessage) + 1;
	strcpy(packet.userText, sendMessage);
	send(m_Sock, (char*)&packet, packet.header.wLen, 0);
}

void TCPManager::SendCreateRoomToServer(char * roomName)
{
	// ���ο� �� ����
	PACKET_CREATE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_CREATE_ROOM;
	packet.header.wLen = sizeof(PACKET_HEADER) + strlen(roomName) + 1;
	strcpy(packet.roomName, roomName);
	send(m_Sock, (const char*)&packet, packet.header.wLen, 0);
}

void TCPManager::SendJoinRoomToServer(int roomNumber)
{
	// ������� �濡 ����
	PACKET_JOIN_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_JOIN_ROOM;
	packet.header.wLen = sizeof(PACKET_JOIN_ROOM);
	packet.roomNumber = roomNumber;
	send(m_Sock, (const char*)&packet, packet.header.wLen, 0);
}

TCPManager::~TCPManager()
{
}
