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
	rcParent =d_parent->getWindowRect();//��ȡ�����ڳ�ȥ�ǿͻ���������
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
	rcParent =d_parent->getWindowRect();//��ȡ�����ڰ����ǿͻ���
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
//��ȡ������ͼ
RECT CLeeNonClientCtrlBase::getCliperRect(void)
{
	if (d_parent==NULL)
	{
		RECT rc;
		GetClientRect(d_rootWnd,&rc);
		return rc;
	}
	//���ⴰ�ڣ���������
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