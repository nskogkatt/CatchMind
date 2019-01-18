#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Resource.h"
#include "../../Common/defineSize.h"
using std::string;
using std::vector;
using std::map;
using std::make_pair;


enum GAME_STATE
{
	GAME_STATE_SELECT_CHARACTER,		// ĳ���� ����â
	GAME_STATE_WAITING_ROOM,			// ���� - �κ�
	GAME_STATE_JOIN_ROOM,				// ���Ӵ���
	GAME_STATE_START,					// ���� ����
	GAME_STATE_GAMEOVER,				// ���� ��
	GAME_STATE_END
};

class Player;
class Object;
class GameManager
{
	static GameManager*		m_pThis;
	GameManager();

	HWND					m_hWnd;
	HINSTANCE				m_hInstance;

	GAME_STATE				m_eState;

	vector<Object*>			m_vObject;
	Player*					m_pPlayer;

	string					m_strTextChatting;

	void Rendering();
public:
	static GameManager* GetInstance()
	{
		if (m_pThis == NULL)
			m_pThis = new GameManager;
		return m_pThis;
	}

	void Init(HWND hWnd, HINSTANCE hInstance, LONG EditSubProc);
	void Update();
	void Release();

	GAME_STATE GetGameState() const
	{
		return m_eState;
	}

	void InputMouseLButtonDown(POINT& ptMouse);
	void JoinWaittingRoom();				// ���� - �κ�� ����
	void JoinGameRoom();					// ���� -> ���ӹ� ����
	void ExecuteCreateRoom();				// ����� ����� ��ư ����
	void AddUserList(int nIdentifyKey, char* szName, char* szLevel, char* szPosition);
	void RemoveUserList(int nIdentifyKey);

	void SetPlayerCharacter(UI_TYPE eType, char* szName);
	void SetPlayerInfo(char* szNickName, char* szLevel, char* szPosition);
	void SetPlayerLive(bool bLive);

	char* GetPlayerCharacterName();
	int GetPlayerCharacterType();
	char* GetPlayerNickName();
	HWND GetEditHandleUI(EDIT_TYPE type);
	WNDPROC GetEditOldHandleUI(EDIT_TYPE type);

	void AddTextChatting(char* strText);

	~GameManager();
};
