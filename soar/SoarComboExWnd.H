#pragma once
#include "SoarWnd.h"
#include "soarpopupListbox.h"
#include "../SoarHeader/SoarExport.h"
#include "LeePopupListViewWnd.h"
class CLeeComboExWnd :
	public CSoarWnd
{
public:
	CLeeComboExWnd(HWND root,ISoarEngine* Eng);
	~CLeeComboExWnd(void);
	virtual void InitializeSegments(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual int addItem(const CLeeString & str,int nID,LPVOID data) ;
	int IsGBK(LPTSTR str);
	virtual LPCTSTR getText(void);
	virtual void setText(const CLeeString & name);
	virtual DWORD getItemID(int iIndex);
	virtual LPCTSTR getItemString(int iIndex);
	virtual LPVOID getItemData(int iIndex);
	virtual int getSelectedItemIndex(void);
	virtual void setCurSel(int islot) ;
	virtual bool getButtonState(void);
	virtual int getItemCount(void) ;
protected:
	ISoarWnd* d_downbutton;
	ISoarWnd* d_contentEdit;
	CLeePopupListViewWnd* d_listWnd;
	bool d_bIsopen;
	
};
