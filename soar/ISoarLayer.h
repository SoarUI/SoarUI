#pragma once
#include "../SoarHeader/leestring.h"
struct ILeeDrawInterface;
class ISoarWnd;
class ISoarLayer
{
public:
	ISoarLayer(void){}
	virtual ~ISoarLayer(void){}
	virtual bool seekWnd(ISoarWnd* newWnd) =0;
	virtual bool seekWnd(const CLeeString & name)=0;
	virtual void addWnd(ISoarWnd* newWnd) =0;
	virtual void removeWnd(ISoarWnd* newWnd) =0;
	virtual void eraseWnd(ISoarWnd* pWnd)=0;
	virtual ISoarWnd* getWnd(const CLeeString & name)=0;
	virtual void eraseAllWnd(void) =0 ;
	virtual bool handlerClickMsg(MSG msg,POINT &pt) =0 ;
	virtual bool filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam)=0 ;
	virtual bool filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam)=0 ;
	virtual bool RenderLoop(ILeeDrawInterface *DrawFuns,float &t)=0;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable)=0;
};

