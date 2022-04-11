#pragma once
/*Soar系统Sheet类实现分场景等同于Cocox2d Scene
弹出窗口与普通窗口在意个图层，没有单独分层
在Soar系统中模态对话框之的是一个Sheet，非模态窗口指的是普通窗口，但是它具有显示优先权
*/
#include"ISoarSheet.h"
#include "SoarBindSheetWnd.h"
#include"ISoarLayer.h"
class CSoarSheet:
	public ISoarSheet
{
public:
	CSoarSheet(HWND root,bool bAutoDestroy =false);
	virtual ~CSoarSheet(void);
	typedef	std::map<INT, ISoarLayer*>	LayerMap;
	virtual ISoarWnd* getPrevFocusWnd(void);
	virtual ISoarWnd* setPrevFocusWnd(ISoarWnd* pFocus);
	virtual ISoarWnd* getFocusWnd(void);
	virtual ISoarWnd* setFocusWnd(ISoarWnd* pFocus);

	virtual ISoarWnd* setMainWnd(ISoarWnd* maidW);
	virtual ISoarWnd* getMainWnd(void);
	virtual void setModalState(bool bModal);
	///////////////////////////
	virtual void addWndRecursive(ISoarWnd* newWnd) ;
	virtual BOOL seekWnd(ISoarWnd* newWnd) ;
	virtual bool seekWnd(const CLeeString & name);
	virtual ISoarWnd* getWnd(const CLeeString & name);
	virtual void removeWndRecursive(ISoarWnd* newWnd);
	virtual void eraseWndRecursive(ISoarWnd* pWnd);
	virtual void clearAllLinkWnd() ;
	//窗口层次改变
	virtual BOOL WndZorderChanged(ISoarWnd* FocusWnd,int newFocusZpos,int oldZindex);
	//窗口样式改变
	virtual BOOL WndStyleChanged(ISoarWnd* newWnd);
	virtual BOOL SoarUIMsgProc ( UINT ,WPARAM ,LPARAM,LRESULT& );
	virtual bool filter_ClickMsg(UINT uMessage,WPARAM wParam,LPARAM lParam,LRESULT&);
	virtual bool RenderLoop(ILeeDrawInterface *DrawFuns,float &t);
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	//Z管理
	virtual long generateZindex();
	virtual long generatePopupZindex();
	virtual void setStartZindex(long iStart);
	virtual void setStartPopupZindex(long iStart);
	virtual long getMaxZindex();
	virtual long getMaxPopupZindex();
	virtual long bringWndToTopLevel(ISoarWnd* pFocusWnd);
	virtual long degradeLevelWnd(ISoarWnd* pFocusWnd,long targetlevel);
	//
	
private:
	virtual bool handlerClickMsg(MSG msg);
	//窗口事件
    virtual bool fireUIEvent_COMMAND(float t,MSG msg);
	virtual bool fireUIEvent_Focus(const ISoarWnd * d_prevFocus);
	virtual bool filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual bool filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam);
	virtual bool filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT&);
	//Z 
	virtual long generateTempZindex();
	virtual void modifyTempZindexWnd(ISoarWnd* newWnd,bool isDownlevel) ;
	virtual void modifyTempZindexWndRecursive(ISoarWnd* newWnd,bool isDownlevel);
	virtual  ISoarWnd* FindTopLevelParent(ISoarWnd* newWnd);
	virtual void addWndToZlayer(int zpos,ISoarWnd* pWnd);
	virtual long ensureZindex(int nwntedZindex);
	virtual long ensurePopupZindex(int nwntedZindex);
	bool findWndfast(ISoarWnd* pWnd);
	virtual void addWnd(ISoarWnd* newWnd) ;
	virtual void removeWnd(ISoarWnd* newWnd);
	virtual void eraseWnd(ISoarWnd* pWnd);
protected:
	LayerMap d_layers;
	LayerMap d_popuplayers;//具有优先权处理的窗口;infodlg modeldialog non-modeldialog systemDialog 
	 //-Sheet级别 和系统级别弹出有区别
	HWND d_rootWnd;//原始窗口
	ISoarWnd* d_sheetmainWnd;//该Sheet的底板
	ISoarWnd* d_PrevSheetfocusWnd;//保存该sheet之前的激活窗口
	ISoarWnd* d_SheetfocusWnd;//保存当前sheet激活窗口
	long d_popupzIndexMax;//记录Sheet级弹出窗口的最大ZIndex
	long d_zIndexMax;//记录普通窗口的最大Zindex
	long d_zIndexOld;//记录当前Sheet的窗口激活前的Z位置
	long d_zPopupIndexOld;//记录激活前的Z位置
	long d_baseZindex;//该Sheet的基础Z值(其实是全局的已部分)
	long d_basePopupZindex;//该Sheet的基础Z值(其实是全局的已部分)
	long d_baseTempZindex;//临时index
	long d_baseTemppopupZindex;//临时index
	//全局Z= d_baseZindex+d_zIndexMax;
	bool d_bAutoDestroy;//是否自动销毁
	bool d_bModalState;//该层为modal层，意味着不处理消息不会传至上层
	bool d_bEnableAlphaBlend;//开启混合
};

