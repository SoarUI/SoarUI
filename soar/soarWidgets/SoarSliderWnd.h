#pragma once
#include "../soarwnd.h"

class CSoarSliderWnd :
	public CSoarWnd
{
public:
	CSoarSliderWnd(HWND root,ISoarEngine* Eng,bool bvert =false);
	~CSoarSliderWnd(void);
	virtual float getPos(void);
	virtual void setPos(float pos);
	virtual DWORD setRange(int nMin,int nMax);
	virtual bool  getRange(int &nMin,int &nMax);
	virtual void InitializeSegments(void);
	virtual void  DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr);
	virtual void checklookState(POINT &pt);
protected:
	INT d_min;
	INT d_max;
	float d_curPos;
	float d_step;
	bool d_bVertbar;
	bool d_lbuttondown;
	//ÉÏ¡¢ÓÒ
	SoarSegmentItem d_right_or_up;
	//ÏÂ¡¢×ó
	SoarSegmentItem d_left_or_down;
	//»¬¿é
	SoarSegmentItem d_thunder;
	
};
