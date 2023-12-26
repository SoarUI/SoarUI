#include "StdAfx.h"
#include "LeeFrameEventHandler.h"
#include "../SoarHeader/ISoarWnd.h"
#include "../SoarHeader/SoarExport.h"
#include "../SoarHeader/ISoar.h"
#include "FrameSampleUsing.h"
extern ISoarRoot * root;
extern CFrameSampleUsing *s;;
CLeeFrameEventHandler::CLeeFrameEventHandler(void)
{
}

CLeeFrameEventHandler::~CLeeFrameEventHandler(void)
{
}
LRESULT CLeeFrameEventHandler::CallbackEvent ( ISoarWnd* pOwner,UINT uMsg ,WPARAM wParam ,LPARAM,LRESULT& lr )
{
	//ֻ��LWNDT::LWNDT_SYS_SHEET���ʹ�����һ�������Ǹ�����
	if (pOwner->getType()==LWNDT::LWNDT_SYS_SHEET && uMsg== SOAR_COMMAND)
	{
		SOARMSG* pMsg=(SOARMSG*)wParam;
		UINT nID =LOWORD(pMsg->wParam);
		if (nID == SOAR_SYSMIN &&
			pMsg->sourceWnd==pOwner &&
			HIWORD(pMsg->wParam)==BN_CLICKED && 
			pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			pOwner->setState(LWNDST_HIDE);
			return false;
		}
		if (nID == SOAR_SYSMAX && 
			pMsg->sourceWnd==pOwner &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			if(pOwner->getState()==LWNDST_MAX)
			{
				pOwner->setState(LWNDST_RESTORE);
			}
			else
				pOwner->setState(LWNDST_MAX);
			lr =0;
			return TRUE;//����true������Soarϵͳ����
		}
		if (nID == SOAR_SYSCLOSE &&
			pMsg->sourceWnd==pOwner &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			if(IDYES == ::MessageBoxA(pOwner->getRootWnd(), "�Ƿ��˳�", "Exit System?", MB_ICONINFORMATION | MB_YESNO))
				return false;//����false����SoarϵͳĬ�ϴ���
			lr =0;
			return true ;
		}
		
	}
	CLeeString itemname;
	itemname.Format("SOARREAM__%p__%s",this,"closebutton");
	CLeeString itemname1;
	itemname1.Format("%s","ProFileDialog");
	ISoarWnd* pBut=root->getWnd(itemname);
	ISoarWnd* pParent=root->getWnd(itemname1);
	if (pOwner==pParent && uMsg== SOAR_COMMAND)
	{
		SOARMSG* pMsg=(SOARMSG*)wParam;
		UINT nID =LOWORD(pMsg->wParam);
		//::MessageBoxA(NULL,"Hit me","Hit me",MB_OK);
		if (nID == SOAR_SYSCLOSE &&
			pMsg->sourceWnd==pOwner &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			//if(IDYES == ::MessageBoxA(NULL,"�Ƿ��˳�","Exit System?",MB_ICONINFORMATION|MB_YESNO) )
			//	return false;//����false����SoarϵͳĬ�ϴ���
			lr =0;
			//root->EndModal();
			return true ;
		}
	}
	//��LWNDT::LWNDT_BUTTON�����¼����нػ�
	if (pOwner->getType()==LWNDT::LWNDT_BUTTON&& uMsg==WM_LBUTTONDOWN&& pOwner==s->d_confirmButton)
	{
		if(IDYES == ::MessageBoxA(NULL,"hit me","Are you Kidding me?",MB_ICONINFORMATION|MB_YESNO) )
			return false;//����false����SoarϵͳĬ�ϴ���
	}
	if (pOwner->getType()==LWNDT::LWNDT_BUTTON&& uMsg==WM_LBUTTONDOWN && pOwner==s->d_EnableAlphaButton)
	{
		pOwner->GetParent()->enableAlphaBlend(true);
	}
	if(uMsg==SOAR_SLIDERCHANGED)
	{  SOARMSG* pMsg=(SOARMSG*)wParam;
		//if(IDYES == ::MessageBoxA(NULL,"hit me","Are you Kidding me?",MB_ICONINFORMATION|MB_YESNO) )
			//return false;//����false����SoarϵͳĬ�ϴ���
	}
	return false;
}