#include "UI.h"
#include "Resource.h"
#include "Bitmap.h"
#include "GameManager.h"
#include "UIManager.h"
#include "UIRoomList.h"

void UI::Init(UI_TYPE eType, Bitmap ** pBitmap, RECT rcRect)
{
	m_eType = eType;

	m_pBitmap[UI_TYPE_SELECT_CHARACTER] = pBitmap[RES_TYPE_SELECT_CHARACTER];
	m_pBitmap[UI_TYPE_WAITING_ROOM] = pBitmap[RES_TYPE_WAITING_ROOM];
	m_pBitmap[UI_TYPE_WAITING_ROOM_INFO] = pBitmap[RES_TYPE_WAITING_ROOM_INFO];
	m_pBitmap[UI_TYPE_WAITING_ROOM_INGAME] = pBitmap[RES_TYPE_WAITING_ROOM_INGAME];
	m_pBitmap[UI_TYPE_CREATE_ROOM] = pBitmap[RES_TYPE_CREATE_ROOM];

	m_pBitmap[UI_TYPE_BUTTON_JOIN] = pBitmap[RES_TYPE_BUTTON_JOIN];
	m_pBitmap[UI_TYPE_BUTTON_EXIT] = pBitmap[RES_TYPE_BUTTON_EXIT];
	m_pBitmap[UI_TYPE_BUTTON_CHANGECHENNEL] = pBitmap[RES_TYPE_BUTTON_CHANGECHENNEL];
	m_pBitmap[UI_TYPE_BUTTON_CREATEROOM] = pBitmap[RES_TYPE_BUTTON_CREATEROOM];
	m_pBitmap[UI_TYPE_BUTTON_JOINROOM] = pBitmap[RES_TYPE_BUTTON_JOINROOM];
	m_pBitmap[UI_TYPE_BUTTON_VISITORINFORMATION] = pBitmap[RES_TYPE_BUTTON_VISITORINFORMATION];
	m_pBitmap[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE] = pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_EXCUTE];
	m_pBitmap[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE] = pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_CANCLE];

	m_pBitmap[UI_TYPE_CHARACTER_BANANA] = pBitmap[RES_TYPE_CHARACTER_BANANA];
	m_pBitmap[UI_TYPE_CHARACTER_BEANCAKE] = pBitmap[RES_TYPE_CHARACTER_BEANCAKE];
	m_pBitmap[UI_TYPE_CHARACTER_DONG] = pBitmap[RES_TYPE_CHARACTER_DONG];
	m_pBitmap[UI_TYPE_CHARACTER_EGG] = pBitmap[RES_TYPE_CHARACTER_EGG];
	m_pBitmap[UI_TYPE_CHARACTER_EGGHALF] = pBitmap[RES_TYPE_CHARACTER_EGGHALF];
	m_pBitmap[UI_TYPE_CHARACTER_GARLIC] = pBitmap[RES_TYPE_CHARACTER_GARLIC];
	m_pBitmap[UI_TYPE_CHARACTER_GRAPE] = pBitmap[RES_TYPE_CHARACTER_GRAPE];
	m_pBitmap[UI_TYPE_CHARACTER_KIMCHI] = pBitmap[RES_TYPE_CHARACTER_KIMCHI];
	m_pBitmap[UI_TYPE_CHARACTER_LETTUCE] = pBitmap[RES_TYPE_CHARACTER_LETTUCE];
	m_pBitmap[UI_TYPE_CHARACTER_MUSHROOM] = pBitmap[RES_TYPE_CHARACTER_MUSHROOM];
	m_pBitmap[UI_TYPE_CHARACTER_ONION] = pBitmap[RES_TYPE_CHARACTER_ONION];
	m_pBitmap[UI_TYPE_CHARACTER_ORANGE] = pBitmap[RES_TYPE_CHARACTER_ORANGE];
	m_pBitmap[UI_TYPE_CHARACTER_PEPPER] = pBitmap[RES_TYPE_CHARACTER_PEPPER];
	m_pBitmap[UI_TYPE_CHARACTER_RADISH] = pBitmap[RES_TYPE_CHARACTER_RADISH];
	m_pBitmap[UI_TYPE_CHARACTER_RICEROCK] = pBitmap[RES_TYPE_CHARACTER_RICEROCK];
	m_pBitmap[UI_TYPE_CHARACTER_SALAD] = pBitmap[RES_TYPE_CHARACTER_SALAD];
	m_pBitmap[UI_TYPE_CHARACTER_TOMATO] = pBitmap[RES_TYPE_CHARACTER_TOMATO];

	if (m_eType != UI_TYPE_USER_LIST && m_eType != UI_TYPE_ROOM_LIST)
		SetRect(&m_rcRect, rcRect.left, rcRect.top,
			rcRect.left + m_pBitmap[m_eType]->GetBitmapSize().cx, rcRect.top + m_pBitmap[m_eType]->GetBitmapSize().cy);
}

void UI::Draw()
{
	if (m_bLive == false)
		return;

	if (m_eType == UI_TYPE_USER_LIST)
	{
		// WaitingRoom - 접속자 목록
		int y = 0;
		const map<int, UserInfo*>&	mapUserList = UIManager::GetInstance()->GetUserListMap();
		for (auto iter = mapUserList.begin(); iter != mapUserList.end(); iter++)
		{
			Bitmap::DrawTextOut(m_hFont, m_rcRect.left, m_rcRect.top + y, (*iter).second->szNickName, RGB(0, 0, 0), RGB(255, 255, 255), TA_LEFT);
			Bitmap::DrawTextOut(m_hFont, m_rcRect.left + 100, m_rcRect.top + y, (*iter).second->szLevel, RGB(0, 0, 0), RGB(255, 255, 255), TA_LEFT);
			Bitmap::DrawTextOut(m_hFont, m_rcRect.left + 150, m_rcRect.top + y, (*iter).second->szPosition, RGB(0, 0, 0), RGB(255, 255, 255), TA_LEFT);
			y += 15;
		}
		return;
	}

	if (m_eType == UI_TYPE_ROOM_LIST)
	{
		//WaitingRoom - 방 리스트
		int y = 145;
		const map<int, UIRoomList*>&	mapRoomList = UIManager::GetInstance()->GetRoomListMap();
		for (auto iter = mapRoomList.begin(); iter != mapRoomList.end(); iter++, y += 15)
		{
			(*iter).second->SetPositionY(y);
			(*iter).second->Draw();
		}
		return;
	}

	m_pBitmap[m_eType]->Draw(m_rcRect.left, m_rcRect.top);

	if (m_eType == UI_TYPE_WAITING_ROOM_INFO)
	{
		m_pBitmap[m_eType]->DrawTextOut(m_hFont, m_rcRect.left + 200, m_rcRect.top + 30, GameManager::GetInstance()->GetPlayerNickName(), RGB(0, 0, 0), RGB(206, 206, 206), TA_CENTER);
		m_pBitmap[m_eType]->DrawTextOut(m_hFont, m_rcRect.left + 200, m_rcRect.top + 80, "0", RGB(0, 0, 0), RGB(206, 206, 206), TA_CENTER);
		m_pBitmap[m_eType]->DrawTextOut(m_hFont, m_rcRect.left + 200, m_rcRect.top + 105, "0", RGB(0, 0, 0), RGB(206, 206, 206), TA_CENTER);
	}

	if (m_eType >= UI_TYPE_CHARACTER_BANANA && m_eType <= UI_TYPE_CHARACTER_TOMATO)
	{
		m_pBitmap[m_eType]->DrawTextOut(m_hFont, m_rcRect.left + 25, m_rcRect.top + 80, m_szTypeName, RGB(255, 41, 247), RGB(255, 255, 255), TA_CENTER);
	}


	if (GameManager::GetInstance()->GetGameState() == GAME_STATE_WAITING_ROOM && m_eType == UI_TYPE_WAITING_ROOM)
	{
		int iCount = 0, y = 680;
		const vector<string>& vecStrChattingText = UIManager::GetInstance()->GetChatHistory();
		for (auto iter = vecStrChattingText.rbegin(); iter != vecStrChattingText.rend(); iter++, iCount++)
		{
			m_pBitmap[m_eType]->DrawTextOut(m_hFont, 62, y -= 15, (char*)(*iter).c_str(), RGB(0, 0, 0), RGB(255, 255, 255), TA_LEFT);
			if (iCount > 8)
				break;
		}
	}
}

bool UI::InputMouseLButtonDown(POINT& ptMouse)
{
	char szTemp[NAMESIZE];
	int tempType;

	if (m_eType == UI_TYPE_SELECT_CHARACTER || m_eType == UI_TYPE_WAITING_ROOM)
		return false;

	if (PtInRect(&m_rcRect, ptMouse))
	{
		// SelectCharacter에서 Character 클릭시
		if (GameManager::GetInstance()->GetGameState() == GAME_STATE_SELECT_CHARACTER)
		{
			if (m_eType >= UI_TYPE_CHARACTER_BANANA && m_eType <= UI_TYPE_CHARACTER_TOMATO)
			{
				//캐릭터 변경
				strcpy(szTemp, GameManager::GetInstance()->GetPlayerCharacterName());
				tempType = GameManager::GetInstance()->GetPlayerCharacterType();
				GameManager::GetInstance()->SetPlayerCharacter(m_eType, m_szTypeName);

				strcpy(m_szTypeName, szTemp);
				m_eType = (UI_TYPE)tempType;
				return true;
			}
			else if (m_eType == UI_TYPE_BUTTON_JOIN)
			{
				// 대기실 입장

				GameManager::GetInstance()->JoinWaittingRoom();

				return true;
			}
			else if (m_eType == UI_TYPE_BUTTON_EXIT)
			{
				// 종료
				exit(1);
				return true;
			}
		}

		// Waiting room 에서 클릭시
		if (GameManager::GetInstance()->GetGameState() == GAME_STATE_WAITING_ROOM)
		{
			if (m_eType == UI_TYPE_BUTTON_CREATEROOM)
			{
				// 방 생성시
				UIManager::GetInstance()->ShowCreateRoomWindowPop();
			}

			if (m_eType == UI_TYPE_BUTTON_JOINROOM)
			{
				// 방 입장시
				GameManager::GetInstance()->JoinGameRoom();
			}

			if (m_eType == UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE)
			{
				//방생성 실행버튼 클릭
				GameManager::GetInstance()->ExecuteCreateRoom();
			}

			if (m_eType == UI_TYPE_BUTTON_CREATE_ROOM_CANCLE)
			{
				//방생성 취소
				UIManager::GetInstance()->HideCreateRoomWindowPop();
			}

			if (m_eType == UI_TYPE_USER_LIST)
			{
				// 대기실 방목록 클릭시
				
			}
		}
	}

	return false;
}

void UI::SetPosition(RECT rcRect)
{
	if (m_eType != UI_TYPE_USER_LIST && m_eType != UI_TYPE_ROOM_LIST)
		SetRect(&m_rcRect, rcRect.left, rcRect.top, rcRect.left + m_pBitmap[m_eType]->GetBitmapSize().cx, rcRect.top + m_pBitmap[m_eType]->GetBitmapSize().cy);
	else
		SetRect(&m_rcRect, rcRect.left, rcRect.top, 0, 0);
}

void UI::SetTypeName(char * szName)
{
	strcpy(m_szTypeName, szName);
}

UI::UI()
{
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
	strcpy(m_lf.lfFaceName, "굴림체");
	m_hFont = CreateFontIndirect(&m_lf);
}


UI::~UI()
{
	DeleteObject(m_hFont);
}
