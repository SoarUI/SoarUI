#pragma once
/********************************************************************
    created:    2017/11/30
    created:    2017/11/30::   01:05
    file base:  ISoar
    file ext:   h
    author:     nicklisir/Soar/flydream
    copyright:  QQ327681967 ecsuperpay@163.com
    purpose:    �ڷ�ϵͳ��ʾ����ģ��
*********************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ISoarRoot.h"
#include "ISoarWnd.h"
#include "leestring.h"
#include "../XML/ILeeXmlParser.h"
#include "ISoarLoopRunner.h"
class ISoar
{
public:

	ISoar(void){}
	virtual ~ISoar(void){}
public:
	//ͨ�����еĴ��ڴ���Soarϵͳ
	virtual ISoarRoot* createRootWindow(HWND hmainwindow,const CLeeString& cfgfile) =0;
	//ͨ�����д��ڴ���SoarUI��������(Ƕ��)
	virtual ISoarRoot* createSoarUIRootWindow(HWND hmainwindow,const CLeeString& cfgfile,int x,int y,int W ,int H) =0;
	//���ٴ���Soarϵͳ
	virtual ISoarRoot* createRootWindow(const CLeeString& cfgfile, const CLeeString &szTitle,DWORD style,int x,int y,int W ,int H) =0;
	virtual ISoarRoot* getRootWindow(HWND hmainwindow) =0;
	virtual bool InitEnv(void) =0;
	virtual void UnLoadEnv(void) =0;
	virtual int DoMessgeloop(void) =0;
	virtual int DoMessageUILoop(ISoarLoopRunner* pGameLoop) =0;
	virtual bool CloneXmlParser(ILeeXmlParser** pRet)=0;
	//SoarUI֧��
	virtual void RunSoarMessageSystem(void) =0;
	virtual void RunSoarUIRenderSystem(float t) =0;
	virtual int RunSoarUIModelDialogSystem(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop) =0;
	//��ʼ��Ļ����
	virtual void RenderScene_Begin(void) =0;
	//������Ļ����
	virtual void RenderScene_End(void) =0;
	//��ʼ��������
	virtual void RenderPatch_Begin(void)=0 ;
	//������������
	virtual void RenderPatch_End(void)=0 ;
	virtual bool Render_AlphaBlendEnable(bool bEnable)=0;
	virtual bool Render_GetAlphaBlendState()=0;
	//�����Ļ
	virtual void RenderScene_Clear(void) =0;
	virtual void RenderScene_Present(void) =0;
	virtual void InitRenderEx( LPVOID d3dDevice  )=0;
	virtual void InitRender( HWND hwnd)=0;
};
