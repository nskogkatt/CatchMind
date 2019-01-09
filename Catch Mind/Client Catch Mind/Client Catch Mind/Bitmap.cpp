#include "Bitmap.h"

HDC			Bitmap::g_hMemDC = NULL;
HBITMAP		Bitmap::g_hBitmap = NULL;
HBITMAP		Bitmap::g_hOldBitmap = NULL;
SIZE		Bitmap::g_size = { NULL, NULL };

void Bitmap::Init(HDC hdc, const char * szFileName)
{
	m_hMemDC = CreateCompatibleDC(g_hMemDC);
	m_hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	BITMAP bmpinfo;
	GetObject(m_hBitmap, sizeof(bmpinfo), &bmpinfo);
	m_size.cx = bmpinfo.bmWidth;
	m_size.cy = bmpinfo.bmHeight;
}

void Bitmap::Draw(float x, float y)
{
	TransparentBlt(g_hMemDC, (int)x, (int)y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));

}

void Bitmap::Draw(RECT rcRect, RECT rcImage)
{
	TransparentBlt(g_hMemDC, rcRect.left, rcRect.top, rcImage.right, rcImage.bottom, m_hMemDC, rcImage.left, rcImage.top, rcImage.right, rcImage.bottom, RGB(255, 0, 255));
}

void Bitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}

Bitmap::Bitmap()
{
}


Bitmap::~Bitmap()
{
}
