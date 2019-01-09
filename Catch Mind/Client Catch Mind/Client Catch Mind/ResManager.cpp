#include "ResManager.h"
#include "Bitmap.h"
#include "Macro.h"

ResManager* ResManager::m_pThis = NULL;


void ResManager::InitButtonImage(HDC hdc)
{

	m_pBitmap[RES_TYPE_BUTTON_JOIN] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_JOIN]->Init(hdc, "Res\\Button_join.bmp");

	m_pBitmap[RES_TYPE_BUTTON_EXIT] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_EXIT]->Init(hdc, "Res\\Button_exit.bmp");

	m_pBitmap[RES_TYPE_BUTTON_CHANGECHENNEL] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_CHANGECHENNEL]->Init(hdc, "Res\\Button_ChangeChennel.bmp");

	m_pBitmap[RES_TYPE_BUTTON_CREATEROOM] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_CREATEROOM]->Init(hdc, "Res\\Button_CreateRoom.bmp");

	m_pBitmap[RES_TYPE_BUTTON_JOINROOM] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_JOINROOM]->Init(hdc, "Res\\Button_JoinRoom.bmp");

	m_pBitmap[RES_TYPE_BUTTON_VISITORINFORMATION] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_VISITORINFORMATION]->Init(hdc, "Res\\Button_VisitorInformation.bmp");

	m_pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_EXCUTE] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_EXCUTE]->Init(hdc, "Res\\Button_CreateRoom_Excute.bmp");

	m_pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_CANCLE] = new Bitmap;
	m_pBitmap[RES_TYPE_BUTTON_CREATE_ROOM_CANCLE]->Init(hdc, "Res\\Button_CreateRoom_Cancle.bmp");
}

void ResManager::InitCharacterImage(HDC hdc)
{
	m_pBitmap[RES_TYPE_CHARACTER_BANANA] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_BANANA]->Init(hdc, "Res\\Character_Banana.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_BEANCAKE] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_BEANCAKE]->Init(hdc, "Res\\Character_BeanCake.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_DONG] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_DONG]->Init(hdc, "Res\\Character_Dong.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_EGG] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_EGG]->Init(hdc, "Res\\Character_Egg.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_EGGHALF] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_EGGHALF]->Init(hdc, "Res\\Character_EggHalf.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_GARLIC] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_GARLIC]->Init(hdc, "Res\\Character_Garlic.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_GRAPE] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_GRAPE]->Init(hdc, "Res\\Character_Grape.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_KIMCHI] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_KIMCHI]->Init(hdc, "Res\\Character_Kimchi.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_LETTUCE] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_LETTUCE]->Init(hdc, "Res\\Character_Lettuce.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_MUSHROOM] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_MUSHROOM]->Init(hdc, "Res\\Character_MushRoom.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_ONION] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_ONION]->Init(hdc, "Res\\Character_Onion.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_ORANGE] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_ORANGE]->Init(hdc, "Res\\Character_Orange.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_PEPPER] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_PEPPER]->Init(hdc, "Res\\Character_Pepper.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_RADISH] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_RADISH]->Init(hdc, "Res\\Character_Radish.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_RICEROCK] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_RICEROCK]->Init(hdc, "Res\\Character_RiceRock.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_SALAD] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_SALAD]->Init(hdc, "Res\\Character_Salad.bmp");

	m_pBitmap[RES_TYPE_CHARACTER_TOMATO] = new Bitmap;
	m_pBitmap[RES_TYPE_CHARACTER_TOMATO]->Init(hdc, "Res\\Character_Tomato.bmp");

}

void ResManager::Init(HDC hdc)
{
	m_pBitmap[RES_TYPE_BACKGROUND] = new Bitmap;
	m_pBitmap[RES_TYPE_BACKGROUND]->Init(hdc, "Res\\UI_BackGround_Title.bmp");

	m_pBitmap[RES_TYPE_WAITING_ROOM] = new Bitmap;
	m_pBitmap[RES_TYPE_WAITING_ROOM]->Init(hdc, "Res\\Waiting_room.bmp");

	m_pBitmap[RES_TYPE_WAITING_ROOM_INFO] = new Bitmap;
	m_pBitmap[RES_TYPE_WAITING_ROOM_INFO]->Init(hdc, "Res\\Waiting_room_Info.bmp");

	m_pBitmap[RES_TYPE_WAITING_ROOM_INGAME] = new Bitmap;
	m_pBitmap[RES_TYPE_WAITING_ROOM_INGAME]->Init(hdc, "Res\\Waiting_room_ingame.bmp");

	m_pBitmap[RES_TYPE_SELECT_CHARACTER] = new Bitmap;
	m_pBitmap[RES_TYPE_SELECT_CHARACTER]->Init(hdc, "Res\\Select_Character.bmp");

	m_pBitmap[RES_TYPE_CREATE_ROOM] = new Bitmap;
	m_pBitmap[RES_TYPE_CREATE_ROOM]->Init(hdc, "Res\\Create_Room.bmp");

	InitButtonImage(hdc);
	InitCharacterImage(hdc);



}

Bitmap ** ResManager::GetBitmap()
{
	return m_pBitmap;
}

Bitmap * ResManager::GetBitmap(RES_TYPE type)
{
	return m_pBitmap[type];
}

void ResManager::Reslease()
{
	for (int i = RES_TYPE_BACKGROUND; i < RES_TYPE_END; i++)
	{
		SAFE_RELEASE(m_pBitmap[i]);
		SAFE_DELETE(m_pBitmap[i]);
	}

	SAFE_DELETE(m_pThis);

}

ResManager::ResManager()
{
}


ResManager::~ResManager()
{
}
