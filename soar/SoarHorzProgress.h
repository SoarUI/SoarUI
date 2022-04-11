#pragma once
#include "../SoarHeader/SoarExport.h"
#include "soarwnd.h"
class CLeeHorzProgress :
	public CSoarWnd
{
public:
	CLeeHorzProgress(HWND root,ISoarEngine* Eng);
	~CLeeHorzProgress(void);
	virtual float getPos(void);
	virtual void setPos(float pos);
	virtual DWORD setRange(int nMin,int nMax);
	virtual bool  getRange(int &nMin,int &nMax);
	virtual float  setStep(float stp);
	virtual void  DrawSelf(ILeeDrawInterface *DrawFuns) ;
protected:
	CLeeString d_progressSegment;
	INT d_min;
	INT d_max;
	float d_curPos;
	float d_step;

};
