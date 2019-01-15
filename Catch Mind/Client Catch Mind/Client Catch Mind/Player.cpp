#include "Player.h"
#include "Bitmap.h"


void Player::Init(PLAYER_TYPE eType, Bitmap ** pBitmap, RECT rcRect)
{
	m_identifyKey = 0;
	m_eType = eType;
	strcpy(m_szTypeName, "¾çÆÄ·î");

	m_pBitmap[PLAYER_TYPE_CHARACTER_BANANA] = pBitmap[RES_TYPE_CHARACTER_BANANA];
	m_pBitmap[PLAYER_TYPE_CHARACTER_BEANCAKE] = pBitmap[RES_TYPE_CHARACTER_BEANCAKE];
	m_pBitmap[PLAYER_TYPE_CHARACTER_DONG] = pBitmap[RES_TYPE_CHARACTER_DONG];
	m_pBitmap[PLAYER_TYPE_CHARACTER_EGG] = pBitmap[RES_TYPE_CHARACTER_EGG];
	m_pBitmap[PLAYER_TYPE_CHARACTER_EGGHALF] = pBitmap[RES_TYPE_CHARACTER_EGGHALF];
	m_pBitmap[PLAYER_TYPE_CHARACTER_GARLIC] = pBitmap[RES_TYPE_CHARACTER_GARLIC];
	m_pBitmap[PLAYER_TYPE_CHARACTER_GRAPE] = pBitmap[RES_TYPE_CHARACTER_GRAPE];
	m_pBitmap[PLAYER_TYPE_CHARACTER_KIMCHI] = pBitmap[RES_TYPE_CHARACTER_KIMCHI];
	m_pBitmap[PLAYER_TYPE_CHARACTER_LETTUCE] = pBitmap[RES_TYPE_CHARACTER_LETTUCE];
	m_pBitmap[PLAYER_TYPE_CHARACTER_MUSHROOM] = pBitmap[RES_TYPE_CHARACTER_MUSHROOM];
	m_pBitmap[PLAYER_TYPE_CHARACTER_ONION] = pBitmap[RES_TYPE_CHARACTER_ONION];
	m_pBitmap[PLAYER_TYPE_CHARACTER_ORANGE] = pBitmap[RES_TYPE_CHARACTER_ORANGE];
	m_pBitmap[PLAYER_TYPE_CHARACTER_PEPPER] = pBitmap[RES_TYPE_CHARACTER_PEPPER];
	m_pBitmap[PLAYER_TYPE_CHARACTER_RADISH] = pBitmap[RES_TYPE_CHARACTER_RADISH];
	m_pBitmap[PLAYER_TYPE_CHARACTER_RICEROCK] = pBitmap[RES_TYPE_CHARACTER_RICEROCK];
	m_pBitmap[PLAYER_TYPE_CHARACTER_SALAD] = pBitmap[RES_TYPE_CHARACTER_SALAD];
	m_pBitmap[PLAYER_TYPE_CHARACTER_TOMATO] = pBitmap[RES_TYPE_CHARACTER_TOMATO];

	SetRect(&m_rcRect, rcRect.left, rcRect.top, rcRect.left + m_pBitmap[m_eType]->GetBitmapSize().cx, rcRect.top + m_pBitmap[m_eType]->GetBitmapSize().cy);
}

void Player::Draw()
{
	m_pBitmap[m_eType]->Draw(m_rcRect.left, m_rcRect.top);
}

void Player::SetPositionPlayer(RECT rcRect)
{
	SetRect(&m_rcRect, rcRect.left, rcRect.top, m_pBitmap[m_eType]->GetBitmapSize().cx, m_pBitmap[m_eType]->GetBitmapSize().cy);
}

void Player::SetPlayerCharacter(int eType, char* szName)
{
	m_eType = (PLAYER_TYPE)eType;
	strcpy(m_szTypeName, szName);
}

void Player::SetPlayerInfo(char * szNickName, char* szLevel, char* szPosition)
{
	strcpy(m_szNickName, szNickName);
	strcpy(m_szLevel, szLevel);
	strcpy(m_szPosition, szPosition);
}

void Player::SetPlayerInfo(UserInfo & userInfo)
{
	m_identifyKey = userInfo.identifyKey;
	m_eType = userInfo.m_ePlayerType;
	strcpy(m_szLevel, userInfo.szLevel);
	strcpy(m_szNickName, userInfo.szNickName);
	strcpy(m_szPosition, userInfo.szPosition);
}



PLAYER_TYPE Player::GetPlayerType() const
{
	return m_eType;
}

void Player::CreateRoom(char * roomName)
{
	RECT	rcRect;
	SetRect(&rcRect, 139, 164, 0, 0 );
	SetPositionPlayer(rcRect);
	strcpy(m_szRoomName, roomName);
	m_isSuperVisor = true;
}

char * Player::GetNickName()
{
	return m_szNickName;
}

Player::Player()
{
}


Player::~Player()
{
}
