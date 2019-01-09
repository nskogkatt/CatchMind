#pragma once
#include <Windows.h>
#include "../../Common/defineSize.h"



class ClientInfo
{
private:

public:	
	SOCKET				m_sock;

	int					m_iIndex;
	int					m_roomNumber;

	PLAYER_TYPE			m_ePlayerType;
	char				m_szName[NAMESIZE];
	char				m_szLevel[LEVELSZIE];
	char				m_szPosition[POSITIONSIZE];

	bool				m_isLeave;
	bool				m_isSuperVisor;
		
	//네트워크 데이터전송시 필요한 버퍼 및 버퍼크기
	char		m_Buf[BUFSIZE+1];
	int			m_recvBytes;


	ClientInfo();
	~ClientInfo();
};

