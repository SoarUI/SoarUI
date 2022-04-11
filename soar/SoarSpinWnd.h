#pragma once
#include "Soarwnd.h"

class CLeeSpinWnd :
	public CSoarWnd
{
public:
	CLeeSpinWnd(HWND root,ISoarEngine* Eng);
	~CLeeSpinWnd(void);
	
	virtual DWORD setRange(int nMin,int nMax);
	virtual  bool  getRange(int &nMin,int &nMax);
	virtual void setlinkWnd(ISoarWnd* pTargetWnd);
	virtual void InitializeSegments(void);
	virtual void  DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual LRESULT HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp );
	virtual void checklookState(POINT &pt);
protected:
	INT d_min;
	INT d_max;
	float d_curPos;
	float d_step;
	//ÉÏ¡¢ÓÒ
	SoarSegmentItem d_right_or_up;
	//ÏÂ¡¢×ó
	SoarSegmentItem d_left_or_down;
	ISoarWnd* d_linkwnd;
};
