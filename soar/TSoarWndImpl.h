#ifndef TSOARWND_IMPL_H
#define TSOARWND_IMPL_H
#include "../SoarHeader/ISoarWnd.h"
#include "../SoarHeader/Dimensions.h"
#include "../SoarHeader/ISoarEngine.h"
template<typename T>
class TSoarWndImpl
{
public:
	TSoarWndImpl(HWND root,ISoarEngine* Eng);
	virtual ~TSoarWndImpl(void);
	///
	virtual void InitializeSegments(void);
	virtual HWND getRootWnd(void) ;
	//��ȡ�����ڵĴ�С�����ͻ������С
	virtual LWNDT getType(void) ;
	virtual RECT getClientRect(void) ;//��ȡ����ڸ����ڵľ���
	virtual RECT getWindowRect(void) ;
	virtual void setArea(RECT& rc);
	virtual void setArea(ComponentArea& area);
	virtual void getArea(ComponentArea& area);
	virtual void setWindowPos(int x,int y);
	virtual void setWindowHeight(int h) ;
	virtual void setWindowWidth(int w) ;
	virtual void setWindowHeight(Dimension h) ;
	virtual void setWindowWidth(Dimension w) ;
	virtual ISoarWnd* GetParent(void);
	virtual RECT getCliperRect(void);
	virtual void setParent(ISoarWnd * lParent) ;
	virtual INT setZindex(INT dwZindex) ;
	virtual INT getZindex(void) ;
	virtual INT setPopupZindex(INT dwZindex) ;
	virtual INT getPopupZindex(void) ;
	virtual CLeeString getText(void);
	virtual void setText(CLeeString name);
	virtual void setTitle(CLeeString name);
	virtual void setWndName(CLeeString name) ;
	virtual CLeeString getWndName(void) ;
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual CLeeString getDreamLook(void) ;
	virtual void setDreamLook(CLeeString name) ;
	virtual void checklookState(POINT &pt);
	virtual void attachRoot(HWND root);
	virtual LWNDSTY getWndStyle(void) ;
	virtual bool  setWndStyle(LWNDSTY lstyle) ;
	virtual bool  isPopupWnd(void) ;
	virtual void  setWndPopup(bool bPopup) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	//
	virtual LRESULT HandleEvent( UINT ,WPARAM ,LPARAM ) ;
	virtual LRESULT HandleEvent ( ISoarWnd* pOwner,UINT ,WPARAM ,LPARAM );
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo);
	virtual ISoarEventHandler* RegisterEventHandler( ISoarEventHandler * pEventHandler) ;
	//���ڻ�ȡ�Ӵ���:���ֻ���������򷵻�
	virtual ISoarWnd* getChildWnd(const CLeeString & name);
	virtual bool addChildWnd(ISoarWnd* pWnd);
	virtual bool eraseChildWnd(ISoarWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	virtual ISoarBar* getMenuBar(void) ;
	virtual ISoarBar* getTitleBar(void) ;
	virtual ISoarScroll* getScrollSegment(bool vert) ;
	virtual int setScrollInfo(int width,int height);
	virtual int getScrollInfo(bool bHeight);
	virtual int setTitleBarSegment(int nHeight);
	virtual int getTitleBarSegment(void);
	virtual int setMenuBarSegment(int nHeight);
	virtual int getMenuBarSegment(void);
	//////////////////////////////////////////////////
	//���㺯��
	virtual RECT CalcClientRec(RECT & rcWnd) ;
	virtual ISoarBar* AddNonClientBar(const CLeeString& elementName,
		SOARBARALIGN align,LWNDT classType,int nHeight_or_Width);
	virtual void DrawNonClientBars(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL HandleNonClientBarsEvent( UINT umsg ,WPARAM wp ,LPARAM lp ,LRESULT &lr);
	protected:
    HWND d_rootWnd;//ԭʼ����
	LeeWndPtrMap d_WndPtrs;//���е��Ӵ���
	ISoarEngine *d_DrawEng;//��ͼ���
	INT d_zIndex; //Z -order
	INT d_zPopupIndex;//��������
	//ͳһ����
	ComponentArea d_area;//����ͳһ������Ϣ
	ISoarWnd* d_parent;//������
	CLeeString d_wndText;//��������
	CLeeString d_wndName;//��������
	CLeeString d_wndlook;//��ӦƤ���ؼ�
	CLeeString d_wndlookState;//Ƥ��״̬
	LWNDT d_wndtype;//��������
	LWNDST d_wndState;//����״̬
	LWNDSTY d_wndStyle;//��������
	bool d_isPopupWnd;//���Ϊ��������
	//�¼��ҽ�
	ISoarEventHandler* p_EventHandler;
	ISoarBar* p_titlebar;
	ISoarBar* p_menubar;
	ISoarScroll* p_HScrollSegment;
	ISoarScroll* p_VScrollSegment;
	//��Ҫ�Ĳ�������
	int d_scrollSegmentHeight;
	int d_scrollSegmentWidth;
	int d_TitleBarSegmentHeight;
	int d_MenuBarSegmentHeight;
	int d_GroupBoxTitleHeight;
	//
	ANYBARS d_nonclientbars;
};

#endif