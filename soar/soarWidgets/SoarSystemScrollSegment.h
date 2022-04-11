#pragma once
#include "../soarwnd.h"

class CSoarSystemScrollSegment :
	public ISoarBar
{
public:
	CSoarSystemScrollSegment(ISoarWnd *parent,ISoarEngine* Eng,bool bvert =false);
	~CSoarSystemScrollSegment(void);
	virtual void InitializeSegments(void);
	float getPos(void);
	void setPos(float pos);
	DWORD setRange(int nMin,int nMax);
	bool  getRange(int &nMin,int &nMax);
	float  setStep(float stp);
	float  setPage(float page);
	int getPageIndex(void);
	int getPageCount(void);
	virtual LRESULT HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp );
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual int getItemCount(void);
	virtual bool removeItem(const DWORD dId);
	virtual RECT getBarRect(void) ;
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc) ;
	virtual void setArea(ComponentArea& area);
	virtual ComponentArea getArea();
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void) ;
	virtual SOARBARALIGN getBarAlign(void);
	virtual void setTitle(CLeeString name);
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
protected:
	INT d_min;
	INT d_max;
	float d_curPos;
	float d_step;
	float d_Curpage;
	int d_cntPage;
	bool d_lbuttondown;
	ISoarWnd* d_OwnerWnd;
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	ComponentArea d_area;//保存统一坐标信息
	ISoarEngine *d_DrawEng;//作图组件
	LWNDT d_wndtype;//窗口类型
	bool d_bVertbar;
	//上、右
	SoarSegmentItem d_right_or_up;
	//下、左
	SoarSegmentItem d_left_or_down;
	//滑块
	SoarSegmentItem d_thunder;
	SOARBARALIGN d_barAlign;//摆放
	CLeeString d_wndText;//窗口内容
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//窗口状态
	SEGMENTITEMS d_items;
};
