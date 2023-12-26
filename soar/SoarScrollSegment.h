#pragma once
#include "soarwnd.h"

class CSoarScrollSegment :
	public ISoarScroll
{
public:
	CSoarScrollSegment(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign,bool bvert =false);
	~CSoarScrollSegment(void);
	virtual void InitializeSegments(void);
	float getPos(void);
	void setPos(float pos);
	DWORD setRange(int nMin,int nMax);
	bool  getRange(int &nMin,int &nMax);
	float  setStep(float stp);
	float  setPage(float page);
	int getPageIndex(void);
	int getPageCount(void);
	virtual BOOL HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr);
	virtual RECT getBarRect(void) ;
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc) ;
	virtual void setArea(ComponentArea& area);
	virtual void getArea(ComponentArea& area);
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
	CLeeString d_wndlook;//��ӦƤ���ؼ�
	CLeeString d_wndlookState;//Ƥ��״̬
	ComponentArea d_area;//����ͳһ������Ϣ
	ISoarEngine *d_DrawEng;//��ͼ���
	LWNDT d_wndtype;//��������
	bool d_bVertbar;
	//�ϡ���
	SoarSegmentItem d_right_or_up;
	//�¡���
	SoarSegmentItem d_left_or_down;
	SOARBARALIGN d_barAlign;//�ڷ�
	CLeeString d_wndText;//��������
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//����״̬
};
