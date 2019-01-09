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
	UIManager::GetInstance()->Init(hWnd, hInstance, EditSubProc);
	UIManager::GetInstance()->SettingSelectCharacterUI();

	m_strTextChatting = "";

	Bitmap::InitDrawBackBuffer(hdc, 1024, 741);

	m_eState = GAME_STATE_SELECT_CHARACTER;

	// Player Character
	RECT rcRect;
	SetRect(&rcRect, 500, 600, 0, 0);
	m_pPlayer = new Player;
	m_pPlayer->Init(PLAYER_TYPE_CHARACTER_ONION, ResManager::GetInstance()->GetBitmap(), rcRect);

	ReleaseDC(hWnd, hdc);
}

void GameManager::Update()
{

	// ȭ��ó��
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
	short roomNumber = UIManager::GetInstance()->JoinRoom();
	if (roomNumber > 0)
	{
		// ������ �� �������� ����
		TCPManager::GetInstance()->SendJoinRoomToServer(roomNumber);

		m_eState = GAME_STATE_JOIN_ROOM;

		// UI�ʱ�ȭ ��, ���ӹ� UI ����
		UIManager::GetInstance()->InitUIObjects();
		UIManager::GetInstance()->SettingWaitingRoomInGameUI();


		return;
	}

	MessageBox(m_hWnd, "�� ���� ����!", "Error!!", MB_OK);
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

void GameManager::AddUserList(int nIdentifyKey, char * szName, char * szLevel, char * szPosition)
{
	UIManager::GetInstance()->AddUserList(nIdentifyKey, szName, szLevel, szPosition);
}

void GameManager::RemoveUserList(int nIdentifyKey)
{
	UIManager::GetInstance()->RemoveUserList(nIdentifyKey);
}


void GameManager::SetPlayerCharacter(UI_TYPE eType, char* szName)
{
	m_pPlayer->SetPlayerCharacter(eType, szName);
}

void GameManager::SetPlayerInfo(char * szNickName, char* szLevel, char* szPosition)
{
	m_pPlayer->SetPlayerInfo(szNickName, szLevel, szPosition);
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
