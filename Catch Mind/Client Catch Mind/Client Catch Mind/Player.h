#pragma once
#include "Object.h"
#include "../../Common/defineSize.h"


class Player :
	public Object
{
private:
	Bitmap*				m_pBitmap[PLAYER_TYPE_END];

	PLAYER_TYPE			m_eType;
	int					m_identifyKey;
	short				m_joinRoomSequence;
	char				m_szTypeName[NAMESIZE];		//캐릭터명
	char				m_szNickName[NAMESIZE];
	char				m_szLevel[LEVELSZIE];
	char				m_szPosition[POSITIONSIZE];
	char				m_szRoomName[ROOM_NAMESIZE];

	bool				m_isSuperVisor;

public:
	virtual void Init(PLAYER_TYPE eType, Bitmap** pBitmap, RECT rcRect);
	virtual void Draw();


	void SetPositionPlayer(RECT rcRect);
	void SetPlayerCharacter(int eType, char* szName);
	void SetPlayerInfo(char * szNickName, char* szLevel, char* szPosition);
	void SetPlayerInfo(UserInfo& userInfo);
	PLAYER_TYPE GetPlayerType() const;

	void CreateRoom(char* roomName);

	char* GetNickName();
	char* GetTypeName()
	{
		return m_szTypeName;
	}
	int GetType()
	{
		return (int)m_eType;
	}

	Player();
	virtual ~Player();
};

