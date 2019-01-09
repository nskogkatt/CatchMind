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
	GAME_STATE_SELECT_CHARACTER,		// 캐릭터 선택창
	GAME_STATE_WAITING_ROOM,			// 대기실 - 로비
	GAME_STATE_JOIN_ROOM,				// 게임대기방
	GAME_STATE_START,					// 게임 시작
	GAME_STATE_GAMEOVER,				// 게임 끝
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
	void JoinWaittingRoom();				// 대기실 - 로비로 들어가기
	void JoinGameRoom();					// 대기실 -> 게임방 들어가기
	void ExecuteCreateRoom();				// 방생성 만들기 버튼 실행
	void AddUserList(int nIdentifyKey, char* szName, char* szLevel, char* szPosition);
	void RemoveUserList(int nIdentifyKey);

	void SetPlayerCharacter(UI_TYPE eType, char* szName);
	void SetPlayerInfo(char* szNickName, char* szLevel, char* szPosition);

	char* GetPlayerCharacterName();
	int GetPlayerCharacterType();
	char* GetPlayerNickName();
	HWND GetEditHandleUI(EDIT_TYPE type);
	WNDPROC GetEditOldHandleUI(EDIT_TYPE type);

	void AddTextChatting(char* strText);

	~GameManager();
};
