#include "LeePopupBase.h"

CLeePopupBase::CLeePopupBase(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{

}

CLeePopupBase::~CLeePopupBase(void)
{
}
RECT CLeePopupBase::getWindowRect(void) 
{
	RECT rcTmp;
	
	if (d_parent ==NULL)
	{
		GetClientRect(d_rootWnd,&rcTmp);
		RectF rc=rcTmp;
		rc=d_area.getRect(rcTmp);
		RECT rt;
		rc.toWindowRect(rt);
		return rt;
	}
	RECT rcParent =d_parent->getWindowRect();
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	return rc;
}
RECT CLeePopupBase::getCliperRect(void)
{
	if (d_parent==NULL)
	{
		RECT rc;
		GetClientRect(d_rootWnd,&rc);
		return rc;
	}
	RECT rcCur =getWindowRect();//����ĵط�(��Ϊ�����ڴ���������)
	RectF rccCur =rcCur;
	RECT rcP;//ȫ��
	GetClientRect(d_rootWnd,&rcP);//����ȫ����

	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}
void CLeePopupBase::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//�����ڻ���
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc," ",-1,-1,&rcparent,true);//���ü��й���
	}
}
