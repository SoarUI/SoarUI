#pragma once
#include "leestring.h"
/********************************************************************
    created:    2016/01/26
    created:    2016/01/26::   22:38
    filename:   ISoarRoot.h
    file path:  
    file base:  ISoarRoot
    file ext:   h
    author:     nicklisir/Soar
    copyright:  nicklisir@gmail.com QQ327681967 ecsuperpay@163.com
    purpose:    �ڷ�ϵͳ�����ڣ��������
*********************************************************************/
#include "ISoarWnd.h"
#include <map>
#include "Dimensions.h"
#include "ISoarLoopRunner.h"
class ISoarRoot
{
public:

	ISoarRoot(void){}
	virtual ~ISoarRoot(void){}
	
public:
	//���ӵ�ϵͳ���ڣ���ζ�Ÿô���������Ϣ����Soarϵͳ�ӹ�
	virtual bool attachMsWindow(HWND hMainWnd) = 0;
	virtual bool CreateSoarUI(HWND hMainWnd,int x,int y,int W ,int H) =0;
	virtual bool CreateSoarUI(HWND hMainWnd,Dimension& x,Dimension& y,Dimension& w,Dimension& h)=0;
	virtual void setMainSheetTitle(CLeeString title) =0;
	virtual HWND getMsWindow(void) =0 ;
	virtual ISoarWnd* getSoarSheet(void) = 0;
	//��ȡ���ڵĴ�С���ǿͻ�����ͻ���
	virtual RECT getWindowRect(void) =0 ;
	//��ȡ���ڿͻ����Ĵ�С
	virtual RECT getClientRect(void) =0 ;
	virtual ISoarWnd* getWnd(const CLeeString &windowName) =0 ;
	virtual ISoarWnd* createWnd(const CLeeString & name,LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) =0;
	virtual ISoarWnd* createWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)=0;
	//��������
	virtual ISoarWnd* createWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,
		LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)=0;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,LWNDT classType ,ISoarWnd* Parent,
		INT popupZindex,LWNDSTY dwstyle)=0;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType ,
		ISoarWnd* Parent,INT popupZindex ,LWNDSTY dwstyle) =0 ;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,
		LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) =0;
	//�������Ƴ�����
	virtual bool destroyWnd(ISoarWnd * destroyWnd) =0 ;
	virtual bool eraseWnd(ISoarWnd * eraseWnd) =0 ;
	//����ӿڣ����������Զ������
	virtual ISoarWnd* attachCustomClassWnd(ISoarWnd* Wnd,ISoarWnd* Parent ,INT dwZindex )=0 ;
	
	//----Engine_UI---start---
	virtual void ChangTheme(CLeeString & szThemeName)= 0;
	//UI����
	virtual void endPatch(void)=0;
	virtual void beginPatch(void)=0;
	virtual void clearScene(void)=0;
	virtual void beginScene(void)=0;
	virtual void endScene(void)=0;
	virtual void Present(void)=0 ;
	virtual bool SoarUIRenderLoop(float t) =0;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable)=0;
	virtual bool isValidateWnd(ISoarWnd* hWnd) =0;
	//ִ����Ϣ
	virtual void RunSoarMessageSytem(void) =0;
	//��ʼ����������״̬
	virtual void StartModal(ISoarWnd* hModelWnd) =0;
	virtual int EndModal()=0;
	//��ʾģ̬����--���Ƿϳ�2021-12-11 4��28
	virtual int DoModal(ISoarWnd* hMainWnd) =0;
	virtual int DoModal(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop) =0;
	virtual bool CheckIsSoarMainWnd(ISoarWnd* pChkWnd)=0;//
	//----Engine_UI---End------
	/************************************************************************/
	/* ��Ϣϵͳ��                                                            */
	/************************************************************************/
	virtual LRESULT SoarUIMsgProc ( UINT ,WPARAM ,LPARAM )=0 ;
	virtual bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner) =0 ;
	virtual bool UnRegister(ISoarWnd* pOwner,UINT uMsg) =0;
	//���������Ϣ�൱��MS��postMessage
	virtual bool addOfflineMsg(SOARMSG &msg) =0;
	//----2018-3-24
	///ע����Ϣ��Soarϵͳ�������Ϣ�ͼ�����Ϣ
	virtual BOOL InJectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

};
