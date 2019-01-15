#pragma once
#include "../Common/defineSize.h"
#include <Windows.h>


#pragma pack(1)
enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET,
	PACKET_INDEX_MODIFY_PLAYERTYPE,
	PACKET_INDEX_USER_LIST,
	PACKET_INDEX_LEAVE_CLIENT,
	PACKET_INDEX_SEND_TEXTMESSAGE,
	PACKET_INDEX_CREATE_ROOM,
	PACKET_INDEX_JOIN_ROOM,
	PACKET_INDEX_REMOVE_ROOM,
	PACKET_INDEX_ROOM_LIST,
	PACKET_INDEX_JOINROOM_USERLIST,
	PACKET_INDEX_PROGRAM_EXIT,
	PACKET_INDEX_MODIFY_USER,
	PACKET_INDEX_END
};


struct PACKET_HEADER
{
	WORD 	wIndex;
	WORD	wLen;
};


struct PACKET_LOGIN_RET
{
	PACKET_HEADER	header;
	UserInfo		userInfo;
};

struct PACKET_MODIFY_PLAYERTYPE
{
	PACKET_HEADER	header;
	PLAYER_TYPE		type;
};

struct PACKET_USER_LIST
{
	PACKET_HEADER	header;
	WORD			userCount;
	UserInfo		userInfo;

};

struct PACKET_PROGRAM_EXIT
{
	PACKET_HEADER	header;
	int				identifyKey;
};

struct PACKET_LEAVE_CLIENT
{
	PACKET_HEADER	header;
	int				identifyKey;
};

struct PACKET_SEND_TEXTMESSAGE
{
	PACKET_HEADER	header;
	char			userText[1024];
};


struct PACKET_CREATE_ROOM
{
	PACKET_HEADER	header;
	char			roomName[ROOM_NAMESIZE];
};

struct PACKET_JOIN_ROOM
{
	PACKET_HEADER	header;
	short			roomNumber;
};

struct PACKET_REMOVE_ROOM
{
	PACKET_HEADER	header;
	short			roomNumber;
};

struct PACKET_ROOM_LIST
{
	PACKET_HEADER	header;
	short			roomNumber;
	char			roomName[ROOM_NAMESIZE];
	char			superVisorName[NAMESIZE];
	short			roomSize;
};

struct PACKET_JOINROOM_USERLIST
{
	PACKET_HEADER	header;
	UserInfo		userInfo;
	short			joinRoomSequence;
	bool			bIsEnd;
};

struct PACKET_MODIFY_USER
{
	PACKET_HEADER	header;
	char			szPosition[POSITIONSIZE];
};

#pragma pack()