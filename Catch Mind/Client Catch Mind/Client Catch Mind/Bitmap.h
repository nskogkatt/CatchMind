#pragma once
#include <Windows.h>

class Bitmap
{
private:
	static HDC			g_hMemDC;
	static HBITMAP		g_hBitmap;
	static HBITMAP		g_hOldBitmap;
	static SIZE			g_size;

	HDC					m_hMemDC;
	HBITMAP				m_hBitmap;
	HBITMAP				m_hOldBitmap;
	SIZE				m_size;


public:
	static void InitDrawBackBuffer(HDC hdc, int cx, int cy)
	{
		g_hMemDC = CreateCompatibleDC(hdc);
		g_hBitmap = CreateCompatibleBitmap(hdc, cx, cy);
		g_hOldBitmap = (HBITMAP)SelectObject(g_hMemDC, g_hBitmap);
		g_size.cx = cx;
		g_size.cy = cy;
	}

	static void DrawBackBuffer(HDC hdc)
	{
		BitBlt(hdc, 0, 0, g_size.cx, g_size.cy, g_hMemDC, 0, 0, SRCCOPY);
	}

	static void ReleaseBackBuffer()
	{
		SelectObject(g_hMemDC, g_hOldBitmap);
		DeleteObject(g_hBitmap);
		DeleteDC(g_hMemDC);
	}

	static void DrawTextOut(HFONT hFont, float x, float y, char* szText, COLORREF textColor, COLORREF bkColor, UINT align)
	{
			COLORREF prevColor1, prevColor2;
			prevColor1 = SetTextColor(g_hMemDC, textColor);
			prevColor2 = SetBkColor(g_hMemDC, bkColor);

			SetTextAlign(g_hMemDC, align);
			HFONT oldFont = (HFONT)SelectObject(g_hMemDC, hFont);
			TextOut(g_hMemDC, x, y, szText, strlen(szText) + 1);
			SelectObject(g_hMemDC, oldFont);
			SetTextColor(g_hMemDC, prevColor1);
			SetBkColor(g_hMemDC, prevColor2);
	}

	void Init(HDC hdc, const char* szFileName);
	void Draw(float x, float y);
	void Draw(RECT rcRect, RECT rcImage);

	SIZE GetBitmapSize() const
	{
		return m_size;
	}
	
	void Release();


	Bitmap();
	~Bitmap();
};

