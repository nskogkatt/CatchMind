#include "Object.h"



void Object::Init(int iIndex, Bitmap** pBitmap, RECT rcRect)
{

}


bool Object::InputMouseLButtonDown(POINT& ptMouse)
{
	if (PtInRect(&m_rcRect, ptMouse))
	{

		return true;
	}

	return false;
}

void Object::SetLiveObject(bool bLive)
{
	m_bLive = bLive;
}

Object::Object()
{
	SetRect(&m_rcRect, 0, 0, 0, 0);
	m_bLive = false;
}


Object::~Object()
{
}
