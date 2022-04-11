#pragma once
#include "../SoarHeader/ISoarRoot.h"
#include "../SoarHeader/CcriticalSection.h"
#include "../SoarHeader/ISoarEngine.h"
/********************************************************************
    created:    2016/04/15
    created:    2016/04/15::   22:29
    filename:   ILeeRoot.h
    file path:  
    file base:  ILeeRoot
    file ext:   h
    author:     nicklisir/SOAR /FLYDREAM
    copyright:  QQ327681967 ecsuperpay@163.com
    purpose:    系统环境类
*********************************************************************/
class CSoarEnv
{
public:
	CSoarEnv(void);
	typedef std::map< HWND , ISoarRoot* > LeeRootMap ; 
	//Root
	ISoarRoot* getRootFromHandle (HWND hWind) ;
	ISoarRoot* createRoot(HWND h,ISoarEngine* Eng);
	//作为第三方嵌入
	ISoarRoot* createSoarUIRoot(HWND h,ISoarEngine* Eng,int x,int y,int W ,int H);
	ISoarRoot* CreateSoarUI(HWND hMainWnd,ISoarEngine *Eng,Dimension& x,Dimension& y,Dimension& w,Dimension& h);
	void RemoveFromTrackMap ( HWND hWnd ) ;
public:
	virtual ~CSoarEnv(void);
	CcriticalSection d_HwndCcSection;
	 LeeRootMap d_TrackRootWinList; //窗口列表
};
