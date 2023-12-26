#pragma once
#include "../SoarHeader/iSoarmenuwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include"../SoarHeader/ISoarEngine.h"
class CSoarMenuBar :
	public ISoarBar
{
public:
	CSoarMenuBar(ISoarWnd *parent,ISoarEngine* Eng);
	~CSoarMenuBar(void);
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual bool removeItem(const DWORD dId);
	virtual int getItemCount(void);
	bool AppendMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem);
	virtual bool ModifyMenuString(__in UINT nID,__in_opt LPCSTR lpNewItem);
	virtual bool ModifyMenuData(__in UINT nID,__in void* uData);
	virtual bool CheckItem(__in UINT uIDCheckItem,__in UINT uCheck);
	virtual bool EnableItem(__in UINT uIDEnableItem,__in LWNDST uEnable);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual RECT getBarRect(void);
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc);
	virtual void setArea(ComponentArea& area);
	virtual ComponentArea getArea();
	
	
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void);
	virtual SOARBARALIGN getBarAlign(void);
	virtual void setTitle(const CLeeString& name);
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	BOOL HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr) ;
protected:
	LWNDT d_wndtype;//窗口类型
	SEGMENTITEMS d_Items;
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	ComponentArea d_area;//保存统一坐标信息
	ISoarEngine *d_DrawEng;//作图组件
	ISoarWnd* d_OwnerWnd;//消息处理窗口
	int d_iIndex;
	int d_nItemHeight;
	int d_nItemWidth;
	SOARBARALIGN d_barAlign;//摆放
	CLeeString d_wndText;//窗口内容
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//窗口状态
};
