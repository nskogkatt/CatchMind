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




void TCPManager::Init()
{
	// 윈속 시작
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		err_quit("WSAStartup() err!");

	// CompletionPort 생성
	m_hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!m_hCp) exit(1);

	// 소켓 생성
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

	// 소켓과 CompletionPort 연결
	CreateIoCompletionPort((HANDLE)m_Sock, m_hCp, 0, 0);


	// 서버 접속
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

	//서버에 종료 알림
	send(m_Sock, (const char*)&packet, sizeof(PACKET_PROGRAM_EXIT),0);

	//소켓 종료
	shutdown(m_Sock, SD_BOTH);
	closesocket(m_Sock);
	WSACleanup();
}

bool TCPManager::ProcessPacket(char * szBuf, int& recvBytes)
{
	// 처음 데이터 버퍼에 복사 or 남은 데이터 덧붙이기
	if (recvBytes > 0)
	{
		memcpy(&m_szBuf[m_recvBytes], szBuf, recvBytes);
		m_recvBytes += recvBytes;
		recvBytes = 0;
	}

	// 받은 데이터가 헤더 패킷 데이터 크기보다 작으면 false
	if (m_recvBytes < sizeof(PACKET_HEADER))
		return false;

	// 패킷의 크기보다 받은 데이터 크기가 작으면 false 리턴 - 데이터가 덜 온 경우
	PACKET_HEADER header;
	memcpy(&header, m_szBuf, sizeof(PACKET_HEADER));
	if (header.wLen > m_recvBytes)
		return false;

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		//로그인시 닉네임, 레벨, 현재위치 지정
		PACKET_LOGIN_RET packet;
		memcpy(&packet, m_szBuf, sizeof(PACKET_LOGIN_RET));
		GameManager::GetInstance()->SetPlayerInfo(packet.userInfo.szNickName, packet.userInfo.szLevel, packet.userInfo.szPosition);
	}
	break;
	case PACKET_INDEX_USER_LIST:
	{
		// 유저 목록
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



	}
	break;
	}


	// 데이터가 더 많이 왔을때 ( 패킷 2개이상 붙어서 온경우 )
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
	packet.type = PLAYER_TYPE_CHARACTER_ONION;
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
	// 새로운 방 생성
	PACKET_CREATE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_CREATE_ROOM;
	packet.header.wLen = sizeof(PACKET_HEADER) + strlen(roomName) + 1;
	strcpy(packet.roomName, roomName);
	send(m_Sock, (const char*)&packet, packet.header.wLen, 0);
}

void TCPManager::SendJoinRoomToServer(int roomNumber)
{
	// 만들어진 방에 접속
	PACKET_JOIN_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_JOIN_ROOM;
	packet.header.wLen = sizeof(PACKET_JOIN_ROOM);
	packet.roomNumber = roomNumber;
	send(m_Sock, (const char*)&packet, packet.header.wLen, 0);
}

TCPManager::~TCPManager()
{
}
