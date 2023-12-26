#pragma once
#include "soarwnd.h"
#include "../SoarHeader/ISoarmenuwnd.h"
/*SoarUI作为第三方控件嵌入 可以固定在原点，也可以移动*/
class CSoarUISheetWnd :
	public CSoarWnd
{
public:
	CSoarUISheetWnd( HWND root,ISoarEngine* Eng);
	virtual ~CSoarUISheetWnd(void);
	virtual void InitializeSegments(void);
	virtual RECT getClientRect(void) ;//除去标题栏的尺寸
	virtual RECT getWindowRect(void) ;
	virtual bool Bind2Window(HWND h);
	virtual void setTitle( const CLeeString &name);
	BOOL HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo);
protected:
	ISoarMenuWnd* d_menu;
};

