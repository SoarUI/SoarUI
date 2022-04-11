#pragma once
#include"ISoarLayer.h"
#include"../SoarHeader/ISoarWnd.h"
/*Soar作为SoarWind实体的容器，实现分层的意义 一个layer代表一个层 ，但它不打破SoarWind体系的继承关系*/
class ISoarSheet;
class CSoarLayer:
	public ISoarLayer
{
public:
	explicit CSoarLayer(ISoarSheet* sheet);
	virtual ~CSoarLayer(void);
	virtual bool seekWnd(ISoarWnd* newWnd) ;
	virtual bool seekWnd(const CLeeString & name);
	virtual void addWnd(ISoarWnd* newWnd) ;
	virtual void removeWnd(ISoarWnd* newWnd);
	virtual void eraseWnd(ISoarWnd* pWnd);
	void eraseAllWnd(void);
	virtual ISoarWnd* getWnd(const CLeeString & name);
	
	//
	virtual bool handlerClickMsg(MSG msg,POINT &pt);
	virtual bool filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual bool filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam);
	virtual bool RenderLoop(ILeeDrawInterface *DrawFuns,float &t);
	bool enableAlphaBlend(bool bEnable);
	ISoarSheet* getSheet(void);
private:
	LeeWndPtrMap d_WndPtrs;
	ISoarSheet* d_pSheet;
	bool d_bEnableAlphaBlend;
};

