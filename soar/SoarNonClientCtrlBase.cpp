#include "SoarNonClientCtrlBase.h"

CLeeNonClientCtrlBase::CLeeNonClientCtrlBase(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
}

CLeeNonClientCtrlBase::~CLeeNonClientCtrlBase(void)
{
}
RECT CLeeNonClientCtrlBase::getClientRect(void) 
{
	RECT rcParent;
	rcParent =d_parent->getWindowRect();//获取父窗口出去非客户区的区域
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
RECT CLeeNonClientCtrlBase::getWindowRect(void) 
{
	
	RECT rcParent;
	rcParent =d_parent->getWindowRect();//获取父窗口包括非客户区
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
//获取剪切视图
RECT CLeeNonClientCtrlBase::getCliperRect(void)
{
	if (d_parent==NULL)
	{
		RECT rc;
		GetClientRect(d_rootWnd,&rc);
		return rc;
	}
	//特殊窗口：弹出窗口
	if (d_zIndex==0)
	{
		return  d_parent->getWindowRect(); 
	}
	RECT rcCur =d_parent->getWindowRect();
	RectF rccCur =rcCur;
	RECT rcP =d_parent->getCliperRect();

	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}