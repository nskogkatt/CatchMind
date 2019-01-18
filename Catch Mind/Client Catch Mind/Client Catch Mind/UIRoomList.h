#pragma once
#include "Object.h"
#include "UI.h"



class UIRoomList : public Object
{
private:

	short			m_roomNumber;
	char			m_roomName[ROOM_NAMESIZE];
	char			m_superVisorName[NAMESIZE];
	short			m_headCount;
	short			m_roomMaxSize;
	
	bool			m_bIsSelected;

	LOGFONT			m_lf;
	HFONT			m_hFont;

public:

	virtual void Draw();
	virtual bool InputMouseLButtonDown(POINT& ptMouse);

	void InitRoomInfo(short roomNumber, char* roomName, char* superVisorName, short headCount, short roomMaxSize);
	
	void SetPositionY(int y);
	void SetSlectRoomList(bool set);
	short GetRoomNumber() const;
	short GetHeadCount() const;

	UIRoomList();
	~UIRoomList();
};

