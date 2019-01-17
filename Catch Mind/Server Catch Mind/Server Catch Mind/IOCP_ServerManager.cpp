#include "IOCP_ServerManager.h"
#include "../../Common/PACKET.h"
#include "ClientInfo.h"
#include "WaitingRoomManager.h"


IOCP_ServerManager*			IOCP_ServerManager::m_pThis = NULL;


IOCP_ServerManager::IOCP_ServerManager()
{
	m_iClientIndex = 0;
}

void IOCP_ServerManager::err_quit(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void IOCP_ServerManager::err_display(char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void IOCP_ServerManager::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}





void IOCP_ServerManager::Init()
{
	int retval;

	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
	{
		exit(1);
	}

	// 입출력 완료포트 생성
	m_hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hCp == NULL) exit(1);



	//socket()
	m_listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	int opt_val = TRUE;
	if (m_listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retval = bind(m_listen_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(m_listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


}

void IOCP_ServerManager::Release()
{
	// 윈속 종료
	closesocket(m_listen_sock);
	WSACleanup();

	delete m_pThis;
	m_pThis = NULL;
}

void IOCP_ServerManager::ProcessSocketMessage()
{
	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	int retval, addLen;
	DWORD recvBytes, flags;
	bool opt_val = true;
	while (1)
	{
		// accept()
		addLen = sizeof(clientAddr);
		client_sock = accept(m_listen_sock, (SOCKADDR*)&clientAddr, &addLen);
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		printf("[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트번호 %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// 소켓과 입출력 완료포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, m_hCp, client_sock, 0);


		// 소켓 정보 구조체 할당
		SOCKETINFO* ptr = new SOCKETINFO;
		if (ptr == NULL) break;
		ZeroMemory(ptr, sizeof(SOCKETINFO));

		ClientInfo* pClient = new ClientInfo;
		pClient->m_sock = client_sock;
		sprintf(pClient->m_szName, "익명%d", client_sock);
		pClient->m_iIndex = m_iClientIndex++;
		pClient->m_isSuperVisor = false;
		pClient->m_roomNumber = 0;
		pClient->m_JoinRoomSequence = 0;
		strcpy(pClient->m_szLevel, "룽");
		strcpy(pClient->m_szPosition, "대기실");
		m_mapClient.insert(std::make_pair(pClient->m_sock, pClient));

		PACKET_LOGIN_RET packet;
		packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
		packet.header.wLen = sizeof(PACKET_LOGIN_RET);

		packet.userInfo.identifyKey = client_sock;
		packet.userInfo.m_ePlayerType = PLAYER_TYPE_CHARACTER_ONION;
		strcpy(packet.userInfo.szNickName, pClient->m_szName);
		strcpy(packet.userInfo.szLevel, pClient->m_szLevel);
		strcpy(packet.userInfo.szPosition, pClient->m_szPosition);
		memcpy(ptr->buf, &packet, sizeof(packet));

		ptr->sock = client_sock;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = sizeof(PACKET_LOGIN_RET);



		retval = send(client_sock, ptr->wsabuf.buf, ptr->wsabuf.len, 0);
		printf("send 1 \n");
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				err_display("WSASend()");
			}
			continue;
		}


		// 유저 접속 목록 갱신
		int i = 0;
		PACKET_USER_LIST packetList;
		packetList.header.wIndex = PACKET_INDEX_USER_LIST;
		packetList.header.wLen = sizeof(PACKET_USER_LIST);
		packetList.userCount = m_mapClient.size();

		for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
		{
			packetList.userInfo.identifyKey = (*iter).first;
			strcpy(packetList.userInfo.szNickName, (*iter).second->m_szName);
			strcpy(packetList.userInfo.szLevel, (*iter).second->m_szLevel);
			strcpy(packetList.userInfo.szPosition, (*iter).second->m_szPosition);

			for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
			{
				send(iter->first, (const char*)&packetList, sizeof(PACKET_USER_LIST), 0);
				printf("send 2 \n");
			}
		}

		// 방 목록 갱신
		if (WaitingRoomManager::GetInstance()->GetRoomCount() > 0)
		{
			for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
			{
				WaitingRoomManager::GetInstance()->SendRoomListToClient(iter->first);
			}
		}


		// 비동기 입출력 시작
		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				err_display("WSARecv()");
			}
			continue;
		}
	}
}

bool IOCP_ServerManager::ProcessPacket(SOCKET& clientSock, char* szBuf, int& recvBytes)
{
	// 처음 데이터 버퍼에 복사 or 남은 데이터 덧붙이기
	if (recvBytes > 0)
	{
		memcpy(&m_mapClient[clientSock]->m_Buf[m_mapClient[clientSock]->m_recvBytes], szBuf, recvBytes);
		m_mapClient[clientSock]->m_recvBytes += recvBytes;
		recvBytes = 0;
	}

	// 받은 데이터가 헤더 패킷 데이터 크기보다 작으면 false
	if (m_mapClient[clientSock]->m_recvBytes < sizeof(PACKET_HEADER))
		return false;

	// 패킷의 크기보다 받은 데이터 크기가 작으면 false 리턴 - 데이터가 덜 온 경우
	PACKET_HEADER header;
	memcpy(&header, m_mapClient[clientSock]->m_Buf, sizeof(PACKET_HEADER));
	if (header.wLen > m_mapClient[clientSock]->m_recvBytes)
		return false;

	//패킷 처리
	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
	{
		// 클라이언트 로그인시 
		PACKET_LOGIN_RET packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, sizeof(PACKET_LOGIN_RET));

	}
	break;
	case PACKET_INDEX_MODIFY_PLAYERTYPE:
	{
		PACKET_MODIFY_PLAYERTYPE packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, sizeof(PACKET_MODIFY_PLAYERTYPE));
		m_mapClient[clientSock]->m_ePlayerType = packet.type;
	}
	break;
	case PACKET_INDEX_PROGRAM_EXIT:
	{
		// 클라이언트 종료 수신
		PACKET_PROGRAM_EXIT packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, sizeof(PACKET_PROGRAM_EXIT));

		m_mapClient[clientSock]->m_isLeave = true;
		short roomNumber = WaitingRoomManager::GetInstance()->LeaveRoom(m_mapClient[clientSock]->m_roomNumber, m_mapClient[clientSock]);

		if (roomNumber > 0)
		{
			// 방 제거 리스트 갱신
			for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
			{
				if (iter->first == clientSock)
					continue;
				WaitingRoomManager::GetInstance()->SendRemoveRoomList(roomNumber, iter->first);
			}
		}

	}
	break;
	case PACKET_INDEX_SEND_TEXTMESSAGE:
	{
		PACKET_SEND_TEXTMESSAGE packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, header.wLen);

		char szText[1025];
		strcpy(szText, packet.userText);

		for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
		{
			send(iter->first, (const char*)&packet, packet.header.wLen, 0);
		}
		//Sleep(1);
	}
	break;
	case PACKET_INDEX_CREATE_ROOM:
	{
		// 유저가 방 생성시
		PACKET_CREATE_ROOM packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, header.wLen);

		WaitingRoomManager::GetInstance()->CreateRoom(packet.roomName, clientSock, m_mapClient[clientSock]);

		// 대기실 - 방 리스트 갱신
		for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
		{
			WaitingRoomManager::GetInstance()->SendRoomListToClient(iter->first);
		}

		// 게임룸 - 유저목록 갱신
		WaitingRoomManager::GetInstance()->JoinRoomUserListInfo(clientSock, m_mapClient[clientSock]->m_roomNumber);
	}
	break;
	case PACKET_INDEX_JOIN_ROOM:
	{
		PACKET_JOIN_ROOM	packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, header.wLen);

		WaitingRoomManager::GetInstance()->JoinRoom(packet.roomNumber,m_mapClient[clientSock]);

		// 방안 유저목록 갱신
		WaitingRoomManager::GetInstance()->JoinRoomUserListInfo(clientSock, m_mapClient[clientSock]->m_roomNumber);

	}
	break;
	case PACKET_INDEX_MODIFY_USER:
	{
		PACKET_MODIFY_USER packet;
		memcpy(&packet, m_mapClient[clientSock]->m_Buf, header.wLen);

		strcpy(m_mapClient[clientSock]->m_szPosition, packet.szPosition);


	}
	break;
	}


	// 데이터가 더 많이 왔을때 ( 패킷 2개이상 붙어서 온경우 )
	if (m_mapClient[clientSock]->m_recvBytes - header.wLen > 0)
	{
		memcpy(m_mapClient[clientSock]->m_Buf, &m_mapClient[clientSock]->m_Buf[header.wLen], m_mapClient[clientSock]->m_recvBytes - header.wLen);
	}


	m_mapClient[clientSock]->m_recvBytes -= header.wLen;

	return true;
}

int IOCP_ServerManager::GetClientRecvBytes(SOCKET & clientSock)
{
	return m_mapClient[clientSock]->m_recvBytes;
}

bool IOCP_ServerManager::GetIsClientLeave(SOCKET & clientSock)
{
	return m_mapClient[clientSock]->m_isLeave;
}

void IOCP_ServerManager::LeaveClient(SOCKET & clientSock)
{
	delete m_mapClient[clientSock];
	m_mapClient[clientSock] = NULL;
	m_mapClient.erase(clientSock);

	PACKET_LEAVE_CLIENT packet;
	packet.header.wIndex = PACKET_INDEX_LEAVE_CLIENT;
	packet.header.wLen = sizeof(PACKET_LEAVE_CLIENT);
	packet.identifyKey = clientSock;

	int i = 1;
	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		send(iter->first, (const char*)&packet, sizeof(PACKET_LEAVE_CLIENT), 0);
		printf("clientLeave %d \n", i);
	}
}

IOCP_ServerManager::~IOCP_ServerManager()
{

}
