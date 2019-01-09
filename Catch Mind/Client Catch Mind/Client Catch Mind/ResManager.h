#pragma once
#include <Windows.h>
#include "Resource.h"



class Bitmap;
class ResManager
{
private:
	static ResManager*			m_pThis;
	ResManager();


	Bitmap*			m_pBitmap[RES_TYPE_END];


	void InitButtonImage(HDC hdc);
	void InitCharacterImage(HDC hdc);
public:
	static ResManager* GetInstance()
	{
		if (m_pThis == NULL)
			m_pThis = new ResManager;
		return m_pThis;
	}

	void		Init(HDC hdc);
	void		Reslease();

	Bitmap**	GetBitmap();
	Bitmap*		GetBitmap(RES_TYPE type);


	~ResManager();
};

