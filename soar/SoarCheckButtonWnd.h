#pragma once
//LWNDT_BUTTON
#include "SoarWnd.h"
#include "../SoarHeader/SoarExport.h"

class CLeeCheckButtonWnd :
	public CSoarWnd
{
public:
	CLeeCheckButtonWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CLeeCheckButtonWnd(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual bool getCheck(void);
	virtual void setCheck(bool bChecked);
protected:
	bool d_checked;
};
