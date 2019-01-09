#include "ClientInfo.h"



ClientInfo::ClientInfo()
{
	m_recvBytes = 0;
	m_isSuperVisor = false;
	m_roomNumber = 0;
	strcpy(m_szName, "");
	strcpy(m_szLevel, "");
	strcpy(m_szPosition, "");
	m_isLeave = false;
}


ClientInfo::~ClientInfo()
{
}
