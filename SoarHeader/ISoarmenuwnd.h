#pragma once
#include "ISoarWnd.h"
/*
����ϵͳ�Ĳ˵�ϵͳ�����볣��˵�����������������ʵ�ַ�ʽ��
*/
class ISoarMenuWnd :
	public ISoarWnd
{
public:
	virtual bool ModifyMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem) =0;
	virtual bool CheckMenuItem(__in UINT uIDCheckItem,__in UINT uCheck)=0;
	virtual bool EnableMenuItem(__in UINT uIDEnableItem,__in LWNDST uEnable)=0;
	virtual ISoarMenuWnd * GetSubMenu(__in int nPos) =0;
	virtual DWORD GetMenuItemID(__in int nPos) =0;
	virtual DWORD GetMenuItemCount(void ) =0 ;
	virtual bool AppendMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem) =0;
	virtual void RemoveMenu(__in UINT uID) =0;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd) =0;
};
