#pragma once
#include "soarwnd.h"
#include "../SoarHeader/ISoarmenuwnd.h"
/*SoarUI��Ϊ�������ؼ�Ƕ�� ���Թ̶���ԭ�㣬Ҳ�����ƶ�*/
class CSoarUISheetWnd :
	public CSoarWnd
{
public:
	CSoarUISheetWnd( HWND root,ISoarEngine* Eng);
	virtual ~CSoarUISheetWnd(void);
	virtual void InitializeSegments(void);
	virtual RECT getClientRect(void) ;//��ȥ�������ĳߴ�
	virtual RECT getWindowRect(void) ;
	virtual bool Bind2Window(HWND h);
	virtual void setTitle( const CLeeString &name);
	BOOL HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo);
protected:
	ISoarMenuWnd* d_menu;
};

