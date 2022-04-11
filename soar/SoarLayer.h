#pragma once
#include"ISoarLayer.h"
#include"../SoarHeader/ISoarWnd.h"
/*Soar��ΪSoarWindʵ���������ʵ�ֲַ������ һ��layer����һ���� ������������SoarWind��ϵ�ļ̳й�ϵ*/
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

