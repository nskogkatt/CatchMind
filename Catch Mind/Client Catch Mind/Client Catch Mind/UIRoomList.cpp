#include "UIRoomList.h"
#include "Bitmap.h"
#include "UIManager.h"

#define ROOMNUMBER_X	10
#define	ROOMTITLE_X		110
#define SUPERVIOSR_X	300
#define HEADCOUNT_X		400	




void UIRoomList::Draw()
{
	COLORREF textColor = RGB(0,0,0), bkColor = RGB(255,255,255);
	if (m_bIsSelected)
	{
		textColor = RGB(255, 255, 255);
		bkColor = RGB(51, 153, 255);
		Bitmap::DrawTextOut(m_hFont, m_rcRect.left, m_rcRect.top, "                                                                                               ",
			RGB(0, 0, 0), RGB(51, 153, 255), TA_LEFT);
	}
		
	Bitmap::DrawTextOut(m_hFont, m_rcRect.left + ROOMNUMBER_X, m_rcRect.top, (char*)std::to_string(m_roomNumber).c_str(), textColor, bkColor, TA_LEFT);
	Bitmap::DrawTextOut(m_hFont, m_rcRect.left + ROOMTITLE_X, m_rcRect.top, m_roomName, textColor, bkColor, TA_LEFT);
	Bitmap::DrawTextOut(m_hFont, m_rcRect.left + SUPERVIOSR_X, m_rcRect.top, m_superVisorName, textColor, bkColor, TA_LEFT);
	Bitmap::DrawTextOut(m_hFont, m_rcRect.left + HEADCOUNT_X, m_rcRect.top, (char*)std::to_string(m_headCount).c_str(), textColor, bkColor, TA_LEFT);
	
}

bool UIRoomList::InputMouseLButtonDown(POINT & ptMouse)
{
	if (PtInRect(&m_rcRect, ptMouse))
	{
		UIManager::GetInstance()->IsSelectedRoomList(m_roomNumber, m_bIsSelected);
		m_bIsSelected = true;
		return true;
	}
	return false;
}

void UIRoomList::InitRoomInfo(short roomNumber, char * roomName, char * superVisorName, short headCount, short roomMaxSize)
{
	m_roomNumber = roomNumber;
	strcpy(m_roomName, roomName);
	strcpy(m_superVisorName, superVisorName);
	m_headCount = headCount;
	m_roomMaxSize = roomMaxSize;
}

void UIRoomList::SetPositionY(int y)
{
	m_rcRect.top = y;
	m_rcRect.bottom = m_rcRect.top + 15;
	
}

void UIRoomList::SetSlectRoomList(bool bSelect)
{
	m_bIsSelected = bSelect;
}

short UIRoomList::GetRoomNumber() const
{
	return m_roomNumber;
}


UIRoomList::UIRoomList()
{
	m_bIsSelected = false;

	m_rcRect.left = 55;
	m_rcRect.right = 600;
	m_rcRect.top = 0;
	m_rcRect.bottom = 15;

	m_lf.lfHeight = 12;
	m_lf.lfWidth = 0;
	m_lf.lfEscapement = 0;
	m_lf.lfOrientation = 0;
	m_lf.lfWeight = 0;
	m_lf.lfItalic = 0;
	m_lf.lfUnderline = 0;
	m_lf.lfStrikeOut = 0;
	m_lf.lfCharSet = HANGEUL_CHARSET;
	m_lf.lfOutPrecision = 0;
	m_lf.lfClipPrecision = 0;
	m_lf.lfQuality = 0;
	m_lf.lfPitchAndFamily = 0;
	strcpy(m_lf.lfFaceName, "±¼¸²Ã¼");
	m_hFont = CreateFontIndirect(&m_lf);
}


UIRoomList::~UIRoomList()
{
	DeleteObject(m_hFont);
}
