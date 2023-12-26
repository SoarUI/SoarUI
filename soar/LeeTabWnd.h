#pragma once
#include <vector>
#include "SoarListbox.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "../SoarHeader/iSoarmenuwnd.h"
#include "LeeTabCtrlHeader.h"
/*
ID:�洢����bututtons  Data����Ŷ�Ӧ�Ĵ��ھ��  
*/
class CLeeTabWnd :
	public CSoarWnd
{
public:
	
	CLeeTabWnd(HWND root,ISoarEngine* Eng);
	~CLeeTabWnd(void);
	virtual void InitializeSegments(void);
	virtual bool addTab(DWORD islot,CLeeString &szTab,LPVOID data);
	virtual bool linkTab(DWORD islot,ISoarWnd* dataWnd);
	virtual int setTabHeight(int h);
	virtual int setTabWidth(int w);
	virtual int setButtonsInoneLine(int n);
	virtual RECT getClientRect(void);
	virtual int getCurSel(void);
	virtual void setCurSel(int islot);
protected:
	CLeeTabCtrlHeader *d_tabHeader;
};
