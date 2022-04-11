#pragma once
#include "../SoarHeader/ISoarmenuwnd.h"
#include "Soarwnd.h"
/*始固定在原点*/
class CSoarBindSheetWnd :
	public CSoarWnd
{
public:
	CSoarBindSheetWnd(HWND root,ISoarEngine* Eng);
	~CSoarBindSheetWnd(void);
	//virtual ILeeWnd* setDefaultSheet(ILeeWnd * defWnd);
	//virtual ILeeWnd* getDefaultSheet(void);
	virtual void InitializeSegments(void);
	virtual RECT getClientRect(void) ;//除去标题栏的尺寸
	virtual RECT getWindowRect(void) ;
	virtual bool Bind2Window(HWND h);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual void MoveWindow(__in int X, __in int Y, __in int nWidth, __in int nHeight, __in BOOL bRepaint);
	virtual void setState(LWNDST state) ;
	virtual void setTitle(const CLeeString &name);
	LRESULT HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam);
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo);
protected:
	ISoarMenuWnd* d_menu;
};
