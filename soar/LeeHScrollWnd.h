#pragma once
#include "SoarExport.h"

class CLeeHScrollWnd :
	public ISoarScrollWnd 
{
public:
	CLeeHScrollWnd(HWND root,ISoarEngine* Eng);
	~CLeeHScrollWnd(void);
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
protected:
	INT d_min;
	INT d_max;
	float d_curPos;
	float d_step;
	float d_Curpage;
	int d_cntPage;
	ISoarWnd *p_Leftbutton;
	ISoarWnd *p_Rightbutton;
};
