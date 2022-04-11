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
	rcParent =d_OwnerWnd->getWindowRect();//��ȡ�����ڳ�ȥ�ǿͻ���������
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
	rcParent =d_OwnerWnd->getWindowRect();//��ȡ�����ڰ����ǿͻ���
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	RectF rccP =rcCur.Intersection(rcParent);
	rccP.toWindowRect(rc);
	return rc;
}
//��ȡ������ͼ
RECT CSoarNonClientSegmentBase::getCliperRect(void)
{
	if (d_OwnerWnd==NULL)
	{
		RECT rc;
		rc.top=rc.bottom=rc.right=rc.left=0;
		return rc;
	}
	//���ⴰ�ڣ���������
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