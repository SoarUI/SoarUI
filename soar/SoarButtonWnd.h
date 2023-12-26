#pragma once
//LWNDT_BUTTON
#include "SoarWnd.h"
#include "../SoarHeader/SoarExport.h"

class CLeeButtonWnd :
	public CSoarWnd
{
public:
	CLeeButtonWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CLeeButtonWnd(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual void checklookState(POINT &pt);
};
