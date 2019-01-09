#pragma once
#include "ResManager.h"
#include "../../Common/defineSize.h"

class Bitmap;
class Object
{
protected:
	RECT		m_rcRect;
	bool		m_bLive;

public:
	virtual void Init(int iIndex, Bitmap** pBitmap, RECT rcRect);
	virtual void Draw() = 0;

	virtual bool InputMouseLButtonDown(POINT& ptMouse);

	virtual void SetLiveObject(bool bLive);

	Object();
	virtual ~Object();
};

