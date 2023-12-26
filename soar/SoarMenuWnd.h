#pragma once
#include "../SoarHeader/isoarmenuwnd.h"
#include "SoarWnd.h"
class CLeeMenuWnd :
	public CSoarWnd
{
public:
	CLeeMenuWnd(HWND root,ISoarEngine* Eng);
	~CLeeMenuWnd(void);
	virtual bool ModifyMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem);
	virtual bool CheckMenuItem(__in UINT uIDCheckItem,__in UINT uCheck);
	virtual bool EnableMenuItem(__in UINT uIDEnableItem,__in LWNDST uEnable);
	virtual ISoarMenuWnd * GetSubMenu(__in int nPos) ;
	virtual DWORD GetMenuItemID(__in int nPos) ;
	virtual DWORD GetMenuItemCount(void )  ;
	virtual bool AppendMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem);
	virtual void  RemoveMenu(__in UINT uID);
	//
	virtual  void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual  void setState(LWNDST state) ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_MsgOwnerWnd = pOwnerWnd;}
	virtual ISoarWnd* getOwnerWnd(void){ return d_MsgOwnerWnd;}
	virtual BOOL HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr) ;
protected:
	SEGMENTITEMS d_Items;
	int d_iIndex;
	int d_nItemHeight;
	//ISoarWnd* d_childMenu;//二级菜单
	ISoarWnd* d_MsgOwnerWnd;//消息处理窗口
};
