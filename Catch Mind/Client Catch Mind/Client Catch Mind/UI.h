#pragma once
#include "Object.h"
#include "Resource.h"
#include "../../Common/defineSize.h"
#include <map>
#include <vector>
#include <string>
using std::vector;
using std::map;
using std::string;




class Bitmap;
class UI : public Object
{
private:
	Bitmap*				m_pBitmap[UI_TYPE_END];
	UI_TYPE				m_eType;
	char				m_szTypeName[NAMESIZE];

	LOGFONT				m_lf;
	HFONT				m_hFont;




public:
	virtual void Init(UI_TYPE eType, Bitmap** pBitmap, RECT rcRect);
	virtual void Draw();

	virtual bool InputMouseLButtonDown(POINT& ptMouse);
	void SetPosition(RECT rcRect);
	void SetTypeName(char* szName);
	
	
	UI_TYPE GetUIType() const
	{
		return m_eType;
	}

	UI();
	virtual ~UI();
};

