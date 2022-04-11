#pragma once
#include "Soarwnd.h"
#include "SoarEditWnd.h"
#include "../SoarHeader/SoarExport.h"

class CLeeInternetWnd :
	public CSoarWnd
{
public:
	CLeeInternetWnd(HWND root,ISoarEngine* Eng);
	~CLeeInternetWnd(void);
	virtual void InitializeSegments(void);
	//CLeeString getAddress(void);
	DWORD getAddress(void);
	void setAddress(CLeeString addr);
	void setAddress(DWORD addr);
	void setAddress(int prt0,int prt1,int prt2,int prt3);
protected:
	CLeeEditWnd * d_firstpart;
	CLeeEditWnd * d_secondpart;
	CLeeEditWnd * d_thirdpart;
	CLeeEditWnd * d_fouthpart;
};
