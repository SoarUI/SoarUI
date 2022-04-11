#include "SoarNonClientSegmentBase.h"

CSoarNonClientSegmentBase::CSoarNonClientSegmentBase(ISoarWnd *parent,ISoarEngine* Eng):d_DrawEng(Eng),d_OwnerWnd(parent),d_lbuttondown(false)
{
}

CSoarNonClientSegmentBase::~CSoarNonClientSegmentBase(void)
{
}
RECT CSoarNonClientSegmentBase::getClientRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_OwnerWnd->getWindowRect();//获取父窗口出去非客户区的区域
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
RECT CSoarNonClientSegmentBase::getWindowRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_OwnerWnd->getWindowRect();//获取父窗口包括非客户区
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
//获取剪切视图
RECT CSoarNonClientSegmentBase::getCliperRect(void)
{
	if (d_OwnerWnd==NULL)
	{
		RECT rc;
		rc.top=rc.bottom=rc.right=rc.left=0;
		return rc;
	}
	//特殊窗口：弹出窗口
	if (d_OwnerWnd->getZindex()==0)
	{
		return  d_OwnerWnd->getWindowRect(); 
	}
	RECT rcCur =d_OwnerWnd->getWindowRect();
	RectF rccCur =rcCur;
	RECT rcP =d_OwnerWnd->getCliperRect();

	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}