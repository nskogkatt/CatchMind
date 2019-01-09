#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include <string>
#include "Resource.h"
#include "../../Common/defineSize.h"

using std::vector;
using std::map;
using std::make_pair;
using std::string;



class UI;
class UIRoomList;
class UIManager
{
	static UIManager*		m_pThis;
	UIManager();

	HWND					m_hWnd;
	HWND					m_hEdit[EDIT_TYPE_END];
	HINSTANCE				m_hInstance;
	WNDPROC					m_hOldEdit[EDIT_TYPE_END];
	LONG					m_lEditSubProc;

	map<UI_TYPE, UI*>		m_mapUIObjects;
	map<int, UserInfo*>		m_mapUserList;
	map<int, UIRoomList*>	m_mapRoomList;
	map<int, UIRoomList*>::iterator	m_prevSelectRoomList;


	vector<string>			m_vStrChattingText;

	void CreateUISelectCharacter();
	void CreateUIWaitingRoom();
	void CreateUIWaitingRoomInGame();

public:
	static UIManager* GetInstance()
	{
		if (m_pThis == NULL)
			m_pThis = new UIManager;
		return m_pThis;
	}

	void Init(HWND hWnd, HINSTANCE hInstance, LONG EditSubProc);
	void Draw();
	void Release();

	void InitUIObjects();
	void SettingSelectCharacterUI();
	void SettingWaittingRoomUI();
	void SettingWaitingRoomInGameUI();

	// �� ����â
	void ShowCreateRoomWindowPop();
	void HideCreateRoomWindowPop();

	// �� Join
	short JoinRoom();

	void AddUserList(int nIdentifyKey, char* szNickName, char* szLevel, char* szPosition);
	void RemoveUserList(int nIdentifyKey);
	
	void AddRoomList(int nRoomNumber, char* roomName, int roomSize, char* superVisorName);
	void RemoveRoomList(int nRoomNumber);

	void AddChattingText(string str);

	HWND GetEditHandle(EDIT_TYPE type);
	WNDPROC GetEditOldHandle(EDIT_TYPE type);
	const map<int, UserInfo*>& GetUserListMap() const;
	const map<int, UIRoomList*>& GetRoomListMap() const;
	const vector<string>& GetChatHistory() const;
	char*	GetEditText(EDIT_TYPE type);

	bool InputMouseLButtonDown(POINT& ptMouse);

	bool IsSelectedRoomList(short roomNumber, bool isSelected);

	
	~UIManager();
};

