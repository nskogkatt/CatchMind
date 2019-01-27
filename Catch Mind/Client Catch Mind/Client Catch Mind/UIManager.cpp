#include "UIManager.h"
#include "UI.h"
#include "ResManager.h"
#include "GameManager.h"
#include "UIRoomList.h"
#include "Player.h"



UIManager*		UIManager::m_pThis = NULL;


UIManager::UIManager()
{
}


void UIManager::Init(HWND hWnd, HINSTANCE hInstance, LONG EditSubProc)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	m_lEditSubProc = EditSubProc;
	m_prevSelectRoomList = m_mapRoomList.end();

	CreateUISelectCharacter();
	CreateUIWaitingRoom();
	CreateUIWaitingRoomInGame();
	CreateUIJoinRoomUserList();
}

void UIManager::Draw()
{
	for (auto iter = m_mapUIObjects.begin(); iter != m_mapUIObjects.end(); iter++)
	{
		iter->second->Draw();
	}

	//¹æ Á¢¼Ó½Ã À¯Àú¸ñ·Ï °»½Å
	if (GameManager::GetInstance()->GetGameState() == GAME_STATE_JOIN_ROOM)
		for (auto iter = m_mapJoinRoomUserList.begin(); iter != m_mapJoinRoomUserList.end(); iter++)
		{
			iter->second->Draw();
		}
}

void UIManager::Release()
{
	delete m_pThis;
	m_pThis = NULL;

	for (auto iter = m_mapUIObjects.begin(); iter != m_mapUIObjects.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}
	m_mapUIObjects.clear();

	for (int i = 0; i < EDIT_TYPE_END; i++)
	{
		if (m_hEdit[i] != NULL)
		{
			DestroyWindow(m_hEdit[i]);
			m_hEdit[i] = NULL;
		}
	}

}

bool UIManager::InputMouseLButtonDown(POINT & ptMouse)
{
	for (auto iter = m_mapUIObjects.begin(); iter != m_mapUIObjects.end(); iter++)
	{
		if (iter->second->InputMouseLButtonDown(ptMouse))
			return true;
	}

	for (auto iter = m_mapRoomList.begin(); iter != m_mapRoomList.end(); iter++)
	{
		if (iter->second->InputMouseLButtonDown(ptMouse))
			return true;
	}
	if (m_prevSelectRoomList != m_mapRoomList.end())
	{
		m_prevSelectRoomList->second->SetSlectRoomList(false);
		m_prevSelectRoomList = m_mapRoomList.end();
	}

	return false;
}

bool UIManager::IsSelectedRoomList(short roomNumber, bool isSelected)
{
	if (m_prevSelectRoomList != m_mapRoomList.end())
		m_prevSelectRoomList->second->SetSlectRoomList(false);

	m_prevSelectRoomList = m_mapRoomList.find(roomNumber);
	return !isSelected;
}

void UIManager::CreateUIWaitingRoom()
{
	UI* pUI;
	RECT rcRect;

	// WaitingRoom - BackGround
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_WAITING_ROOM, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_WAITING_ROOM, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_WAITING_ROOM_INFO, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_WAITING_ROOM_INFO, pUI));

	// WaitingRoom - Create_Room Ã¢
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CREATE_ROOM, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CREATE_ROOM, pUI));


	// WaitingRoom - UserList
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_USER_LIST, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_USER_LIST, pUI));

	// WaitingRoom - RoomList
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_ROOM_LIST, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_ROOM_LIST, pUI));

	//===============================================================================================

	// WaitingRoom - Button_CHANGECHENNEL
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_CHANGECHENNEL, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_CHANGECHENNEL, pUI));


	// WaitingRoom - Button_CreateRoom
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_CREATEROOM, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_CREATEROOM, pUI));


	// WaitingRoom - Button_JoinRoom
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_JOINROOM, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_JOINROOM, pUI));

	// WaitingRoom - BUTTON_VISITORINFORMATION
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_VISITORINFORMATION, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_VISITORINFORMATION, pUI));

	// WaitingRoom - CreateRoom - Button_Excute
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE, pUI));

	// WaitingRoom - CreateRoom - Button_Cancle
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_CREATE_ROOM_CANCLE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_CREATE_ROOM_CANCLE, pUI));


	m_hEdit[EDIT_TYPE_WAITING_ROOM_CHATTING] = CreateWindowEx(0, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 55, 690, 525, 25, m_hWnd, (HMENU)IDC_CHATTING, m_hInstance, NULL);
	m_hOldEdit[EDIT_TYPE_WAITING_ROOM_CHATTING] = (WNDPROC)SetWindowLong(m_hEdit[EDIT_TYPE_WAITING_ROOM_CHATTING], GWL_WNDPROC, m_lEditSubProc);
	ShowWindow(m_hEdit[EDIT_TYPE_WAITING_ROOM_CHATTING], SW_HIDE);

	m_hEdit[EDIT_TYPE_CREATE_ROOMNAME] = CreateWindowEx(0, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 185, 145, 250, 25, m_hWnd, (HMENU)IDC_ROOM_NAME, m_hInstance, NULL);
	ShowWindow(m_hEdit[EDIT_TYPE_CREATE_ROOMNAME], SW_HIDE);

	m_hEdit[EDIT_TYPE_CREATE_PASSWORD] = CreateWindowEx(0, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 185, 203, 100, 25, m_hWnd, (HMENU)IDC_PASSWORD, m_hInstance, NULL);
	ShowWindow(m_hEdit[EDIT_TYPE_CREATE_PASSWORD], SW_HIDE);



}

void UIManager::CreateUIWaitingRoomInGame()
{
	// WaitingRoomInGame BackGround
	UI* pUI;
	RECT rcRect;

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_WAITING_ROOM_INGAME, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_WAITING_ROOM_INGAME, pUI));

	m_hEdit[EDIT_TYPE_WAITING_ROOM_INGAME_CHATTING] = CreateWindowEx(0, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		ES_AUTOHSCROLL, 495, 607, 225, 25, m_hWnd, (HMENU)IDC_CHATTING, m_hInstance, NULL);
	ShowWindow(m_hEdit[EDIT_TYPE_WAITING_ROOM_INGAME_CHATTING], SW_HIDE);


}

void UIManager::CreateUIJoinRoomUserList()
{
	Player*		pPlayer;
	RECT		rcRect;

	for (int i = 1; i < MAX_HEADCOUNT; i++)
	{
		pPlayer = new Player;
		SetRect(&rcRect, 2000, 0, 0, 0);
		pPlayer->Init(PLAYER_TYPE_CHARACTER_ONION, ResManager::GetInstance()->GetBitmap(), rcRect);
		m_mapJoinRoomUserList.insert(make_pair(i, pPlayer));
	}
}

void UIManager::ShowCreateRoomWindowPop()
{
	RECT rcRect;
	int x = 100, y = 100;

	SetRect(&rcRect, x, y, 0, 0);
	m_mapUIObjects[UI_TYPE_CREATE_ROOM]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CREATE_ROOM]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_CREATE_ROOM]->SetPosition(rcRect);

	SetRect(&rcRect, x + 100, y + 203, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->SetPosition(rcRect);

	SetRect(&rcRect, x + 185, y + 203, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE]->SetPosition(rcRect);



	if (m_hEdit[EDIT_TYPE_CREATE_ROOMNAME] != NULL)
	{
		ShowWindow(m_hEdit[EDIT_TYPE_CREATE_ROOMNAME], SW_SHOW);
	}

	if (m_hEdit[EDIT_TYPE_CREATE_PASSWORD] != NULL)
	{
		ShowWindow(m_hEdit[EDIT_TYPE_CREATE_PASSWORD], SW_SHOW);
	}

}

void UIManager::HideCreateRoomWindowPop()
{
	m_mapUIObjects[UI_TYPE_CREATE_ROOM]->SetLiveObject(false);
	m_mapUIObjects[UI_TYPE_CREATE_ROOM]->SetTypeName("");

	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->SetLiveObject(false);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->SetTypeName("");

	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE]->SetLiveObject(false);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATE_ROOM_CANCLE]->SetTypeName("");

	ShowWindow(m_hEdit[EDIT_TYPE_CREATE_ROOMNAME], SW_HIDE);
	ShowWindow(m_hEdit[EDIT_TYPE_CREATE_PASSWORD], SW_HIDE);
}

short UIManager::GetSelectedRoomNumber()
{
	if (m_prevSelectRoomList != m_mapRoomList.end())
	{
		return m_prevSelectRoomList->second->GetRoomNumber();
	}

	return 0;
}

short UIManager::GetSelectedHeadCount()
{
	if (m_prevSelectRoomList != m_mapRoomList.end())
	{
		return m_prevSelectRoomList->second->GetHeadCount();
	}

	return 0;
}


void UIManager::RefreshUserList(deque<UserInfo>& dequeUserInfo)
{
	UserInfo userInfo;

	while (!dequeUserInfo.empty())
	{
		userInfo = dequeUserInfo.front();
		dequeUserInfo.pop_front();

		auto iter = m_mapUserList.insert(std::make_pair(userInfo.identifyKey, nullptr));

		if (iter.second)
		{
			//Áßº¹µÈ Å°°ª ¾øÀÌ insert ¼º°ø½Ã
			iter.first->second = new UserInfo;
			iter.first->second->identifyKey = userInfo.identifyKey;
			strcpy(iter.first->second->szNickName, userInfo.szNickName);
			strcpy(iter.first->second->szLevel, userInfo.szLevel);
			strcpy(iter.first->second->szPosition, userInfo.szPosition);
		}
		else
		{
			//MessageBox(NULL, "Add User Failed..", "Caution!", MB_OK);
		}
	}


}

void UIManager::RemoveUserList(int nIdentifyKey)
{
	delete m_mapUserList[nIdentifyKey];
	m_mapUserList[nIdentifyKey] = nullptr;
	m_mapUserList.erase(nIdentifyKey);
}

void UIManager::RefreshRoomList(deque<RoomInfo>&	dequeRoomList)
{
	RoomInfo	roomInfo;
	
	while (!dequeRoomList.empty())
	{
		roomInfo = dequeRoomList.front();
		dequeRoomList.pop_front();

		auto iter = m_mapRoomList.insert(make_pair(roomInfo.roomNumber, nullptr));

		if (iter.second)
		{
			UIRoomList* pRoomList = new UIRoomList;
			pRoomList->InitRoomInfo(roomInfo.roomNumber, roomInfo.roomName, roomInfo.superVisorName, roomInfo.roomSize, ROOM_MAX_SIZE);

			iter.first->second = pRoomList;
		}
		else
		{
			//MessageBox(NULL, "Create Room Failed..", "Caution!", MB_OK);
		}
	}
}

void UIManager::RemoveRoomList()
{
	for (auto iter = m_mapRoomList.begin(); iter != m_mapRoomList.end(); iter++)
	{
		
	}
}

void UIManager::RemoveRoomList(int nRoomNumber)
{
	delete m_mapRoomList[nRoomNumber];
	m_mapRoomList[nRoomNumber] = nullptr;
	m_mapRoomList.erase(nRoomNumber);
}

void UIManager::RefreshJoinRoomUserList(deque<UserInfo>& dequeUserInfo)
{
	UserInfo userInfo;
	RECT rcRect;

	while (!dequeUserInfo.empty())
	{
		userInfo = dequeUserInfo.front();
		dequeUserInfo.pop_front();

		m_mapJoinRoomUserList[userInfo.joinRoomSequence]->SetLiveObject(true);
		m_mapJoinRoomUserList[userInfo.joinRoomSequence]->SetPlayerInfo(userInfo);
		// ÇÃ·¹ÀÌ¾î À§Ä¡, ºÐ¹è, µå·Î¿ì ¸¸µé¾î¾ßµÊ.
	}
}



void UIManager::AddChattingText(string str)
{
	m_vStrChattingText.push_back(str);
}

HWND UIManager::GetEditHandle(EDIT_TYPE type)
{
	return m_hEdit[type];
}

WNDPROC UIManager::GetEditOldHandle(EDIT_TYPE type)
{
	return m_hOldEdit[type];
}

const map<int, UserInfo*>& UIManager::GetUserListMap() const
{
	return m_mapUserList;
}

const map<int, UIRoomList*>& UIManager::GetRoomListMap() const
{
	return m_mapRoomList;
}

const vector<string>& UIManager::GetChatHistory() const
{
	return m_vStrChattingText;
}

char * UIManager::GetEditText(EDIT_TYPE type)
{
	char roomName[ROOM_NAMESIZE];
	GetWindowText(m_hEdit[type], roomName, ROOM_NAMESIZE);

	return roomName;
}


void UIManager::InitUIObjects()
{
	RECT rcRect;
	for (auto iter = m_mapUIObjects.begin(); iter != m_mapUIObjects.end(); iter++)
	{
		SetRect(&rcRect, 2000, 0, 0, 0);
		iter->second->SetLiveObject(false);
		iter->second->SetPosition(rcRect);
	}

	GameManager::GetInstance()->SetPlayerLive(false);

	// JoinRoomUser List ÃÊ±âÈ­ ??

}

void UIManager::SettingSelectCharacterUI()
{
	RECT rcRect;

	SetRect(&rcRect, 0, 0, 0, 0);
	m_mapUIObjects[UI_TYPE_SELECT_CHARACTER]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_SELECT_CHARACTER]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_SELECT_CHARACTER]->SetPosition(rcRect);

	SetRect(&rcRect, 93, 79, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_JOIN]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_JOIN]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_JOIN]->SetPosition(rcRect);

	SetRect(&rcRect, 6, 79, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_EXIT]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_EXIT]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_EXIT]->SetPosition(rcRect);

	SetRect(&rcRect, 490, 480, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_BANANA]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_BANANA]->SetTypeName("¹Ù³ª³ª·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_BANANA]->SetPosition(rcRect);


	SetRect(&rcRect, 440, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_BEANCAKE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_BEANCAKE]->SetTypeName("µÎºÎ·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_BEANCAKE]->SetPosition(rcRect);

	SetRect(&rcRect, 750, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_DONG]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_DONG]->SetTypeName("¶Ë±º·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_DONG]->SetPosition(rcRect);

	SetRect(&rcRect, 390, 240, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_EGG]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_EGG]->SetTypeName("¿Ï¶õ·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_EGG]->SetPosition(rcRect);

	SetRect(&rcRect, 600, 240, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_EGGHALF]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_EGGHALF]->SetTypeName("¹Ý¶õ·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_EGGHALF]->SetPosition(rcRect);

	SetRect(&rcRect, 270, 480, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_GARLIC]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_GARLIC]->SetTypeName("¸¶´Ã·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_GARLIC]->SetPosition(rcRect);

	SetRect(&rcRect, 700, 480, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_GRAPE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_GRAPE]->SetTypeName("Æ÷µµ·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_GRAPE]->SetPosition(rcRect);

	SetRect(&rcRect, 700, 240, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_KIMCHI]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_KIMCHI]->SetTypeName("±èÄ¡·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_KIMCHI]->SetPosition(rcRect);

	SetRect(&rcRect, 340, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_LETTUCE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_LETTUCE]->SetTypeName("¾ç¾ß·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_LETTUCE]->SetPosition(rcRect);

	SetRect(&rcRect, 380, 480, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_MUSHROOM]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_MUSHROOM]->SetTypeName("¹ö¼¸·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_MUSHROOM]->SetPosition(rcRect);

	SetRect(&rcRect, 280, 240, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_ORANGE]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_ORANGE]->SetTypeName("¿À·»Áö·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_ORANGE]->SetPosition(rcRect);

	SetRect(&rcRect, 550, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_PEPPER]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_PEPPER]->SetTypeName("ÇÇ¹«·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_PEPPER]->SetPosition(rcRect);

	SetRect(&rcRect, 500, 240, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_RADISH]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_RADISH]->SetTypeName("¾ËÅ¸·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_RADISH]->SetPosition(rcRect);

	SetRect(&rcRect, 240, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_RICEROCK]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_RICEROCK]->SetTypeName("ÁÖ¹«·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_RICEROCK]->SetPosition(rcRect);

	SetRect(&rcRect, 650, 360, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_SALAD]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_SALAD]->SetTypeName("»ø·¯·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_SALAD]->SetPosition(rcRect);

	SetRect(&rcRect, 590, 480, 0, 0);
	m_mapUIObjects[UI_TYPE_CHARACTER_TOMATO]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_CHARACTER_TOMATO]->SetTypeName("Åä¸¶Åä·î");
	m_mapUIObjects[UI_TYPE_CHARACTER_TOMATO]->SetPosition(rcRect);

	GameManager::GetInstance()->SetPlayerLive(true);
}

void UIManager::SettingWaittingRoomUI()
{
	RECT rcRect;

	SetRect(&rcRect, 0, 0, 0, 0);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_WAITING_ROOM]->SetPosition(rcRect);

	SetRect(&rcRect, 696, 514, 0, 0);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INFO]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INFO]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INFO]->SetPosition(rcRect);

	SetRect(&rcRect, 311, 29, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATEROOM]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_CREATEROOM]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_CREATEROOM]->SetPosition(rcRect);

	SetRect(&rcRect, 225, 29, 0, 0);
	m_mapUIObjects[UI_TYPE_BUTTON_JOINROOM]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_BUTTON_JOINROOM]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_BUTTON_JOINROOM]->SetPosition(rcRect);

	SetRect(&rcRect, 730, 150, 0, 0);
	m_mapUIObjects[UI_TYPE_USER_LIST]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_USER_LIST]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_USER_LIST]->SetPosition(rcRect);

	SetRect(&rcRect, 60, 150, 0, 0);
	m_mapUIObjects[UI_TYPE_ROOM_LIST]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_ROOM_LIST]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_ROOM_LIST]->SetPosition(rcRect);


	GameManager::GetInstance()->SetPlayerLive(true);

	ShowWindow(m_hEdit[EDIT_TYPE_WAITING_ROOM_CHATTING], SW_SHOW);
}

void UIManager::SettingWaitingRoomInGameUI()
{
	RECT rcRect;

	HideCreateRoomWindowPop();
	ShowWindow(m_hEdit[EDIT_TYPE_WAITING_ROOM_CHATTING], SW_HIDE);
	ShowWindow(m_hEdit[EDIT_TYPE_WAITING_ROOM_INGAME_CHATTING], SW_SHOW);


	SetRect(&rcRect, 0, 0, 0, 0);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INGAME]->SetLiveObject(true);
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INGAME]->SetTypeName("");
	m_mapUIObjects[UI_TYPE_WAITING_ROOM_INGAME]->SetPosition(rcRect);
}

void UIManager::CreateUISelectCharacter()
{
	// SelectCharacter BackGround
	UI* pUI = new UI;
	RECT rcRect;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_SELECT_CHARACTER, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_SELECT_CHARACTER, pUI));

	//=====================================================================================

	// SelectCharacter - Button_JOIN
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_JOIN, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_JOIN, pUI));


	// SelectCharacter - Button_EXIT
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_BUTTON_EXIT, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_BUTTON_EXIT, pUI));


	//========================================================================================

	// SelectCharacter - Characters
	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_BANANA, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_BANANA, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_BEANCAKE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_BEANCAKE, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_DONG, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_DONG, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_EGG, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_EGG, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_EGGHALF, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_EGGHALF, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_GARLIC, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_GARLIC, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_GRAPE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_GRAPE, pUI));

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_KIMCHI, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_KIMCHI, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_LETTUCE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_LETTUCE, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_MUSHROOM, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_MUSHROOM, pUI));


	//pUI = new UI;
	//SetRect(&rcRect, 6, 80, 0, 0);
	//pUI->Init(UI_TYPE_CHARACTER_ONION, ResManager::GetInstance()->GetBitmap(), rcRect);
	// 

	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_ORANGE, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_ORANGE, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_PEPPER, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_PEPPER, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_RADISH, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_RADISH, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_RICEROCK, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_RICEROCK, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_SALAD, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_SALAD, pUI));


	pUI = new UI;
	SetRect(&rcRect, 2000, 0, 0, 0);
	pUI->Init(UI_TYPE_CHARACTER_TOMATO, ResManager::GetInstance()->GetBitmap(), rcRect);
	m_mapUIObjects.insert(make_pair(UI_TYPE_CHARACTER_TOMATO, pUI));

}

UIManager::~UIManager()
{
}
