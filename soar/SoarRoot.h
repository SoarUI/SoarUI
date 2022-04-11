#pragma once
#include "../SoarHeader/ISoarRoot.h"
#include "../SoarHeader/singleton_t.h"
#include "SoarBindSheetWnd.h"
#include "SoarMsgCenterMgr.h"
#include "../SoarHeader/CcriticalSection.h"
#include<list>
#include"ISoarSheet.h"
struct WndZindexStruct
{
	INT CurZindex;//0 Ϊ���ⴰ��ռ��
	INT PrevZindex;
};
class CSoarRoot :
	public ISoarRoot,public LEE::Singleton_t<CSoarRoot>
{
public:
	CSoarRoot(ISoarEngine* eng);
	typedef	std::map<CLeeString, ISoarWnd*, CLeeString::FastLessCompare>	LeeWndMap;
	typedef	std::map<CLeeString, WndZindexStruct, CLeeString::FastLessCompare>	LeeWndZMap;
	typedef	std::list<ISoarSheet*>	LEESHEETSTACK;
	typedef	std::map<CLeeString, ISoarSheet*, CLeeString::FastLessCompare>	LEESHEETMAP;
public:
	virtual ~CSoarRoot(void);
	//����ӿ�
	virtual void endPatch(void);
	virtual void beginPatch(void);
	virtual void clearScene(void);
	virtual void beginScene(void);
	virtual void endScene(void);
	virtual void Present(void) ;
	virtual bool SoarUIRenderLoop(float t) ;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	virtual bool isValidateWnd(ISoarWnd* hWnd);
	virtual void RunSoarMessageSytem(void);
	////Z���
	/*virtual INT setWndZIndex(ISoarWnd* Wnd,INT dwZindex);
	virtual INT setWndpopupZIndex(ISoarWnd* Wnd,INT dwZindex);*/
	virtual ISoarWnd* setFocusWnd(ISoarWnd* pFocus);
	virtual ISoarWnd* getFocusWnd(void){return d_theActivateSheet->getFocusWnd();}
	virtual void NotifyChildZIndexChanged(ISoarWnd* ChildWnd,int newFocusZpos,int oldZindex);
	//���ӵ�ϵͳ���ڣ���ζ�Ÿô���������Ϣ����Soarϵͳ�ӹ�
	virtual bool attachMsWindow(HWND hMainWnd) ;
	//����UI����
	virtual bool CreateSoarUI(HWND hMainWnd,Dimension& x,Dimension& y,Dimension& w,Dimension& h);
	virtual bool CreateSoarUI(HWND hMainWnd,int x,int y,int W ,int H);
	virtual void setMainSheetTitle(CLeeString title);
	virtual HWND getMsWindow(void)  ;
	virtual ISoarWnd* getSoarSheet(void){return d_bindingSheet;}
	virtual ISoarSheet* getCurrentActiveSheet(){return d_theActivateSheet;}
	/*Sheet�ຯ��*/
	//��ȡ�����ڵĴ�С�����ͻ������С
	virtual RECT getWindowRect(void)  ;
	//��ȡ�����ڵĴ�С�����ͻ������С
	virtual RECT getClientRect(void)  ;
	virtual ISoarWnd* getWnd(const CLeeString &windowName)  ;
	virtual ISoarWnd* createWnd(const CLeeString & name,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD) ;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,LWNDT classType =LWNDT_WND,ISoarWnd* Parent=NULL,
		INT popupZindex =1,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT popupZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD) ;
	virtual bool destroyWnd(ISoarWnd * destroyWnd);
	virtual bool eraseWnd(ISoarWnd * eraseWnd) ;
	//ͳһ����
	virtual ISoarWnd* createWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle=LWNDSTY_CHILD);
	//����ӿڣ����������Զ������
	virtual ISoarWnd* attachCustomClassWnd(ISoarWnd* Wnd,ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP) ;
	/************************************************************************/
	/* ��Ϣϵͳ��                                                            */
	/************************************************************************/
	virtual bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner);
	virtual bool UnRegister(ISoarWnd* pOwner,UINT uMsg);
	virtual bool addOfflineMsg(SOARMSG &msg);
	virtual bool RunSoarMsg(MSG &msg,LRESULT& lr);
	//Ĭ�϶��㴦��(�����)
	virtual bool RunTopSheetMsg(MSG &msg,LRESULT& lr);
	//��ΪUI����������øú���
	virtual BOOL InJectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	INT translateMouseMessage(MSG &msg,bool bNCMsg);
	/************************************************************************/
	/* UIEngine method                                                      */
	/************************************************************************/
	virtual void ChangTheme(CLeeString & szThemeName);
	/*
	*ģ̬�Ի���֧��
	*/
	virtual void StartModal(ISoarWnd* hModelWnd);
	virtual int EndModal();
	virtual int DoModal(ISoarWnd* hMainWnd);
	virtual int DoModal(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop);
	virtual bool CheckIsSoarMainWnd(ISoarWnd* pChkWnd);//
	//2018-2-20
	virtual LRESULT SoarDefWndProc ( UINT ,WPARAM ,LPARAM ) ;
private:
	virtual LRESULT SoarUIMsgProc ( UINT ,WPARAM ,LPARAM ) ;
	//���������ص���Ϣ
	virtual bool filter_MouseMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& );
	virtual bool filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lr);
	ISoarWnd* createWnd(LWNDT classType,LWNDSTY lstyle);
	
protected:
	//�û�����ƽӿ�
	static void CustomDrawSelfT(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt);
	static void CustomDrawSelf(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect);
	static bool CustomEnableAlphaBlend(bool bEnable);
	static bool CustomGraphicGetAlphaBlendState();
protected:
	static LeeWndMap d_Wnds;//�����б�
	static LeeWndZMap d_WndZs;//����˳�����߼�˳��
	//����
	static LeeWndMap d_PopupWnds;//���������б�
	static LeeWndZMap d_PopupWndZs;//��������˳�����߼�˳��
    HWND d_hOwner;
	INT d_zIndex;//����˳��
	INT d_popupZindex;//������������
	//static ISoarWnd* d_focusWnd;//���㴰��
	WNDPROC d_oldWndProc;
	static ISoarEngine * d_DrawEngine;//���ƶ���
	ISoarWnd *d_bindingSheet;//���ڵװ壬�������д��ڵĸ�����
	CSoarMsgCenterMgr d_themsgCenter;
	ISoarSheet*       d_theActivateSheet;//
	CcriticalSection d_WndCcSection;
	CcriticalSection d_PopupWndCcSection;
	CcriticalSection d_WndZCcSection;
	CcriticalSection d_PopupWndZCcSection;
	LEESHEETSTACK d_sheetStack; //����Ҫ��Ⱦ��sheet
	LEESHEETMAP d_sheetlists;//�������е�sheet
	bool d_bEnableAlphaBlend;//�������
};
