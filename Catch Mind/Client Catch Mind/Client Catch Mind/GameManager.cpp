#include "GameManager.h"
#include "TCPManager.h"
#include "ResManager.h"
#include "UIManager.h"
#include "Bitmap.h"
#include "Object.h"
#include "UI.h"
#include "Player.h"


GameManager* GameManager::m_pThis = NULL;

GameManager::GameManager()
{

}

void GameManager::Rendering()
{
	HDC hdc = GetDC(m_hWnd);

	UIManager::GetInstance()->Draw();
	for (auto iter = m_vObject.begin(); iter != m_vObject.end(); iter++)
	{
		(*iter)->Draw();
	}

	m_pPlayer->Draw();

	Bitmap::DrawBackBuffer(hdc);

	ReleaseDC(m_hWnd, hdc);
}


void GameManager::Init(HWND hWnd, HINSTANCE hInstance, LONG EditSubProc)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	HDC hdc = GetDC(hWnd);

	ResManager::GetInstance()->Init(hdc);


	m_strTextChatting = "";

	Bitmap::InitDrawBackBuffer(hdc, 1024, 741);

	m_eState = GAME_STATE_SELECT_CHARACTER;

	// Player Character
	RECT rcRect;
	SetRect(&rcRect, 500, 600, 0, 0);
	m_pPlayer = new Player;
	m_pPlayer->Init(PLAYER_TYPE_CHARACTER_ONION, ResManager::GetInstance()->GetBitmap(), rcRect);

	//UI 초기화 및 세팅
	UIManager::GetInstance()->Init(hWnd, hInstance, EditSubProc);
	UIManager::GetInstance()->SettingSelectCharacterUI();

	ReleaseDC(hWnd, hdc);
}

void GameManager::Update()
{

	// 화면처리
	Rendering();
}

void GameManager::Release()
{
	delete m_pPlayer;
	m_pPlayer = NULL;

	delete m_pThis;
	m_pThis = NULL;


	Bitmap::ReleaseBackBuffer();
}

void GameManager::InputMouseLButtonDown(POINT& ptMouse)
{
	if (UIManager::GetInstance()->InputMouseLButtonDown(ptMouse))
		return;

	for (auto iter = m_vObject.begin(); iter != m_vObject.end(); iter++)
	{
		if ((*iter)->InputMouseLButtonDown(ptMouse))
			return;
	}
}

void GameManager::JoinWaittingRoom()
{
	RECT rcRect;
	m_eState = GAME_STATE_WAITING_ROOM;
	SetRect(&rcRect, 750, 555, 0, 0);
	m_pPlayer->SetPositionPlayer(rcRect);

	UIManager::GetInstance()->InitUIObjects();
	UIManager::GetInstance()->SettingWaittingRoomUI();

	TCPManager::GetInstance()->SendModifyPlayerTypeToServer(m_pPlayer->GetPlayerType());
}

void GameManager::JoinGameRoom()
{
	short roomNumber = UIManager::GetInstance()->GetSelectedRoomNumber();
	short headCount = UIManager::GetInstance()->GetSelectedHeadCount();
	if (roomNumber > 0 && headCount < 8)
	{
		// 서버로 방 접속정보 보냄
		TCPManager::GetInstance()->SendJoinRoomToServer(roomNumber);
		
		return;
	}

	if (headCount == ROOM_MAX_SIZE)
		MessageBox(m_hWnd, "방이 꽉찼습니다.", "Error!!", MB_OK);
	else
		MessageBox(m_hWnd, "방이 선택되지 않았습니다.", "Error!!", MB_OK);
}

void GameManager::FeedBackJoinRoom(bool bIsSucces)
{
	if (bIsSucces)
	{
		m_eState = GAME_STATE_JOIN_ROOM;

		// UI초기화 후, 게임방 UI 셋팅
		UIManager::GetInstance()->InitUIObjects();
		UIManager::GetInstance()->SettingWaitingRoomInGameUI();
	}
	else
	{
		MessageBox(m_hWnd, "방이 없습니다.", "Error!!", MB_OK);
	}

	
}



void GameManager::ExecuteCreateRoom()
{
	char roomName[ROOM_NAMESIZE] = "";
	strcpy(roomName, UIManager::GetInstance()->GetEditText(EDIT_TYPE_CREATE_ROOMNAME));
	TCPManager::GetInstance()->SendCreateRoomToServer(roomName);

	m_eState = GAME_STATE_JOIN_ROOM;

	m_pPlayer->CreateRoom(roomName);



	UIManager::GetInstance()->InitUIObjects();
	UIManager::GetInstance()->SettingWaitingRoomInGameUI();
}



void GameManager::SetPlayerCharacter(UI_TYPE eType, char* szName)
{
	m_pPlayer->SetPlayerCharacter(eType, szName);
}

void GameManager::SetPlayerInfo(char * szNickName, char* szLevel, char* szPosition)
{
	m_pPlayer->SetPlayerInfo(szNickName, szLevel, szPosition);
}

void GameManager::SetPlayerLive(bool bLive)
{
	m_pPlayer->SetLiveObject(bLive);
}

char* GameManager::GetPlayerCharacterName()
{
	return m_pPlayer->GetTypeName();
}

int GameManager::GetPlayerCharacterType()
{
	return m_pPlayer->GetType();
}

char * GameManager::GetPlayerNickName()
{
	return m_pPlayer->GetNickName();
}

HWND GameManager::GetEditHandleUI(EDIT_TYPE type)
{
	return UIManager::GetInstance()->GetEditHandle(type);
}

WNDPROC GameManager::GetEditOldHandleUI(EDIT_TYPE type)
{
	return UIManager::GetInstance()->GetEditOldHandle(type);
}



void GameManager::AddTextChatting(char * strText)
{
	m_strTextChatting.assign(strText);

	UIManager::GetInstance()->AddChattingText(m_strTextChatting);
}

GameManager::~GameManager()
{
}
