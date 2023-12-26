#pragma once
#include "Soarwnd.h"
#include "SoarpopupListbox.h"
#include "../SoarHeader/SoarExport.h"
class CLeeComboWnd :
	public CSoarWnd
{
public:
	CLeeComboWnd(HWND root,ISoarEngine* Eng);
	~CLeeComboWnd(void);

	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual int addItem(const CLeeString & str,int nID,LPVOID data) ;
	virtual LPCTSTR getText(void);
	virtual void setText(const CLeeString & name);
	virtual DWORD getItemID(int iIndex);
	virtual LPCTSTR getItemString(int iIndex);
	virtual LPVOID getItemData(int iIndex);
	virtual int getSelectedItemIndex(void);
	virtual bool getButtonState(void);
	virtual void setCurSel(int dwIdex) ;
	virtual int getItemCount(void)  ;
	virtual void InitializeSegments(void);
	
protected:
	ISoarWnd* d_downbutton;
	ISoarWnd* d_contentEdit;
	CLeePopupListbox* d_listWnd;
	bool d_bIsopen;
	
};
