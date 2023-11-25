#pragma once
#include "../SoarHeader/ISoarWnd.h"
#include "../SoarHeader/Dimensions.h"
#include "../SoarHeader/ISoarEngine.h"
/************************************************************************/
/* Soar ϵͳ ���ڻ���                                                */
/************************************************************************/
class CSoarWnd :
	public ISoarWnd
{
public:
	CSoarWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CSoarWnd(void);
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
	virtual void setText(const CLeeString & name);
	virtual void setTitle(const CLeeString & name);
	virtual void setWndName(const CLeeString & name) ;
	virtual CLeeString getWndName(void) ;
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual void setVisibleState(LWNDST state) ; //���� ��ϵͳ����
	virtual LWNDST getVisibleState(void);
	virtual CLeeString getDreamLook(void) ;
	virtual void setDreamLook(const CLeeString & name) ;
	virtual void checklookState(POINT &pt);
	virtual void attachRoot(HWND root);
	virtual LWNDSTY getWndStyle(void) ;
	virtual bool  setWndStyle(LWNDSTY lstyle) ;
	virtual bool  isPopupWnd(void) ;
	virtual void  setWndPopup(bool bPopup) ;
	virtual bool  isModalWnd(void) ;
	virtual void  setWndModal(bool bModal) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	virtual void setAlphaValue(unsigned char value);
	virtual void scrollwnd(int cx,int cy);
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
	virtual ISoarBar* getScrollSegment(bool vert) ;
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
	virtual LeeWndPtrMap& getChidWndSet(){return d_WndPtrs;}
	//������Ⱦ���
	virtual void setVertFormatting(HorizontalTextFormatting hfmt);
    virtual void setHorzFormatting(VerticalTextFormatting vfmt);
private://�������ĺ���Ҫ�����private,����������Ӱ������λ��
	int CalcClientTOP(int & rcTop) ;
	int CalcClientBOTTOM(int & rcBottom) ;
	int CalcClientLEFT(int & rcLeft) ;
	int CalcClientRIGHT(int & rcRight) ;
	void CalcLeft_Right_Bars() ;
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
	LWNDST d_wndVisibleState;//���ڿ��ӻ�״̬(��ϵͳ����������d_wndState һ�� ��ʾ ��һ�� ����)
	LWNDSTY d_wndStyle;//��������
	bool d_isPopupWnd;//���Ϊ��������
	bool d_bModalState;//�ò�Ϊmodal�㣬��ζ�Ų�������Ϣ���ᴫ���ϲ�
	bool d_bAutoDestroy;//�Ƿ��Զ�����
	bool d_bEnableAlphaBlend;//�������
	//�¼��ҽ�
	ISoarEventHandler* p_EventHandler;
	ISoarBar* p_titlebar;
	ISoarBar* p_menubar;
	ISoarBar* p_HScrollSegment;
	ISoarBar* p_VScrollSegment;
	//��Ҫ�Ĳ�������
	int d_scrollSegmentHeight;
	int d_scrollSegmentWidth;
	int d_TitleBarSegmentHeight;
	int d_MenuBarSegmentHeight;
	int d_GroupBoxTitleHeight;
	//���ָ�ʽ
	int d_horzTextFormat;
	int d_VertTextFormat;
	//
	ANYBARS d_nonclientbars_TOP;
	ANYBARS d_nonclientbars_LEFT;
	ANYBARS d_nonclientbars_RIGHT;
	ANYBARS d_nonclientbars_BOTTOM;

};
